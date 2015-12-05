//
//  c3dObject.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-13.
//
//

#include "c3dObject.h"
#include "c3dAutoreleasePool.h"
void Cc3dObject::autorelease(){
    //add to autorelease pool
    Cc3dAutoreleasePool::sharedAutoreleasePool()->addObject(this);
}
