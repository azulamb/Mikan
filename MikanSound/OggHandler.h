#pragma once

#include "IAudioHandler.h"
#include "IFile.h"

#include <functional>
#include "vorbis/vorbisfile.h"

namespace Thaleia
{

	/**
	 *	OGG�t�@�C���������N���X
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
		// �t�@�C���C���^�[�t�F�C�X�̃C���X�^���X
		std::unique_ptr<IFile> m_file;

		// ogg-vorbis���C�u���������p����`���̃t�@�C���n���h��
		std::unique_ptr<OggVorbis_File, std::function<void( OggVorbis_File* )>> m_oggvorbis;

		// public constructor
	public:
		OggHandler()
		{
		}

		virtual ~OggHandler()
		{
			// unbind()�����m_oggvorbis, m_file�̏��Ŕj������邱�Ƃ��ۏႳ���.
			// ogg-vorbis���C�u�����̃X���b�h�������������Ȃ̂�, �O�̂��߂��̏�������邱��.
			unbind();
		}

		// public static method (callback from ogg vorbis library)
	public:
		/// ogg vorbis����Ăяo�����R�[���o�b�N�֐�.
		/// �t�@�C����ǂݎ��.
		///
		/// buffer	�ǂݎ���o�b�t�@.
		/// size	�ǂݍ��ݒP��(�u���b�N).
		/// n		�ő�̓ǂݍ��ݒP�ʐ�. �ő�̓ǂݍ��݃o�C�g���� size*n �ŋ��܂�.
		/// self	�C�Ӄf�[�^�ւ̃|�C���^. �����ł͎��ȃC���X�^���X�ւ̃|�C���^���w�肷��.
		///
		/// returns ���ۂɓǂݍ��񂾒P�ʐ�. 0�̏ꍇ�̓G���[���Ӗ�����.
		static size_t StaticReadProc( void* buffer, size_t size, size_t n, void* self );

		/// ogg vorbis����Ăяo�����R�[���o�b�N�֐�.
		/// �t�@�C���̓ǂݎ��ʒu��ύX����.
		/// 
		/// self	�C�Ӄf�[�^�ւ̃|�C���^. �����ł͎��ȃC���X�^���X�ւ̃|�C���^���w�肷��.
		/// offset	�ǂݎ��ʒu�̈ړ���.
		/// flag	�ړ���ʒu�̊�_.
		///			SEEK_CUR = ���݈ʒu����̑��Έʒu.
		///			SEEK_END = �t�@�C�����[����̑��Έʒu.
		///			SEEK_SET = �t�@�C���擪����̑��Έʒu(��Έʒu).
		///
		/// returns	0�Ő���I��, ����ȊO�̓G���[ID.
		static int    StaticSeekProc( void* self, ogg_int64_t offset, int flag );

		/// ogg vorbis����Ăяo�����R�[���o�b�N�֐�.
		/// �t�@�C�������.
		///
		/// self	�C�Ӄf�[�^�ւ̃|�C���^. �����ł͎��ȃC���X�^���X�ւ̃|�C���^���w�肷��.
		///
		/// returns	0�Ő���I��, ����ȊO�̓G���[ID.
		static int    StaticCloseProc( void* self );

		/// ogg vorbis����Ăяo�����R�[���o�b�N�֐�.
		/// �t�@�C���̓ǂݎ��ʒu��ʒm����.
		///
		/// self	�C�Ӄf�[�^�ւ̃|�C���^. �����ł͎��ȃC���X�^���X�ւ̃|�C���^���w�肷��.
		///
		/// returns	���݂̓ǂݍ��݈ʒu. �����̓G���[ID.
		static long   StaticTellProc( void* self );

		// private method
	private:
		size_t ReadProc( void* buffer, size_t size, size_t n );
		int    SeekProc( ogg_int64_t offset, int flag );
		long   TellProc();
	};

};