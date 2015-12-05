//
//  shotBall.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-22.
//
//

#ifndef __HelloOpenGL__shotBall__
#define __HelloOpenGL__shotBall__

#include <iostream>
#include "c3d.h"
#include "archScene.h"
#include "sprite.h"
#include "ground.h"
#include "blast.h"
#include "mobilePlatform.h"
class CshotBall:public Cc3dActor
{
protected:
    int typeID;//类型号--abc
    bool m_isActive;
public:
    string fileName_launchSound;//发射声音文件名--abc
    string fileName_collisionWithSolidSound;//与固体碰撞声音文件名--abc
    Cc3dVector4 posf;//前一时刻位置--abc
    void*ptgBoss;//目标敌人--abc
    bool tgBossIsFly;//目标敌人是否为飞行boss（仅在ptgBoss非空时有效)
    float pathLen;//记录从射出到当前走过的路程--abc
    float shotBallReachDis;//子弹射程--abc
    float vy0;//vy的初始速度--abc
    float vy;//竖直速度--abc
    float g;//重力加速度--abc
    float hurt;//子弹的伤害--abc
    float hurtDestroyablePlatform;//对运动平台的伤害--abc
    int blastType;//本子弹的爆炸类型--abc
    float modelScale;//模型缩放比例--abc
protected:
    float Rc;
public:
    CshotBall(){
        typeID=0;
        tgBossIsFly=false;
        blastType=0;
        hurt=1;
        hurtDestroyablePlatform=1;
        shotBallReachDis=300;
        vy0=3;
        vy=vy0;
        g=0.12;
        m_isActive=false;
        ptgBoss=NULL;
        pathLen=0;
        Rc=0;
    }
    virtual~CshotBall(){
    //    cout<<"destruct shotBall"<<endl;
        
    };
   
