//
//  octree.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-23.
//
//

#ifndef __HelloOpenGL__octree__
#define __HelloOpenGL__octree__

#include <iostream>
#include <vector>
#include "core/c3dSubMesh.h"
#include "geometry/geoMath.h"
#include "octree/triangleWithNorm.h"
#include "core/c3dMesh.h"
#include "common/c3dCommonFunc.h"
#include "core/c3dActor.h"
class CIDTriForOctree
{
protected:
    Cc3dIDTriangle m_IDtri;
    int m_subMeshID;//指明此三角形属于哪个subMesh
    bool m_isAdded;//避免三角面重复--abc
    Cc3dVector4 m_norm;
public:
   
    
    CIDTriForOctree(){
        m_subMeshID=-1;
        m_isAdded=false;
    }
    virtual ~CIDTriForOctree(){}
    Cc3dVector4 getNorm()const{return m_norm;}
    void setNorm(Cc3dVector4 norm){m_norm=norm;}
    void setSubMeshID(int subMeshID){
        m_subMeshID=subMeshID;
    }
    int getSubMeshID()const{return m_subMeshID;};
    void setIsAdded(bool value){
        m_isAdded=value;
    }
    bool getIsAdded()const{return m_isAdded;}
    void setIDtri(const Cc3dIDTriangle&IDtri){
        m_IDtri=IDtri;
    }
    Cc3dIDTriangle getIDtri()const{return m_IDtri;}
    
};

class CocNode
{
protected:
    Cc3dRange m_range;
	vector<CIDTriForOctree*> m_pIDtriExList;//必须用指针列表,因为不同叶子的pIDtriExList可能共用一些IDtriEx,并用IDtriEx.added来避免重复加入--abc
	CocNode*m_children[8];
	bool m_isLeaf;
public:
    CocNode(){
        for(int i=0;i<8;i++){m_children[i]=NULL;}
        m_isLeaf=false;
    }
    virtual~CocNode(){}
    void clearpIDtriExListWithoutReleaseReferredElements();
    void clearpIDtriExListWithoutReleaseReferredElements_freeMem();
    void addpIDtriEx(CIDTriForOctree*pIDtriEx);
    vector<CIDTriForOctree*> getpIDTriExList()const{return m_pIDtriExList;}
    float getBoundingSphereRadius()const;
    Cc3dVector4 getCenter()const;
    int getIDtriExCount()const;
    CIDTriForOctree* getpIDtriExByIndex(int index)const;
    CocNode*getChildByIndex(int index)const;
    void setChildByIndex(int index,CocNode*node);
	bool getIsLeaf()const{return m_isLeaf;}
	void setIsLeaf(bool isLeaf){m_isLeaf=isLeaf;}
    void setRange(const Cc3dRange&range);
    Cc3dRange getRange()const{return m_range;}
};


class Coctree:public Cc3dObject
{
protected:
    CocNode*m_pRoot;//八叉树根指针--abc
    vector<CIDTriForOctree*> m_pIDtriList;//所有叶子上的IDtriEx统一索引到这个表里--abc
	vector<CocNode*> m_pVisibleLeafList;//可见叶子列表--abc
    vector<CIDTriForOctree*> m_pVisibleIDtriList;//可见三角面列表(由可见叶子列表计算得来)
    Cc3dRange m_range;
	int m_leafCount;//叶子数--abc
    int m_IDtriCount;//三角形数--abc
public:
    Coctree(){
        initMembers();
	}
    virtual ~Coctree(){
        destory();
    }
    bool init(){

        return true;
    }
    void makeOctree(Cc3dModel*model);
    Cc3dRange getRangeOfIDtris()const;
    Cc3dRange getRangeOfIDtrisWithTags(const vector<int>&tagList,Cc3dModel*model)const;
    vector<CocNode*>  getpCollisionLeafList(const Cc3dVector4&c,float R);
	vector<CtriangleWithNorm> getCollisionTriangleList(const Cc3dVector4&c,float R,const vector<int>&skipTagList,Cc3dModel*model);
    int getLeafCount();
    int getIDtriCount(Cc3dModel*model);
    void updateVisibleIDTriList(const vector<int>&skipTagList,Cc3dModel*model);
    void submitVisibleIDTriList(Cc3dModel*model);
public://temp open
    vector<CIDTriForOctree*> getCollisionIDtriList(const Cc3dVector4&c,float R,const vector<int>&skipTagList,Cc3dModel*model);
protected:
    void destory();
   
	void updateVisibleNodeList(Cc3dModel*model);
    void getLeafCount_inn(CocNode*pNode,int&leafCount);
    void getpCollisionLeafList_inn(CocNode*pNode,const Cc3dVector4&c,float R, vector<CocNode*>&pCollisionLeafList);
    void updateVisibleNodeList_inn(CocNode*pNode,Cc3dModel*model);
    vector<CIDTriForOctree*> getIDtrisWithTags(const vector<int>&tagList,Cc3dModel*model)const;
    void makeOctree_inn(CocNode*&pNode,const vector<Cc3dSubMesh*>&pmeshList);
    void deletepIDtriExListForEachNONLeafNode(CocNode*pNode);
    void initMembers();
    void destoryAllNode();
    void destoryAllNode_inn(CocNode*pNode);
	bool intersect_IDTri_AABBBox(const float v0[3],const float v1[3],const float v2[3],
                                 float xmin,float xmax,float ymin,float ymax,float zmin,float zmax);
};
#endif /* defined(__HelloOpenGL__octree__) */
