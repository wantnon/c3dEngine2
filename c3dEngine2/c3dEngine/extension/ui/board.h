//
//  board.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#ifndef __HelloOpenGL__board__
#define __HelloOpenGL__board__

#include <iostream>
#include "c3d.h"
class Cboard:public Cc3dActor
{
protected:
  float m_width;
  float m_height;
public:
    Cboard(){
		m_width=0;
		m_height=0;
    } 
    virtual~Cboard(){ 
    }
	bool init(float width,float height,string imagePath);
	bool init(float width,float height,Cc3dTexture*texture);

};



#endif /* defined(__HelloOpenGL__board__) */
