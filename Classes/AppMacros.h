#ifndef __APPMACROS_H__
#define __APPMACROS_H__
#include "cocos2d.h"

#define ANDROID_MI3
#ifdef ANDROID_MI3
#define		SCALE_FACTOR	0.625f
#define		WINSIZE_W		1920
#define		WINSIZE_H		1080
#else
#define		SCALE_FACTOR		1.f
#define		WINSIZE_W			1200
#define		WINSIZE_H			675
#endif

#endif
