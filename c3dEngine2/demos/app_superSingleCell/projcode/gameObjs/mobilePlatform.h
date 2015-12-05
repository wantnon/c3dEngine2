//
//  mobilePlatform.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-1-20.
//
//

#ifndef __HelloOpenGL__mobilePlatform__
#define __HelloOpenGL__mobilePlatform__

#include <iostream>
#include "collisionWithModelAtTriLevel.h"
#include "sprite.h"
#include "life.h"
#include "c3d.h"


class CIDTriangleEx
{
protected:
    Cc3dIDTriangle m_IDtri;
    Cc3dVector4 m_norm;
    int m_meshID;//指明此三角形属于哪个submesh
public:
    CIDTriangleEx(){
        m_meshID=-1;
    }
    void setNorm(const Cc3dVector4&norm){m_norm=norm;}
    const Cc3dVector4&getNorm()const{return m_norm;}
    int getSubMeshID()const{return m_meshID;}
    void setSubMeshID(int meshID){m_meshID=meshID;}
    const Cc3dIDTriangle&getIDtri()const{return m_IDtri;}
    void setIDtri(const Cc3dIDTriangle&IDtri){
        this->m_IDtri.vID[0]=IDtri.vID[0];
        this->m_IDtri.vID[1]=IDtri.vID[1];
        this->m_IDtri.vID[2]=IDtri.vID[2];
    }
    
};

//运动类型--abc
#define movType_STILL 0 //静止--abc
#define movType_MOV 1 //平移--abc
#define movType_ROT 2 //旋转--abc
#define movType_MOV_AND_ROT 3 //旋转与平移混合--abc
//触发类型--abc
#define triggerType_NONEEDTOTRIGGER 0
#define triggerType_TOUCHWALL 1
#define triggerType_TOUCHCEIL 2
#define triggerType_TOUCHSURFACE 3
#define triggerType_TOUCHANYPART 4
class CmobilePlatform:public Cc3dActor
{
protected:
    bool m_isDied;//是否已死亡--abc
public:
    string fileName_motionSoundAfterTrigger;//可能为"null"
    bool motionSoundAfterTrigger_looping;//是否循环--abc
public:
    vector<CIDTriangleEx*> pIDtriExList;//pmeshList中所有三角面统一索引到这个表里--abc
public://为了优化而增加的结构--abc
    int movType;//运动类型--abc
    float bounceVup;//自动弹跳速度，如果为0则不弹跳--abc
    bool moved;//是否发生了运动--abc
public:
    Cc3dMatrix4 RTmat_store;
    vector<vector<Cc3dVertex> > vlistStoreList;
public:
    int ID;
    Cc3dMatrix4 initRMat;//初始旋转矩阵--abc
    Cc3dVector4 startPos,endPos;//起始点--abc
    float movSpeed;//移动速度--abc
    Cc3dVector4 rot_n;//单位旋转轴--abc
    float dA,cos_dA,sin_dA;//旋转角速度及其余弦和正弦--abc
    string geoTypeName;//几何类型名--abc
    float rx,rz,ry;//半长，半宽，半高--abc
    string texImageName;//纹理图片名--abc
    int triggerType_mov;//平移的触发类型--abc
    int triggerType_rot;//旋转的触发类型--abc
    bool triggered_mov;//平移是否已触发--abc
    bool triggered_rot;//旋转是否已触发--abc
    float rotAnglePerTrigger;//一次触发旋转的角度--abc
    float angleRotedCurTrigger;//本次触发以来已旋过的角度--abc
    Cc3dVector4 movVec;//实际移动向量--abc
    bool isTrigger_movLocked;//移动触发锁定，不再触发移动--abc
    bool isTrigger_rotLocked;//旋转触发锁定，不再触发旋转--abc
    bool applyTrigger_movLock;//使用移动触发锁定--abc
    bool applyTrigger_rotLock;//使用旋转触发锁定--abc
    float initMovPercentage;//初始移动百分比--abc
    float k_inertialVup;//竖直惯性速度系数--abc
    bool destroyable;//可破坏--abc
    Clife life;//生命值，当destroyable==true时有效--abc
 
    bool isShotTrigger;//是否通过射击触发--abc

public:
    float len2_startPos_endPos;//startPos与endPos之间距离的平方--abc
    bool isMovingForth;//是否在前进(向endPos方向移动)
    Cc3dVector4 speedvec_forth;//前往移动速度向量--abc
protected:
    Csprite*m_IDBoard0;//显示ID的公告牌，ID的第0位--abc
    Csprite*m_IDBoard1;//显示ID的公告牌，ID的第1位--abc
protected:
    float Rc;//碰撞半径--abc
public:
   
