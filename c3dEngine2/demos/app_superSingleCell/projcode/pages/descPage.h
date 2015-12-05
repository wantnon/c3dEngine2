//
//  descPage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__descPage__
#define __HelloOpenGL__descPage__

#include <iostream>
using namespace std;
#include "key.h"



class CdescPage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    CdescPage();
    virtual ~CdescPage();
protected:
    Ckey*m_quad_descPage_backGround;
    Ckey*m_foeKey;
    Ckey*m_nextKey;
    
};
#endif /* defined(__HelloOpenGL__descPage__) */
