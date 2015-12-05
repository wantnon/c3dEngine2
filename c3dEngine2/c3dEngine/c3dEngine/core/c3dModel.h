//
//  c3dModel.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#ifndef __HelloOpenGL__c3dModel__
#define __HelloOpenGL__c3dModel__

#include <iostream>
#include "c3dMesh.h"
#include "c3dAudioCache.h"
class Cc3dModel:public Cc3dNode
{
protected:
    vector<Cc3dMesh*> m_meshList;
public:
    Cc3dModel(){
        init_dft();
    }
    virtual~Cc3dModel(){
   
    }
    void init_dft(){};
    int getMeshCount()const{return (int)m_meshList.size();}
    Cc3dMesh*getMeshByIndex(int index){
        assert(index>=0&&index<(int)m_meshList.size());
        return m_meshList[index];
    }
   
    void setLight(Cc3dLight*light){
        this->Cc3dNode::setLight(light);
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            if(m_meshList[i]==NULL)continue;
            m_meshList[i]->setLight(light);
        }
    }
    
    void setProgram(Cc3dProgram *program){
        assert(program);
        this->Cc3dNode::setProgram(program);
        
        int n=(int)m_meshList.size();
        for(int i=0;i<n;i++){
            Cc3dMesh*p=m_meshList[i];
            if(p==NULL)continue;
            p->setProgram(program);
        }
    }
    void setPassUnifoCallback(c3dPassUnifoCallbackPtr callback){
        assert(callback);
        this->Cc3dNode::setPassUnifoCallback(callback);
        
        int n=(int)m_meshList.size();
        for(int i=0;i<n;i++){
            Cc3dMesh*p=m_meshList[i];
            if(p==NULL)continue;
            p->setPassUnifoCallback(callback);
        }
        
    }
    void setIsDoUpdate(bool value){
        this->Cc3dNode::setIsDoUpdate(value);
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            if(m_meshList[i]==NULL)continue;
            m_meshList[i]->setIsDoUpdate(value);
        }
    }
    void setIsDoUpdateRecursively(bool value){
        this->Cc3dNode::setIsDoUpdateRecursively(value);
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            if(m_meshList[i]==NULL)continue;
            m_meshList[i]->setIsDoUpdateRecursively(value);
        }
    }
	void setIsWireMode(bool value){
		int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            if(m_meshList[i]==NULL)continue;
            m_meshList[i]->setIsWireMode(value);
        }
	}
    void setIsVisible(bool value){
        this->Cc3dNode::setIsVisible(value);
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            if(m_meshList[i]==NULL)continue;
            m_meshList[i]->setIsVisible(value);
        }
    }
    void setIsVisibleRecursively(bool value){
        this->Cc3dNode::setIsVisibleRecursively(value);
        int nMesh=(int)m_meshList.size();
        for(int i=0;i<nMesh;i++){
            if(m_meshList[i]==NULL)continue;
            m_meshList[i]->setIsVisibleRecursively(value);
        }
    }
    void setCamera(Cc3dCamera*camera){
        
        this->Cc3dNode::setCamera(camera);
        
        int n=(int)m_meshList.size();
        for(int i=0;i<n;i++){
            Cc3dMesh*p=m_meshList[i];
            if(p==NULL)continue;
            p->setCamera(camera);
        }
        
    }
	void setIsDoDepthTest(bool value){
		int n=(int)m_meshList.size();
        for(int i=0;i<n;i++){
            Cc3dMesh*p=m_meshList[i];
            if(p==NULL)continue;
            p->setIsDoDepthTest(value);
        }
        
    }
	void setIsWriteDepthBuffer(bool value){
		int n=(int)m_meshList.size();
        for(int i=0;i<n;i++){
            Cc3dMesh*p=m_meshList[i];
            if(p==NULL)continue;
            p->setIsWriteDepthBuffer(value);
        }
        
    }
	void setCullFace(Ec3dCullFace cullFace){
		int n=(int)m_meshList.size();
        for(int i=0;i<n;i++){
            Cc3dMesh*p=m_meshList[i];
            if(p==NULL)continue;
            p->setCullFace(cullFace);
        }
        
    }
    vector<Cc3dMesh*> getMeshList()const{return m_meshList;}
    void addMesh(Cc3dMesh*mesh);
    void submitVertex(GLenum usage);
    void submitIndex(GLenum usage);
	void genVBOBuffers(){
		int n=(int)m_meshList.size();
        for(int i=0;i<n;i++){
            Cc3dMesh*p=m_meshList[i];
            if(p==NULL)continue;
            p->genVBOBuffers();
        }
	}

};



#endif /* defined(__HelloOpenGL__c3dActor__) */
