#include <process.h>

#include "Thread.h"

namespace Thaleia
{

	//----------------------------------------------------------
	void Thread::staticProcedure( void* pointer )
	{
		// 自己の型にキャスト
		auto self = (Thread*)pointer;

		// オーバーライド可能なプロシージャを呼び出す
		self->threadProcedure();

		// ハンドルを解放する.
		auto cs = self->threadLock();
		self->m_thread_handle = nullptr;
	}

	//----------------------------------------------------------
	Thread::CriticalSection Thread::threadLock()
	{
		// クリティカルセクションに進入
		EnterCriticalSection( &m_critical_section );

		// 破棄された際にクリティカルセクションから退出するようなオブジェクトを返す.
		return
			Thread::CriticalSection
			(
				&m_critical_section,
				[]( CRITICAL_SECTION* cs )
		{
			LeaveCriticalSection( cs );
		}
		);
	}

	bool Thread::threadAlive( unsigned long time )
	{
		// ハンドルが無効ならばそのまま終了
		if ( m_thread_handle == nullptr )
			return false;

		//
		DWORD result = WaitForSingleObject( m_thread_handle, time );
		switch ( result )
		{
		case WAIT_TIMEOUT:  // まだ終了していない
			return true;

		case WAIT_FAILED:   // WaitForSingleObjectが失敗した
		case WAIT_OBJECT_0: // スレッドが終了した
			return false;

		default: //不明
			return false;
		}
	}

	bool Thread::threadRun()
	{
		// スレッドが生存しているならば失敗
		if ( threadAlive() )
			return false;

		// スレッド作成
		m_thread_handle = (HANDLE)_beginthread( staticProcedure, 0, ( void* )this );
		if ( m_thread_handle == nullptr )
			return false;

		// 完了
		return true;
	}

};