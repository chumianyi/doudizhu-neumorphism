#ifndef __APPMACROS_H__
#define __APPMACROS_H__
#include "cocos2d.h"

#define ANDROID_MODERN
#ifdef ANDROID_MODERN
#define		SCALE_FACTOR	1.0f
#define		WINSIZE_W		1920
#define		WINSIZE_H		1080
#else
#define		SCALE_FACTOR		1.f
#define		WINSIZE_W			1200
#define		WINSIZE_H			675
#endif

#endif
