//
//  c3dCommonFunc.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-16.
//
//

#ifndef __HelloOpenGL__c3dCommonFunc__
#define __HelloOpenGL__c3dCommonFunc__

#include <iostream>
#include <string>
#include <vector>
using namespace std;

string numberToStr(int n);

template<class T>
int findFirst(const vector<T>&list,const T&element)//return -1 if not found
{
    int n=(int)list.size();
    for(int i=0;i<n;i++){
        if(list[i]==element)return i;
    }
    return -1;
}


#endif /* defined(__HelloOpenGL__c3dCommonFunc__) */
