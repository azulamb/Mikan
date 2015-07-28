#pragma once

#include <memory>
#include <functional>
#include <string>

namespace Thaleia
{

	///
	/// ディスク上のファイルを読み取るクラス.
	/// 
	class DiskFile : public IFile
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
		std::unique_ptr<void, std::function<void( HANDLE )>> m_file_handle; /// ファイルハンドル

		// public constructor
	public:
		DiskFile() :
			m_file_handle( nullptr )
		{
		}

		virtual ~DiskFile()
		{
			if ( m_file_handle != nullptr )
				close();
		}

		// public method
	public:
		/// ファイルを開く
		/// 
		/// file_path	開くファイルのパス
		///
		/// returns		ファイルオープンに成功したならばtrue, さもなくばfalse.
		bool open( const std::string& file_path );

		/// ファイルを閉じる
		void close();

		/// 既に開かれているファイルハンドルをこのインスタンスと結びつける.
		/// 以降, ハンドルの所有権はこのインスタンスが保持する.
		/// (勝手に ReadFile や CloseHandle をしてはいけない)
		/// 
		/// file_handle	バインドする
		///
		/// returns		ファイルオープンに成功したならばtrue, さもなくばfalse.
		bool bind( HANDLE file_handle );

		/// open または bind によってこのインスタンスと結びついた
		/// ファイルハンドルの所有権を解放する.
		/// (自前で CloseHandle すること)
		/// 
		/// returns	解放されたファイルハンドル.
		///			インスタンスがファイルハンドルを保持していない場合はnullptr.
		HANDLE unbind();

	};

};