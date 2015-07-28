#pragma once

#define _MEMORY_CHECK

#ifdef _DEBUG

//�f�o�b�O�o�͊֘A�C���N���[�h
#include <windows.h>
#include <string>
#include <tchar.h>

//�f�o�b�O�o�͊֐�
void DebugLog( LPCTSTR pszFileName, int nLine, LPCTSTR pszFormat, ... );
#define LOG(fmt, ...) DebugLog(_T(__FILE__), __LINE__, _T(fmt), __VA_ARGS__)

#ifdef _MEMORY_CHECK

//���������[�N�֘A�C���N���[�h
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <new>
#include <crtdbg.h>

//�������m�ۂ̊Ď�
#define new                ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s)          _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define calloc(c, s)       _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define realloc(p, s)      _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _recalloc(p, c, s) _recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _expand(p, s)      _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)

//�ݒ�֐�
void SetDebugReport();
#define SET_DEBUG_REPORT() SetDebugReport()

#else //_MEMORY_CHECK

#define SET_DEBUG_REPORT()

#endif //_MEMORY_CHECK

#else //_DEBUG

#define LOG(fmt, ...)
#define SET_DEBUG_REPORT()

#endif //_DEBUG
