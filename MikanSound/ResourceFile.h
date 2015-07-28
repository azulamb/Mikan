#pragma once

#include <memory>
#include <functional>
#include <string>

namespace Thaleia
{

	///
	/// �f�B�X�N��̃t�@�C����ǂݎ��N���X.
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
		/// �t�@�C�����J��
		/// 
		/// file_path	�J���t�@�C���̃p�X
		///
		/// returns		�t�@�C���I�[�v���ɐ��������Ȃ��true, �����Ȃ���false.
		bool open( HMODULE module, const char *resource );

		/// �t�@�C�������
		void close();
	};

};