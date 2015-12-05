//
//  c3dCommonFunc.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-16.
//
//

#include "c3dCommonFunc.h"
string numberToStr(int n){//整数转string
    char numbuf[128];
    sprintf(numbuf, "%i", n);//xcode中没有itoa
    string numStr=numbuf;
    return numStr;
}
