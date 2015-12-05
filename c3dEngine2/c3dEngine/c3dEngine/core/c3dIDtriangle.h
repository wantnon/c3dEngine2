//
//  c3dIDtriangle.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-25.
//
//

#ifndef __HelloOpenGL__c3dIDtriangle__
#define __HelloOpenGL__c3dIDtriangle__

#include <iostream>
using namespace std;
#include "c3dDefine.h"
class Cc3dIDTriangle USED_FOR_SUBMIT_TO_VBO
{
public:
    int vID[3];
    Cc3dIDTriangle(){
        vID[0]=0;
        vID[1]=0;
        vID[2]=0;
    };
    ~Cc3dIDTriangle(){};
    Cc3dIDTriangle(int ID0,int ID1,int ID2){
        vID[0]=ID0;vID[1]=ID1;vID[2]=ID2;
    }
    void setvID(int ID0,int ID1,int ID2){
        vID[0]=ID0;
        vID[1]=ID1;
        vID[2]=ID2;
    }
};


#endif /* defined(__HelloOpenGL__c3dIDtriangle__) */
