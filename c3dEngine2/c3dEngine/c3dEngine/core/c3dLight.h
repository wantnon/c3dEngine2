//
//  c3dLight.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-8.
//
//

#ifndef __HelloOpenGL__c3dLight__
#define __HelloOpenGL__c3dLight__

#include <iostream>
#include "c3dObject.h"
#include "c3dVector.h"
#include "c3dCamera.h"
class Cc3dLight:public Cc3dObject
{
protected:
    Cc3dVector4 m_pos;
    Cc3dVector4 m_diffuse;
    Cc3dVector4 m_ambient;
    Cc3dVector4 m_specular;
    vector<Cc3dCamera*> m_lightViewCameraList;
public:
    Cc3dLight(){
        m_pos.init(0,0,0,1);
        m_diffuse.init(1,1,1,1);
        m_ambient.init(1,1,1,1);
        m_specular.init(0,0,0,1);
        init_dft();
    }
    ~Cc3dLight(){
        int nLightViewCamera=(int)m_lightViewCameraList.size();
        for(int i=0;i<nLightViewCamera;i++){
            m_lightViewCameraList[i]->release();
        }
    }
    void init_dft(){};
    void addLightViewCamera(Cc3dCamera*lightViewCamera){
        m_lightViewCameraList.push_back(lightViewCamera);
        lightViewCamera->retain();
    }
    int getLightViewCameraCount()const {
        return (int)m_lightViewCameraList.size();
    }
    Cc3dCamera*getLightViewCameraByIndex(int index)const {
        assert(index>=0&&index<getLightViewCameraCount());
        return m_lightViewCameraList[index];
    }
    void updateEyePosForAllLightViewCameras(){
        int nLightViewCamera=(int)m_lightViewCameraList.size();
        for(int i=0;i<nLightViewCamera;i++){
            m_lightViewCameraList[i]->setEyePos(m_pos);
        }
    }
    Cc3dVector4 getPos()const{return m_pos;}

    Cc3dVector4 getDiffuse()const{return m_diffuse;}
    Cc3dVector4 getAmbient()const{return m_ambient;}
    Cc3dVector4 getSpecular()const{return m_specular;}
    void setPos(float x,float y,float z){
        m_pos.setx(x);
        m_pos.sety(y);
        m_pos.setz(z);
     
    }
	void setSpecular(const Cc3dVector4&specular){m_specular=specular;}
	void setDiffuse(const Cc3dVector4&diffuse){m_diffuse=diffuse;}
	void setAmbient(const Cc3dVector4&ambient){m_ambient=ambient;}
    void setSpecular(float r,float g,float b){
        m_specular.setr(r);
        m_specular.setg(g);
        m_specular.setb(b);
    }
	
    void setDiffuse(float r,float g,float b){
        m_diffuse.setr(r);
        m_diffuse.setg(g);
        m_diffuse.setb(b);
    }
	
    void setAmbient(float r,float g,float b){
        m_ambient.setr(r);
        m_ambient.setg(g);
        m_ambient.setb(b);
    }
};
#endif /* defined(__HelloOpenGL__c3dLight__) */
