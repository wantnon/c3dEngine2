//
//  c3dSpan.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-19.
//
//

#ifndef __HelloOpenGL__c3dSpan__
#define __HelloOpenGL__c3dSpan__

#include <iostream>
using namespace std;
class Cc3dSpan{
protected:
    float m_spanX;
    float m_spanY;
    float m_spanZ;
public:
    Cc3dSpan(){
        initMembers();
    }
    Cc3dSpan(float spanX,float spanY,float spanZ){
        initMembers();
        init(spanX,spanY,spanZ);
    }
    virtual~Cc3dSpan(){}
    void init(float spanX,float spanY,float spanZ){
        m_spanX=spanX;
        m_spanY=spanY;
        m_spanZ=spanZ;
    }
    void setSpanX(float spanX){
        m_spanX=spanX;
    }
    void setSpanY(float spanY){
        m_spanY=spanY;
    }
    void setSpanZ(float spanZ){
        m_spanZ=spanZ;
    }
    float getSpanX()const{return m_spanX;}
    float getSpanY()const{return m_spanY;}
    float getSpanZ()const{return m_spanZ;}
protected:
    void initMembers(){
        m_spanX=0;
        m_spanY=0;
        m_spanZ=0;
    }
    
};

#endif /* defined(__HelloOpenGL__c3dSpan__) */
