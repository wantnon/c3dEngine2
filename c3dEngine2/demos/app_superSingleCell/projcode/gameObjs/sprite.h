//
//  sprite.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-22.
//
//

#ifndef __HelloOpenGL__sprite__
#define __HelloOpenGL__sprite__

#include <iostream>
#include "c3d.h"
#include "actorSet.h"
class Csprite:public Cc3dActor
{
protected:
    float xvec[3],yvec[3],zvec[3];
    float c[3];//中心坐标--abc
    float rx;//x方向边心距--abc
    float ry;//y方向边心距--abc
    bool died;
    Cc3dVector4 transpColor;//透明颜色--abc
    float Rc;//碰撞半径--abc
public:
    float getRx()const{return rx;};
    float getRy()const{return ry;};
    void setRc(float _Rc){
        Rc=_Rc;
    }
    float getRc()const{return Rc;}
    
    
    Csprite(){
        Rc=0;
        died=false;
        transpColor.init(1,1,1,1);
    }
    virtual~Csprite(){
        
    }
    void setDied(bool _died){
        died=_died;
        
    }
    bool getDied()const {
        return died;
    }
    Cc3dVector4 getTranspColor()const{return transpColor;}
    Cc3dVector4 getCenter()const{return Cc3dVector3(c).toV4(1);}
    void setCenter(const Cc3dVector4&center){
        c[0]=center.x();
        c[1]=center.y();
        c[2]=center.z();
    }
    Cc3dVector4 getXvec()const{
        return xvec;
    }
    Cc3dVector4 getYvec()const{
        return yvec;
    }
    Cc3dVector4 getZvec(){
        return zvec;
    }
    bool init(float _rx,float _ry,float sRepeat=1,float tRepeat=1){
        rx=_rx;
        ry=_ry;
        //生成mesh
        Cc3dMesh*mesh=new Cc3dMesh();
        mesh->autorelease();
        addMesh(mesh);
        //生成submesh
        int nPoint=4;
        Cc3dSubMesh *subMesh=new Cc3dSubMesh();
        subMesh->autorelease();
        for(int i=0;i<nPoint;i++){
            Cc3dVertex v(Cc3dVector4(0,0,0,1),Cc3dVector2(0,0),Cc3dVector4(0,0,0,0));
            subMesh->getSubMeshData()->addVertex(v);
        }
        Cc3dIDTriangle IDtri0,IDtri1;
        IDtri0.setvID(0, 1, 2);
        IDtri1.setvID(0, 2, 3);
        subMesh->getSubMeshData()->addIDtri(IDtri0);
        subMesh->getSubMeshData()->addIDtri(IDtri1);
        Cc3dVertex v0=subMesh->getSubMeshData()->getVertexByIndex(0);
        Cc3dVertex v1=subMesh->getSubMeshData()->getVertexByIndex(1);
        Cc3dVertex v2=subMesh->getSubMeshData()->getVertexByIndex(2);
        Cc3dVertex v3=subMesh->getSubMeshData()->getVertexByIndex(3);
        Cc3dVector2 u0(0,tRepeat);
        Cc3dVector2 u1(sRepeat,tRepeat);
        Cc3dVector2 u2(sRepeat,0);
        Cc3dVector2 u3(0,0);
        v0.setTexCoord(u0);
        v1.setTexCoord(u1);
        v2.setTexCoord(u2);
        v3.setTexCoord(u3);
        subMesh->getSubMeshData()->setVertexByIndex(0,v0);
        subMesh->getSubMeshData()->setVertexByIndex(1,v1);
        subMesh->getSubMeshData()->setVertexByIndex(2,v2);
        subMesh->getSubMeshData()->setVertexByIndex(3,v3);
        //加入到模型--abc
        this->getMesh()->addSubMesh(subMesh);
		return true;
    }
    Cc3dMesh* getMesh()const {
        assert((int)getModel()->getMeshCount()==1);
        return getModel()->getMeshByIndex(0);
    }
    void setPos(const Cc3dVector4&pos){
        c[0]=pos.x();
        c[1]=pos.y();
        c[2]=pos.z();
    }
    Cc3dVector4 getPos() {
        return Cc3dVector3(c).toV4(1);
    }
    void adjustFaceAndSubmitVertex(const Cc3dVector4&eyePos){
        adjustFace(eyePos);
        this->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STREAM_DRAW);
        
    }
    void adjustFace_centerAtBottomAndSubmitVertex(const Cc3dVector4&eyePos){
        adjustFace_centerAtBottom(eyePos);
        this->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STREAM_DRAW);
        
    }
    void calAxis(const Cc3dVector4&zvec,Cc3dVector4&xvec,Cc3dVector4&yvec)
    //计算出一个以zvec为z轴的空间直角坐标系--abc
    //zvec须为单位向量--abc
    //返回坐标系鼍两个轴的单位向量xvec,yvec
    {
        Cc3dVector4 up(0,1,0,0);//up朝上--abc
        xvec=cross(up, zvec);
        xvec=normalize(xvec);
        yvec=cross(zvec, xvec);
        
    }
    
    
    void adjustFace(const Cc3dVector4&eyePos){//调整朝向(旋转中心在中心)
        //以pos为原点，normalize(pos->eyePos)为z轴建立一个直角坐标系，up朝上--abc
        Cc3dVector4 t_zvec=normalize(eyePos-Cc3dVector3(c).toV4(1));
        Cc3dVector4 t_xvec,t_yvec;
        calAxis(t_zvec, t_xvec, t_yvec);
        zvec[0]=t_zvec.x();
        zvec[1]=t_zvec.y();
        zvec[2]=t_zvec.z();
        xvec[0]=t_xvec.x();
        xvec[1]=t_xvec.y();
        xvec[2]=t_xvec.z();
        yvec[0]=t_yvec.x();
        yvec[1]=t_yvec.y();
        yvec[2]=t_yvec.z();
        //制作正方形面的四个顶点--abc
        Cc3dVector4 xvecMulrx=Cc3dVector3(xvec).toV4(0)*rx;
        Cc3dVector4 yvecMulry=Cc3dVector3(yvec).toV4(0)*ry;
        //求左下角p0
        Cc3dVector4 p0=Cc3dVector3(c).toV4(1)-xvecMulrx-yvecMulry;
        //求右下角p1
        Cc3dVector4 p1=Cc3dVector3(c).toV4(1)+xvecMulrx-yvecMulry;
        //求右上角p2
        Cc3dVector4 p2=Cc3dVector3(c).toV4(1)+xvecMulrx+yvecMulry;
        //求左上角p3
        Cc3dVector4 p3=Cc3dVector3(c).toV4(1)-xvecMulrx+yvecMulry;
        Cc3dVertex v0=this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(0);
        Cc3dVertex v1=this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(1);
        Cc3dVertex v2=this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(2);
        Cc3dVertex v3=this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(3);
        v0.setPos(p0);
        v1.setPos(p1);
        v2.setPos(p2);
        v3.setPos(p3);
        this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(0,v0);
        this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(1,v1);
        this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(2,v2);
        this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(3,v3);
    }
    void adjustFace_centerAtBottom(const Cc3dVector4&eyePos){//调整朝向(旋转中心在底部)
        //以pos为原点，normalize(pos->eyePos)为z轴建立一个直角坐标系，up朝上--abc
        Cc3dVector4 t_zvec=normalize(eyePos-Cc3dVector3(c).toV4(1));
        Cc3dVector4 t_xvec,t_yvec;
        calAxis(t_zvec, t_xvec,t_yvec);
        zvec[0]=t_zvec.x();
        zvec[1]=t_zvec.y();
        zvec[2]=t_zvec.z();
        xvec[0]=t_xvec.x();
        xvec[1]=t_xvec.y();
        xvec[2]=t_xvec.z();
        yvec[0]=t_yvec.x();
        yvec[1]=t_yvec.y();
        yvec[2]=t_yvec.z();
        //制作正方形面的四个顶点--abc
        Cc3dVector4 xvecMulrx=Cc3dVector3(xvec).toV4(0)*rx;
        Cc3dVector4 yvecMul2ry=Cc3dVector3(yvec).toV4(0)*(2*ry);
        
        //求左下角p0
        Cc3dVector4 p0=Cc3dVector3(c).toV4(1)-xvecMulrx;
        //求右下角p1
        Cc3dVector4 p1=Cc3dVector3(c).toV4(1)+xvecMulrx;
        //求右上角p2
        Cc3dVector4 p2=Cc3dVector3(c).toV4(1)+xvecMulrx+yvecMul2ry;
        //求左上角p3
        Cc3dVector4 p3=Cc3dVector3(c).toV4(1)-xvecMulrx+yvecMul2ry;
        
        Cc3dVertex v0=this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(0);
        Cc3dVertex v1=this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(1);
        Cc3dVertex v2=this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(2);
        Cc3dVertex v3=this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(3);
        v0.setPos(p0);
        v1.setPos(p1);
        v2.setPos(p2);
        v3.setPos(p3);
        this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(0,v0);
        this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(1,v1);
        this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(2,v2);
        this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(3,v3);
        
    }
    
    
};
class Csprite_timing:public Csprite
{
public:
    int duringTime;
    int timeCounter;

