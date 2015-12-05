//
//  c3dDeviceAndOSInfo.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-4-18.
//
//

#include "c3dDeviceAndOSInfo.h"
static Cc3dDeviceAndOSInfo*s_deviceAndOSInfo=NULL;
Cc3dDeviceAndOSInfo*Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo(){
    if(s_deviceAndOSInfo==NULL){
        s_deviceAndOSInfo=new Cc3dDeviceAndOSInfo();
    }
    return s_deviceAndOSInfo;
}

Cc3dVector2 Cc3dDeviceAndOSInfo::getScreenSize(){
    return getScreenSize_plat();
}
Cc3dVector2 Cc3dDeviceAndOSInfo::getResolutionSize(){
    return getResolutionSize_plat();
}
void Cc3dDeviceAndOSInfo::getMemStatistics(int&memUsed,int&memFree,int&memTotal){//获得内存统计数据--abc
    getMemStatistics_plat(memUsed, memFree, memTotal);
    
}
int Cc3dDeviceAndOSInfo::getSupportedMaxTextureSize(){
    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    return maxTextureSize;
}
void Cc3dDeviceAndOSInfo::showSystemInfo(){
    showSystemInfo_plat();
}

