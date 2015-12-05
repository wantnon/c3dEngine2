//
//  ground.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-4.
//
//

#include "ground.h"
#include "c3d.h"
void Cground::loadConfig(const string&fileNameWithExt){
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
    //------------------提取文件信息--abc
    {
        char buffer[1024]={0};
        //提取地形高度缩放因子--abc
        fscanf(fp, "%s",buffer);
        fscanf(fp, "%f",&m_heightScale);
        
    }
    //关闭文件--abc
    fclose(fp);
}
void Cground::loadConfig_texBlend(const string&fileNameWithExt){
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
    //------------------提取文件信息--abc
    {
        char buffer[1024]={0};
        //提取纹理缩放系数--abc
        fscanf(fp, "%s",buffer);
        fscanf(fp, "%f",&m_uvScale);
        
    }
    //关闭文件--abc
    fclose(fp);
}