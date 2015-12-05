//
//  meteor.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-1-18.
//
//

#ifndef __HelloOpenGL__meteor__
#define __HelloOpenGL__meteor__

#include <iostream>
#include "sprite.h"
class Cmeteor:public Csprite_timing
{
public:
    float speed;
    Cmeteor(){
        speed=0;
    }
    void set_speed(float _speed){
        speed=_speed;
    }
    
};


class CmeteorSet:public CspriteSet
{
public:
    void timing(const vector<bool>&maskList){
        if((int)maskList.size()!=(int)actorList.size()){
            cout<<"error:maskList元素数不正确!"<<endl;
            assert(false);
        }
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            if(maskList[i])continue;
            Cmeteor*pmeteor=(Cmeteor*)actorList[i];
            pmeteor->timing();
            
        }
    }
        
};

#endif /* defined(__HelloOpenGL__meteor__) */
