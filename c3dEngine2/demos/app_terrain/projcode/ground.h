//
//  ground.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#ifndef __HelloOpenGL__ground__
#define __HelloOpenGL__ground__

#include <iostream>
#include "c3d.h"
#include "extension.h"
class Cground:public Cterrain
{
protected:
  
public:
    Cground(){

    } 
    virtual~Cground(){ 
      
      
    }
	void update(float dt);
	
	

};



#endif /* defined(__HelloOpenGL__ground__) */
