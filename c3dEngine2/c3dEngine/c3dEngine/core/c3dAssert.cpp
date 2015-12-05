//
//  c3dAssert.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-9.
//
//

#include "c3dAssert.h"
void C3DASSERT(bool cond, const string&msg){
    if (!cond) {
        C3DLOG("Assert failed: %s", msg.c_str());
        assert(cond);
    }
}