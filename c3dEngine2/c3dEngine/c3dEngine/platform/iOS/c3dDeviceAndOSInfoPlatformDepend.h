//
//  c3dDeviceAndOSInfoPlatformDepend.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-6.
//
//

#ifndef __HelloOpenGL__c3dDeviceAndOSInfoPlatformDepend__
#define __HelloOpenGL__c3dDeviceAndOSInfoPlatformDepend__

#include <iostream>
using namespace std;
#import <mach/mach.h>
#import <mach/mach_host.h>
#include "c3dVector.h"
Cc3dVector2 getScreenSize_plat();
Cc3dVector2 getResolutionSize_plat();
void getMemStatistics_plat(int&memUsed,int&memFree,int&memTotal);
void showSystemInfo_plat();
#endif /* defined(__HelloOpenGL__c3dDeviceAndOSInfoPlatformDepend__) */
