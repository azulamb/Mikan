#pragma once

#include <memory>
#include <functional>
#include <string>

namespace Thaleia
{

	///
	/// ディスク上のファイルを読み取るクラス.
	/// 
	class ResourceFile : public IFile
	{
		//----------------------------------------------------------
		// IFile

		// public method (override)
	public:
		unsigned long read( void* buffer, unsigned long length );
		unsigned long size() const;
		unsigned long tell() const;
		bool seek( long long offset, bool relatively = false );

		//----------------------------------------------------------
		// DiskFile

		// private variable
	private:
		void* data;
		unsigned long filesize, load;

		// public constructor
	public:
		ResourceFile()
		{
			data = NULL;
		}

		virtual ~ResourceFile()
		{
			if ( data != NULL )
				close();
		}

		// public method
	public:
		/// ファイルを開く
		/// 
		/// file_path	開くファイルのパス
		///
		/// returns		ファイルオープンに成功したならばtrue, さもなくばfalse.
		bool open( HMODULE module, const char *resource );

		/// ファイルを閉じる
		void close();
	};

};