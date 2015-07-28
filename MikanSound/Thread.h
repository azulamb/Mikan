#include <memory>
#include <functional>

#include <windows.h>


namespace Thaleia
{

	class Thread
	{
		//----------------------------------------------------------
		// Callback Method

	private:
		static void staticProcedure( void* pointer );

	protected:
		virtual void threadProcedure() = 0;

		//----------------------------------------------------------
		// Thread

		// private variable
	private:
		CRITICAL_SECTION m_critical_section;	/// クリティカルセクションオブジェクト
		HANDLE m_thread_handle;					/// スレッドハンドル

		// public constructor
	public:
		Thread() :
			m_thread_handle( nullptr )
		{
			InitializeCriticalSection( &m_critical_section );
		}

		virtual ~Thread()
		{
#ifndef _DEBUG
			threadAlive( -1 );
#else
			for ( int i = 0; threadAlive( 50 ); i++ )
				if ( i >= 20 )
					abort();
#endif

			DeleteCriticalSection( &m_critical_section );
		}

		// public type
	public:
		typedef std::unique_ptr<CRITICAL_SECTION, std::function<void( CRITICAL_SECTION* )>> CriticalSection;

		// public method
	public:
		/// クリティカルセクションに進入すると共に, 
		/// 破棄された際にクリティカルセクションから退出するようなオブジェクトを得る.
		/// auto cs = lock(); のように使用する.
		/// 明示的にクリティカルセクションから退出したい場合は cs.reset() する.
		/// なお, cs.release() を使用するとクリティカルセクションから退出できなくなり, 
		/// デッドロックになるので注意.
		CriticalSection threadLock();

		/// スレッドが生存しているか調べる.
		/// または, スレッドの終了を待つ.
		///
		/// time	スレッドが生存している場合に終了を待つ時間(ミリ秒).
		///			0を指定するとその瞬間のスレッドの生存状況のみを確認する.
		bool threadAlive( unsigned long time = 0 );

		//
		bool threadRun();
	};

};
