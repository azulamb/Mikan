#pragma once

#include <dsound.h>

#ifndef _UNLINK_LIBRARY
#  pragma comment(lib, "dsound.lib")
//#pragma comment(lib, "dxguid.lib")
#  ifdef _MIKANDEV
#    ifndef _DEBUG
#      pragma comment(lib, "Dev/MikanSound.lib")
#    else
#      pragma comment(lib, "Dev/MikanSoundD.lib")
#    endif
#  else
#    ifndef _DEBUG
#      pragma comment(lib, "MikanSound.lib")
#    else
#      pragma comment(lib, "MikanSoundD.lib")
#    endif
#  endif
#endif

#ifdef _MIKANDEV
#ifdef USEMIKANLIBRARY
#include "Dev/Thaleia.h"
#else
#include "Thaleia.h"
#endif
#else
#include "Thaleia.h"
#endif
#define CLASSDEF_DIRECTX_SOUND class ThaleiaSound

//���JSound�N���X
class MikanSoundPub
{
public:
	virtual ~MikanSoundPub() {}

public:
	//!�T�E���h��ǂݍ���
	/*!
		\param sound_number �Ǎ���̃T�E���h�ԍ�
		\param file_path �Ǎ����̃T�E���h�t�@�C��
		\param streaming �X�g���[�~���O�Đ����s�����ǂ���
		\retval true ����
		\retval false ���s
		*/
	virtual bool Load( int sound_number, const char* file_path, bool streaming = false );
	//!�T�E���h��ǂݍ���
	/*!
		\param sound_number �Ǎ���̃T�E���h�ԍ�
		\param module ���W���[���n���h���B
		\param resource �Ǎ����̃T�E���h�t�@�C�����\�[�X��(���\�[�X�^�C�v��SOUND)�B
		\param streaming �X�g���[�~���O�Đ����s�����ǂ���
		\retval true ����
		\retval false ���s
		*/
	virtual bool Load( int sound_number, HMODULE module, const char* resource );

	//!�T�E���h��j������
	/*!
		\param sound_number �j������T�E���h�ԍ�
		*/
	virtual void Unload( int sound_number );

	//!�S�T�E���h��j������
	virtual void UnloadAll();

	//!�Đ�����
	/*!
		\param sound_number �Đ�����T�E���h�ԍ�
		\param loop ���[�v�Đ����邩�ǂ���
		\retval true ����
		\retval false ���s
		*/
	virtual bool Play( int sound_number, bool loop );
	//!�Đ����Ă��邩�ǂ������ׂ�
	/*!
		\param sound_number �Đ����m�F����T�E���h�ԍ�
		\retval true �Đ����Ă���
		\retval false �Đ����Ă��Ȃ�
		*/
	virtual bool IsPlaying( int sound_number );

	//!�ꎞ��~����
	/*!
		\param sound_number �ꎞ��~����T�E���h�ԍ�
		*/
	virtual void Pause( int sound_number );

	//!�S�ẴT�E���h���ꎞ��~����
	virtual void PauseAll();

	//!��~����
	/*!
		\param sound_number ��~����T�E���h�ԍ�
		*/
	virtual void Stop( int sound_number );

	//!�S�ẴT�E���h���~����
	virtual void StopAll( void );

	//!���ʂ��擾����
	/*!
		\param sound_number ���ʂ��擾����T�E���h�ԍ�
		\retval 0(����)�`100(�ő�)�ŕ\������鉹��
		*/
	virtual int GetVolume( int sound_number );

	//!���ʂ�ݒ肷��
	/*!
		\param sound_number ���ʂ�ݒ肷��T�E���h�ԍ�
		\param volume 0(����)�`100(�ő�)�ŕ\������鉹��
		*/
	virtual void SetVolume( int sound_number, int volume );

	//!��ʂ��擾����
	/*!
		\param sound_number ��ʂ��擾����T�E���h�ԍ�
		\retval -100(�ō�)�`0(����)�`100(�ŉE)�ŕ\���������
		*/
	virtual int GetPan( int sound_number );

	//!��ʂ�ݒ肷��
	/*!
		\param sound_number ��ʂ�ݒ肷��T�E���h�ԍ�
		\param pan -100(�ō�)�`0(����)�`100(�ŉE)�ŕ\���������
		*/
	virtual void SetPan( int sound_number, int pan );

	//!�Đ����g�����擾����
	/*!
		\param sound_number �Đ����g�����擾����T�E���h�ԍ�
		\retval �Đ����g��
		*/
	virtual unsigned long GetFrequency( int sound_number );

	//!�Đ����g����ݒ肷��
	/*!
		\param sound_number �Đ����g�����擾����T�E���h�ԍ�
		\param frequency �Đ����g��
		*/
	virtual void SetFrequency( int sound_number, unsigned long frequency );

