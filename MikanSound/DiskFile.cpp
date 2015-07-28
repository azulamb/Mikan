#include <windows.h>
#include <dsound.h>
#include "IFile.h"
#include "DiskFile.h"
#include "debug.h"

namespace Thaleia
{
	//----------------------------------------------------------
	unsigned long DiskFile::read( void* buffer, unsigned long length )
	{
		//ファイルハンドルを持っていなければエラー
		if ( m_file_handle == nullptr )
		{
			LOG( "This instance does not have a file handle." );
			return 0;
		}

		//引数がNULLならばエラー
		if ( buffer == nullptr )
		{
			LOG( "Argument 'buffer' is nullptr." );
			return 0;
		}

		//ファイル読み込み
		DWORD read_size; /// 読み込みに成功したバイト数
		ReadFile( m_file_handle.get(), buffer, length, &read_size, nullptr );

		//読み込んだバイト数を返す
		return read_size;
	}

	unsigned long DiskFile::size() const
	{
		//ファイルハンドルを持っていなければエラー
		if ( m_file_handle == nullptr )
		{
			LOG( "This instance does not have a file handle." );
			return 0;
		}

		//ファイルサイズ取得
		unsigned long low = GetFileSize( m_file_handle.get(), nullptr );

		//GetFileSizeが失敗したとき
		if ( low == -1 )
		{
			LOG( "Getting file size failed." );
			return 0;
		}

		//ファイルサイズを返す
		return low;
	}

	unsigned long DiskFile::tell() const
	{
		//ファイルハンドルを持っていなければエラー
		if ( m_file_handle == nullptr )
		{
			LOG( "Not initialized." );
			return 0;
		}

		//現在位置取得
		unsigned long low = SetFilePointer( m_file_handle.get(), 0, nullptr, FILE_CURRENT );

		//SetFilePointerが失敗したとき
		if ( low == INVALID_SET_FILE_POINTER )
		{
			LOG( "Getting file cursor failed." );
			return 0;
		}

		//現在位置を返す
		return low;
	}

	bool DiskFile::seek( long long offset, bool relatively )
	{
		//ファイルハンドルを持っていなければエラー
		if ( m_file_handle == nullptr )
		{
			LOG( "Not initialized." );
			return 0;
		}

		//8バイト型のoffsetを4バイト型の2変数にバラす
		long low = offset & 0xFFFFFFFF;	/// offsetの下位4バイト
		long high = offset >> 32;			/// offsetの上位4バイト

		//読み取り位置を変更する
		unsigned long result = SetFilePointer( m_file_handle.get(), low, &high, ( relatively ? FILE_CURRENT : FILE_BEGIN ) );

		//SetFilePointerが失敗したとき
		if ( result == INVALID_SET_FILE_POINTER )
		{
			LOG( "Setting file cursor failed." );
			return false;
		}

		//完了
		return true;
	}

	//----------------------------------------------------------
	bool DiskFile::open( const std::string& file_path )
	{
		//既にファイルハンドルを開いているならばエラー
		if ( m_file_handle != nullptr )
		{
			LOG( "This instance already has a file handle." );
			return false;
		}

		//読み取りモードでファイルを開く
		m_file_handle = decltype( m_file_handle )
			(
				CreateFileA( file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr ),
				[]( HANDLE handle ) //解放時の処理
		{
			CloseHandle( handle );
		}
		);

		//ファイルが開けていなければエラー.
		//CreateFileAの失敗時の戻り値はNULLではないことに注意.
		if ( m_file_handle.get() == INVALID_HANDLE_VALUE )
		{
			LOG( "Cannot open file '%s'.", file_path );
			m_file_handle.release();
			return false;
		}

		//完了
		return true;
	}

	void DiskFile::close()
	{
		//ファイルを破棄
		m_file_handle.reset();
	}

	bool DiskFile::bind( HANDLE file_handle )
	{
		//既にファイルハンドルを開いているならばエラー
		if ( m_file_handle != nullptr )
		{
			LOG( "This instance already has a file handle." );
			return false;
		}

		//引数がNULLならばエラー
		if ( file_handle == nullptr )
		{
			LOG( "Argument 'file_handle' is NULL." );
			return false;
		}

		//引数が無効なハンドルならばエラー
		if ( file_handle == INVALID_HANDLE_VALUE )
		{
			LOG( "Argument 'file_handle' is INVALID_HANDLE_VALUE." );
			return false;
		}

		//unique_ptrに管理を委譲
		m_file_handle = decltype( m_file_handle )
			(
				file_handle,
				[]( HANDLE handle ) //解放時の処理
		{
			CloseHandle( handle );
		}
		);
		return true;
	}

	HANDLE DiskFile::unbind()
	{
		//ハンドルをコピーしておく
		HANDLE file_handle = m_file_handle.get();

		//ハンドルの管理を放棄
		m_file_handle.release();

		//ハンドルを返す
		return file_handle;
	}

};