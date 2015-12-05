//
//  homePage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__homePage__
#define __HelloOpenGL__homePage__

#include <iostream>
using namespace std;

#include "key.h"


class ChomePage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    ChomePage();
    virtual ~ChomePage();
    int getLevelCount()const{return m_levelCount;}
protected:
    void debugSwitch();
protected:
    Ckey*m_debugKey;
    vector<Ckey*> m_gameKeyList;
    vector<Ckey*> m_gameKeyList_lock;
    vector<Ckey*> m_gameKeyList_star;
    Ckey*m_backKey;
    Ckey*m_quad_homePage_backGround;
    Ckey*m_quad_loadingPage_backGround;
    Ckey*m_quad_endPage_backGround;
protected:
    int m_levelCount;
protected:
    vector<Cc3dTexture*> m_tex_quad_starList;
    
    

};
#endif /* defined(__HelloOpenGL__homePage__) */
