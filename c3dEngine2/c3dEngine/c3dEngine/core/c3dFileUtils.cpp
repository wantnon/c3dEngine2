//
//  c3dFileUtils.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#include "core/c3dFileUtils.h"
#include "common/c3dPathFunc.h"
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
#include "platform/win32/c3dPathPlatformDepend.h"
#else
#include "platform/iOS/c3dPathPlatformDepend.h"
#endif
#include "core/c3dAssert.h"
static Cc3dFileUtils*s_fileUtils=NULL;
Cc3dFileUtils*Cc3dFileUtils::sharedFileUtils(){
    if(s_fileUtils==NULL){
        s_fileUtils=new Cc3dFileUtils();
    }
    return s_fileUtils;
}
string Cc3dFileUtils::getFullPath(const string&filePath){
    return getFullPath_plat(filePath);
}


bool Cc3dFileUtils::getIsFileOrClipExistUnderPath(const string&path,const string&fileOrClipName)
{
    if(path.empty()){
        assert(fileOrClipName.empty()==false);
        assert(fileOrClipName[(int)fileOrClipName.size()-1]!='/');
        string fileOrClipFullPath=getFullPath(path);
        bool isExist=isFileExistsAtPath_plat(fileOrClipFullPath);
        return isExist;
    }else{
        assert(path.empty()==false);
        assert(path[(int)path.size()-1]!='/');
        assert(fileOrClipName.empty()==false);
        assert(fileOrClipName[(int)fileOrClipName.size()-1]!='/');
        string fileOrClipFullPath=getFullPath(path)+"/"+fileOrClipName;
		//cout<<"full path:"<<fileOrClipFullPath.c_str()<<endl;
        bool isExist=isFileExistsAtPath_plat(fileOrClipFullPath);
        return isExist;
    }
}

bool Cc3dFileUtils::getIsFileOrClipExist(const string&fileOrClipPath)
{
    assert(fileOrClipPath.empty()==false);
    assert(fileOrClipPath[(int)fileOrClipPath.size()-1]!='/');
    string path,fileOrClipName;
    vector<string>rs=splitStrInTwoByLastBar(fileOrClipPath);
    path=rs[0];
    fileOrClipName=rs[1];
    return getIsFileOrClipExistUnderPath(path, fileOrClipName);
    
}

unsigned char* Cc3dFileUtils::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
{
    unsigned char * pBuffer = NULL;
    C3DASSERT(pszFileName != NULL && pSize != NULL && pszMode != NULL, "Invalid parameters.");
    *pSize = 0;
    do
    {
        // read the file from hardware
        std::string fullPath = getFullPath(pszFileName);
        FILE *fp = fopen(fullPath.c_str(), pszMode);
        if(!fp)break;
        
        fseek(fp,0,SEEK_END);
        *pSize = ftell(fp);
        fseek(fp,0,SEEK_SET);
        pBuffer = new unsigned char[*pSize];
        *pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
        fclose(fp);
    } while (0);
    
    if (! pBuffer)
    {
        std::string msg = "Get data from file(";
        msg.append(pszFileName).append(") failed!");
        
        C3DLOG("%s", msg.c_str());
    }
    return pBuffer;
}

bool Cc3dFileUtils::makeDirectory(string path){
    //Return true if the operation was successful or already exists, otherwise false

    return makeDirectory_plat(path);
}