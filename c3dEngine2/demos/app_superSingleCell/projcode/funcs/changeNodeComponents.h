//
//  changeNodeComponents.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-23.
//
//

#ifndef __HelloOpenGL__changeNodeComponents__
#define __HelloOpenGL__changeNodeComponents__

#include <iostream>
#include <map>
using namespace std;
#include "c3d.h"
map<Cc3dNode*,Cc3dProgram*> changeProgramRecursively(Cc3dNode*node,Cc3dProgram*program);
map<Cc3dNode*,Cc3dCamera*> changeCameraRecursively(Cc3dNode*node,Cc3dCamera*camera);
map<Cc3dNode*,c3dPassUnifoCallbackPtr> changePassUnifoCallbackRecursively(Cc3dNode*node,c3dPassUnifoCallbackPtr callback);
void setPrograms( map<Cc3dNode*,Cc3dProgram*>&nodeProgramMap);
void setCameras( map<Cc3dNode*,Cc3dCamera*>&nodeCameraMap);
void setPassUnifoCallbacks( map<Cc3dNode*,c3dPassUnifoCallbackPtr>&nodeCallbackMap);

#endif /* defined(__HelloOpenGL__changeNodeComponents__) */
