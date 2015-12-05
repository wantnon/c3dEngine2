//
//  ship.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-2-11.
//
//

#ifndef __HelloOpenGL__ship__
#define __HelloOpenGL__ship__

#include <iostream>
using namespace std;
#include "environmentInfo.h"
#include "cell.h"
#include "c3d.h"
const float A_randmRotAroundX=5;
const float cos_A_randmRotAroundX=cosf(A_randmRotAroundX*c3d_PI/180);
const float sin_A_randmRotAroundX=sinf(A_randmRotAroundX*c3d_PI/180);
const float A_randmRotAroundZ=5;
const float cos_A_randmRotAroundZ=cosf(A_randmRotAroundZ*c3d_PI/180);
const float sin_A_randmRotAroundZ=sinf(A_randmRotAroundZ*c3d_PI/180);
const float A_randmRotAroundY=2.5;
const float cos_A_randmRotAroundY=cosf(A_randmRotAroundY*c3d_PI/180);
const float sin_A_randmRotAroundY=sinf(A_randmRotAroundY*c3d_PI/180);
class Cship:public Ccell{
public:
    bool isMoving;//是否在移动--abc
    Ccur_foe<bool> isCollisionWithArchWall_curFoe;//本刻和上一刻是与arch墙壁撞击--abc
    Ccur_foe<bool> isCollisionWithMobilePlatformWall_curFoe;//本刻和上一刻是否与运动平台墙壁撞击--abc
    Ccur_foe<bool> isCollisionWithArchCeil_curFoe;//本刻和上一刻是与arch天花板撞击--abc
    Ccur_foe<bool> isCollisionWithMobilePlatformCeil_curFoe;//本刻和上一刻是否与运动平台天花板撞击--abc
    string fileName_engineSound;//引擎音效文件名--abc
    string fileName_collisionSound;//碰撞声音--abc
    CtimeCounter revTimeCounter_headingToTargetAgain;//再次向目标前进倒计时--abc
    long afterHowLongHeadingToTargetAgain;//多长时间后再次向目标前进--abc
    bool initiativeAttack;
public:
    int templateID;//模板名--abc
    float vy_push;//竖直速度（由自身动力提供的升速)
    float vforward_push;//推动前进速度（由自身动力提供的前进速度)
    Cc3dVector4 v_byOhterFoce;//漂行速度（由外界力提供的漂行速度)
    float v_pushForward_value;//推动前进速度值--abc
    float v_pushBack_value;//推动后退速度值--abc
    float v_pushUp_value;//推动上升速度值--abc
    float v_pushDown_value;//推动下降速度值--abc
    float a_vyPushDec;//推动竖直速度衰减--abc
    float a_vPushForwardDec;//推动前进速度衰减--abc
    float Rc_getOnShip;//上船需要的碰撞半径--abc
    Cc3dVector4 seatPos_local;//座位位置（局部坐标)，主角上船后中心处于此位置--abc
    Cc3dVector4 up_disturbed;//扰动过的up向量--abc
    bool isFlyable;//是否可飞行--abc
    Cc3dMatrix4 RTmat_store_beforeFloating;//在漂行之前保存的RTmat
    float modelScale;//模型缩放比例--abc
    //   Cc3dVector4 modelOffsetVec;//模型偏移向量--abc
    Cship(){
        initiativeAttack=false;
        afterHowLongHeadingToTargetAgain=70;
        isMoving=false;
        RTmat_store_beforeFloating=unitMat();

        isFlyable=false;
        v_pushUp_value=2.7;
        v_pushDown_value=2.7;
        up_disturbed.init(0,1,0,0);
        v_byOhterFoce.init(0,0,0,0);
        templateID=-1;
        seatPos_local.init(0,0,0,1);
        v_pushForward_value=4.5;
        v_pushBack_value=4;
        vy_push=0;
        vforward_push=0;
        a_vyPushDec=0.07;
        a_vPushForwardDec=0.08;
        v_rot=3;
        cos_v_rot=cosf(c3d_PI/180*v_rot);
        sin_v_rot=sinf(c3d_PI/180*v_rot);

    }
    void set_isMoving(bool _isMoving){
        isMoving=_isMoving;
    }
    bool get_isMoving()const{
        return isMoving;
    }
    
