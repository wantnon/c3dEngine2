//
//  triangleWithNorm.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-26.
//
//

#ifndef __HelloOpenGL__triangleWithNorm__
#define __HelloOpenGL__triangleWithNorm__

#include <iostream>
using namespace std;
#include "core/c3dVector.h"
class CtriangleWithNorm{
protected:
    Cc3dVector4 m_vert[3];
    Cc3dVector4 m_norm;
public:
    CtriangleWithNorm(){
    
    }
    CtriangleWithNorm(const Cc3dVector4&vert0,const Cc3dVector4&vert1,const Cc3dVector4&vert2,const Cc3dVector4&norm){
        assert(norm.w()==0);
        m_vert[0]=vert0;
        m_vert[1]=vert1;
        m_vert[2]=vert2;
        m_norm=norm;
    }
    virtual~CtriangleWithNorm(){
        
    }
    Cc3dVector4 getVert(int index)const{
        assert(index>=0&&index<=2);
        return m_vert[index];
    }
    void setVert(int index,const Cc3dVector4&vert){
        assert(index>=0&&index<=2);
        m_vert[index]=vert;
    }
    Cc3dVector4 getNorm()const{return m_norm;}
    void setNorm(const Cc3dVector4&norm){
        m_norm=norm;
    }
};
#endif /* defined(__HelloOpenGL__triangleWithNorm__) */
