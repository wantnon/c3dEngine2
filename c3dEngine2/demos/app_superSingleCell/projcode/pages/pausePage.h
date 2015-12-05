//
//  pausePage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__pausePage__
#define __HelloOpenGL__pausePage__

#include <iostream>
using namespace std;
#include "key.h"


class CpausePage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    CpausePage();
    virtual ~CpausePage();
    
protected:
    Ckey*m_quad_pause;
    Ckey*m_continueKey;
    Ckey*m_homeKey;
    Ckey*m_restartKey;


};
#endif /* defined(__HelloOpenGL__pausePage__) */
