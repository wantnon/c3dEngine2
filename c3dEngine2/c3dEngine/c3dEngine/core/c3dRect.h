//
//  c3dRect.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-31.
//
//

#ifndef __HelloOpenGL__c3dRect__
#define __HelloOpenGL__c3dRect__

#include <iostream>
using namespace std;
class Cc3dRect{
public:
    Cc3dRect(){
        initMembers();
    }
    Cc3dRect(float xmin,float ymin,float width,float height){
        initMembers();
        init(xmin, ymin, width, height);
    }
    virtual~Cc3dRect(){}
    void init(float xmin,float ymin,float width,float height){
        m_xmin=xmin;
        m_ymin=ymin;
        m_width=width;
        m_height=height;
    }
    float getMinX()const{return m_xmin;}
    float getMaxX()const{return m_xmin+m_width;}
    float getMinY()const{return m_ymin;}
    float getMaxY()const{return m_ymin+m_height;}
    float getWidth()const{return m_width;}
    float getHeight()const{return m_height;}
protected:
    void initMembers(){
        m_xmin=m_ymin=0;
        m_width=m_height=0;
    }
protected:
    float m_xmin,m_ymin;
    float m_width,m_height;

};
#endif /* defined(__HelloOpenGL__c3dRect__) */
