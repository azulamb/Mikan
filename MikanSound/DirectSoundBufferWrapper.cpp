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

		//DSBUFFERDESC作成
		DSBUFFERDESC desc;
		desc.dwSize = sizeof( DSBUFFERDESC );
		desc.dwFlags =
			DSBCAPS_CTRLFREQUENCY |		// 周波数コントロールを可能にする
			DSBCAPS_CTRLPAN |		// PANコントロールを可能にする
			DSBCAPS_CTRLVOLUME |		// 音量コントロールを可能にする
			DSBCAPS_GETCURRENTPOSITION2 |		// 新しいGetCurrentPosition()の挙動を使用する
			( global ? DSBCAPS_GLOBALFOCUS : 0 );	// 非アクティブで再生するかどうか
		desc.dwBufferBytes = size;
		desc.dwReserved = 0;
		desc.lpwfxFormat = waveformatex;
		desc.guid3DAlgorithm = GUID_NULL;

		//IDirectSoundからDirectSoundBufferを生成
		if ( directsound->CreateSoundBuffer( &desc, &m_dsbuffer, nullptr ) != DS_OK )
			return false;

		//初期化前に設定されていたパラメータを適用
		if ( m_frequency > 0 )
			setFrequency( m_frequency );
		else
		{
			if ( m_dsbuffer->GetFrequency( &m_frequency ) != DS_OK )
			{
				//何故か周波数が取れない場合
				release();
				return false;
			}
		}
		setVolume( m_volume );
		setPan( m_pan );

		//完了
		return true;
	}

	void DirectSoundBufferWrapper::setVolume( double volume )
	{
		//ミリベルに変換
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
			//計算
			volume_mb = (long)( log10( ( volume*volume ) ) * 1000 );

			//誤差防止
			if ( volume_mb < DSBVOLUME_MIN )
				volume_mb = DSBVOLUME_MIN;
			else if ( volume_mb > DSBVOLUME_MAX )
				volume_mb = DSBVOLUME_MAX;
		}

		//完了
		m_volume = volume;
		if ( m_dsbuffer != nullptr )
		{
			HRESULT result = m_dsbuffer->SetVolume( volume_mb );
			validateResult( result );
		}
	}

	void DirectSoundBufferWrapper::setPan( double pan )
	{
		//ミリベルに変換
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
			//計算
			double diff = 1.0f - fabs( pan );
			pan_mb = (long)( log10( diff*diff ) * 1000 );
			if ( pan > 0 )
				pan_mb *= -1;

			//誤差防止
			if ( pan_mb < DSBPAN_LEFT )
				pan_mb = DSBPAN_LEFT;
			else if ( pan_mb > DSBPAN_RIGHT )
				pan_mb = DSBPAN_RIGHT;
		}

		//完了
		m_pan = pan;
		if ( m_dsbuffer != nullptr )
		{
			HRESULT result = m_dsbuffer->SetPan( pan_mb );
			validateResult( result );
		}
	}

};