//
//  ball.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#ifndef __HelloOpenGL__ball__
#define __HelloOpenGL__ball__

#include <iostream>
#include "c3d.h"
class Cball:public Cc3dActor
{
protected:
  
public:
    Cball(){

    } 
    virtual~Cball(){ 
      
      
    }
	void makeBall(float r,int nSlice,int nStack,string texFilePath);
	

};



#endif /* defined(__HelloOpenGL__ball__) */
