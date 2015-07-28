#include <math.h>
#include <windows.h>
#include <dsound.h>

#include "debug.h"

#include "IFile.h"
#include "IAudioHandler.h"
#include "IVoice.h"

#include "DirectSoundBufferWrapper.h"
#include "DirectSoundVoice.h"

namespace Thaleia
{
	//----------------------------------------------------------
	bool DirectSoundVoice::bind( std::unique_ptr<IAudioHandler>&& audio )
	{
		//既にバインド済みならばエラー
		if ( m_audio != nullptr )
		{
			LOG( "This instance is already binded with PCM." );
			return false;
		}

		//DirectSoundがNULLならばエラー
		if ( m_directsound == nullptr )
		{
			LOG( "Internal DirectSound pointer is nullptr." );
			return false;
		}

		//引数がNULLならばエラー
		if ( audio == nullptr )
		{
			LOG( "Argument 'audio' is nullptr." );
			return false;
		}

		//バッファサイズ取得
		unsigned long buffer_size = audio->size();
		if ( buffer_size == 0 )
		{
			LOG( "PCM size is zero." );
			return false;
		}

		//フォーマット取得
		WAVEFORMATEX format;
		audio->getWaveFormatEx( &format, sizeof( format ) );

		//バッファ初期化
		{
			bool result = m_dsbuffer.initialize(
				m_directsound,
				audio->size(),
				&format
				);
			if ( !result )
			{
				LOG( "DirectSoundBuffer initializing failed." );
				return false;
			}
		}

		//バッファにオーディオを展開
		{
			//オーディオをの読み込み位置を初期化.
			audio->seek( 0 );

			//書きこみ関連の変数を初期化.
			void* write_cursor = nullptr;  ///書きこみアドレス
			unsigned long locked_size = 0; ///書きこみロックしているサイズ(バイト数)

			//書きこみロックを得る.
			{
				HRESULT result = m_dsbuffer.Lock( 0, buffer_size, &write_cursor, &locked_size, nullptr, nullptr, 0 );

				//Lockの結果がDS_OKでなければエラー.
				if ( result != DS_OK )
				{
					LOG( "DirectSoundBuffer cannot be locked." );
					m_dsbuffer.release();
					return false;
				}

				//ロックサイズが要求サイズより小さければエラー.
				if ( locked_size < buffer_size )
				{
					LOG( "locked size is too small." );
					m_dsbuffer.Unlock( write_cursor, locked_size, nullptr, 0 );
					m_dsbuffer.release();
				}
			}

			//DirectSoundバッファにオーディオをデータをコピーする.
			unsigned long copied_size = audio->read( write_cursor, locked_size );

			//書きこみロックを解放する.
			m_dsbuffer.Unlock( write_cursor, locked_size, nullptr, 0 );

			//コピーに成功したサイズが要求サイズより小さいならばエラー.
			if ( copied_size != buffer_size )
			{
				LOG( "PCM reading failed." );
				m_dsbuffer.release();
				return false;
			}
		}

		//オーディオの所有権を取得
		m_audio = std::move( audio );

		//完了
		return true;
	}

	std::unique_ptr<IAudioHandler> DirectSoundVoice::unbind()
	{
		//バッファ開放
		m_dsbuffer.Stop();
		m_dsbuffer.release();

		//設定初期化
		m_loop = false;

		//PCMの所有権を返却
		return std::move( m_audio );
	}

	void DirectSoundVoice::play()
	{
		if ( isPlaying() )
			seek( 0 );
		else
			m_dsbuffer.Play( 0, 0, m_loop ? DSBPLAY_LOOPING : 0 );
	}

	void DirectSoundVoice::pause()
	{
		m_dsbuffer.Stop();
	}

	void DirectSoundVoice::stop()
	{
		m_dsbuffer.Stop();
		m_dsbuffer.SetCurrentPosition( 0 );
	}

	unsigned long DirectSoundVoice::tell() const
	{
		unsigned long play_cursor = 0;
		m_dsbuffer.GetCurrentPosition( &play_cursor, nullptr );

		return toMillisecond( play_cursor );
	}

	void DirectSoundVoice::seek( unsigned long millisecond )
	{
		unsigned long moveto = toByte( millisecond );
		if ( moveto >= m_audio->size() )
			moveto = m_audio->size() - 1; // 終端ピッタリには移動できない. AlignはDirectSoundが調整してくれる.

		m_dsbuffer.SetCurrentPosition( moveto );
	}

	double DirectSoundVoice::getVolume() const
	{
		return m_dsbuffer.getVolume();
	}

	double DirectSoundVoice::getPan() const
	{
		return m_dsbuffer.getPan();
	}

	unsigned int DirectSoundVoice::getFrequency() const
	{
		return m_dsbuffer.getFrequency();
	}

	bool DirectSoundVoice::isLoopEnabled() const
	{
		return m_loop;
	}

	bool DirectSoundVoice::getLoopTime( unsigned long* start, unsigned long* end ) const
	{
		return false;
	}

	void DirectSoundVoice::setVolume( double volume )
	{
		m_dsbuffer.setVolume( volume );
	}

	void DirectSoundVoice::setPan( double pan )
	{
		m_dsbuffer.setPan( pan );
	}

	void DirectSoundVoice::setFrequency( unsigned int frequency )
	{
		m_dsbuffer.setFrequency( frequency );
	}

	bool DirectSoundVoice::enableLoop( bool enable )
	{
		m_loop = enable;
		return true;
	}

	bool DirectSoundVoice::setLoopTime( unsigned long start, unsigned long end )
	{
		return false;
	}

	bool DirectSoundVoice::isPlaying() const
	{
		unsigned long status;
		if ( m_dsbuffer.GetStatus( &status ) != DS_OK )
			return false;

		return status & DSBSTATUS_PLAYING;
	}

};