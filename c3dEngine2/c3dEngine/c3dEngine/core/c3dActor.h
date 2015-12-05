//
//  c3dActor.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#ifndef __HelloOpenGL__c3dActor__
#define __HelloOpenGL__c3dActor__

#include <iostream>
#include "c3dModel.h"
#include "c3dAudioCache.h"
class Cc3dActor:public Cc3dNode
{
protected:
    Cc3dModel* m_model;
    vector<Cc3dALSource*> m_alSourceList;
public:
    Cc3dActor(){
        m_model=NULL;
        init_dft();
    }
    virtual~Cc3dActor(){ 
      
        int nSource=(int)m_alSourceList.size();
        for(int i=0;i<nSource;i++){
            Cc3dALSource*source=m_alSourceList[i];
            source->release();
            //cout<<"release source name:"<<source->getName()<<endl;
        }
    }
    void init_dft(){
        Cc3dModel*model=new Cc3dModel();
		model->autorelease();
        model->setName("model");
		setModel(model);
    }
    void setModel(Cc3dModel*model){
        if(m_model==NULL){
            m_model=model;
            addChild(m_model);
        }else{
            m_model->removeFromParent();
            m_model=model;
            addChild(m_model);
        }
    }
	Cc3dModel*getModel()const {return m_model;}
    void addALSource(Cc3dALSource*source){
        assert(source);
        assert(getALSourceByName(source->getName())==NULL);
        m_alSourceList.push_back(source);
        source->retain();
    }
    void stopAllALSource(){
        int nSource=(int)m_alSourceList.size();
        for(int i=0;i<nSource;i++){
            Cc3dALSource*source=m_alSourceList[i];
            source->stop();
        }
    }
    Cc3dALSource* getALSourceByName(const string&name){
        int nSource=(int)m_alSourceList.size();
        for(int i=0;i<nSource;i++){
            Cc3dALSource*source=m_alSourceList[i];
            if(source->getName()==name)return source;
        }
        return NULL;
    }
    void setLight(Cc3dLight*light){
		assert(light);
		this->Cc3dNode::setLight(light);
        m_model->setLight(light);
    }
    
    void setProgram(Cc3dProgram *program){
        assert(program);
		this->Cc3dNode::setProgram(program);
		m_model->setProgram(program);
    }
    void setPassUnifoCallback(c3dPassUnifoCallbackPtr callback){
        assert(callback);
		this->Cc3dNode::setPassUnifoCallback(callback);
        m_model->setPassUnifoCallback(callback);
        
    }
	void setIsWireMode(bool value){
		m_model->setIsWireMode(value);
	}
    void setIsDoUpdate(bool value){
		this->Cc3dNode::setIsDoUpdate(value);
        m_model->setIsDoUpdate(value);
    }
    void setIsDoUpdateRecursively(bool value){
        this->Cc3dNode::setIsDoUpdateRecursively(value);
        m_model->setIsDoUpdateRecursively(value);
    }
    void setIsVisible(bool value){
        this->Cc3dNode::setIsVisible(value);
        m_model->setIsVisible(value);
    }
    void setIsVisibleRecursively(bool value){
        this->Cc3dNode::setIsVisibleRecursively(value);
		m_model->setIsVisibleRecursively(value);
    }
	void setIsDoDepthTest(bool value){
		m_model->setIsDoDepthTest(value);
	}
	void setIsWriteDepthBuffer(bool value){
	
        m_model->setIsWriteDepthBuffer(value);
       
        
    }
	void setCullFace(Ec3dCullFace cullFace){
		m_model->setCullFace(cullFace);
	}
    void setCamera(Cc3dCamera*camera){
        
        this->Cc3dNode::setCamera(camera);
        
        m_model->setCamera(camera);
        
    }
    void addMesh(Cc3dMesh*mesh);
	Cc3dMesh* getMeshByIndex(int index){
		return m_model->getMeshByIndex(index);
	}
	int getMeshCount(){
		return m_model->getMeshCount();
	}
    void submitVertex(GLenum usage);
    void submitIndex(GLenum usage);
	void submit(GLenum usage){
		submitVertex(usage);
		submitIndex(usage);
	}
	void genVBOBuffers(){
		m_model->genVBOBuffers();
	}

};



#endif /* defined(__HelloOpenGL__c3dActor__) */
