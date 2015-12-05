//
//  layer.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#ifndef __HelloOpenGL__layer__
#define __HelloOpenGL__layer__

#include <iostream>
using namespace std;
#include "c3d.h"
#include "globalVars.h"
//#include "extension.h"
#include "ball.h"
void buildinProgramPassUnifoCallback_cubemap(Cc3dNode*node, Cc3dProgram*program);
class Clayer:public Cc3dActor
{

public:
    Clayer(){
        m_ball=NULL;
        m_CamAngleX=90-45;
        m_CamAngleY=90-30;
        m_CamDisToTarget=20;
        m_CamTarget=Cc3dVector4(0,0,0,1);
    } 
    virtual~Clayer(){
        
        //----remove unused resources in caches on next frame
		Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
		Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();
    }
	void update(float dt){
        Cc3dVector4 eyePos(m_CamTarget.x()+m_CamDisToTarget*sinf(m_CamAngleY*c3d_PI/180)*cosf(m_CamAngleX*c3d_PI/180),
                           m_CamTarget.y()+m_CamDisToTarget*cosf(m_CamAngleY*c3d_PI/180),
                           m_CamTarget.z()+m_CamDisToTarget*sinf(m_CamAngleY*c3d_PI/180)*sinf(m_CamAngleX*c3d_PI/180),
                           1);
        Cc3dVector4 eyeTg=m_CamTarget;
        Cc3dVector4 up(0,1,0,0);
        camera->setEyePos(eyePos);
        camera->setCenter(eyeTg);
        camera->setUp(up);
        camera->updateFrustum();


  
    }
    void touchesBegan(const vector<Cc3dVector2>&points){
        if(points.empty()==false){
			Cc3dVector2 point=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->convertScreenToGL(points[0]);
            m_touchPointFoe=m_touchPoint;
            m_touchPoint=point;
        }
    }
    void touchesMoved(const vector<Cc3dVector2>&points){
        if(points.empty()==false){
			Cc3dVector2 point=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->convertScreenToGL(points[0]);
            m_touchPointFoe=m_touchPoint;
            m_touchPoint=point;
            float dAngleX=m_touchPoint.x()-m_touchPointFoe.x();
            float dAngleY=m_touchPoint.y()-m_touchPointFoe.y();
            m_CamAngleX+=dAngleX;
            m_CamAngleY+=dAngleY;
            if(m_CamAngleY<=0||m_CamAngleY>=180){
                m_CamAngleY-=dAngleY;//recover
            }
        }
    }
    void touchesEnded(const vector<Cc3dVector2>&points){
        if(points.empty()==false){
			Cc3dVector2 point=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->convertScreenToGL(points[0]);
            m_touchPointFoe=m_touchPoint;
            m_touchPoint=point;
        }
    }

    bool init(){
        //----create cubeTexture
		vector<string> filePathList;
		filePathList.push_back("cubemap_resource/px.png");
		filePathList.push_back("cubemap_resource/nx.png");
		filePathList.push_back("cubemap_resource/py.png");
		filePathList.push_back("cubemap_resource/ny.png");
		filePathList.push_back("cubemap_resource/pz.png");
		filePathList.push_back("cubemap_resource/nz.png");
		Cc3dTexture*cubeTex=new Cc3dTexture();
		cubeTex->autorelease();
		cubeTex->initCubeTexture(filePathList);
        
        
		//----ball
        
		m_ball=new Cball();
		m_ball->autorelease();
		m_ball->setName("box");
		m_ball->makeBall(4,40,40,"");
        
		m_ball->setPos(Cc3dVector4(0,0,0,1));
		m_ball->setLight(light0);
		m_ball->setCamera(camera);
		m_ball->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("cubemap"));
	    m_ball->setPassUnifoCallback(buildinProgramPassUnifoCallback_cubemap);
        
	    m_ball->genVBOBuffers();
		m_ball->submit(GL_STATIC_DRAW);
        
		//change texture to cubeTexture
		int nMesh=(int)m_ball->getModel()->getMeshCount();
		for(int i=0;i<nMesh;i++){
			int nSubMesh=(int)m_ball->getModel()->getMeshByIndex(i)->getSubMeshCount();
			for(int j=0;j<nSubMesh;j++){
				m_ball->getModel()->getMeshByIndex(i)->getSubMeshByIndex(j)->setTexture(cubeTex);
			}
		}
        addChild(m_ball);
        return true;
    }
protected:
    Cball*m_ball;
    float m_CamAngleX;
    float m_CamAngleY;
    float m_CamDisToTarget;
    Cc3dVector4 m_CamTarget;
    Cc3dVector2 m_touchPoint;
    Cc3dVector2 m_touchPointFoe;
  
};



#endif /* defined(__HelloOpenGL__layer__) */
