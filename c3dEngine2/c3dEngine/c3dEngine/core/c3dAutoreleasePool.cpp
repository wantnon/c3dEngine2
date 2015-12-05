//
//  c3dAutoreleasePool.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-13.
//
//

#include "c3dAutoreleasePool.h"

static Cc3dAutoreleasePool*autoreleasePool=NULL;
Cc3dAutoreleasePool*Cc3dAutoreleasePool::sharedAutoreleasePool(){
    if(autoreleasePool==NULL){
        autoreleasePool=new Cc3dAutoreleasePool();
    }
    return autoreleasePool;
}
