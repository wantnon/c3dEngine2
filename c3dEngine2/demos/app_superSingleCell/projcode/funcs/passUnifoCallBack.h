//
//  passUnifoCallBack.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-18.
//
//

#ifndef __HelloOpenGL__passUnifoCallBack__
#define __HelloOpenGL__passUnifoCallBack__

#include <iostream>
using namespace std;
#include "c3d.h"
void passUnifoCallback_diffuse_ambient_noTransf_shadowMap(Cc3dNode*node, Cc3dProgram*program);
void passUnifoCallback_diffuse_ambient_noTransf_noSelfShadow(Cc3dNode*node, Cc3dProgram*program);
void passUnifoCallback_diffuse_ambient_noSelfShadow(Cc3dNode*node, Cc3dProgram*program);
void passUnifoCallback_water(Cc3dNode*node, Cc3dProgram*program);
void passUnifoCallback_texOnly_useTexAsAlpha(Cc3dNode*node, Cc3dProgram*program);
void passUnifoCallback_diffuse_ambient_specular_noSelfShadow(Cc3dNode*node, Cc3dProgram*program);
void passUnifoCallback_texOnly_noSelfShadow(Cc3dNode*node, Cc3dProgram*program);
void passUnifoCallback_texOnly_blackTransp(Cc3dNode*node, Cc3dProgram*program);
void passUnifoCallback_texOnly_brightBlueTransp(Cc3dNode*node, Cc3dProgram*program);
void passUnifoCallback_renderDepth(Cc3dNode*node, Cc3dProgram*program);
void passUnifoCallback_noLight(Cc3dNode*node, Cc3dProgram*program);
#endif /* defined(__HelloOpenGL__passUnifoCallBack__) */
