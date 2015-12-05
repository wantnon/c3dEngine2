//
//  c3d.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-13.
//
//

#ifndef __HelloOpenGL__c3d__
#define __HelloOpenGL__c3d__

#include "common/c3dAL.h"
#include "common/c3dGL.h"
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
#include "platform/win32/appFrame/mainWindowGlobal.h"
#include "platform/win32/appFrame/mainWindowFuncs.h"
#else
//#include "platform/iOS/appFrame/mainWindowGlobal.h"
#include "platform/iOS/appFrame/mainWindowFuncs.h"
#endif
#include "core/c3dGlobalTimer.h"
#include "core/c3dDeviceAndOSInfo.h"
#include "core/c3dActor.h"
#include "core/c3dSceneManger.h"
#include "core/c3dAudioCache.h"
#include "core/c3dDocuments.h"
#include "core/c3dFileUtils.h"
#include "core/c3dFbo.h"
#include "core/c3dProgramCache.h"
#include "core/c3dTextureCache.h"
#include "shader/c3dPassUnifoCallBack.h"
#include "shader/c3dProgramSetUp.h"

#endif /* defined(__HelloOpenGL__c3d__) */
