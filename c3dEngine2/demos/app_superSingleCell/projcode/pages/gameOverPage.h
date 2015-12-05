//
//  gameOverPage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__gameOverPage__
#define __HelloOpenGL__gameOverPage__

#include <iostream>
using namespace std;
#include "key.h"



class CgameOverPage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    CgameOverPage();
    virtual ~CgameOverPage();

protected:
    Ckey*m_quad_gameover;
    Ckey*m_restartKey;
    Ckey*m_rebirthKey;
    
};
#endif /* defined(__HelloOpenGL__gameOverPage__) */
