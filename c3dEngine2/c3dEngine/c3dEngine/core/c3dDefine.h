//
//  c3dDefine.h
//  HelloCpp
//
//  Created by Yang Chao (wantnon) on 14-1-2.
//
//

#ifndef __HelloCpp__c3dDefine__
#define __HelloCpp__c3dDefine__
//mask some warning
#pragma warning (disable:4996)
#pragma warning (disable:4819)
#pragma warning (disable:4183)
#pragma warning (disable:4786)
#pragma warning (disable:4804)
#pragma warning (disable:4800)

#include <iostream>
#include <string>
using namespace std;
#include <limits>
#include "common/c3dGL.h"

#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32) 
	#define C3DFINAL  //vs2010 not support final key word, so just ignor
#else
    #define C3DFINAL final
#endif

#define USED_FOR_SUBMIT_TO_VBO C3DFINAL //if used for submit to vbo, it must not contain virtual member function, so it's destructor should not be virtual, so we mark it final


enum Ec3dProjectionMode{
    ec3dOrthographicMode = 0,
    ec3dPerspectiveMode,
};

enum Ec3dCullFace{
    ec3dNotCull=0,
    ec3dCullFront,
    ec3dCullBack,
    
};


const float c3d_PI=3.14159265358979323846264338327950288;
#ifndef M_PI
#define M_PI c3d_PI
#endif

#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32) 
	const float c3d_INF=0.5*(numeric_limits<float>::max)();
#else
    const float c3d_INF=0.5*numeric_limits<float>::max();//????????????????<>????float???????const float
#endif





//??????????????????xcode???????(????GLKit)?????
const GLint ATTRIB_LOC_position=0;
const GLint ATTRIB_LOC_normal=1;
const GLint ATTRIB_LOC_color=2;
const GLint ATTRIB_LOC_texCoord=3;
const GLint ATTRIB_LOC_texCoord2=4;



typedef enum {

    //! 32-bit texture: RGBA8888
    kCCTexture2DPixelFormat_RGBA8888,
    //! 24-bit texture: RGBA888
    kCCTexture2DPixelFormat_RGB888,
    //! 16-bit texture without Alpha channel
    kCCTexture2DPixelFormat_RGB565,
    //! 8-bit textures used as masks
    kCCTexture2DPixelFormat_A8,
    //! 8-bit intensity texture
    kCCTexture2DPixelFormat_I8,
    //! 16-bit textures used as masks
    kCCTexture2DPixelFormat_AI88,
    //! 16-bit textures: RGBA4444
    kCCTexture2DPixelFormat_RGBA4444,
    //! 16-bit textures: RGB5A1
    kCCTexture2DPixelFormat_RGB5A1,    
    //! 4-bit PVRTC-compressed texture: PVRTC4
    kCCTexture2DPixelFormat_PVRTC4,
    //! 2-bit PVRTC-compressed texture: PVRTC2
    kCCTexture2DPixelFormat_PVRTC2,


    //! Default texture format: RGBA8888
    kCCTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_RGBA8888,

    // backward compatibility stuff
    kTexture2DPixelFormat_RGBA8888 = kCCTexture2DPixelFormat_RGBA8888,
    kTexture2DPixelFormat_RGB888 = kCCTexture2DPixelFormat_RGB888,
    kTexture2DPixelFormat_RGB565 = kCCTexture2DPixelFormat_RGB565,
    kTexture2DPixelFormat_A8 = kCCTexture2DPixelFormat_A8,
    kTexture2DPixelFormat_RGBA4444 = kCCTexture2DPixelFormat_RGBA4444,
    kTexture2DPixelFormat_RGB5A1 = kCCTexture2DPixelFormat_RGB5A1,
    kTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_Default

} CCTexture2DPixelFormat;

#endif /* defined(__HelloCpp__c3dDefine__) */
