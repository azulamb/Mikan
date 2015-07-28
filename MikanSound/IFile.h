#pragma once

namespace Thaleia
{

	///	�t�@�C���ǂݍ��݂̃C���^�[�t�F�[�X.
	///
	///	�Í����t�@�C���̓ǂݍ��݂�, �p�b�L���O�f�[�^����̃t�@�C�����o���s���ꍇ��
	/// ���̃C���^�[�t�F�[�X���p����, read�֐��Ő��̃f�[�^��ǂݏo����悤�ɂ���΂悢.
	class IFile
	{
	public:
		virtual ~IFile() {}

	public:
		/// �t�@�C�����e��ǂ�.
		///
		/// buffer	�ǂݎ�����f�[�^�̏������ݐ�.
		/// length	�ő�̓ǂݎ��o�C�g��.
		///
		/// returns	���ۂɓǂݎ�����o�C�g��. �K��length�ȉ��̒l�ɂȂ�.
		virtual unsigned long read( void* buffer, unsigned long length ) = 0;

		/// �t�@�C���T�C�Y�𓾂�.
		///
		/// returns	�t�@�C���T�C�Y.
		virtual unsigned long size() const = 0;

		/// ���݂̓ǂݎ��ʒu�𓾂�.
		/// 
		/// returns	���݂̓ǂݎ��ʒu
		virtual unsigned long tell() const = 0;

		/// �ǂݎ��ʒu��ύX����
		/// 
		/// offset		�ړ���.
		/// elatively	���̈�����ture�̂Ƃ�, ���݂̓ǂݎ��ʒu���瑊�Έړ�����.
		/// 
		/// returns		�ǂݎ��ʒu�̕ύX�ɐ��������true, �����Ȃ���false.
		virtual bool seek( long long offset, bool relatively = false ) = 0;
	};

};