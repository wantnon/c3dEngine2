//
//  c3dSceneManager.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-21.
//
//

#include "c3dSceneManger.h"
Cc3dSceneManager*s_sceneManager=NULL;
Cc3dSceneManager*Cc3dSceneManager::sharedSceneManager(){
    if(s_sceneManager==NULL){
        s_sceneManager=new Cc3dSceneManager();
    }
    return s_sceneManager;

}