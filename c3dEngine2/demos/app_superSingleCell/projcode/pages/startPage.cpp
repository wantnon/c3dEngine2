//
//  startPage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "startPage.h"
#include "globalVars.h"
#include "menuPage.h"
#include "c3d.h"
#include "passUnifoCallBack.h"
#include "gestureAnalyzer.h"
bool CstartPage::init(){
    const float wReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x();
    const float hReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y();
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    string resoStr=numberToStr(wReso)+"x"+numberToStr(hReso);
	//cout<<"resoStr:"<<resoStr<<endl;
    //--quad_startPage_backGround
    {
        m_quad_startPage_backGround=new Ckey();
        m_quad_startPage_backGround->autorelease();
        //
        string imagePathShort="singleCellGameResource/data/global/tex/pageTexs/startPageTex/backGround_";
        imagePathShort+=resoStr+".png";
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage(imagePathShort,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        const float texW=ptex->getWidth();
        const float texH=ptex->getHeight();
        float bx=0;
        float by=0;
        m_quad_startPage_backGround->genAndInit(bx, bx+width, by, by+height,0,wReso/texW,1-hReso/texH,1);
        m_quad_startPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_startPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_quad_startPage_backGround->setKeyUpColor(1, 1, 1, 1);
        m_quad_startPage_backGround->useKeyUpColor();
        m_quad_startPage_backGround->setCamera(camera2D);
        m_quad_startPage_backGround->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_startPage_backGround->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_startPage_backGround->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        m_quad_startPage_backGround->reSetLeftDownCorner(0, 0);
        m_quad_startPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STREAM_DRAW);
        //
        addChild(m_quad_startPage_backGround);
    }
    //----startMusic
    Cc3dALSource*pALSource_startMusic=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/startMusic.wav","startMusic");
    this->addALSource(pALSource_startMusic);
    pALSource_startMusic->setIsLooping(true);
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    
    
    //----开始播放--abc
    this->getALSourceByName("startMusic")->play();
    return true;
}
void CstartPage::update(float dt){
    //----quad_startPage_backGround
    //quad_startPage_backGround响应单击--abc
  /*  Cc3dTouch touch=Cc3dTouchSequence::sharedTouchSequence()->getLatestTouches()[0];
    cout<<"-----"<<endl;
    touch.print();
    cout<<"timeCount: "<<timeCounter.getCount()<<endl;
    */
    //if(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce())cout<<"hi"<<endl;
    if(m_quad_startPage_backGround->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
        this->getALSourceByName("switchPage")->play();
        CmenuPage*menuPage=new CmenuPage();
        menuPage->autorelease();
        menuPage->init();
        Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
        Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(menuPage);
        
        
    }

}
CstartPage::CstartPage(){
    m_quad_startPage_backGround=NULL;
}
CstartPage::~CstartPage(){
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();

}
