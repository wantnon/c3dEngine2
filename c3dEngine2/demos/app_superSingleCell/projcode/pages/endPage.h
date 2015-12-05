//
//  endPage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__endPage__
#define __HelloOpenGL__endPage__

#include <iostream>
using namespace std;

#include "key.h"


class CendPage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    void draw();
    CendPage();
    virtual ~CendPage();
protected:
    Ckey*m_quad_endPage_backGround;
    Ckey*m_quad_waitingForMusicToStop;
    Ckey*m_homeKey;
};
#endif /* defined(__HelloOpenGL__endPage__) */
