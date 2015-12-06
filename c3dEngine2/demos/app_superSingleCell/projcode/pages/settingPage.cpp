//
//  settingPage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "settingPage.h"
#include "globalVars.h"
#include "menuPage.h"
#include "c3d.h"
#include "define.h"
#include "gameSettings.h"
#include "passUnifoCallBack.h"
#include "gestureAnalyzer.h"
bool CsettingPage::init(){
    const float wReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x();
    const float hReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y();
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    string resoStr=numberToStr(wReso)+"x"+numberToStr(hReso);
    
    const float keyMovUp=height-480;//按钮向上移动量--abc
    //--quad_settingPage_backGround
    {
        m_quad_settingPage_backGround=new Ckey();
        m_quad_settingPage_backGround->autorelease();
        //
        string imagePathShort="singleCellGameResource/data/global/tex/pageTexs/settingPageTex/backGround_";
        imagePathShort+=resoStr+".png";
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage(imagePathShort,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        const float texW=ptex->getWidth();
        const float texH=ptex->getHeight();
        float bx=0;
        float by=0;
        m_quad_settingPage_backGround->genAndInit(bx, bx+width, by, by+height,0,wReso/texW,1-hReso/texH,1);
        m_quad_settingPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_settingPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_quad_settingPage_backGround->setKeyUpColor(1, 1, 1, 1);
        m_quad_settingPage_backGround->useKeyUpColor();
        m_quad_settingPage_backGround->setCamera(camera2D);
        m_quad_settingPage_backGround->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_settingPage_backGround->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_settingPage_backGround->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        m_quad_settingPage_backGround->reSetLeftDownCorner(0, 0);
        m_quad_settingPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_settingPage_backGround);
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
    
    //----turnOnAudioKey
    {
        m_turnOnAudioKey=new Ckey();
        m_turnOnAudioKey->autorelease();
        //
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_audioOn.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_turnOnAudioKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_turnOnAudioKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_turnOnAudioKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_turnOnAudioKey->setKeyUpColor(1, 1, 1, 1);
        m_turnOnAudioKey->useKeyUpColor();
        m_turnOnAudioKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_turnOnAudioKey->setCamera(camera2D);
        m_turnOnAudioKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_turnOnAudioKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_turnOnAudioKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={160,380+keyMovUp/2};
        m_turnOnAudioKey->reSetCenter(center[0], center[1]);
        m_turnOnAudioKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_turnOnAudioKey->setKeyUpColor(1, 1, 1, 1);
        m_turnOnAudioKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_turnOnAudioKey);
        
    }
    //----turnOffAudioKey
    {
        m_turnOffAudioKey=new Ckey();
        m_turnOffAudioKey->autorelease();
        //
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_audioOff.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_turnOffAudioKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_turnOffAudioKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_turnOffAudioKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_turnOffAudioKey->setKeyUpColor(1, 1, 1, 1);
        m_turnOffAudioKey->useKeyUpColor();
        m_turnOffAudioKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_turnOffAudioKey->setCamera(camera2D);
        m_turnOffAudioKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_turnOffAudioKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_turnOffAudioKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={160,380+keyMovUp/2};
        m_turnOffAudioKey->reSetCenter(center[0], center[1]);
        m_turnOffAudioKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_turnOffAudioKey->setKeyUpColor(1, 1, 1, 1);
        m_turnOffAudioKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_turnOffAudioKey);
        
        
    }
    
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    return true;
}
void CsettingPage::update(float dt){

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
    //----turnOnAudioKey和turnOffAudioKey
    if(CgameSettings::sharedGameSettings()->getAudioOn()==false){
        //turnOnAudioKey响应单击--abc
        if(m_turnOnAudioKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            CgameSettings::sharedGameSettings()->setAudioOn(true);
            alListenerf(AL_GAIN, 1.0);
        }
        //turnOnAudioKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_turnOnAudioKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_turnOnAudioKey->useKeyDownColor();
        }else{
            m_turnOnAudioKey->useKeyUpColor();
        }
    }else{//audioOn==true
        //turnOffAudioKey响应单击--abc
        if(m_turnOffAudioKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            CgameSettings::sharedGameSettings()->setAudioOn(false);
            alListenerf(AL_GAIN, 0.0);
        }
        //turnOffAudioKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_turnOnAudioKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_turnOffAudioKey->useKeyDownColor();
        }else{
            m_turnOffAudioKey->useKeyUpColor();
        }
    }
    
 
}
void CsettingPage::draw(){
    m_backKey->setIsVisible(true);
    if(CgameSettings::sharedGameSettings()->getAudioOn()){
        m_turnOffAudioKey->setIsVisible(true);
        m_turnOnAudioKey->setIsVisible(false);
    }else{
        m_turnOnAudioKey->setIsVisible(true);
        m_turnOffAudioKey->setIsVisible(false);
    }
    
    //call father draw
    Cc3dNode::draw();
}
CsettingPage::CsettingPage(){
  
}
CsettingPage::~CsettingPage(){
   
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
}