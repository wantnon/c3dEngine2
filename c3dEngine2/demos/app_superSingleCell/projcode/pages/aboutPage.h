//
//  aboutPage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__aboutPage__
#define __HelloOpenGL__aboutPage__

#include <iostream>
using namespace std;
#include "key.h"


class CaboutPage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    CaboutPage();
    virtual ~CaboutPage();
protected:
    Ckey*m_quad_aboutPage_backGround;
    Ckey*m_backKey;

};
#endif /* defined(__HelloOpenGL__aboutPage__) */
