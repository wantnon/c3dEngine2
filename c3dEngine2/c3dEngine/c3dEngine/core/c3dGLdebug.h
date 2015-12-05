//
//  c3dGLdebug.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-27.
//
//

#ifndef __HelloOpenGL__c3dGLdebug__
#define __HelloOpenGL__c3dGLdebug__

#include <iostream>
using namespace std;
#include "core/c3dLog.h"
#include "common/c3dGL.h"
#if DEBUG == 1
#define C3DCHECK_GL_ERROR_DEBUG() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            c3dLog("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)
#else
#define C3DCHECK_GL_ERROR_DEBUG()  void(0)
#endif
#endif /* defined(__HelloOpenGL__c3dGLdebug__) */
