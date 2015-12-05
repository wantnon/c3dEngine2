//
//  c3dInitGame.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#ifndef __HelloOpenGL__c3dInitGame__
#define __HelloOpenGL__c3dInitGame__

#include <iostream>
using namespace std;
//
#include "core/c3dSceneManger.h"
#include "core/c3dAutoreleasePool.h"
#include "core/c3dDeviceAndOSInfo.h"
#include "core/c3dAppState.h"
#include "core/c3dTextureCache.h"
#include "core/c3dAudioCache.h"
#include "core/c3dGlobalTimer.h"
#include "core/c3dAssert.h"
#include "shader/c3dProgramSetUp.h"
//
void initOpenGL();
void initOpenAL();
void teardownOpenAL();
void c3dInitGame();
#endif /* defined(__HelloOpenGL__c3dInitGame__) */
