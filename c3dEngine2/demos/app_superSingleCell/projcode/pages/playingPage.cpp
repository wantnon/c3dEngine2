//
//  playingPage.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-9.
//
//

#include "playingPage.h"

#include "globalVars.h"
#include "makeTemplate.h"
#include "pausePage.h"
#include "viewingTipPage.h"
#include "succedPage.h"
#include "gameOverPage.h"
#include "c3d.h"
#include "define.h"
#include "changeNodeComponents.h"
#include "c3d.h"
#include "gameSettings.h"
#include "gameState.h"
#include "userConfig.h"
#include "gestureAnalyzer.h"
 CplayingPage:: CplayingPage(){
    m_cell=NULL;
    m_ground=NULL;
    m_seabed=NULL;
    m_water=NULL;
    m_archScene=NULL;
    m_levelBackGroundMusicCube=NULL;
    m_depthTex_got=false;
  
 
}
 CplayingPage::~CplayingPage(){
    m_fbo->release();
    m_depthTex->release();
    m_fbo2->release();
    m_depthTex2->release();
    //
    for(int i=0;i<(int)m_numberTexList.size();i++){
        m_numberTexList[i]->release();
    }
    m_dotTex->release();
    m_tex_quad_key_blackWhite->release();
    m_tex_quad_key_colored->release();
    //
    unloadGame();
    //----remove unused resources in caches on next frame
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
}
void  CplayingPage::unloadGame(){
    cout<<"unload Game..."<<endl;
    /*  if(ground)assert(ground->getRetainCount()==1);
     if(water)assert(water->getRetainCount()==1);
     if(seabed)assert(seabed->getRetainCount()==1);
     Cc3dSceneManager::sharedSceneManager()->releaseCurrentScene();*/
    m_ground=NULL;
    m_water=NULL;
    m_seabed=NULL;
	//恢复gameSetting
	CgameSettings::sharedGameSettings()->reset();
    //恢复备份或重置--abc
    //  *cell=*cell_backup;
    m_quad_halfFuelHaveBeenUsed->haveBeenDisplayed=false;//以便下一关还能再display

    //----卸载模型--abc
    {
        m_mobilePlatformSet.clear();
        m_shotBlastSet.clear();
        m_bubbleBlastSet.clear();
        m_fakeLightSet.clear();
        m_bubbleSet.clear();
        m_bossCellSet.clear();
        m_shipSet.clear();
        m_meteorSet.clear();
      
    }
    //----卸载碰撞网格--abc
    m_collisionGrid_boss.clear();
    m_collisionGrid_mobilePlatform.clear();
    m_collisionGrid_bubble.clear();
    //----停止本actor的各alsource，注意，并不是停止所有alsource
    //Cc3dAudioCache::sharedAudioCache()->stopAllPlayingSource();
    this->stopAllALSource();
    cout<<"unload game done!"<<endl;
}
void  CplayingPage::debugSwitch(){
    CgameSettings::sharedGameSettings()->setShowBubbleID(!CgameSettings::sharedGameSettings()->getShowBubbleID());
    {
        int n=m_bubbleSet.getActorCount();
        for(int i=0;i<n;i++){
            Cbubble*p=(Cbubble*)m_bubbleSet.getActorByIndex(i);
            p->getIDBoard0()->setIsVisible(CgameSettings::sharedGameSettings()->getShowBubbleID());
            p->getIDBoard1()->setIsVisible(CgameSettings::sharedGameSettings()->getShowBubbleID());
        }
        
    }
    CgameSettings::sharedGameSettings()->setShowMobilePlatformID(!CgameSettings::sharedGameSettings()->getShowMobilePlatformID());
    {
        int n=m_mobilePlatformSet.getActorCount();
        for(int i=0;i<n;i++){
            CmobilePlatform*p=(CmobilePlatform*)m_mobilePlatformSet.getActorByIndex(i);
            p->getIDBoard0()->setIsVisible(CgameSettings::sharedGameSettings()->getShowMobilePlatformID());
            p->getIDBoard1()->setIsVisible(CgameSettings::sharedGameSettings()->getShowMobilePlatformID());
        }
        
    }
    CgameSettings::sharedGameSettings()->setShowDepthTexs(!CgameSettings::sharedGameSettings()->getShowDepthTexs());
    {
        m_quad_depthTex->setIsVisible(CgameSettings::sharedGameSettings()->getShowDepthTexs());
        m_quad_depthTex2->setIsVisible(CgameSettings::sharedGameSettings()->getShowDepthTexs());
    }
    
	CgameSettings::sharedGameSettings()->setShowMusicCube(!CgameSettings::sharedGameSettings()->getShowMusicCube());
	{
		m_levelBackGroundMusicCube->setIsVisible(CgameSettings::sharedGameSettings()->getShowMusicCube());
	}
    CgameSettings::sharedGameSettings()->setGroundIsWireMode(!CgameSettings::sharedGameSettings()->getGroundIsWireMode());
	{
		if(m_ground)m_ground->setIsWireMode(CgameSettings::sharedGameSettings()->getGroundIsWireMode());
	}
    
	

}
void  CplayingPage::update(float dt){
    //---------------------logic
    //必须将logic放在这里，不要将logic放在update里!!!，否则造成卡顿--abc
    this->logic();
    //------------响应按键--abc
    vector<Ckey*> pKeyList;//按键列表--abc
    {
        //----pauseKey
        pKeyList.push_back(m_pauseKey);
        //pauseKey响应单击--abc
        if(m_pauseKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            Cc3dAudioCache::sharedAudioCache()->pauseAllPlayingSource();//暂停所有正在播放的source
            CpausePage*pausePage=new CpausePage();
            pausePage->autorelease();
            pausePage->init();
            pausePage->setVisitDrawOrder(10000);
            this->addChild(pausePage);
            this->setIsDoUpdate(false);
        }
        //pauseKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_pauseKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())){
            m_pauseKey->useKeyDownColor();
        }else{
            m_pauseKey->useKeyUpColor();
        }
        //----debugKey
        pKeyList.push_back(m_debugKey);
        //debugKey响应单击--abc
        if(m_debugKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->debugSwitch();
        }
        //debugKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_debugKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_debugKey->useKeyDownColor();
        }else{
            m_debugKey->useKeyUpColor();
        }
        //----tipKey
        pKeyList.push_back(m_tipKey);
        //tipKey响应单击--abc
        if(m_tipKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
            this->getALSourceByName("switchPage")->play();
            CviewingTipPage*viewingTipPage=new CviewingTipPage();
            viewingTipPage->autorelease();
            viewingTipPage->init();
            viewingTipPage->setVisitDrawOrder(10000);
            this->addChild(viewingTipPage);
            this->setIsDoUpdate(false);
        }
        //tipKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_tipKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())){
            m_tipKey->useKeyDownColor();
        }else{
            m_tipKey->useKeyUpColor();
        }
    
        //----zoomOutKey
        pKeyList.push_back(m_zoomOutKey);
        //zoomOutKey响应按下和抬起--abc
        if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_zoomOutKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
            m_zoomOutKey->useKeyDownColor();
            m_eyePosInCell=(Cc3dVector3(eyePosInCell_normal)*k_eyePosInCell_zoomOut).toV4(1);
        }else{
            m_zoomOutKey->useKeyUpColor();
            m_eyePosInCell=Cc3dVector3(eyePosInCell_normal).toV4(1);
        }
        //----goDownShipKey
        pKeyList.push_back(m_goDownShipKey);
        if(m_cell->get_onShip()){
            //goDownShipKey响应单击--abc
            if(m_goDownShipKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
                Cship*pship=m_cell->get_ship();
                if(m_cell->get_ship()->getALSourceByName("shipEngineSound")){
                    m_cell->get_ship()->getALSourceByName("shipEngineSound")->stop();//飞船停止声音,必须放在getDownShip之前--abc
                }
                m_cell->getDownShip();//下船--abc
                m_goDownShipKey->setIsVisible(false);
                m_goDownShipKey->setIsDoUpdate(false);
                
                float margin=5;
                Cc3dVector4 up=m_cell->getUp();
                up=normalize(up);
                up=up*(pship->Rc_getOnShip+m_cell->getRc()-pship->seatPos_local.y()+margin);
                m_cell->getTransform()->moveRelativeToFather(up.x(), up.y(), up.z());
                m_cell->doJump(2.5/2);
            }
            //goDownShipKey响应按下和抬起--abc
            if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_goDownShipKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())){
                m_goDownShipKey->useKeyDownColor();
            }else{
                m_goDownShipKey->useKeyUpColor();
            }
        }
        //----shot
        pKeyList.push_back(m_shotKey);
        {

     //       if(m_shotKey->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint()) &&Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsTapTwice()){
     //           inputSignal.shotKeyTapTwice=true;
     //       }else{
     //           inputSignal.shotKeyTapTwice=false;
     //       }
            if(m_shotKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) &&CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()){
                //inputSignal.shotKeyDown.set(true);
                m_shotKey->setIsDown(true);
            }else{
                //inputSignal.shotKeyDown.set(false);
                m_shotKey->setIsDown(false);
            }
            //shotKey按下抬起--abc
            if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_shotKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())){
                m_shotKey->useKeyDownColor();
            }else{
                m_shotKey->useKeyUpColor();
            }
            
            
        }
        //----shotBig
        pKeyList.push_back(m_shotBigKey);
        {
            if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsTapOnce()&&m_shotBigKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
                //inputSignal.shotBigKeyDown.set(true);
                m_shotBigKey->setIsDown(true);
            }else{
                //inputSignal.shotBigKeyDown.set(false);
                m_shotBigKey->setIsDown(false);
            }
            //shotBigKey按下和抬起--abc
            if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&m_shotBigKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint()) ){
                m_shotBigKey->useKeyDownColor();
            }else{
                m_shotBigKey->useKeyUpColor();
            }
        }
        
        
    }//得到pKeyList和haveKeyEvent
    //检查mos是否在对某个Key进行操作--abc
    bool usingKey=false;
    {
        int nKey=(int)pKeyList.size();
        for(int i=0;i<nKey;i++){
            Ckey*pKey=pKeyList[i];
            //如果mos在pKey上且刚按下时也在pKey上，则认为在操作Key
            if(pKey->isContainPoint(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint())){
                usingKey=true;
                break;
            }
        }//得到usingKey
    }
    //   cout<<"Cc3dGestureAnalyzer::sharedGestureAnalyzer()->movSpeed:"<<Cc3dGestureAnalyzer::sharedGestureAnalyzer()->movSpeed.x<<" "<<Cc3dGestureAnalyzer::sharedGestureAnalyzer()->movSpeed.y<<endl;
    //----------------------手势--abc
    {
        //----dnKey
        if(m_cell->get_onShip()){
            //下滑--abc
            if(usingKey==false&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()
               &&fabs(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().x()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().x())<maxf( 10,CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().y())
               &&CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().y()-CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().y()>10){
                //inputSignal.downKeyDown.set(true);
                m_cell->dnSignal=true;
            }else{
                //inputSignal.downKeyDown.set(false);
                m_cell->dnSignal=false;
            }
            
        }
        //----upKey
        if(m_cell->get_onShip()){
            //上滑--abc
            if(usingKey==false&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()
               &&fabs(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().x()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().x())<maxf(10,CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().y()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().y())
               &&CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().y()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().y()>10){
                //inputSignal.upKeyDown.set(true);
                m_cell->upSignal=true;
            }else{
                //inputSignal.upKeyDown.set(false);
                m_cell->upSignal=false;
            }
        }
        //----jump
        if(m_cell->get_onShip()==false){
            //上滑--abc
            if(usingKey==false&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()
               &&fabs(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().x()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().x())<maxf(10,CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().y()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().y())
               &&CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().y()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().y()>10){
                //inputSignal.jumpKeyDown.set(true);
                m_cell->jumpSignal=true;
            }else{
                //inputSignal.jumpKeyDown.set(false);
                m_cell->jumpSignal=false;
            }
            
        }
        
        //----rotKey
        {
            //左右滑--abc
            if(usingKey==false&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()
               &&fabsf(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().y()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().y())<maxf(20,2*fabsf(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().x()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().x()))
               &&fabsf(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().x()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().x())>6){
                //inputSignal.rotKeyDown.set(true);
                m_cell->rotSignal=true;
            }
            if(CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()==false){
                //inputSignal.rotKeyDown.set(false);
                m_cell->rotSignal=false;
            }
            
            
            
        }
        
        //----back
        if(m_cell->get_onShip()==false){
            //下滑--abc
            if(usingKey==false&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()
               &&fabs(CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().x()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().x())<maxf(10,CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().y())
               &&CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganPoint().y()-CgestureAnalyzer::sharedGestureAnalyzer()->getPoint().y()>6){
                //inputSignal.backKeyDown.set(true);
                //inputSignal.forwardKeyDown.set(false);
                //inputSignal.jumpForwardKeyDown.set(false);
                m_cell->backSignal=true;
                m_cell->forwardSignal=false;
                m_cell->jumpForwardSignal=false;
            }else{
                //inputSignal.backKeyDown.set(false);
                m_cell->backSignal=false;
            }
            
        }
        //----jumpForward
        {
            int span=2;
            if(usingKey==false&&m_cell->isTouchSurface(3.0)==false&&CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()
               &&Cc3dFrameCounter::sharedFrameCounter()->getCount()-CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganTime()>span
               &&//inputSignal.backKeyDown.get()==false
               m_cell->backSignal==false
               ){
                //inputSignal.jumpForwardKeyDown.set(true);
                m_cell->jumpForwardSignal=true;
            }else{
                //inputSignal.jumpForwardKeyDown.set(false);
                m_cell->jumpForwardSignal=false;
            }
        }
        //----forward
        //一定要放在最后，因为可能需要使用其它的inputSignal进行控制--abc
        {
   //         cout<<"touch move speed: "<<Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getTouchMoveSpeed().x()<<" "<<Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getTouchMoveSpeed().y()<<endl;
            //如果Cc3dGestureAnalyzer::sharedGestureAnalyzer()->isDown==true且time_down到现在大于span，且从time_down到现在所有的坐标都局限在一个较小的范围内--abc
            //则认为是一直按住一点--abc
            long latestTouchBeganTime=CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganTime();
            long downTime=Cc3dFrameCounter::sharedFrameCounter()->getCount()-latestTouchBeganTime;
     /*       cout<<"latestTouchBeganTime: "<<latestTouchBeganTime<<endl;
            cout<<"timeCount: "<<timeCounter.getCount()<<endl;
            cout<<"downTime: "<<downTime<<endl;
            cout<<"isDown: "<<Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsDown()<<endl;
      */
            long span=4;
            if(usingKey==false
               &&//inputSignal.rotKeyDown.get()==false
               m_cell->rotSignal==false
               &&(m_cell->get_onShip()?
                  //!inputSignal.upKeyDown.get()&&!inputSignal.downKeyDown.get()
                  !m_cell->upSignal&&!m_cell->dnSignal
                  //:!inputSignal.backKeyDown.get()&&!inputSignal.jumpKeyDown.get())
                  :!m_cell->backSignal&&!m_cell->jumpSignal)
               &&CgestureAnalyzer::sharedGestureAnalyzer()->getIsDown()
               &&downTime>span
               ){
                long timeRef=CgestureAnalyzer::sharedGestureAnalyzer()->getLatestTouchBeganTime();
                //求Cc3dGestureAnalyzer::sharedGestureAnalyzer()->posList中所有时间戳在timeRef之后者的坐标范围--abc
                float xmin=c3d_INF;
                float ymin=c3d_INF;
                float xmax=-c3d_INF;
                float ymax=-c3d_INF;
                int count_laterThanTimeRef=0;//晚于timeRef的坐标的个数--abc
                const vector<Ctouch> posList=CtouchSequence::sharedTouchSequence()->getTouchList();
                int nPos=(int)posList.size();
                for(int i=0;i<nPos;i++){
                    const Ctouch&xy=posList[i];
                    float x=xy.getPoint().x();
                    float y=xy.getPoint().y();
                    long time=xy.getTime();
                    if(time>=timeRef){
                        count_laterThanTimeRef++;
                        if(x>xmax)xmax=x;
                        if(y>ymax)ymax=y;
                        if(x<xmin)xmin=x;
                        if(y<ymin)ymin=y;
                    }
                }
        //        cout<<"range:"<<xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<endl;
                
                if(count_laterThanTimeRef==0
                   ||(count_laterThanTimeRef!=0&&fabs(xmax-xmin)<5&&fabs(ymax-ymin)<5)
                   ){//如果最近span时间内没有坐标变化，或者此间坐标变化范围较小，则认为是按住一点不动--abc
                    //inputSignal.forwardKeyDown.set(true);
                    m_cell->forwardSignal=true;
                }else{
                    //inputSignal.forwardKeyDown.set(false);
                    m_cell->forwardSignal=false;
                }
            }else{
                //inputSignal.forwardKeyDown.set(false);
                m_cell->forwardSignal=false;
            }
            
            
            
        }
        
        
    }

    //------------游戏结束或过关--abc
    if(this->getIsGameOver()){//没血了且已从摔扁恢复--abc
   //     gameState=GAME_STATE_gameover;
        Cc3dAudioCache::sharedAudioCache()->stopAllPlayingSource();//停止所有声音--abc
        this->getALSourceByName("gameOver")->play();//gameOver声音--abc
        //--
        CgameOverPage*gameOverPage=new CgameOverPage();
        gameOverPage->autorelease();
        gameOverPage->init();
        gameOverPage->setVisitDrawOrder(10000);
        this->addChild(gameOverPage);
        this->setIsDoUpdate(false);
    }
    if(this->getIsSucc()){
     //   gameState=GAME_STATE_succeed;
        Cc3dAudioCache::sharedAudioCache()->stopAllPlayingSource();//停止所有声音--abc
        this->getALSourceByName("levelDone")->play();//播放胜利声音--abc
        //计算本关获得星数，并更新gameKeyList_star的相应元素--abc
        int nEnemyKilledCount=this->getEnemyKilledCount();
        int nCoinEatenCount=this->getCoinEatenCount();
        int nStar;//获得星数--abc
        {
            //----计算星数--abc
            int nBoss=m_bossCellSet.getActorCount();
            int nCoin=m_bubbleSet.getCoinBubbleCount();
            float divee=(float)(nEnemyKilledCount+nCoinEatenCount);//被除数--abc
            float diver=(float)(nBoss+nCoin);//除数--abc
            cout<<divee<<" "<<diver<<endl;
            float rate=(diver==0)?1:divee/diver;
            if(rate<1.0/3){
                nStar=0;
            }else if(rate<2.0/3){
                nStar=1;
            }else if(rate<1){
                nStar=2;
            }else{
                nStar=3;
            }//得到nStar
        }
        //解锁下一关并写入文件--abc
        if(CgameSettings::sharedGameSettings()->getUnlockAll()==false){//在unlockAll开启的情况下解锁的关卡不存储，将来还得再解--abc
            //int index_nextLevel=(curlevel-1)+1;
            int index_nextLevel=(CgameState::sharedGameState()->getCurrentLevel()-1)+1;
            CuserConfig::sharedUserConfig()->unLockList[index_nextLevel]=true;
            CuserConfig::sharedUserConfig()->starNumberList[index_nextLevel-1]=nStar;
            CuserConfig::sharedUserConfig()->writeToConfigFileInDocuments("userConfig_superSingleCell.txt");
        }
        //输出userConig内容--abc
        cout<<"unlockList:"<<endl;
        for(int i=0;i<CuserConfig::sharedUserConfig()->unLockList.size();i++){
            cout<<CuserConfig::sharedUserConfig()->unLockList[i]<<" ";
        }cout<<endl;
        cout<<"starNumberList:"<<endl;
        for(int i=0;i<CuserConfig::sharedUserConfig()->starNumberList.size();i++){
            cout<<CuserConfig::sharedUserConfig()->starNumberList[i]<<" ";
        }cout<<endl;
        //将this中所有界面元素隐藏--abc
        for(int i=0;i<(int)pKeyList.size();i++){
            Ckey*key=pKeyList[i];
            key->setIsVisible(false);
        }
        //进入成功界面--abc
        CsuccedPage*succedPage=new CsuccedPage();

        succedPage->autorelease();
        succedPage->init(nStar,nEnemyKilledCount,nCoinEatenCount);
        succedPage->setVisitDrawOrder(10000);
        this->addChild(succedPage);
        this->setIsDoUpdate(false);
        //将uiNode及子节点隐藏--abc
        this->m_uiRoot->setIsDoUpdateRecursively(false);
        this->m_uiRoot->setIsVisibleRecursively(false);
        
        
    }
    


}
bool  CplayingPage::init(int level){
    const float wReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x();
    const float hReso=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y();
    const float width=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x();
    const float height=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y();
    string resoStr=numberToStr(wReso)+"x"+numberToStr(hReso);
    m_level=level;
    cout<<"load level..."<<endl;
    cout<<"level"<<level<<endl;
   
    //----重新渲染shadow map
    m_depthTex_got=false;
    //----eyePosInCell
    m_eyePosInCell=Cc3dVector4(eyePosInCell_normal);
    //----初始化模型--abc
    //将level转成字符串--abc
    string levelStr=numberToStr(level);
    //制作根路径--abc
    string rootPath="singleCellGameResource/data/levels/level_"+levelStr;
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

    //----cell
    //拷贝模板--abc
    m_cell=makeCellTemplate(5);
    //腿的转速--abc
    m_cell->set_v_legsRot(80);
    //核颜色--abc
    m_cell->getModel()->getMeshList()[CELLPART_core]->setDiffuseRGB(0.7,0,0);
    m_cell->getModel()->getMeshList()[CELLPART_core]->setAmbient(0.4,0,0);
    //
    m_cell->getModel()->getMeshList()[CELLPART_body]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_specular_noSelfShadow"));
    m_cell->getModel()->getMeshList()[CELLPART_body]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_specular_noSelfShadow);
    m_cell->getModel()->getMeshList()[CELLPART_body]->setVisitDrawOrder(1000);
    
    m_cell->getModel()->getMeshList()[CELLPART_wingL]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_specular_noSelfShadow"));
    m_cell->getModel()->getMeshList()[CELLPART_wingL]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_specular_noSelfShadow);
    
    m_cell->getModel()->getMeshList()[CELLPART_wingR]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_specular_noSelfShadow"));
    m_cell->getModel()->getMeshList()[CELLPART_wingR]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_specular_noSelfShadow);
    
    m_cell->getModel()->getMeshList()[CELLPART_core]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
    m_cell->getModel()->getMeshList()[CELLPART_core]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
    
    m_cell->getModel()->getMeshList()[CELLPART_ene]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
    m_cell->getModel()->getMeshList()[CELLPART_ene]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
    
    m_cell->getModel()->getMeshList()[CELLPART_shell]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
    m_cell->getModel()->getMeshList()[CELLPART_shell]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
    
    m_cell->getModel()->getMeshList()[CELLPART_tooth]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
    m_cell->getModel()->getMeshList()[CELLPART_tooth]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
    
    m_cell->getModel()->getMeshList()[CELLPART_eye]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
    m_cell->getModel()->getMeshList()[CELLPART_eye]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
    
    m_cell->setLight(&light0);
    m_cell->setCamera(camera);
    
    //声音--abc
    {
     //   m_cell->pALSource_ya=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/ya.wav");
     //   m_cell->pALSsource_fall_in_water=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/fall_in_water.wav");
     //   m_cell->pALSource_water_puddle=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/water_puddle.wav");
     //   m_cell->pALSource_move_shasha=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/move_shasha.wav");
     //   m_cell->pALSource_alert_out_of_fuel=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/alert_out_of_fuel.wav");
     //   m_cell->pALSource_oops=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/oops.wav");
     //   m_cell->pALSource_fallFlat_paa_oops=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/paa_oops.wav");
     //   m_cell->pALSource_onFire=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/onFire.wav");
     //   m_cell->pALSource_onFire->setIsLooping(true);
        
        Cc3dALSource*pALSource_ya=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/ya.wav","ya");
        Cc3dALSource*pALSsource_fall_in_water=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/fall_in_water.wav","fall_in_water");
        Cc3dALSource*pALSource_water_puddle=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/water_puddle.wav","water_puddle");
        Cc3dALSource*pALSource_move_shasha=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/move_shasha.wav","move_shasha");
        Cc3dALSource*pALSource_alert_out_of_fuel=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/alert_out_of_fuel.wav","alert_out_of_fuel");
        Cc3dALSource*pALSource_oops=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/oops.wav","oops");
        Cc3dALSource*pALSource_fallFlat_paa_oops=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/paa_oops.wav","fallFlat_paa_oops");
        Cc3dALSource*pALSource_onFire=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/onFire.wav","onFire");
        
        m_cell->addALSource(pALSource_ya);
        m_cell->addALSource(pALSsource_fall_in_water);
        m_cell->addALSource(pALSource_water_puddle);
        m_cell->addALSource(pALSource_move_shasha);
        m_cell->addALSource(pALSource_alert_out_of_fuel);
        m_cell->addALSource(pALSource_oops);
        m_cell->addALSource(pALSource_fallFlat_paa_oops);
        m_cell->addALSource(pALSource_onFire);
        
        m_cell->getALSourceByName("onFire")->setIsLooping(true);
        
        
        
        
    }
    //生命值和燃料值--abc
    m_cell->life.set_maxValue(200);
    m_cell->life.chargeFull();
    //生成cell.pshotBallList
    int nEachShotBallType=40;//对应于每个shotBall模板生成多少个shotBall
    int nReuseALSourcePercentage=0.9;//重用ALSource的比例--abc
    int j=0;
    while(1){
        //生成nEachShotBallType个shotBall
        bool jumpOutOfWhile=false;
        for(int k=0;k<nEachShotBallType;k++){
            bool isReuseALSource=(nEachShotBallType%(int)(1-nReuseALSourcePercentage)!=0);
            CshotBall*pball=makeShotBallTemplate(j,isReuseALSource);
            if(pball==NULL){
                jumpOutOfWhile=true;
                break;
            }
            pball->setLight(&light0);
            pball->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
            pball->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
            pball->setCamera(camera);
            m_cell->addShotBall(pball);
         //   pball->retain();
            pball->setIsVisible(false);
        }
        if(jumpOutOfWhile)break;
        j++;
        
    }
    m_cell->setVisitDrawOrder(1000);
    
    m_cell->setName("cell");
    
    //----cell和相机--abc
  //  assert(cell->getFather()==NULL);
    this->addChild(m_cell);
    for(int i=0;i<(int)m_cell->getShotBallList().size();i++){
        CshotBall*ball=m_cell->getShotBallList()[i];
        this->addChild(ball);
    }
    //cell初始信息--abc
    m_cell->loadConfig(rootPath+"/modelInfo/cellInfo/cellInfo.txt");
    //--------相机--abc
    //根据cell的初始位置设置相机位置--abc
    float eyeUp[4]={0,1,0,0};
    Cc3dVector4 eyePos=m_cell->getTransform()->convertToWorldSpace(m_eyePosInCell);
    m_cell->isJump.set(true);//cell起初是跳起的--abc

    camera->setEyePos(eyePos);
    camera->setCenter(m_cell->getTransform()->getPos());
    camera->setUp(eyeUp);
    camera->updateFrustum();
    
    cout<<"cell shotBall count:"<<(int)m_cell->getShotBallList().size()<<endl;
    
    //创建fireList
    {
        m_fireList.clear();
        //fireList的元素数--abc
        const int nFire=3;
        //fireList中各元素的大小--abc
        vector<float> rxList(nFire),ryList(nFire);//(rxList[i],ryList[i])为cell.pmoleculeList[i]的横纵边心距--abc
        rxList[0]=9*1.5*1.2;rxList[1]=9;rxList[2]=9;
        ryList[0]=9*1.5*2;ryList[1]=9*2;ryList[2]=9*2;
        for(int i=0;i<nFire;i++){
            Cmolecule*pmolecule=new Cmolecule();
            pmolecule->autorelease();
            pmolecule->setDied(false);
            
            float rx=rxList[i];
            float ry=ryList[i];
            pmolecule->init(rx,ry);
            //setProgram, setPassUnifoCallback, setIsVisible,setIsWriteDepthBuffer,setVisitDrawOrder等--abc
            //都必须放在init之后，否则无法生效--abc
            pmolecule->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_rgb011Transp"));
            pmolecule->setPassUnifoCallback(passUnifoCallback_texOnly_blackTransp);
            pmolecule->setCamera(camera);
            pmolecule->setIsVisible(false);
            pmolecule->getMesh()->setIsWriteDepthBuffer(false);
            pmolecule->setVisitDrawOrder(1001);
            pmolecule->setRc(0);
            pmolecule->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            string texClipName="singleCellGameResource/data/global/tex/moleculeLibrary/fire3/";
            pmolecule->loadFrameTexList(texClipName);
            pmolecule->useTex(0);
            pmolecule->getMesh()->getSubMeshByIndex(0)->submitIndex(GL_STATIC_DRAW);
            pmolecule->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STATIC_DRAW);
            m_fireList.push_back(pmolecule);
            this->addChild(pmolecule);
            
        }
    }
    
    //----light
    {
        loadInfo_light(&light0,rootPath+"/modelInfo/lightInfo/lightInfo.txt");
        light0.setDiffuse(1, 1, 1);
        light0.setAmbient(1, 1, 1);
        light0.setSpecular(1, 1, 1);
        
    }
    
    //--shadowMap
    {
        //----fbo and depthTex
        m_fbo=new Cc3dFbo();
        m_fbo->autorelease();
        m_fbo->retain();
        m_depthTex=new Cc3dTexture();
        m_depthTex->autorelease();
        m_depthTex->retain();
        int shadowMapSize=minf(4096,Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getSupportedMaxTextureSize());
        m_depthTex->initDepthTexture(shadowMapSize,shadowMapSize,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_NEAREST,GL_NEAREST);
        m_fbo->attachDepthTex(m_depthTex);
        m_fbo2=new Cc3dFbo();
        m_fbo2->autorelease();
        m_fbo2->retain();
        m_depthTex2=new Cc3dTexture();
        m_depthTex2->autorelease();
        m_depthTex2->retain();
        int shadowMapSize2=shadowMapSize/4;//(shadowMapSize==4096?512:256);
        m_depthTex2->initDepthTexture(shadowMapSize2,shadowMapSize2,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_NEAREST,GL_NEAREST);
        m_fbo2->attachDepthTex(m_depthTex2);
        
        //----shadowMapInfo
        m_shadowMapInfo.loadConfig(rootPath+"/modelInfo/shadowMapInfo/shadowMap1.txt");
        m_shadowMapInfo2.loadConfig(rootPath+"/modelInfo/shadowMapInfo/shadowMap2.txt");
        //----camera_light
        Cc3dCamera*lightViewCamera=new Cc3dCamera();
        lightViewCamera->autorelease();
        
        Cc3dCamera*lightViewCamera2=new Cc3dCamera();
        lightViewCamera2->autorelease();
        //
        lightViewCamera->setFovy(m_shadowMapInfo.lightCamAngle);
        lightViewCamera->setAspect(((float)m_depthTex->getWidth())/m_depthTex->getHeight());
        lightViewCamera->setzNear(600);//近截面在保证不裁掉物体的前提下尽量取大些，能更允分利用深度纹理的精度--abc
        lightViewCamera->setzFar(10000);
        
        lightViewCamera2->setFovy(m_shadowMapInfo2.lightCamAngle);
        lightViewCamera2->setAspect(((float)m_depthTex2->getWidth())/m_depthTex2->getHeight());
        lightViewCamera2->setzNear(600);//近截面在保证不裁掉物体的前提下尽量取大些，能更允分利用深度纹理的精度--abc
        lightViewCamera2->setzFar(10000);
        //
        light0.addLightViewCamera(lightViewCamera);
        light0.addLightViewCamera(lightViewCamera2);
        //
        glActiveTexture(GL_TEXTURE1);//挂shadow map1
        glBindTexture(GL_TEXTURE_2D, m_depthTex->getID());
        glActiveTexture(GL_TEXTURE2);//挂shadow map2
        glBindTexture(GL_TEXTURE_2D, m_depthTex2->getID());
        //回到texture0
        glActiveTexture(GL_TEXTURE0);
    }
    //--sky
    {
        bool haveSky=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(rootPath+"/sky");
        if(haveSky){
            m_sky=new CskySemiBall();
            m_sky->init();
            m_sky->autorelease();
            loadInfo_mesh(m_sky->getMesh(), "singleCellGameResource/data/global/model/sky");
            m_sky->scaleToFitR(r_sky);
            m_sky->getMesh()->genVBOBuffers();
            m_sky->getMesh()->submit(GL_STATIC_DRAW);
            
            m_sky->setName("sky");
            this->addChild(m_sky);
            //         sky->set_isExist(true);
            m_sky->loadConfig(rootPath+"/sky/config.txt");
            //更换纹理--abc
            string skyTexPathShort="singleCellGameResource/data/global/tex/skyTexLibrary/sky";
            skyTexPathShort+=numberToStr(m_sky->get_skyTexID())+".png";
            Cc3dTexture* pskyTex=Cc3dTextureCache::sharedTextureCache()->addImage(skyTexPathShort);
            m_sky->getMesh()->getSubMeshByIndex(0)->setTexture(pskyTex);
            m_sky->setCamera(camera);
        }else{
            //     sky->set_isExist(false);
        }
    }
    
    //--water
    
    {
        bool haveWater=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(rootPath+"/water");
        if(haveWater){
            m_water=new Cwater();
            m_water->init();
            m_water->autorelease();
            m_water->setName("water");
            this->addChild(m_water);
            m_water->getMesh()->setDiffuseAlpha(0.5);
            //preflectSquare
            {
                //----创建water.preflectSquare
                Csprite*preflectSquare=new Csprite();
                preflectSquare->setDied(false);
                preflectSquare->setIsVisible(true);
                float r=r_sky+1000;
                preflectSquare->init(r,r);
                preflectSquare->setRc(r);
                //pos
                {
                    Cc3dVector4 pos=m_cell->getTransform()->getPos();
                    preflectSquare->setPos(pos);
                };
                preflectSquare->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        //        cout<<"reflect map path:"<<rootPath+"/water/reflect.png"<<endl;
                Cc3dTexture* ptexture_reflect=Cc3dTextureCache::sharedTextureCache()->addImage(rootPath+"/water/reflect.png");
                preflectSquare->getMesh()->getSubMeshByIndex(0)->setTexture(ptexture_reflect);
                preflectSquare->getMesh()->getSubMeshByIndex(0)->submitIndex(GL_STATIC_DRAW);
                preflectSquare->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STATIC_DRAW);
                preflectSquare->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_water"));

                preflectSquare->setPassUnifoCallback(passUnifoCallback_water);
                preflectSquare->setCamera(camera);
                preflectSquare->getMesh()->setIsWriteDepthBuffer(false);
                m_water->setReflectSquare(preflectSquare);
                
      //          preflectSquare->setIsVisible(true);
            }
            //ptexSquare
            {
                //----创建water.ptexSquare
                Csprite*pArabesquiticSquare=new Csprite();
                pArabesquiticSquare->setDied(false);
                pArabesquiticSquare->setIsVisible(true);
                float r=r_sky+1000;
                pArabesquiticSquare->init(r,r);
                pArabesquiticSquare->setRc(r);
                //pos
                {
                    Cc3dVector4 pos=m_cell->getTransform()->getPos();
                    pArabesquiticSquare->setPos(pos);
                };
                pArabesquiticSquare->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
                pArabesquiticSquare->getMesh()->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/water_alpha.png"));
                pArabesquiticSquare->getMesh()->getSubMeshByIndex(0)->submitIndex(GL_STATIC_DRAW);
                pArabesquiticSquare->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STATIC_DRAW);
                pArabesquiticSquare->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_useTexAsAlpha"));
                pArabesquiticSquare->setPassUnifoCallback(passUnifoCallback_texOnly_useTexAsAlpha);
                pArabesquiticSquare->setCamera(camera);
                pArabesquiticSquare->getMesh()->setIsWriteDepthBuffer(false);
                m_water->setArabesquiticSquare(pArabesquiticSquare);
                
         //       pArabesquiticSquare->setIsVisible(true);
            }
            
            //pcenterSquare
            {
                //----创建water.pcenterSquare
                Cmolecule*pcenterSquare=new Cmolecule();
                pcenterSquare->setDied(false);
                pcenterSquare->setIsVisible(true);
                float r=60;
                pcenterSquare->init(r,r);
                pcenterSquare->setRc(r);
                //pos
                {
                    Cc3dVector4 pos=m_cell->getTransform()->getPos();
                    pcenterSquare->setPos(pos);
                };
                pcenterSquare->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
                pcenterSquare->loadFrameTexList("singleCellGameResource/data/global/tex/moleculeLibrary/ripple/");
                pcenterSquare->useTex(0);
                pcenterSquare->getMesh()->getSubMeshByIndex(0)->submitIndex(GL_STATIC_DRAW);
                pcenterSquare->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STATIC_DRAW);
                pcenterSquare->useLastTex();//播到最后一帧--abc
                pcenterSquare->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_useTexAsAlpha"));
                pcenterSquare->setPassUnifoCallback(passUnifoCallback_texOnly_useTexAsAlpha);
                pcenterSquare->setCamera(camera);
                pcenterSquare->getMesh()->setIsWriteDepthBuffer(false);
                m_water->setCenterSquare(pcenterSquare);
                m_water->setVisitDrawOrder(999);
                
            //    pcenterSquare->setIsVisible(false);
                
            }
            //声音--abc
            m_water->pALSource_water_gulu=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/water_gulu.wav","water_gulu");
            //----
            m_water->set_isExist(true);
            m_water->loadConfig(rootPath+"/water/config.txt");

            //water停止水波动画，即将水波动画播到最后--abc
            m_water->getCenterSquare()->useLastTex();
            
            m_water->setCamera(camera);
        }else{
            m_water->set_isExist(false) ;
        }
        
        
        
    }
    
    //--ground
    bool landClipExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(rootPath+"/land");
    bool landMapExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(rootPath+"/land/land.bmp");
    
    if(landClipExist&&landMapExist)
    {
        m_ground=new Cground();
        m_ground->autorelease();
        m_ground->setName("ground");
        this->addChild(m_ground);
        
        //加载数据并生成地形--abc
        
        //----读取配置--abc
        m_ground->loadConfig(rootPath+"/land/config.txt");
        //----地面--abc
        //加载texture
        string texFilePathShort=rootPath+"/land/singleTex/texture_2048x2048.png";
        Cc3dTexture*texture=Cc3dTextureCache::sharedTextureCache()->addImage(texFilePathShort);
        //初始化ground
        Cc3dRect rect(-blockWidth/2, -blockHeight/2, blockWidth, blockHeight);
        m_ground->initWithFile(rootPath+"/land/land.bmp",rect,m_ground->getHeightScale(),9,texture);
        m_ground->setUVScale(1);
        m_ground->getMesh()->setDiffuseRGB(1, 1, 1);
        m_ground->getMesh()->setAmbient(0.2, 0.2, 0.2);
        m_ground->setCamera(camera);
        m_ground->setLight(&light0);
        m_ground->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noTransf_noSelfShadow);
        m_ground->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noTransf_noSelfShadow"));
        m_ground->setReso(10);
        //提交数据--abc
        m_ground->submitVertex(GL_STATIC_DRAW);
        m_ground->getMesh()->getSubMeshByIndex(0)->clearMeshData();
        
    }
    //seabed
    {
        bool seabedTexExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(rootPath+"/land/seabed.png");
        if(seabedTexExist){
            m_seabed=new Cseabed();
            m_seabed->autorelease();
            m_seabed->setName("seabed");
            this->addChild(m_seabed);
            
            m_seabed->loadConfig(rootPath+"/land/config_seabed.txt");
            Cc3dTexture* texture=Cc3dTextureCache::sharedTextureCache()->addImage(rootPath+"/land/seabed.png");
            m_seabed->init(r_sky, texture);
            m_seabed->getModel()->getMeshList()[0]->setDiffuseRGB(1, 1, 1);
            m_seabed->getModel()->getMeshList()[0]->setAmbient(1, 1, 1);
            m_seabed->setCamera(camera);
            //制作和提交seabed
            m_seabed->getModel()->getMeshList()[0]->submit(GL_STATIC_DRAW);
            
    //        m_seabed->setIsVisible(false);
        }
        
    }
    
    
    //--archScene
    {
        m_archScene=new CarchScene();
        m_archScene->autorelease();
        m_archScene->setName("archScene");
        this->addChild(m_archScene);
        m_archScene->init(rootPath+"/scene/config.txt",rootPath+"/scene");
        m_archScene->getModel()->getMeshByIndex(0)->setDiffuseRGB(0.8, 0.8, 0.8);
        m_archScene->getModel()->getMeshByIndex(0)->setAmbient(0.5, 0.5, 0.5);
        m_archScene->getModel()->getMeshByIndex(0)->setColor(1, 1, 1, 0.7);//设置透明度，但在show_solid时不开启混合，在show_transp时开启混合--abc
        m_archScene->setLight(&light0);
        m_archScene->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noTransf_shadowMap"));
        m_archScene->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noTransf_shadowMap);
        m_archScene->setCamera(camera);
        
    }
    
    //----shotBlastSet
    {
        Cc3dALSource *pALSource_bigBlast=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/bigBlast.wav","bigBlast");
        int nBlast=40;
        int nBlast_big=10;
        vector<string> texClipPathShortList;
        vector<string> blastClipNameList;
        vector<Cc3dALSource*> pALSourceList;
        vector<float> rList;
        for(int i=0;i<nBlast;i++){
            string blastClipName="blast_small";
            texClipPathShortList.push_back("singleCellGameResource/data/global/tex/moleculeLibrary/"+blastClipName+"/");
            blastClipNameList.push_back(blastClipName);
            rList.push_back(6);
            pALSourceList.push_back(NULL);
        }
        for(int i=0;i<nBlast_big;i++){
            string blastClipName="blast_big0";
            texClipPathShortList.push_back("singleCellGameResource/data/global/tex/moleculeLibrary/"+blastClipName+"/");
            blastClipNameList.push_back(blastClipName);
            rList.push_back(20);
            pALSourceList.push_back(pALSource_bigBlast);
        }
        int nBlastTotal=(int)texClipPathShortList.size();
        for(int i=0;i<nBlastTotal;i++){
            Cblast*pblast=new Cblast();
            pblast->autorelease();
            pblast->setName("shot blast i");
            this->addChild(pblast);
            pblast->setDied(true);
            pblast->setIsVisible(false);
            string texClipPathShort=texClipPathShortList[i];
            pblast->loadFrameTexList(texClipPathShort);
            pblast->set_blastType(blastClipNameList[i]);
            float r_molecule=rList[i];
            float rx_molecule,ry_molecule;
            if(pblast->wDivh>=1){//让rx和ry中较大者等于r
                rx_molecule=r_molecule;
                ry_molecule=r_molecule/pblast->wDivh;
            }else{
                ry_molecule=r_molecule;
                rx_molecule=r_molecule*pblast->wDivh;
            }
            pblast->init(rx_molecule,ry_molecule);
            float Rc=rx_molecule;
            pblast->setRc(Rc);
            pblast->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            pblast->useTex(0);
            if(pblast->effectType==MOLECULEEFFECTTYPE_useColorValueAsAlpha){
                pblast->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_useTexAsAlpha"));
                pblast->setPassUnifoCallback(passUnifoCallback_texOnly_useTexAsAlpha);
            }else if(pblast->effectType==MOLECULEEFFECTTYPE_specifiedColorTransp){
                pblast->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_rgb011Transp"));
                pblast->setPassUnifoCallback(passUnifoCallback_texOnly_blackTransp);
            }else{
                assert(false);
            }
            pblast->setCamera(camera);
            pblast->getMesh()->setIsWriteDepthBuffer(false);
            pblast->setVisitDrawOrder(1001);
            pblast->getMesh()->submit(GL_STATIC_DRAW);
            if(pALSourceList[i]){
                pblast->addALSource(pALSourceList[i]);
            }
            m_shotBlastSet.addActor((Cc3dMesh*)pblast);
        }
        
    }
    //----bubbleBlastSet
    {
        string blastClipName="blast_big0";
        int n=10;
        for(int i=0;i<n;i++){
            Cblast*pblast=new Cblast();
            pblast->autorelease();
            pblast->setName("bubble blast i");
            this->addChild(pblast);
            pblast->setDied(true);
            pblast->setIsVisible(false);
            string texClipName="singleCellGameResource/data/global/tex/moleculeLibrary/"+blastClipName+"/";
            pblast->loadFrameTexList(texClipName);
            pblast->set_blastType(blastClipName);
            float r_molecule=20;
            float rx_molecule,ry_molecule;
            if(pblast->wDivh>=1){//让rx和ry中较大者等于r
                rx_molecule=r_molecule;
                ry_molecule=r_molecule/pblast->wDivh;
            }else{
                ry_molecule=r_molecule;
                rx_molecule=r_molecule*pblast->wDivh;
            }
            pblast->init(rx_molecule,ry_molecule);
            float Rc=rx_molecule;
            pblast->setRc(Rc);
            pblast->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_rgb011Transp"));
            pblast->setPassUnifoCallback(passUnifoCallback_texOnly_blackTransp);
            pblast->setCamera(camera);
            pblast->getMesh()->setIsWriteDepthBuffer(false);
            pblast->setVisitDrawOrder(1001);
            pblast->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            pblast->useTex(0);
            pblast->getMesh()->submit(GL_STATIC_DRAW);
            m_bubbleBlastSet.addActor((Cc3dMesh*)pblast);
        }
    }
    
    //--mobilePlatformSet
    {
        Cc3dALSource*pALSource_woodBreak=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/woodBreak.wav","destroySound");
        Cc3dALSource*pALSource_plat_bouncing=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/bouncing.wav","plat_bouncing");
        m_mobilePlatformSet.loadConfig(rootPath+"/modelInfo/mobilePlatformInfo/mobilePlatformInfo.txt");
        int nPlat=(int)m_mobilePlatformSet.getActorCount();
        for(int i=0;i<nPlat;i++){
            CmobilePlatform*pmobilePlatform=(CmobilePlatform*)m_mobilePlatformSet.getActorByIndex(i);
            this->addChild(pmobilePlatform);
            
            //----生成或加载模型--abc
            if(pmobilePlatform->geoTypeName=="cube_program"){//程序生成cube
                Cc3dSubMesh*subMesh=new Cc3dSubMesh();
                subMesh->autorelease();
                pmobilePlatform->getMesh()->addSubMesh(subMesh);
                Cc3dSubMeshData*meshData=genSampleCubeWithCode(0, 0, 0, pmobilePlatform->rx,pmobilePlatform->ry, pmobilePlatform->rz,
                                                         0.01,0.01,cubUVMode_UNIFORM);
                subMesh->setMeshData(meshData);
            }else{//导入模型--abc
                string modelClipPathShort="singleCellGameResource/data/global/model/platformModelLibrary/";
                modelClipPathShort+=pmobilePlatform->geoTypeName;
                loadInfo_mesh(pmobilePlatform->getMesh(), modelClipPathShort);
                //对模型以模型坐标系原点为中心进行放缩，使其包围盒尺寸等于2*pmobilePlatform->rx,2*pmobilePlatform->ry, 2*pmobilePlatform->rz
                pmobilePlatform->scaleToFitSurroundingBoxMatchingRxRyRz();
            }

            
            //----设置texture
            //判断是用platformTexLibrary中的贴图还是用自带贴图--abc
            if(pmobilePlatform->texImageName=="null"){//用自带贴图--abc
                //不用做任何事--abc
            }else{//用platformTexLibrary中的贴图--abc
                Cc3dTexture *ptex=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/platformTexLibrary/"+pmobilePlatform->texImageName);
                int nSubMesh=(int)pmobilePlatform->getMesh()->getSubMeshCount();
                for(int i=0;i<nSubMesh;i++){
                    Cc3dSubMesh*subMesh=pmobilePlatform->getMesh()->getSubMeshByIndex(i);
                    subMesh->setTexture(ptex);//重设贴图--abc
                }
            }
            //其它初始工作--abc
            int nSubMesh=(int)pmobilePlatform->getMesh()->getSubMeshCount();
            for(int i=0;i<nSubMesh;i++){
                Cc3dSubMesh*subMesh=pmobilePlatform->getMesh()->getSubMeshByIndex(i);
                reCalNorms_precise_normByFace(subMesh->getSubMeshData());
       //         subMesh->setMeshData());
                //存储顶点--abc
                pmobilePlatform->vlistStoreList.push_back(subMesh->getSubMeshData()->getvlist());
                //生成vbo缓冲并提交顶点--abc
                subMesh->getIndexVBO()->genBuffers();
                subMesh->submit(GL_STATIC_DRAW);
            }
            pmobilePlatform->getMesh()->setDiffuseRGB(0.8, 0.8, 0.8);
            pmobilePlatform->getMesh()->setAmbient(0.5,0.5,0.5);
            pmobilePlatform->gen_pIDtriExList();
            pmobilePlatform->updateAndSubmitVertex(true);//无条件更新vlist
            pmobilePlatform->setLight(&light0);
            pmobilePlatform->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noTransf_noSelfShadow"));
            pmobilePlatform->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noTransf_noSelfShadow);
            pmobilePlatform->setCamera(camera);
            pmobilePlatform->setIsIgnorTransform(true);
            //声音--abc
            if(pmobilePlatform->fileName_motionSoundAfterTrigger=="null"){
                //     pmobilePlatform->removeALSourceByName("plat_motion");
                //pmobilePlatform->pALSource_plat_motion=&nullSource;
            }else{
                string audioFilePathShort="singleCellGameResource/data/global/audio/";
                audioFilePathShort+=pmobilePlatform->fileName_motionSoundAfterTrigger;
                Cc3dALSource*source=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(audioFilePathShort,"plat_motion",false);
                source->setIsLooping(pmobilePlatform->motionSoundAfterTrigger_looping);
                pmobilePlatform->addALSource(source);
                
                //    pmobilePlatform->pALSource_plat_motion=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(audioFilePathShort);
                //    pmobilePlatform->pALSource_plat_motion->setIsLooping(pmobilePlatform->motionSoundAfterTrigger_looping);
                
            }
            pmobilePlatform->addALSource(pALSource_plat_bouncing);
            //   pmobilePlatform->pALSource_plat_bouncing=pALSource_plat_bouncing;
            if(pmobilePlatform->get_destroyable()){
                pmobilePlatform->addALSource(pALSource_woodBreak);
                //    pmobilePlatform->pALSource_destroySound=pALSource_woodBreak;
            }
            //初始化IDboard
            {
                int ID=pmobilePlatform->ID;
                if(ID>99){
                    cout<<"error:ID>99，目前无法在屏幕上显示三位数!"<<endl;
                    assert(false);
                }
                //IDboard0
                Csprite*IDboard0=new Csprite();
                IDboard0->autorelease();
                pmobilePlatform->setIDBoard0(IDboard0);
                IDboard0->init(10, 10);
                IDboard0->getMesh()->setColor(0.7, 0.7, 1, 1);
                IDboard0->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
                IDboard0->getMesh()->getSubMeshByIndex(0)->setTexture(m_numberTexList[(ID/10)%10]);
                IDboard0->getMesh()->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);//submitIndex(GL_STATIC_DRAW);
                IDboard0->getMesh()->setIsDoDepthTest(false);
                IDboard0->getMesh()->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
                IDboard0->getMesh()->setPassUnifoCallback(passUnifoCallback_noLight);
                IDboard0->getMesh()->setCamera(camera);
                IDboard0->setIsVisible(CgameSettings::sharedGameSettings()->getShowMobilePlatformID());
                IDboard0->setVisitDrawOrder(10000);
                //IDboard1
                Csprite*IDboard1=new Csprite();
                IDboard1->autorelease();
                pmobilePlatform->setIDBoard1(IDboard1);
                IDboard1->init(10, 10);
                IDboard1->getMesh()->setColor(0.7, 0.7, 1, 1);
                IDboard1->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
                IDboard1->getMesh()->getSubMeshByIndex(0)->setTexture(m_numberTexList[ID%10]);
                IDboard1->getMesh()->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);//submitIndex(GL_STATIC_DRAW);
                IDboard1->getMesh()->setIsDoDepthTest(false);
                IDboard1->getMesh()->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
                IDboard1->getMesh()->setPassUnifoCallback(passUnifoCallback_noLight);
                IDboard1->setCamera(camera);
                IDboard1->setIsVisible(CgameSettings::sharedGameSettings()->getShowMobilePlatformID());
                IDboard1->setVisitDrawOrder(10000);
            }
        }
    }

    //----bubbleSet
    {
        //生成bubble
        m_bubbleSet.loadConfig(rootPath+"/modelInfo/bubbleInfo/bubbleInfo.txt");
        int nBubble=(int)m_bubbleSet.getActorCount();
        for(int i=0;i<nBubble;i++){
            Cbubble*pbubble=(Cbubble*)m_bubbleSet.getActorByIndex(i);
            pbubble->setName("bubble i");
            this->addChild(pbubble);
            pbubble->setDied(false);
            pbubble->setIsVisible(true);
            pbubble->life.set_maxValue(24);
            pbubble->life.chargeFull();
            pbubble->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_useTexAsAlpha"));
            pbubble->setPassUnifoCallback(passUnifoCallback_texOnly_useTexAsAlpha);
            pbubble->setCamera(camera);
            pbubble->getMesh()->setIsWriteDepthBuffer(false);
            pbubble->setVisitDrawOrder(1001);
            pbubble->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            string bubbleTexPath="singleCellGameResource/data/global/tex/sprites/"+pbubble->name+".png";
            pbubble->getMesh()->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage(bubbleTexPath));
            pbubble->getMesh()->getSubMeshByIndex(0)->submitIndex(GL_STATIC_DRAW);
            pbubble->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STATIC_DRAW);
            //----声音--abc
            if(pbubble->fileName_sound_eaten!="null"){
                string filePathShort_bubble_eaten="singleCellGameResource/data/global/audio/bubble_eaten/";
                filePathShort_bubble_eaten+=pbubble->fileName_sound_eaten;
             //   pbubble->pALSource_eaten=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(filePathShort_bubble_eaten);
                Cc3dALSource*pALSource_eaten=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(filePathShort_bubble_eaten,"eaten");
                pbubble->addALSource(pALSource_eaten);
            }
            //----pbubble->pmolecule
            Cmolecule*pmolecule=pbubble->getMolecule();
            pmolecule->setDied(false);
            pmolecule->setIsVisible(true);
            string texClipName="singleCellGameResource/data/global/tex/moleculeLibrary/"+pmolecule->name+"/";
            pmolecule->loadFrameTexList(texClipName);
            float r_bubble=(pbubble->getRx()+pbubble->getRy())/2;
            float r_molecule=r_bubble*0.7;
            float rx_molecule,ry_molecule;
            if(pmolecule->wDivh>=1){//让rx和ry中较大者等于r
                rx_molecule=r_molecule;
                ry_molecule=r_molecule/pmolecule->wDivh;
            }else{
                ry_molecule=r_molecule;
                rx_molecule=r_molecule*pmolecule->wDivh;
            }
            pmolecule->init(rx_molecule,ry_molecule);
            pmolecule->setRc(r_molecule);
            pmolecule->useTex(0);
            if(isEqual(pmolecule->getTranspColor(),Cc3dVector4(0,0,0,1))){
                pmolecule->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_rgb011Transp"));
                pmolecule->setPassUnifoCallback(passUnifoCallback_texOnly_blackTransp);
            }else if(isEqual(pmolecule->getTranspColor(),Cc3dVector4(0,1,1,1))){
                pmolecule->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_rgb011Transp"));
                pmolecule->setPassUnifoCallback(passUnifoCallback_texOnly_brightBlueTransp);
            }else{
                assert(false);//还没处理--abc
            }
            pmolecule->setCamera(camera);
            pmolecule->getMesh()->setIsWriteDepthBuffer(false);
            pmolecule->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            pmolecule->getMesh()->getSubMeshByIndex(0)->submitIndex(GL_STATIC_DRAW);
            pmolecule->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STATIC_DRAW);
            for(int j=0;j<(int)pmolecule->getMesh()->getSubMeshCount();j++){
                pmolecule->getMesh()->getSubMeshByIndex(j)->setName("molecule submesh j");
            }
            
            //初始化IDboard
            {
                int ID=pbubble->ID;
                if(ID>99){
                    cout<<"error:ID>99，目前无法在屏幕上显示三位数!"<<endl;
                    assert(false);
                }
                //IDboard0
                Csprite*IDboard0=new Csprite();
                IDboard0->autorelease();
                pbubble->setIDBoard0(IDboard0);
                IDboard0->init(10, 10);
                IDboard0->getMesh()->setColor(1, 1, 0, 1);
                IDboard0->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
                IDboard0->getMesh()->getSubMeshByIndex(0)->setTexture(m_numberTexList[(ID/10)%10]);
                IDboard0->getMesh()->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);//submitIndex(GL_STATIC_DRAW);
                IDboard0->getMesh()->setIsDoDepthTest(false);
                IDboard0->getMesh()->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
                IDboard0->getMesh()->setPassUnifoCallback(passUnifoCallback_noLight);
                IDboard0->setCamera(camera);
                IDboard0->setIsVisible(CgameSettings::sharedGameSettings()->getShowBubbleID());
                IDboard0->setVisitDrawOrder(10000);
                //IDboard1
                Csprite*IDboard1=new Csprite();
                IDboard1->autorelease();
                pbubble->setIDBoard1(IDboard1);
                IDboard1->init(10, 10);
                IDboard1->getMesh()->setColor(1, 1, 0, 1);
                IDboard1->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
                IDboard1->getMesh()->getSubMeshByIndex(0)->setTexture(m_numberTexList[ID%10]);
                IDboard1->getMesh()->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);//submitIndex(GL_STATIC_DRAW);
                IDboard1->getMesh()->setIsDoDepthTest(false);
                IDboard1->getMesh()->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
                IDboard1->getMesh()->setPassUnifoCallback(passUnifoCallback_noLight);
                IDboard1->setCamera(camera);
                IDboard1->setIsVisible(CgameSettings::sharedGameSettings()->getShowBubbleID());
                IDboard1->setVisitDrawOrder(10000);
                
                
            }
            
        }
        //统计bubbleSet中func为keyAdd的元素个数，填充cell.keyRemain
        {
            int nBubble=(int)m_bubbleSet.getActorCount();
            int count=0;
            for(int i=0;i<nBubble;i++){
                Cbubble*pbubble=(Cbubble*)m_bubbleSet.getActorByIndex(i);
                if(pbubble->getMolecule()->funcID==MOLECULE_FUNC_keyAdd){
                    count++;
                }
            }//得到count
            m_cell->nkey=count;
            assert(count==1);
    /*        if(m_cell->nkey>maxKeyCount){
                cout<<"error:关卡钥匙数量超过了预定限制!"<<endl;
                exit(0);
            }*/
        }
        
    }
    //----fakeLightSet
    {
        //加载fakeLightInfo
        m_fakeLightSet.loadConfig(rootPath+"/modelInfo/fakeLightInfo/fakeLightInfo.txt");
        //生成fakeLight
        int nfakeLight=(int)m_fakeLightSet.getActorCount();
        for(int i=0;i<nfakeLight;i++){
            CfakeLight*pfakeLight=(CfakeLight*)m_fakeLightSet.getActorByIndex(i);
            pfakeLight->setName("fake light i");
            pfakeLight->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_useTexAsAlpha"));
            pfakeLight->setPassUnifoCallback(passUnifoCallback_texOnly_useTexAsAlpha);
            pfakeLight->setCamera(camera);
            pfakeLight->getMesh()->setIsWriteDepthBuffer(false);
            pfakeLight->setVisitDrawOrder(1001);
            this->addChild(pfakeLight);
            pfakeLight->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            pfakeLight->getMesh()->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/sprites/glow_sprite_big.png"));
            pfakeLight->getMesh()->getSubMeshByIndex(0)->submitIndex(GL_STATIC_DRAW);
            pfakeLight->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STATIC_DRAW);
        }
    }
    
    //----shipSet
    {
        //检查是否存在ship的信息文件，如果没有就不加载ship
        bool haveInfo=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(rootPath+"/modelInfo/shipInfo");
        if(haveInfo){
            //加载shipInfo
            m_shipSet.loadConfig(rootPath+"/modelInfo/shipInfo/shipInfo.txt");
            int nship=(int)m_shipSet.getActorCount();
            for(int i=0;i<nship;i++){
                Cship*ship=(Cship*)m_shipSet.getActorByIndex(i);
                ship->setName("ship i");
                ship->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
                ship->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
                ship->setCamera(camera);
                ship->setLight(&light0);
                this->addChild(ship);
            }
        }
    }
    
    //----bossCellSet
    {
        //加载bossInfo
        m_bossCellSet.loadConfig(rootPath+"/modelInfo/bossInfo/bossInfo.txt");
        //补写及改写参数值--abc
        int nBoss=m_bossCellSet.getActorCount();
        for(int i=0;i<nBoss;i++){
            CbossCell*pbossCell=(CbossCell*)m_bossCellSet.getActorByIndex(i);
            pbossCell->setName("boss cell i");
            this->addChild(pbossCell);
            //
            pbossCell->getModel()->getMeshList()[CELLPART_body]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_specular_noSelfShadow"));
            pbossCell->getModel()->getMeshList()[CELLPART_body]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_specular_noSelfShadow);
            
            pbossCell->getModel()->getMeshList()[CELLPART_core]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
            pbossCell->getModel()->getMeshList()[CELLPART_core]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
            
            pbossCell->getModel()->getMeshList()[CELLPART_ene]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
            pbossCell->getModel()->getMeshList()[CELLPART_ene]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
            
            pbossCell->getModel()->getMeshList()[CELLPART_shell]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
            pbossCell->getModel()->getMeshList()[CELLPART_shell]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
            
            pbossCell->getModel()->getMeshList()[CELLPART_tooth]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
            pbossCell->getModel()->getMeshList()[CELLPART_tooth]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
            
            pbossCell->getModel()->getMeshList()[CELLPART_eye]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
            pbossCell->getModel()->getMeshList()[CELLPART_eye]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
            
            pbossCell->setLight(&light0);
            pbossCell->setCamera(camera);
            
            //rotLegs
            if(pbossCell->v_legsRot!=0)pbossCell->rotLegs=true;
            //重力加速度--abc
            pbossCell->set_a_gravity(0.05*4);
            //生命值--abc
            if(pbossCell->get_isFly()){
                pbossCell->life.set_maxValue(4);//由于飞行boss不容易打中，所以将其生命值设小些--abc
            }else{
                pbossCell->life.set_maxValue(8);
            }
            pbossCell->life.chargeFull();
        }
    }
    //----meteorSet
    {
        assert(m_meteorSet.getActorCount()==0);
        int nMeteor=5;
        for(int i=0;i<nMeteor;i++){
            Cmeteor*pmeteor=new Cmeteor();
            pmeteor->autorelease();
            pmeteor->setName("meteor");
            this->addChild(pmeteor);
            pmeteor->setDied(false);
            pmeteor->setIsVisible(true);
            float rx=500*0.6;
            float ry=500*0.6;
            int duringTime=200+rand()%50;
            pmeteor->init(duringTime,rx,ry);
            pmeteor->set_speed(rand()%30+30);
            pmeteor->setRc(0);
            Cc3dVector4 pos(0,-200,-r_sky*0.7,1);
            pmeteor->setPos(pos);
            pmeteor->setDied(true);
            pmeteor->setIsVisible(false);
            pmeteor->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_useTexAsAlpha"));
            pmeteor->setPassUnifoCallback(passUnifoCallback_texOnly_useTexAsAlpha);
            pmeteor->setCamera(camera);
            pmeteor->getMesh()->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            pmeteor->getMesh()->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/sprites/meteor.png"));
            pmeteor->getMesh()->getSubMeshByIndex(0)->submitIndex(GL_STATIC_DRAW);
            pmeteor->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STATIC_DRAW);
            m_meteorSet.addActor(pmeteor);
        }
    }
    
    //----levelBackGroundMusicCube
    {
        m_levelBackGroundMusicCube=new CbackGroundMusicCube();
        m_levelBackGroundMusicCube->autorelease();
        //
        Cc3dMesh*mesh=new Cc3dMesh();
        mesh->autorelease();
        loadInfo_mesh(mesh, "singleCellGameResource/data/global/model/musicCube");
        mesh->scale_modifyVertexDirectly(0.1, 0.1, 0.1);
        mesh->genVBOBuffers();
        mesh->submit(GL_STATIC_DRAW);
        m_levelBackGroundMusicCube->addMesh(mesh);
        m_levelBackGroundMusicCube->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_levelBackGroundMusicCube->setPassUnifoCallback(passUnifoCallback_noLight);
        m_levelBackGroundMusicCube->setCamera(camera);//如果物体显示不出来，可能是忘记了设置相机--abc
        m_levelBackGroundMusicCube->setIsVisible(false);
        //
        m_levelBackGroundMusicCube->loadConfig(rootPath+"/backGroundMusic/config.txt");
        if(m_levelBackGroundMusicCube->audioFileName!="null"){
            string audioFilePathShort="singleCellGameResource/data/global/audio/backGroundMusic/";
            audioFilePathShort+=m_levelBackGroundMusicCube->audioFileName;
          //  assert(levelBackGroundMusicCube.pALSource->getIsValidSource()==false);
          //  m_levelBackGroundMusicCube->pALSource=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(audioFilePathShort);
          //  m_levelBackGroundMusicCube->pALSource->setIsLooping(true);
          //  m_levelBackGroundMusicCube->pALSource->setGain(1.0);
          //  m_levelBackGroundMusicCube->pALSource->updatePosition(m_levelBackGroundMusicCube->sourcePos);
            Cc3dALSource*source=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(audioFilePathShort,"levelbackGroundMusic");
            m_levelBackGroundMusicCube->addALSource(source);
            m_levelBackGroundMusicCube->getALSourceByName("levelbackGroundMusic")->setIsLooping(true);
            m_levelBackGroundMusicCube->getALSourceByName("levelbackGroundMusic")->setGain(1.0);
        //    m_levelBackGroundMusicCube->getALSourceByName("levelbackGroundMusic")->updatePosition(m_levelBackGroundMusicCube->sourcePos);

            
            //更新musicCube位置--abc
            m_levelBackGroundMusicCube->getTransform()->setPos(m_levelBackGroundMusicCube->sourcePos.x(),
                                                                         m_levelBackGroundMusicCube->sourcePos.y(),
                                                                         m_levelBackGroundMusicCube->sourcePos.z());
          //  m_levelBackGroundMusicCube->sourcePos.print();
            
        }
        //
        addChild(m_levelBackGroundMusicCube);
        //----播放背景音乐--abc
    //    float originPos[4]={0,0,0,1};
        // m_levelBackGroundMusicCube->pALSource->updatePosition(originPos);
        // m_levelBackGroundMusicCube->pALSource->play();
     //   m_levelBackGroundMusicCube->getALSourceByName("levelbackGroundMusic")->updatePosition(originPos);//???????
        m_levelBackGroundMusicCube->getALSourceByName("levelbackGroundMusic")->updatePosition(m_levelBackGroundMusicCube->sourcePos);

        
    }

    
    
    
    //----初始化boss的碰撞网格--abc
    //碰撞墙的范围小些，对减小网格尺寸有帮助--abc
    {
        float maxRc=-c3d_INF;
        int n=(int)m_bossCellSet.getActorCount();
        for(int i=0;i<n;i++){
            CbossCell*p=(CbossCell*)m_bossCellSet.getActorByIndex(i);
            if(p->getRc()>maxRc){
                maxRc=p->getRc();
            }
        }//got maxRc
        float d_ccell=maxRc;//格子边长--abc
        //boss的碰撞网格范围为碰撞墙(cbTri)范围再并上各bossShip的活动范围--abc
        vector<int> skipTagList;
        skipTagList.push_back(tag_collisionSubMesh);
        Cc3dRange range_cbTri=m_archScene->getRangeOfIDtrisWithTags(skipTagList);
        Cc3dRange range_fly=m_bossCellSet.getBossShipFlyRange();
        Cc3dRange range_CF=rangeAdd(range_cbTri,range_fly);
        m_collisionGrid_boss.init(range_CF.getMinX(), range_CF.getMaxX(),
                                range_CF.getMinY(), range_CF.getMaxY(),
                                range_CF.getMinZ(), range_CF.getMaxZ(),
                                d_ccell, d_ccell, d_ccell,
                                20,20,20);
        //输出grid大小--abc
        {
            int nI=(int)m_collisionGrid_boss.grid.size();
            int nJ=(nI==0)?0:(int)m_collisionGrid_boss.grid[0].size();
            int nK=(nI==0||nJ==0)?0:(int)m_collisionGrid_boss.grid[0][0].size();
            cout<<"collisionGrid_boss.grid size:"<<nI<<" "<<nJ<<" "<<nK<<endl;
        }
        //检查是否有地面bossCell出界（飞行bossCell出界没关系)
        {
            int nBoss=(int)m_bossCellSet.getActorCount();
            for(int i=0;i<nBoss;i++){
                CbossCell*pbossCell=(CbossCell*)m_bossCellSet.getActorByIndex(i);
                if(pbossCell->get_isFly())continue;
                float x=pbossCell->getTransform()->getPos().x();
                float y=pbossCell->getTransform()->getPos().y();
                float z=pbossCell->getTransform()->getPos().z();
                if(!(x>range_CF.getMinX()&&x<range_CF.getMaxX()
                     &&y>range_CF.getMinY()&&y<range_CF.getMaxY()
                     &&z>range_CF.getMinZ()&&z<range_CF.getMaxZ())){//如果出界--abc
                    cout<<"error:有地面bossCell出界!"<<endl;
                    cout<<"tempID:"<<pbossCell->templateID<<endl;
                    cout<<"pos:";
                    pbossCell->getTransform()->getPos().print();
                    assert(false);
                }
            }
            
        }
        
    }
    //----初始化mobilePlatform的碰撞网格--abc
    {
        float maxRc=-c3d_INF;
        int n=(int)m_mobilePlatformSet.getActorCount();
        for(int i=0;i<n;i++){
            CmobilePlatform*p=(CmobilePlatform*)m_mobilePlatformSet.getActorByIndex(i);
            if(p->getRc()>maxRc){
                maxRc=p->getRc();
            }
        }//got maxRc
        float d_ccell=maxRc;//格子边长--abc
        //mobilePlatform的碰撞网格范围为所有mobilePlatform的startPos和endPos的范围--abc
        float xmin,xmax,ymin,ymax,zmin,zmax;
        m_mobilePlatformSet.calRange(xmin,xmax,ymin,ymax,zmin,zmax);
        m_collisionGrid_mobilePlatform.init(xmin, xmax,
                                          ymin, ymax,
                                          zmin, zmax,
                                          d_ccell, d_ccell, d_ccell);
        //输出grid大小--abc
        {
            int nI=(int)m_collisionGrid_mobilePlatform.grid.size();
            int nJ=(nI==0)?0:(int)m_collisionGrid_mobilePlatform.grid[0].size();
            int nK=(nI==0||nJ==0)?0:(int)m_collisionGrid_mobilePlatform.grid[0][0].size();
            cout<<"collisionGrid_mobilePlatform.grid size:"<<nI<<" "<<nJ<<" "<<nK<<endl;
        }
    }
    
    
    //----初始化bubble的碰撞网格--abc
    {
        float maxRc=-c3d_INF;
        int n=(int)m_bubbleSet.getActorCount();
        for(int i=0;i<n;i++){
            Cbubble*p=(Cbubble*)m_bubbleSet.getActorByIndex(i);
            if(p->getRc()>maxRc){
                maxRc=p->getRc();
            }
        }//got maxRc
        float d_ccell=maxRc;//格子边长--abc
        //bubble的碰撞网格范围为所有bubble的pos范围--abc
        float xmin,xmax,ymin,ymax,zmin,zmax;
        m_bubbleSet.calRange(xmin,xmax,ymin,ymax,zmin,zmax);
        m_collisionGrid_bubble.init(xmin, xmax,
                                  ymin, ymax,
                                  zmin, zmax,
                                  d_ccell, d_ccell, d_ccell,
                                  20,20,20);
        //输出grid大小--abc
        {
            int nI=(int)m_collisionGrid_bubble.grid.size();
            int nJ=(nI==0)?0:(int)m_collisionGrid_bubble.grid[0].size();
            int nK=(nI==0||nJ==0)?0:(int)m_collisionGrid_bubble.grid[0][0].size();
            cout<<"collisionGrid_bubble.grid size:"<<nI<<" "<<nJ<<" "<<nK<<endl;
        }
        //更新collisionGrid，只在初始化时更新即可，因为bubble都是不动的--abc
        {
            //制作屏蔽列表--abc
            int n=(int)m_bubbleSet.getActorCount();
            vector<bool> maskList(n);//屏蔽列表--abc
            for(int i=0;i<n;i++)maskList[i]=false;
          /*  cout<<"actorCount:"<<m_bubbleSet.getActorCount()<<endl;
            for(int i=0;i<m_bubbleSet.getActorCount();i++){
                m_bubbleSet.getActorByIndex(i)->getPos().print();
            }*/
            m_collisionGrid_bubble.fillGrid(m_bubbleSet.getActorList(),maskList);
        }
        //m_collisionGrid_bubble.print();
        
    }
    
    
    
    cout<<"level load finish !"<<endl;
