#include "CriticalLock.h"

////////////////////////////////////////////////////////////////////////////////
// CriticalLock
////////////////////////////////////////////////////////////////////////////////

CriticalLock::CriticalLock()
{
	lockhandle = CreateMutex( 0, FALSE, "VMRSurfaceAllocator" );
}

CriticalLock::~CriticalLock( void )
{
	CloseHandle( lockhandle );
}

unsigned long CriticalLock::Lock( unsigned long timeout )
{
	return WaitForSingleObjectEx( lockhandle, timeout, false );
}

int CriticalLock::Unlock( void )
{
	return ReleaseMutex( lockhandle );
}
