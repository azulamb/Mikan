#pragma once

#include <windows.h>

namespace Thaleia
{

	class DirectSoundBufferWrapper
	{
	private:
		IDirectSoundBuffer* m_dsbuffer;
		unsigned long m_frequency;
		double m_volume;
		double m_pan;

	public:
		DirectSoundBufferWrapper() :
			m_dsbuffer( nullptr ),
			m_frequency( 0 ),
			m_volume( 1.0f ),
			m_pan( 0.0f )
		{

		}

		~DirectSoundBufferWrapper()
		{
			release();
		}

	public:
		inline HRESULT GetCaps( DSBCAPS* pDSBufferCaps ) const
		{
			if ( m_dsbuffer == nullptr )
			{
				LOG( "DirectSouondBuffer is NULL." );
				return DSERR_UNINITIALIZED;
			}

			HRESULT result = m_dsbuffer->GetCaps( pDSBufferCaps );
			validateResult( result );
			return result;
		}

		inline HRESULT GetCurrentPosition( DWORD* pdwCurrentPlayCursor, DWORD* pdwCurrentWriteCursor ) const
		{
			if ( m_dsbuffer == nullptr )
			{
				LOG( "DirectSouondBuffer is NULL." );
				return DSERR_UNINITIALIZED;
			}

			HRESULT result = m_dsbuffer->GetCurrentPosition( pdwCurrentPlayCursor, pdwCurrentWriteCursor );
			validateResult( result );
			return result;
		}

		inline HRESULT GetFormat( WAVEFORMATEX* pwfxFormat, DWORD dwSizeAllocated, DWORD* pdwSizeWritten ) const
		{
			if ( m_dsbuffer == nullptr )
			{
				LOG( "DirectSouondBuffer is NULL." );
				return DSERR_UNINITIALIZED;
			}

			HRESULT result = m_dsbuffer->GetFormat( pwfxFormat, dwSizeAllocated, pdwSizeWritten );
			validateResult( result );
			return result;
		}

		inline HRESULT GetStatus( DWORD* pdwStatus ) const
		{
			if ( m_dsbuffer == nullptr )
			{
				LOG( "DirectSouondBuffer is NULL." );
				return DSERR_UNINITIALIZED;
			}

			HRESULT result = m_dsbuffer->GetStatus( pdwStatus );
			validateResult( result );
			return result;
		}

		inline HRESULT Lock( DWORD dwOffset, DWORD dwBytes, VOID** ppvAudioPtr1, DWORD* pdwAudioBytes1, VOID** ppvAudioPtr2, DWORD* pdwAudioBytes2, DWORD dwFlags )
		{
			if ( m_dsbuffer == nullptr )
			{
				LOG( "DirectSouondBuffer is NULL." );
				return DSERR_UNINITIALIZED;
			}

			HRESULT result = m_dsbuffer->Lock( dwOffset, dwBytes, ppvAudioPtr1, pdwAudioBytes1, ppvAudioPtr2, pdwAudioBytes2, dwFlags );
			validateResult( result );
			return result;
		}

		inline HRESULT Play( DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags )
		{
			if ( m_dsbuffer == nullptr )
			{
				LOG( "DirectSouondBuffer is NULL." );
				return DSERR_UNINITIALIZED;
			}

			HRESULT result = m_dsbuffer->Play( dwReserved1, dwPriority, dwFlags );
			validateResult( result );
			return result;
		}

		inline HRESULT SetCurrentPosition( DWORD dwNewPosition )
		{
			if ( m_dsbuffer == nullptr )
			{
				LOG( "DirectSouondBuffer is NULL." );
				return DSERR_UNINITIALIZED;
			}

			HRESULT result = m_dsbuffer->SetCurrentPosition( dwNewPosition );
			validateResult( result );
			return result;
		}

		inline HRESULT SetFormat( const WAVEFORMATEX* pcfxFormat )
		{
			if ( m_dsbuffer == nullptr )
			{
				LOG( "DirectSouondBuffer is NULL." );
				return DSERR_UNINITIALIZED;
			}

			HRESULT result = m_dsbuffer->SetFormat( pcfxFormat );
			validateResult( result );
			return result;
		}

		inline HRESULT Stop()
		{
			if ( m_dsbuffer == nullptr )
			{
				LOG( "DirectSouondBuffer is NULL." );
				return DSERR_UNINITIALIZED;
			}

			HRESULT result = m_dsbuffer->Stop();
			validateResult( result );
			return result;
		}