//------------------------------------------------------------------------------------------------------------------------------------
    //--quad_oops
    {
        m_quad_oops=new Ckey();
        m_quad_oops->autorelease();
        //
        float bx=0;
        float by=0;
        float w=120;
        float h=60;
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_oops.png");
        m_quad_oops->genAndInit(bx, bx+w, by, by+h,0,1,0,1);
        m_quad_oops->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_oops->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_quad_oops->setKeyUpColor(1, 1, 1, 1);
        m_quad_oops->useKeyUpColor();
        m_quad_oops->setCamera(camera2D);
        m_quad_oops->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_oops->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_oops->setPassUnifoCallback(passUnifoCallback_noLight);
        m_quad_oops->setVisitDrawOrder(10000);
        //
        float c[2]={164,300};
        m_quad_oops->reSetCenter(c[0], c[1]);
        m_quad_oops->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //设为不可见--abc
        //   quad_oops.set_invisible(true);
        m_quad_oops->setIsVisible(false);
        //
        addChild(m_quad_oops);
    }
    //--quad_out_of_fuel
    {
        m_quad_out_of_fuel=new Ckey();
        m_quad_out_of_fuel->autorelease();
        //
        float bx=0;
        float by=0;
        float w=200;
        float h=50;
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_out_of_fuel.png");
        m_quad_out_of_fuel->genAndInit(bx, bx+w, by, by+h,0,1,0,1);
        m_quad_out_of_fuel->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_out_of_fuel->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_quad_out_of_fuel->setKeyUpColor(1, 1, 1, 1);
        m_quad_out_of_fuel->useKeyUpColor();
        m_quad_out_of_fuel->setCamera(camera2D);
        m_quad_out_of_fuel->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_out_of_fuel->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_out_of_fuel->setPassUnifoCallback(passUnifoCallback_noLight);
        m_quad_out_of_fuel->setVisitDrawOrder(10000);
        //
        float c[2]={164,300};
        m_quad_out_of_fuel->reSetCenter(c[0], c[1]);
        m_quad_out_of_fuel->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //设为不可见--abc
        m_quad_out_of_fuel->setIsVisible(false);
        //
        addChild(m_quad_out_of_fuel);
    }
    //--quad_halfFuelHaveBeenUsed
    {
        m_quad_halfFuelHaveBeenUsed=new Ckey();
        m_quad_halfFuelHaveBeenUsed->autorelease();
        //
        float bx=0;
        float by=0;
        float w=200;
        float h=50;
        Cc3dTexture* ptex=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_halfFuelHaveBeenUsed.png");
        m_quad_halfFuelHaveBeenUsed->genAndInit(bx, bx+w, by, by+h,0,1,0,1);
        m_quad_halfFuelHaveBeenUsed->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_halfFuelHaveBeenUsed->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex);
        m_quad_halfFuelHaveBeenUsed->setKeyUpColor(1, 1, 1, 1);
        m_quad_halfFuelHaveBeenUsed->useKeyUpColor();
        m_quad_halfFuelHaveBeenUsed->setCamera(camera2D);
        m_quad_halfFuelHaveBeenUsed->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_halfFuelHaveBeenUsed->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_halfFuelHaveBeenUsed->setPassUnifoCallback(passUnifoCallback_noLight);
        m_quad_halfFuelHaveBeenUsed->setVisitDrawOrder(10000);
        //
        float c[2]={164,300};
        m_quad_halfFuelHaveBeenUsed->reSetCenter(c[0], c[1]);
        m_quad_halfFuelHaveBeenUsed->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //设为不可见--abc
        m_quad_halfFuelHaveBeenUsed->setIsVisible(false);
        //
        addChild(m_quad_halfFuelHaveBeenUsed);
    }
    

