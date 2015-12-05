//
//  settingPage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__settingPage__
#define __HelloOpenGL__settingPage__

#include <iostream>
using namespace std;
#include "key.h"


class CsettingPage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    CsettingPage();
    virtual ~CsettingPage();
    void draw();
protected:
    Ckey*m_quad_settingPage_backGround;
    Ckey*m_backKey;
    Ckey*m_turnOnAudioKey;
    Ckey*m_turnOffAudioKey;

    
};
#endif /* defined(__HelloOpenGL__settingPage__) */
