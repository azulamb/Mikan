#pragma once

#include <memory>
#include <string>

namespace Thaleia
{

	///
	/// ディスク上のファイルをメモリ上に展開し, 
	/// 随時アクセスを高速に行わせるクラス.
	/// 
	class MemoryFile : public IFile
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
		// MemoryFile

		// private variable
	private:
		std::unique_ptr<char[]> m_buffer;	/// ファイル内容を保持するバッファ
		unsigned long m_size;				/// ファイルサイズ
		unsigned long m_read_cursor;		/// 読み取り位置

		// public constructor
	public:
		MemoryFile() :
			m_buffer( nullptr ),
			m_size( 0 ),
			m_read_cursor( 0 )
		{
		}

		~MemoryFile()
		{
			unload();
		}

		// public method
	public:
		/// ファイル内容を読み込み, メモリ上に展開・保持する.
		///
		/// file_path	展開するファイルのパス
		///
		/// returns		読み込みに成功したならばtrue, さもなくばfalse.
		bool load( const std::string& file_path );

		/// ファイル内容を読み込み, メモリ上に展開・保持する.
		/// 引数に渡されたハンドルは CloseHandle されないが,
		/// 読み取り位置は変更される.
		///
		/// file_handle	展開するファイルの, 既に開いているファイルハンドル.
		///
		/// returns		読み込みに成功したならばtrue, さもなくばfalse.
		bool load( HANDLE file_handle );

		/// メモリ上に保持しているファイル内容を解放する.
		void unload();
	};

};