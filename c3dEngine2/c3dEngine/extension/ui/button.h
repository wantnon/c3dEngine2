//
//  button.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#ifndef __HelloOpenGL__button__
#define __HelloOpenGL__button__

#include <iostream>
#include "c3d.h"
class Cbutton:public Cc3dActor
{
protected:
  float m_width;
  float m_height;
  Cc3dTexture*m_textureUp;
  Cc3dTexture*m_textureDn;
public:
    Cbutton(){
		m_width=0;
		m_height=0;
		m_textureUp=NULL;
		m_textureDn=NULL;
    } 
    virtual~Cbutton(){ 
		if(m_textureUp)m_textureUp->release();
		if(m_textureDn)m_textureDn->release();
    }
	bool init(float width,float height,string imagePath_up,string imagePath_dn,string imagePath_text);
	void update(float dt);
	bool isContainPoint(const Cc3dVector2&point);
	void setText(string imagePath_text);

};



#endif /* defined(__HelloOpenGL__button__) */
