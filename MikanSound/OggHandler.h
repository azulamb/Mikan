#pragma once

#include "IAudioHandler.h"
#include "IFile.h"

#include <functional>
#include "vorbis/vorbisfile.h"

namespace Thaleia
{

	/**
	 *	OGGファイルを扱うクラス
	 */
	class OggHandler : public IAudioHandler
	{
		//----------------------------------------------------------
		// IAudioHandler

		// public method (override)
	public:
		bool bind( std::unique_ptr<IFile>&& file );
		std::unique_ptr<IFile> unbind();
		unsigned long read( void* buffer, unsigned long length );
		unsigned long tell() const;
		void seek( int offset, bool relatively = false );
		unsigned long size() const;
		size_t getWaveFormatEx( void* format, size_t size ) const;
		int getTagLength() const;
		std::string getTag( int index ) const;

		//----------------------------------------------------------
		// OggHandler

		// private variable
	private:
		// ファイルインターフェイスのインスタンス
		std::unique_ptr<IFile> m_file;

		// ogg-vorbisライブラリが利用する形式のファイルハンドル
		std::unique_ptr<OggVorbis_File, std::function<void( OggVorbis_File* )>> m_oggvorbis;

		// public constructor
	public:
		OggHandler()
		{
		}

		virtual ~OggHandler()
		{
			// unbind()するとm_oggvorbis, m_fileの順で破棄されることが保障される.
			// ogg-vorbisライブラリのスレッド挙動が未調査なので, 念のためこの順序を守ること.
			unbind();
		}

		// public static method (callback from ogg vorbis library)
	public:
		/// ogg vorbisから呼び出されるコールバック関数.
		/// ファイルを読み取る.
		///
		/// buffer	読み取り先バッファ.
		/// size	読み込み単位(ブロック).
		/// n		最大の読み込み単位数. 最大の読み込みバイト数は size*n で求まる.
		/// self	任意データへのポインタ. ここでは自己インスタンスへのポインタを指定する.
		///
		/// returns 実際に読み込んだ単位数. 0の場合はエラーを意味する.
		static size_t StaticReadProc( void* buffer, size_t size, size_t n, void* self );

		/// ogg vorbisから呼び出されるコールバック関数.
		/// ファイルの読み取り位置を変更する.
		/// 
		/// self	任意データへのポインタ. ここでは自己インスタンスへのポインタを指定する.
		/// offset	読み取り位置の移動先.
		/// flag	移動先位置の基点.
		///			SEEK_CUR = 現在位置からの相対位置.
		///			SEEK_END = ファイル末端からの相対位置.
		///			SEEK_SET = ファイル先頭からの相対位置(絶対位置).
		///
		/// returns	0で正常終了, それ以外はエラーID.
		static int    StaticSeekProc( void* self, ogg_int64_t offset, int flag );

		/// ogg vorbisから呼び出されるコールバック関数.
		/// ファイルを閉じる.
		///
		/// self	任意データへのポインタ. ここでは自己インスタンスへのポインタを指定する.
		///
		/// returns	0で正常終了, それ以外はエラーID.
		static int    StaticCloseProc( void* self );

		/// ogg vorbisから呼び出されるコールバック関数.
		/// ファイルの読み取り位置を通知する.
		///
		/// self	任意データへのポインタ. ここでは自己インスタンスへのポインタを指定する.
		///
		/// returns	現在の読み込み位置. 負数はエラーID.
		static long   StaticTellProc( void* self );

		// private method
	private:
		size_t ReadProc( void* buffer, size_t size, size_t n );
		int    SeekProc( ogg_int64_t offset, int flag );
		long   TellProc();
	};

};