	//!�Đ��ʒu���擾����
	/*!
		\param sound_number �Đ����Ԃ��擾����T�E���h�ԍ�
		\retval �Đ��ʒu�i�~���b�P�ʁj
		*/
	virtual unsigned long Tell( int sound_number );

	//!�Đ��ʒu��ݒ肷��
	/*!
		\param sound_number �Đ��ʒu��ݒ肷��T�E���h�ԍ�
		\param time �Đ��ʒu�i�~���b�P�ʁj
		*/
	virtual void Seek( int sound_number, unsigned long time );

	//!���[�v�ʒu���擾����
	/*!
		\param sound_number ���[�v�ʒu���擾����T�E���h�ԍ�
		\param loop_start ���[�v�J�n�ʒu���i�[����ϐ��ւ̃|�C���^ NULL���w�肵�Ă��悢
		\param loop_end ���[�v�I���ʒu���i�[����ϐ��ւ̃|�C���^ NULL���w�肵�Ă��悢
		\retval true ����
		\retval false ���s
		*/
	virtual bool GetLoop( int sound_number, unsigned long* loop_start, unsigned long* loop_end );

	//!���[�v�ʒu��ݒ肷��
	/*!
		\param sound_number ���[�v�ʒu���擾����T�E���h�ԍ�
		\param loop_start ���[�v�J�n�ʒu
		\param loop_end ���[�v�I���ʒu
		\retval true ����
		\retval false ���s
		*/
	virtual bool SetLoop( int sound_number, unsigned long loop_start, unsigned long loop_end );
};

//����JSound�N���X(����)
//  �����̃��b�p�[�Ȃ̂Ńw�b�_�ɒ���������B
//  Thaleia���g�������l������΂�����T���v���Ƃ��ė~�����B
class ThaleiaSound
{
private:
	static const int MAX_SOUND_LENGTH = 50; //�T�E���h�̍ő吔

private:
	IDirectSound8* m_directsound; //DirectSound�ւ̃|�C���^
	//Thaleia::IPcmPlayer* m_players[MAX_SOUND_LENGTH]; //�T�E���h���X�g
	std::unique_ptr<Thaleia::IVoice> m_players[ MAX_SOUND_LENGTH ];
public:
	ThaleiaSound() :
		m_directsound( NULL )
	{
		//�T�E���h������
		for ( int i = 0; i < MAX_SOUND_LENGTH; i++ )
			m_players[ i ] = NULL;
	}

	~ThaleiaSound()
	{
		StopAll();
		UnloadAll();
	}

	//MikanSound
public:
	bool Load( int sound_number, const char* file_path, bool streaming )
	{
		if ( m_directsound == NULL )
			return false;

		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//�t�@�C���p�X����t�@�C������
		//Thaleia::IFile* file = Thaleia::createFile(file_path, !streaming);
		auto file = Thaleia::createFile( file_path, !streaming );
		if ( file == NULL )
			return false;

		//�t�@�C���^�C�v����
		Thaleia::FileType file_type = Thaleia::suggestFileType( file );

		//PCM�n���h������
		//Thaleia::IPcmHandler* pcm = Thaleia::createPcmHandler(file_type);
		auto pcm = Thaleia::createPcmHandler( file_type );
		if ( pcm == NULL )
		{
			//delete file;
			return false;
		}

		//PCM�n���h���̃\�[�X�Ƃ��Đ�قǂ̃t�@�C�����w�肷��
		//if(!pcm->bind(file))
		if ( !pcm->bind( std::move( file ) ) )
		{
			//delete pcm;
			//delete file;
			return false;
		}

		Unload( sound_number );

		//PCM�v���C���[����
		//WAVE�t�@�C���ɑ΂��Ă͔񃊃��O�o�b�t�@�v���C���[���A
		//����ȊO�̃t�@�C���ɑ΂��Ă̓����O�o�b�t�@�v���C���[��p�ӂ���B
		//Thaleia::IPcmPlayer* player = Thaleia::createPlayer(m_directsound, (file_type!=Thaleia::FILETYPE_WAVE));
		//std::unique_ptr<Thaleia::IVoice> player
		m_players[ sound_number ] = Thaleia::createPlayer( m_directsound, ( file_type != Thaleia::FILETYPE_WAVE ) );
		if ( m_players[ sound_number ] == NULL )
		{
			//delete pcm; //���̎��_��file��pcm�̊Ǘ����ɂ���̂ŁApcm�̂�delete����΂悢
			return false;
		}

		//�^�O��񂩂烋�[�v��ݒ肷��
		{
			int loop_start = -1;
			int loop_end = -1;
			int loop_length = -1;
			int tag_length = pcm->getTagLength(); //�^�O�̌����擾
			for ( int i = 0; i < tag_length; i++ ) //�S�Ẵ^�O�𑖍�
			{
				//"����=�l"�̌`������l��ǂݎ��
				/*const char* str = pcm->getTag(i);
				if(strstr(str, "LOOPSTART=") == str)
				loop_start = atoi(str+10);
				else if(strstr(str, "LOOPEND=") == str)
				loop_end = atoi(str+8);
				else if(strstr(str, "LOOPLENGTH=") == str)
				loop_length = atoi(str+11);*/
				std::string tag = pcm->getTag( i );
				int equal_position = tag.find( "=" );
				if ( equal_position == std::string::npos )
					continue;

				std::string element = tag.substr( 0, equal_position );
				std::string value = tag.substr( equal_position + 1 );
				if ( element == "LOOPSTART" )
					loop_start = atoi( value.c_str() );
				else if ( element == "LOOPEND" )
					loop_end = atoi( value.c_str() );
				else if ( element == "LOOPLENGTH" )
					loop_length = atoi( value.c_str() );
			}

			//LOOPSTART��LOOPEND���w�肳��Ă���P�[�X
			if ( loop_start >= 0 && loop_end > loop_start )
			{
				m_players[ sound_number ]->setLoopTime( loop_start, loop_end ); //���[�v�ʒu�ݒ�
				//        m_players[sound_number]->enableLoop(true);
			}
			//LOOPSTART��LOOPLENGTH���w�肳��Ă���P�[�X�iRPG�c�N�[���̎w��t�H�[�}�b�g�j
			else if ( loop_start >= 0 && loop_length > 0 )
			{
				m_players[ sound_number ]->setLoopTime( loop_start, loop_start + loop_length );
				//        m_players[sound_number]->enableLoop(true);
			}
		}

		//PCM�v���C���[�̃\�[�X�Ƃ��Đ�قǂ�PCM�n���h�����w�肷��
		//if(!player->bind(pcm))
		if ( !m_players[ sound_number ]->bind( std::move( pcm ) ) )
		{
			//delete player;
			//delete pcm;
			return false;
		}

		//�T�E���h���X�g�ɔ��f
		//Unload(sound_number); //��Ɏw��ԍ��ɂ������T�E���h��Unload�����
		//m_players[sound_number] = player;

		//�Đ���������
		return true;
	}

