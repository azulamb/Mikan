#pragma once

#define _MEMORY_CHECK

#ifdef _DEBUG

//デバッグ出力関連インクルード
#include <windows.h>
#include <string>
#include <tchar.h>

//デバッグ出力関数
void DebugLog( LPCTSTR pszFileName, int nLine, LPCTSTR pszFormat, ... );
#define LOG(fmt, ...) DebugLog(_T(__FILE__), __LINE__, _T(fmt), __VA_ARGS__)

#ifdef _MEMORY_CHECK

//メモリリーク関連インクルード
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <new>
#include <crtdbg.h>

//メモリ確保の監視
#define new                ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s)          _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define calloc(c, s)       _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define realloc(p, s)      _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _recalloc(p, c, s) _recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _expand(p, s)      _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)

//設定関数
void SetDebugReport();
#define SET_DEBUG_REPORT() SetDebugReport()

#else //_MEMORY_CHECK

#define SET_DEBUG_REPORT()

#endif //_MEMORY_CHECK

#else //_DEBUG

#define LOG(fmt, ...)
#define SET_DEBUG_REPORT()

#endif //_DEBUG
