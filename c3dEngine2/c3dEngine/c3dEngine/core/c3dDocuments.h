//
//  c3dDocuments.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#ifndef __HelloOpenGL__c3dDocuments__
#define __HelloOpenGL__c3dDocuments__

#include <iostream>
#include <string>
using namespace std;
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
#include "platform/win32/c3dDocumentsPlatformDepend.h"
#include "platform/win32/c3dPathPlatformDepend.h"
#else
#include "platform/iOS/c3dDocumentsPlatformDepend.h"
#include "platform/iOS/c3dPathPlatformDepend.h"
#endif
class Cc3dDocuments{
public:
    static Cc3dDocuments*sharedDocuments();
    string getDocumentsPath();
    bool getIsFileOrClipExistInDocument(const string&pathInDoc){
        string fullPath=getDocumentsPath()+"/"+pathInDoc;
        return isFileExistsAtPath_plat(fullPath);
    }
    
    
};
#endif /* defined(__HelloOpenGL__c3dDocuments__) */
