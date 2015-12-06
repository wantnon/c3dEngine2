//
//  succedPage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "succedPage.h"
#include "globalVars.h"
#include "endPage.h"
#include "loadingPage.h"
#include "homePage.h"
#include "c3d.h"
#include "playingPage.h"
#include "define.h"
#include "gameState.h"
#include "userConfig.h"
#include "gestureAnalyzer.h"
bool CsuccedPage::init(int nStar,int nEnemyKilled,int nCoinEaten){
    const float wReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x();
    const float hReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y();
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    string resoStr=numberToStr(wReso)+"x"+numberToStr(hReso);
    const float keyMovUp=height-480;
    //----创建数字纹理和符号纹理--abc
    {
        for(int i=0;i<10;i++){
            string texPath="singleCellGameResource/data/global/tex/numberAndSymbolTex/"+numberToStr(i)+".png";
            Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage(texPath);
            m_numberTexList.push_back(ptex);
            ptex->retain();
        }
        m_dotTex=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/numberAndSymbolTex/dot.png");
        m_dotTex->retain();
    }
    //--quad_succeed
    {
        m_quad_succeed=new Ckey();
        m_quad_succeed->autorelease();
        //
        Cc3dTexture* ptexture_quad_succeed=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_succeed.png");
        float bx=0;
        float by=0;
        float w=220;
        float h=55;
        m_quad_succeed->genAndInit(bx, bx+w, by, by+h,0,1,0,1);
        m_quad_succeed->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_succeed->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptexture_quad_succeed);
        m_quad_succeed->setKeyUpColor(1, 1, 1, 1);
        m_quad_succeed->useKeyUpColor();
        m_quad_succeed->setCamera(camera2D);
        m_quad_succeed->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_succeed->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_succeed->setPassUnifoCallback(passUnifoCallback_noLight);
        m_quad_succeed->setVisitDrawOrder(10000);
        //
        float _bx=52;
        float _by=380;
        m_quad_succeed->reSetLeftDownCorner(_bx, _by+keyMovUp);
        m_quad_succeed->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_succeed);
        
    }
    //--quad_star
    {
        m_quad_star=new Ckey();
        m_quad_star->autorelease();
        //
        float bx=0;
        float by=0;
        float w=200;
        float h=50;
        m_quad_star->genAndInit(bx, bx+w, by, by+h,0,1,0,1);
        m_quad_star->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_star->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage(string("singleCellGameResource/data/global/tex/quads/quad_star")+numberToStr(nStar)+".png"));
        m_quad_star->setKeyUpColor(1, 1, 1, 1);
        m_quad_star->useKeyUpColor();
        m_quad_star->setCamera(camera2D);
        m_quad_star->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_star->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_star->setPassUnifoCallback(passUnifoCallback_noLight);
        m_quad_star->setVisitDrawOrder(10000);
        //
        float _bx=60;
        float _by=265;
        m_quad_star->reSetLeftDownCorner(_bx, _by+keyMovUp);
        m_quad_star->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_star);
    }
    
    //--nextLevelKey
    {
        m_nextLevelKey=new Ckey();
        m_nextLevelKey->autorelease();
        //
        Cc3dTexture* ptexture=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_nextLevel.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_nextLevelKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_nextLevelKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_nextLevelKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_nextLevelKey->setKeyUpColor(1, 1, 1, 1);
        m_nextLevelKey->useKeyUpColor();
        m_nextLevelKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptexture);
        m_nextLevelKey->setCamera(camera2D);
        m_nextLevelKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_nextLevelKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_nextLevelKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_nextLevelKey->setVisitDrawOrder(10000);
        //
        Cc3dVector4 center;
        if(CgameState::sharedGameState()->getCurrentLevel()==CuserConfig::sharedUserConfig()->get_levelCount()){
            center.init(160, 340+10+keyMovUp, 0,1);
        }else{
            center.init(223, 340+10+keyMovUp, 0,1);
        }
        m_nextLevelKey->reSetCenter(center.x(), center.y());
        m_nextLevelKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_nextLevelKey);
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
        m_homeKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_home.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false));
        m_homeKey->setCamera(camera2D);
        m_homeKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_homeKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_homeKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_homeKey->setVisitDrawOrder(10000);
        m_homeKey->setIsVisible(CgameState::sharedGameState()->getCurrentLevel()!=CuserConfig::sharedUserConfig()->get_levelCount());
        //
        float _c[2]={95,340+10+keyMovUp};
        m_homeKey->reSetCenter(_c[0], _c[1]);
        m_homeKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_homeKey);
    }
    //--restartKey
    {
        m_restartKey=new Ckey();
        m_restartKey->autorelease();
        //
        float c[2]={0,0};
        m_restartKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_restartKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_restartKey->setKeyDownColor(0.7, 0.7, 0.7, 1);
        m_restartKey->setKeyUpColor(1, 1, 1, 1);
        m_restartKey->useKeyUpColor();
        m_restartKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_restart.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false));
        m_restartKey->setCamera(camera2D);
        m_restartKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_restartKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_restartKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_restartKey->setVisitDrawOrder(10000);
        m_restartKey->setIsVisible(CgameState::sharedGameState()->getCurrentLevel()!=CuserConfig::sharedUserConfig()->get_levelCount());
        //
        float _c[2]={160,340+10+keyMovUp};
        m_restartKey->reSetCenter(_c[0],_c[1]);
        m_restartKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_restartKey);
    }
    //----quad_coin
    {
        m_quad_coin=new Ckey();
        m_quad_coin->autorelease();
        //
        Cc3dTexture* ptex_quad_coin=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_coin.png");
        m_quad_coin->genAndInit(0, 30, 0, 30,0,1,0,1);
        m_quad_coin->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_coin->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex_quad_coin);
        m_quad_coin->setKeyUpColor(1, 1, 1, 1);
        m_quad_coin->useKeyUpColor();
        m_quad_coin->setCamera(camera2D);
        m_quad_coin->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_coin->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_coin->setPassUnifoCallback(passUnifoCallback_noLight);
        m_quad_coin->setVisitDrawOrder(10000);
        //quad_coin
        float bx=100;
        float by=165-60;
        m_quad_coin->reSetLeftDownCorner(bx, by);
        m_quad_coin->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_coin);
    
    }
    //----quad_nCoinPositivePlaceList
    {
        //提取nCoin各位数字--abc
        int _nCoinEaten=nCoinEaten;
        vector<int> numberList;
        for(int i=0;i<nCoinPositivePlaceCount;i++){
            int number=_nCoinEaten%10;
            _nCoinEaten/=10;
            numberList.push_back(number);
        }
        for(int i=0;i<nCoinPositivePlaceCount;i++){//反序--abc
            if(i>=nCoinPositivePlaceCount/2)break;
            int temp=numberList[i];
            numberList[i]=numberList[nCoinPositivePlaceCount-i-1];
            numberList[nCoinPositivePlaceCount-i-1]=temp;
        }
       
        int nPositivePlace=nCoinPositivePlaceCount;
        for(int i=0;i<nPositivePlace;i++){
            
            Ckey*quad=new Ckey();//quad_nCoinPositivePlaceList[i];
            quad->genAndInit(0, 26, 0, 26,0,1,0,1);
            quad->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            quad->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(m_numberTexList[numberList[i]]);
            quad->setKeyUpColor(1, 1, 1, 0.8);
            quad->useKeyUpColor();
            quad->setCamera(camera2D);
            quad->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
            quad->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
            quad->setPassUnifoCallback(passUnifoCallback_noLight);
            quad->setVisitDrawOrder(10000);
            addChild(quad);
            m_quad_nCoinPositivePlaceList.push_back(quad);
            
        }

        //quad_nCoinPostivePlaceList
        float _bx=100+10;
        float _by=165-60;
        for(int i=0;i<nPositivePlace;i++){
            Ckey*quad=m_quad_nCoinPositivePlaceList[i];
            _bx+=20;
            quad->reSetLeftDownCorner(_bx, _by);
            quad->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        }
    }
    //----quad_killEnemy
    {
        m_quad_killEnemy=new Ckey();
        m_quad_killEnemy->autorelease();
        //
        Cc3dTexture* ptex_quad_killEnemy=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_killEnemy.png");
        m_quad_killEnemy->genAndInit(0, 30, 0, 30,0,1,0,1);
        m_quad_killEnemy->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_killEnemy->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex_quad_killEnemy);
        m_quad_killEnemy->setKeyUpColor(1, 1, 1, 1);
        m_quad_killEnemy->useKeyUpColor();
        m_quad_killEnemy->setCamera(camera2D);
        m_quad_killEnemy->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_killEnemy->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_killEnemy->setPassUnifoCallback(passUnifoCallback_noLight);
        m_quad_killEnemy->setVisitDrawOrder(10000);
        //
        float bx=100;
        float by=130-60;
        m_quad_killEnemy->reSetLeftDownCorner(bx, by);
        m_quad_killEnemy->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        addChild(m_quad_killEnemy);
        
    }
    //----quad_nKillEnemyPostivePlaceList
    {
        //提取nBoss各位数字--abc
        int _nEnemyKilled=nEnemyKilled;
        vector<int> numberList;
        for(int i=0;i<nCoinPositivePlaceCount;i++){
            int number=_nEnemyKilled%10;
            _nEnemyKilled/=10;
            numberList.push_back(number);
        }
        for(int i=0;i<nCoinPositivePlaceCount;i++){//反序--abc
            if(i>=nCoinPositivePlaceCount/2)break;
            int temp=numberList[i];
            numberList[i]=numberList[nCoinPositivePlaceCount-i-1];
            numberList[nCoinPositivePlaceCount-i-1]=temp;
        }

        
        int nPositivePlace=nKillEnemyPostivePlaceCount;
        for(int i=0;i<nPositivePlace;i++){
            Ckey*quad=new Ckey();//quad_nKillEnemyPositivePlaceList[i];
            quad->genAndInit(0, 26, 0, 26,0,1,0,1);
            quad->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            quad->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(m_numberTexList[numberList[i]]);
            quad->setKeyUpColor(1, 1, 1, 0.8);
            quad->useKeyUpColor();
            quad->setCamera(camera2D);
            quad->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
            quad->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
            quad->setPassUnifoCallback(passUnifoCallback_noLight);
            quad->setVisitDrawOrder(10000);
            addChild(quad);
            m_quad_nKillEnemyPositivePlaceList.push_back(quad);
        }
        //
        float _bx=100+10;
        float _by=130-60;
        for(int i=0;i<nPositivePlace;i++){
            Ckey*quad=m_quad_nKillEnemyPositivePlaceList[i];
            _bx+=20;
            quad->reSetLeftDownCorner(_bx, _by);
            quad->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        }
    }
    
    //----quad_keyList
    {
        m_tex_quad_key_blackWhite=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_geneCube_blackWhite.png");
        m_tex_quad_key_blackWhite->retain();
        m_tex_quad_key_colored=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_geneCube.png");
        m_tex_quad_key_colored->retain();
        int nKey=1;
        for(int i=0;i<nKey;i++){
            Ckey*quad_key=new Ckey();//quad_keyList[i];
            quad_key->genAndInit(0, 30, 0, 30,0,1,0,1);
            quad_key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            quad_key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(m_tex_quad_key_blackWhite);
            quad_key->setKeyUpColor(1, 1, 1, 1);
            quad_key->useKeyUpColor();
            quad_key->setCamera(camera2D);
            quad_key->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
            quad_key->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
            quad_key->setPassUnifoCallback(passUnifoCallback_noLight);
            quad_key->setVisitDrawOrder(10000);
            addChild(quad_key);
            m_quad_keyList.push_back(quad_key);
        }

        
        //在quad_keyList中先前nkey个quad_key进行绘制--abc
        float by=145;
        float xBeginMin=100;
        float xEndMax=500;
        //在xbeginMin和xEndMax区间内画nKey个quad_key
        //为此重新设定各quad_key的大小--abc
        float keyWidth=minf((xEndMax-xBeginMin)/nKey,50);
        float keyHeight=keyWidth;
        for(int i=0;i<nKey;i++){
            Ckey*quad_key=m_quad_keyList[i];
            quad_key->reSetWidth(keyWidth);
            quad_key->reSetHeight(keyHeight);
        }
        //重设各quad_key的位置（通过左下角)
        for(int i=0;i<nKey;i++){
            Ckey*quad_key=m_quad_keyList[i];
            quad_key->reSetLeftDownCorner(xBeginMin+keyWidth*i, by);
        }
        //将各quad_key纹理都设置为彩色--abc
        for(int i=0;i<nKey;i++){
            Ckey*quad_key=m_quad_keyList[i];
            quad_key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(m_tex_quad_key_colored);
        }
        //提交--abc
        for(int i=0;i<nKey;i++){
            Ckey*quad_key=m_quad_keyList[i];
            quad_key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        }
        
    }
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    return true;
}
void CsuccedPage::update(float dt){
    //----nextLevelKey
    //nextLevelKey响应单击--abc
    if(m_nextLevelKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
        if(CgameState::sharedGameState()->getCurrentLevel()==CuserConfig::sharedUserConfig()->get_levelCount()){//通过了最后一关--abc
            CendPage*endPage=new CendPage();
            endPage->autorelease();
            endPage->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(endPage);
            
        }else{//如果还没有过完最后一关，进入下一关--abc
            //进入加载--abc
            //curlevel++;
            CgameState::sharedGameState()->setCurrentLevel(CgameState::sharedGameState()->getCurrentLevel()+1);
            CloadingPage*loadingPage=new CloadingPage();
            loadingPage->autorelease();
            loadingPage->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(loadingPage);
        }
    }
    //nextLevelKey响应按下和抬起--abc
    if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_nextLevelKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
        m_nextLevelKey->useKeyDownColor();
    }else{
        m_nextLevelKey->useKeyUpColor();
    }
    if(CgameState::sharedGameState()->getCurrentLevel()!=CuserConfig::sharedUserConfig()->get_levelCount()){//不是最后一关才响应下面按键--abc
        //----homeKey
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
        //----restartKey
        if(m_restartKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
    //        gameState=GAME_STATE_loading;
            CloadingPage*loadingPage=new CloadingPage();
            loadingPage->autorelease();
            loadingPage->init();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
            Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(loadingPage);
        }
        //restartKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_restartKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_restartKey->useKeyDownColor();
        }else{
            m_restartKey->useKeyUpColor();
        }
    }

}
CsuccedPage::CsuccedPage(){

}
CsuccedPage::~CsuccedPage(){
    for(int i=0;i<10;i++){
        m_numberTexList[i]->release();
    }
    m_dotTex->release();
    m_tex_quad_key_blackWhite->release();
    m_tex_quad_key_colored->release();
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();

}
