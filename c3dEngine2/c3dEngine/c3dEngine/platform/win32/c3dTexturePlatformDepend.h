//
//  c3dTexturePlatformDepend.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-9.
//
//

#ifndef __HelloOpenGL__c3dTexturePlatformDepend__
#define __HelloOpenGL__c3dTexturePlatformDepend__

#include <iostream>
using namespace std;
#include "core/c3dGLdebug.h"
#include "c3dImage.h"
#include "core/c3dDefine.h"
GLuint createGLTexture_plat(const string&filePath,int wrapS,int wrapT,GLint minFilter,GLint magFilter,
                           float&textureWidth,float&textureHeight);

unsigned char* getImageData_plat(const string&filePath,CCTexture2DPixelFormat&_pixelFormat,float&_imageWidth,float&_imageHeight);
#endif /* defined(__HelloOpenGL__c3dTexturePlatformDepend__) */