	bool Load( int sound_number, HMODULE module, const char* resource )
	{
		if ( m_directsound == NULL )
			return false;

		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//�t�@�C���p�X����t�@�C������
		auto file = Thaleia::createResourceFile( module, resource );
		if ( file == NULL )
			return false;

		//�t�@�C���^�C�v����
		Thaleia::FileType file_type = Thaleia::suggestFileType( file );

		//PCM�n���h������
		auto pcm = Thaleia::createPcmHandler( file_type );
		if ( pcm == NULL )
		{
			return false;
		}

		//PCM�n���h���̃\�[�X�Ƃ��Đ�قǂ̃t�@�C�����w�肷��
		if ( !pcm->bind( std::move( file ) ) )
		{
			return false;
		}

		Unload( sound_number );

		//PCM�v���C���[����
		//WAVE�t�@�C���ɑ΂��Ă͔񃊃��O�o�b�t�@�v���C���[���A
		//����ȊO�̃t�@�C���ɑ΂��Ă̓����O�o�b�t�@�v���C���[��p�ӂ���B
		m_players[ sound_number ] = Thaleia::createPlayer( m_directsound, ( file_type != Thaleia::FILETYPE_WAVE ) );
		if ( m_players[ sound_number ] == NULL )
		{
			//delete pcm; //���̎��_��file��pcm�̊Ǘ����ɂ���̂ŁApcm�̂�delete����΂悢
			return false;
		}

		//�^�O��񂩂烋�[�v��ݒ肷��
		{
			int loop_start = -1;
			int loop_end = -1;
			int loop_length = -1;
			int tag_length = pcm->getTagLength(); //�^�O�̌����擾
			for ( int i = 0; i < tag_length; i++ ) //�S�Ẵ^�O�𑖍�
			{
				std::string tag = pcm->getTag( i );
				int equal_position = tag.find( "=" );
				if ( equal_position == std::string::npos )
					continue;

				std::string element = tag.substr( 0, equal_position );
				std::string value = tag.substr( equal_position + 1 );
				if ( element == "LOOPSTART" )
					loop_start = atoi( value.c_str() );
				else if ( element == "LOOPEND" )
					loop_end = atoi( value.c_str() );
				else if ( element == "LOOPLENGTH" )
					loop_length = atoi( value.c_str() );
			}

			//LOOPSTART��LOOPEND���w�肳��Ă���P�[�X
			if ( loop_start >= 0 && loop_end > loop_start )
			{
				m_players[ sound_number ]->setLoopTime( loop_start, loop_end ); //���[�v�ʒu�ݒ�
			}
			//LOOPSTART��LOOPLENGTH���w�肳��Ă���P�[�X�iRPG�c�N�[���̎w��t�H�[�}�b�g�j
			else if ( loop_start >= 0 && loop_length > 0 )
			{
				m_players[ sound_number ]->setLoopTime( loop_start, loop_start + loop_length );
			}
		}

		//PCM�v���C���[�̃\�[�X�Ƃ��Đ�قǂ�PCM�n���h�����w�肷��
		if ( !m_players[ sound_number ]->bind( std::move( pcm ) ) )
		{
			return false;
		}

		//�Đ���������
		return true;
	}