		inline HRESULT Unlock( VOID* pvAudioPtr1, DWORD dwAudioBytes1, VOID* pvAudioPtr2, DWORD dwAudioBytes2 )
		{
			if ( m_dsbuffer == nullptr )
			{
				LOG( "DirectSouondBuffer is NULL." );
				return DSERR_UNINITIALIZED;
			}

			HRESULT result = m_dsbuffer->Unlock( pvAudioPtr1, dwAudioBytes1, pvAudioPtr2, dwAudioBytes2 );
			validateResult( result );
			return result;
		}

		inline HRESULT Restore()
		{
			if ( m_dsbuffer == nullptr )
			{
				LOG( "DirectSouondBuffer is NULL." );
				return DSERR_UNINITIALIZED;
			}

			HRESULT result = m_dsbuffer->Restore();
			validateResult( result );
			return result;
		}

	public:
		bool initialize( IDirectSound* directsound, unsigned long size, WAVEFORMATEX* waveformatex, bool global = true );

		inline bool wrap( IDirectSoundBuffer* dsbuffer )
		{
			if ( m_dsbuffer != nullptr || dsbuffer == nullptr )
				return false;

			m_dsbuffer = dsbuffer;

			//�������O�ɐݒ肳��Ă����p�����[�^��K�p
			if ( m_frequency > 0 )
				setFrequency( m_frequency );
			else
			{
				if ( m_dsbuffer->GetFrequency( &m_frequency ) != DS_OK )
				{
					//���̂����g�������Ȃ��ꍇ
					m_dsbuffer = nullptr;
					return false;
				}
			}
			setVolume( m_volume );
			setPan( m_pan );

			return true;
		}

		inline void release()
		{
			if ( m_dsbuffer != nullptr )
				m_dsbuffer->Release();
			m_dsbuffer = nullptr;

			m_frequency = 0;
			m_volume = 1.0f;
			m_pan = 0.0f;
		}

		inline double getVolume() const
		{
			return m_volume;
		}

		inline double getPan() const
		{
			return m_pan;
		}

		inline long getFrequency() const
		{
			return m_frequency;
		}

		void setVolume( double volume );

		void setPan( double pan );

		inline void setFrequency( unsigned long frequency )
		{
			if ( frequency == 0 )
				return;

			m_frequency = frequency;
			if ( m_dsbuffer != nullptr )
				m_dsbuffer->SetFrequency( frequency );
		}

