//
//  cell.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//

#ifndef __HelloOpenGL__cell__
#define __HelloOpenGL__cell__

#include <iostream>
using namespace std;
//#import <OpenAL/al.h> [change]
#include "c3d.h"
#include "shotBall.h"
#include "life.h"
#include "collisonGrid.h"
#include "environmentInfo.h"
#include "passUnifoCallBack.h"
#include "myFunc.h"
template<typename T>
class Ccur_foe
{
private:
    T cur;
    T foe;
public:
    Ccur_foe(){
        reSet();
    }
    void reSet(){
        memset(&cur, 0, sizeof(cur));
        memset(&foe, 0, sizeof(foe));
    }
    void init(const T&x){
        cur=x;
        foe=x;
    }
    void set(const T&x){
        foe=cur;
        cur=x;
    }
    void setBoth(const T&x){
        foe=x;
        cur=x;
    }
    void setFoe(const T&x){
        foe=x;
    }
    T get()const {
        return cur;
    }
    T get_foe()const {
        return foe;
    }
    bool isChanged()const {
        return (cur!=foe);
    }
    
};


//model0:核，model1:线粒体，model2:外壳，model3:腿，model4:胞体--abc
#define CELLPART_core 0
#define CELLPART_ene 1
#define CELLPART_shell 2
#define CELLPART_legs 3
#define CELLPART_body 4
#define CELLPART_tooth 5
#define CELLPART_eye 6
#define CELLPART_wingL 7
#define CELLPART_wingR 8
#define CELLPARTCOUNT 9//part数量--abc
//位姿调整硬度--abc
#define adjustSoftness_VERYSOFT 0
#define adjustSoftness_MIDSOFT 1
#define adjustSoftness_HARD 2
//onFire的持续时间--abc
const long onFireDuringTime=600;
//子弹速度--abc
const float v_shotBall=10.0;
//发射延时--abc
const long shotTimeLasts_long=200;
const long shotTimeLasts_short=90;
class CbossCellSet;
class Cship;
class Ccell:public Cc3dActor
{
public:
    bool isPuddleWater;//是否在划水--abc
    bool isMovingOnSolidSurface;//是否在固体表面移动--abc
public:
    CenvironmentInfo environmentInfo;
    float v_forward_value;
    float v_forward_roll_value;
    float cos_v_forward_roll_value;
    float sin_v_forward_roll_value;
    float v_back_value;
    float v_back_roll_value;
    float cos_v_back_roll_value;
    float sin_v_back_roll_value;
    float v_rot;
    float cos_v_rot;
    float sin_v_rot;
    float v_legsRot;
    float cos_v_legsRot;
    float sin_v_legsRot;
    float cos_A_wing;
    float sin_A_wing;
    bool wingIsMovingUp;//wing是否在扬起--abc
    float vup;//竖直速度(每帧竖直位移量)
    float a_gravity;//重力加速度(每帧竖直位移改变量)
    Ccur_foe<bool> isJump;//是否已起跳--abc
    Cc3dMatrix4 RTmat_store;//存储RTmat，用于恢复--abc
    Clife life;//生命--abc
    Clife fuel;//燃料--abc
    Ccur_foe<float> nEateCoin;//吃金币数量(充许出分数)
    Ccur_foe<int> nKillEnemy;//杀敌人数量--abc
    float Rc_small;//中心到底或顶的距离（半短轴长度)
    float Rc_small_value_flat;//摔扁时中心悬起高度--abc
    float Rc_small_value_normal;//正常时中心悬起高度--abc
    float H_causeFlat;//摔扁所需高度--abc
    bool isFallFlat;//摔扁--abc
    long flatTimeCount;//摔扁时间计数--abc
    Ccur_foe<float> jumpH_landH;//起跳高度和落地高度--abc
    int nkey;//多少个钥匙过关--abc
    int key_got;//已获得了多少个钥匙--abc
    Cship*pShip_latest;//最近驾驶的ship（如果当前pShip不为NULL，则pShip_latest==pShip)
    float H_sinkInWater;//吃水深度--abc
    bool rotLegs;//是否旋转腿--abc
    float xmin,xmax,ymin,ymax,zmin,zmax;//活动范围--abc
    bool isFly;//是否飞行--abc
    float v0up_basic;//基本竖直起跳速度--abc
    bool isBouncing;//是否处于自动弹跳状态（一但被加速)
    bool bouncingAlreadyAccelerated;//自动弹跳已被加速--abc
    CtimeCounter revTimeCounter_bouncingReceiveAccelerate;//自动弹跳接受加速倒计时--abc
    float dH_autoJump;//高度差多大时自动转为跳起状态（用于实现从墙头自然落下的效果)
    bool onFire;//是否着火--abc
    CtimeCounter timeCounter_onFire;//onFire计时--abc
    int nGun;//子弹路数--abc
    CtimeCounter revTimeCounter_shot;//射击结束倒计时--abc
    bool isShotTimeLastsLong;//是否是长延时--abc
    bool shotBallTrace;//是否进行子弹追踪--abc
    float deathY;//低于此值死亡--abc
    //----控制信号--abc
    bool rotSignal;
    bool jumpSignal;
    bool upSignal;
    bool dnSignal;
    bool backSignal;
    bool forwardSignal;
    bool jumpForwardSignal;
protected:
    bool m_isDied;
    vector<CshotBall*> pshotBallList;//子弹列表--abc
    Cship*pShip;//当前所驾驶的ship（如果当前不在ship上，则pShip==NULL)
    float Rc;
public:
    Ccell(){
        rotSignal=false;
        jumpSignal=false;
        upSignal=false;
        dnSignal=false;
        backSignal=false;
        forwardSignal=false;
        jumpForwardSignal=false;
        //
        isPuddleWater=false;
        isMovingOnSolidSurface=false;
        deathY=-c3d_INF;
        nGun=1;
        isShotTimeLastsLong=false;
        onFire=false;
        shotBallTrace=true;
        dH_autoJump=0.2;
        bouncingAlreadyAccelerated=true;
        isBouncing=false;
        v0up_basic=3;//2.5;
        isFly=false;
        sin_A_wing=sinf(0);
        cos_A_wing=cosf(0);
        wingIsMovingUp=true;
        nEateCoin.setFoe(-1);//为使isChange初始为true，从而迫使界面更新--abc
        nKillEnemy.setFoe(-1);//为使isChange初始为true，从而迫使界面更新--abc
        v_legsRot=0;
        cos_v_legsRot=cosf(c3d_PI/180*v_legsRot);
        sin_v_legsRot=sinf(c3d_PI/180*v_legsRot);
        rotLegs=false;
        H_sinkInWater=0;
        pShip=NULL;
        pShip_latest=NULL;
        nkey=0;
        key_got=0;
        H_causeFlat=c3d_INF;
        flatTimeCount=0;
        isFallFlat=false;
        Rc_small_value_flat=0;
        Rc_small_value_normal=0;
        m_isDied=false;
        RTmat_store=unitMat();
        Rc=0;
        v_forward_value=3;//2.7;
        v_forward_roll_value=0;
        cos_v_forward_roll_value=cosf(c3d_PI/180*v_forward_roll_value);
        sin_v_forward_roll_value=sinf(c3d_PI/180*v_forward_roll_value);
        v_back_value=2.7;//1.5;
        v_back_roll_value=0;
        cos_v_back_roll_value=cosf(c3d_PI/180*v_back_roll_value);
        sin_v_back_roll_value=sinf(c3d_PI/180*v_back_roll_value);
        v_rot=2;
        cos_v_rot=cosf(c3d_PI/180*v_rot);
        sin_v_rot=sinf(c3d_PI/180*v_rot);
        vup=0;
        a_gravity=0.05;
        xmin=ymin=zmin=-c3d_INF;
        xmax=ymax=zmax=c3d_INF;
    }
    virtual~Ccell(){
        
    }
/*    void addMolecule(Cmolecule*molecule){
        pmoleculeList.push_back(molecule);
        addChild(molecule);
    }
    int getMoleculeCount()const{return (int)pmoleculeList.size();}

    Cmolecule* getMoleculeByIndex(int index){
        assert(index>=0&&index<(int)pmoleculeList.size());
        return pmoleculeList[index];
    } */
    void setIsDied(bool value){
        m_isDied=value;
    }
    bool getIsDied()const{return m_isDied;}
    void setPosture(const Cc3dVector4&norm){
        setPosture_soft(norm, 1.0);
        
    }
    void setPosture_soft(const Cc3dVector4&norm,const float k)
    //调整位姿(软调整)，使局部坐标系的Y轴与世界空间中norm一致,norm须为单位向量--abc
    //k越小越软，k最大为1，为1时即硬调整--abc
    {
        Cc3dMatrix4 RTmat=this->getTransform()->getRTmat();
        //--------------------
        bool isSoft=(k!=1.0);//是否为软调整--abc
        Cc3dVector4 Xvec,Zvec;//局部空间X轴和Z轴的世界向量--abc
        Zvec.init(RTmat.getAt(8),RTmat.getAt(9),RTmat.getAt(10),0);//将RTmat[8~10]拷给Zvec，即取得局部空间z轴的世界向量（Zvec已为单位向量)
        if(fabsf(dot(Zvec, norm)-1)>0.01){//如果Zvec与norm不平行--abc
            //根据norm和Zvec计算新X轴--abc
            Xvec=cross(norm,Zvec);
            //虽然norm和Zvec都是单位向量，但由于二者不一定垂直，所以其叉乘结果不一定是单位向量（即|norm||Zvec|sinA未必等于1)
            //所以Xvec不一定是单位向量--abc
            //将Xvec单位化（如果是软调整，则由于后面要统一进行规范正交化，所以此处就可以省去规范化)
            if(!isSoft)Xvec=normalize(Xvec);
            //重新计算Z轴--abc
            Zvec=cross(Xvec,norm);//由于Xvec和norm垂直且都是单位向量，所以Zvec必是单位向量，不用再单位化--abc
        }else{//如果Zvec与norm平行--abc
            //改用X轴进行计算--abc
            Xvec.init(RTmat.getAt(0),RTmat.getAt(1),RTmat.getAt(2),0);//将RMmat[0~2]拷给Xvec，即取得局部空间x轴世界向量--abc
            //根据norm和Xvec计算新Z轴--abc
            Zvec=cross(Xvec,norm);//Xvec和norm虽然都是单位向量，但不一定垂直所以Zvec不一定是单位向量--abc
            //将Zvec单位化（如果是软调整，则由于后面要统一进行规范正交化，所以此处就可以省去规范化)
            if(!isSoft)Zvec=normalize(Zvec);
            //重新计算X轴--abc
            Xvec=cross(norm,Zvec);//由于norm与Zvec垂直，且二者都是单位向量，所以Xvec是单位向量，不用再单位化--abc
        }//得到Xvec,norm,Zvec
        //由Xvec,norm,Zvec构成新的坐标系，而平移分量仍取RTmat的--abc
        //制作新的位姿矩阵--abc
        Cc3dMatrix4 mat(
            Xvec.x(),Xvec.y(),Xvec.z(),0,//第一列--abc
            norm.x(),norm.y(),norm.z(),0,//第二列--abc
            Zvec.x(),Zvec.y(),Zvec.z(),0,//第三列--abc
            RTmat.getAt(12),RTmat.getAt(13),RTmat.getAt(14),1//第四列--abc
        );
        //软插值--abc
        if(isSoft){
            //在RTmat和mat之间进行一小步插值作为新RTmat
            for(int i=0;i<=15;i++){
                RTmat.setAt(i, RTmat.getAt(i)+(mat.getAt(i)-RTmat.getAt(i))*k);
            }
            //以上用简单的线性插会导致矩阵不再正交，所以此时要进行一下正交化--abc
            RTmat=orthogonalization3x3(RTmat);
        }else{
            RTmat=mat;
        }
        //--------------------------
        Cc3dTransform*transform=this->getTransform();
        transform->setRTmat(RTmat);

    }
    void setDir(const Cc3dVector4&dir){
        setDir_soft(dir, 1.0);
    }
    void setDir_soft(const Cc3dVector4&dir,const float k)
    //dir为世界坐标，dir必须是单位向量--abc
    //假设局部dir是(0,0,-1)
    {
        Cc3dMatrix4 RTmat=this->getTransform()->getRTmat();
        //-----------
        bool isSoft=(k!=1.0);//是否为软调整--abc
        //获得局部up的世界向量--abc
        Cc3dVector4 up(0,1,0,0);
        up=this->getTransform()->convertToWorldSpace(up);//up已转为世界坐标--abc
        //根据dir和up计算right
        Cc3dVector4 right=cross(dir, up);//虽然dir和up都是单位向量，但二者不一定垂直，所以right不一定是单位向量--abc
        if(!isSoft)right=normalize(right);//right单位化（如果是软调整，则由于后面要统一进行规范正交化，所以此处就可以省去规范化)
        //根据right和dir重新计算up
        up=cross(right, dir);//由于right与dir垂直且都为单位向量，所以得到的up必为单位向量--abc
        //将right作为X轴,up作为Y轴，-dir作为Z轴建立直角坐标系--abc
        //生成此坐标系的矩阵，同时将原来的RTmat的平移分量拷过来--abc
        Cc3dMatrix4 mat(
            right.x(),right.y(),right.z(),0,//第一列--abc
            up.x(),up.y(),up.z(),0,//第二列--abc
            -dir.x(),-dir.y(),-dir.z(),0,//第三列--abc
            RTmat.getAt(12),RTmat.getAt(13),RTmat.getAt(14),1//第四列--abc
        );
        //软插值--abc
        if(isSoft){
            //在RTmat和mat之间进行一小步插值作为新RTmat
            for(int i=0;i<=15;i++){
                RTmat.setAt(i, RTmat.getAt(i)+(mat.getAt(i)-RTmat.getAt(i))*k);
            }
            //以上用简单的线性插会导致矩阵不再正交，所以此时要进行一下正交化--abc
            RTmat=orthogonalization3x3(RTmat);
        }else{
            RTmat=mat;
        }
        //---------------
        this->getTransform()->setRTmat(RTmat);
    }
    
    

