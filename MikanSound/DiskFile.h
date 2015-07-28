#pragma once

#include <memory>
#include <functional>
#include <string>

namespace Thaleia
{

	///
	/// �f�B�X�N��̃t�@�C����ǂݎ��N���X.
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
		std::unique_ptr<void, std::function<void( HANDLE )>> m_file_handle; /// �t�@�C���n���h��

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
		/// �t�@�C�����J��
		/// 
		/// file_path	�J���t�@�C���̃p�X
		///
		/// returns		�t�@�C���I�[�v���ɐ��������Ȃ��true, �����Ȃ���false.
		bool open( const std::string& file_path );

		/// �t�@�C�������
		void close();

		/// ���ɊJ����Ă���t�@�C���n���h�������̃C���X�^���X�ƌ��т���.
		/// �ȍ~, �n���h���̏��L���͂��̃C���X�^���X���ێ�����.
		/// (����� ReadFile �� CloseHandle �����Ă͂����Ȃ�)
		/// 
		/// file_handle	�o�C���h����
		///
		/// returns		�t�@�C���I�[�v���ɐ��������Ȃ��true, �����Ȃ���false.
		bool bind( HANDLE file_handle );

		/// open �܂��� bind �ɂ���Ă��̃C���X�^���X�ƌ��т���
		/// �t�@�C���n���h���̏��L�����������.
		/// (���O�� CloseHandle ���邱��)
		/// 
		/// returns	������ꂽ�t�@�C���n���h��.
		///			�C���X�^���X���t�@�C���n���h����ێ����Ă��Ȃ��ꍇ��nullptr.
		HANDLE unbind();

	};

};