    bool get_isFlyable()const {
        return isFlyable;
    
    }
    void set_v_rot(float _v_rot){
        v_rot=_v_rot;
        cos_v_rot=cosf(c3d_PI/180*v_rot);
        sin_v_rot=sinf(c3d_PI/180*v_rot);
    
    }
    void set_up_disturbed(const Cc3dVector4&_up_disturbed) {
        up_disturbed=_up_disturbed;
    
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
            //modelScale
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&this->modelScale);
            //seatPos_local
            float t_seatPos_local[4]={0,0,0,1};
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&t_seatPos_local[0]);
            fscanf(fp, "%f",&t_seatPos_local[1]);
            fscanf(fp, "%f",&t_seatPos_local[2]);
            seatPos_local.init(t_seatPos_local);
            //Rc
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&Rc);
            //Rc_getOnShip
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&Rc_getOnShip);
            //Rc_small
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&Rc_small);
            //H_sinkInWater
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&H_sinkInWater);
            //flyable
            int isFlyable;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%d",&isFlyable);
            this->isFlyable=isFlyable;
            //engineSound
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%s",buffer);
            this->fileName_engineSound=buffer;
            //collisionSound
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%s",buffer);
            this->fileName_collisionSound=buffer;
            
        }
        //关闭文件--abc
        fclose(fp);
    }
    void randomRot(){//随机转弯--abc
        
        this->m_transform->rotate(0, 1, 0, cos_A_randmRotAroundY, sin_A_randmRotAroundY);
        if(rand()%3==0){
            this->m_transform->rotate(1, 0, 0, cos_A_randmRotAroundX, sin_A_randmRotAroundX);
        }
        if(rand()%3==0){
            this->m_transform->rotate(0, 0, 1, cos_A_randmRotAroundZ, sin_A_randmRotAroundZ);
        }

    }
    void autoMov(const Cc3dVector4&targetPos){//targetPos为this的攻击目标z位置--abc
        if(initiativeAttack&&revTimeCounter_headingToTargetAgain.getCount()==0){//主动攻击为true且倒计时结束才重新朝向目标--abc
            //求与由this指向主角的单位向量--abc
            Cc3dVector4 posThis=getTransform()->getPos();
            Cc3dVector4 thisToTarget_e=targetPos-posThis;
            thisToTarget_e=normalize(thisToTarget_e);//得到thisToTarget_e
            //将this的dir软调整为thisToTarget_e
            this->setDir_soft(thisToTarget_e,0.15);
        }else{//倒计时没结束，随机朝向--abc
            //右转--abc
            if(rand()%50==0){
                this->rotRight();
            }
            //左转--abc
            if(rand()%30==0){
                this->rotLeft();
            }
            //上升--abc
            if(rand()%200==0){
                this->pushUp();
            }
            //下降--abc
            if(rand()%200==0){
                this->pushDown();
            }

            
        }
        //前进--abc
        this->pushForward();


    
        
    }

    void set_Rc_getOnShip(float _Rc_getOnShip){
        Rc_getOnShip=_Rc_getOnShip;
    
    }
    void pushUp(){
        vy_push= v_pushUp_value;
        
    }
    void pushDown(){
        vy_push=-v_pushDown_value;
    }
    void pushForward(){
        vforward_push=v_pushForward_value;
    }
    void adjustPosToCeil(float softness=0.2){
        const float margin=0;
        bool collisionWithCeil=isTouchCeil(margin);
        //更新isCollisionWithArchCeil_curFoe
        isCollisionWithArchCeil_curFoe.set(environmentInfo.ceilType==CEILTYPE_ARCH&&collisionWithCeil);
        //更新isCollisionWithMobilePlatformCeil_curFoe
        isCollisionWithMobilePlatformCeil_curFoe.set(environmentInfo.ceilType==CEILTYPE_MOBILEPLATFORM&&collisionWithCeil);
        //碰撞反应--abc
        if(collisionWithCeil){
            //竖直速度衰减--abc
            vy_push/=2;
            //计算wantedHeight
            const float wantedHeight=environmentInfo.ceilH-Rc_small-margin;
            //如果高度高于wantedHeight，则调整为wantedHeight,如果低于，则不管，这样可以避免粘黏--abc
            if(m_transform->getRTmat().getAt(13)>wantedHeight){
                Cc3dMatrix4 RTmat=m_transform->getRTmat();
                RTmat.setAt(13, wantedHeight);
                m_transform->setRTmat(RTmat);
            }
            //声音--abc
            if((isCollisionWithArchCeil_curFoe.isChanged()&&isCollisionWithArchCeil_curFoe.get()==true)//如果isCollisionWithArchCeil由false变为true
               ||(isCollisionWithMobilePlatformCeil_curFoe.isChanged()&&isCollisionWithMobilePlatformCeil_curFoe.get()==true)//或者isCollisionWithMobilePlatformCeil由false变为true
               ){//播放声音--abc
            //    pALSource_ship_collision->updatePosition(getTransform().getPos());
            //    pALSource_ship_collision->playIfNotPlaying();
                if(this->getALSourceByName("ship_collision")){
                this->getALSourceByName("ship_collision")->updatePosition(getTransform()->getPos());
                this->getALSourceByName("ship_collision")->playIfNotPlaying();
                }
            }
        }
    }
    
    Cc3dMatrix4 getSeatRTmat()const {
        Cc3dMatrix4 seatRTmat=m_transform->getRTmat();
        //求seat的世界坐标--abc
        Cc3dVector4 seatPos_world=m_transform->convertToWorldSpace(seatPos_local);
        //用seatPos_world替换seatRTmat的平移分量--abc
        seatRTmat.setAt(12, seatPos_world.x());
        seatRTmat.setAt(13, seatPos_world.y());
        seatRTmat.setAt(14, seatPos_world.z());
        return seatRTmat;
    }
    bool collisionAndReActWithArchWall_flyBoss(CarchScene&archScene){
        float d;
        Cc3dVector4 e_back;
        vector<int> skipTagList;
        skipTagList.push_back(tag_collisionSubMesh);
        d=collisionTestWithArchWall(archScene, false, skipTagList, 3, e_back);
        const float *posf=this->RTmat_store.getArray()+12;
        collisionReAct_simple(d,e_back,posf);
        bool isCollision=(d>0);
        if(isCollision){
            vforward_push/=2;
            vy_push/=2;
        }
        return isCollision;
    }


    bool collisionAndReActWithArchWall(CarchScene&archScene,bool multiPoint,const vector<int>&skipTagList,const int intended_nSP){
        float d;
        Cc3dVector4 e_back;
        d=collisionTestWithArchWall(archScene, multiPoint, skipTagList, intended_nSP, e_back);
        const float *posf=this->RTmat_store.getArray()+12;
        collisionReAct_simple(d,e_back,posf);
        bool isCollision=(d>0);
        if(isCollision){
            vforward_push/=2;
            vy_push/=2;
        }
        //更新isCollisionWithArch_curFoe
        isCollisionWithArchWall_curFoe.set(isCollision);
        //碰撞扰动--abc
        if(isCollisionWithArchWall_curFoe.isChanged()&&isCollisionWithArchWall_curFoe.get()==true){//如果isCollisionWithArch_curFoe发生变化且是由false变为true，则播放碰撞声音--abc
            if(isFlyable){//只对会飞行的ship调整朝向，船只不调--abc
                //调整朝向--abc
                Cc3dVector4 wantedDir=Cc3dVector4(0,0,0,0)-e_back;
                if(isFlyable==false)wantedDir.sety(0);//如果不能飞，则舍弃Y分量--abc
                setDir_soft(wantedDir,0.35);
            }
            //声音--abc
          //  pALSource_ship_collision->updatePosition(getTransform().getPos());
          //  pALSource_ship_collision->playIfNotPlaying();
            this->getALSourceByName("ship_collision")->updatePosition(getTransform()->getPos());
            this->getALSourceByName("ship_collision")->playIfNotPlaying();
        }
        return isCollision;
    }
    bool prisonInCube(const Cc3dRange&range)
    //将this囚禁在以c为中心R为边心距的cube内--abc
    //前端返回是否出界--abc
    {
        bool isOutRange=false;//是否出界--abc
        //求cube范围--abc
        float xmin,xmax,ymin,ymax,zmin,zmax;
        xmin=range.getMinX();
        xmax=range.getMaxX();
        ymin=range.getMinY();
        ymax=range.getMaxY();
        zmin=range.getMinZ();
        zmax=range.getMaxZ();
        //如果this超出cube则强制拉回cube中--abc
        Cc3dVector4 pos=this->getTransform()->getPos();
        float x=pos.x();
        float y=pos.y();
        float z=pos.z();
        if(x<xmin){x=xmin;isOutRange=true;}
        if(x>xmax){x=xmax;isOutRange=true;}
        if(y<ymin){y=ymin;isOutRange=true;}
        if(y>ymax){y=ymax;isOutRange=true;}
        if(z<zmin){z=zmin;isOutRange=true;}
        if(z>zmax){z=zmax;isOutRange=true;}
        if(isOutRange){//如果出界，则重定位--abc
            this->m_transform->setPos(x, y, z);
        }
        return isOutRange;
    }
    
    bool collisionAndReActWithmobilePlatformWall(CmobilePlatformSet&mobilePlatformSet,bool triggerCollisionPlatforms,bool multiPoint,const int intended_nSP){
        bool collision;
        if(multiPoint){
            collision=Ccell::collisionAndReActWithmobilePlatformWall(mobilePlatformSet,triggerCollisionPlatforms,multiPoint,intended_nSP);
        }else{
            collision=Ccell::collisionAndReActWithmobilePlatformWall(mobilePlatformSet,triggerCollisionPlatforms,multiPoint,intended_nSP);
        }
        if(collision){
            vforward_push/=2;
            vy_push/=2;
        }
        //更新isCollisionWithMobilePlatform_curFoe
        isCollisionWithMobilePlatformWall_curFoe.set(collision);
        //声音--abc
        if(isCollisionWithMobilePlatformWall_curFoe.isChanged()&&isCollisionWithMobilePlatformWall_curFoe.get()==true){//如果isCollisionWithMobilePlatform_curFoe发生变化且是由false变为true，则播放碰撞声音--abc
        //    pALSource_ship_collision->updatePosition(getTransform().getPos());
        //    pALSource_ship_collision->playIfNotPlaying();
            if(this->getALSourceByName("ship_collision")){
            this->getALSourceByName("ship_collision")->updatePosition(getTransform()->getPos());
            this->getALSourceByName("ship_collision")->playIfNotPlaying();
            }
        }

        return collision;
    }
    float floatingInAir()//升起和下降--abc
    //前端返回本次由自身动力引起的移动路程--abc
 	{
        float s=0;
        //--------速度衰减--abc
        //--动力速度衰减--abc
        if(vy_push>0){
            vy_push-=a_vyPushDec;
            if(vy_push<0)vy_push=0;
        }
        if(vy_push<0){
            vy_push+=a_vyPushDec;
            if(vy_push>0)vy_push=0;
        }
        if(vforward_push>0){
            vforward_push-=a_vPushForwardDec;
            if(vforward_push<0)vforward_push=0;
        }
        if(vforward_push<0){
            vforward_push+=a_vPushForwardDec;
            if(vforward_push>0)vforward_push=0;
        }
        //--其它外力速度衰减 (用指数衰减即可)
        v_byOhterFoce=v_byOhterFoce*0.95;
        //--------按速度移动--abc
        //移动前暂存高度，以便恢复--abc
        //--其它外力引起位移(不计入s中)
        this->m_transform->moveRelativeToFather(v_byOhterFoce.x(), v_byOhterFoce.y(), v_byOhterFoce.z());
        //--前进--abc
        this->m_transform->moveZ(-vforward_push);//注意，这句不只是水平移动，也可能包含了竖直方向上的移动--abc
        s+=fabsf(vforward_push);
        //升降--abc
        //不管是否会飞，都要响应升起降（不会飞时是响应降，以便能紧贴表面)
        //但是如果不会飞，则升降不计入移动距离s
        if(vy_push<0){//如果速度向下--abc
            //向下移动--abc
            this->m_transform->moveY(vy_push);
            if(isFlyable)s+=fabsf(vy_push);
            
        }
        if(vy_push>0){//如果速度向上--abc
            //向上移动--abc
            this->m_transform->moveY(vy_push);
            if(isFlyable)s+=fabsf(vy_push);
        }
        return s;
    }
    void adjustPosAndPostureToSurface()
    //triggerPlatform指示是否触发移动平台--abc
    {
        //----着陆--abc
        const float margin=1.0;
        if(isTouchSurface(margin)){//接触表面--abc
            //竖直速度衰减--abc
            vy_push/=2;
            //调整高度到surface
            //计算wantedHeight
            float wantedHeight;
            if(environmentInfo.surfaceType==SURFACETYPE_WATER){
                //如果是水面，要将H_sinkInWater考虑进去--abc
                 wantedHeight=environmentInfo.surfaceH+Rc_small+margin-H_sinkInWater;
            }else{
                wantedHeight=environmentInfo.surfaceH+Rc_small+margin;
            }
            //如果低于wantedHeight，则调整为wantedHeight
            if(m_transform->getRTmat().getAt(13)<wantedHeight){//如果小于就拉上来，否则不影响，这样能避免粘黏--abc
                Cc3dMatrix4 RTmat=m_transform->getRTmat();
                RTmat.setAt(13, wantedHeight);
                m_transform->setRTmat(RTmat);
            }
            //调整姿态--abc
            setPosture_soft(environmentInfo.surfaceNorm,0.2);
        }
    }
    virtual~Cship(){
   
    }
    
};
class CshipSet:public CactorSet{
public:
    void calRange(float &xmin,float &xmax,float &ymin,float &ymax,float &zmin,float &zmax){
        int n=(int)actorList.size();
        if(n==0){
            xmin=0;
            xmax=0;
            ymin=0;
            ymax=0;
            zmin=0;
            zmax=0;
            return;
        }
        xmin=c3d_INF;
		xmax=-c3d_INF;
		ymin=c3d_INF;
		ymax=-c3d_INF;
		zmin=c3d_INF;
		zmax=-c3d_INF;
        
        for(int i=0;i<n;i++){
            Cship*pship= (Cship*)actorList[i];
            Cc3dVector4 v=pship->getTransform()->getPos();
            if(v.x()<xmin)xmin=v.x();
            if(v.x()>xmax)xmax=v.x();
            if(v.y()<ymin)ymin=v.y();
            if(v.y()>ymax)ymax=v.y();
            if(v.z()<zmin)zmin=v.z();
            if(v.z()>zmax)zmax=v.z();
        }
        //加个margin
        const float margin=10;
        xmin-=margin;
        xmax+=margin;
        ymin-=margin;
        ymax+=margin;
        zmin-=margin;
        zmax+=margin;
        
        
    }

  /*  void show(const Ccamera&camera,const Clight&light,const Cprogram&program){
        int n=(int)pModelBaseList.size();
        for(int i=0;i<n;i++){
            Cship*pship=(Cship*) pModelBaseList[i];
            if(pship->visible){
                  pship->getModel()->getMeshList()[0]->show_pmeshList(camera, light, program,&pship->transf);
            }
        }
    
    }*/
    void loadConfig(const string&fileNameWithExt);
};
#endif /* defined(__HelloOpenGL__ship__) */
