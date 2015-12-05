//
//  homePage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "homePage.h"
#include "globalVars.h"
#include "menuPage.h"
#include "loadingPage.h"
#include "c3d.h"
#include "define.h"
#include "gameSettings.h"
#include "gameState.h"
#include "userConfig.h"
#include "passUnifoCallBack.h"
#include "molecule.h"
#include "gestureAnalyzer.h"
ChomePage::ChomePage(){
    m_backKey=NULL;
    m_quad_homePage_backGround=NULL;
    m_quad_loadingPage_backGround=NULL;
    m_quad_endPage_backGround=NULL;
}
ChomePage::~ChomePage(){
    for(int i=0;i<(int)m_tex_quad_starList.size();i++){
        m_tex_quad_starList[i]->release();
    }
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
}
bool ChomePage::init(){
  
    const float wReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x();
    const float hReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y();
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    string resoStr=numberToStr(wReso)+"x"+numberToStr(hReso);
    
    
    //--quad_homePage_backGround
    {
        m_quad_homePage_backGround=new Ckey();
        m_quad_homePage_backGround->autorelease();
        string imagePathShort="singleCellGameResource/data/global/tex/pageTexs/homePageTex/backGround_";
        imagePathShort+=resoStr+".png";
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage(imagePathShort,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
        const float texW=ptex->getWidth();
        const float texH=ptex->getHeight();
        float bx=0;
        float by=0;
        m_quad_homePage_backGround->genAndInit(bx, bx+width, by, by+height,0,wReso/texW,1-hReso/texH,1);
        m_quad_homePage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_homePage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_quad_homePage_backGround->setKeyUpColor(1, 1, 1, 1);
        m_quad_homePage_backGround->useKeyUpColor();
        m_quad_homePage_backGround->setCamera(camera2D);
        m_quad_homePage_backGround->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_homePage_backGround->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_homePage_backGround->setPassUnifoCallback(passUnifoCallback_noLight);
        
        m_quad_homePage_backGround->reSetLeftDownCorner(0, 0);
        m_quad_homePage_backGround->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        addChild(m_quad_homePage_backGround);
    }
    //--debugKey
    {
        m_debugKey=new Ckey();
        m_debugKey->autorelease();
        //
        float c[2]={0,0};
        m_debugKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_debugKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_debugKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_debugKey->setKeyUpColor(1, 1, 1, 1);
        m_debugKey->useKeyUpColor();
        m_debugKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_debug.png"));
        m_debugKey->setCamera(camera2D);
        m_debugKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_debugKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_debugKey->setPassUnifoCallback(passUnifoCallback_noLight);
        //
        float center[2]={width-r_key,0+r_key};
        m_debugKey->reSetCenter(center[0], center[1]);
        m_debugKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_debugKey);
    }

    //----backKey
    {
        m_backKey=new Ckey();
        m_backKey->autorelease();
        Cc3dTexture* ptexture=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_back.png");
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
        
        float center[2]={160,30};
        m_backKey->reSetCenter(center[0], center[1]);
        m_backKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        addChild(m_backKey);
    }
    //----m_gameKeyList和m_gameKeyList_lock和m_gameKeyList_star
    {
        //----gameKeyList和gameKeyList_lock和gameKeyList_star
        //m_gameKeyList
        {
            //生成gameKeyTexClip
            Cmolecule gameKeyTexClip;//临时对象--abc
            gameKeyTexClip.loadFrameTexList("singleCellGameResource/data/global/tex/pageTexs/homePageTex/gameKey/");
            
            //制作各texture的相应按钮--abc
            int nGameKey=(int)gameKeyTexClip.getFrameTexList().size();
            
            m_levelCount=nGameKey;
            
            for(int i=0;i<nGameKey;i++){
                Ckey*key=new Ckey();
                key->autorelease();
                float c[2]={0,0};
                float r=35;
                key->genAndInit(c[0]-r,c[0]+r,c[1]-r,c[1]+r, 0,1,0,1);
                key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
                key->setKeyDownColor(0.7, 0.7, 0.7, 1);
                key->setKeyUpColor(1, 1, 1, 1);
                key->useKeyUpColor();
                key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(gameKeyTexClip.getFrameTexList()[i]);
                key->setCamera(camera2D);
                key->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
                key->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
                key->setPassUnifoCallback(passUnifoCallback_noLight);
                addChild(key);
                m_gameKeyList.push_back(key);
            }
            //判断各key对应的关卡文件夹是否存在，设置为grey并使用keyDownColor
            for(int i=0;i<nGameKey;i++){
                Ckey*key=m_gameKeyList[i];
                int level=i+1;//关卡号--abc
                string curLevelClip="level_";
                curLevelClip+=numberToStr(level);//注：直接写"level_"+numberToStr(level)不报错，但得到错误结果--abc
                bool rootPathExists=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(string("singleCellGameResource/data/levels/")+curLevelClip);
                if(rootPathExists==false){
                    key->set_isGrey(true);
                    key->useKeyDownColor();
                }
            }
            //定位--abc
            {
                int nKey=(int)m_gameKeyList.size();
                //按每行四个进行排列，排满为止--abc
                int nCol=4;//按钮列数--abc
                int nRow=ceil((float)nKey/nCol);//按钮行数--abc
                for(int i=0;i<nRow;i++){
                    for(int j=0;j<nCol;j++){
                        int I=j+i*nCol;
                        if(I>=nKey)continue;//如果已超出范围，跳过--abc
                        Ckey*key=m_gameKeyList[I];
                        const float marginH=110;
                        float c[2]={width/nCol*(j+0.5),(height-marginH)-(height-marginH)/nRow*(i+0.5)+marginH/2};
                        key->reSetCenter(c[0], c[1]);
                        key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
                    }
                }
            }

        }
        //gameKeyList_lock
        {
            Cc3dTexture* ptexture_lockKey=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/pageTexs/homePageTex/lockKey.png");
            int nKey=(int)m_gameKeyList.size();//与gameKeyList元素数保持一致--abc
            for(int i=0;i<nKey;i++){
                Ckey*key=new Ckey();
                key->autorelease();
                float c[2]={0,0};
                float r=45;
                key->genAndInit(c[0]-r,c[0]+r,c[1]-r,c[1]+r, 0,1,0,1);
                key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
                key->setKeyDownColor(0.7, 0.7, 0.7, 1);
                key->setKeyUpColor(1, 1, 1, 1);
                key->useKeyUpColor();
                key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptexture_lockKey);
                key->setCamera(camera2D);
                key->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
                key->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
                key->setPassUnifoCallback(passUnifoCallback_noLight);
                addChild(key);
                m_gameKeyList_lock.push_back(key);
            }
            //定位--abc
            {
                int nKey_lock=(int)m_gameKeyList_lock.size();
                for(int i=0;i<nKey_lock;i++){
                    Ckey*key_lock=m_gameKeyList_lock[i];
                    Ckey*key=m_gameKeyList[i];
                    float c[2]={key->getCenterX(),key->getCenterY()};
                    key_lock->reSetCenter(c[0], c[1]);
                    key_lock->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
                }
            }
            //可见性--abc
            {
                int nKey=(int)m_gameKeyList_lock.size();
                for(int i=0;i<nKey;i++){
                    Ckey*key=m_gameKeyList_lock[i];
                    if(CuserConfig::sharedUserConfig()->unLockList[i]==false&&CgameSettings::sharedGameSettings()->getUnlockAll()==false){
                        key->setIsVisible(true);
                    }else{
                        key->setIsVisible(false);
                    }
                }
            }

        }
        //gameKeyList_star
        {
            m_tex_quad_starList.push_back(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_star0.png"));
            m_tex_quad_starList[(int)m_tex_quad_starList.size()-1]->retain();
            m_tex_quad_starList.push_back(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_star1.png"));
            m_tex_quad_starList[(int)m_tex_quad_starList.size()-1]->retain();
            m_tex_quad_starList.push_back(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_star2.png"));
            m_tex_quad_starList[(int)m_tex_quad_starList.size()-1]->retain();
            m_tex_quad_starList.push_back(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_star3.png"));
            m_tex_quad_starList[(int)m_tex_quad_starList.size()-1]->retain();
            int nKey=(int)m_gameKeyList.size();//与gameKeyList元素数保持一致--abc
            for(int i=0;i<nKey;i++){
                Ckey*key=new Ckey();
                key->autorelease();
                float c[2]={0,0};
                float r=45;
                key->genAndInit(c[0]-r,c[0]+r,c[1]-r/4,c[1]+r/4, 0,1,0,1);
                key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
                key->setKeyDownColor(0.7, 0.7, 0.7, 1);
                key->setKeyUpColor(1, 1, 1, 1);
                key->useKeyUpColor();
                key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(m_tex_quad_starList[CuserConfig::sharedUserConfig()->starNumberList[i]]);
                key->setCamera(camera2D);
                key->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
                key->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
                key->setPassUnifoCallback(passUnifoCallback_noLight);
                addChild(key);
                m_gameKeyList_star.push_back(key);
            }
            //定位--abc
            {
                int nKey_star=(int)m_gameKeyList_star.size();
                for(int i=0;i<nKey_star;i++){
                    Ckey*key_star=m_gameKeyList_star[i];
                    Ckey*key=m_gameKeyList[i];
                    float c[2]={key->getCenterX(),key->getCenterY()};
                    key_star->reSetCenter(c[0], c[1]-45);
                    key_star->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
                }
            }
            //可见性--abc
            {
                int nKey=(int)m_gameKeyList_star.size();
                for(int i=0;i<nKey;i++){
                    Ckey*key=m_gameKeyList_star[i];
                    if(CuserConfig::sharedUserConfig()->unLockList[i]==true||CgameSettings::sharedGameSettings()->getUnlockAll()==true){
                        key->setIsVisible(true);
                    }else{
                        key->setIsVisible(false);
                    }
                }
            }
        }
        
    }
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    return true;

}
void ChomePage::debugSwitch(){
    CgameSettings::sharedGameSettings()->setUnlockAll(!CgameSettings::sharedGameSettings()->getUnlockAll());
}
void ChomePage::update(float dt){
    float screenwidth=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    float screenheight=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
   
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
    //----debugKey
    {
        //debugKey响应单击--abc
        if(m_debugKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->debugSwitch();
        }
        //debugKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_debugKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())){
            m_debugKey->useKeyDownColor();
        }else{
            m_debugKey->useKeyUpColor();
        }
    }
    //----gameKey
    {
        int nKey=(int)m_gameKeyList.size();
        for(int i=0;i<nKey;i++){
            Ckey*key=m_gameKeyList[i];
            if(key->isGrey)continue;//跳过grey的key
            if(CuserConfig::sharedUserConfig()->unLockList[i]==false&&CgameSettings::sharedGameSettings()->getUnlockAll()==false)continue;//跳过未解锁的关--abc
            //key响应单击--abc
            if(i<CuserConfig::sharedUserConfig()->get_levelCount()){
                if(key->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
                    //curlevel=i+1;
                    CgameState::sharedGameState()->setCurrentLevel(i+1);
                    this->getALSourceByName("switchPage")->play();
                    CloadingPage*loadingPage=new CloadingPage();
                    loadingPage->autorelease();
                    loadingPage->init();
                    Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
                    Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(loadingPage);
                    
                    break;
                }
            }
            //key响应按下和抬起--abc
            if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&key->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
                key->useKeyDownColor();
            }else{
                key->useKeyUpColor();
            }
            
        }
        
    }
    //----------根据unlockAll决定是否显示锁--abc
    {
        int nlock=(int)m_gameKeyList_lock.size();
        for(int i=0;i<nlock;i++){
            Ckey*lock=m_gameKeyList_lock[i];
            bool unlocked=CuserConfig::sharedUserConfig()->unLockList[i];
            if(CgameSettings::sharedGameSettings()->getUnlockAll()){
                lock->setIsVisible(false);
            }else{
                if(unlocked){
                    lock->setIsVisible(false);
                }else{
                    lock->setIsVisible(true);
                }
            }
            
        }
        
    }

}

