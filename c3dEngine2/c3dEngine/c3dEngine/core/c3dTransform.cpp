//
//  c3dTransform.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//
#include <math.h>
#include "c3dTransform.h"
Cc3dMatrix4 RTmatToRmat(const Cc3dMatrix4&RTmat){
    Cc3dMatrix4 Rmat=RTmat;
    Rmat.setAt(12,0);
    Rmat.setAt(13,0);
    Rmat.setAt(14,0);
    return Rmat;

}
void Cc3dTransform::setRmat(const Cc3dMatrix4&Rmat){
    assert(Rmat.getAt(12)==0);
    assert(Rmat.getAt(13)==0);
    assert(Rmat.getAt(14)==0);
    float x=m_RTmat.getAt(12);
    float y=m_RTmat.getAt(13);
    float z=m_RTmat.getAt(14);
    m_RTmat=Rmat;
    m_RTmat.setAt(12, x);
    m_RTmat.setAt(13, y);
    m_RTmat.setAt(14, z);
}
void Cc3dTransform::setRotation(const Cc3dMatrix4&Rmat){
    setRmat(Rmat);
}
void Cc3dTransform::setRTmat(const Cc3dMatrix4&RTmat){
    m_RTmat=RTmat;
}
void Cc3dTransform::setRTSmat(const Cc3dMatrix4&RTSmat){
	Cc3dVector4 scaleVector=extractScaleFromRTSmat(RTSmat);
	assert(scaleVector.x()!=0);
	assert(scaleVector.y()!=0);
	assert(scaleVector.z()!=0);
	assert(scaleVector.w()!=0);
	Cc3dMatrix4 scaleReverse=calculateScaleMatrix(1.0/scaleVector.x(),1.0/scaleVector.y(),1.0/scaleVector.z(),1.0/scaleVector.w());
	Cc3dMatrix4 RTmat=RTSmat*scaleReverse;
	this->setRTmat(RTmat);
	this->setScale(scaleVector.x(),scaleVector.y(),scaleVector.z());

}
Cc3dVector4 Cc3dTransform::getPos()const{
    Cc3dVector4 pos(m_RTmat.getAt(12),m_RTmat.getAt(13),m_RTmat.getAt(14),1);
    return pos;
}
float Cc3dTransform::getPosY()const {
    return m_RTmat.getAt(13);
}
void Cc3dTransform::setPos(float x,float y,float z){
    m_RTmat.setAt(12, x);
    m_RTmat.setAt(13, y);
    m_RTmat.setAt(14, z);
}
void Cc3dTransform::setPos(const Cc3dVector4&pos){
    setPos(pos.x(),pos.y(),pos.z());
}
void Cc3dTransform::setRot(float nx,float ny,float nz,float cosa,float sina){
    //n must be normalized vector
    float _cosa=1-cosa;
    float nx_cosa=nx*_cosa;
    float nz_cosa=nz*_cosa;
    float nxnx_cosa=nx*nx_cosa;
    float nxny_cosa=ny*nx_cosa;
    float nxnz_cosa=nz*nx_cosa;
    float nyny_cosa=ny*ny*_cosa;
    float nynz_cosa=ny*nz_cosa;
    float nznz_cosa=nz*nz_cosa;
    float nxsina=nx*sina;
    float nzsina=nz*sina;
    float nysina=ny*sina;
    float rotmat[16]={
        nxnx_cosa+cosa,nxny_cosa+nzsina,nxnz_cosa-nysina,0,//col 1
        nxny_cosa-nzsina,nyny_cosa+cosa,nynz_cosa+nxsina,0,//col 2
        nxnz_cosa+nysina,nynz_cosa-nxsina,nznz_cosa+cosa,0,//col 3
        0,0,0,1//col 4
    };
    setRmat(rotmat);
}


void Cc3dTransform::setPosY(float y){
    m_RTmat.setAt(13, y);
}
Cc3dVector4 Cc3dTransform::convertToWorldSpace(const Cc3dVector4&pos_local)const{
    return m_RTmat*pos_local;
}

