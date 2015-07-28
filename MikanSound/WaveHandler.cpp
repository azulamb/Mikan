#include <string>
#include <windows.h>
#include <dsound.h>

#include "debug.h"

#include "IFile.h"
#include "IAudioHandler.h"
#include "WaveHandler.h"

namespace Thaleia
{
	//--------------------------------------------------
	bool WaveHandler::bind( std::unique_ptr<IFile>&& file )
	{
		//既にファイルをバインドしているならばエラー
		if ( m_file != nullptr )
		{
			LOG( "This instance is already binded." );
			return false;
		}

		//引数がNULLならばエラー
		if ( file == nullptr )
		{
			LOG( "Argument 'file' is nullptr." );
			return false;
		}

		//メンバ変数を更新
		if ( !openWave( file ) )
			return false;

		//ファイルの所有権を取得
		m_file = std::move( file );

		//完了
		return true;
	}

	std::unique_ptr<IFile> WaveHandler::unbind()
	{
		//メンバ変数初期化
		m_datachunk_offset = 0;
		m_datachunk_size = 0;
		ZeroMemory( &m_format, WAVEFORMATEX_SIZE );

		//ファイルの所有権を返却
		return std::move( m_file );
	}

	unsigned long WaveHandler::read( void* buffer, unsigned long length )
	{
		//ファイルとバインドされていなければエラー
		if ( m_file == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//データチャンクの前にいるならデータチャンク先頭にシーク
		if ( m_file->tell() < m_datachunk_offset )
			m_file->seek( m_datachunk_offset );

		//データチャンクの後にいるならデータチャンク末尾にシーク
		if ( m_file->tell() > m_datachunk_offset + m_datachunk_size )
			m_file->seek( m_datachunk_offset + m_datachunk_size );

		//PCM読み込み位置取得
		unsigned long offset = m_file->tell() - m_datachunk_offset;

		//データチャンク末尾に達するときの読み込み量調整
		if ( offset + length > m_datachunk_size )
			length = m_datachunk_size - offset;

		//読み込み量の端数を丸める
		//length -= length % m_waveformat.nBlockAlign;

		//読み込み
		return m_file->read( buffer, length );
	}

	unsigned long WaveHandler::tell() const
	{
		//ファイルとバインドされていなければエラー
		if ( m_file == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//データチャンクの前にいる
		if ( m_file->tell() < m_datachunk_offset )
			return 0;

		//データチャンクの後にいる
		if ( m_file->tell() > m_datachunk_offset + m_datachunk_size )
			return m_datachunk_size;

		//PCMデータ開始地点からの位置を返す
		unsigned long offset = m_file->tell() - m_datachunk_offset;
		return offset;
	}

	void WaveHandler::seek( int offset, bool relatively )
	{
		//ファイルとバインドされていなければエラー
		if ( m_file == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return;
		}

		//絶対位置を取得
		unsigned long absolute_offset = relatively ? tell() + offset : offset;

		//ブロックサイズにアライン
		absolute_offset -= absolute_offset % m_format.nBlockAlign;

		//PCMデータ開始地点からの位置
		unsigned long raw_offset = m_datachunk_offset + absolute_offset;

		//範囲外チェック
		if ( raw_offset > m_datachunk_offset + m_datachunk_size )
			raw_offset = m_datachunk_offset + m_datachunk_size;
		else if ( raw_offset < m_datachunk_offset )
			raw_offset = m_datachunk_offset;

		//移動を実行
		m_file->seek( raw_offset );
	}

	unsigned long WaveHandler::size() const
	{
		//ファイルとバインドされていなければエラー
		if ( m_file == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//データチャンクの大きさを返す
		return m_datachunk_size;
	}

	size_t WaveHandler::getWaveFormatEx( void* format, size_t size ) const
	{
		//拡張サイズを含めたコピーするサイズを得る
		size_t copy_size = sizeof( m_format ) + m_format.cbSize;

		//コピーするサイズの調整
		if ( copy_size > size )
			copy_size = size;
		if ( copy_size > WAVEFORMATEX_SIZE )
			copy_size = WAVEFORMATEX_SIZE;

		//コピーの実行
		memcpy( format, &m_format, copy_size );

		//コピーしたサイズを返す
		return copy_size;
	}

	int WaveHandler::getTagLength() const
	{
		LOG( "Not implemented." );
		return 0;
	}

	std::string WaveHandler::getTag( int index ) const
	{
		LOG( "Not implemented." );
		return std::string();
	}

	//--------------------------------------------------
	bool WaveHandler::openWave( const std::unique_ptr<IFile>& file )
	{
		file->seek( 0 );

		//four-ccを初期化.
		char fourcc[ 5 ];
		fourcc[ 4 ] = 0; //ゼロ終端文字列とする.

		//four-ccがRIFFでなければエラー
		file->read( (void*)fourcc, 4 );
		if ( strcmp( fourcc, "RIFF" ) != 0 )
		{
			LOG( "File is not a RIFF format." );
			return false;
		}

		//RIFFサイズを取得
		DWORD riff_size;
		file->read( (void*)&riff_size, 4 );

		//four-ccがWAVEでなければエラー
		file->read( (void*)fourcc, 4 );
		if ( strcmp( fourcc, "WAVE" ) != 0 )
		{
			LOG( "File is not a WAVE format." );
			return false;
		}

		//フォーマットチャンク検索
		DWORD fmt_size; //フォーマットチャンクの大きさ
		{
			DWORD chunk_size;
			while ( true )
			{
				//four-ccを取得取得
				if ( !file->read( (void*)fourcc, 4 ) )
				{
					LOG( "File structure seems to be broken." );
					return false;
				}

				//チャンクサイズを取得
				if ( !file->read( (void*)&chunk_size, 4 ) )
				{
					LOG( "File structure seems to be broken." );
					return false;
				}

				//フォーマットチャンクならばbreak
				if ( strcmp( fourcc, "fmt " ) == 0 )
					break;

				//フォーマットチャンクでなければ読み飛ばし
				if ( !file->seek( chunk_size, true ) )
					return false;
			}

			//フォーマットチャンクの大きさを取得
			fmt_size = chunk_size;
		}

		//フォーマット取得
		if ( fmt_size > WAVEFORMATEX_SIZE )
		{
			LOG( "Format data is too large." );
			return false;
		}
		file->read( (void*)&m_format, fmt_size );

		//WAVEチャンクの先頭に移動
		if ( !file->seek( 12 ) )
		{
			LOG( "File structure seems to be broken." );
			return false;
		}

		//データチャンク検索
		{
			DWORD chunk_size;
			while ( true )
			{
				//four-ccを取得
				if ( !file->read( (void*)fourcc, 4 ) )
				{
					LOG( "File structure seems to be broken." );
					return false;
				}

				//チャンクサイズを取得
				if ( !file->read( (void*)&chunk_size, 4 ) )
				{
					LOG( "File structure seems to be broken." );
					return false;
				}

				//データチャンクならばbreak
				if ( strcmp( fourcc, "data" ) == 0 )
					break;

				//データチャンクでなければ読み飛ばし
				if ( !file->seek( chunk_size, true ) )
				{
					LOG( "File structure seems to be broken." );
					return false;
				}
			}

			//データチャンクの大きさを取得
			m_datachunk_size = chunk_size;
		}

		//データオフセットを取得
		m_datachunk_offset = file->tell();

		return true;
	}


};