#include <functional>
#include <stdio.h>
#include <windows.h>
#include <dsound.h>
#include "debug.h"
#include "IFile.h"
#include "MemoryFile.h"

namespace Thaleia
{
	//----------------------------------------------------------
	unsigned long MemoryFile::read( void* buffer, unsigned long length )
	{
		//ファイルの読み込み前ならばエラー
		if ( m_buffer == nullptr )
		{
			LOG( "This instance does not load a file yet." );
			return 0;
		}

		//引数がNULLならばエラー
		if ( buffer == nullptr )
		{
			LOG( "Argument 'buffer' is nullptr." );
			return 0;
		}

		//読み取りサイズ
		unsigned long read_size = length;

		//読み取りサイズがファイル末尾を超えるとき
		if ( m_read_cursor + read_size > m_size )
		{
			//読み取り位置が既に末尾にいるならば, 何も読み取らない
			if ( m_size <= m_read_cursor )
				return 0;

			//読み取りサイズを末尾の位置まで減らす
			read_size = m_size - m_read_cursor;
		}

		//読み取り
		memcpy( buffer, m_buffer.get() + m_read_cursor, read_size );

		//読み取り位置前進
		m_read_cursor += read_size;

		//完了
		return read_size;
	}

	unsigned long MemoryFile::size() const
	{
		//ファイルの読み込み前ならばエラー
		if ( m_buffer == nullptr )
		{
			LOG( "This instance does not load a file yet." );
			return 0;
		}

		//ファイルサイズを返す
		return m_size;
	}

	unsigned long MemoryFile::tell() const
	{
		//ファイルの読み込み前ならばエラー
		if ( m_buffer == nullptr )
		{
			LOG( "This instance does not load a file yet." );
			return 0;
		}

		//読み取り位置を返す
		return m_read_cursor;
	}

	bool MemoryFile::seek( long long offset, bool relatively )
	{
		//ファイルの読み込み前ならばエラー
		if ( m_buffer == nullptr )
		{
			LOG( "This instance does not load a file yet." );
			return 0;
		}

		//移動先を計算する
		long long position; ///移動先
		if ( relatively )
			position = m_read_cursor + offset;
		else
			position = offset;

		//移動先がファイルサイズの範囲外に出ないようにチェック
		if ( position < 0 )
			position = 0;
		else if ( m_size < position )
			position = m_size;

		//移動
		m_read_cursor = (unsigned long)position;

		//完了
		return true;
	}

	//----------------------------------------------------------
	bool MemoryFile::load( const std::string& file_path )
	{
		//読み取りモードでファイルを開く
		auto file_handle = std::unique_ptr<void, std::function<void( HANDLE )>>
			(
				CreateFileA( file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr ),
				[]( HANDLE handle )
		{
			CloseHandle( handle );
		}
		);

		//ファイルが開けていなければエラー.
		//CreateFileAの失敗時の戻り値はNULLではないことに注意.
		if ( file_handle.get() == INVALID_HANDLE_VALUE )
		{
			LOG( "Cannot open file '%s'.", file_path );
			file_handle.release();
			return false;
		}

		//読み込み部分は load(HANDLE file_handle) に丸投げ
		bool result = load( file_handle.get() );

		//完了
		return result;
	}

	bool MemoryFile::load( HANDLE file_handle )
	{
		//既に読み込んだファイルがある場合はエラー
		if ( m_buffer != nullptr )
		{
			LOG( "This instance already has a file buffer." );
			return false;
		}

		//引数がNULLならばエラー
		if ( file_handle == nullptr )
		{
			LOG( "Argument 'file_handle' is NULL." );
			return false;
		}

		//ファイルサイズ取得
		unsigned long size;
		{
			unsigned long low = GetFileSize( file_handle, nullptr );
			if ( low == -1 )
			{
				LOG( "Getting file size error." );
				return false;
			}

			size = low;
		}

		//バッファ確保
		decltype( m_buffer ) buffer;
		try
		{
			buffer.reset( new char[ size ] );
		} catch ( ... )
		{
			LOG( "Memory allocation failed." );
			buffer.release();
			return false;
		}

		//ファイル読み込み
		unsigned long read_size;
		SetFilePointer( file_handle, 0, nullptr, FILE_BEGIN ); //先頭にシーク
		ReadFile( file_handle, buffer.get(), size, &read_size, nullptr );

		//読み込み要求サイズと実際に読み込んだサイズが異なる場合はエラー
		if ( read_size != size )
		{
			LOG( "Reading file failed." );
			return false;
		}

		//完了
		m_buffer = std::move( buffer );
		m_size = size;
		m_read_cursor = 0;
		return true;
	}

	void MemoryFile::unload()
	{
		//バッファを解放する
		m_buffer.reset();

		//変数の初期化
		m_size = 0;
		m_read_cursor = 0;
	}

};