//
//  playingPage.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-9.
//
//

#ifndef __HelloOpenGL__playingPage__
#define __HelloOpenGL__playingPage__

#include <iostream>
using namespace std;
#include "c3d.h"
#include "key.h"
#include "progressBar.h"
#include "bossCell.h"
#include "seabed.h"
#include "backGroundMusicCube.h"
#include "c3d.h"
#include "shadowMapInfo.h"
#include "bubble.h"
#include "fakeLight.h"
#include "meteor.h"
#include "sky.h"
#include "checkPoint.h"

class  CplayingPage:public Cc3dActor//public Cc3dNode
{
public:
    void visitDraw();
    bool init(int level);
    void update(float dt);
     CplayingPage();
    virtual~CplayingPage();

public:
    
    bool resetToCheckPoint();
    bool getIsCheckPointEmpty()const ;
    int getLevel()const{return m_level;}
   
protected:
    void unloadGame();
    void debugSwitch();
    void makeSound();
    void logic();
    bool getIsGameOver(){
        return (m_cell->life.value==0&&m_cell->isFallFlat==false);
    }
    bool getIsSucc(){
        return (m_cell->get_isAllKeyGot());
    }
    int getEnemyKilledCount(){
        return (m_cell->nKillEnemy.get());
    }
    int getCoinEatenCount(){
        return (m_cell->nEateCoin.get());
    }
    
    
protected:
    Cc3dNode*m_uiRoot;//所有的ui元素都挂到这上面--abc
    //--ui
    Ckey*m_quad_depthTex;
    Ckey*m_quad_depthTex2;
    Ckey*m_debugKey;
    Ckey*m_tipKey;
    Ckey*m_pauseKey;
    Ckey*m_zoomOutKey;
    Ckey*m_quad_oops;
    Ckey*m_quad_out_of_fuel;
    Ckey*m_quad_halfFuelHaveBeenUsed;
    Ckey*m_shotBigKey;
    Ckey*m_shotKey;
    Ckey*m_goDownShipKey;
    CprogressBar*m_lifeBar;
    vector<Ckey*> m_quad_keyList;
    Ckey*m_quad_coin;
    vector<Ckey*> m_quad_nCoinPositivePlaceList;
    Ckey*m_quad_killEnemy;
    vector<Ckey*> m_quad_nKillEnemyPositivePlaceList;
    CprogressBar* m_fuelBar;
    //--actor
    CskySemiBall*m_sky;
    CbossCell*m_cell;
    Cground*m_ground;
    Cseabed*m_seabed;
    Cwater*m_water;
    CarchScene*m_archScene;
    CbackGroundMusicCube*m_levelBackGroundMusicCube;
    vector<Cmolecule*> m_fireList;
    //--data
    int m_level;
    Cc3dVector4 m_eyePosInCell;
    CcheckPoint m_checkPoint;
    //--actor set
    CbubbleSet m_bubbleSet;
    CblastSet m_shotBlastSet;
    CblastSet m_bubbleBlastSet;
    CfakeLightSet m_fakeLightSet;
    CmeteorSet m_meteorSet;
    CmobilePlatformSet m_mobilePlatformSet;
    CshipSet m_shipSet;
    CbossCellSet m_bossCellSet;
    //--collisionGrid
    CcollisionGrid m_collisionGrid_boss;
    CcollisionGrid m_collisionGrid_bubble;
    CcollisionGrid m_collisionGrid_mobilePlatform;
    //--shadow map
    Cc3dFbo*m_fbo;
    Cc3dTexture*m_depthTex;
    bool m_depthTex_got;
    CshadowMapInfo m_shadowMapInfo;
    Cc3dFbo*m_fbo2;
    Cc3dTexture*m_depthTex2;
    CshadowMapInfo m_shadowMapInfo2;
    //--texure
    vector<Cc3dTexture*> m_numberTexList;
    Cc3dTexture* m_dotTex;
    Cc3dTexture* m_tex_quad_key_blackWhite;//m_quad_keyList的黑白纹理--abc
    Cc3dTexture* m_tex_quad_key_colored;//m_quad_keyList的彩色纹理--abc
   
};
#endif /* defined(__HelloOpenGL__playingPage__) */
