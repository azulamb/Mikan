#pragma once

#include "IFile.h"

#include <memory>
#include <string>

namespace Thaleia
{
	/// Wave�f�[�^�̃t�H�[�}�b�g���`����\����.
	/// ���p: http://msdn.microsoft.com/ja-jp/library/cc371559.aspx
	struct WaveFormatEx
	{
		/// (wFormatTag)
		/// �E�F�[�u�t�H�[�� �I�[�f�B�I �t�H�[�}�b�g�̃^�C�v. 
		/// �����̈��k�A���S���Y���̃t�H�[�}�b�g �^�O��, Microsoft �œo�^����Ă���. 
		/// �t�H�[�}�b�g �^�O�̊��S�ȃ��X�g��, �w�b�_�[ �t�@�C�� Mmreg.h �ɋL�q����Ă���. 
		/// 1 �`�����l���܂��� 2 �`�����l���� PCM �f�[�^�̏ꍇ, ���̒l�� WAVE_FORMAT_PCM �łȂ���΂Ȃ�Ȃ�. 
		unsigned short format_tag;

		/// (nChannels)
		/// �E�F�[�u�t�H�[�� �I�[�f�B�I �f�[�^�̃`�����l����. 
		/// ���m���� �f�[�^�� 1 �̃`�����l�����g��, �X�e���I �f�[�^�� 2 �̃`�����l�����g��.  
		unsigned short channels;

		/// (nSamplesPerSec)
		/// �T���v�����O ���[�g. 1 �b������̃T���v���� (Hz) �ŕ\��. 
		/// wFormatTag �� WAVE_FORMAT_PCM �̏ꍇ, nSamplesPerSec �̈�ʓI�Ȓl�� 8.0 kHz, 11.025 kHz, 22.05 kHz, 44.1 kHz �ł���. 
		/// PCM �t�H�[�}�b�g�łȂ��ꍇ��, ���[�J�[�̃t�H�[�}�b�g �^�O�̎d�l�ɏ]���Ă��̃����o���v�Z����K�v������.  
		unsigned long  samples_per_second;

		/// (nAvgBytesPerSec)
		/// �t�H�[�}�b�g �^�O�ŕK�v�ȕ��σf�[�^�]�����x. �P�ʂ�, 1 �b������̃o�C�g��. 
		/// wFormatTag �� WAVE_FORMAT_PCM �ł���ꍇ, nAvgBytesPerSec �� nSamplesPerSec �� nBlockAlign �̐ςɓ������Ȃ���΂Ȃ�Ȃ�. 
		/// PCM �t�H�[�}�b�g�łȂ��ꍇ��, ���[�J�[�̃t�H�[�}�b�g �^�O�̎d�l�ɏ]���Ă��̃����o���v�Z����K�v������.  
		unsigned long  average_bytes_per_second;

		/// (nBlockAlign)
		/// �u���b�N �A���C�����g (�o�C�g�P��). 
		/// �u���b�N �A���C�����g��, wFormatTag �t�H�[�}�b�g �^�C�v�̃f�[�^�̍ŏ��P�ʂł���. 
		/// wFormatTag �� WAVE_FORMAT_PCM �܂��� WAVE_FORMAT_EXTENSIBLE �ł���ꍇ, 
		/// nBlockAlign �� nChannels �� wBitsPerSample �̐ς� 8 �Ŋ������l (�o�C�g������̃r�b�g��) �ɓ������Ȃ���΂Ȃ�Ȃ�. 
		/// PCM �t�H�[�}�b�g�łȂ��ꍇ��, ���[�J�[�̃t�H�[�}�b�g �^�O�̎d�l�ɏ]���Ă��̃����o���v�Z����K�v������. 
		///
		/// �\�t�g�E�F�A��, �f�[�^�̕����� nBlockAlign �o�C�g��, ��x�ɏ������Ȃ���΂Ȃ�Ȃ�. 
		/// �f�o�C�X�ɏ������ރf�[�^, �f�o�C�X����ǂݍ��ރf�[�^��, ��Ƀu���b�N�̐擪����J�n���Ȃ���΂Ȃ�Ȃ�. 
		/// ���Ƃ���, PCM �f�[�^�̍Đ���, �T���v���̒��� (�܂�, ��u���b�N �A���C�����g�̋��E) �ŊJ�n���邱�Ƃ͕s���ł���.  
		unsigned short block_align;