	private:
		/// DirectSound�֘A�̊֐��Ăяo���ɂ��߂�l�����؂�,
		/// �G���[���������Ă���΂��̓��e��\������.
		/// ���p: http://msdn.microsoft.com/ja-jp/library/ee416775.aspx
		inline static void validateResult( HRESULT result )
		{
#ifdef _DEBUG
			if ( result == DS_OK )
				return;

			switch ( result )
			{
			case DS_NO_VIRTUALIZATION:
				LOG( "�o�b�t�@�[���쐬����܂�����, �ʂ� 3D �A���S���Y��������Ɏg�p����܂���. " );
				break;

				/*
				case DS_INCOMPLETE:
				LOG("���\�b�h�͐������܂�����, ���ׂẴI�v�V���� �G�t�F�N�g���擾���ꂽ�킯�ł͂���܂���. 	");
				break;
				*/

			case DSERR_ACCESSDENIED:
				LOG( "�A�N�Z�X�����ۂ��ꂽ����, �v���͎��s���܂���. " );
				break;

			case DSERR_ALLOCATED:
				LOG( "�D�惌�x���Ȃ�, ���\�[�X�����ɕʂ̌Ăяo�����ɂ��g�p����Ă��邽��, �v���͎��s���܂���. 	" );
				break;

			case DSERR_ALREADYINITIALIZED:
				LOG( "�I�u�W�F�N�g�͊��ɏ���������Ă��܂�. " );
				break;

			case DSERR_BADFORMAT:
				LOG( "�w�肳�ꂽ Wave �t�H�[�}�b�g�̓T�|�[�g����Ă��܂���. " );
				break;

			case DSERR_BADSENDBUFFERGUID:
				LOG( "�I�[�f�B�I�p�X �t�@�C���Ŏw�肳�ꂽ GUID ���L���ȃ~�b�N�X�C�� �o�b�t�@�[�ƈ�v���܂���. " );
				break;

			case DSERR_BUFFERLOST:
				LOG( "�o�b�t�@�[ �������[�������܂���. ��������K�v������܂�. " );
				break;

			case DSERR_BUFFERTOOSMALL:
				LOG( "�o�b�t�@�[ �T�C�Y�ɂ�, �G�t�F�N�g���������s���邽�߂̏\���ȑ傫��������܂���. " );
				break;

			case DSERR_CONTROLUNAVAIL:
				LOG( "�Ăяo�����ɂ��v�����ꂽ�o�b�t�@�[ �R���g���[�� (�{�����[��, �p���Ȃ�) �𗘗p�ł��܂���. �R���g���[����, �o�b�t�@�[�̍쐬���� DSBUFFERDESC �� dwFlags �����o�[���g�p���Ďw�肷��K�v������܂�. 	" );
				break;

			case DSERR_DS8_REQUIRED:
				LOG( "�v�����ꂽ�@�\�ɂ�, CLSID_DirectSound8 �N���X�ȍ~�� DirectSound �I�u�W�F�N�g���K�v�ł�. �ڍׂɂ��Ă�, IDirectSound8 �C���^�[�t�F�C�X���Q�Ƃ��Ă�������. " );
				break;

			case DSERR_FXUNAVAILABLE:
				LOG( "�v�����ꂽ�G�t�F�N�g��, �V�X�e���Ō�����Ȃ�������, �������Ԉ���Ă��邩, �Ԉ�����ꏊ�ɂ���܂�. ���Ƃ���, �n�[�h�E�F�A�ɂ��邱�Ƃ��z�肳���G�t�F�N�g���\�t�g�E�F�A�Ō�����܂���. " );
				break;

			case DSERR_GENERIC:
				LOG( "DirectSound �T�u�V�X�e�����Ō����s���̃G���[���������܂���. " );
				break;

			case DSERR_INVALIDCALL:
				LOG( "���̊֐���, ���̃I�u�W�F�N�g�̌��݂̏�Ԃɑ΂��ėL���ł͂���܂���. " );
				break;

			case DSERR_INVALIDPARAM:
				LOG( "�߂�֐��ɖ����ȃp�����[�^�[���n����܂���. " );
				break;

			case DSERR_NOAGGREGATION:
				LOG( "�I�u�W�F�N�g�͏W�v���T�|�[�g���Ă��܂���. " );
				break;

			case DSERR_NODRIVER:
				LOG( "�g�p�ł���T�E���h �h���C�o�[������܂���. �܂���, �w�肳�ꂽ GUID �͗L���� DirectSound �f�o�C�X ID �ł͂���܂���. " );
				break;

			case DSERR_NOINTERFACE:
				LOG( "�v�����ꂽ COM �C���^�[�t�F�C�X�͎g�p�ł��܂���. " );
				break;

			case DSERR_OBJECTNOTFOUND:
				LOG( "�v�����ꂽ�I�u�W�F�N�g��������܂���ł���. " );
				break;

			case DSERR_OTHERAPPHASPRIO:
				LOG( "�ʂ̃A�v���P�[�V�����������D�惌�x���������Ă��邽��, ���̌Ăяo���𑱍s�ł��܂���. " );
				break;

			case DSERR_OUTOFMEMORY:
				LOG( "DirectSound �T�u�V�X�e����, �Ăяo�����̗v���������ł��邾���̏\���ȃ������[�����蓖�Ă邱�Ƃ��ł��܂���ł���. " );
				break;

			case DSERR_PRIOLEVELNEEDED:
				LOG( "DSSCL_PRIORITY �Ɠ����ȏ�̋������x�����K�v�ł�. " );
				break;

			case DSERR_SENDLOOP:
				LOG( "�G�t�F�N�g�̑��M�̏z���[�v�����o���܂���. " );
				break;

			case DSERR_UNINITIALIZED:
				LOG( "IDirectSound8::Initialize ���\�b�h���Ăяo����Ȃ�������, �ʂ̃��\�b�h�̌Ăяo���O��, ���̃��\�b�h�̌Ăяo���Ɏ��s���܂���. " );
				break;

			case DSERR_UNSUPPORTED:
				LOG( "�Ăяo���ꂽ�֐��͂��̎��_�ł̓T�|�[�g����Ă��܂���. " );
				break;

			default:
				LOG( "�����s���̃G���[. HRESULT = 0x%X", result );
				break;
			}
#endif
		}
	};

};