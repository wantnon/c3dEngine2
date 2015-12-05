//
//  c3dAppState.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-21.
//
//

#ifndef __HelloOpenGL__c3dAppState__
#define __HelloOpenGL__c3dAppState__

#include <iostream>
using namespace std;
class Cc3dAppState{
public:
    Cc3dAppState(){
        m_isInBackGround=false;
    }
    static Cc3dAppState*sharedAppState();
    void setIsInBackGround(bool value){
        m_isInBackGround=value;
    }
    bool getIsInBackGround()const{return m_isInBackGround;}
protected:
    bool m_isInBackGround;
};
#endif /* defined(__HelloOpenGL__c3dAppState__) */
