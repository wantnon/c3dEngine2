//
//  box.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#ifndef __HelloOpenGL__box__
#define __HelloOpenGL__box__

#include <iostream>
#include "c3d.h"
class Cbox:public Cc3dActor
{
protected:
  
public:
    Cbox(){

    } 
    virtual~Cbox(){ 
      
      
    }
	void makeBox(float d,string texFilePath);
	

};



#endif /* defined(__HelloOpenGL__box__) */
