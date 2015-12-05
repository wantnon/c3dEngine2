//
//  gameState.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-22.
//
//

#include "gameState.h"
static CgameState*s_gameState=NULL;
CgameState*CgameState::sharedGameState(){
    if(s_gameState==NULL){
        s_gameState=new CgameState();
    }
    return s_gameState;
}