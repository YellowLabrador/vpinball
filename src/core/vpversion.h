#pragma once

#include "git_version.h"

#define VP_VERSION_MAJOR    10 // X Digits
#define VP_VERSION_MINOR    8  // Max 2 Digits
#define VP_VERSION_REV      1  // Max 1 Digit

#define CURRENT_FILE_FORMAT_VERSION  1081
#define NO_ENCRYPTION_FORMAT_VERSION 1050
#define NEW_SOUND_FORMAT_VERSION     1031 // introduced surround option

#define _STR(x)    #x
#define STR(x)     _STR(x)

#define VP_VERSION_DIGITS VP_VERSION_MAJOR,VP_VERSION_MINOR,VP_VERSION_REV,GIT_REVISION
#define VP_VERSION_STRING_DIGITS STR(VP_VERSION_MAJOR) STR(VP_VERSION_MINOR) STR(VP_VERSION_REV) STR(GIT_REVISION)
#define VP_VERSION_STRING_POINTS STR(VP_VERSION_MAJOR) "." STR(VP_VERSION_MINOR) "." STR(VP_VERSION_REV) "." STR(GIT_REVISION)
#define VP_VERSION_STRING_COMMAS STR(VP_VERSION_MAJOR) ", " STR(VP_VERSION_MINOR) ", " STR(VP_VERSION_REV) ", " STR(GIT_REVISION)
#define VP_VERSION_STRING_POINTS_FULL STR(VP_VERSION_MAJOR) "." STR(VP_VERSION_MINOR) "." STR(VP_VERSION_REV) "." STR(GIT_REVISION) "." GIT_SHA

// Complete version string for log, crash handler,...
#if defined(ENABLE_OPENGL)
#ifndef __OPENGLES__
#define VP_VERSION_STRING_FULL_LITERAL "v" STR(VP_VERSION_MAJOR) "." STR(VP_VERSION_MINOR) "." STR(VP_VERSION_REV) \
	" Beta (Rev. " STR(GIT_REVISION) " (" GIT_SHA "), " GET_PLATFORM_OS " OpenGL " GET_PLATFORM_BITS "bits)"
#else
#define VP_VERSION_STRING_FULL_LITERAL "v" STR(VP_VERSION_MAJOR) "." STR(VP_VERSION_MINOR) "." STR(VP_VERSION_REV) \
	" Beta (Rev. " STR(GIT_REVISION) " (" GIT_SHA "), " GET_PLATFORM_OS " GLES " GET_PLATFORM_BITS "bits)"
#endif
#elif defined(ENABLE_BGFX)
#define VP_VERSION_STRING_FULL_LITERAL "v" STR(VP_VERSION_MAJOR) "." STR(VP_VERSION_MINOR) "." STR(VP_VERSION_REV) \
	" Beta (Rev. " STR(GIT_REVISION) " (" GIT_SHA "), " GET_PLATFORM_OS " BGFX " GET_PLATFORM_BITS "bits)"
#else
#define VP_VERSION_STRING_FULL_LITERAL "v" STR(VP_VERSION_MAJOR) "." STR(VP_VERSION_MINOR) "." STR(VP_VERSION_REV) \
	" Beta (Rev. " STR(GIT_REVISION) " (" GIT_SHA "), " GET_PLATFORM_OS " DirectX " GET_PLATFORM_BITS "bits)"
#endif

#define PASTE2(a,b) a##b
#define PASTE(a,b) PASTE2(a,b)
