//
//  descPage2.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__descPage2__
#define __HelloOpenGL__descPage2__

#include <iostream>
using namespace std;
#include "key.h"

class CdescPage2:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    CdescPage2();
    virtual ~CdescPage2();
protected:
    Ckey*m_quad_descPage2_backGround;
    Ckey*m_foeKey;
    Ckey*m_nextKey;
};
#endif /* defined(__HelloOpenGL__descPage2__) */
