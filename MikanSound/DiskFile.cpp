#include <windows.h>
#include <dsound.h>
#include "IFile.h"
#include "DiskFile.h"
#include "debug.h"

namespace Thaleia
{
	//----------------------------------------------------------
	unsigned long DiskFile::read( void* buffer, unsigned long length )
	{
		//�t�@�C���n���h���������Ă��Ȃ���΃G���[
		if ( m_file_handle == nullptr )
		{
			LOG( "This instance does not have a file handle." );
			return 0;
		}

		//������NULL�Ȃ�΃G���[
		if ( buffer == nullptr )
		{
			LOG( "Argument 'buffer' is nullptr." );
			return 0;
		}

		//�t�@�C���ǂݍ���
		DWORD read_size; /// �ǂݍ��݂ɐ��������o�C�g��
		ReadFile( m_file_handle.get(), buffer, length, &read_size, nullptr );

		//�ǂݍ��񂾃o�C�g����Ԃ�
		return read_size;
	}

	unsigned long DiskFile::size() const
	{
		//�t�@�C���n���h���������Ă��Ȃ���΃G���[
		if ( m_file_handle == nullptr )
		{
			LOG( "This instance does not have a file handle." );
			return 0;
		}

		//�t�@�C���T�C�Y�擾
		unsigned long low = GetFileSize( m_file_handle.get(), nullptr );

		//GetFileSize�����s�����Ƃ�
		if ( low == -1 )
		{
			LOG( "Getting file size failed." );
			return 0;
		}

		//�t�@�C���T�C�Y��Ԃ�
		return low;
	}

	unsigned long DiskFile::tell() const
	{
		//�t�@�C���n���h���������Ă��Ȃ���΃G���[
		if ( m_file_handle == nullptr )
		{
			LOG( "Not initialized." );
			return 0;
		}

		//���݈ʒu�擾
		unsigned long low = SetFilePointer( m_file_handle.get(), 0, nullptr, FILE_CURRENT );

		//SetFilePointer�����s�����Ƃ�
		if ( low == INVALID_SET_FILE_POINTER )
		{
			LOG( "Getting file cursor failed." );
			return 0;
		}

		//���݈ʒu��Ԃ�
		return low;
	}

	bool DiskFile::seek( long long offset, bool relatively )
	{
		//�t�@�C���n���h���������Ă��Ȃ���΃G���[
		if ( m_file_handle == nullptr )
		{
			LOG( "Not initialized." );
			return 0;
		}

		//8�o�C�g�^��offset��4�o�C�g�^��2�ϐ��Ƀo����
		long low = offset & 0xFFFFFFFF;	/// offset�̉���4�o�C�g
		long high = offset >> 32;			/// offset�̏��4�o�C�g

		//�ǂݎ��ʒu��ύX����
		unsigned long result = SetFilePointer( m_file_handle.get(), low, &high, ( relatively ? FILE_CURRENT : FILE_BEGIN ) );

		//SetFilePointer�����s�����Ƃ�
		if ( result == INVALID_SET_FILE_POINTER )
		{
			LOG( "Setting file cursor failed." );
			return false;
		}

		//����
		return true;
	}

	//----------------------------------------------------------
	bool DiskFile::open( const std::string& file_path )
	{
		//���Ƀt�@�C���n���h�����J���Ă���Ȃ�΃G���[
		if ( m_file_handle != nullptr )
		{
			LOG( "This instance already has a file handle." );
			return false;
		}

		//�ǂݎ�胂�[�h�Ńt�@�C�����J��
		m_file_handle = decltype( m_file_handle )
			(
				CreateFileA( file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr ),
				[]( HANDLE handle ) //������̏���
		{
			CloseHandle( handle );
		}
		);

		//�t�@�C�����J���Ă��Ȃ���΃G���[.
		//CreateFileA�̎��s���̖߂�l��NULL�ł͂Ȃ����Ƃɒ���.
		if ( m_file_handle.get() == INVALID_HANDLE_VALUE )
		{
			LOG( "Cannot open file '%s'.", file_path );
			m_file_handle.release();
			return false;
		}

		//����
		return true;
	}

	void DiskFile::close()
	{
		//�t�@�C����j��
		m_file_handle.reset();
	}

	bool DiskFile::bind( HANDLE file_handle )
	{
		//���Ƀt�@�C���n���h�����J���Ă���Ȃ�΃G���[
		if ( m_file_handle != nullptr )
		{
			LOG( "This instance already has a file handle." );
			return false;
		}

		//������NULL�Ȃ�΃G���[
		if ( file_handle == nullptr )
		{
			LOG( "Argument 'file_handle' is NULL." );
			return false;
		}

		//�����������ȃn���h���Ȃ�΃G���[
		if ( file_handle == INVALID_HANDLE_VALUE )
		{
			LOG( "Argument 'file_handle' is INVALID_HANDLE_VALUE." );
			return false;
		}

		//unique_ptr�ɊǗ����Ϗ�
		m_file_handle = decltype( m_file_handle )
			(
				file_handle,
				[]( HANDLE handle ) //������̏���
		{
			CloseHandle( handle );
		}
		);
		return true;
	}

	HANDLE DiskFile::unbind()
	{
		//�n���h�����R�s�[���Ă���
		HANDLE file_handle = m_file_handle.get();

		//�n���h���̊Ǘ������
		m_file_handle.release();

		//�n���h����Ԃ�
		return file_handle;
	}

};