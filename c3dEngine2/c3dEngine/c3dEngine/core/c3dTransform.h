//
//  c3dTransform.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//

#ifndef __HelloOpenGL__c3dTransform__
#define __HelloOpenGL__c3dTransform__

#include <iostream>
#include <math.h>
#include "c3dMatrix.h"
#include "c3dVector.h"
#include "c3dMath.h"
#include "c3dGLMath.h"
#include "c3dObject.h"
Cc3dMatrix4 RTmatToRmat(const Cc3dMatrix4&RTmat);
class Cc3dTransform:public Cc3dObject
{
protected:
    Cc3dMatrix4 m_RTmat;//rotation and translation
    float m_scaleX,m_scaleY,m_scaleZ;//scale
public:
    Cc3dTransform(){
        m_RTmat=unitMat();
        m_scaleX=m_scaleY=m_scaleZ=1;
        
    }
    void init_dft(){}
    virtual~Cc3dTransform(){}
    bool init(const Cc3dMatrix4&RTmat,float scaleX=1,float scaleY=1,float scaleZ=1){
        m_RTmat=RTmat;
        m_scaleX=scaleX;m_scaleY=scaleY;m_scaleZ=scaleZ;
        return true;
    }
    //----scale
    float getScaleX()const{return m_scaleX;}
    float getScaleY()const{return m_scaleY;}
    float getScaleZ()const{return m_scaleZ;}
    bool getIsHasScale()const {
        return (m_scaleX!=1||m_scaleY!=1||m_scaleZ!=1);
    }
    void setScale(float scaleX,float scaleY,float scaleZ);
    void scale(float scaleX,float scaleY,float scaleZ);
    //----rotate
    void rotate(float nx,float ny,float nz,float cosa,float sina);
    void rotateRelativeToFather(float nx,float ny,float nz,float cosa,float sina);
    void setRotation(const Cc3dMatrix4&Rmat);
    Cc3dMatrix4 getRotation()const{return getRmat();};
    //----move
    void move(float dx,float dy,float dz);
    void moveX(float dx);
    void moveY(float dy);
    void moveZ(float dz);
    void moveRelativeToFather(float dx,float dy,float dz);
    //----position
    Cc3dVector4 getPos()const;
    float getPosY()const;
    void setPos(float x,float y,float z);
    void setPos(const Cc3dVector4&pos);
    void setRot(float nx,float ny,float nz,float cosa,float sina);
    void setPosY(float y);
    //----convert between space
    Cc3dVector4 convertToWorldSpace(const Cc3dVector4&pos_local)const;
    //----matrix access (for advanced user)
    void setRmat(const Cc3dMatrix4&Rmat);
    Cc3dMatrix4 getRmat()const{return RTmatToRmat(m_RTmat);}
    void setRTmat(const Cc3dMatrix4&RTmat);
    Cc3dMatrix4 getRTmat()const{return m_RTmat;}
	void setRTSmat(const Cc3dMatrix4&RTSmat);
    Cc3dMatrix4 getRTSmat()const;
    
};

#endif /* defined(__HelloOpenGL__c3dTransform__) */
