//
//  ground.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-4.
//
//

#ifndef __HelloOpenGL__ground__
#define __HelloOpenGL__ground__

#include <iostream>
using namespace std;
#include "extension.h"
class Cground:public Cterrain
{
protected:
    bool m_isDoTexBlend;//是否通进纹理混合--abc
public:
    void loadConfig(const string&fileNameWithExt);
    void loadConfig_texBlend(const string&fileNameWithExt);
    Cground(){
        m_isDoTexBlend=false;
    }
    virtual~Cground(){}
    void setIsDoTexBlend(bool value){
        m_isDoTexBlend=value;
    }
    bool getIsDoTexBlend()const {
        return m_isDoTexBlend;
        
    }
    void update(float dt){
        this->updateTerrain(*this->getCamera());
        this->submitIndex(GL_DYNAMIC_DRAW);
    }
    

};
#endif /* defined(__HelloOpenGL__ground__) */
