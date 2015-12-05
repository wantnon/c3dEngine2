//
//  archScene.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-26.
//
//

#include "archScene.h"

bool matchPrefix(const string&str,const string&strPrefix)
//判断str是否以strPrefix开头--abc
{
    if(strPrefix.empty()){
        cout<<"error:prefix is empty!"<<endl;
		assert(false);
    }
    int strLen=(int)str.size();
    int prefixLen=(int)strPrefix.size();
    if(strLen<prefixLen){
        return false;
    }
    for(int i=0;i<prefixLen;i++){
        if(strPrefix[i]!=str[i]){
            return  false;
        }
    }
    return true;
    
}