//
//  life.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-1-19.
//
//

#ifndef __HelloOpenGL__life__
#define __HelloOpenGL__life__

#include <iostream>
#include "c3d.h"
using namespace std;
class Clife
{
public:
    float value;//当前值--abc
    float maxValue;//最大值--abc
    Clife(){
        maxValue=0;
        value=0;
    }
  

    void set_maxValue(float _maxValue){
        maxValue=_maxValue;
     //   value=maxValue;
    }
    void chargeFull(){//充满--abc
        value=maxValue;
    }
    bool isFull()const {
        if(value==maxValue)return true;
        else return false;
    }
    void add(float dvalue){
        value+=dvalue;
        value=min(value,maxValue);//不能超过满值--abc
    }
    void dec(float dvalue){//dvalue为value的减少量，为正数--abc
        value-=dvalue;
        value=maxf(0.0,value);//最少减到0
    }
    void decToZero(){//减为0
        value=0;
    }

    
};
#endif /* defined(__HelloOpenGL__life__) */
