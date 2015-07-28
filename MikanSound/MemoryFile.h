#pragma once

#include <memory>
#include <string>

namespace Thaleia
{

	///
	/// �f�B�X�N��̃t�@�C������������ɓW�J��, 
	/// �����A�N�Z�X�������ɍs�킹��N���X.
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
		std::unique_ptr<char[]> m_buffer;	/// �t�@�C�����e��ێ�����o�b�t�@
		unsigned long m_size;				/// �t�@�C���T�C�Y
		unsigned long m_read_cursor;		/// �ǂݎ��ʒu

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
		/// �t�@�C�����e��ǂݍ���, ��������ɓW�J�E�ێ�����.
		///
		/// file_path	�W�J����t�@�C���̃p�X
		///
		/// returns		�ǂݍ��݂ɐ��������Ȃ��true, �����Ȃ���false.
		bool load( const std::string& file_path );

		/// �t�@�C�����e��ǂݍ���, ��������ɓW�J�E�ێ�����.
		/// �����ɓn���ꂽ�n���h���� CloseHandle ����Ȃ���,
		/// �ǂݎ��ʒu�͕ύX�����.
		///
		/// file_handle	�W�J����t�@�C����, ���ɊJ���Ă���t�@�C���n���h��.
		///
		/// returns		�ǂݍ��݂ɐ��������Ȃ��true, �����Ȃ���false.
		bool load( HANDLE file_handle );

		/// ��������ɕێ����Ă���t�@�C�����e���������.
		void unload();
	};

};