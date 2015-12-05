//
//  shadowMapInfo.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-2-5.
//
//

#ifndef __HelloOpenGL__shadowMapInfo__
#define __HelloOpenGL__shadowMapInfo__

#include <iostream>
#include <string>
using namespace std;
#include "c3d.h"
class CshadowMapInfo
{
public:
    float lightCamAngle;//光源视角相机张角--abc
    CshadowMapInfo(){
        lightCamAngle=0;
    }
    void loadConfig(const string&fileNameWithExt){
        //------------------获得绝对路径--abc
        string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(fileNameWithExt);
        //打开文件--abc
        FILE  *fp=NULL;
        fp=fopen(pathName.c_str(),"r");
        if(fp==NULL)
        {
            cout<<"Open "<<pathName<<" failed!"<<endl;
            assert(false);
        }
        //------------------提取文件信息，并建立列模型列表--abc
        {
            //提取lightCamAngle
            fscanf(fp, "%f",&lightCamAngle);
        }
        //关闭文件--abc
        fclose(fp);
    }

};
#endif /* defined(__HelloOpenGL__shadowMapInfo__) */
