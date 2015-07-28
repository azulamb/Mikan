#include <windows.h>
#include <dsound.h>
#include "IFile.h"
#include "ResourceFile.h"
#include "debug.h"

namespace Thaleia
{
	//----------------------------------------------------------
	unsigned long ResourceFile::read( void* buffer, unsigned long length )
	{
		DWORD read_size; /// 読み込みに成功したバイト数

		//データの先頭ポインタを持っていなければエラー
		if ( data == NULL )
		{
			LOG( "This instance does not have a resource." );
			return 0;
		}

		//引数がNULLならばエラー
		if ( buffer == NULL )
		{
			LOG( "Argument 'buffer' is nullptr." );
			return 0;
		}

		// データ読み込み
		// という名のポインタ移動とか。
		if ( load + length > filesize )
		{
			length = filesize - load;
		}
		CopyMemory( buffer, (char *)data + load, length );
		read_size = length;
		load += length;

		//読み込んだバイト数を返す
		return read_size;
	}

	unsigned long ResourceFile::size() const
	{
		//データの先頭ポインタを持っていなければエラー
		if ( data == NULL )
		{
			LOG( "This instance does not have a resource." );
			return 0;
		}

		//ファイルサイズを返す
		return filesize;
	}

	unsigned long ResourceFile::tell() const
	{
		//データの先頭アドレスを持っていなければエラー
		if ( data == NULL )
		{
			LOG( "Not initialized." );
			return 0;
		}

		//現在位置を返す
		return load;
	}

	bool ResourceFile::seek( long long offset, bool relatively )
	{
		//データの先頭アドレスを持っていなければエラー
		if ( data == NULL )
		{
			LOG( "Not initialized." );
			return 0;
		}

		//8バイト型のoffsetを4バイト型の2変数にバラす
		long low = offset & 0xFFFFFFFF;	/// offsetの下位4バイト
		long high = offset >> 32;			/// offsetの上位4バイト

		//読み取り位置を変更する
		if ( relatively )
		{
			load += low;
		} else
		{
			load = low;
		}

		//完了
		return true;
	}

	//----------------------------------------------------------
	bool ResourceFile::open( HMODULE module, const char *resource )
	{
		HRSRC hres;

		//既にリソースを読み込んでいるならばエラー
		if ( data != NULL )
		{
			LOG( "This instance already has a file handle." );
			return false;
		}

		//読み取りモードでファイルを開く

		hres = FindResource( module, resource, "SOUND" );

		data = LockResource( LoadResource( module, hres ) );

		//リソースが開けていなければエラー.
		if ( data == NULL )
		{
			LOG( "Cannot load resource '%s'.", resource );
			return false;
		}

		filesize = SizeofResource( module, hres );
		load = 0;

		//完了
		return true;
	}

	void ResourceFile::close()
	{
		//リソースを破棄……できない
		data = NULL;
	}

};