		/// (wBitsPerSample)
		/// wFormatTag �t�H�[�}�b�g �^�C�v��, �T���v�����O������̃r�b�g��. 
		/// wFormatTag �� WAVE_FORMAT_PCM �ł���ꍇ, wBitsPerSample �� 8 �܂��� 16 �łȂ���΂Ȃ�Ȃ�. 
		/// PCM �t�H�[�}�b�g�łȂ��ꍇ��, ���[�J�[�̃t�H�[�}�b�g �^�O�̎d�l�ɏ]���Ă��̃����o��ݒ肷��K�v������. 
		/// wFormatTag �� WAVE_FORMAT_EXTENSIBLE �ł���ꍇ��, �C�ӂ� 8 �̔{����ݒ�ł���. 
		/// ���k�X�L�[���� wBitsPerSample �̒l���`�ł��Ȃ����Ƃ�����̂�, ���̃����o�� 0 �ƂȂ�. 
		unsigned short bits_per_sample;

		/// (cbSize)
		/// WAVEFORMATEX �\���̂̌��ɒǉ������t�H�[�}�b�g���̃T�C�Y (�o�C�g�P��). 
		/// ���̏���� PCM �t�H�[�}�b�g�Ŏg����, wFormatTag �̒ǉ��������i�[���邱�Ƃ��ł���. 
		/// wFormatTag �ŕK�v�Ȓǉ���񂪂Ȃ��ꍇ, ���̃����o�� 0 �ɐݒ肵�Ȃ���΂Ȃ�Ȃ�. 
		/// WAVE_FORMAT_PCM �t�H�[�}�b�g�ł� (������ WAVE_FORMAT_PCM �t�H�[�}�b�g�̏ꍇ�Ɍ���), ���̃����o�͖��������.  
		unsigned short extend_size;
	};

	/// �I�[�f�B�I�t�@�C���������N���X�̃C���^�[�t�F�[�X
	///
	/// IFile�C���^�[�t�F�[�X�����C���X�^���X����I�[�f�B�I�t�@�C����ǂݍ���,
	/// PCM�f�[�^(�g�`�f�[�^)��Đ��ɕK�v�ȃp�����[�^���擾���邽�߂̃C���^�[�t�F�[�X��񋟂���.
	class IAudioHandler
	{
	public:
		virtual ~IAudioHandler() {}

	public:
		/// �I�[�f�B�I�t�@�C�������̃C���X�^���X�Ɍ��т���.
		/// �ȍ~, �t�@�C���̏��L���͂��̃C���X�^���X���ێ�����.
		/// 
		/// file	���̃N���X�ƌ��т���t�@�C��. �ǂݎ�菀�����������Ă���K�v������.
		///
		/// returns	���ѕt���ɐ��������ture, �����Ȃ���false.
		virtual bool bind( std::unique_ptr<IFile>&& file ) = 0;

		/// �t�@�C���Ƃ̌��т�����������.
		/// �t�@�C���̏��L���͕ԋp�����.
		virtual std::unique_ptr<IFile> unbind() = 0;

		/// PCM�f�[�^��ǂݎ��.
		///
		/// buffer	�ǂݎ�����f�[�^�̏������ݐ�.
		/// length	�ő�̓ǂݎ��o�C�g��.
		///
		/// returns	���ۂɓǂݎ�����o�C�g��. �K��length�ȉ��̒l�ɂȂ�.
		virtual unsigned long read( void* buffer, unsigned long length ) = 0;

		/// ���݂�PCM�f�[�^�ǂݎ��ʒu�𓾂�
		///
		/// returns	���݂̓ǂݎ��ʒu
		virtual unsigned long tell() const = 0;

		/// ���݂�PCM�f�[�^�ǂݎ��ʒu��ύX����
		///
		/// offset		�ړ���.
		/// elatively	���̈�����ture�̂Ƃ�, ���݂̓ǂݎ��ʒu���瑊�Έړ�����.
		virtual void seek( int offset, bool relatively = false ) = 0;

		/// PCM�̃T�C�Y�𓾂�
		///
		/// returns	PCM�̃T�C�Y
		virtual unsigned long size() const = 0;

		/// WAVEFORMATEX�`���Ńt�H�[�}�b�g�f�[�^�𓾂�
		virtual size_t getWaveFormatEx( void* format, size_t size ) const = 0;

		/// �^�O�̐����擾����
		///
		/// returns	�^�O�̐�.
		virtual int getTagLength() const = 0;

		/// �^�O���擾����.
		///
		/// index	�^�O�̔ԍ�. 0���ŏ�, getTagLength()-1���ő�.
		///
		/// returns	�^�O. �����ƒl�̑Ή������^�O��"����=�l"�̂悤��"="����؂蕶���Ƃ����������Ԃ�.
		virtual std::string getTag( int index ) const = 0;
	};

};