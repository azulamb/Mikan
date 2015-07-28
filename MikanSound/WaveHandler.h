#pragma once

#include "IAudioHandler.h"

namespace Thaleia
{

	/**
	 *	Wave�t�@�C���������N���X
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
		std::unique_ptr<IFile> m_file;		/// �t�@�C���C���^�[�t�F�[�X�̃C���X�^���X
		union
		{
			WAVEFORMATEX m_format;						/// �t�H�[�}�b�g���
			unsigned char m_padding[ WAVEFORMATEX_SIZE ]; /// �g���t�H�[�}�b�g�p�̃p�f�B���O
		};
		unsigned long m_datachunk_offset;	/// �f�[�^�`�����N�̊J�n�ʒu
		unsigned long m_datachunk_size;		/// �f�[�^�`�����N�̑傫��

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
		/// �^����ꂽWAVE�t�@�C������
		/// m_format, m_datachunk_offset, m_datachunk_size�̒l���擾����.
		/// 
		/// file	WAVE�t�@�C��
		bool openWave( const std::unique_ptr<IFile>& file );

	};

};