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
		CRITICAL_SECTION m_critical_section;	/// �N���e�B�J���Z�N�V�����I�u�W�F�N�g
		HANDLE m_thread_handle;					/// �X���b�h�n���h��

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
		/// �N���e�B�J���Z�N�V�����ɐi������Ƌ���, 
		/// �j�����ꂽ�ۂɃN���e�B�J���Z�N�V��������ޏo����悤�ȃI�u�W�F�N�g�𓾂�.
		/// auto cs = lock(); �̂悤�Ɏg�p����.
		/// �����I�ɃN���e�B�J���Z�N�V��������ޏo�������ꍇ�� cs.reset() ����.
		/// �Ȃ�, cs.release() ���g�p����ƃN���e�B�J���Z�N�V��������ޏo�ł��Ȃ��Ȃ�, 
		/// �f�b�h���b�N�ɂȂ�̂Œ���.
		CriticalSection threadLock();

		/// �X���b�h���������Ă��邩���ׂ�.
		/// �܂���, �X���b�h�̏I����҂�.
		///
		/// time	�X���b�h���������Ă���ꍇ�ɏI����҂���(�~���b).
		///			0���w�肷��Ƃ��̏u�Ԃ̃X���b�h�̐����󋵂݂̂��m�F����.
		bool threadAlive( unsigned long time = 0 );

		//
		bool threadRun();
	};

};
