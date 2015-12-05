//
//  loadingPage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__loadingPage__
#define __HelloOpenGL__loadingPage__

#include <iostream>
using namespace std;

#include "key.h"

class CloadingPage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    CloadingPage();
    virtual ~CloadingPage();
protected:
    Ckey*m_quad_loading;
    Ckey*m_quad_loadingPage_backGround;
    
};
#endif /* defined(__HelloOpenGL__loadingPage__) */
