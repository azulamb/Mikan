#include <windows.h>
#include <dsound.h>
#include "IFile.h"
#include "ResourceFile.h"
#include "debug.h"

namespace Thaleia
{
	//----------------------------------------------------------
	unsigned long ResourceFile::read( void* buffer, unsigned long length )
	{
		DWORD read_size; /// �ǂݍ��݂ɐ��������o�C�g��

		//�f�[�^�̐擪�|�C���^�������Ă��Ȃ���΃G���[
		if ( data == NULL )
		{
			LOG( "This instance does not have a resource." );
			return 0;
		}

		//������NULL�Ȃ�΃G���[
		if ( buffer == NULL )
		{
			LOG( "Argument 'buffer' is nullptr." );
			return 0;
		}

		// �f�[�^�ǂݍ���
		// �Ƃ������̃|�C���^�ړ��Ƃ��B
		if ( load + length > filesize )
		{
			length = filesize - load;
		}
		CopyMemory( buffer, (char *)data + load, length );
		read_size = length;
		load += length;

		//�ǂݍ��񂾃o�C�g����Ԃ�
		return read_size;
	}

	unsigned long ResourceFile::size() const
	{
		//�f�[�^�̐擪�|�C���^�������Ă��Ȃ���΃G���[
		if ( data == NULL )
		{
			LOG( "This instance does not have a resource." );
			return 0;
		}

		//�t�@�C���T�C�Y��Ԃ�
		return filesize;
	}

	unsigned long ResourceFile::tell() const
	{
		//�f�[�^�̐擪�A�h���X�������Ă��Ȃ���΃G���[
		if ( data == NULL )
		{
			LOG( "Not initialized." );
			return 0;
		}

		//���݈ʒu��Ԃ�
		return load;
	}

	bool ResourceFile::seek( long long offset, bool relatively )
	{
		//�f�[�^�̐擪�A�h���X�������Ă��Ȃ���΃G���[
		if ( data == NULL )
		{
			LOG( "Not initialized." );
			return 0;
		}

		//8�o�C�g�^��offset��4�o�C�g�^��2�ϐ��Ƀo����
		long low = offset & 0xFFFFFFFF;	/// offset�̉���4�o�C�g
		long high = offset >> 32;			/// offset�̏��4�o�C�g

		//�ǂݎ��ʒu��ύX����
		if ( relatively )
		{
			load += low;
		} else
		{
			load = low;
		}

		//����
		return true;
	}

	//----------------------------------------------------------
	bool ResourceFile::open( HMODULE module, const char *resource )
	{
		HRSRC hres;

		//���Ƀ��\�[�X��ǂݍ���ł���Ȃ�΃G���[
		if ( data != NULL )
		{
			LOG( "This instance already has a file handle." );
			return false;
		}

		//�ǂݎ�胂�[�h�Ńt�@�C�����J��

		hres = FindResource( module, resource, "SOUND" );

		data = LockResource( LoadResource( module, hres ) );

		//���\�[�X���J���Ă��Ȃ���΃G���[.
		if ( data == NULL )
		{
			LOG( "Cannot load resource '%s'.", resource );
			return false;
		}

		filesize = SizeofResource( module, hres );
		load = 0;

		//����
		return true;
	}

	void ResourceFile::close()
	{
		//���\�[�X��j���c�c�ł��Ȃ�
		data = NULL;
	}

};