    void setRc(float _Rc){Rc=_Rc;}
    float getRc()const{return Rc;}
    vector<CshotBall*> getShotBallList()const{return pshotBallList;}
    void addShotBall(CshotBall*shotBall){
        pshotBallList.push_back(shotBall);
    
    }
    bool get_isAllKeyGot()const {
        if(key_got==nkey)return true;
        else return false;
    }
    bool get_isPuddleWater()const {
        return isPuddleWater;
    
    }
    void set_isPuddleWater(bool _isPuddleWater){
        isPuddleWater=_isPuddleWater;
    }
    void set_isMovingOnSolidSurface(bool _isMovingOnSolidSurface){
        isMovingOnSolidSurface=_isMovingOnSolidSurface;
    }
    bool get_isMovingOnSolidSurface()const {
        return isMovingOnSolidSurface;
    }
    void set_bouncingAlreadyAccelerated(bool _bouncingAlreadyAccelerated){
        bouncingAlreadyAccelerated=_bouncingAlreadyAccelerated;
    }
    bool get_bouncingAlreadyAccelerated()const {
        return bouncingAlreadyAccelerated;
    }
    void accelerateBouncing(){
        //如果是处于自动弹跳中且还未被加速过且刚开始起跳不久，则可以被加速--abc
        if(isBouncing&&get_bouncingAlreadyAccelerated()==false&&revTimeCounter_bouncingReceiveAccelerate.getCount()!=0){
            vup+=get_v0up_basic()*0.5;
            set_bouncingAlreadyAccelerated(true);
        }
    }

