//
//  c3dAL.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-13.
//
//

#ifndef __HelloOpenGL__c3dAL__
#define __HelloOpenGL__c3dAL__
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32) 
	#include "platform/win32/AL/al.h"
	#include "platform/win32/AL/alc.h"
	#include "platform/win32/AL/alut.h"
#else
    #include <OpenAL/al.h>
	#include <OpenAL/alc.h>
	#include <AudioToolbox/AudioToolbox.h>
#endif

#endif /* defined(__HelloOpenGL__c3dAL__) */
