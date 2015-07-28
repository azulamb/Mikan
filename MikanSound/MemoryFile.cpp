#include <functional>
#include <stdio.h>
#include <windows.h>
#include <dsound.h>
#include "debug.h"
#include "IFile.h"
#include "MemoryFile.h"

namespace Thaleia
{
	//----------------------------------------------------------
	unsigned long MemoryFile::read( void* buffer, unsigned long length )
	{
		//�t�@�C���̓ǂݍ��ݑO�Ȃ�΃G���[
		if ( m_buffer == nullptr )
		{
			LOG( "This instance does not load a file yet." );
			return 0;
		}

		//������NULL�Ȃ�΃G���[
		if ( buffer == nullptr )
		{
			LOG( "Argument 'buffer' is nullptr." );
			return 0;
		}

		//�ǂݎ��T�C�Y
		unsigned long read_size = length;

		//�ǂݎ��T�C�Y���t�@�C�������𒴂���Ƃ�
		if ( m_read_cursor + read_size > m_size )
		{
			//�ǂݎ��ʒu�����ɖ����ɂ���Ȃ��, �����ǂݎ��Ȃ�
			if ( m_size <= m_read_cursor )
				return 0;

			//�ǂݎ��T�C�Y�𖖔��̈ʒu�܂Ō��炷
			read_size = m_size - m_read_cursor;
		}

		//�ǂݎ��
		memcpy( buffer, m_buffer.get() + m_read_cursor, read_size );

		//�ǂݎ��ʒu�O�i
		m_read_cursor += read_size;

		//����
		return read_size;
	}

	unsigned long MemoryFile::size() const
	{
		//�t�@�C���̓ǂݍ��ݑO�Ȃ�΃G���[
		if ( m_buffer == nullptr )
		{
			LOG( "This instance does not load a file yet." );
			return 0;
		}

		//�t�@�C���T�C�Y��Ԃ�
		return m_size;
	}

	unsigned long MemoryFile::tell() const
	{
		//�t�@�C���̓ǂݍ��ݑO�Ȃ�΃G���[
		if ( m_buffer == nullptr )
		{
			LOG( "This instance does not load a file yet." );
			return 0;
		}

		//�ǂݎ��ʒu��Ԃ�
		return m_read_cursor;
	}

	bool MemoryFile::seek( long long offset, bool relatively )
	{
		//�t�@�C���̓ǂݍ��ݑO�Ȃ�΃G���[
		if ( m_buffer == nullptr )
		{
			LOG( "This instance does not load a file yet." );
			return 0;
		}

		//�ړ�����v�Z����
		long long position; ///�ړ���
		if ( relatively )
			position = m_read_cursor + offset;
		else
			position = offset;

		//�ړ��悪�t�@�C���T�C�Y�͈̔͊O�ɏo�Ȃ��悤�Ƀ`�F�b�N
		if ( position < 0 )
			position = 0;
		else if ( m_size < position )
			position = m_size;

		//�ړ�
		m_read_cursor = (unsigned long)position;

		//����
		return true;
	}

	//----------------------------------------------------------
	bool MemoryFile::load( const std::string& file_path )
	{
		//�ǂݎ�胂�[�h�Ńt�@�C�����J��
		auto file_handle = std::unique_ptr<void, std::function<void( HANDLE )>>
			(
				CreateFileA( file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr ),
				[]( HANDLE handle )
		{
			CloseHandle( handle );
		}
		);

		//�t�@�C�����J���Ă��Ȃ���΃G���[.
		//CreateFileA�̎��s���̖߂�l��NULL�ł͂Ȃ����Ƃɒ���.
		if ( file_handle.get() == INVALID_HANDLE_VALUE )
		{
			LOG( "Cannot open file '%s'.", file_path );
			file_handle.release();
			return false;
		}

		//�ǂݍ��ݕ����� load(HANDLE file_handle) �Ɋۓ���
		bool result = load( file_handle.get() );

		//����
		return result;
	}

	bool MemoryFile::load( HANDLE file_handle )
	{
		//���ɓǂݍ��񂾃t�@�C��������ꍇ�̓G���[
		if ( m_buffer != nullptr )
		{
			LOG( "This instance already has a file buffer." );
			return false;
		}

		//������NULL�Ȃ�΃G���[
		if ( file_handle == nullptr )
		{
			LOG( "Argument 'file_handle' is NULL." );
			return false;
		}

		//�t�@�C���T�C�Y�擾
		unsigned long size;
		{
			unsigned long low = GetFileSize( file_handle, nullptr );
			if ( low == -1 )
			{
				LOG( "Getting file size error." );
				return false;
			}

			size = low;
		}

		//�o�b�t�@�m��
		decltype( m_buffer ) buffer;
		try
		{
			buffer.reset( new char[ size ] );
		} catch ( ... )
		{
			LOG( "Memory allocation failed." );
			buffer.release();
			return false;
		}

		//�t�@�C���ǂݍ���
		unsigned long read_size;
		SetFilePointer( file_handle, 0, nullptr, FILE_BEGIN ); //�擪�ɃV�[�N
		ReadFile( file_handle, buffer.get(), size, &read_size, nullptr );

		//�ǂݍ��ݗv���T�C�Y�Ǝ��ۂɓǂݍ��񂾃T�C�Y���قȂ�ꍇ�̓G���[
		if ( read_size != size )
		{
			LOG( "Reading file failed." );
			return false;
		}

		//����
		m_buffer = std::move( buffer );
		m_size = size;
		m_read_cursor = 0;
		return true;
	}

	void MemoryFile::unload()
	{
		//�o�b�t�@���������
		m_buffer.reset();

		//�ϐ��̏�����
		m_size = 0;
		m_read_cursor = 0;
	}

};