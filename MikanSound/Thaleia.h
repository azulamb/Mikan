#pragma once

#ifdef _MIKANDEV
#ifdef USEMIKANLIBRARY
#include "Dev/IFile.h"
#include "Dev/IAudioHandler.h"
#include "Dev/IVoice.h"
#include "Dev/functions.h"
#else
#include "IFile.h"
#include "IAudioHandler.h"
#include "IVoice.h"
#include "functions.h"
#endif
#else
#include "IFile.h"
#include "IAudioHandler.h"
#include "IVoice.h"
#include "functions.h"
#endif