//------------------------------------------------------------------------------------------------------------------------------------
    //----uiRoot
    m_uiRoot=new Cc3dNode();
    m_uiRoot->autorelease();
    m_uiRoot->setVisitDrawOrder(10000);
    addChild(m_uiRoot);
    //----ui
    const float keyMovUp=height-480;//按钮向上移动量--abc
      //--quad_depthTex
    {
        m_quad_depthTex=new Ckey();
        m_quad_depthTex->autorelease();
        //
        float bx=120;
        float by=0;
        float w=80.0;
        float h=w/m_depthTex->getWidth()*m_depthTex->getHeight();
        m_quad_depthTex->genAndInit(bx, bx+w, by, by+h,0,1,0,1);
        m_quad_depthTex->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_depthTex->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(m_depthTex);
        m_quad_depthTex->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        m_quad_depthTex->setCamera(camera2D);
        m_quad_depthTex->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_depthTex->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_depthTex->setPassUnifoCallback(passUnifoCallback_noLight);
        m_quad_depthTex->setIsVisible(CgameSettings::sharedGameSettings()->getShowDepthTexs());
        m_quad_depthTex->setVisitDrawOrder(10000);
        //
        m_quad_depthTex->reSetLeftDownCorner(0, 80);
        m_quad_depthTex->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_quad_depthTex);
        
    }
    //--quad_depthTex2
    {
        m_quad_depthTex2=new Ckey();
        m_quad_depthTex2->autorelease();
        //
        float bx=210;
        float by=0;
        float w=80.0;
        float h=w/m_depthTex2->getWidth()*m_depthTex2->getHeight();
        m_quad_depthTex2->genAndInit(bx, bx+w, by, by+h,0,1,0,1);
        m_quad_depthTex2->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_quad_depthTex2->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(m_depthTex2);
        m_quad_depthTex2->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        m_quad_depthTex2->setCamera(camera2D);
        m_quad_depthTex2->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_quad_depthTex2->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_quad_depthTex2->setPassUnifoCallback(passUnifoCallback_noLight);
        m_quad_depthTex2->setIsVisible(CgameSettings::sharedGameSettings()->getShowDepthTexs());
        m_quad_depthTex2->setVisitDrawOrder(10000);
        //
        m_quad_depthTex2->reSetLeftDownCorner(100, 80);
        m_quad_depthTex2->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_quad_depthTex2);
        
        
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
        m_debugKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_debug.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false));
        m_debugKey->setCamera(camera2D);
        m_debugKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_debugKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_debugKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_debugKey->setVisitDrawOrder(10000);
        //
        float center[2]={width-r_key,0+r_key};
        m_debugKey->reSetCenter(center[0], center[1]);
        m_debugKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_debugKey);
    }
    //--tipKey
    {
        m_tipKey=new Ckey();
        m_tipKey->autorelease();
        //
        float c[2]={0,0};
        m_tipKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_tipKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_tipKey->setKeyDownColor(0.7, 0.7, 0.7, 0.5);
        m_tipKey->setKeyUpColor(1, 1, 1, 0.5);
        m_tipKey->useKeyUpColor();
        m_tipKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_tip.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false));
        m_tipKey->setCamera(camera2D);
        m_tipKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_tipKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_tipKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_tipKey->setVisitDrawOrder(10000);
        //
        float center[2]={30,30};
        m_tipKey->reSetCenter(center[0], center[1]);
        m_tipKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_tipKey);
    }
    //--pauseKey
    {
        m_pauseKey=new Ckey();
        m_pauseKey->autorelease();
        //
        float c[2]={0,0};
        m_pauseKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_pauseKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_pauseKey->setKeyDownColor(0.7, 0.7, 0.7, 0.5);
        m_pauseKey->setKeyUpColor(1, 1, 1, 0.5);
        m_pauseKey->useKeyUpColor();
        m_pauseKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_pause.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false));
        m_pauseKey->setCamera(camera2D);
        m_pauseKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_pauseKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_pauseKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_pauseKey->setVisitDrawOrder(10000);
        //
        float center[2]={35,450+keyMovUp};
        m_pauseKey->reSetCenter(center[0], center[1]);
        m_pauseKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_pauseKey);
    }
    //--zoomOutKey
    {
        m_zoomOutKey=new Ckey();
        m_zoomOutKey->autorelease();
        //
        Cc3dTexture* ptex_zoomOut=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_zoomOut.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_zoomOutKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_zoomOutKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_zoomOutKey->setKeyDownColor(0.7, 0.7, 0.7, 0.5);
        m_zoomOutKey->setKeyUpColor(1, 1, 1, 0.5);
        m_zoomOutKey->useKeyUpColor();
        m_zoomOutKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex_zoomOut);
        m_zoomOutKey->setCamera(camera2D);
        m_zoomOutKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_zoomOutKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_zoomOutKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_zoomOutKey->setVisitDrawOrder(10000);
        //
        float center[2]={2*r_key+r_key,30};
        m_zoomOutKey->reSetCenter(center[0], center[1]);
        m_zoomOutKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_zoomOutKey);
    }
    
    //--shotBigKey
    {
        float pos_shotBigKey[2]={95+2*r_key+2*r_key,110-2*r_key};
        //
        m_shotBigKey=new Ckey();
        m_shotBigKey->autorelease();
        //
  //      ptex_shotBigKey_adjusting=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_shot_big_adjusting.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        Cc3dTexture*ptex_shotBigKey=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_shot_big.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_shotBigKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_shotBigKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_shotBigKey->setKeyDownColor(0.7, 0.7, 0.7, 0.5);
        m_shotBigKey->setKeyUpColor(1, 1, 1, 0.5);
        m_shotBigKey->useKeyUpColor();
        m_shotBigKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex_shotBigKey);
        m_shotBigKey->setCamera(camera2D);
        m_shotBigKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_shotBigKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_shotBigKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_shotBigKey->setVisitDrawOrder(10000);
        //
        m_shotBigKey->reSetCenter(pos_shotBigKey[0], pos_shotBigKey[1]);
        m_shotBigKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_shotBigKey);
    }
    //--shotKey
    {
        float pos_shotKey[2]={95+2*r_key,110-2*r_key};
        m_shotKey=new Ckey();
        m_shotKey->autorelease();
        //
        //ptex_shotKey_adjusting=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_shot_adjusting.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        Cc3dTexture*ptex_shotKey=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_shot.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_shotKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_shotKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_shotKey->setKeyDownColor(0.7, 0.7, 0.7, 0.5);
        m_shotKey->setKeyUpColor(1, 1, 1, 0.5);
        m_shotKey->useKeyUpColor();
        m_shotKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptex_shotKey);
        m_shotKey->setCamera(camera2D);
        m_shotKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_shotKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_shotKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_shotKey->setVisitDrawOrder(10000);
        //
        m_shotKey->reSetCenter(pos_shotKey[0], pos_shotKey[1]);
        m_shotKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_shotKey);
    }
    
    
    //--goDownShipKey
    {
        m_goDownShipKey=new Ckey();
        m_goDownShipKey->autorelease();
        //
        Cc3dTexture*ptexture_goDownShipKey=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/keys/key_goDownShip.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
        float c[2]={0,0};
        m_goDownShipKey->genAndInit(c[0]-r_key,c[0]+r_key,c[1]-r_key,c[1]+r_key, 0,1,0,1);
        m_goDownShipKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_goDownShipKey->setKeyDownColor(0.7, 0.7, 0.7, 0.5);
        m_goDownShipKey->setKeyUpColor(1, 1, 1, 0.5);
        m_goDownShipKey->useKeyUpColor();
        m_goDownShipKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(ptexture_goDownShipKey);
        m_goDownShipKey->setCamera(camera2D);
        m_goDownShipKey->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_goDownShipKey->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_goDownShipKey->setPassUnifoCallback(passUnifoCallback_noLight);
        m_goDownShipKey->setIsVisible(false);
        m_goDownShipKey->setIsDoUpdate(false);
        m_goDownShipKey->setVisitDrawOrder(10000);
        //
        float center[2]={35,450+keyMovUp-r_key*3};
        m_goDownShipKey->reSetCenter(center[0], center[1]);
        m_goDownShipKey->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_goDownShipKey);
    }
    
    
    
    //---lifeBar
    {
        m_lifeBar=new CprogressBar();
        m_lifeBar->autorelease();
        //
        m_lifeBar->genAndInit(0, 0, 123, 15);
        //quad_back
        m_lifeBar->getQuadBack()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_lifeBar->getQuadBack()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/white.png"));
        m_lifeBar->getQuadBack()->setKeyUpColor(1, 0.7, 0.7, 0.7);
        m_lifeBar->getQuadBack()->useKeyUpColor();
        m_lifeBar->getQuadBack()->setCamera(camera2D);
        m_lifeBar->getQuadBack()->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_lifeBar->getQuadBack()->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_lifeBar->getQuadBack()->setPassUnifoCallback(passUnifoCallback_noLight);
        m_lifeBar->getQuadBack()->setVisitDrawOrder(10000);
        //quad_front
        m_lifeBar->getQuadFront()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_lifeBar->getQuadFront()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/white.png"));
        m_lifeBar->getQuadFront()->setKeyUpColor(1, 0, 0, 0.5);
        m_lifeBar->getQuadFront()->useKeyUpColor();
        m_lifeBar->getQuadFront()->setCamera(camera2D);
        m_lifeBar->getQuadFront()->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_lifeBar->getQuadFront()->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_lifeBar->getQuadFront()->setPassUnifoCallback(passUnifoCallback_noLight);
        m_lifeBar->getQuadFront()->setVisitDrawOrder(10000);
        //
        m_lifeBar->reSetLeftDownCorner(190, 450+keyMovUp);
        m_lifeBar->getQuadBack()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //quad_front
        m_lifeBar->getQuadFront()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_lifeBar);
    }
    //----quad_keyList
    {
        int nKey=10;
        m_tex_quad_key_blackWhite=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_geneCube_blackWhite.png");
        m_tex_quad_key_blackWhite->retain();
        m_tex_quad_key_colored=Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/quads/quad_geneCube.png");
        m_tex_quad_key_colored->retain();
        for(int i=0;i<nKey;i++){
            Ckey*quad_key=new Ckey();
            quad_key->autorelease();
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
            m_uiRoot->addChild(quad_key);
            m_quad_keyList.push_back(quad_key);
        }
        //定位--abc
        int nkey=m_cell->nkey;
        //在quad_keyList中先前nkey个quad_key进行绘制--abc
        float by=415+keyMovUp;
        float xBeginMin=190;
        float xEndMax=300;
        //在xbeginMin和xEndMax区间内画nKey个quad_key
        //为此重新设定各quad_key的大小--abc
        float keyWidth=minf((xEndMax-xBeginMin)/nkey,30);
        float keyHeight=keyWidth;
        for(int i=0;i<nkey;i++){
            Ckey*quad_key=m_quad_keyList[i];
            quad_key->reSetWidth(keyWidth);
            quad_key->reSetHeight(keyHeight);
        }
        //重设各quad_key的位置（通过左下角)
        for(int i=0;i<nkey;i++){
            Ckey*quad_key=m_quad_keyList[i];
            quad_key->reSetLeftDownCorner(xBeginMin+keyWidth*i, by);
        }
        //提交--abc
        for(int i=0;i<nkey;i++){
            Ckey*quad_key=m_quad_keyList[i];
            quad_key->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        }
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
        //
        float bx=190;
        float by=415-35+keyMovUp;
        m_quad_coin->reSetLeftDownCorner(bx, by);
        m_quad_coin->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_quad_coin);
    }
    //----quad_nCoinPositivePlaceList
    {
        int nPositivePlace=nCoinPositivePlaceCount;
        for(int i=0;i<nPositivePlace;i++){
            Ckey*quad=new Ckey();
            quad->autorelease();
            quad->genAndInit(0, 26, 0, 26,0,1,0,1);
            quad->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            quad->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(m_numberTexList[0]);
            quad->setKeyUpColor(1, 1, 1, 0.8);
            quad->useKeyUpColor();
            quad->setCamera(camera2D);
            quad->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
            quad->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
            quad->setPassUnifoCallback(passUnifoCallback_noLight);
            quad->setVisitDrawOrder(10000);
            m_uiRoot->addChild(quad);
            m_quad_nCoinPositivePlaceList.push_back(quad);
            
        }
        //定位--abc
        float _bx=190+10;
        float _by=415-35+keyMovUp;
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
        float bx=190;
        float by=380-35+keyMovUp;
        m_quad_killEnemy->reSetLeftDownCorner(bx, by);
        m_quad_killEnemy->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_quad_killEnemy);
    }
    //----quad_nKillEnemyPostivePlaceList
    {
        int nPositivePlace=nKillEnemyPostivePlaceCount;
        for(int i=0;i<nPositivePlace;i++){
            Ckey*quad=new Ckey();
            quad->autorelease();
            quad->genAndInit(0, 26, 0, 26,0,1,0,1);
            quad->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
            quad->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(m_numberTexList[0]);
            quad->setKeyUpColor(1, 1, 1, 0.8);
            quad->useKeyUpColor();
            quad->setCamera(camera2D);
            quad->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
            quad->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
            quad->setPassUnifoCallback(passUnifoCallback_noLight);
            quad->setVisitDrawOrder(10000);
            m_uiRoot->addChild(quad);
            m_quad_nKillEnemyPositivePlaceList.push_back(quad);
        }
        //定位--abc
        float _bx=190+10;
        float _by=380-35+keyMovUp;
        for(int i=0;i<nPositivePlace;i++){
            Ckey*quad=m_quad_nKillEnemyPositivePlaceList[i];
            _bx+=20;
            quad->reSetLeftDownCorner(_bx, _by);
            quad->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        }

    }
    //---fuelBar
    {
        m_fuelBar=new CprogressBar();
        m_fuelBar->autorelease();
        //
        m_fuelBar->genAndInit(0, 0, 123, 15);
        //quad_back
        m_fuelBar->getQuadBack()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_fuelBar->getQuadBack()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/white.png"));
        m_fuelBar->getQuadBack()->setKeyUpColor(0.7, 1, 0.7, 0.7);
        m_fuelBar->getQuadBack()->useKeyUpColor();
        m_fuelBar->getQuadBack()->setCamera(camera2D);
        m_fuelBar->getQuadBack()->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_fuelBar->getQuadBack()->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_fuelBar->getQuadBack()->setPassUnifoCallback(passUnifoCallback_noLight);
        m_fuelBar->getQuadBack()->setVisitDrawOrder(10000);
        //quad_front
        m_fuelBar->getQuadFront()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->genBuffers();
        m_fuelBar->getQuadFront()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage("singleCellGameResource/data/global/tex/white.png"));
        m_fuelBar->getQuadFront()->setKeyUpColor(0, 0.6, 0, 0.5);
        m_fuelBar->getQuadFront()->useKeyUpColor();
        m_fuelBar->getQuadFront()->setCamera(camera2D);
        m_fuelBar->getQuadFront()->getModel()->getMeshList()[0]->setIsWriteDepthBuffer(false);
        m_fuelBar->getQuadFront()->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shaderNoLight"));
        m_fuelBar->getQuadFront()->setPassUnifoCallback(passUnifoCallback_noLight);
        m_fuelBar->getQuadFront()->setVisitDrawOrder(10000);
        //
        m_fuelBar->reSetLeftDownCorner(190, 360-35+keyMovUp);
        m_fuelBar->getQuadBack()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        m_fuelBar->getQuadFront()->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->submit(GL_STATIC_DRAW);
        //
        m_uiRoot->addChild(m_fuelBar);
        //根据shipSet是否为空决定是否显示fuelBar
      //  cout<<"ship count:"<<m_shipSet.getMeshCount()<<endl;
        m_fuelBar->setIsVisibleRecursively(m_shipSet.getActorCount()!=0);
    }
    //----
    
    
    
    //-----------music
    //----levelDone
    Cc3dALSource*pALSource_levelDone=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/level_done.wav","levelDone");
    this->addALSource(pALSource_levelDone);
    //----gameOver
    Cc3dALSource*pALSource_gameOver=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/gameOver.wav","gameOver");
    this->addALSource(pALSource_gameOver);
    //----switchPage
    Cc3dALSource*pALSource_switchPage=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("singleCellGameResource/data/global/audio/switchPage.wav","switchPage");
    this->addALSource(pALSource_switchPage);
    //---backGroundMusic
    m_levelBackGroundMusicCube->getALSourceByName("levelbackGroundMusic")->play();
    //---
    Cc3dAudioCache::sharedAudioCache()->print();
    Cc3dTextureCache::sharedTextureCache()->print();
    
    return true;
}
void  CplayingPage::visitDraw(){
    GLint oldFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFrameBuffer);

  
    Cc3dVector4 cellPos=m_cell->getTransform()->getPos();
  /*  glEnable( GL_POLYGON_OFFSET_FILL );
    float g_OffsetFactor=10;//0.1;//The initial value is zero.
    float g_OffsetUnit=0;//The initial value is zero.
    glPolygonOffset( g_OffsetFactor,g_OffsetUnit);*/
    //------------------------渲染shadow map--------------------
    {
        //---渲染静态shadow map
        if(m_depthTex_got==false){
            cout<<"render depthTex..."<<endl;
            //change program, passUnifoCallback, light for all nodes of scene
            map<Cc3dNode*,Cc3dProgram*> nodeProgramMap=changeProgramRecursively(this, Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_renderDepth"));
            map<Cc3dNode*,c3dPassUnifoCallbackPtr> nodeCallbackMap=changePassUnifoCallbackRecursively(this, passUnifoCallback_renderDepth);
            map<Cc3dNode*,Cc3dCamera*> nodeCameraMap=changeCameraRecursively(this,light0.getLightViewCameraByIndex(0));
            //设置光源视角--abc
            Cc3dRange sceneRange=m_archScene->getModel()->getMeshByIndex(0)->getRange();
            float cx=(sceneRange.getMinX()+sceneRange.getMaxX())/2;
            float cy=(sceneRange.getMinY()+sceneRange.getMaxY())/2;
            float cz=(sceneRange.getMinZ()+sceneRange.getMaxZ())/2;
            Cc3dVector4 center(cx,cy,cz,1);//看向场景中心--abc
            Cc3dVector4 up(1,0,0,0);//向上向量（不再用Y了)
            light0.updateEyePosForAllLightViewCameras();
            light0.getLightViewCameraByIndex(0)->setCenter(center);
            light0.getLightViewCameraByIndex(0)->setUp(up);
            light0.getLightViewCameraByIndex(0)->updateFrustum();
            //----渲染depthTex
            glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->getID());
            //we should ensoure depthMask is true before call glClear(GL_DEPTH_BUFFER_BIT...)
            //see: http://www.flipcode.com/forums/thread/9101
            //http://stackoverflow.com/questions/12475369/does-glcolormask-affect-glclear
            //http://www.opengl.org/sdk/docs/man2/xhtml/glClear.xml
            glDepthMask(GL_TRUE);
            glClear(GL_DEPTH_BUFFER_BIT);//只有深度缓存，所以只清除深度缓存，另外tile-base architecture要求bind之后立即clear更快些--abc
            glViewport(0, 0, m_depthTex->getWidth() , m_depthTex->getHeight());//离屏窗口--abc
            
            
            //注意是用camera_light
            vector<int> skipTagList;
            skipTagList.push_back(tag_collisionSubMesh);
            m_archScene->updateVisibleIDTriList(skipTagList);
            m_archScene->submitVisibleIDTriList();
            m_archScene->visitDraw();
            
            m_depthTex_got=true;
            
            cout<<"render depthTex done!"<<endl;
            //recover
            setPrograms(nodeProgramMap);
            setPassUnifoCallbacks(nodeCallbackMap);
            setCameras(nodeCameraMap);
            
        }
        
        //----渲染动态shadow map
        
        //设置光源视角--abc
        float center[4]={cellPos.x(),cellPos.y(),cellPos.z(),1};//看主角--abc
        float up[4]={1,0,0,0};//向上向量（不再用Y了)
        light0.updateEyePosForAllLightViewCameras();
        light0.getLightViewCameraByIndex(1)->setCenter(center);
        light0.getLightViewCameraByIndex(1)->setUp(up);
        //change program, passUnifoCallback, light for all nodes of scene
        map<Cc3dNode*,Cc3dProgram*> nodeProgramMap=changeProgramRecursively(this, Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_renderDepth"));
        map<Cc3dNode*,c3dPassUnifoCallbackPtr> nodeCallbackMap=changePassUnifoCallbackRecursively(this, passUnifoCallback_renderDepth);
        map<Cc3dNode*,Cc3dCamera*> nodeCameraMap=changeCameraRecursively(this,light0.getLightViewCameraByIndex(1));
        //----渲染depthTex2
        glBindFramebuffer(GL_FRAMEBUFFER,m_fbo2->getID());
        //we should ensoure depthMask is true before call glClear(GL_DEPTH_BUFFER_BIT...)
        //see: http://www.flipcode.com/forums/thread/9101
        //http://stackoverflow.com/questions/12475369/does-glcolormask-affect-glclear
        //http://www.opengl.org/sdk/docs/man2/xhtml/glClear.xml
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);//只有深度缓存，所以只清除深度缓存，另外tile-base architecture要求bind之后立即clear更快些--abc
        glViewport(0, 0, m_depthTex2->getWidth() , m_depthTex2->getHeight());//离屏窗口--abc

        //注意是用camera_light2
        if(m_cell->get_onShip()==false){
            bool isLegVisible=m_cell->getModel()->getMeshList()[CELLPART_legs]->getIsVisible();
            m_cell->getModel()->getMeshList()[CELLPART_legs]->setIsVisible(false);
            m_cell->visitDraw();
            m_cell->getModel()->getMeshList()[CELLPART_legs]->setIsVisible(isLegVisible);
            
        }else{
            m_cell->get_ship()->visitDraw();
            
        }
        //recover
        setPrograms(nodeProgramMap);
        setPassUnifoCallbacks(nodeCallbackMap);
        setCameras(nodeCameraMap);
    }
 /*   //恢复
    {
        float g_OffsetFactor=0;//The initial value is zero.
        float g_OffsetUnit=0;//The initial value is zero.
        glPolygonOffset( g_OffsetFactor,g_OffsetUnit);
        glDisable( GL_POLYGON_OFFSET_FILL );
    }*/

    
    //-----------------------切回到相机视角--abc
    //获得当前相机位置--abc
    const float *eyePos=camera->getEyePos().getArray();
    //根据主角位置计算相机的目标位置--abc
    Cc3dVector4 eyePos_tg=m_cell->getTransform()->convertToWorldSpace(m_eyePosInCell);//目标相机位置--abc
    //计算软移动后实际相机位置--abc
    Cc3dVector4 eyePos_real;
    const float k=(m_cell->get_onShip()?0.1:0.25);
    eyePos_real=softMov(eyePos, eyePos_tg, k);
    Cc3dVector4 up(0,1,0,0);//向上向量--abc
    camera->setEyePos(eyePos_real);
    camera->setCenter(cellPos);
    camera->setUp(up);
    camera->updateFrustum();
    
    
    

    
    //----------------------------------------------------------------------------------------------------
    //---------------------切换回屏幕fbo---------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, oldFrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);//tile-base architecture要求bind之后立即clear更快些--abc
    glViewport(Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getMinX(),
               Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getMinY(),
               Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getWidth() ,
               Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getHeight());//屏幕窗口--abc


    
    
 
    //archScene收集可见面--abc
    vector<int> skipTagList;
    skipTagList.push_back(tag_collisionSubMesh);
    m_archScene->updateVisibleIDTriList(skipTagList);
    m_archScene->submitVisibleIDTriList();
    //绘图--abc
    {
        
        glEnable(GL_BLEND);
        {
            //场景--abc
            Cc3dNode::visitDraw();
            
            //相机在墙内时看到的透明面--abc
            if((int)m_archScene->getModel()->getMeshByIndex(0)->getSubMeshCount()>=1){
                float oldDiffuseAlpha=m_archScene->getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getMaterial()->getDiffuse().a();
                Ec3dCullFace oldCullFace=m_archScene->getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getCullFace();
                m_archScene->getModel()->getMeshByIndex(0)->setDiffuseAlpha(0.5);
                m_archScene->getModel()->getMeshByIndex(0)->setCullFace(ec3dCullFront);
                m_archScene->submitIDtriAroundSphere(cellPos,m_cell->getRc()*3);
                m_archScene->visitDraw();
                m_archScene->getModel()->getMeshByIndex(0)->setCullFace(oldCullFace);
                m_archScene->getModel()->getMeshByIndex(0)->setDiffuseAlpha(oldDiffuseAlpha);
                
            }
            
        }
 
        
     
    }
    
    

    

}



