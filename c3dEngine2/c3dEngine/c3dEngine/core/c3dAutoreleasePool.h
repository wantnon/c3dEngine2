//
//  Tc3dAutoreleasePool.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-13.
//
//

#ifndef __HelloOpenGL__c3dAutoreleasePool__
#define __HelloOpenGL__c3dAutoreleasePool__

#include <iostream>
#include <vector>
using namespace std;
#include "c3dObject.h"
class Cc3dAutoreleasePool{
public:
    static Cc3dAutoreleasePool*sharedAutoreleasePool();
    void addObject(Cc3dObject*obj){
        m_list.push_back(obj);
    }
    void releaseAll(){
        int n=(int)m_list.size();
        for(int i=0;i<n;i++){
            m_list[i]->release();
        }
        m_list.clear();
    }
protected:
    vector<Cc3dObject*> m_list;

    
};


#endif /* defined(__HelloOpenGL__c3dAutoreleasePool__) */
