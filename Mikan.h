#ifndef USEMIKANLIBRARY
#define USEMIKANLIBRARY

//#define MIKAN_NO_SOUND

#include "MikanLibrary.h"

#ifndef _UNLINK_LIBRARY
//公開クラス
extern CLASSDEF_MIKAN_SYSTEM        *MikanSystem;
extern CLASSDEF_MIKAN_WINDOW        *MikanWindow;
extern CLASSDEF_MIKAN_DIRECTX_DRAW  *MikanDraw;
extern CLASSDEF_MIKAN_MIKANMOVIE    *MikanMovie;
extern CLASSDEF_MIKAN_DIRECTX_INPUT *MikanInput;
#ifndef MIKAN_NO_SOUND
extern CLASSDEF_MIKAN_DIRECTX_SOUND *MikanSound;
#endif
extern CLASSDEF_MIKAN_NETWORK       *MikanNet;
extern CLASSDEF_MIKAN_FILE          *MikanFile;
//非公開クラス
extern CLASSDEF_SYSTEM              *_MikanSystem;
extern CLASSDEF_WINDOW              *_MikanWindow;
extern CLASSDEF_DIRECTX_DRAW        *_MikanDraw;
extern CLASSDEF_MIKANMOVIE          *_MikanMovie;
extern CLASSDEF_DIRECTX_INPUT       *_MikanInput;
#ifndef MIKAN_NO_SOUND
extern CLASSDEF_DIRECTX_SOUND       *_MikanSound;
#endif
extern CLASSDEF_NETWORK             *_MikanNet;
extern CLASSDEF_FILEIO_MANAGEMENT   *_MikanFile;
#endif
#endif
