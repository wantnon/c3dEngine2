//
//  c3dNode.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-15.
//
//

#ifndef __HelloOpenGL__c3dNode__
#define __HelloOpenGL__c3dNode__

#include <iostream>
#include <algorithm>
using namespace std;
#include "core/c3dObject.h"
#include "core/c3dModelMatStack.h"
#include "core/c3dTransform.h"
#include "core/c3dProgram.h"
#include "core/c3dCamera.h"
#include "core/c3dLight.h"
#include "core/c3dProgramCache.h"
#include "core/c3dGlobalTimer.h"
#include "shader/c3dPassUnifoCallBack.h"
class Cc3dNode;
typedef void(*c3dPassUnifoCallbackPtr)(Cc3dNode*, Cc3dProgram*);
class Cc3dNode:public Cc3dObject{
public:
    Cc3dNode(){
        m_visitDrawOrder=0;
        m_visitUpdateOrder=0;
        m_tag=0;
        m_isVisible=true;
        m_isDoUpdate=true;
        m_father=NULL;
        m_isIgnorTransform=false;
        m_program=NULL;
        m_light=NULL;
        m_camera=NULL;
        m_transform=NULL;
        m_isRemoveOnNextFrame=false;
        m_isVisibleRecursively=true;
        m_isDoUpdateRecursively=true;
		m_lastUpdateTime=Cc3dGlobalTimer::sharedGlobalTimer()->getTimeFromStart();
        init_dft();
    }
    virtual~Cc3dNode(){
       // cout<<"析构:"<<m_name<<endl;
        if(m_program)m_program->release();
        if(m_light)m_light->release();
        if(m_camera)m_camera->release();
        if(m_transform)m_transform->release();
        //call release for all direct children
        int nchild=(int)m_childList.size();
        for(int i=0;i<nchild;i++){
            assert(m_childList[i]);
            m_childList[i]->setFather(NULL);
            m_childList[i]->release();
        }
    }

    void init_dft();
    void setLight(Cc3dLight*light){
        assert(light);
        if(m_light==NULL){
            m_light=light;
            m_light->retain();
        }else{
            m_light->release();
            m_light=light;
            m_light->retain();
        }
        
    }
    int getChildCount()const{return (int)m_childList.size();}
    Cc3dLight*getLight()const{return m_light;}
    void setCamera(Cc3dCamera*camera){
        assert(camera);
        if(m_camera==NULL){
            m_camera=camera;
            m_camera->retain();
        }else{
            m_camera->release();
            m_camera=camera;
            m_camera->retain();
        }
    }
    Cc3dCamera*getCamera()const{return m_camera;}
    void setPassUnifoCallback(c3dPassUnifoCallbackPtr callback){
        assert(callback);
        m_passUnifoCallback=callback;
    }
    c3dPassUnifoCallbackPtr getPassUnifoCallback()const{return m_passUnifoCallback;}
    void setProgram(Cc3dProgram*program){
        assert(program);
        if(m_program==NULL){
            m_program=program;
            program->retain();
        }else{
            m_program->release();
            m_program=program;
            m_program->retain();
        }
    }
    Cc3dProgram* getProgram()const{return m_program;}


    
    
    void addChild(Cc3dNode*node){
        assert(node);
        assert(node->getFather()==NULL);//node can not be added twice
        m_childList.push_back(node);
        node->retain();
        node->setFather(this);
    }
    bool isHaveChild(Cc3dNode*node){
        int index=getChildIndexInChildList(node);
        return (index!=-1);
    }
    void removeChild(Cc3dNode*node){
        int index=getChildIndexInChildList(node);
        assert(index!=-1);
        m_childList.erase(m_childList.begin()+index);
        node->setFather(NULL);
        node->release();
    }
    void removeChildOnNextFrame(Cc3dNode*node){
        int index=getChildIndexInChildList(node);
        assert(index!=-1);
        node->removeFromParentOnNextFrame();
    }
    void setIsVisible(bool isVisible){
        m_isVisible=isVisible;
    }
    void setIsVisibleRecursively(bool value){
        m_isVisibleRecursively=value;
    }
    bool getIsVisibleRecursively()const{return m_isVisibleRecursively;}
    bool getIsDoUpdateRecursively()const{return m_isDoUpdateRecursively;}
    void setIsDoUpdateRecursively(bool value){
        m_isDoUpdateRecursively=value;
    }
    void setIsDoUpdate(bool isDoUpdate){
        m_isDoUpdate=isDoUpdate;
    }
    bool getIsDoUpdate()const{return m_isDoUpdate;}
    bool getIsVisible()const{return m_isVisible;}
    void transform(){
        Cc3dModelMatStack::sharedModelMatStack()->mulMatrix(m_transform->getRTSmat());
    }
    void removeAllChild(){
        int nChild=(int)m_childList.size();
        for(int i=0;i<nChild;i++){
            removeChild(m_childList[i]);
        }
    }
    void removeAllChildOnNextFrame(){
        int nChild=(int)m_childList.size();
        for(int i=0;i<nChild;i++){
            removeChildOnNextFrame(m_childList[i]);
        }
    }

