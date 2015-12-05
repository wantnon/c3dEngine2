//
//  c3dNode.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-15.
//
//

#include "c3dNode.h"

void Cc3dNode::init_dft(){
    //default transform
    Cc3dTransform*transform=new Cc3dTransform();
    transform->autorelease();
    setTransform(transform);
    //default camera
    Cc3dCamera*camera=new Cc3dCamera();
    camera->autorelease();
    setCamera(camera);
    //default light
    Cc3dLight*light=new Cc3dLight();
    light->autorelease();
    setLight(light);
    //default passUniformCallback
    m_passUnifoCallback=buildinProgramPassUnifoCallback_texColorOnly;
   
    //default program
    Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->getProgramByName("shader_texColorOnly");
    setProgram(program);
    
}
void Cc3dNode::setTransform(Cc3dTransform*transform){
    assert(transform);
    if(m_transform==transform)return;
    if(m_transform==NULL){
        m_transform=transform;
        m_transform->retain();
    }else{
        m_transform->release();
        m_transform=transform;
        m_transform->retain();
    }
   
    
}
void Cc3dNode::visitTouchesBegan(const vector<Cc3dVector2>&points){
    this->touchesBegan(points);
    //sort children by their visitUpdateOrder
    int nchild=(int)m_childList.size();
    if(nchild!=0){
        //sort
        stable_sort(m_childList.begin(), m_childList.end(),comp_smallervisitUpdateOrder());
        for(int i=0;i<nchild;i++){
            assert(m_childList[i]);
            m_childList[i]->visitTouchesBegan(points);
        }
        
    }

}
void Cc3dNode::visitTouchesMoved(const vector<Cc3dVector2>&points){
    this->touchesMoved(points);
    //sort children by their visitUpdateOrder
    int nchild=(int)m_childList.size();
    if(nchild!=0){
        //sort
        stable_sort(m_childList.begin(), m_childList.end(),comp_smallervisitUpdateOrder());
        for(int i=0;i<nchild;i++){
            assert(m_childList[i]);
            m_childList[i]->visitTouchesMoved(points);
        }
        
    }
}
void Cc3dNode::visitTouchesEnded(const vector<Cc3dVector2>&points){
    this->touchesEnded(points);
    //sort children by their visitUpdateOrder
    int nchild=(int)m_childList.size();
    if(nchild!=0){
        //sort
        stable_sort(m_childList.begin(), m_childList.end(),comp_smallervisitUpdateOrder());
        for(int i=0;i<nchild;i++){
            assert(m_childList[i]);
            m_childList[i]->visitTouchesEnded(points);
        }
        
    }
    
}
void Cc3dNode::visitUpdate(){
    double curTime=Cc3dGlobalTimer::sharedGlobalTimer()->getTimeFromStart();
    double lastUpdateTime=m_lastUpdateTime;
    double dt=maxf(0,curTime-lastUpdateTime);
    
    if(this->getIsDoUpdateRecursively()){
        if(this->getIsDoUpdate()){
            this->update(dt);
        }
        
        //sort children by their visitUpdateOrder
        int nchild=(int)m_childList.size();
        if(nchild!=0){
            //sort
            stable_sort(m_childList.begin(), m_childList.end(),comp_smallervisitUpdateOrder());
            for(int i=0;i<nchild;i++){
                assert(m_childList[i]);
                m_childList[i]->visitUpdate();
            }
            
        }
    }

    m_lastUpdateTime=curTime;

}
void Cc3dNode::visitDraw(){
 //   cout<<"enter node: "<<m_name<<endl;
    if(this->getIsVisibleRecursively()){
        Cc3dModelMatStack::sharedModelMatStack()->pushMatrix();
       // cout<<"NAME:"<<this->getName()<<endl;
        if(!m_isIgnorTransform)this->transform();
        
        if(this->getIsVisible()){
            this->draw();
        }
        
        
        //sort children by their visitDrawOrder
        int nchild=(int)m_childList.size();
        if(nchild!=0){
            //sort
            stable_sort(m_childList.begin(), m_childList.end(),comp_smallerVisitDrawOrder());
            for(int i=0;i<nchild;i++){
                assert(m_childList[i]);
                m_childList[i]->visitDraw();
            }
            
        }
        Cc3dModelMatStack::sharedModelMatStack()->popMatrix();
    }
  //  cout<<"quit node: "<<m_name<<endl;
}

