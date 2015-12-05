//
//  menuPage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__menuPage__
#define __HelloOpenGL__menuPage__

#include <iostream>
using namespace std;
#include "c3d.h"
#include "key.h"

class CmenuPage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    CmenuPage();
    virtual ~CmenuPage();
protected:
    Ckey*m_quad_menuPage_backGround;
    Ckey*m_playKey;
    Ckey*m_descKey;
    Ckey*m_settingKey;
    Ckey*m_aboutKey;
    
    
    
};
#endif /* defined(__HelloOpenGL__menuPage__) */
