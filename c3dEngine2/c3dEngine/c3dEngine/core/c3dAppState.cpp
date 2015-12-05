//
//  c3dAppState.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-21.
//
//

#include "c3dAppState.h"
static Cc3dAppState*s_appState=NULL;
Cc3dAppState*Cc3dAppState::sharedAppState(){
    if(s_appState==NULL){
        s_appState=new Cc3dAppState();
    }
    return s_appState;
}