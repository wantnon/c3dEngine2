//
//  c3dPassUnifoCallBack.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-18.
//
//

#ifndef __HelloOpenGL__c3dPassUnifoCallBack__
#define __HelloOpenGL__c3dPassUnifoCallBack__

#include <iostream>
using namespace std;
#include "core/c3dNode.h"
#include "core/c3dProgram.h"
class Cc3dNode;

void buildinProgramPassUnifoCallback_texColorOnly(Cc3dNode*node, Cc3dProgram*program);
void buildinProgramPassUnifoCallback_classicLighting(Cc3dNode*node, Cc3dProgram*program);

#endif /* defined(__HelloOpenGL__c3dPassUnifoCallBack__) */
