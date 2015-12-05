//
//  gameState.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-22.
//
//

#ifndef __HelloOpenGL__gameState__
#define __HelloOpenGL__gameState__

#include <iostream>
using namespace std;
class CgameState{
public:
    CgameState(){
        m_currentLevel=0;
    }
    static CgameState*sharedGameState();
    void setCurrentLevel(int level){
        m_currentLevel=level;
    }
    int getCurrentLevel()const{return m_currentLevel;}
protected:
    int m_currentLevel;//0表示没进入任何关卡，有效关卡号从1开始--abc
};
#endif /* defined(__HelloOpenGL__gameState__) */
