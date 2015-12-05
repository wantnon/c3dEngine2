//
//  changeNodeComponents.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-23.
//
//

#include "changeNodeComponents.h"

void changeProgramRecursively_inn(Cc3dNode*node,Cc3dProgram*program,map<Cc3dNode*,Cc3dProgram*>&_map){
    if(node==NULL)return;
    Cc3dProgram*oldProgram=node->getProgram();
    if(oldProgram==NULL){
        //do nothing
    }else{
        _map[node]=oldProgram;//store old program to map
        oldProgram->retain();
        ((Cc3dNode*)node)->setProgram(program);//change program
    }
    //do same thing to children
    vector<Cc3dNode*> children=node->getChildren();
    int nchild=(int)children.size();
    for(int i=0;i<nchild;i++){
        Cc3dNode*child=children[i];
        changeProgramRecursively_inn(child, program, _map);
    }
}
map<Cc3dNode*,Cc3dProgram*> changeProgramRecursively(Cc3dNode*node,Cc3dProgram*program){
    map<Cc3dNode*,Cc3dProgram*> _map;
    changeProgramRecursively_inn(node, program, _map);
    return _map;
}



void changeCameraRecursively_inn(Cc3dNode*node,Cc3dCamera*camera,map<Cc3dNode*,Cc3dCamera*>&_map){
    if(node==NULL)return;
    Cc3dCamera*oldCamera=node->getCamera();
    if(oldCamera==NULL){
        //do nothing
    }else{
        _map[node]=oldCamera;//store old camera to map
        oldCamera->retain();
        ((Cc3dNode*)node)->setCamera(camera);//change camera
    }
    //do same thing to children
    vector<Cc3dNode*> children=node->getChildren();
    int nchild=(int)children.size();
    for(int i=0;i<nchild;i++){
        Cc3dNode*child=children[i];
        changeCameraRecursively_inn(child, camera, _map);
    }
}
map<Cc3dNode*,Cc3dCamera*> changeCameraRecursively(Cc3dNode*node,Cc3dCamera*camera){
    map<Cc3dNode*,Cc3dCamera*> _map;
    changeCameraRecursively_inn(node, camera, _map);
    return _map;
}

void changePassUnifoCallbackRecursively_inn(Cc3dNode*node,c3dPassUnifoCallbackPtr callback,map<Cc3dNode*,c3dPassUnifoCallbackPtr>&_map){
    if(node==NULL)return;
    c3dPassUnifoCallbackPtr oldCallback =node->getPassUnifoCallback();
    if(oldCallback==NULL){
        //do nothing
    }else{
        _map[node]=oldCallback;//store old callback to map
        ((Cc3dNode*)node)->setPassUnifoCallback(callback);//change callback
    }
    //do same thing to children
    vector<Cc3dNode*> children=node->getChildren();
    int nchild=(int)children.size();
    for(int i=0;i<nchild;i++){
        Cc3dNode*child=children[i];
        changePassUnifoCallbackRecursively_inn(child, callback, _map);
    }
}
map<Cc3dNode*,c3dPassUnifoCallbackPtr> changePassUnifoCallbackRecursively(Cc3dNode*node,c3dPassUnifoCallbackPtr callback){
    map<Cc3dNode*, c3dPassUnifoCallbackPtr> _map;
    changePassUnifoCallbackRecursively_inn(node, callback,_map);
    return _map;
}



void setPrograms( map<Cc3dNode*,Cc3dProgram*>& nodeProgramMap){
    map<Cc3dNode*,Cc3dProgram*>::iterator iter;
    for ( iter = nodeProgramMap.begin(); iter != nodeProgramMap.end(); iter++ ){
        Cc3dNode*node=iter->first;
        Cc3dProgram*program=iter->second;
        if(node){
            ((Cc3dNode*)node)->setProgram(program);
            program->release();
        }
        
    }
    
    
}
void setCameras( map<Cc3dNode*,Cc3dCamera*>&nodeCameraMap){
    map<Cc3dNode*,Cc3dCamera*>::iterator iter;
    for ( iter = nodeCameraMap.begin(); iter != nodeCameraMap.end(); iter++ ){
        Cc3dNode*node=iter->first;
        Cc3dCamera*camera=iter->second;
        if(node){
            ((Cc3dNode*)node)->setCamera(camera);
            camera->release();
        }
        
    }
}
void setPassUnifoCallbacks( map<Cc3dNode*,c3dPassUnifoCallbackPtr>&nodeCallbackMap){
    map<Cc3dNode*,c3dPassUnifoCallbackPtr>::iterator iter;
    for ( iter = nodeCallbackMap.begin(); iter != nodeCallbackMap.end(); iter++ ){
        Cc3dNode*node=iter->first;
        c3dPassUnifoCallbackPtr callback=iter->second;
        if(node){
            ((Cc3dNode*)node)->setPassUnifoCallback(callback);
        }
        
    }
}
