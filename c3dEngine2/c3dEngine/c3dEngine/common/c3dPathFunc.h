//
//  c3dPathFunc.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-13.
//
//

#ifndef __HelloOpenGL__c3dPathFunc__
#define __HelloOpenGL__c3dPathFunc__

#include <iostream>
#include <vector>
using namespace std;

vector<string> splitStrInTwoByLastDotBehindLastBar(const string&str);
vector<string> splitStrInTwoByLastBar(const string&str);
string getFileNameFromFilePath(string filePath);
#endif /* defined(__HelloOpenGL__c3dPathFunc__) */
