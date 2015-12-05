//
//  c3dDocumentsPlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-6.
//
//

#include "c3dDocumentsPlatformDepend.h"
string getDocumentsPath_plat(){
    NSArray*paths=NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES);
    NSString*docs_dir=[paths objectAtIndex:0];
    string pathName=[docs_dir cStringUsingEncoding:NSASCIIStringEncoding];
    return pathName;
}