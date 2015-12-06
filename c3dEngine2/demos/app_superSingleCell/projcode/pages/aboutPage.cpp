//
//  aboutPage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "aboutPage.h"
#include "globalVars.h"
#include "menuPage.h"
#include "c3d.h"
#include "define.h"
#include "passUnifoCallBack.h"
#include "gestureAnalyzer.h"
bool CaboutPage::init(){
    const float wReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x();
    const float hReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y();
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    string resoStr=numberToStr(wReso)+"x"+numberToStr(hReso);
    //--quad_aboutPage_backGround
    {
        m_quad_aboutPage_backGround=new Ckey();
        m_quad_aboutPage_backGround->autorelease();
        //
        string imagePathShort="singleCellGameResource/data/global/tex/pageTexs/aboutPageTex/backGround_";
        imagePathShort+=resoStr+".png";
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage(imagePathShort,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        const float texW=ptex->getWidth();
        const float texH=ptex->getHeight();
        float bx=0;
        float by=0;
        m_quad_aboutPage_backGround->genAndInit(bx, bx+width, by, by+height,0,wReso/texW,1-hReso/texH,1);
        m_quad_aboutPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_aboutPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_quad_aboutPage_backGround->setKeyUpColor(1, 1, 1, 1);
        m_quad_aboutPage_backGround->useKeyUpColor();
        m_quad_aboutPage_backGround->setCamera(camera2D);
        m_quad_aboutPage_backGround->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        //
        m_quad_aboutPage_backGround->reSetLeftDownCorner(0, 0);
        m_quad_aboutPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_aboutPage_backGround);
    }
    //----backKey
    {
        m_backKey=new Ckey();
        m_backKey->autorelease();
        //
        Cc3dTexture* ptexture=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_back.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_backKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_backKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_backKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_backKey->setKeyUpColor(1, 1, 1, 1);
        m_backKey->useKeyUpColor();
        m_backKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptexture);
        m_backKey->setCamera(camera2D);
        m_backKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_backKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_backKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={30,30};
        m_backKey->reSetCenter(center[0], center[1]);
        m_backKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_backKey);
    }
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    return true;
}
void CaboutPage::update(float dt){
    //----backKey
    {
        //backKey响应单击--abc
        if(m_backKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->getALSourceByName("switchPage")->play();
            CmenuPage*menuPage=new CmenuPage();
            menuPage->autorelease();
            menuPage->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(menuPage);
        }
        //backKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_backKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_backKey->useKeyDownColor();
        }else{
            m_backKey->useKeyUpColor();
        }
    }

}
CaboutPage::CaboutPage(){
    m_quad_aboutPage_backGround=NULL;
    m_backKey=NULL;
}
CaboutPage::~CaboutPage(){
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();

}
