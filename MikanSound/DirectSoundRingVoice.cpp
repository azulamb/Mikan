#include <process.h>
#include <windows.h>
#include <dsound.h>

#include "debug.h"

#include "IFile.h"
#include "IAudioHandler.h"
#include "IVoice.h"

#include "DirectSoundRingVoice.h"

namespace Thaleia
{
	//--------------------------------------------------
	bool DirectSoundRingVoice::bind( std::unique_ptr<IAudioHandler>&& pcm )
	{
		auto cs = threadLock();

		//エラーチェック
		if ( m_directsound == nullptr )
		{
			LOG( "Internal DirectSound pointer is nullptr." );
			return false;
		}
		if ( m_audio != nullptr )
		{
			LOG( "Already binded." );
			return false;
		}
		if ( pcm == nullptr )
		{
			LOG( "Argument 'pcm' is nullptr." );
			return false;
		}

		//WAVEフォーマット取得
		WAVEFORMATEX format;
		if ( pcm->getWaveFormatEx( &format, sizeof( format ) ) != sizeof( format ) )
		{
			LOG( "Cannot get WaveFormat." );
			return false;
		}

		//バッファサイズ調整
		m_dsbuffer_size = DEFAULT_BUFFER_SIZE;
		if ( m_division < 1 )
			m_division = 2;
		m_dsbuffer_size -= m_dsbuffer_size % ( m_division * format.nBlockAlign );

		//フィルインデックス作成
		m_fill_position = new unsigned long[ m_division ];
		for ( unsigned short i = 0; i < m_division; i++ )
			m_fill_position[ i ] = 0;

		//バッファ初期化
		{
			bool result = m_dsbuffer.initialize(
				m_directsound,
				m_dsbuffer_size,
				&format
				);
			if ( !result )
			{
				LOG( "DirectSoundBuffer initializing failed." );
				return false;
			}
		}

		//PCMの所有権を取得
		m_audio = std::move( pcm );

		return true;
	}

	std::unique_ptr<IAudioHandler> DirectSoundRingVoice::unbind()
	{
		auto cs = threadLock();

		//スレッド解放
		m_dsbuffer.Stop();
		terminateThread( std::move( cs ) );

		//DirectSoundバッファ解放
		m_dsbuffer.release();

		//フィルインデックス削除
		if ( m_fill_position != nullptr )
			delete[] m_fill_position;
		m_fill_position = nullptr;

		//PCMの所有権を返却
		return std::move( m_audio );
	}

	void DirectSoundRingVoice::play()
	{
		auto cs = threadLock();

		if ( m_thread_terminate )
		{
			LOG( "Waiting for termination of streaming thread." );
			return;
		}

		//play中なら停止し, 最初に戻る
		if ( isPlaying() )
		{
			m_dsbuffer.Stop();
			terminateThread( std::move( cs ) );
			m_audio->seek( 0 );
		}

		if ( !threadAlive() ) //stop中
		{
			//スレッド作成
			if ( !threadRun() )
				LOG( "Creating streaming thread failed." );
		} else //pause中
			m_dsbuffer.Play( 0, 0, DSBPLAY_LOOPING );
	}

	void DirectSoundRingVoice::pause()
	{
		m_dsbuffer.Stop();
	}

	void DirectSoundRingVoice::stop()
	{
		auto cs = threadLock();

		//DirectSoundバッファの再生停止
		m_dsbuffer.Stop();

		//スレッド停止要求を発行する
		if ( threadAlive() )
			m_thread_terminate = true;

		//巻き戻し
		if ( m_audio != nullptr )
			m_audio->seek( 0 );
	}

	unsigned long DirectSoundRingVoice::tell() const
	{
		return toMillisecond( tellByte() );
	}

	void DirectSoundRingVoice::seek( unsigned long millisecond )
	{
		auto cs = threadLock();

		if ( m_audio == nullptr )
		{
			LOG( "Not initialized." );
			return;
		}

		//再生中ならば強制停止
		bool playing = isPlaying();
		if ( threadAlive() ) //ストリームスレッドが存在する場合
		{
			if ( playing ) //play中
			{
				m_dsbuffer.Stop();
				terminateThread( std::move( cs ) );
			} else //pause中
			{
				//スレッド停止要求を発行する
				m_thread_terminate = true;
			}
		}

		//移動先の検証
		unsigned long moveto = toByte( millisecond );
		if ( moveto >= m_audio->size() )
			moveto = m_audio->size();

		//移動
		m_audio->seek( moveto );

		//元の再生状態に合わせる
		if ( playing )
			play();
	}

