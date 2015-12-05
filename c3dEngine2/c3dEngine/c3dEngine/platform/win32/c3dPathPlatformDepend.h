//
//  c3dPathPlatformDepend.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-9.
//
//

#ifndef __HelloOpenGL__c3dPathPlatformDepend__
#define __HelloOpenGL__c3dPathPlatformDepend__

#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include "common/c3dPathFunc.h"
string getFullPath_plat(const string&filePath);
bool isFileExistsAtPath_plat(const string&path);
bool makeDirectory_plat(string path);
#endif /* defined(__HelloOpenGL__c3dPathPlatformDepend__) */
