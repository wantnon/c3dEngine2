//
//  c3dModelMatStack.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-16.
//
//

#include "c3dModelMatStack.h"
static Cc3dModelMatStack*modelMatStack=NULL;
Cc3dModelMatStack* Cc3dModelMatStack::sharedModelMatStack(){
    if(modelMatStack==NULL){
        modelMatStack=new Cc3dModelMatStack();
    }
    return modelMatStack;
}