    bool get_isFly()const {
        return isFly;
    }
    void waveWing(){
        if((int)this->getModel()->getMeshByIndex(CELLPART_wingL)->getSubMeshCount()==0
           &&(int)this->getModel()->getMeshByIndex(CELLPART_wingR)->getSubMeshCount()==0)return;
        if(wingIsMovingUp){
            sin_A_wing+=0.15;
            if(sin_A_wing>0.5){
                wingIsMovingUp=false;
            }
        }else{//wingIsMovingUp==false
            sin_A_wing-=0.15;
            if(sin_A_wing<-0.5){
                wingIsMovingUp=true;
            }
        }
        cos_A_wing=1-0.5*sin_A_wing*sin_A_wing;
        this->getModel()->getMeshByIndex(CELLPART_wingL)->getTransform()->setRot(0, 0, 1, cos_A_wing, sin_A_wing);
        this->getModel()->getMeshByIndex(CELLPART_wingR)->getTransform()->setRot(0, 0, 1, cos_A_wing, -sin_A_wing);
    }
    void nEateCoinAdd(float dnCoin){
        nEateCoin.set(nEateCoin.get()+dnCoin);
    }
    float get_nEateCoin()const {
        return nEateCoin.get();
    }
    void nEateCoinReset(){
        nEateCoin.set(0);
    }
    void nKillEnemyAdd(){
        nKillEnemy.set(nKillEnemy.get()+1);
    }
    int get_nKillEnemy()const {
        return nKillEnemy.get();
    }
    void nKillEnemyReset(){
        nKillEnemy.set(0);
    }

