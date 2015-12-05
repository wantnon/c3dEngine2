//
//  viewingTipPage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__viewingTipPage__
#define __HelloOpenGL__viewingTipPage__

#include <iostream>
using namespace std;
#include "key.h"

class CviewingTipPage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init();
    void update(float dt);
    CviewingTipPage();
    virtual ~CviewingTipPage();
    
protected:
    Ckey*m_quad_tipTex;
    
    
};
#endif /* defined(__HelloOpenGL__viewingTipPage__) */
