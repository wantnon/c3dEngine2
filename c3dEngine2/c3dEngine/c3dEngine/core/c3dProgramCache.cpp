//
//  c3dProgramCache.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-16.
//
//

#include "c3dProgramCache.h"
static Cc3dProgramCache*programCache=NULL;
Cc3dProgramCache*Cc3dProgramCache::sharedProgramCache(){
    if(programCache==NULL){
        programCache=new Cc3dProgramCache();
    }
    return programCache;
}