	void Unload( int sound_number )
	{
		//�����͈̔͌���
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		if ( m_players[ sound_number ] != NULL )
		{
			//delete����NULL�������Ă���
			//delete m_players[sound_number];
			m_players[ sound_number ] = NULL;
		}
	}

	void UnloadAll()
	{
		for ( int i = 0; i < MAX_SOUND_LENGTH; i++ )
		{
			//Unload()�𖈉�ĂԂƈ����͈̔͌��������ʂȂ̂Ŏ��O�ŏ���
			if ( m_players[ i ] != NULL )
			{
				//delete m_players[i];
				m_players[ i ] = NULL;
			}
		}
	}

	bool Play( int sound_number, bool loop )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return false;

		//player->setLoop(loop);
		m_players[ sound_number ]->enableLoop( loop );
		m_players[ sound_number ]->play();

		return true;
	}

	bool IsPlaying( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return false;

		return m_players[ sound_number ]->isPlaying();
	}

	void Pause( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] != NULL )
			m_players[ sound_number ]->pause();
	}

	void PauseAll()
	{
		for ( int i = 0; i < MAX_SOUND_LENGTH; i++ )
		{
			//Thaleia::IPcmPlayer* player = m_players[i];
			//std::unique_ptr<Thaleia::IVoice> player = m_players[i];
			if ( m_players[ i ] != NULL )
				m_players[ i ]->pause();
		}
	}

	void Stop( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] != NULL )
			m_players[ sound_number ]->stop();
	}

	void StopAll()
	{
		for ( int i = 0; i < MAX_SOUND_LENGTH; i++ )
		{
			//Thaleia::IPcmPlayer* player = m_players[i];
			//std::unique_ptr<Thaleia::IVoice> player = m_players[i];
			if ( m_players[ i ] != NULL )
				m_players[ i ]->stop();
		}
	}

	int GetVolume( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return 0;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return 0;

		return (int)( m_players[ sound_number ]->getVolume() * 100 );
	}

	void SetVolume( int sound_number, int volume )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return;

		m_players[ sound_number ]->setVolume( (double)volume / 100 );
	}

	int GetPan( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return 0;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return 0;

		return (int)( m_players[ sound_number ]->getPan() * 100 );
	}

	void SetPan( int sound_number, int pan )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return;

		m_players[ sound_number ]->setPan( (double)pan / 100 );
	}

	unsigned long GetFrequency( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return 0;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return 0;

		return m_players[ sound_number ]->getFrequency();
	}

	void SetFrequency( int sound_number, unsigned long frequency )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return;

		m_players[ sound_number ]->setFrequency( frequency );
	}

	/*  unsigned long Size( int sound_number )
	  {
	  if(sound_number < 0 || sound_number >= MAX_SOUND_LENGTH)
	  return 0;

	  if(m_players[sound_number] == NULL)
	  return 0;

	  return m_players[sound_number]->size();
	  }*/

	unsigned long Tell( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return 0;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return 0;

		return m_players[ sound_number ]->tell();
	}

	void Seek( int sound_number, unsigned long time )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return;

		m_players[ sound_number ]->seek( time );
	}

	bool GetLoop( int sound_number, unsigned long* loop_start, unsigned long* loop_end )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return false;

		return m_players[ sound_number ]->getLoopTime( loop_start, loop_end );
	}

	bool SetLoop( int sound_number, unsigned long loop_start, unsigned long loop_end )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return false;

		return m_players[ sound_number ]->setLoopTime( loop_start, loop_end );
	}

	//ThaleiaSound
public:
	//!�������֐�
	/*!
		\param window_handle ���C���E�B���h�E�̃n���h��
		\retval true ����
		\retval false ���s
		*/
	bool Initialize( HWND window_handle )
	{
		if ( m_directsound != NULL )
			return false;

		HRESULT ret;

		//DirectSound����
		ret = DirectSoundCreate8( NULL, &m_directsound, NULL );
		if ( ret != DS_OK )
			return false;

		//�������x���ݒ�
		ret = m_directsound->SetCooperativeLevel( window_handle, DSSCL_PRIORITY );
		if ( ret != DS_OK )
			return false;

		//����
		return true;
	}

};