	double DirectSoundRingVoice::getVolume() const
	{
		return m_dsbuffer.getVolume();
	}

	double DirectSoundRingVoice::getPan() const
	{
		return m_dsbuffer.getPan();
	}

	unsigned int DirectSoundRingVoice::getFrequency() const
	{
		return m_dsbuffer.getFrequency();
	}

	bool DirectSoundRingVoice::isLoopEnabled() const
	{
		return m_loop;
	}

	bool DirectSoundRingVoice::getLoopTime( unsigned long* start, unsigned long* end ) const
	{
		if ( start != nullptr )
			*start = m_loop_start;
		if ( end != nullptr )
			*end = m_loop_end;
		return true;
	}

	void DirectSoundRingVoice::setVolume( double volume )
	{
		m_dsbuffer.setVolume( volume );
	}

	void DirectSoundRingVoice::setPan( double pan )
	{
		m_dsbuffer.setPan( pan );
	}

	void DirectSoundRingVoice::setFrequency( unsigned int frequency )
	{
		m_dsbuffer.setFrequency( frequency );
	}

	bool DirectSoundRingVoice::enableLoop( bool enable )
	{
		m_loop = enable;
		return true;
	}

	bool DirectSoundRingVoice::setLoopTime( unsigned long start, unsigned long end )
	{
		m_loop_start = start;
		m_loop_end = end;
		return true;
	}

	bool DirectSoundRingVoice::isPlaying() const
	{
		unsigned long status;
		if ( m_dsbuffer.GetStatus( &status ) != DS_OK )
			return false;

		return status & DSBSTATUS_PLAYING;
	}

	//--------------------------------------------------
	void DirectSoundRingVoice::threadProcedure()
	{
		{
			auto cs = threadLock();

			//エラーチェック
			if ( m_audio == nullptr )
			{
				LOG( "Not initialized." );
				m_dsbuffer.Stop();
				m_thread_terminate = false;
				return;
			}

			unsigned long position = m_audio->tell();

			//最初の読み込み
			m_dsbuffer.SetCurrentPosition( 0 );
			fillBuffer( 0 );
			m_last_index = 0;

			//再生開始
			HRESULT result = m_dsbuffer.Play( 0, 0, DSBPLAY_LOOPING );
			if ( result != S_OK )
			{
				LOG( "DirectSound Play failed. HRESULT=%Xh", result );
				m_thread_terminate = false;
			}
		}

		//バッファ監視ループ
		while ( true )
		{
			auto cs = threadLock();

			if ( m_thread_terminate ) //ループ抜け判定はwhile条件式ではなくlockに成功した直後に行う
				break;

			//再生完了チェック
			if ( tellByte() > m_audio->size() )
			{
				m_dsbuffer.Stop();
				m_audio->seek( 0 );
				break;
			}

			//ブロック位置を取得
			DWORD play_cursor, write_cursor;
			m_dsbuffer.GetCurrentPosition( &play_cursor, &write_cursor );
			unsigned long current_index = play_cursor / ( m_dsbuffer_size / m_division );
			unsigned long next_index = ( current_index + 1 ) % m_division;

			//次のブロックが未読なら読み込む
			if ( m_last_index != next_index )
			{
				fillBuffer( next_index );
				m_last_index = next_index;
			} else
			{
				cs.reset();
				Sleep( 100 );
			}
		}

		//終了処理
		auto cs = threadLock();
		m_thread_terminate = false;
	}

	//--------------------------------------------------

