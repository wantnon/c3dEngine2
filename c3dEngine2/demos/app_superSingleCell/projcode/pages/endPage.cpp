//
//  endPage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "endPage.h"
#include "globalVars.h"
#include "homePage.h"
#include "c3d.h"
#include "define.h"
#include "passUnifoCallBack.h"
#include "gestureAnalyzer.h"
bool CendPage::init(){
    const float wReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x();
    const float hReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y();
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    string resoStr=numberToStr(wReso)+"x"+numberToStr(hReso);
    
    const float keyMovUp=height-480;//按钮向上移动量--abc
    //--quad_endPage_backGround
    {
        m_quad_endPage_backGround=new Ckey();
        m_quad_endPage_backGround->autorelease();
        //
        string imagePathShort="singleCellGameResource/data/global/tex/pageTexs/endPageTex/backGround_";
        imagePathShort+=resoStr+".png";
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage(imagePathShort,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
        const float texW=ptex->getWidth();
        const float texH=ptex->getHeight();
        float bx=0;
        float by=0;
        m_quad_endPage_backGround->genAndInit(bx, bx+width, by, by+height,0,wReso/texW,1-hReso/texH,1);
        m_quad_endPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_endPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_quad_endPage_backGround->setKeyUpColor(1, 1, 1, 1);
        m_quad_endPage_backGround->useKeyUpColor();
        m_quad_endPage_backGround->setCamera(camera2D);
        m_quad_endPage_backGround->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_endPage_backGround->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_endPage_backGround->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        m_quad_endPage_backGround->reSetLeftDownCorner(0, 0);
        m_quad_endPage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STREAM_DRAW);
        //
        addChild(m_quad_endPage_backGround);
    }
    //----quad_waitingForMusicToStop
    {
        m_quad_waitingForMusicToStop=new Ckey();
        m_quad_waitingForMusicToStop->autorelease();
        //
        float bx=0;
        float by=0;
        float w=200;
        float h=50;
        m_quad_waitingForMusicToStop->genAndInit(bx, bx+w, by, by+h,0,1,0,1);
        m_quad_waitingForMusicToStop->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_waitingForMusicToStop->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_waiting_for_music_to_stop.png"));
        m_quad_waitingForMusicToStop->setKeyUpColor(1, 1, 1, 1);
        m_quad_waitingForMusicToStop->useKeyUpColor();
        m_quad_waitingForMusicToStop->setCamera(camera2D);
        m_quad_waitingForMusicToStop->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_waitingForMusicToStop->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_waitingForMusicToStop->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float c[2]={160,440+keyMovUp};
        m_quad_waitingForMusicToStop->reSetCenter(c[0], c[1]);
        m_quad_waitingForMusicToStop->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_waitingForMusicToStop);
    }
    
    //--homeKey
    {
        m_homeKey=new Ckey();
        m_homeKey->autorelease();
        //
        float c[2]={0,0};
        m_homeKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_homeKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_homeKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_homeKey->setKeyUpColor(1, 1, 1, 1);
        m_homeKey->useKeyUpColor();
        m_homeKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_home.png"));
        m_homeKey->setCamera(camera2D);
        m_homeKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_homeKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_homeKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={30,450+keyMovUp};
        m_homeKey->reSetCenter(center[0], center[1]);
        m_homeKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_homeKey);
    }
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    //----endMusic
    {
        Cc3dALSource*pALSource_endMusic=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/endMusic.wav","endMusic");
        this->addALSource(pALSource_endMusic);
        //重设声音源位置和听者位姿--abc
        //听者放在(0,0,0)，声源放在(0,0,-1)
        pALSource_endMusic->updatePosition(Cc3dVector4(0,0,-1,1));
        float ori[6]={0,0,-1,0,1,0};//注意，ori有六个三元，由at和up组成，而非dir
        alListenerfv(AL_ORIENTATION, ori);
        alListenerfv(AL_POSITION,Cc3dVector4(0,0,0,1).getArray());
        //play music
        pALSource_endMusic->play();
        
    }

    return true;
}
void CendPage::update(float dt){
    //----homeKey
    if(this->getALSourceByName("endMusic")->getIsPlaying()==false){//如果音乐停止则响应按键--abc
        //homeKey响应单击--abc
        if(m_homeKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->getALSourceByName("switchPage")->play();
            ChomePage*homePage=new ChomePage();
            homePage->autorelease();
            homePage->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(homePage);
            
        }
        //homeKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_homeKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_homeKey->useKeyDownColor();
        }else{
            m_homeKey->useKeyUpColor();
        }
    }

}
void CendPage::draw(){
    if(this->getALSourceByName("endMusic")->getIsPlaying()==false){//如果音乐停止则显示按钮--abc
        m_homeKey->setIsVisible(true);
        m_quad_waitingForMusicToStop->setIsVisible(false);
    }else{//如果音乐未停止则显示waiting for music to stop
        m_homeKey->setIsVisible(false);
        m_quad_waitingForMusicToStop->setIsVisible(true);
        
    }
    //call father draw
    Cc3dNode::draw();
    
}
CendPage::CendPage(){
    
}
CendPage::~CendPage(){
    
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
}
