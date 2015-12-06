//
//  loadingPage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "loadingPage.h"
#include "globalVars.h"
#include "c3d.h"
#include "playingPage.h"
#include "gameState.h"
bool CloadingPage::init(){
    const float wReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x();
    const float hReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y();
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    string resoStr=numberToStr(wReso)+"x"+numberToStr(hReso);
    //--quad_loadingPage_backGround
    {
        m_quad_loadingPage_backGround=new Ckey();
        m_quad_loadingPage_backGround->autorelease();
        //
        string imagePathShort="singleCellGameResource/data/global/tex/pageTexs/loadingPageTex/backGround_";
        imagePathShort+=resoStr+".png";
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage(imagePathShort,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        const float texW=ptex->getWidth();
        const float texH=ptex->getHeight();
        float bx=0;
        float by=0;
        m_quad_loadingPage_backGround->genAndInit(bx, bx+width, by, by+height,0,wReso/texW,1-hReso/texH,1);
        m_quad_loadingPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_loadingPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_quad_loadingPage_backGround->setKeyUpColor(1, 1, 1, 1);
        m_quad_loadingPage_backGround->useKeyUpColor();
        m_quad_loadingPage_backGround->setCamera(camera2D);
        m_quad_loadingPage_backGround->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_loadingPage_backGround->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_loadingPage_backGround->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        m_quad_loadingPage_backGround->reSetLeftDownCorner(0, 0);
        m_quad_loadingPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_loadingPage_backGround);
    }

    //--quad_loading
    {
        m_quad_loading=new Ckey();
        m_quad_loading->autorelease();
        //
        Cc3dTexture* ptexture_quad_loading=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/pageTexs/loadingPageTex/quad_loading.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float bx=0;
        float by=0;
        float w=200;
        float h=50;
        m_quad_loading->genAndInit(bx, bx+w, by, by+h,0,1,0,1);
        m_quad_loading->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_loading->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptexture_quad_loading);
        m_quad_loading->setKeyUpColor(1, 1, 1, 1);
        m_quad_loading->useKeyUpColor();
        m_quad_loading->setCamera(camera2D);
        m_quad_loading->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_loading->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_loading->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float _bx=60;
        float _by=40;
        m_quad_loading->reSetLeftDownCorner(_bx, _by);
        m_quad_loading->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_loading);
    }
    return true;
}
void CloadingPage::update(float dt){
    CplayingPage*playingPage=new  CplayingPage();
    playingPage->autorelease();
    playingPage->init(CgameState::sharedGameState()->getCurrentLevel());
    playingPage->setName("playingPage");
    Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
    Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(playingPage);
}
CloadingPage::CloadingPage(){

}
CloadingPage::~CloadingPage(){
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
}

