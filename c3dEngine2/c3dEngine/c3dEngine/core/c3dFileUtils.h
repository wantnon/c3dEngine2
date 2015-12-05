//
//  c3dFileUtils.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#ifndef __HelloOpenGL__c3dFileUtils__
#define __HelloOpenGL__c3dFileUtils__

#include <iostream>
#include <string>
using namespace std;
class Cc3dFileUtils{
public:
    static Cc3dFileUtils*sharedFileUtils();
    string getFullPath(const string&filePath);
    bool getIsFileOrClipExist(const string&fileOrClipPath);
	unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);
    
    bool makeDirectory(string path);

protected:
    bool getIsFileOrClipExistUnderPath(const string&path,const string&fileOrClipName);
    
};
#endif /* defined(__HelloOpenGL__c3dFileUtils__) */
