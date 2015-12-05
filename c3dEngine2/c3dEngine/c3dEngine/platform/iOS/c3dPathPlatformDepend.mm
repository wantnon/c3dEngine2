//
//  c3dPathPlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-9.
//
//

#include "c3dPathPlatformDepend.h"
#include "c3dDocuments.h"
//note: the getFullPath_plat function require the folderNames not contain dot character!
string getFullPath_plat(const string&filePath){
    string documentPath=Cc3dDocuments::sharedDocuments()->getDocumentsPath();
    if(filePath.substr(0,(int)documentPath.size())==documentPath){
           return filePath;
    }
   // cout<<"filePath:"<<filePath<<endl;
    string fileName;//文件名
    string ext;//后缀
    vector<string> rs=splitStrInTwoByLastDotBehindLastBar(filePath);
    fileName=rs[0];
    ////cout<<"fileName:"<<fileName<<endl;
    ext=rs[1];
    NSString *fileName_NS=[[[NSString alloc] initWithUTF8String:fileName.c_str()] autorelease];
    NSString *ext_NS=[[[NSString alloc] initWithUTF8String:ext.c_str()] autorelease];
    NSString* fullPath_NS;
    if((int)ext.size()==0){
        fullPath_NS= [[NSBundle mainBundle] pathForResource:fileName_NS ofType:nil];
    }else{
        fullPath_NS= [[NSBundle mainBundle] pathForResource:fileName_NS ofType:ext_NS];
    }
    if(fullPath_NS==nil){
        cout<<"warning: got nil fullPath, the path may be out of bundle. is it in ios Documents?"<<endl;
        assert(false);

      
    }
    string fullPath=[fullPath_NS cStringUsingEncoding:NSASCIIStringEncoding];
    //cout<<"fullPath:"<<fullPath<<endl;
    return fullPath;
}
bool isFileExistsAtPath_plat(const string&path){
    NSString *path_NS=[[[NSString alloc] initWithUTF8String:path.c_str()] autorelease];
    bool isExist =  [[NSFileManager defaultManager] fileExistsAtPath:path_NS];
    return isExist;
}

bool makeDirectory_plat(string path){
    
    //http://www.cocoachina.com/bbs/read.php?tid-123693.html
    //http://www.cocoachina.com/ask/questions/show/57422
    //http://www.cocoachina.com/bbs/read.php?tid-36048.html
    NSString *path_NS=[[[NSString alloc] initWithUTF8String:path.c_str()] autorelease];
    return [[NSFileManager defaultManager] createDirectoryAtPath:path_NS withIntermediateDirectories:YES attributes:nil error:nil];
    //Return Value YES if the operation was successful or already exists, otherwise NO
    
   /*
    - (BOOL)createDirectoryAtPath:(NSString *)path withIntermediateDirectories:(BOOL)createIntermediates attributes:(NSDictionary *)attributes error:(NSError **)error NS_AVAILABLE(10_5, 2_0);
    
    
    1，path：所要创建文件夹的路径。
    2，createIntermediates：所要创建的目录的父目录等相关的目录可能并不存在。如果想要把相关目录一起创建了，就把这个参数设成YES，否则设成NO。设成NO的话，假如父目录不存在，函数就会失败，函数返回值就是NO。
    3，attributes：这个参数就是创建目录时的一些选项设置，所有可以配置项的键可以在这里找到。一般设成nil就行了。采用默认的设置。
    4，error：函数执行错误时返回的错误对象，注意应返回的是一个地址；
*/
}

