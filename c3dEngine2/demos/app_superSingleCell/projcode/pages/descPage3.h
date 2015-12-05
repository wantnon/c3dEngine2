//
//  descPage3.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__descPage3__
#define __HelloOpenGL__descPage3__

#include <iostream>
using namespace std;

#include "key.h"

class CdescPage3:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    CdescPage3();
    virtual ~CdescPage3();
protected:
    Ckey*m_quad_descPage3_backGround;
    Ckey*m_foeKey;
    Ckey*m_menuKey;
    
};
#endif /* defined(__HelloOpenGL__descPage3__) */
