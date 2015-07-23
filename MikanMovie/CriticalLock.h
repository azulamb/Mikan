#ifndef CLASSDEF_CRITICALLOCK
#define CLASSDEF_CRITICALLOCK

#include "MikanMovie.h"

class CriticalLock
{
private:
	HANDLE lockhandle;
public:
	CriticalLock();
	virtual ~CriticalLock( void );
	virtual unsigned long Lock( unsigned long timeout = INFINITE );
	virtual int Unlock( void );
};

#endif