//
//  startPage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__startPage__
#define __HelloOpenGL__startPage__

#include <iostream>
using namespace std;
#include "c3d.h"
#include "key.h"



class CstartPage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    CstartPage();
    virtual ~CstartPage();
protected:
    Ckey*m_quad_startPage_backGround;
    
    
};
#endif /* defined(__HelloOpenGL__startPage__) */
