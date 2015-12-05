//
//  touchSequence.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-21.
//
//

#include "touchSequence.h"
static CtouchSequence*s_touchSequence=NULL;
CtouchSequence*CtouchSequence::sharedTouchSequence(){
    if(s_touchSequence==NULL){
        s_touchSequence=new CtouchSequence();
    }
    return s_touchSequence;
}