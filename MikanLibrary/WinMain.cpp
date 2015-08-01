#include "MikanLibrary.h"

extern CLASSDEF_SYSTEM *_MikanSystem;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow )
{

#ifdef _CRTDBG_MAP_ALLOC
	// 全てのレポート出力をウィンドウに送る
#ifdef _DEBUG
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG );
	// #else
	//  _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_WNDW );
	//  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_WNDW );
	//  _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_WNDW );
	// #endif
	// プログラムの最初でコールする
	//  _CrtSetDbgFlag( _CrtSetDbgFlag( 0 ) | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
#endif

	//クラスを宣言
	_MikanSystem = new MikanLibrary();
	if ( _MikanSystem )
	{
		_MikanSystem->WinMain( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
		delete( _MikanSystem );
		return 0;
	}

	return 1;
}