    virtual ~CmobilePlatform(){

    }
    CmobilePlatform(){
        motionSoundAfterTrigger_looping=false;
        isShotTrigger=true;
        m_isDied=false;
        life.set_maxValue(50);
        life.chargeFull();
        destroyable=true;
        k_inertialVup=0;
        initMovPercentage=0;
        applyTrigger_movLock=true;
        applyTrigger_rotLock=true;
        isTrigger_movLocked=false;
        isTrigger_rotLocked=false;
        moved=true;
        angleRotedCurTrigger=0;
        rotAnglePerTrigger=360;
        bounceVup=3;
        ID=0;
        movType=movType_STILL;
        RTmat_store=unitMat();
        startPos.init(0,0,0,1);
        endPos.init(0,0,0,1);
        len2_startPos_endPos=0;
        rot_n.init(0, 1, 0, 0);
        cos_dA=1;
        sin_dA=0;
        isMovingForth=true;
        movSpeed=0;
        speedvec_forth.init(0,0,0,0);
        movVec.init(0, 0, 0, 0);
        rx=ry=rz=0;
        triggerType_mov=triggerType_NONEEDTOTRIGGER;
        triggered_mov=false;
        triggerType_rot=triggerType_NONEEDTOTRIGGER;
        triggered_rot=false;
        m_IDBoard0=NULL;
        m_IDBoard1=NULL;
        Rc=0;
    }
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
    void updateNormsOfpIDtriExList(){//对pIDtriExList中和IDtriEx的法向量进行更新--abc
        int nIDtriEx=(int)pIDtriExList.size();
        for(int i=0;i<nIDtriEx;i++){
            CIDTriangleEx*pIDtriEx=pIDtriExList[i];
            //v0,v1,v2的法向量都相等，都可以用作pIDtriEx的法向量--abc
            int meshID=pIDtriEx->getSubMeshID();
            int vID=pIDtriEx->getIDtri().vID[0];
            const Cc3dVertex &v0=getModel()->getMeshByIndex(0)->getSubMeshByIndex(meshID)->getSubMeshData()->getVertexByIndex(vID);
            pIDtriEx->setNorm(v0.getNorm());
        }
        
    }

    
    void gen_pIDtriExList(){//生成pIDtriExList，要放在pmeshList全部形成后调用--abc
        if((int)pIDtriExList.size()!=0){
            cout<<"error: pIDtriExList非空!"<<endl;
            assert(false);
        }
        int nSubMesh=(int)getModel()->getMeshByIndex(0)->getSubMeshCount();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh&submesh=*getModel()->getMeshByIndex(0)->getSubMeshByIndex(i);
            int nIDtri=(int)submesh.getSubMeshData()->getIDtriCount();
            for(int j=0;j<nIDtri;j++){
                const Cc3dIDTriangle IDtri=submesh.getSubMeshData()->getIDtriByIndex(j);
                //由IDtri生成pIDtriEx
                CIDTriangleEx*pIDtriEx=new CIDTriangleEx();
                pIDtriEx->setIDtri(IDtri);
                pIDtriEx->setSubMeshID(i);
                //v0,v1,v2的法向量都相等，都可以用作pIDtriEx的法向量--abc
                const Cc3dVertex v0=submesh.getSubMeshData()->getVertexByIndex(IDtri.vID[0]);
                pIDtriEx->setNorm(v0.getNorm());
                //将pIDtriEx添加到pIDtriExList
                pIDtriExList.push_back(pIDtriEx);
                
            }
        }//得到pIDtriExList
    }
    vector<CtriangleWithNorm> getCollisionTriList(){
        //将pIDtriExList转成triangleWithNormList并返回--abc
        vector<CtriangleWithNorm> triWithNormList;
        {
            int nTri=(int)pIDtriExList.size();
            for(int i=0;i<nTri;i++){
                CIDTriangleEx*IDtriEx=pIDtriExList[i];
                const Cc3dVertex v0=getModel()->getMeshByIndex(0)->getSubMeshByIndex(IDtriEx->getSubMeshID())->getSubMeshData()->getVertexByIndex(IDtriEx->getIDtri().vID[0]);
                const Cc3dVertex v1=getModel()->getMeshByIndex(0)->getSubMeshByIndex(IDtriEx->getSubMeshID())->getSubMeshData()->getVertexByIndex(IDtriEx->getIDtri().vID[1]);
                const Cc3dVertex v2=getModel()->getMeshByIndex(0)->getSubMeshByIndex(IDtriEx->getSubMeshID())->getSubMeshData()->getVertexByIndex(IDtriEx->getIDtri().vID[2]);
                const Cc3dVector4 pos0=v0.getPos();
                const Cc3dVector4 pos1=v1.getPos();
                const Cc3dVector4 pos2=v2.getPos();
                const Cc3dVector4 norm=IDtriEx->getNorm();
                CtriangleWithNorm triWithNorm(pos0,pos1,pos2,norm);
                triWithNormList.push_back(triWithNorm);
            }
        }
        return triWithNormList;
        
    }
    Csprite*getIDBoard0()const{return m_IDBoard0;}
    Csprite*getIDBoard1()const{return m_IDBoard1;}
    void setIDBoard0(Csprite*IDBoard0){
        assert(IDBoard0);
        if(m_IDBoard0==NULL){
            m_IDBoard0=IDBoard0;
            addChild(m_IDBoard0);

        }else{
            m_IDBoard0->removeFromParent();
            m_IDBoard0=IDBoard0;
            addChild(m_IDBoard0);
        }
    }
    void setIDBoard1(Csprite*IDBoard1){
        assert(IDBoard1);
        if(m_IDBoard1==NULL){
            m_IDBoard1=IDBoard1;
            addChild(m_IDBoard1);
        }else{
            m_IDBoard1->removeFromParent();
            m_IDBoard1=IDBoard1;
            addChild(m_IDBoard1);
        }
    }
    
    
    bool get_destroyable()const {
        return destroyable;
    }
    bool get_isBouncing(){//是否自动弹跳--abc
        return (bounceVup!=0);
    }
    void setIsDied(bool isDied){
        m_isDied=isDied;
    }
    bool getIsDied()const {
        return m_isDied;
    }
    
    void scaleToFitSurroundingBoxMatchingRxRyRz(){
        //求模型的范围--abc
        float xmin=c3d_INF;
        float xmax=-c3d_INF;
        float ymin=c3d_INF;
        float ymax=-c3d_INF;
        float zmin=c3d_INF;
        float zmax=-c3d_INF;
        int nSubMesh=(int)getModel()->getMeshByIndex(0)->getSubMeshCount();
        for(int i=0;i<nSubMesh;i++){
            int nv=(int)getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getSubMeshData()->getVertexCount();
            for(int j=0;j<nv;j++){
                const Cc3dVertex v=getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getSubMeshData()->getVertexByIndex(j);
                Cc3dVector4 pos=v.getPos();
                if(pos.x()<xmin)xmin=pos.x();
                if(pos.x()>xmax)xmax=pos.x();
                if(pos.y()<ymin)ymin=pos.y();
                if(pos.y()>ymax)ymax=pos.y();
                if(pos.z()<zmin)zmin=pos.z();
                if(pos.z()>zmax)zmax=pos.z();
            }
        }//得到xmin,xmax,ymin,ymax,zmin,zmax
        //求模型的_rx,_ry,_rz
        float _rx=max(fabsf(xmin),fabsf(xmax));
        float _ry=max(fabsf(ymin),fabsf(ymax));
        float _rz=max(fabsf(zmin),fabsf(zmax));
        //求由_rx,_ry,_rz放缩到rx,ry,rz的放缩比--abc
        float kx=_rx==0?1:rx/_rx;
        float ky=_ry==0?1:ry/_ry;
        float kz=_rz==0?1:rz/_rz;
        //对this进行放缩（以原点为中心)
        for(int i=0;i<nSubMesh;i++){
            int nv=(int)getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getSubMeshData()->getVertexCount();
            for(int j=0;j<nv;j++){
                Cc3dVertex v=getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getSubMeshData()->getVertexByIndex(j);
                Cc3dVector4 pos=v.getPos();
                pos.setx(pos.x()*kx);
                pos.sety(pos.y()*ky);
                pos.setz(pos.z()*kz);
                v.setPos(pos);
                getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getSubMeshData()->setVertexByIndex(j,v);
            }
        }
        
    }

    void calMovType(){
        //根据startPos,endPos,movSpeed,dA来计算movType
        movType=movType_STILL;
        if(isEqual(startPos, endPos,0.01)//如果起止点重合--abc
           ||movSpeed==0//或平移速度为0
           ){//没有平移--abc
            //看是静止还是旋转--abc
            if(dA==0){//静止--abc
                movType=movType_STILL;
            }else{//旋转--abc
                movType=movType_ROT;
            }
        }else{//有平移--abc
            //看是平移还是平移和旋转都有--abc
            if(dA==0){//只平移--abc
                movType=movType_MOV;
            }else{//平移和旋转--abc
                movType=movType_MOV_AND_ROT;
            }
        }//得到movType
        
    }
   
    void set_startPosAndEndPosAndCalLen2_start_end(const Cc3dVector4&_startPos,const Cc3dVector4&_endPos){//设置startPos和endPos，并计算len2_startPos_endPos
        startPos=_startPos;
        endPos=_endPos;
        len2_startPos_endPos=getLength2(startPos-endPos);
    
    }
    void set_movSpeed(float _movSpeed){
        movSpeed=_movSpeed;
    }
    void calSpeedvec_forth_baseOnStartPosEndPosAndMovSpeed(){//根据startPos,endPos,movSpeed计算speedvec_forth
        speedvec_forth=endPos-startPos;
        Cc3dVector4 zerovec(0,0,0,0);
        if(isEqual(speedvec_forth, zerovec)){
            return;
        }else{
            speedvec_forth=normalize(speedvec_forth);
            speedvec_forth=speedvec_forth*movSpeed;
            
        }
    
    }
    void mov(){//transf中仍然保存位姿，但是在顶点却不是transf空间从标，而是世界空间坐标--abc
        if(m_isDied)return;
        if(movType==movType_STILL){
            return;
        }

        //----平移--abc
        if(triggerType_mov!=triggerType_NONEEDTOTRIGGER&&triggered_mov==false){//需要触发却又还没触发--abc
            //不平移--abc
        }else{
            if(movType==movType_MOV||movType==movType_MOV_AND_ROT){
                //平移--abc
                moved=true;
                //获得当前位置--abc
                Cc3dVector4 pos=m_transform->getPos();
                //判断是否超已到达或超过边界--abc
                if(isMovingForth){//如果在前进，则看是到startPos的距离平方是否大于len2_startPos_endPos
                    movVec=speedvec_forth;
                    float len2_startPos_pos=getLength2(startPos-pos);
                    if(len2_startPos_pos>=len2_startPos_endPos){//到达边界或越界--abc
                        isMovingForth=false;//变向--abc
                        movVec=Cc3dVector4(0, 0, 0, 0)-movVec;
                    }else{//未到达边界，可以前进--abc
                        m_transform->moveRelativeToFather(movVec.x(), movVec.y(), movVec.z());
                    }
                }else{//如果在返回，则看到endPos的距离平方是否大于len2_startPos_endPos
                    movVec=Cc3dVector4(0, 0, 0, 0)-speedvec_forth;
                    float len2_endPos_pos=getLength2(endPos-pos);
                    if(len2_endPos_pos>=len2_startPos_endPos){//到达边界或越界--abc
                        isMovingForth=true;//变向--abc
                        movVec=Cc3dVector4(0, 0, 0, 0)-speedvec_forth;//movVec=speedvec_forth
                        if(triggerType_mov!=triggerType_NONEEDTOTRIGGER){//如果需要触发，则此时已满一个周期，停止运动，等待下一次触发--abc
                            triggered_mov=false;
                            movVec.init(0,0,0,0);
                            if(applyTrigger_movLock)isTrigger_movLocked=true;
                        }
                    }else{//未到达边界，可以继续返回--abc
                        m_transform->moveRelativeToFather(movVec.x(), movVec.y(), movVec.z());
                    }
                }
            }
        
        }
        //----旋转--abc
        if(triggerType_rot!=triggerType_NONEEDTOTRIGGER&&triggered_rot==false){//需要触发却又还没触发--abc
            //不旋转--abc
        }else{
            if(movType==movType_ROT||movType==movType_MOV_AND_ROT){
                //旋转--abc
                moved=true;
                if(triggered_rot!=triggerType_NONEEDTOTRIGGER
                   &&angleRotedCurTrigger>=rotAnglePerTrigger){//如果需要触发且已转满规定的角度，停止--abc
                    //将转过头的角度反向回转--abc
                    {
                        float A_overRoted=angleRotedCurTrigger-rotAnglePerTrigger;
                        float A_overRoted_radian=A_overRoted*c3d_PI/180;
                        float cosA_overRoted=cosf(-A_overRoted_radian);
                        float sinA_overRoted=sinf(-A_overRoted_radian);
                        m_transform->rotate(rot_n.x(), rot_n.y(), rot_n.z(), cosA_overRoted, sinA_overRoted);
                    }
                    triggered_rot=false;
                    angleRotedCurTrigger=0;
                    if(applyTrigger_rotLock)isTrigger_rotLocked=true;
                }else{//否则继续旋转--abc
                    m_transform->rotate(rot_n.x(), rot_n.y(), rot_n.z(), cos_dA, sin_dA);
                    angleRotedCurTrigger+=fabsf(dA);//注意要用绝对值，因为dA可能为负--abc
                }
            }
        }
        //声音停止--abc
     //   cout<<"triggered_mov:"<<triggered_mov<<endl;
        if(triggered_rot==false&&triggered_mov==false){
         //   pALSource_plat_motion->stop();
                this->getALSourceByName("plat_motion")->stop();
        }
    }
    void calRc_baseOn_RxRyRz(){//根据rx,ry,rz计算Rc
        float r=sqrt(rx*rx+ry*ry+rz*rz);
        Rc=r;
    
    }
    void set_movType(int _movType){
        movType=_movType;
    }
    void storeRTmat(){
        RTmat_store=m_transform->getRTmat();
    }
    void updateAndSubmitVertex(bool updateAnyWay=false)
    //更新顶点--abc
    //如果updateAnyWay==true，则无条件更新--abc
    {
        if(m_isDied)return;
   
        if(updateAnyWay==false&&(movType==movType_STILL||moved==false))return;
     //   cout<<"updateVertex"<<endl;
        //----对坐标和法线进行变换--abc
        int nSubMesh=(int)getModel()->getMeshByIndex(0)->getSubMeshCount();
        for(int i=0;i<nSubMesh;i++){
            int nv=(int)getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getSubMeshData()->getVertexCount();
            for(int j=0;j<nv;j++){
                Cc3dVertex v=getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getSubMeshData()->getVertexByIndex(j);
                Cc3dVector4 pos=v.getPos();
                Cc3dVector4 norm=v.getNorm();
             //   Cc3dVertex&v_store=getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->vlist_store[j];
                Cc3dVertex&v_store=vlistStoreList[i][j];
                Cc3dVector4 pos_store=v_store.getPos();
                Cc3dVector4 norm_store=v_store.getNorm();
                //对v进行变换--abc
                if(movType==movType_MOV){
                    //坐标--abc
                    pos.setx(pos_store.x()+m_transform->getRTmat().getArray()[12]);
                    pos.sety(pos_store.y()+m_transform->getRTmat().getArray()[13]);
                    pos.setz(pos_store.z()+m_transform->getRTmat().getArray()[14]);
                    v.setPos(pos);
                    
                }else{
                    //坐标--abc
                    Cc3dVector4 t_pos=m_transform->getRTmat()*pos_store;
                    pos.setx(t_pos.x());
                    pos.sety(t_pos.y());
                    pos.setz(t_pos.z());
                    v.setPos(pos);
                    //法线--abc
                    Cc3dVector4 t_norm=m_transform->getRTmat()*norm_store;
                    norm.setx(t_norm.x());
                    norm.sety(t_norm.y());
                    norm.setz(t_norm.z());
                    v.setNorm(norm);
                    
                    
                }
                getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getSubMeshData()->setVertexByIndex(j,v);
            }
        }
        //更新pIDtriExList的法向量--abc
        updateNormsOfpIDtriExList();
        //将moved置回false
        moved=false;
        //提交顶点--abc
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh&submesh=*(this->getModel()->getMeshByIndex(0)->getSubMeshByIndex(i));
            submesh.submitVertex(GL_STREAM_DRAW);
        }
   
    }

    void doTrigger_mov_byTouchCeil(){
        if(triggerType_mov==triggerType_TOUCHCEIL
           ||triggerType_mov==triggerType_TOUCHANYPART)
        {
                if(isTrigger_movLocked==false)doTrigger_mov_ignoreTouchLocAndTriggerLock();
        }
        
    }
    void doTrigger_mov_byTouchSurface(){
        if(triggerType_mov==triggerType_TOUCHSURFACE
           ||triggerType_mov==triggerType_TOUCHANYPART)
        {
            if(isTrigger_movLocked==false)doTrigger_mov_ignoreTouchLocAndTriggerLock();
        }
    }
    void doTrigger_mov_byTouchWall(){
        if(triggerType_mov==triggerType_TOUCHWALL
           ||triggerType_mov==triggerType_TOUCHANYPART)
        {
            //对wall来说，不考虑triggerLock
            doTrigger_mov_ignoreTouchLocAndTriggerLock();
        }
    }
    void doTrigger_rot_byTouchCeil(){
        if(triggerType_rot==triggerType_TOUCHCEIL
           ||triggerType_rot==triggerType_TOUCHANYPART)
        {
            if(isTrigger_rotLocked==false)doTrigger_rot_ignoreTouchLocAndTriggerLock();
        }
    }
    void doTrigger_rot_byTouchSurface(){
        if(triggerType_rot==triggerType_TOUCHSURFACE
           ||triggerType_rot==triggerType_TOUCHANYPART)
        {
            if(isTrigger_rotLocked==false)doTrigger_rot_ignoreTouchLocAndTriggerLock();
        }
    }
    void doTrigger_rot_byTouchWall(){
        if(triggerType_rot==triggerType_TOUCHWALL
           ||triggerType_rot==triggerType_TOUCHANYPART)
        {
            //对wall来说不考虑triggerLock
            doTrigger_rot_ignoreTouchLocAndTriggerLock();
        }
    }

  
    void doTrigger_mov_ignoreTouchLocAndTriggerLock(){//触发mov，忽略触发位置和触发锁--abc
        if(triggerType_mov==triggerType_NONEEDTOTRIGGER)return;//如果不需触发，则返回--abc
        if(movType==movType_MOV||movType==movType_MOV_AND_ROT){
            bool triggered_mov_foe=triggered_mov;
            triggered_mov=true;
            //声音--abc
            if(triggered_mov_foe==false){//之前是未trigger状态现转为triggered才放声音--abc
              //  pALSource_plat_motion->updatePosition(getTransform().getPos());
              //  pALSource_plat_motion->play();
                this->getALSourceByName("plat_motion")->updatePosition(getTransform()->getPos());
                this->getALSourceByName("plat_motion")->play();
            }
        }
    }
    void doTrigger_rot_ignoreTouchLocAndTriggerLock(){//触发rot，忽略触发位置和触发锁--abc
        if(triggerType_rot==triggerType_NONEEDTOTRIGGER)return;//如果不需触发，则返回--abc
        if(movType==movType_ROT||movType==movType_MOV_AND_ROT){
            if(isTrigger_rotLocked==false){
                bool triggered_rot_foe=triggered_rot;
                triggered_rot=true;
                if(triggered_rot_foe==false){//之前是未trigger状态现转为triggered才放声音--abc
                 //   pALSource_plat_motion->updatePosition(getTransform().getPos());
                 //   pALSource_plat_motion->play();
                    this->getALSourceByName("plat_motion")->updatePosition(getTransform()->getPos());
                    this->getALSourceByName("plat_motion")->play();
                    
                }
            }
        }
    }

};
class CmobilePlatformSet:public CactorSet
{
public:
     

