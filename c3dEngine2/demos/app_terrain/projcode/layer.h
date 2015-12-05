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
#include "c3d.h"
#include "extension.h"
#include "ground.h"
class Clayer:public Cc3dActor
{
protected:
  Cbutton*m_button;
  Cground*m_ground;
  bool m_isWireMode;
public:
    Clayer(){
		m_button=NULL;
		m_ground=NULL;
		m_isWireMode=false;

    } 
    virtual~Clayer(){ 
        //----remove unused resources in caches on next frame
		Cc3dTextureCache::sharedTextureCache()->removeUnusedTexturesOnNextFrame();
		Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSourcesOnNextFrame();

      
    }
	void touchesBegan(const vector<Cc3dVector2>&points){
        
    }
    void touchesMoved(const vector<Cc3dVector2>&points){
       
    }
	void touchesEnded(const vector<Cc3dVector2>&points){
		if(points.empty()==false){
			//backKeyÏìÓ¦µ¥»÷--abc
			Cc3dVector2 point=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->convertScreenToGL(points[0]);
			if(m_button->isContainPoint(point)){
				m_isWireMode=!m_isWireMode;
				if(m_isWireMode){
					m_button->setText("terrain_resource/text2.png");
				}else{
					m_button->setText("terrain_resource/text1.png");

				}
				m_ground->setIsWireMode(m_isWireMode);//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE) not available on iOS
			}
		}

	}
	bool init();
	

};



#endif /* defined(__HelloOpenGL__layer__) */
