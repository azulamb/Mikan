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

			//初期化前に設定されていたパラメータを適用
			if ( m_frequency > 0 )
				setFrequency( m_frequency );
			else
			{
				if ( m_dsbuffer->GetFrequency( &m_frequency ) != DS_OK )
				{
					//何故か周波数が取れない場合
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
		/// DirectSound関連の関数呼び出しによる戻り値を検証し,
		/// エラーが発生していればその内容を表示する.
		/// 引用: http://msdn.microsoft.com/ja-jp/library/ee416775.aspx
		inline static void validateResult( HRESULT result )
		{
#ifdef _DEBUG
			if ( result == DS_OK )
				return;

			switch ( result )
			{
			case DS_NO_VIRTUALIZATION:
				LOG( "バッファーが作成されましたが, 別の 3D アルゴリズムが代わりに使用されました. " );
				break;

				/*
				case DS_INCOMPLETE:
				LOG("メソッドは成功しましたが, すべてのオプション エフェクトが取得されたわけではありません. 	");
				break;
				*/

			case DSERR_ACCESSDENIED:
				LOG( "アクセスが拒否されたため, 要求は失敗しました. " );
				break;

			case DSERR_ALLOCATED:
				LOG( "優先レベルなど, リソースが既に別の呼び出し元により使用されているため, 要求は失敗しました. 	" );
				break;

			case DSERR_ALREADYINITIALIZED:
				LOG( "オブジェクトは既に初期化されています. " );
				break;

			case DSERR_BADFORMAT:
				LOG( "指定された Wave フォーマットはサポートされていません. " );
				break;

			case DSERR_BADSENDBUFFERGUID:
				LOG( "オーディオパス ファイルで指定された GUID が有効なミックスイン バッファーと一致しません. " );
				break;

			case DSERR_BUFFERLOST:
				LOG( "バッファー メモリーが失われました. 復元する必要があります. " );
				break;

			case DSERR_BUFFERTOOSMALL:
				LOG( "バッファー サイズには, エフェクト処理を実行するための十分な大きさがありません. " );
				break;

			case DSERR_CONTROLUNAVAIL:
				LOG( "呼び出し元により要求されたバッファー コントロール (ボリューム, パンなど) を利用できません. コントロールは, バッファーの作成時に DSBUFFERDESC の dwFlags メンバーを使用して指定する必要があります. 	" );
				break;

			case DSERR_DS8_REQUIRED:
				LOG( "要求された機能には, CLSID_DirectSound8 クラス以降の DirectSound オブジェクトが必要です. 詳細については, IDirectSound8 インターフェイスを参照してください. " );
				break;

			case DSERR_FXUNAVAILABLE:
				LOG( "要求されたエフェクトは, システムで見つからなかったか, 順序が間違っているか, 間違った場所にあります. たとえば, ハードウェアにあることが想定されるエフェクトがソフトウェアで見つかりました. " );
				break;

			case DSERR_GENERIC:
				LOG( "DirectSound サブシステム内で原因不明のエラーが発生しました. " );
				break;

			case DSERR_INVALIDCALL:
				LOG( "この関数は, このオブジェクトの現在の状態に対して有効ではありません. " );
				break;

			case DSERR_INVALIDPARAM:
				LOG( "戻り関数に無効なパラメーターが渡されました. " );
				break;

			case DSERR_NOAGGREGATION:
				LOG( "オブジェクトは集計をサポートしていません. " );
				break;

			case DSERR_NODRIVER:
				LOG( "使用できるサウンド ドライバーがありません. または, 指定された GUID は有効な DirectSound デバイス ID ではありません. " );
				break;

			case DSERR_NOINTERFACE:
				LOG( "要求された COM インターフェイスは使用できません. " );
				break;

			case DSERR_OBJECTNOTFOUND:
				LOG( "要求されたオブジェクトが見つかりませんでした. " );
				break;

			case DSERR_OTHERAPPHASPRIO:
				LOG( "別のアプリケーションが高い優先レベルを持っているため, この呼び出しを続行できません. " );
				break;

			case DSERR_OUTOFMEMORY:
				LOG( "DirectSound サブシステムは, 呼び出し元の要求を完了できるだけの十分なメモリーを割り当てることができませんでした. " );
				break;

			case DSERR_PRIOLEVELNEEDED:
				LOG( "DSSCL_PRIORITY と同等以上の協調レベルが必要です. " );
				break;

			case DSERR_SENDLOOP:
				LOG( "エフェクトの送信の循環ループを検出しました. " );
				break;

			case DSERR_UNINITIALIZED:
				LOG( "IDirectSound8::Initialize メソッドが呼び出されなかったか, 別のメソッドの呼び出し前に, このメソッドの呼び出しに失敗しました. " );
				break;

			case DSERR_UNSUPPORTED:
				LOG( "呼び出された関数はこの時点ではサポートされていません. " );
				break;

			default:
				LOG( "原因不明のエラー. HRESULT = 0x%X", result );
				break;
			}
#endif
		}
	};

};