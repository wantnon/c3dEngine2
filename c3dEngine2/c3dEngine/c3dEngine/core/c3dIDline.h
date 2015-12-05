//
//  c3dIDline.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-25.
//
//

#ifndef __HelloOpenGL__c3dIDline__
#define __HelloOpenGL__c3dIDline__

#include <iostream>
using namespace std;
#include "c3dDefine.h"
class Cc3dIDline USED_FOR_SUBMIT_TO_VBO
{
public:
    int vID[2];
    Cc3dIDline(){
        vID[0]=0;
        vID[1]=0;
    };
    ~Cc3dIDline(){};
    Cc3dIDline(int ID0,int ID1){
        vID[0]=ID0;vID[1]=ID1;
    }
    void setvID(int ID0,int ID1){
        vID[0]=ID0;
        vID[1]=ID1;
    }
};


#endif /* defined(__HelloOpenGL__c3dIDline__) */