    void setRange(float _xmin,float _xmax,float _ymin,float _ymax,float _zmin,float _zmax){
        xmin=_xmin;
        xmax=_xmax;
        ymin=_ymin;
        ymax=_ymax;
        zmin=_zmin;
        zmax=_zmax;
        
    }
    bool isInRange(){//是否在活动范围内--abc
        Cc3dVector4 pos=m_transform->getPos();
        float x=pos.x();
        float y=pos.y();
        float z=pos.z();
        if(x>xmin&&x<xmax&&y>ymin&&y<ymax&&z>zmin&&z<zmax){
            return true;
        }else{
            return false;
        }
    }
      void reSetRotLegs(){
        rotLegs=false;
    
    }
    void setRotLegs(bool _rotLegs){
        rotLegs=_rotLegs;
    }
    void set_H_sinkInWater(float _sinkH){
        H_sinkInWater=_sinkH;
    
    }
    void set_Rc_small(float _Rc_small){
        Rc_small=_Rc_small;
    }
    void getDownShip(){
        pShip=NULL;
    }
    void set_ship(Cship*_pShip);
    Cship* get_ship()const {
        return pShip;
    }
    Cship* get_ship_latest()const {
        return pShip_latest;
    }
    bool get_onShip()const {
        return (pShip!=NULL);
    }
    void set_H_causeFlat(float _H){
        H_causeFlat=_H;
    }
    float get_H_causeFlat()const{
        return H_causeFlat;
    
    }
    void doFallFlat(long flatDuringTime){
        flatTimeCount=flatDuringTime;
        isFallFlat=true;
        m_transform->setScale(1.1, 0.5, 1.1);
        set_Rc_small(Rc_small_value_flat);
        //change shader
        this->getModel()->getMeshList()[CELLPART_body]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
        this->getModel()->getMeshList()[CELLPART_body]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);
        this->getModel()->getMeshList()[CELLPART_shell]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texOnly_noSelfShadow"));
        this->getModel()->getMeshList()[CELLPART_shell]->setPassUnifoCallback(passUnifoCallback_texOnly_noSelfShadow);

    }
    void undoFallFlat(){//撤销摔扁--abc
        //使当前flatTime尽快结束--abc
        long _flatTimeCount=flatTimeCount;
        for(int i=0;i<_flatTimeCount;i++){
            doFlatTimeCount();
        }
    
    }
    bool getIsFallFlat()const{
        return isFallFlat;
    }
    void doFlatTimeCount(){
        if(flatTimeCount>0){
            flatTimeCount--;
            if(flatTimeCount==0){
                isFallFlat=false;
                m_transform->setScale(1, 1, 1);
                set_Rc_small(Rc_small_value_normal);
                jumpH_landH.setBoth(0);
                //change shader
                this->getModel()->getMeshList()[CELLPART_body]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_specular_noSelfShadow"));
                this->getModel()->getMeshList()[CELLPART_body]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_specular_noSelfShadow);
                this->getModel()->getMeshList()[CELLPART_shell]->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_diffuse_ambient_noSelfShadow"));
                this->getModel()->getMeshList()[CELLPART_shell]->setPassUnifoCallback(passUnifoCallback_diffuse_ambient_noSelfShadow);

            }
        }
    }
    CmobilePlatform* get_pInertialSurfaceMobilePlatform()const {
        //----计算pInertialSurfaceMobilePlatform
        CmobilePlatform*pInertialSurfaceMobilePlatform=NULL;
        if(isJump.get()==false){//如果落到surface上--abc
            if(environmentInfo.surfaceType==SURFACETYPE_MOBILEPLATFORM){//如果落在了移动平台上--abc
                pInertialSurfaceMobilePlatform=environmentInfo.pSurfaceMobilePlatform;
            }
        }
        return pInertialSurfaceMobilePlatform;

    }
    void setRc_small_value_normal(float _Rc_small_value){
        Rc_small_value_normal=_Rc_small_value;
    }
    void setRc_small_value_flat(float _Rc_small_value){
        Rc_small_value_flat=_Rc_small_value;
    
    }
    void submit(GLenum usage)//提交数据--abc
    {
        submitVertex(usage);
        submitIndex(usage);
    }


    void set_a_gravity(float _g){
        a_gravity=_g;
    }
    void doJump(float _vup){
        vup=_vup;
        isJump.set(true);
        jumpH_landH.set(m_transform->getPosY());
    }
    void doJump_bounce(float _vup){//自动弹跳--abc
        doJump(_vup);
        isBouncing=true;
        bouncingAlreadyAccelerated=false;
        revTimeCounter_bouncingReceiveAccelerate.setValue(10);
    }
    bool isJumping(){
        return isJump.get();
    }
    void endJumping(){
        vup=-0.01;//不是归0，而是给一个很小的向下的速度，以便着陆更稳，尤其是在运动平向上时--abc
        isJump.set(false);
        isBouncing=false;
    }
    void updateJumpingCurFoe(){
        isJump.setFoe(isJump.get());
    }
    float get_vupInertial_causedByMobilePlatform(){//获得在移动平台影响下产生的竖直方向上的惯性速度--abc
        CmobilePlatform*pInertialPlat=this->get_pInertialSurfaceMobilePlatform();
        float vupInertial=pInertialPlat?pInertialPlat->k_inertialVup*pInertialPlat->movVec.y():0;
        return vupInertial;
    }
    float get_v0up_basic()const {
        return v0up_basic;
    }
    void doTriggerCeilPlatform()
    {
        if(isTouchCeil(1.0)){
            if(environmentInfo.ceilType==CEILTYPE_MOBILEPLATFORM){
                CmobilePlatform*pPlatform=environmentInfo.pCeilMobilePlatform;
                if(pPlatform){
                        pPlatform->doTrigger_mov_byTouchCeil();
                }
                if(pPlatform){
                        pPlatform->doTrigger_rot_byTouchCeil();
                }
            }

        }
        
    }

    void doTriggerSurfacePlatform() {
        if(isTouchSurface(1.0)){
            if(environmentInfo.surfaceType==SURFACETYPE_MOBILEPLATFORM){
                CmobilePlatform*pPlatform=environmentInfo.pSurfaceMobilePlatform;
                if(pPlatform){
                    pPlatform->doTrigger_mov_byTouchSurface();
                }
                if(pPlatform){
                    pPlatform->doTrigger_rot_byTouchSurface();
                }
            }
  
        }

    }
    void doRiseAndFall()//升起和下降--abc
    //triggerPlatform指示是否触发移动平台--abc
    {
        updateJumpingCurFoe();
        if(isJumping()==false)return;
        //自由落体--abc
        {
            vup-=a_gravity;
            this->m_transform->moveRelativeToFather(0, vup, 0);
        }
        //判断是否接触天花板--abc
        if(isTouchCeil(0)){
            isJump.set(true);//如果不处于跳起状态，也人为令其为跳起状态--abc
            vup=-0.15;//为了不与天花板发生“粘黏”，不是将vup设为0，而是设为一个负值（即给一个向下的初速度)
            m_transform->setPosY(environmentInfo.ceilH-Rc_small);//让Rc_small上沿正好与天花板重合，注意，一定要正好重合，千万不要加margin，否则会对其它地方的isTouchCeil()的判断产生影响--abc
            Cc3dVector4 norm=Cc3dVector4(0,0,0,0)-environmentInfo.ceilNorm;//取CeilNorm的负值--abc
            setPosture_soft(norm,0.4);
        }
        //判断是否接触surface
        switch(environmentInfo.surfaceType){
            case SURFACETYPE_MOBILEPLATFORM:
            case SURFACETYPE_GROUND:
            case SURFACETYPE_ARCH:
            {
                if(isTouchSurface(1.0)){//接触表面--abc
                    endJumping();
                    jumpH_landH.set(m_transform->getPosY());
                    //自动弹跳--abc
                    CmobilePlatform*pPlatform=environmentInfo.pSurfaceMobilePlatform;
                    if(pPlatform!=NULL&&pPlatform->get_isBouncing()&&isJump.get()==false){//是弹跳平台--abc
                        //跳起--abc
                        float v0up=this->get_vupInertial_causedByMobilePlatform()+pPlatform->bounceVup;
                        doJump_bounce(v0up);
                        //调整位姿(为了避免不断弹跳而位姿却一直来不及调整)
                        adjustPosAndPostureToSurface(adjustSoftness_MIDSOFT);
                        //声音--abc
                      //  pPlatform->pALSource_plat_bouncing->updatePosition(getTransform().getPos());
                      //  pPlatform->pALSource_plat_bouncing->play();
                        pPlatform->getALSourceByName("plat_bouncing")->updatePosition(getTransform()->getPos());
                        pPlatform->getALSourceByName("plat_bouncing")->play();
                        
                    }
                    
                }else{
                    isJump.set(true);
                }
            }break;
            case SURFACETYPE_WATER:
                //水面特殊，要沉下去一段才算接触，然后慢慢浮起，更生动--abc
                float solidSurfaceH=max(environmentInfo.groundH,environmentInfo.archSurfaceH);
                bool stopFalling=this->getTransform()->getPos().y()-Rc_small<=max(solidSurfaceH,environmentInfo.waterH-H_sinkInWater-10);
                if(stopFalling){
                    endJumping();
                    jumpH_landH.set(m_transform->getPosY());
                    //声音--abc
                 //   pALSsource_fall_in_water->updatePosition(getTransform().getPos());
                 //   pALSsource_fall_in_water->play();
                    if(this->getALSourceByName("fall_in_water")){
                    this->getALSourceByName("fall_in_water")->updatePosition(getTransform()->getPos());
                    this->getALSourceByName("fall_in_water")->play();
                    }
                }else{
                    isJump.set(true);
                }
                               break;
        }
    }
    void storeRTmat(){
        RTmat_store=m_transform->getRTmat();
    }
    Cc3dVector4 getDir()const {
        float dir_local[4]={0,0,-1,0};
        Cc3dVector4 dir_world=this->m_transform->convertToWorldSpace(dir_local);
        return dir_world;
    
    }
    Cc3dVector4 getUp()const {
        float up_local[4]={0,1,0,0};
        Cc3dVector4 up_world=this->m_transform->convertToWorldSpace(up_local);
        return up_world;
    }
    void set_v_forward_roll_value(float _value){
        v_forward_roll_value=_value;
        cos_v_forward_roll_value=cosf(c3d_PI/180*v_forward_roll_value);
        sin_v_forward_roll_value=sinf(c3d_PI/180*v_forward_roll_value);
    }
    void set_v_back_roll_value(float _value){
        v_back_roll_value=_value;
        cos_v_back_roll_value=cosf(c3d_PI/180*v_back_roll_value);
        sin_v_back_roll_value=sinf(c3d_PI/180*v_back_roll_value);
    
    }
    virtual void movForward(){
        m_transform->moveZ( -v_forward_value);
    }
    void movForward(long startTime,long curTime,long howLongToTop)
    //startTime为起时刻，其时速度为0
    //curTime是当前时刻--abc
    //howLongToTop是经过多长时间到达峰值，峰值为v_forward_value
    {
        //根据线性插值计算curTime时的速度--abc
        float v;
        long curIsHowLong=curTime-startTime;
        if(curIsHowLong>=howLongToTop){
            v=v_forward_value;
        }else{
            v=(float)(curIsHowLong*v_forward_value)/maxf(1,howLongToTop);//防止分母为0
        }
        m_transform->moveZ(-v);
    
    }
    virtual void movBack(){
        m_transform->moveZ( v_back_value);
    }
    void movBack(float dis){//后退一定距离--abc
        m_transform->moveZ(dis);
 
    }
    void movForward(float dis){
        m_transform->moveZ( -dis);
        
    }
    virtual void rotLeft(){
        m_transform->rotate(0, 1, 0, cos_v_rot,sin_v_rot);
   
    }
     virtual void rotRight(){
        m_transform->rotate(0, 1, 0, cos_v_rot,-sin_v_rot);
        
    }
    void rotRight(float A){
        m_transform->rotate(0, 1, 0, cosf(A),-sinf(A));
    }
    void rotLeft(float A){
        m_transform->rotate(0, 1, 0, cosf(A),sinf(A));
    }


    bool isTouchSurface(const float hmargin)
    //目前在此游戏中hmargin大部分情况统一取1.0，个别特殊情况可能取得更大些--abc
    //要注意保持一致性--abc
    {//判断是否接触表面--abc
        //有hmargin，使得touchSurface的判断不容易miss，避免穿越表面--abc
        return this->getTransform()->getPos().y()-this->Rc_small-hmargin<=environmentInfo.surfaceH;

        
    }
    bool isTouchCeil(const float hmargin) {//判断是否接触天花板--abc
        //有hmargin，使得touchSurface的判断不空易miss，避免穿越天花板--abc
        return this->getTransform()->getPos().y()+this->Rc_small+hmargin>=environmentInfo.ceilH;
        
    }
    
    void pushWater(Cwater&water){
        water.push();
    }
  
    virtual void adjustPosAndPostureToSurface(int adjustSoftness=adjustSoftness_VERYSOFT)
	//调整高度用软调整可以避免严格贴地行走，实现一定飘逸--abc
    //前端返回是否接触表面--abc
    {
        float k_ground_posture,k_water_posture,k_arch_posture,k_ground_H,k_water_H;
        switch (adjustSoftness) {
            case adjustSoftness_VERYSOFT:
                k_ground_posture=0.12;
                k_water_posture=0.166;
                k_arch_posture=0.2;
                k_ground_H=0.12;
                k_water_H=0.166;
                break;
            case adjustSoftness_MIDSOFT:
                k_ground_posture=0.7;
                k_water_posture=0.7;
                k_arch_posture=0.8;
                k_ground_H=0.7;
                k_water_H=0.7;
                break;
            case adjustSoftness_HARD:
                k_ground_posture=1;
                k_water_posture=1;
                k_arch_posture=1;
                k_ground_H=0.7;
                k_water_H=0.7;
                break;
            default:
                cout<<"error:未定义的adjustSoftness类型!"<<endl;
                assert(false);
                break;
        }
        //--------------------
		Cc3dVector4 pos=this->m_transform->getPos();
		//求house地面，地面，水面三者高度的最大值并与pcell当前高度进行比较--abc
        const float margin=1.0;//0.5;
		if(isTouchSurface(margin+dH_autoJump)==false){//如果此时还没有接触地面，则恢复跳跃状态（自由落体)
            isJump.set(true);//这个动作非常重要，是使得主角能从墙头正确自然下落的关键--abc
		}else{
			//根据topSurface决定不同的调整动作--abc
			switch(environmentInfo.surfaceType){
                case SURFACETYPE_MOBILEPLATFORM:
                case SURFACETYPE_ARCH:
				{ //着陆到建筑表面--abc
                    //调整模型高度--abc
                    Cc3dMatrix4 RTmat=m_transform->getRTmat();
                    RTmat.setAt(13, environmentInfo.surfaceH+Rc_small+margin);
                    m_transform->setRTmat(RTmat);
                    //调整姿态--abc
                    setPosture_soft(environmentInfo.surfaceNorm,k_arch_posture);
                }
                    break;
                case SURFACETYPE_GROUND:
				{
                        downToSurface_soft(Rc_small,environmentInfo.surfaceH+margin,
                                           environmentInfo.surfaceNorm,k_ground_H, k_ground_posture ) ;//主角软着陆到地面--abc
                }
                    break;
                case SURFACETYPE_WATER:
				{
                    downToSurface_soft(Rc_small,environmentInfo.surfaceH+margin-H_sinkInWater,
                                       environmentInfo.surfaceNorm, k_water_H,k_water_posture);
				}
                    break;
			}
		}
	}

    
    void adjustH(int adjustSoftness=adjustSoftness_VERYSOFT)
    //简化的着陆，只调整高度，不调整姿态、
    //调整高度用软调整可以避免严格贴地行走，实现一定飘逸--abc
	{
        float k_ground_H,k_water_H;
        switch (adjustSoftness) {
            case adjustSoftness_VERYSOFT:
                k_ground_H=0.12;
                k_water_H=0.166;
                break;
            case adjustSoftness_MIDSOFT:
                k_ground_H=0.7;
                k_water_H=0.7;
                break;
            case adjustSoftness_HARD:
                k_ground_H=1;
                k_water_H=1;
                break;
            default:
                cout<<"error:未定义的adjustSoftness类型!"<<endl;
                assert(false);
                break;
        }
        Cc3dVector4 pos=this->m_transform->getPos();
        //必须有假跳--abc
        const float margin=1.0;//0.5;
        if(isTouchSurface(margin+dH_autoJump)==false){//如果此时还没接触地面，则回到跳跃状态（自由落体)
            isJump.set(true);//这个动作非常重要，是使得主角能从墙头正确自然下落的关键--abc
		}else{
            //调整高度--abc
            switch(environmentInfo.surfaceType){
                case SURFACETYPE_MOBILEPLATFORM:
                case SURFACETYPE_ARCH:
                {
                    //调整模型高度，为了使越过高点时假跳能够发生，必须用软移动--abc
                    Cc3dMatrix4 RTmat=m_transform->getRTmat();
                    RTmat.setAt(13, environmentInfo.surfaceH+Rc_small+margin);
                    m_transform->setRTmat(RTmat);
                }
                    break;
                case SURFACETYPE_GROUND:
                {
                    downToSurface_soft(Rc_small,environmentInfo.surfaceH+margin,
                                       environmentInfo.surfaceNorm, k_ground_H, 1,false) ;//软着陆到地面，但不调整姿态--abc
                }
                    break;
                case SURFACETYPE_WATER:
                {
                    downToSurface_soft(Rc_small,environmentInfo.surfaceH+margin-H_sinkInWater,
                                       environmentInfo.surfaceNorm, k_water_H,1,false);//软着陆到水面，但不调整姿态--abc
                }
                    break;
            }
       
        }
	}
    void collisionReAct_simple(float d,const Cc3dVector4&e_back,const Cc3dVector4&posf)
    //posf为恢复位置--abc
    {
        if(d<=0)return;
        //恢复平移分量，不恢复旋转分量，因为允许旋转--abc
        Cc3dMatrix4 RTmat=m_transform->getRTmat();
        RTmat.setAt(12, posf.x());
        RTmat.setAt(13, posf.y());
        RTmat.setAt(14, posf.z());
        m_transform->setRTmat(RTmat);
        Cc3dVector4 back;
        float d_back=d*0.35;//0.25;//d*0.1//除了恢复原位外另加一个回退--abc
        back=e_back*d_back;
        back.sety(0);
        m_transform->moveRelativeToFather(back.x(),back.y(),back.z());

    }
    void collisionReAct_slip(float d,const Cc3dVector4&e_back){
        if(d<=0)return;
        //求主角前进方向在XZ上的投影--abc
        Cc3dVector4 movVecXZ(m_transform->getRTmat().getArray()[12]-RTmat_store.getAt(12),0,m_transform->getRTmat().getArray()[14]-RTmat_store.getAt(14),0);
        //恢复平移分量，不恢复旋转分量，因为允许旋转--abc
        Cc3dMatrix4 RTmat=m_transform->getRTmat();
        RTmat.setAt(12, this->RTmat_store.getAt(12));
        RTmat.setAt(13, this->RTmat_store.getAt(13));
        RTmat.setAt(14, this->RTmat_store.getAt(14));
        m_transform->setRTmat(RTmat);
        Cc3dVector4 back;
        float d_back=d*0.35;//0.25;//0.1;//除了恢复原位外另加一个回退--abc
        back=e_back*d_back;
        back.sety(0);
        m_transform->moveRelativeToFather(back.x(),back.y(),back.z());
        //----滑动--abc
        const float k_slip=0.4;
        if(dot(movVecXZ,e_back)<=0)
            //movVecXZ在e_back上投影为负时才滑动--abc
            //这是为了避免墙角处由于e_back计算误差而造成滑向墙壁的另一面--abc
        {
            //求e_back在XZ平面上的垂直向量--abc
            Cc3dVector4 e_tangentXZ(-e_back.z(),0,e_back.x(),0);
            //求moVecXZ在e_tangentXZ的投影--abc
            float dot_movVecXZ_e_tangentXZ=dot(movVecXZ, e_tangentXZ);
            //求受限移动向量在XZ上的投影--abc
            Cc3dVector4 movVecRestrictXZ=e_tangentXZ*(dot_movVecXZ_e_tangentXZ*k_slip);//乘以一个因子是为了减慢滑动速度--abc
            //按movVecRestrictXZ进行移动--abc
            m_transform->moveRelativeToFather(movVecRestrictXZ.x(),movVecRestrictXZ.y(),movVecRestrictXZ.z());
        }else{//如果movVecXZ在e_back上投影>0，则直接沿movVecXZ移动（无阻挡)
            //减慢滑动速度--abc
            Cc3dVector4 movVecXZ_slower=movVecXZ*k_slip;
            m_transform->moveRelativeToFather(movVecXZ_slower.x(),movVecXZ_slower.y(),movVecXZ_slower.z());
            
        }


    
    }
    float collisionTestWithArchWall(CarchScene&archScene,bool multiPoint,const vector<int>&skipTagList,const int intended_nSP,Cc3dVector4&e_back){
        //主角的位置--abc
        Cc3dVector4 pos=this->m_transform->getPos();
        //主角的向上方向--abc
        Cc3dVector4 up=Cc3dVector4(m_transform->getRTmat().getArray()+4);
        up.setw(0);
  
        //碰撞--abc
        if(multiPoint){
            return archScene.collisionTestWithWall_multiPoint(pos,Rc,Rc_small_value_normal,up,e_back,skipTagList,intended_nSP);
        }else{
            return archScene.collisionTestWithWall_singlePoint(pos,Rc,Rc_small_value_normal,up,e_back,skipTagList,intended_nSP);
        }
    }
    virtual bool collisionAndReActWithArchWall(CarchScene&archScene,bool multiPoint,const vector<int>&skipTagList,const int intended_nSP){
        float d;
        Cc3dVector4 e_back;
        d=collisionTestWithArchWall(archScene, multiPoint, skipTagList, intended_nSP, e_back);
        collisionReAct_slip(d,e_back);
        return (d>0);
    }
  
    void set_v_legsRot(float _v_legsRot){
        v_legsRot=_v_legsRot;
        cos_v_legsRot=cosf(c3d_PI/180*v_legsRot);
        sin_v_legsRot=sinf(c3d_PI/180*v_legsRot);
    
    }
    void legsRot(){//腿旋转--abc
        if(rotLegs==false)return;
        getModel()->getMeshByIndex(CELLPART_legs)->getTransform()->rotate(0, 1, 0, cos_v_legsRot, sin_v_legsRot);
    }
    void doOnFireTimeCount(){
        if(onFire==false)return;
        //计时--abc
        if(timeCounter_onFire.getCount()>onFireDuringTime){//如果已超过持续时间--abc
            onFire=false;//onFire消失--abc
            timeCounter_onFire.reSet();//重置计时器--abc
        }else{//如果未超过持续时间--abc
            //计时--abc
            timeCounter_onFire.doCount();
        }
    }
    void loadConfig(const string&fileNameWithExt){
        //------------------获得绝对路径--abc
        string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(fileNameWithExt);
        //打开文件--abc
        FILE  *fp=NULL;
        fp=fopen(pathName.c_str(),"r");
        if(fp==NULL)
        {
            cout<<"Open "<<pathName<<" failed!"<<endl;
            assert(false);
        }
        //------------------提取文件信息，并建立列模型列表--abc
        {
            char buffer[1024]={0};
            //提取初始位置--abc
            float initPos[3];
            fscanf(fp, "%f",&initPos[0]);
            fscanf(fp, "%f",&initPos[1]);
            fscanf(fp, "%f",&initPos[2]);
            //提取初始旋转矩阵--abc
            float initRmat[16];
            for(int j=0;j<16;j++){
                fscanf(fp, "%f",&initRmat[j]);
            }
            //根据initPos和initRMat设置transf.RTmat
            Cc3dMatrix4 RTmat=m_transform->getRTmat();
            RTmat.init(initRmat);
            RTmat.setAt(12, initPos[0]);
            RTmat.setAt(13, initPos[1]);
            RTmat.setAt(14, initPos[2]);
            m_transform->setRTmat(RTmat);
            //提取fuelValue
            float fuelValue;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&fuelValue);
            fuel.set_maxValue(fuelValue);
            fuel.chargeFull();
            //提取deathY
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&this->deathY);
            
        }
        //关闭文件--abc
        fclose(fp);
    }

    void set_onFire(bool _onFire){
        onFire=_onFire;
    }


    void shotBall_collisionWithArch(CarchScene&archScene,CblastSet&burstSet,Cc3dCamera&camera){
        int nball=(int)pshotBallList.size();
        for(int i=0;i<nball;i++){
            CshotBall*pball=pshotBallList[i];
            assert(pball);
            if(pball->getIsActive()==false){
                assert(pball->getIsVisible()==false);
                continue;
            }
            pball->collisionWithArch(archScene,burstSet,camera);
        }
        
    }
    void shotBall_collisionWithMobilePlatform(CcollisionGrid&collisionGrid_mobilePlatform,CblastSet&burstSet,Cc3dCamera&camera){
        int nball=(int)pshotBallList.size();
        for(int i=0;i<nball;i++){
            CshotBall*pball=pshotBallList[i];
            if(pball->getIsActive()==false){
                assert(pball->getIsVisible()==false);
                continue;
            }
            Cc3dVector4 ballpos=pball->getTransform()->getPos();
            //获得pball周围的mobilePlatform列表--abc
            CmobilePlatformSet mobilePlatformSet_selected;
            mobilePlatformSet_selected.setActorList(collisionGrid_mobilePlatform.getCollisonList(ballpos));
            pball->collisionWithMobilePlatform(mobilePlatformSet_selected,burstSet,camera);
            
        }
        
    }
    void shotBall_collisionWithGround(const Cground*ground,CblastSet&burstSet,Cc3dCamera&camera){
        int nball=(int)pshotBallList.size();
        for(int i=0;i<nball;i++){
            CshotBall*pball=pshotBallList[i];
            if(pball->getIsActive()==false){
                assert(pball->getIsVisible()==false);
                continue;
            }
            pball->collisionWithGround(ground,burstSet,camera);
            
        }
        
    }
    int getActiveShotBallCount()const {
        int count=0;
        int nball=(int)pshotBallList.size();
        for(int i=0;i<nball;i++){
            CshotBall*pball=pshotBallList[i];
            if(pball->getIsActive()==false){
                assert(pball->getIsVisible()==false);
                continue;
            }
            count++;
            
        }
        return count;
        
    }

    
    void shotBall_mov(/*const Cwater&water*/){//子弹移动--abc
        //获得cell位置--abc
        Cc3dVector4 cellpos=this->m_transform->getPos();
        //将所有active的子弹前移一步--abc
        int nShotBall=(int)pshotBallList.size();
        for(int i=0;i<nShotBall;i++){
            CshotBall*pball=pshotBallList[i];
            if(pball->getIsActive()==false){
                assert(pball->getIsVisible()==false);
                continue;
            }
            //移动前存储位置--abc
            pball->storePos();
            //对于指向敌人的pball，判断pball指向的敌人是否已死，如果已死则将pball->ptgBoss置空--abc
            if(pball->ptgBoss!=NULL&&((Ccell*)pball->ptgBoss)->getIsDied()){
                pball->ptgBoss=NULL;
            }
            //移动--abc
            {
                if(pball->ptgBoss==NULL){//如果ptgBoss为空--abc
                    //前进--abc
                    pball->getTransform()->moveZ( -v_shotBall);
                    pball->pathLen_add(v_shotBall);
                }else{//如果ptgBoss不为空--abc
                    //向ptgBoss转向--abc
                    Cc3dVector4 dir=pball->getDir();//当前dir
                    Cc3dVector4 _dir=((Ccell*)pball->ptgBoss)->getTransform()->getPos()-pball->getTransform()->getPos();
                    _dir=normalize(_dir);
                    //_dir和dir进行插值，结果存到dir
                    const float k=0.1;
                    dir=softMov(dir, _dir, k);
                    //为dir为z轴建立正交矩阵--abc
                    //right=dir x up
                    //up= right x dir
                    //Rmat=(right,up,-dir)
                    Cc3dVector4 up(0,1,0,0);
                    Cc3dVector4 right=cross(dir, up);
                    up=cross(right, dir);
                    float Rmat[16]={
                        right.x(),right.y(),right.z(),0,//第一列--abc
                        up.x(),up.y(),up.z(),0,//第二列--abc
                        -dir.x(),-dir.y(),-dir.z(),0,//第三列--abc
                        0,0,0,1//第四列--abc
                    };
                    pball->getTransform()->setRmat(Rmat);
                    //前进--abc
                    pball->getTransform()->moveZ( -v_shotBall);
                    pball->pathLen_add(v_shotBall);
                }
            }
            //施加重力的影响--abc
            pball->update_vy();
            pball->movDueToGravity();
            //判断子弹是否达到射程或者达到ptgBoss
            //获得pball位置--abc
            Cc3dVector4 ballpos=pball->getTransform()->getPos();
            //如果pball与cell较远，则炮弹转为active=false
            if(pball->get_pathLen()>pball->get_shotBallReachDis()){
                pball->turnToInActive();
                pball->setIsVisible(false);
            }
            //更新pball的生源位置--abc
            //pball->pALSource_launchSound->updatePosition(pball->getTransform().getPos());
            pball->getALSourceByName("launchSound")->updatePosition(pball->getTransform()->getPos());
            
        }
     
    }
    void set_nGun(int n){
        nGun=n;
    }
    void shotBall_launch(const vector<Ccell*>&pTracedBossList,int shotBallTypeID,bool giveUpTraceIfBossIsGroundBossAndMeIsNotJumpingAndShotBallNeedToBendDown=false)
    //发射子弹--abc
    //pFlyLiveBossList为活着的飞行boss列表--abc
    {
        //生成nGun路子弹--abc
        for(int i=0;i<nGun;i++){
            float A=(i-(nGun-1)*0.5)*5*c3d_PI/180;
            float cosA=cosf(A);
            float sinA=sinf(A);
            //从pshotBallList中找一个active==false且shotBallTypeID与传入的shotBallTypeID相匹配的元素--abc
            CshotBall*pshotBall=NULL;
            int nShotBall=(int)pshotBallList.size();
            for(int j=0;j<nShotBall;j++){
                CshotBall*pball=pshotBallList[j];
                if(pball->getIsActive()==false&&pball->get_typeID()==shotBallTypeID){
                    assert(pball->getIsVisible()==false);
                    pshotBall=pshotBallList[j];
                    break;
                }
            }
            if(pshotBall==NULL)break;
            //将pshotBall置为active
            pshotBall->turnToActive();
            pshotBall->setIsVisible(true);
            //pshotBall的vy复位--abc
            pshotBall->reSet_vy();
            //pshotBall的pathLen复位--abc
            pshotBall->set_pathLen(0);
            //为pshotBall定位--abc
            //先让其与cell方位一致--abc
            pshotBall->getTransform()->setRTmat(this->getTransform()->getRTmat());
            //旋转一个角度--abc
            pshotBall->getTransform()->rotate(0, 1, 0, cosA, sinA);
            //调整高度--abc
            pshotBall->getTransform()->moveY(5);//2.3//1.6//1.3
            //求pshotBall->ptgBoss
            if(shotBallTrace){//如果追踪--abc
                Cc3dVector4 dir=pshotBall->getDir();//pshotBall的单位方向向量--abc
                //看pshotBall的射线与哪个敌人最近，就让其ptgBossCell就指向谁--abc
                const float shotBallReachDis=pshotBall->shotBallReachDis;
                const float k2=0.57*0.57;
                vector<Ccell*> ptgBossList;
                vector<float> d2_toThisList;
                pshotBall->ptgBoss=NULL;
                int nLiveBoss=(int)pTracedBossList.size();
                for(int i=0;i<nLiveBoss;i++){
                    Ccell*pboss=pTracedBossList[i];
                    float d2_toThis=getLength2(pboss->getTransform()->getPos()-this->getTransform()->getPos());//boss到this的距离--abc
                    if(d2_toThis>shotBallReachDis*shotBallReachDis)continue;
                    float d2_toLine=d2_pointToRay(pshotBall->getTransform()->getPos(), pshotBall->getDir(), pboss->getTransform()->getPos());//boss到子弹射线的距离--abc
                    if(d2_toLine<d2_toThis*k2+10){
                        if(giveUpTraceIfBossIsGroundBossAndMeIsNotJumpingAndShotBallNeedToBendDown&&isJump.get()==false&&pboss->get_isFly()==false){//如果对地面敌人在需要向下转弯的情况下放弃追踪，且pboss恰好是地面敌人--abc
                            //判断追踪pboss是否需要向下转弯--abc
                            Cc3dVector4 _dir=pboss->getTransform()->getPos()-pshotBall->getTransform()->getPos();
                            _dir=normalize(_dir);
                            //看dir到_dir是否要向下转--abc
                            if(dir.y()>_dir.y()+0.01){//需要向下转--abc
                                //不合要求--abc
                                continue;
                            }
                        }
                        ptgBossList.push_back(pboss);
                        d2_toThisList.push_back(d2_toThis);
                    }
                }
                //看ptgBossList中哪个离this最近，就令pshotBall->ptgBoss等于谁--abc
                float mind2=c3d_INF;
                Ccell*pMind2Boss=NULL;
                int nTgBoss=(int)ptgBossList.size();
                for(int i=0;i<nTgBoss;i++){
                    Ccell*pboss=ptgBossList[i];
                    float d2_toThis=d2_toThisList[i];
                    if(d2_toThis<mind2){
                        mind2=d2_toThis;
                        pMind2Boss=pboss;
                    }
                }//得到mind2和pMind2Boss
                pshotBall->ptgBoss=pMind2Boss;//得到pshotBall->ptgBoss
            }else{//如果不追踪--abc
                //ptgBoss为NULL
                pshotBall->ptgBoss=NULL;
            }
            //播放声音--abc
            //pshotBall->pALSource_launchSound->updatePosition(pshotBall->getTransform().getPos());
            //pshotBall->pALSource_launchSound->play();
            pshotBall->getALSourceByName("launchSound")->updatePosition(pshotBall->getTransform()->getPos());
            pshotBall->getALSourceByName("launchSound")->play();
            
        }
        
    }

    void movCellAndShotBallsWithInertMobilePlatform(){
        //----主角随着其所属的惯性平台一起运动--abc
        CmobilePlatform*pInertialSurfaceMobilePlatform=get_pInertialSurfaceMobilePlatform();
        if(pInertialSurfaceMobilePlatform){
            if(isJump.get()==false){//如果没有挑起--abc
                CmobilePlatform*pPlatform=pInertialSurfaceMobilePlatform;
                //----cell随平台一起运动（下面代码包含了平动和转动)
                //求M(cell in platform_foe)=inv(M(platform_foe))*M(cell)
                Cc3dMatrix4 invM_platform_foe=inverse(Cc3dMatrix4(pPlatform->RTmat_store));
                Cc3dMatrix4 M_cell_in_platform_foe=invM_platform_foe*m_transform->getRTmat();
                //求cell随pPlatform变换后矩阵M(cell_now)=M(platform_now)*M(cell in platform_foe)
                m_transform->setRTmat(pPlatform->getTransform()->getRTmat()* M_cell_in_platform_foe);
            }else{//如果跳起了，则具受平台影响有一个平移速度，包括水平和竖直的（下面代码没考虑转动的影响)
                const float *v_inertPlatform=pInertialSurfaceMobilePlatform->movVec.getArray();
                //但cell所具有的平移速度要比v_inertPlatform小些--abc
                float v_inertCell[3]={0.5*v_inertPlatform[0],0,0.5*v_inertPlatform[2]};//注意，竖直不作位移，因为已在跳起时增加了初速度--abc
                m_transform->moveRelativeToFather(v_inertCell[0], v_inertCell[1], v_inertCell[2]);
            }
            //所有的shotBall受到平移速度影响--abc
            {
                const float *v_inertPlatform=get_pInertialSurfaceMobilePlatform()->movVec.getArray();
                int nShotBall=(int)pshotBallList.size();
                for(int i=0;i<nShotBall;i++){
                    CshotBall*pball=pshotBallList[i];
                    if(pball->getIsActive()){
                        assert(pball->getIsVisible()==true);
                        pball->getTransform()->moveRelativeToFather(v_inertPlatform[0], v_inertPlatform[1], v_inertPlatform[2]);
                    }
                }
            }
        }
        
        
        
    }
    virtual bool collisionAndReActWithmobilePlatformWall(CmobilePlatformSet&mobilePlatformSet,bool triggerCollisionPlatforms,bool multiPoint,const int intended_nSP)
    //必须放在着陆之后，否则容易发生与地面之下的墙面进行误碰撞--abc
    //前端返回是否碰撞--abc
    //triggerCollisionPlatforms指示是否将发生碰撞的移动平台触发--abc
    {
        if(multiPoint){
            return collisionAndReActWithmobilePlatformWall_multiPoint(mobilePlatformSet, triggerCollisionPlatforms, intended_nSP);
        }else{
            return collisionAndReActWithmobilePlatformWall_singlePoint(mobilePlatformSet, triggerCollisionPlatforms, intended_nSP);
        }
    }

