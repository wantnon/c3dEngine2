//
//  viewingTipPage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "viewingTipPage.h"
#include "globalVars.h"
#include "c3d.h"
#include "playingPage.h"
#include "myFunc.h"
#include "gestureAnalyzer.h"
bool CviewingTipPage::init(){
    int level=(( CplayingPage*)Cc3dSceneManager::sharedSceneManager()->getRoot()->getChildByName("playingPage"))->getLevel();
    string levelStr=numberToStr(level);
    string rootPath="singleCellGameResource/data/levels/level_"+levelStr+"/";
    //
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    //--quad_tipTex
    {
        Cc3dTexture*texture=Cc3dTextureCache::sharedTextureCache()->addImage(rootPath+"/tipTex/tipTex.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        m_quad_tipTex=new Ckey();
        m_quad_tipTex->autorelease();
        //
        float bx=0;
        float by=0;
        float w=texture->getWidth()/Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getPixelDensity();
        float h=texture->getHeight()/Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getPixelDensity();
        m_quad_tipTex->genAndInit(bx, bx+w, by, by+h,0,1,0,1);//不考虑分辨率--abc
        m_quad_tipTex->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_tipTex->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(texture);
        m_quad_tipTex->setKeyUpColor(1, 1, 1, 1);
        m_quad_tipTex->useKeyUpColor();
        m_quad_tipTex->setCamera(camera2D);
        m_quad_tipTex->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_tipTex->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_tipTex->setPassUnifoCallback(passUnifoCallback_noLight);
        m_quad_tipTex->setVisitDrawOrder(10000);
        //
        m_quad_tipTex->reSetCenter(width/2,height/2);
        m_quad_tipTex->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_tipTex);
    }
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    return true;
}
void CviewingTipPage::update(float dt){
    //----quad_tipTex响应单击--abc
    if(m_quad_tipTex->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
        this->getALSourceByName("switchPage")->play();
        this->removeFromParentOnNextFrame();
        this->getFather()->setIsDoUpdate(true);
    }

}
CviewingTipPage::CviewingTipPage(){

}
CviewingTipPage::~CviewingTipPage(){
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
}