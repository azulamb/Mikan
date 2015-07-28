#pragma once

#include "IAudioHandler.h"

namespace Thaleia
{

	/**
	 *	Waveファイルを扱うクラス
	 */
	class WaveHandler : public IAudioHandler
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
		// WaveHandler

		// private constance
	private:
		static const size_t WAVEFORMATEX_SIZE = 128;

		// private variable
	private:
		std::unique_ptr<IFile> m_file;		/// ファイルインターフェースのインスタンス
		union
		{
			WAVEFORMATEX m_format;						/// フォーマット情報
			unsigned char m_padding[ WAVEFORMATEX_SIZE ]; /// 拡張フォーマット用のパディング
		};
		unsigned long m_datachunk_offset;	/// データチャンクの開始位置
		unsigned long m_datachunk_size;		/// データチャンクの大きさ

		// public constructor
	public:
		WaveHandler() :
			m_file( nullptr ),
			m_format(),
			m_datachunk_offset( 0 ),
			m_datachunk_size( 0 )
		{

		}

		virtual ~WaveHandler()
		{
			m_file.reset();
		}

		//private method
	private:
		/// 与えられたWAVEファイルから
		/// m_format, m_datachunk_offset, m_datachunk_sizeの値を取得する.
		/// 
		/// file	WAVEファイル
		bool openWave( const std::unique_ptr<IFile>& file );

	};

};