void Cc3dTransform::setScale(float scaleX,float scaleY,float scaleZ){
    m_scaleX=scaleX;
    m_scaleY=scaleY;
    m_scaleZ=scaleZ;
}
void Cc3dTransform::scale(float scaleX,float scaleY,float scaleZ){
    m_scaleX*=scaleX;
    m_scaleY*=scaleY;
    m_scaleZ*=scaleZ;
    
}
Cc3dMatrix4 Cc3dTransform::getRTSmat()const{
    //apply scale to RTmat
    Cc3dMatrix4 RTSmat;
    RTSmat=m_RTmat;
    RTSmat.setAt(0, RTSmat.getAt(0)*m_scaleX);
    RTSmat.setAt(1, RTSmat.getAt(1)*m_scaleX);
    RTSmat.setAt(2, RTSmat.getAt(2)*m_scaleX);
    
    RTSmat.setAt(4, RTSmat.getAt(4)*m_scaleY);
    RTSmat.setAt(5, RTSmat.getAt(5)*m_scaleY);
    RTSmat.setAt(6, RTSmat.getAt(6)*m_scaleY);
    
    RTSmat.setAt(8, RTSmat.getAt(8)*m_scaleZ);
    RTSmat.setAt(9, RTSmat.getAt(9)*m_scaleZ);
    RTSmat.setAt(10, RTSmat.getAt(10)*m_scaleZ);
    
    return RTSmat;
}

void Cc3dTransform::rotate(float nx,float ny,float nz,float cosa,float sina)
//n must be normalized vector
{
    Cc3dMatrix4 rotmat=calculateRotationMatrix(Cc3dVector4(nx,ny,nz,0),cosa,sina); //calculateRotationMatrix(nx, ny, nz, cosa, sina);
    m_RTmat=m_RTmat*rotmat;
}


 void Cc3dTransform::rotateRelativeToFather(float nx,float ny,float nz,float cosa,float sina)
//n must be normalized vector
{
    float _cosa=1-cosa;
    float nx_cosa=nx*_cosa;
    float nz_cosa=nz*_cosa;
    float nxnx_cosa=nx*nx_cosa;
    float nxny_cosa=ny*nx_cosa;
    float nxnz_cosa=nz*nx_cosa;
    float nyny_cosa=ny*ny*_cosa;
    float nynz_cosa=ny*nz_cosa;
    float nznz_cosa=nz*nz_cosa;
    float nxsina=nx*sina;
    float nzsina=nz*sina;
    float nysina=ny*sina;
    float rotmat[16]={
        nxnx_cosa+cosa,nxny_cosa+nzsina,nxnz_cosa-nysina,0,//col 1
        nxny_cosa-nzsina,nyny_cosa+cosa,nynz_cosa+nxsina,0,//col 2
        nxnz_cosa+nysina,nynz_cosa-nxsina,nznz_cosa+cosa,0,//col 3
        0,0,0,1//col 4
    };
    m_RTmat=Cc3dMatrix4(rotmat)*m_RTmat;
}
void Cc3dTransform::move(float dx,float dy,float dz){//����ھֲ�����ϵ�ƶ��ֲ����꣨��dx,dy,dzΪ�ֲ�����ϵ�е�����
    // RTmat    *   Tmat     =    RTmat'
    // a d g x     1 0 0 dx     a d g x+adx+ddy+gdz
    // b e h y  *  0 1 0 dy  =  b e h y+bdx+edy+hdz
    // c f i z     0 0 1 dz     c f i z+cdx+fdy+idz
    // 0 0 0 1     0 0 0  1     0 0 0 1
    float RTmat[16];
    memcpy(RTmat, m_RTmat.getArray(),sizeof(RTmat));
    RTmat[12]+=RTmat[0]*dx+RTmat[4]*dy+RTmat[8]*dz;
    RTmat[13]+=RTmat[1]*dx+RTmat[5]*dy+RTmat[9]*dz;
    RTmat[14]+=RTmat[2]*dx+RTmat[6]*dy+RTmat[10]*dz;
    m_RTmat.init(RTmat);
}
void Cc3dTransform::moveX(float dx){
    move(dx,0,0);
}
void Cc3dTransform::moveY(float dy){//����ھֲ�����ϵ�ƶ��ֲ����꣨��dx=0,dy,dz=0Ϊ�ֲ�����ϵ�е�����
    move(0, dy, 0);
}
void Cc3dTransform::moveZ(float dz){//����ھֲ�����ϵ�ƶ��ֲ����꣨��dx=0,dy=0,dzΪ�ֲ�����ϵ�е�����
    move(0, 0, dz);
}

void Cc3dTransform::moveRelativeToFather(float dx,float dy,float dz){//�ڸ�����ϵ���ƶ��ֲ�����ϵ(��dx,dy,dzΪ������ϵ�е���)
    //  Tmat    *   RTmat   =   RTmat'
    // 1 0 0 dx    a d g x     a d g x+dx
    // 0 1 0 dy *  b e h y  =  b e h y+dy
    // 0 0 1 dz    c f i z     c f i z+dz
    // 0 0 0 1     0 0 0 1     0 0 0 1
    m_RTmat.setAt(12, m_RTmat.getAt(12)+dx);
    m_RTmat.setAt(13, m_RTmat.getAt(13)+dy);
    m_RTmat.setAt(14, m_RTmat.getAt(14)+dz);
}