void  CplayingPage::makeSound(){//必须加static
    //----划水声--abc
    if(m_cell->get_isPuddleWater()){
        //更新声源位置（注意，不管是否重启，都要更新声源)
       // m_cell->pALSource_water_puddle->updatePosition(m_cell->getTransform().getPos());
        m_cell->getALSourceByName("water_puddle")->updatePosition(m_cell->getTransform()->getPos());
        //如果已停止则重启--abc
        //m_cell->pALSource_water_puddle->playIfNotPlaying();
        m_cell->getALSourceByName("water_puddle")->playIfNotPlaying();
    }else{
        //m_cell->pALSource_water_puddle->stop();
        m_cell->getALSourceByName("water_puddle")->stop();
    }
    //----与固体表面的摩擦声--abc
    if(m_cell->get_isMovingOnSolidSurface()){
        //更新声源位置--abc
        //m_cell->pALSource_move_shasha->updatePosition(m_cell->getTransform().getPos());
        m_cell->getALSourceByName("move_shasha")->updatePosition(m_cell->getTransform()->getPos());
        //如果已停止则重启--abc
        //m_cell->pALSource_move_shasha->playIfNotPlaying();
        m_cell->getALSourceByName("move_shasha")->playIfNotPlaying();
    }else{
        //m_cell->pALSource_move_shasha->stop();
        m_cell->getALSourceByName("move_shasha")->stop();
    }
    //----飞船引擎声--abc
    Cship*pship=m_cell->get_ship();
    if(pship){
        if(m_cell->fuel.value!=0){//有燃料才有声音--abc
            //根据速度设定音量大小--abc
            float rate_v_pushForward=pship->vforward_push/pship->v_pushForward_value;
            float rate_vy_push=pship->vy_push>0?(pship->vy_push/pship->v_pushUp_value):(fabsf(pship->vy_push)/pship->v_pushDown_value);
            float rateMax=(pship->get_isFlyable()?maxf(rate_v_pushForward, rate_vy_push):rate_v_pushForward);
            float backSoundWeight=0.3;//背景音权重--abc
            float gain=rateMax*(1-backSoundWeight)+backSoundWeight;
         //   pship->pALSource_shipEngineSound->setGain(gain);
         //   pship->pALSource_shipEngineSound->updatePosition(pship->getTransform().getPos());
            if(pship->getALSourceByName("shipEngineSound")){
            pship->getALSourceByName("shipEngineSound")->setGain(gain);
            pship->getALSourceByName("shipEngineSound")->updatePosition(pship->getTransform()->getPos());
            }
        }
    }

    
    
}

bool  CplayingPage::getIsCheckPointEmpty()const{
    return m_checkPoint.getIsEmpty();
}
bool  CplayingPage::resetToCheckPoint()
{
    if(m_checkPoint.getIsEmpty())return false;
    //恢复检查点--abc
    m_checkPoint.recover(*m_cell);
    //更新和提交mobilePlatform的顶点--abc
    m_mobilePlatformSet.updateAndSubmitVertex(true);
    //血充满--abc
    m_cell->life.chargeFull();
    //燃料充满--abc
    m_cell->fuel.chargeFull();
    //设置无伤害倒计时--abc
    m_cell->revTimeCounter_noHert.setValue(noHurtDuringTime);
    //将cell的竖直速度改为0
    m_cell->vup=0;
    //将cell所在ship的相前速度置为0
    if(m_cell->get_onShip()){
        Cship*pship= m_cell->get_ship();
        pship->vy_push=0;
        pship->vforward_push=0;
        pship->v_byOhterFoce.init(0,0,0,0);
    }
    m_quad_halfFuelHaveBeenUsed->haveBeenDisplayed=false;//以便能再display
    return true;

}
