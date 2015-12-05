//
//  gameSettings.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-22.
//
//

#include "gameSettings.h"
static CgameSettings*s_gameSettings=NULL;
CgameSettings*CgameSettings::sharedGameSettings(){
    if(s_gameSettings==NULL){
        s_gameSettings=new CgameSettings();
    }
    return s_gameSettings;
}