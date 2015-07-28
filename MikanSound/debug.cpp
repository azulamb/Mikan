#ifdef _DEBUG

#include "debug.h"

void DebugLog( LPCTSTR pszFileName, int nLine, LPCTSTR pszFormat, ... )
{
	_TCHAR fmt[ 1024 ];
	_sntprintf_s( fmt, 1024, _T( "%s %d - %s\n" ), pszFileName, nLine, pszFormat );

	_TCHAR buf[ 1024 ];
	va_list ap;
	va_start( ap, pszFormat );
	_vsntprintf_s( buf, 1024, fmt, ap );
	va_end( ap );
	OutputDebugString( buf );
}

#ifdef _MEMORY_CHECK

void SetDebugReport()
{
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG );
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
}

#endif //_MEMORY_CHECK

#endif //_DEBUG