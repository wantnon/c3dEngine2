//
//  c3dMesh.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-3.
//
//

#ifndef HelloOpenGL_c3dMesh_h
#define HelloOpenGL_c3dMesh_h
#include "c3dLight.h"
#include "c3dCamera.h"
#include "c3dTransform.h"
#include "c3dIndexVBO.h"
#include "c3dSubMesh.h"
#include "c3dObject.h"
#include "c3dNode.h"
#include "c3dProgram.h"
#include "c3dLight.h"


class Cc3dMesh:public Cc3dNode
{

protected:
    vector<Cc3dSubMesh*> m_subMeshList;//网格列表--abc
    
public:
    Cc3dMesh(){

    }
    virtual ~Cc3dMesh(){
        
    }
    int getSubMeshCount()const{return (int)m_subMeshList.size();}
    Cc3dSubMesh*getSubMeshByIndex(int index)const{
        assert(index>=0&&index<(int)m_subMeshList.size());
        return m_subMeshList[index];
    }
    vector<Cc3dSubMesh*> getSubMeshList()const{return m_subMeshList;}
    void removeSubMeshByIndex(int index){
        assert(index>=0&&index<(int)m_subMeshList.size());
        m_subMeshList[index]->removeFromParent();
        m_subMeshList.erase(m_subMeshList.begin()+index);
    }
	Cc3dSubMesh* getSubMeshByIndex(int index){
		assert(index>=0&&index<(int)m_subMeshList.size());
		return m_subMeshList[index];
	}
    Cc3dRange getRange()const ;
    void scale_modifyVertexDirectly(float kx,float ky,float kz);


    
    void genVBOBuffers();
  
	void setIsWireMode(bool isWireMode){
		int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*subMesh=m_subMeshList[i];
            if(subMesh==NULL)continue;
            subMesh->setIsWireMode(isWireMode);
        }
	}
    void setCullFace(Ec3dCullFace cullFace){
        int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*psubMesh=m_subMeshList[i];
            if(psubMesh==NULL)continue;
            psubMesh->setCullFace(cullFace);
        }
    }
    void setIsDoDepthTest(bool isDoDepthTest){
        int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*psubMesh=m_subMeshList[i];
            if(psubMesh==NULL)continue;
            psubMesh->setIsDoDepthTest(isDoDepthTest);
        }
    }
    void setIsWriteDepthBuffer(bool value){
        int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*psubMesh=m_subMeshList[i];
            if(psubMesh==NULL)continue;
            psubMesh->setIsWriteDepthBuffer(value);
        }
        
    }
	
    void setCamera(Cc3dCamera*camera){
        
        this->Cc3dNode::setCamera(camera);
        
        int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*psubMesh=m_subMeshList[i];
            if(psubMesh==NULL)continue;
            psubMesh->setCamera(camera);
        }
    
    }
    void setProgram(Cc3dProgram *program){
        assert(program);
        this->Cc3dNode::setProgram(program);
        
        int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*psubMesh=m_subMeshList[i];
            if(psubMesh==NULL)continue;
            psubMesh->setProgram(program);
        }
    }
    void setPassUnifoCallback(c3dPassUnifoCallbackPtr callback){
        assert(callback);
        this->Cc3dNode::setPassUnifoCallback(callback);
        
        int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*psubMesh=m_subMeshList[i];
            if(psubMesh==NULL)continue;
            psubMesh->setPassUnifoCallback(callback);
        }
    
    }
    void setLight(Cc3dLight*light){
        
        this->Cc3dNode::setLight(light);
        
        int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*psubMesh=m_subMeshList[i];
            if(psubMesh==NULL)continue;
            psubMesh->setLight(light);
        }
    }
    void setIsDoUpdate(bool value){
        this->Cc3dNode::setIsDoUpdate(value);
        int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*psubMesh=m_subMeshList[i];
            if(psubMesh==NULL)continue;
            psubMesh->setIsDoUpdate(value);
        }
    }
    void setIsDoUpdateRecursively(bool value){
        this->Cc3dNode::setIsDoUpdateRecursively(value);
        int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*psubMesh=m_subMeshList[i];
            if(psubMesh==NULL)continue;
            psubMesh->setIsDoUpdateRecursively(value);
        }
    }
    void setIsVisible(bool value){
        this->Cc3dNode::setIsVisible(value);
        int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*psubMesh=m_subMeshList[i];
            if(psubMesh==NULL)continue;
            psubMesh->setIsVisible(value);
        }
    }
    void setIsVisibleRecursively(bool value){
        this->Cc3dNode::setIsVisibleRecursively(value);
        int nSubMesh=(int)m_subMeshList.size();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSubMesh*psubMesh=m_subMeshList[i];
            if(psubMesh==NULL)continue;
            psubMesh->setIsVisibleRecursively(value);
        }
    }
    void setColor(float r,float g,float b,float a);
    void setDiffuseAlpha(float alpha);
    void setDiffuseRGB(float r,float g,float b);
    void setAmbient(float r,float g,float b);
    void setSpecular(float r,float g,float b);
    void setShininess(float shininess);
    void setIsHighlightUntransp(bool isHighlightUntransp);
    void submit(GLenum usage);
    void submitVertex(GLenum usage);
    void submitIndex(GLenum usage);
    void addSubMesh(Cc3dSubMesh*submesh);
    void releaseIDtriList();
    void releasevList();


};




#endif