	unsigned long DirectSoundRingVoice::read( void* buffer, unsigned long length )
	{
		//PcmHandler確認
		if ( m_audio == nullptr )
			return 0;

		//ループ位置取得
		bool loop = false;
		unsigned long loop_start, loop_end;
		if ( m_loop )
		{
			if ( m_loop_start < m_loop_end )
			{
				loop_start = toByte( m_loop_start );
				loop_end = toByte( m_loop_end );
				loop = true;
			} else if ( m_loop_start <= m_loop_end )
			{
				loop_start = 0;
				loop_end = m_audio->size();
				loop = true;
			}
		}

		//ループ不要またはループ範囲を抜けているなら普通に読み込み
		if ( !loop || loop_end < m_audio->tell() )
			return m_audio->read( buffer, length );

		//ループを考慮した読み込み
		{
			//フォーマット取得
			WAVEFORMATEX waveformat;
			m_audio->getWaveFormatEx( &waveformat, sizeof( waveformat ) );

			unsigned long remained = length; //読み込むべき残りサイズ
			while ( remained > 0 )
			{
				//現在位置取得
				unsigned long current = m_audio->tell();

				//読み込み要求サイズ計算
				bool loop; //ループフラグ
				unsigned long request; //要求サイズ
				if ( loop_end < current + remained ) //ループ終了地点を跨ぐとき
				{
					loop = true;
					request = loop_end - current; //ループ終了直前まで読み込む
				} else //ループ終了地点を跨がないとき
				{
					loop = false;
					request = remained; //残りサイズを全部読み込む
				}

				//ブロック境界のチェック
				if ( request % waveformat.nBlockAlign != 0 )
				{
					LOG( "block align error." );
					request -= request % waveformat.nBlockAlign;
					if ( request == 0 )
						break;
				}

				//要求サイズ分を読み込む
				unsigned long read_size = m_audio->read( buffer, request );
				buffer = (unsigned char*)buffer + read_size; //書き込みポインタ前進
				remained -= read_size; //残りサイズを減らす

				//要求サイズと実際に読み込めたサイズが違う場合
				if ( read_size != request )
				{
					if ( request >= waveformat.nBlockAlign )
					{
						LOG( "read size[%u] differs form requested size[%u] at %u.", read_size, request, m_audio->tell() );
						return length - remained;
					}
				}

				//必要ならループ開始地点に戻る
				if ( loop )
					m_audio->seek( loop_start );
			}
		}
		return length;
	}

	bool DirectSoundRingVoice::fillBuffer( unsigned int index )
	{
		if ( m_division - 1 < index )
			return false;

		unsigned long block_size = m_dsbuffer_size / m_division;
		LPVOID lpvWrite = 0;
		DWORD dwLength = 0;
		if ( DS_OK == m_dsbuffer.Lock( index*block_size, block_size, &lpvWrite, &dwLength, nullptr, 0, 0 ) )
		{
			//フィルインデックス更新
			m_fill_position[ index ] = m_audio->tell();

			//バッファコピー
			unsigned long read_length = read( lpvWrite, dwLength );

			//コピーされなかった領域をクリア
			if ( dwLength > read_length )
			{
				ZeroMemory( (unsigned char*)lpvWrite + read_length, dwLength - read_length );
				LOG( "%d bytes cleared", dwLength - read_length );
			}

			m_dsbuffer.Unlock( lpvWrite, dwLength, nullptr, 0 );
		}

		return true;
	}

	void DirectSoundRingVoice::terminateThread( Thread::CriticalSection&& cs )
	{
		if ( !threadAlive() )
			return;

		//スレッド終了要求発行
		m_thread_terminate = true;

		//クリティカルセクションが渡されたならば, ロック解除
		bool lock = false;
		if ( cs != nullptr )
		{
			lock = true;
			cs.reset();
		}

		//スレッド終了待ちループ
#ifndef _DEBUG
		while ( threadAlive( 50 ) );
#else
		for ( int i = 0; threadAlive( 50 ); i++ )
			if ( i >= 20 )
			{
				// クリティカルセクション内から, 引数なしでこのメソッドを呼び出すと
				// ここに到達する.
				// (threadProcedureの終了判定がクリティカルセクションを必要とするため.)
				LOG( "Seems dead lock." );
				abort();
			}
#endif

		// 再ロック
		if ( lock )
			cs = threadLock();
	}

	inline unsigned long DirectSoundRingVoice::tellByte() const
	{
		//ループ位置取得
		unsigned long loop_start = toByte( m_loop_start );
		unsigned long loop_end = toByte( m_loop_end );

		//DirectSoundバッファの再生位置を取得
		unsigned long play_cursor, write_cursor;
		HRESULT result = m_dsbuffer.GetCurrentPosition( &play_cursor, &write_cursor );

		//
		int block_index = getCurrentBlockIndex();
		unsigned long fill_position = m_fill_position[ block_index ];
		unsigned long position = fill_position + play_cursor % ( m_dsbuffer_size / m_division );

		//ループ戻り計算
		if ( m_loop )
		{
			if ( loop_start < loop_end && fill_position < loop_end && loop_end <= position )
				position = position % loop_end + loop_start;
			else if ( loop_start == loop_end && m_audio->size() <= position )
				position = position % m_audio->size();
		}

		if ( !SUCCEEDED( result ) )
		{
			LOG( "GetCurrentPosition Failed." );
			return 0;
		}

		//完了
		return position;
	}

	inline int DirectSoundRingVoice::getCurrentBlockIndex() const
	{
		DWORD play_cursor, write_cursor;
		if ( m_dsbuffer.GetCurrentPosition( &play_cursor, &write_cursor ) != DS_OK )
			return 0;

		return play_cursor / ( m_dsbuffer_size / m_division );
	}

};