//
//  c3dPathPlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-9.
//
//

#include "c3dPathPlatformDepend.h"
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32) 
	#include "windows.h" 
	#include <direct.h>
#else
    
#endif
string getFullPath_plat(const string&filePath){
	//on windows, full path is equals to path
	return filePath;
}


bool isFileExistsAtPath_plat(const string&path){
	//char* to LPCWSTR, see: http://user.qzone.qq.com/350479720/blog/1333813760
	const int max_len=200;
	WCHAR path_LPCWSTR[max_len];
    MultiByteToWideChar(0,0,path.c_str(),-1,path_LPCWSTR,max_len);
	//judge whether path exist
	//see http://blog.csdn.net/wangjieest/article/details/7000640 
	//note, the above article has a error, the first method in this article is 'judge whether a directory is exist', not 'whether a directory or file is exist'
	//so here we have corrected it.
	//and see http://zhidao.baidu.com/link?url=E9bPjRsRuXx4NJbped99kepRZdfGCzqrgJIA8eDLELAvWHObrxAJtD-dkrJ9i413dry5btTMFfnIZQF6YcvTGq
    WIN32_FIND_DATA wfd;
    bool rValue = false;
    HANDLE hFind = FindFirstFile(path_LPCWSTR, &wfd);
    if ((hFind!=INVALID_HANDLE_VALUE))
    {
		if(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
			//is a directory
		}else{
			//is a file
		}
		rValue = true;
    }
    FindClose(hFind);
    return rValue;
}

bool makeDirectory_plat(string path){
	int rt=_mkdir(path.c_str());
	return (rt==0);
}