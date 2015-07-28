#include <process.h>

#include "Thread.h"

namespace Thaleia
{

	//----------------------------------------------------------
	void Thread::staticProcedure( void* pointer )
	{
		// ���Ȃ̌^�ɃL���X�g
		auto self = (Thread*)pointer;

		// �I�[�o�[���C�h�\�ȃv���V�[�W�����Ăяo��
		self->threadProcedure();

		// �n���h�����������.
		auto cs = self->threadLock();
		self->m_thread_handle = nullptr;
	}

	//----------------------------------------------------------
	Thread::CriticalSection Thread::threadLock()
	{
		// �N���e�B�J���Z�N�V�����ɐi��
		EnterCriticalSection( &m_critical_section );

		// �j�����ꂽ�ۂɃN���e�B�J���Z�N�V��������ޏo����悤�ȃI�u�W�F�N�g��Ԃ�.
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
		// �n���h���������Ȃ�΂��̂܂܏I��
		if ( m_thread_handle == nullptr )
			return false;

		//
		DWORD result = WaitForSingleObject( m_thread_handle, time );
		switch ( result )
		{
		case WAIT_TIMEOUT:  // �܂��I�����Ă��Ȃ�
			return true;

		case WAIT_FAILED:   // WaitForSingleObject�����s����
		case WAIT_OBJECT_0: // �X���b�h���I������
			return false;

		default: //�s��
			return false;
		}
	}

	bool Thread::threadRun()
	{
		// �X���b�h���������Ă���Ȃ�Ύ��s
		if ( threadAlive() )
			return false;

		// �X���b�h�쐬
		m_thread_handle = (HANDLE)_beginthread( staticProcedure, 0, ( void* )this );
		if ( m_thread_handle == nullptr )
			return false;

		// ����
		return true;
	}

};