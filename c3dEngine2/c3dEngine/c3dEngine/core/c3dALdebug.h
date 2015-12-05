//
//  c3dALdebug.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__c3dALdebug__
#define __HelloOpenGL__c3dALdebug__

#include <iostream>
#include <string>
using namespace std;
#include "core/c3dLog.h"
#include "common/c3dAL.h"

string errorTranslate(ALenum error);
#if DEBUG == 1
#define C3DCHECK_AL_ERROR_DEBUG() \
do { \
ALenum __error = alGetError(); \
if(__error) { \
string __errorStr=errorTranslate(__error); \
c3dLog("OpenAL error 0x%04X (%s) in %s %s %d\n", __error, __errorStr.c_str(), __FILE__, __FUNCTION__, __LINE__); \
} \
} while (false)
#else
#define C3DCHECK_AL_ERROR_DEBUG()  void(0)
#endif
#endif /* defined(__HelloOpenGL__c3dALdebug__) */