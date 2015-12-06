//
//  descPage3.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "descPage3.h"
#include "globalVars.h"
#include "descPage2.h"
#include "menuPage.h"
#include "c3d.h"
#include "define.h"
#include "passUnifoCallBack.h"
#include "gestureAnalyzer.h"
bool CdescPage3::init(){
    
    const float wReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x();
    const float hReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y();
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    string resoStr=numberToStr(wReso)+"x"+numberToStr(hReso);
    //--quad_descPage3_backGround
    {
        m_quad_descPage3_backGround=new Ckey();
        m_quad_descPage3_backGround->autorelease();
        //
        string imagePathShort="singleCellGameResource/data/global/tex/pageTexs/descPage3Tex/backGround_";
        imagePathShort+=resoStr+".png";
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage(imagePathShort,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        const float texW=ptex->getWidth();
        const float texH=ptex->getHeight();
        float bx=0;
        float by=0;
        m_quad_descPage3_backGround->genAndInit(bx, bx+width, by, by+height,0,wReso/texW,1-hReso/texH,1);
        m_quad_descPage3_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_descPage3_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_quad_descPage3_backGround->setKeyUpColor(1, 1, 1, 1);
        m_quad_descPage3_backGround->useKeyUpColor();
        m_quad_descPage3_backGround->setCamera(camera2D);
        m_quad_descPage3_backGround->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_descPage3_backGround->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_descPage3_backGround->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        m_quad_descPage3_backGround->reSetLeftDownCorner(0, 0);
        m_quad_descPage3_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_descPage3_backGround);
    }
    //----foeKey
    {
        m_foeKey=new Ckey();
        m_foeKey->autorelease();
        //
        Cc3dTexture* ptexture=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_foe.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_foeKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key/2,c[1]+r_key/2, 0,1,0,1);
        m_foeKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_foeKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_foeKey->setKeyUpColor(1, 1, 1, 1);
        m_foeKey->useKeyUpColor();
        m_foeKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptexture);
        m_foeKey->setCamera(camera2D);
        m_foeKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_foeKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_foeKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={width/2-50,20};
        m_foeKey->reSetCenter(center[0], center[1]);
        m_foeKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_foeKey);
    }
    //--menuKey
    {
        m_menuKey=new Ckey();
        m_menuKey->autorelease();
        //
        Cc3dTexture* ptexture=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_menu.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_menuKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key/2,c[1]+r_key/2, 0,1,0,1);
        m_menuKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_menuKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_menuKey->setKeyUpColor(1, 1, 1, 1);
        m_menuKey->useKeyUpColor();
        m_menuKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptexture);
        m_menuKey->setCamera(camera2D);
        m_menuKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_menuKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_menuKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={width/2+50,20};
        m_menuKey->reSetCenter(center[0], center[1]);
        m_menuKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_menuKey);
    }
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    return true;
}
void CdescPage3::update(float dt){
    //----foeKey
    {
        //foeKey响应单击--abc
        if(m_foeKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->getALSourceByName("switchPage")->play();
            CdescPage2*descPage2=new CdescPage2();
            descPage2->autorelease();
            descPage2->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(descPage2);
        }
        //foeKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_foeKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_foeKey->useKeyDownColor();
        }else{
            m_foeKey->useKeyUpColor();
        }
    }
    //----menuKey
    {
        //menuKey响应单击--abc
        if(m_menuKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->getALSourceByName("switchPage")->play();
            CmenuPage*menuPage=new CmenuPage();
            menuPage->autorelease();
            menuPage->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(menuPage);
            
        }
        //menuKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_menuKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_menuKey->useKeyDownColor();
        }else{
            m_menuKey->useKeyUpColor();
        }
    }
}
CdescPage3::CdescPage3(){

}
CdescPage3::~CdescPage3(){
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
}