    void updateAndSubmitVertex(bool updateAnyWay=false){
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            CmobilePlatform*pmobilePlatform=(CmobilePlatform*)actorList[i];
            pmobilePlatform->updateAndSubmitVertex(updateAnyWay);
        }
    }
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
            CmobilePlatform*pmobilePlatform=(CmobilePlatform*)actorList[i];
            const Cc3dVector4&startPos=pmobilePlatform->startPos;
            const Cc3dVector4&endPos=pmobilePlatform->endPos;
            Cc3dVector4 v[2];
            v[0]=startPos;
            v[1]=endPos;
            for(int j=0;j<2;j++){
                if(v[j].x()<xmin)xmin=v[j].x();
                if(v[j].x()>xmax)xmax=v[j].x();
                if(v[j].y()<ymin)ymin=v[j].y();
                if(v[j].y()>ymax)ymax=v[j].y();
                if(v[j].z()<zmin)zmin=v[j].z();
                if(v[j].z()>zmax)zmax=v[j].z();
            }
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
            int ID_foe=-1;
            //提取项目数--abc
            int n;
            fscanf(fp, "%d",&n);
            //提取项目--abc
            for(int i=0;i<n;i++){
                CmobilePlatform*pmobilePlatform=new CmobilePlatform();
                pmobilePlatform->autorelease();
                pmobilePlatform->init();
                //----ID
                int ID;
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%d",&ID);
                pmobilePlatform->ID=ID;
                //检查ID是否恰好比ID_foe大1
                if(ID-ID_foe!=1){
                    cout<<"error:ID不连续!"<<endl;
                    assert(false);
                }else{
                    ID_foe=ID;//更新ID_foe
                }
                //----mov触发类型--abc
                {
                    fscanf(fp, "%s",buffer);
                    fscanf(fp, "%s",buffer);
                    string triggerTypeStr_mov=buffer;
                    if(triggerTypeStr_mov=="noNeedToTrigger"){
                        pmobilePlatform->triggerType_mov=triggerType_NONEEDTOTRIGGER;
                    }else if(triggerTypeStr_mov=="touchAnyPart"){
                        pmobilePlatform->triggerType_mov=triggerType_TOUCHANYPART;
                    }else if(triggerTypeStr_mov=="touchCeil"){
                        pmobilePlatform->triggerType_mov=triggerType_TOUCHCEIL;
                    }else if(triggerTypeStr_mov=="touchSurface"){
                        pmobilePlatform->triggerType_mov=triggerType_TOUCHSURFACE;
                    }else if(triggerTypeStr_mov=="touchWall"){
                        pmobilePlatform->triggerType_mov=triggerType_TOUCHWALL;
                    }else{
                        cout<<"error: 未知的触发类型!"<<endl;
                        assert(false);
                    }
             
                }
                //----rot触发类型--abc
                {
                    fscanf(fp, "%s",buffer);
                    fscanf(fp, "%s",buffer);
                    string triggerTypeStr_rot=buffer;
                    if(triggerTypeStr_rot=="noNeedToTrigger"){
                        pmobilePlatform->triggerType_rot=triggerType_NONEEDTOTRIGGER;
                    }else if(triggerTypeStr_rot=="touchAnyPart"){
                        pmobilePlatform->triggerType_rot=triggerType_TOUCHANYPART;
                    }else if(triggerTypeStr_rot=="touchCeil"){
                        pmobilePlatform->triggerType_rot=triggerType_TOUCHCEIL;
                    }else if(triggerTypeStr_rot=="touchSurface"){
                        pmobilePlatform->triggerType_rot=triggerType_TOUCHSURFACE;
                    }else if(triggerTypeStr_rot=="touchWall"){
                        pmobilePlatform->triggerType_rot=triggerType_TOUCHWALL;
                    }else{
                        cout<<"error: 未知的触发类型!"<<endl;
                        assert(false);
                    }
                }
                //----isShotTrigger
                int isShotTrigger;
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%d",&isShotTrigger);
                pmobilePlatform->isShotTrigger=isShotTrigger;
                //----applyTriggerLock_mov
                int applyTriggerLock_mov;
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%d",&applyTriggerLock_mov);
                pmobilePlatform->applyTrigger_movLock=applyTriggerLock_mov;
                //----applyTriggerLock_rot
                int applyTriggerLock_rot;
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%d",&applyTriggerLock_rot);
                pmobilePlatform->applyTrigger_rotLock=applyTriggerLock_rot;
                //----motionSoundAfterTrigger
                fscanf(fp,"%s",buffer);
                fscanf(fp, "%s",buffer);
                pmobilePlatform->fileName_motionSoundAfterTrigger=buffer;
                //----motionSoundAfterTrigger_looping
                int motionSoundAfterTrigger_looping;
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%d",&motionSoundAfterTrigger_looping);
                pmobilePlatform->motionSoundAfterTrigger_looping=motionSoundAfterTrigger_looping;
                //----k_inertialVup
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%f",&pmobilePlatform->k_inertialVup);
                //----bounceVup
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%f",&pmobilePlatform->bounceVup);
                //----initMovPercentage
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%f",&pmobilePlatform->initMovPercentage);
                //----startPos
                float startPos[4]={0,0,0,1};
                fscanf(fp, "%f",&startPos[0]);
                fscanf(fp, "%f",&startPos[1]);
                fscanf(fp, "%f",&startPos[2]);
                //----endPos
                float endPos[4]={0,0,0,1};
                fscanf(fp, "%f",&endPos[0]);
                fscanf(fp, "%f",&endPos[1]);
                fscanf(fp, "%f",&endPos[2]);
                pmobilePlatform->set_startPosAndEndPosAndCalLen2_start_end(startPos, endPos);
                //----movSpeed
                float movSpeed;
                fscanf(fp, "%f",&movSpeed);
                pmobilePlatform->set_movSpeed(movSpeed);
                pmobilePlatform->calSpeedvec_forth_baseOnStartPosEndPosAndMovSpeed();
                //----rot_n
                float t_rot_n[4]={0,0,0,0};
                fscanf(fp, "%f",&t_rot_n[0]);
                fscanf(fp, "%f",&t_rot_n[1]);
                fscanf(fp, "%f",&t_rot_n[2]);
                pmobilePlatform->rot_n.init(t_rot_n);
                //----dA
                float dA;
                fscanf(fp, "%f",&dA);
                pmobilePlatform->dA=dA;
                pmobilePlatform->cos_dA=cosf(c3d_PI/180*dA);
                pmobilePlatform->sin_dA=sinf(c3d_PI/180*dA);
                //----rotAnglePerTrigger
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%f",&pmobilePlatform->rotAnglePerTrigger);
                //----初始旋转轴和旋转角度--abc
                float initRotAxis[3];
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%f",&initRotAxis[0]);
                fscanf(fp, "%f",&initRotAxis[1]);
                fscanf(fp, "%f",&initRotAxis[2]);
                float initRotAngle;
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%f",&initRotAngle);
                //求initRmat
                Cc3dMatrix4 Rmat=calculateRotationMatrix(Cc3dVector4(initRotAxis[0], initRotAxis[1], initRotAxis[2],0),cosf(initRotAngle*c3d_PI/180), sinf(initRotAngle*c3d_PI/180));
                //calculateRotationMatrix(initRotAxis[0], initRotAxis[1], initRotAxis[2], cosf(initRotAngle*c3d_PI/180), sinf(initRotAngle*c3d_PI/180));
                pmobilePlatform->initRMat=Rmat;
                //求初始位置（根据startPos,endPos和initMovPercentage)
                Cc3dVector4 initPos=Cc3dVector4(startPos)+(Cc3dVector4(endPos)-Cc3dVector4(startPos))*pmobilePlatform->initMovPercentage;
                //设置RTmat
                Cc3dTransform *transf=new Cc3dTransform();
                transf->autorelease();
                transf->setRmat(Rmat);
                transf->setPos(initPos.x(), initPos.y(), initPos.z());
                pmobilePlatform->setTransform(transf);
                //----destroyable
                int destroyable;
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%d",&destroyable);
                pmobilePlatform->destroyable=destroyable;
                //----geoType
                {
                    char buffer[1024]={0};
                    fscanf(fp, "%s",buffer);
                    pmobilePlatform->geoTypeName=buffer;
                }
                //----texImageName
                //如果为"null"，则说明是要用自带贴图--abc
                {
                    char buffer[1024]={0};
                    fscanf(fp, "%s",buffer);
                    pmobilePlatform->texImageName=buffer;
                }
                //----rx,ry,rz
                fscanf(fp, "%f",&pmobilePlatform->rx);
                fscanf(fp, "%f",&pmobilePlatform->ry);
                fscanf(fp, "%f",&pmobilePlatform->rz);
                //根据rx,ry,rz,startPos,endPos计算Rc
                pmobilePlatform->calRc_baseOn_RxRyRz();
                //根据startPos,endPos,movSpeed,dA来计算movType
                pmobilePlatform->calMovType();
                //将pmobilePlatform加入到pModelBaseList
                addActor(pmobilePlatform);
            }
            fscanf(fp, "%s",buffer);
            if(buffer[0]!='#'){
                cout<<"error:缺少#或实际条目数比n值大!"<<endl;
                assert(false);
            }
        }
        //关闭文件--abc
        fclose(fp);
    }

    CmobilePlatform* collisionTest_lineSeg(const Cc3dVector4&p1,const Cc3dVector4&p2,Cc3dVector4&p,Cc3dVector4&collisionFaceNorm)
    //前端返回碰撞的平台，如果没有则为NULL
    {
        //作以p1,p2中点为球心，p1p2距离的一半为半径的球体--abc
        Cc3dVector4 c=(p1+p2)*0.5;
		Cc3dVector4 p1p2=p2-p1;
		float R=getLength(p1p2)/2;
        //检测--abc
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            CmobilePlatform*pmobilePlatform=(CmobilePlatform*)actorList[i];
            if(pmobilePlatform->getIsDied())continue;//跳过死亡的运动平台--abc
            Cc3dVector4 tcollisionFaceNorm;
            bool collision=collisionTest_lineSeg_common(pmobilePlatform->getCollisionTriList(), p1, p2, c, R, p,tcollisionFaceNorm);
            if(collision){
                collisionFaceNorm=tcollisionFaceNorm;
                return pmobilePlatform;
            }
        }
        collisionFaceNorm.init(0,0,0,0);
        return NULL;
    }
    float getH_floor(const Cc3dVector4&c,float Rc,Cc3dVector4& mobilePlatformSurfaceNorm,
                     CmobilePlatform*&pSurfaceMobilePlatform//注意要用指针的引用--abc
                     )const {
        float floorHMax=-c3d_INF;//各mobilePlatform在c处的最大地面高度--abc
        Cc3dVector4 floorNormMax(0,1,0,0);//最大地面高度处的法向--abc
        CmobilePlatform*pmobilePlatformMax=NULL;
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            CmobilePlatform*pmobilePlatform=(CmobilePlatform*)actorList[i];
            if(pmobilePlatform->getIsDied())continue;//跳过死亡的运动平台--abc
            //求pmobilePlatform在c处表面高度和法向--abc
            Cc3dVector4 norm;
            float H;
            H=getH_floor_common(pmobilePlatform->getCollisionTriList(), c, Rc, norm);
            if(H>floorHMax){
                floorHMax=H;
                floorNormMax=norm;
                pmobilePlatformMax=pmobilePlatform;
            }
        }//得到floorHMax和floorNormMax和pmobilePlatformMax
        mobilePlatformSurfaceNorm=floorNormMax;
        pSurfaceMobilePlatform=pmobilePlatformMax;
        return floorHMax;
    }
    float getH_ceil(const Cc3dVector4&c,float Rc,Cc3dVector4&mobilePlatformCeilNorm,
                    CmobilePlatform*&pCeilMobilePlatform//注意要用指针的引用--abc
                    )const {
        float ceilHMin=c3d_INF;//各mobilePlatform在c处的最低天花板高度--abc
        Cc3dVector4 ceilNormMin(0,-1,0,0);//最低天花板高度处的法向--abc
        CmobilePlatform*pmobilePlatformMin=NULL;
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            CmobilePlatform*pmobilePlatform=(CmobilePlatform*)actorList[i];
            if(pmobilePlatform->getIsDied())continue;//跳过死亡的运动平台--abc
            //求pmobilePlatform在c处天花板高度和法向--abc
            Cc3dVector4 norm;
            float H;
            H=getH_ceil_common(pmobilePlatform->getCollisionTriList(), c, Rc, norm);
            if(H<ceilHMin){
                ceilHMin=H;
                ceilNormMin=norm;
                pmobilePlatformMin=pmobilePlatform;
            }
        }//得到ceilHMin和ceilNormMin和pmobilePlatformMin
        mobilePlatformCeilNorm=ceilNormMin;
        pCeilMobilePlatform=pmobilePlatformMin;
        return ceilHMin;
    }
  

    
    
};
#endif /* defined(__HelloOpenGL__mobilePlatform__) */