private:
    virtual bool collisionAndReActWithmobilePlatformWall_multiPoint(CmobilePlatformSet&mobilePlatformSet,bool triggerCollisionPlatforms,const int intended_nSP)
    //必须放在着陆之后，否则容易发生与地面之下的墙面进行误碰撞--abc
    //前端返回是否碰撞--abc
    //triggerCollisionPlatforms指示是否将发生碰撞的移动平台触发--abc
    {
        //是否碰撞--abc
        bool collision=false;
        //主角的位置--abc
        Cc3dVector4 pos=this->getTransform()->getPos();
        //主角的向上方向--abc
        Cc3dMatrix4 t_m=m_transform->getRTmat();
        Cc3dVector4 up(t_m.getAt(4),t_m.getAt(5),t_m.getAt(6),0);
        //与mobilePlatformSet中各platform进行碰撞检测及碰撞反应--abc
        int n=(int)mobilePlatformSet.getActorCount();
        for(int i=0;i<n;i++){
            CmobilePlatform*pmobilePlatform=(CmobilePlatform*)mobilePlatformSet.getActorByIndex(i);
            if(pmobilePlatform->getIsDied())continue;//跳过死亡的运动平台--abc
            //求pmobilePlatform的e_backList和dList
            Cc3dVector4 e_back;
            float d=collisionTestWithWall_multiPoint_common(pmobilePlatform->getCollisionTriList(), pos, Rc, Rc_small_value_normal,up, e_back,intended_nSP);
            if(d>0){//碰撞--abc
                collision=true;
                if(triggerCollisionPlatforms){
                    //将platform触发--abc
                    pmobilePlatform->doTrigger_mov_byTouchWall();
                }
                if(triggerCollisionPlatforms){
                    //将platform触发--abc
                    pmobilePlatform->doTrigger_rot_byTouchWall();
                }
                //碰撞反应--abc
                collisionReAct_slip(d,e_back);
                
                break;
            }
            
        }
        return collision;
    }
    virtual bool collisionAndReActWithmobilePlatformWall_singlePoint(CmobilePlatformSet&mobilePlatformSet,bool triggerCollisionPlatforms,const int intended_nSP)
    //必须放在着陆之后，否则容易发生与地面之下的墙面进行误碰撞--abc
    //前端返回是否碰撞--abc
    //triggerCollisionPlatforms指示是否将发生碰撞的移动平台触发--abc
    {
        //是否碰撞--abc
        bool collision=false;
        //主角的位置--abc
        Cc3dVector4 pos=this->m_transform->getPos();
        //主角的向上方向--abc
        Cc3dMatrix4 t_m=m_transform->getRTmat();
        Cc3dVector4 up(t_m.getAt(4),t_m.getAt(5),t_m.getAt(6),0);
        //与mobilePlatformSet中各platform进行碰撞检测及碰撞反应--abc
        int n=(int)mobilePlatformSet.getActorCount();
        for(int i=0;i<n;i++){
            CmobilePlatform*pmobilePlatform=(CmobilePlatform*)mobilePlatformSet.getActorByIndex(i);
            if(pmobilePlatform->getIsDied())continue;//跳过死亡的运动平台--abc
            //求pmobilePlatform的e_backList和dList
            Cc3dVector4 e_back;
            float d=collisionTestWithWall_singlePoint_common(pmobilePlatform->getCollisionTriList(), pos, Rc, Rc_small_value_normal,up, e_back,intended_nSP);
            if(d>0){//碰撞--abc
                collision=true;
                if(triggerCollisionPlatforms){
                    //将platform触发--abc
                    pmobilePlatform->doTrigger_mov_byTouchWall();
                }
                if(triggerCollisionPlatforms){
                    //将platform触发--abc
                    pmobilePlatform->doTrigger_rot_byTouchWall();
                }
                //----恢复和回退--abc
                //恢复--abc
                //恢复平移分量，不恢复旋转分量，因为允许旋转--abc
                Cc3dMatrix4 RTmat=m_transform->getRTmat();
                RTmat.setAt(12, this->RTmat_store.getAt(12));
                RTmat.setAt(13, this->RTmat_store.getAt(13));
                RTmat.setAt(14, this->RTmat_store.getAt(14));
                m_transform->setRTmat(RTmat);
                //求回退向量--abc
                Cc3dVector4 back;
                float d_back=d*0.35;//0.25;//0.1;//除了恢复原位外另加一个回退--abc
                back=e_back*d_back;
                back.sety(0);
                //将回退向量加上平台移动速度向量的k倍(k属于(0,1])
                Cc3dVector4 movVecMulk;
                movVecMulk=pmobilePlatform->movVec*0.5;
                back=back+movVecMulk;
                //回退--abc
                m_transform->moveRelativeToFather(back.x(),back.y(),back.z());
                break;
            }
            
        }
        return collision;
    }
private:
    void downToSurface_soft(float H0,float surfaceH,const Cc3dVector4&surfaceNorm,float k_H,float k_posture,bool adjustPosture=true)
    //软着陆(k越小越软，k最大为1，为1时即硬着陆)
    //surfaceH和surfaceNorm分别为表面的高度和法向量--abc
    //H0为中心相对于表面悬起高度--abc
    {
        //调整物高度--abc
        Cc3dVector4 pos=m_transform->getPos();
        float dH=(surfaceH+H0-pos.y())*k_H;//软移动量--abc
        this->m_transform->moveY(dH);
        //调整模型姿态--abc
        if(adjustPosture)this->setPosture_soft(surfaceNorm,k_posture);
    }
   
    
    
};

#endif /* defined(__HelloOpenGL__cell__) */
