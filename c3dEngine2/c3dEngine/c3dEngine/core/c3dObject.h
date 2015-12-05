//
//  c3dObject.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-13.
//
//

#ifndef __HelloOpenGL__c3dObject__
#define __HelloOpenGL__c3dObject__

#include <iostream>
#include <string>
using namespace std;
#include <assert.h>

class Cc3dObject{
public:
    Cc3dObject(){
        m_retainCount=1;
    }
    virtual~Cc3dObject(){}
    void retain(){
        m_retainCount++;
    }
    void autorelease();
    void release(){
        assert(m_retainCount>0);
        m_retainCount--;
        if(m_retainCount==0)delete this;
    
    }
    int getRetainCount()const {return m_retainCount;}
  
protected:
    int m_retainCount;
};
#endif /* defined(__HelloOpenGL__c3dObject__) */