    Csprite_timing(){
        duringTime=100;
        timeCounter=0;
 
    }
    virtual~Csprite_timing(){};
    void init(int _duringTime,float _rx,float _ry){
        duringTime=_duringTime;
        Csprite::init(_rx,_ry);
        
    }
    void timing(){
        timeCounter++;
        if(timeCounter>=duringTime){
            died=true;
            timeCounter=0;
            return;
        }
    }

    
};
class CspriteSet:public CactorSet
{
public:
    void adjustFaceAndSubmitVertex(const Cc3dVector4&eyePos,const vector<bool>&maskList){
        if((int)maskList.size()!=(int)actorList.size()){
            cout<<"error:maskList元素数不正确!"<<endl;
            assert(false);
        }
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            Csprite*pSprite=(Csprite*)actorList[i];
            if(maskList[i])continue;
            pSprite->adjustFaceAndSubmitVertex(eyePos);
        }
    }
    void adjustFaceAndSubmitVertex(const Cc3dVector4&eyePos){
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            Csprite*pSprite=(Csprite*)actorList[i];
            pSprite->adjustFaceAndSubmitVertex(eyePos);
        }
    }

    vector<bool> get_diedList()const {
        vector<bool> diedList;
        int n=(int)actorList.size();
        diedList.resize(n);
        for(int i=0;i<n;i++){
            Csprite*pSprite=(Csprite*)actorList[i];
            diedList[i]=pSprite->getDied();
        }
        return diedList;
    }
    vector<bool> get_undiedList()const {
        vector<bool> undiedList;
        int n=(int)actorList.size();
        undiedList.resize(n);
        for(int i=0;i<n;i++){
            Csprite*pSprite=(Csprite*)actorList[i];
            undiedList[i]=!pSprite->getDied();
        }
        return undiedList;
    }
    
};
#endif /* defined(__HelloOpenGL__sprite__) */
