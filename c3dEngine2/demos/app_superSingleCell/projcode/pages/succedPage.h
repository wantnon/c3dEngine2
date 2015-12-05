//
//  succedPage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#ifndef __HelloOpenGL__succedPage__
#define __HelloOpenGL__succedPage__

#include <iostream>
using namespace std;
#include "key.h"
#include "progressBar.h"


class CsuccedPage:public Cc3dActor//public Cc3dNode
{
public:
    
    bool init(int nStar,int nEnemyKilled,int nCoinEaten);
    void update(float dt);
    CsuccedPage();
    virtual ~CsuccedPage();
protected:
    Ckey*m_quad_succeed;
    Ckey*m_quad_star;
    Ckey*m_nextLevelKey;
    Ckey*m_homeKey;
    Ckey*m_restartKey;

    Ckey*m_quad_coin;
    vector<Ckey*>m_quad_nCoinPositivePlaceList;
    Ckey*m_quad_killEnemy;
    vector<Ckey*>m_quad_nKillEnemyPositivePlaceList;
    vector<Ckey*>m_quad_keyList;
    
    //--texure
    vector<Cc3dTexture*> m_numberTexList;
    Cc3dTexture* m_dotTex;
    Cc3dTexture* m_tex_quad_key_blackWhite;//m_quad_keyList的黑白纹理--abc
    Cc3dTexture* m_tex_quad_key_colored;//m_quad_keyList的彩色纹理--abc

    
};
#endif /* defined(__HelloOpenGL__succedPage__) */
