//
//  menuPage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "menuPage.h"
#include "globalVars.h"
#include "homePage.h"
#include "settingPage.h"
#include "aboutPage.h"
#include "descPage.h"
#include "c3d.h"
#include "define.h"
#include "passUnifoCallBack.h"
#include "gestureAnalyzer.h"
bool CmenuPage::init(){
    const float wReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x();
    const float hReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y();
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    string resoStr=numberToStr(wReso)+"x"+numberToStr(hReso);
    const float keyMovUp=height-480;//按钮向上移动量--abc
    //--quad_menuPage_backGround
    {
        m_quad_menuPage_backGround=new Ckey();
        m_quad_menuPage_backGround->autorelease();
        //
        string imagePathShort="singleCellGameResource/data/global/tex/pageTexs/menuPageTex/backGround_";
        imagePathShort+=resoStr+".png";
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage(imagePathShort,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        const float texW=ptex->getWidth();
        const float texH=ptex->getHeight();
        float bx=0;
        float by=0;
        m_quad_menuPage_backGround->genAndInit(bx, bx+width, by, by+height,0,wReso/texW,1-hReso/texH,1);
        m_quad_menuPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_menuPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_quad_menuPage_backGround->setKeyUpColor(1, 1, 1, 1);
        m_quad_menuPage_backGround->useKeyUpColor();
        m_quad_menuPage_backGround->setCamera(camera2D);
        m_quad_menuPage_backGround->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_menuPage_backGround->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_menuPage_backGround->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        m_quad_menuPage_backGround->reSetLeftDownCorner(0, 0);
        m_quad_menuPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_menuPage_backGround);
    }
    //--playKey
    {
        m_playKey=new Ckey();
        m_playKey->autorelease();
        //
        float c[2]={0,0};
        m_playKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_playKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_playKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_playKey->setKeyUpColor(1, 1, 1, 1);
        m_playKey->useKeyUpColor();
        m_playKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_play.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false));
        m_playKey->setCamera(camera2D);
        m_playKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_playKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_playKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={160,350+keyMovUp/2};
        m_playKey->reSetCenter(center[0], center[1]);
        m_playKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_playKey);
        
    }
    //--descKey
    {
        m_descKey=new Ckey();
        m_descKey->autorelease();
        //
        Cc3dTexture* ptexture=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_desc.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_descKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_descKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_descKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_descKey->setKeyUpColor(1, 1, 1, 1);
        m_descKey->useKeyUpColor();
        m_descKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptexture);
        m_descKey->setCamera(camera2D);
        m_descKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_descKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_descKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={160,290+keyMovUp/2};
        m_descKey->reSetCenter(center[0], center[1]);
        m_descKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_descKey);
        
    }
    
    //--settingKey
    {
        m_settingKey=new Ckey();
        m_settingKey->autorelease();
        //
        Cc3dTexture* ptexture=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_setting.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_settingKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_settingKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_settingKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_settingKey->setKeyUpColor(1, 1, 1, 1);
        m_settingKey->useKeyUpColor();
        m_settingKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptexture);
        m_settingKey->setCamera(camera2D);
        m_settingKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_settingKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_settingKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={160,230+keyMovUp/2};
        m_settingKey->reSetCenter(center[0], center[1]);
        m_settingKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_settingKey);
    }
    
    //--aboutKey
    {
        m_aboutKey=new Ckey();
        m_aboutKey->autorelease();
        //
        Cc3dTexture* ptexture=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_about.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_aboutKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_aboutKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_aboutKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_aboutKey->setKeyUpColor(1, 1, 1, 1);
        m_aboutKey->useKeyUpColor();
        m_aboutKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptexture);
        m_aboutKey->setCamera(camera2D);
        m_aboutKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_aboutKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_aboutKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={160,170+keyMovUp/2};
        m_aboutKey->reSetCenter(center[0], center[1]);
        m_aboutKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_aboutKey);
    }
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    return true;
}
void CmenuPage::update(float dt){
    //----playKey
    {
      
        //playKey响应单击--abc
        if(m_playKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->getALSourceByName("switchPage")->play();
            ChomePage*homePage=new ChomePage();
            homePage->autorelease();
            homePage->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(homePage);

        }
        //playKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_playKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_playKey->useKeyDownColor();
        }else{
            m_playKey->useKeyUpColor();
        }
        
    }
    //----descKey
    {
        //descKey响应单击--abc
        if(m_descKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->getALSourceByName("switchPage")->play();
            CdescPage*descPage=new CdescPage();
            descPage->autorelease();
            descPage->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(descPage);
            
        }
        //descKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_descKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_descKey->useKeyDownColor();
        }else{
            m_descKey->useKeyUpColor();
        }
    }
    //----aboutKey
    {
        //aboutKey响应单击--abc
        if(m_aboutKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->getALSourceByName("switchPage")->play();
            CaboutPage*aboutPage=new CaboutPage();
            aboutPage->autorelease();
            aboutPage->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(aboutPage);
            
        }
        //aboutKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_aboutKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_aboutKey->useKeyDownColor();
        }else{
            m_aboutKey->useKeyUpColor();
        }
    }
    //----settingKey
    {
        //settingKey响应单击--abc
        if(m_settingKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->getALSourceByName("switchPage")->play();
            CsettingPage*settingPage=new CsettingPage();
            settingPage->autorelease();
            settingPage->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(settingPage);
            
        }
        //settingKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_settingKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_settingKey->useKeyDownColor();
        }else{
            m_settingKey->useKeyUpColor();
        }
    }
    

}
CmenuPage::CmenuPage(){
    m_quad_menuPage_backGround=NULL;
    m_playKey=NULL;
    m_descKey=NULL;
    m_settingKey=NULL;
    m_aboutKey=NULL;
}
CmenuPage::~CmenuPage(){
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
}

