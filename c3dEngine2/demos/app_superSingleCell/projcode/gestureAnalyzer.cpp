//
//  gestureAnalyzer.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-13.
//
//
#include "gestureAnalyzer.h"

static CgestureAnalyzer*s_gestureAnalyzer=NULL;
CgestureAnalyzer* CgestureAnalyzer::sharedGestureAnalyzer(){
    if(s_gestureAnalyzer==NULL){
        s_gestureAnalyzer=new CgestureAnalyzer();
    }
    return s_gestureAnalyzer;
}