    bool init(){
        assert((int)getModel()->getMeshCount()==0);
        Cc3dMesh*mesh=new Cc3dMesh();
        mesh->autorelease();
        addMesh(mesh);
        
        return true;
    }
    Cc3dMesh*getMesh(){
        assert((int)getModel()->getMeshCount()==1);
        return getModel()->getMeshByIndex(0);
    }
    void setRc(float _Rc){Rc=_Rc;}
    float getRc()const{return Rc;}
    void set_typeID(int _typeID){
        typeID=_typeID;
    }
    int get_typeID()const {
        return typeID;
    }
    bool getIsActive()const {
        return m_isActive;
    }
    void turnToActive(){
        m_isActive=true;
    }
    void turnToInActive(){//转为非活动状态--abc
        m_isActive=false;
        //停止声音--abc
        //this->pALSource_launchSound->stop();
        this->getALSourceByName("launchSound")->stop();
    
    }
    void loadConfig(const string&configPathShort){
        //------------------获得绝对路径--abc
        string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(configPathShort);
        //打开文件--abc
        FILE  *fp=NULL;
        fp=fopen(pathName.c_str(),"r");
        if(fp==NULL)
        {
            cout<<"Open "<<pathName<<" failed!"<<endl;
            assert(false);
        }
        //------------------提取文件信息，并建立模型列表--abc
        {
            char buffer[1024]={0};
            //提取scale
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&this->modelScale);
            //提取vy0
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&this->vy0);
            //提取gravity
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&this->g);
            //提取reachDis
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&this->shotBallReachDis);
            //提取hurt
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&this->hurt);
            //提取hurtDestroyablePlatform
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&this->hurtDestroyablePlatform);
            //提取blastTypeStr
            string blastTypeStr;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%s",buffer);
            blastTypeStr=buffer;
            this->blastType=Cblast::blastTypeStrToblastTypeID(blastTypeStr);
            //提取Rc
            float Rc;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&Rc);
            this->setRc(Rc);
            //提取fileName_launchSound;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%s",buffer);
            this->fileName_launchSound=buffer;
            //提取fileName_collisionWithSolidSound
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%s",buffer);
            this->fileName_collisionWithSolidSound=buffer;
        }
        //关闭文件--abc
        fclose(fp);
    }
    void set_tgBossIsFly(bool _tgBossIsFly){
        tgBossIsFly=_tgBossIsFly;
    }
    bool get_tgBossIsFly()const {
        return tgBossIsFly;
    }
    void set_blastType(int _blastType){
        blastType=_blastType;
    }
    int get_blastType()const {
        return blastType;
    }
    void set_hurt(float _hurt){
        hurt=_hurt;
    }
    float get_hurt()const{
        return hurt;
    }
    void set_hurtDestroyablePlatform(float _hurtDestroyablePlatform){
        hurtDestroyablePlatform=_hurtDestroyablePlatform;
    }
    float get_hurtDestroyablePlatform()const {
        return hurtDestroyablePlatform;
    }
    void set_shotBallReachDis(float _shotBallReachDis){
        shotBallReachDis=_shotBallReachDis;
    }
    float get_shotBallReachDis()const {
        return shotBallReachDis;
    
    }
    void set_vy0(float _vy0){
        vy0=_vy0;
    }
    void reSet_vy(){
        vy=vy0;
    }
    void update_vy(){
        vy-=g;
    }
    void movDueToGravity(){
        this->m_transform->moveRelativeToFather(0, vy, 0);
    }
    void set_pathLen(float _pathLen){
        pathLen=_pathLen;
    
    }
    float get_pathLen()const {
        return pathLen;
    }
    void pathLen_add(float dpathLen){
        pathLen+=dpathLen;
    }
    Cc3dVector4 getDir()const {
        float dir_local[4]={0,0,-1,0};
        Cc3dVector4 dir_world=m_transform->convertToWorldSpace(dir_local);
        return dir_world;
    }
    
    void storePos(){
        Cc3dVector4 pos=m_transform->getPos();
        posf=pos;
    }
    void collisionWithGround(const Cground*ground,CblastSet&shotBlastSet,Cc3dCamera&camera){//与地面碰撞--abc
        if(ground){
            Cc3dVector4 pos=m_transform->getPos();
            float H=ground->getH(pos.x(), pos.z());
            if(pos.y()<=H){
                //子弹转为active=false
                this->turnToInActive();
                this->setIsVisible(false);
                //在子弹消失处放置一个爆炸--abc
                float collisionFaceNom[4]={0,1,0,0};
                shotBlastSet.layBlast(posf, camera,blastType,collisionFaceNom);
                //子弹与ground碰撞声音--abc
                //this->pALSource_collisionWithSolidSound->updatePosition(pos);
                //this->pALSource_collisionWithSolidSound->play();
                if(this->getALSourceByName("collisionWithSolidSound")){
                this->getALSourceByName("collisionWithSolidSound")->updatePosition(pos);
                this->getALSourceByName("collisionWithSolidSound")->playIfNotPlaying();
                }
            }
        }
    }
    void collisionWithArch(CarchScene&archScene,CblastSet&shotBlastSet,Cc3dCamera&camera)
	{
        if(this->m_isActive==false){
            assert(getIsVisible()==false);
            return;
        }
        Cc3dVector4 pos=m_transform->getPos();//当前位置--abc
        //判断线段posf--pos是否与arch相交--abc
        Cc3dVector4 p;//交点坐标--abc
        Cc3dVector4 collisionFaceNorm;
        bool collision=archScene.collisionTest_lineSeg(posf,pos,p,collisionFaceNorm);
        if(collision){
            //子弹转为active=false
            turnToInActive();
            setIsVisible(false);
            //在此处放一个爆炸--abc
           shotBlastSet.layBlast(p, camera,blastType,collisionFaceNorm);
            //子弹与arch碰撞声音--abc
            //this->pALSource_collisionWithSolidSound->updatePosition(pos);
            //this->pALSource_collisionWithSolidSound->play();
            if(this->getALSourceByName("collisionWithSolidSound")){
                //xxx why?
            this->getALSourceByName("collisionWithSolidSound")->updatePosition(pos);
            this->getALSourceByName("collisionWithSolidSound")->playIfNotPlaying();
            }
        }
	}
    void collisionWithMobilePlatform(CmobilePlatformSet&mobilePlatformSet,CblastSet&shotBlastSet,Cc3dCamera&camera)
	{
        if(this->m_isActive==false){
            assert(getIsVisible()==false);
            return;
        }
        Cc3dVector4 pos=m_transform->getPos();//当前位置--abc
        //判断线段posf--pos是否与arch相交--abc
        Cc3dVector4 p;//交点坐标--abc
        Cc3dVector4 collisionFaceNorm;
        CmobilePlatform*pPlat=mobilePlatformSet.collisionTest_lineSeg(posf,pos,p,collisionFaceNorm);
        if(pPlat){//碰撞--abc
            //子弹转为active=false
            turnToInActive();
            setIsVisible(false);
            //在此处放一个爆炸--abc
           shotBlastSet.layBlast(p, camera,blastType,collisionFaceNorm);
            //触发运动平台--abc
            if(pPlat->isShotTrigger){
                //触发--abc
                pPlat->doTrigger_mov_ignoreTouchLocAndTriggerLock();
                pPlat->doTrigger_rot_ignoreTouchLocAndTriggerLock();
            }
            //掉血--abc
            if(pPlat->get_destroyable()){
                pPlat->life.dec(this->get_hurtDestroyablePlatform());
                if(pPlat->life.value==0){//击毁--abc
                    pPlat->setIsDied(true);
                    pPlat->setIsVisible(false);
                    //声音--abc
                  //  pPlat->pALSource_destroySound->updatePosition(pPlat->getTransform().getPos());
                  //  pPlat->pALSource_destroySound->play();
                    if(pPlat->getALSourceByName("destroySound")){
                    pPlat->getALSourceByName("destroySound")->updatePosition(pPlat->getTransform()->getPos());
                    pPlat->getALSourceByName("destroySound")->play();
                    }
                }
            }
            //子弹与mobliePlat碰撞的声音--abc
            //this->pALSource_collisionWithSolidSound->updatePosition(pos);
            //this->pALSource_collisionWithSolidSound->play();
            if(this->getALSourceByName("collisionWithSolidSound")){
                //xxx why?
            this->getALSourceByName("collisionWithSolidSound")->updatePosition(pos);
            this->getALSourceByName("collisionWithSolidSound")->playIfNotPlaying();
            }
        }
	}

};
#endif /* defined(__HelloOpenGL__shotBall__) */