    void removeFromParent(){
        //ref to cocos2d-x's removeFromParent mechanism
        //see: http://user.qzone.qq.com/350479720/blog/1390301814
        assert(this->getFather());
        this->getFather()->removeChild(this);
        
        
    }
    void removeFromParentOnNextFrame(){
        m_isRemoveOnNextFrame=true;
    }
    int getTag()const{return m_tag;}
    void setTag(int tag){
        assert(tag>=0);
        m_tag=tag;
    }
    void setVisitDrawOrder(float visitDrawOrder){m_visitDrawOrder=visitDrawOrder;}
    float getVisitDrawOrder()const{return m_visitDrawOrder;}
    void setVisitUpdateOrder(float visitUpdateOrder){m_visitUpdateOrder=visitUpdateOrder;}
    float getVisitUpdateOrder()const{return m_visitUpdateOrder;}
    void setName(string name){m_name=name;}
    string getName()const{return m_name;}
    virtual void draw(){
    }
    virtual void update(float dt){
    }

    virtual void visitDraw();
    virtual void visitUpdate();
    virtual void visitTouchesBegan(const vector<Cc3dVector2>&points);
    virtual void visitTouchesMoved(const vector<Cc3dVector2>&points);
    virtual void visitTouchesEnded(const vector<Cc3dVector2>&points);
    virtual void touchesBegan(const vector<Cc3dVector2>&points){}
    virtual void touchesMoved(const vector<Cc3dVector2>&points){}
    virtual void touchesEnded(const vector<Cc3dVector2>&points){}
    virtual Cc3dVector4 getPos(){return getTransform()->getPos();}//chance to override
    virtual void setPos(const Cc3dVector4&pos){getTransform()->setPos(pos);}//chance to override
    virtual Cc3dMatrix4 getRotation(){return getTransform()->getRotation();}
    void setRotation(const Cc3dMatrix4&Rmat){
      /*  Cc3dTransform*transform=getTransform();
        transform->setRotation(Rmat);
        setTransform(transform);*/
        getTransform()->setRotation(Rmat);
    }
    vector<Cc3dNode*> getChildren(){return m_childList;};
    Cc3dNode* getChildByName(const string&name)const{
        int nChild=(int)m_childList.size();
        int count=0;
        Cc3dNode*node=NULL;
        for(int i=0;i<nChild;i++){
            if(m_childList[i]->getName()==name){
                node=m_childList[i];
                count++;
            }
        }
        assert(count<=1);
        return node;
    
    }
    Cc3dNode*getFather()const{return m_father;}
    void setIsIgnorTransform(bool value){m_isIgnorTransform=value;}
    bool getIsIgnorTransform()const{return m_isIgnorTransform;}
    Cc3dTransform*getTransform(){return m_transform;}
    void setTransform(Cc3dTransform*transform);
    bool getIsRemoveOnNextFrame()const{return m_isRemoveOnNextFrame;}
    void setIsRemoveOnNextFrame(bool value){m_isRemoveOnNextFrame=value;};
	void setRTSmat(const Cc3dMatrix4&RTSmat){
		assert(m_transform);
		m_transform->setRTSmat(RTSmat);
	}
protected:
    int getChildIndexInChildList(Cc3dNode*node){//return -1 if failed
        int nchild=(int)m_childList.size();
        for(int i=0;i<nchild;i++){
            if(node==m_childList[i]){
                return i;
            }
        }
        return -1;
    }
    void setFather(Cc3dNode*node){
        m_father=node;
    }
protected:
    Cc3dNode*m_father;
    vector<Cc3dNode*>m_childList;
    bool m_isVisibleRecursively;
    bool m_isDoUpdateRecursively;
    bool m_isVisible;
    bool m_isDoUpdate;
    string m_name;
    bool m_isIgnorTransform;//just ignore self's transform, but will not ignore parent's transform
    int m_tag;
    Cc3dTransform*m_transform;//transform (see decision_and_reason.h -- decision 1)
    float m_visitDrawOrder;
    float m_visitUpdateOrder;
    Cc3dProgram*m_program;
    Cc3dCamera*m_camera;
    c3dPassUnifoCallbackPtr m_passUnifoCallback;
    Cc3dLight*m_light;
    bool m_isRemoveOnNextFrame;
    double m_lastUpdateTime;//second
};
//compare function object for stable_sort
//http://www.codeproject.com/Articles/38381/STL-Sort-Comparison-Function
struct comp_smallerVisitDrawOrder
{
    inline bool operator()(const Cc3dNode*a, const Cc3dNode*b)
    {
        return a->getVisitDrawOrder()<b->getVisitDrawOrder();
    }
};
struct comp_smallervisitUpdateOrder
{
    inline bool operator()(const Cc3dNode*a, const Cc3dNode*b)
    {
        return a->getVisitDrawOrder()<b->getVisitUpdateOrder();
    }
};

#endif /* defined(__HelloOpenGL__c3dNode__) */
