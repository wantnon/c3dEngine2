//
//  c3dTextureCache.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//
#include <string>
#include <iostream>
using namespace std;
#include "c3dTextureCache.h"
#include "c3dGLdebug.h"

static Cc3dTextureCache*s_textureCache=NULL;
Cc3dTextureCache* Cc3dTextureCache::sharedTextureCache(){
    if(s_textureCache==NULL){
        s_textureCache=new Cc3dTextureCache();
    }
    return s_textureCache;
}