#include <math.h>
#include <windows.h>
#include <dsound.h>
#include "debug.h"
#include "DirectSoundBufferWrapper.h"

namespace Thaleia
{
	//--------------------------------------------------
	bool DirectSoundBufferWrapper::initialize( IDirectSound* directsound, unsigned long size, WAVEFORMATEX* waveformatex, bool global )
	{
		if ( m_dsbuffer != nullptr )
			return false;

		//DSBUFFERDESC�쐬
		DSBUFFERDESC desc;
		desc.dwSize = sizeof( DSBUFFERDESC );
		desc.dwFlags =
			DSBCAPS_CTRLFREQUENCY |		// ���g���R���g���[�����\�ɂ���
			DSBCAPS_CTRLPAN |		// PAN�R���g���[�����\�ɂ���
			DSBCAPS_CTRLVOLUME |		// ���ʃR���g���[�����\�ɂ���
			DSBCAPS_GETCURRENTPOSITION2 |		// �V����GetCurrentPosition()�̋������g�p����
			( global ? DSBCAPS_GLOBALFOCUS : 0 );	// ��A�N�e�B�u�ōĐ����邩�ǂ���
		desc.dwBufferBytes = size;
		desc.dwReserved = 0;
		desc.lpwfxFormat = waveformatex;
		desc.guid3DAlgorithm = GUID_NULL;

		//IDirectSound����DirectSoundBuffer�𐶐�
		if ( directsound->CreateSoundBuffer( &desc, &m_dsbuffer, nullptr ) != DS_OK )
			return false;

		//�������O�ɐݒ肳��Ă����p�����[�^��K�p
		if ( m_frequency > 0 )
			setFrequency( m_frequency );
		else
		{
			if ( m_dsbuffer->GetFrequency( &m_frequency ) != DS_OK )
			{
				//���̂����g�������Ȃ��ꍇ
				release();
				return false;
			}
		}
		setVolume( m_volume );
		setPan( m_pan );

		//����
		return true;
	}

	void DirectSoundBufferWrapper::setVolume( double volume )
	{
		//�~���x���ɕϊ�
		long volume_mb;
		if ( volume <= 0.0f )
		{
			volume = 0.0f;
			volume_mb = DSBVOLUME_MIN;
		} else if ( volume >= 1.0f )
		{
			volume = 1.0f;
			volume_mb = DSBVOLUME_MAX;
		} else
		{
			//�v�Z
			volume_mb = (long)( log10( ( volume*volume ) ) * 1000 );

			//�덷�h�~
			if ( volume_mb < DSBVOLUME_MIN )
				volume_mb = DSBVOLUME_MIN;
			else if ( volume_mb > DSBVOLUME_MAX )
				volume_mb = DSBVOLUME_MAX;
		}

		//����
		m_volume = volume;
		if ( m_dsbuffer != nullptr )
		{
			HRESULT result = m_dsbuffer->SetVolume( volume_mb );
			validateResult( result );
		}
	}

	void DirectSoundBufferWrapper::setPan( double pan )
	{
		//�~���x���ɕϊ�
		long pan_mb;
		if ( pan <= -1.0f )
		{
			pan = -1.0f;
			pan_mb = DSBPAN_LEFT;
		} else if ( pan >= 1.0f )
		{
			pan = 1.0f;
			pan_mb = DSBPAN_RIGHT;
		} else if ( pan == 0.0f )
			pan_mb = DSBPAN_CENTER;
		else
		{
			//�v�Z
			double diff = 1.0f - abs( pan );
			pan_mb = (long)( log10( diff*diff ) * 1000 );
			if ( pan > 0 )
				pan_mb *= -1;

			//�덷�h�~
			if ( pan_mb < DSBPAN_LEFT )
				pan_mb = DSBPAN_LEFT;
			else if ( pan_mb > DSBPAN_RIGHT )
				pan_mb = DSBPAN_RIGHT;
		}

		//����
		m_pan = pan;
		if ( m_dsbuffer != nullptr )
		{
			HRESULT result = m_dsbuffer->SetPan( pan_mb );
			validateResult( result );
		}
	}

};