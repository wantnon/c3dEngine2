//
//  c3dGLPlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-6.
//
//

#include "platform/win32/c3dGLPlatformDepend.h"
#include "core/c3dAssert.h"
#include "core/c3dGLdebug.h"




 void initOpenGL_plat(){
	 

    GLenum GlewInitResult = glewInit();//must call after the window created 
    if (GLEW_OK != GlewInitResult)
    {
		c3dLog((char *)glewGetErrorString(GlewInitResult));//, "OpenGL error");
        C3DASSERT(false);
    }
	glGetError();//clear error bit


 }