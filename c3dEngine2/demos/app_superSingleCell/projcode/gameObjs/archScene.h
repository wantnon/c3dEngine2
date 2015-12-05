//
//  archScene.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-26.
//
//

#ifndef __HelloOpenGL__archScene__
#define __HelloOpenGL__archScene__

#include <iostream>
#include "c3d.h"
#include "extension.h"
#include "collisionWithModelAtTriLevel.h"
#include "loadInfo.h"
bool matchPrefix(const string&str,const string&strPrefix);
#define tag_collisionSubMesh 1
class CarchScene:public Cc3dActor
{
public:
    int nSubMesh;//submesh数量--abc
    float modelScale;//模型缩放比例--abc
	Coctree*m_octree;
    CarchScene(){
        nSubMesh=0;
		m_octree=NULL;
    }
    virtual ~CarchScene() {
		if(m_octree)m_octree->release();
    }
	Cc3dRange getRangeOfIDtrisWithTags(const vector<int>&tagList)const{
		return m_octree->getRangeOfIDtrisWithTags(tagList,this->getModel());
	}
	void updateVisibleIDTriList(const vector<int>&skipTagList){
		m_octree->updateVisibleIDTriList(skipTagList,this->getModel());
	}
	void submitVisibleIDTriList(){
		m_octree->submitVisibleIDTriList(this->getModel());
	}
    void loadConfig(const  string&fileNameWithExt){
        //------------------获得绝对路径--abc
        string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(fileNameWithExt);
        
        //打开文件--abc
        FILE  *fp=NULL;
        fp=fopen(pathName.c_str(),"r");
        if(fp==NULL)
        {
            cout<<"Open "<<pathName<<" failed!"<<endl;
			assert(false);
        }
        //------------------提取文件信息，并建立模型列表--abc
        {
            char buffer[1024]={0};
            //提取放缩系数--abc
            float scale;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&scale);
            assert(scale==1);
            this->modelScale=scale;
        }
        //关闭文件--abc
        fclose(fp);
    }
    void init(const string&configFilePathShort,const string&modelClipPathShort){
		m_octree=new Coctree();
		m_octree->autorelease();
		m_octree->retain();
		m_octree->init();
        //加载配置文件--abc
        {
            loadConfig(configFilePathShort);
        }
        //加载模型--abc
        {
			Cc3dMesh*mesh=new Cc3dMesh();
			mesh->autorelease();
			addMesh(mesh);
            loadInfo_mesh(this->getModel()->getMeshByIndex(0),modelClipPathShort,this->modelScale);
        }
        //将定位submesh舍去--abc
        {
            int nSubMesh=(int)this->getModel()->getMeshByIndex(0)->getSubMeshCount();
            for(int i=0;i<nSubMesh;i++){
                Cc3dSubMesh*subMesh=this->getModel()->getMeshByIndex(0)->getSubMeshByIndex(i);
                if(matchPrefix(getFileNameFromFilePath(subMesh->getTexture()->getFilePath()),"boss")||getFileNameFromFilePath(subMesh->getTexture()->getFilePath())=="myCell.png"){
                    assert(this->getModel()->getMeshByIndex(0)->isHaveChild(subMesh));
                    this->getModel()->getMeshByIndex(0)->removeSubMeshByIndex(i);
                    nSubMesh--;
                    i--;
                }
            }
        }
        //对collision submesh标记tag
        {
            int nSubMesh=(int)this->getModel()->getMeshByIndex(0)->getSubMeshCount();
            for(int i=0;i<nSubMesh;i++){
                Cc3dSubMesh*subMesh=this->getModel()->getMeshByIndex(0)->getSubMeshByIndex(i);
                if(getFileNameFromFilePath(subMesh->getTexture()->getFilePath())=="cb.png"){
                    subMesh->setTag(tag_collisionSubMesh);
                }
            }

            
        }
        //为indexVBO创建缓冲--abc
        {
            int nSubMesh=(int)this->getModel()->getMeshByIndex(0)->getSubMeshCount();
            for(int i=0;i<nSubMesh;i++){
                this->getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getIndexVBO()->genBuffers();
            }
        }
        //制作octree
        m_octree->makeOctree(this->getModel());
        //提交各submesh的顶点数据--abc
        {
            int nSubMesh=(int)this->getModel()->getMeshByIndex(0)->getSubMeshCount();
            for(int i=0;i<nSubMesh;i++){
                this->getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getIndexVBO()->submitVertex(this->getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getSubMeshData()->getvlist(), GL_STATIC_DRAW);
            }
        }


    }

    
    
    void submitIDtriAroundSphere(const Cc3dVector4&c,float R)
    //显示球体(c,R)附近透明三角面--abc
    {
		//-------------------------获得主角周围的三角形--abc
        vector<int> skipTagList;
        skipTagList.push_back(tag_collisionSubMesh);
        vector<CIDTriForOctree*> pIDtriExList=m_octree->getCollisionIDtriList(c, R, skipTagList,this->getModel());
        //将pIDtriExList填充到各submesh
        {
            //----清空各submesh的IDtriList
            int nSubMesh=(int)this->getModel()->getMeshByIndex(0)->getSubMeshCount();
            for(int i=0;i<nSubMesh;i++){
                this->getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getSubMeshData()->clearIDtriList();
            }
            //----将pIDtriExList中的三角形填充到相应的submesh的IDtriList中--abc
            int nIDtriEx=(int)pIDtriExList.size();
            for(int i=0;i<nIDtriEx;i++){
                CIDTriForOctree*pIDtriEx=pIDtriExList[i];
                int meshID=pIDtriEx->getSubMeshID();
                this->getModel()->getMeshByIndex(0)->getSubMeshByIndex(meshID)->getSubMeshData()->addIDtri(pIDtriEx->getIDtri());
            }
            //----提交各submesh的索引表--abc
            for(int i=0;i<nSubMesh;i++){
                this->getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getIndexVBO()->submitIndex(this->getModel()->getMeshByIndex(0)->getSubMeshByIndex(i)->getSubMeshData()->getIDtriList() , GL_STREAM_DRAW);
            }
        }

	}

    
    
    
    //-------------------------碰撞 ----------------------------------------
    
	

    bool collisionTest_lineSeg(const Cc3dVector4&p1,const Cc3dVector4&p2,Cc3dVector4&p,Cc3dVector4&collisionFaceNorm)
    //判断线段p1p2是否与this相交，同时得到碰撞点坐标--abc
    //前端返回是否相交--abc
    //p带会交点坐标--abc
	{
        assert(p1.w()==1);
        assert(p2.w()==1);
		//作以p1,p2中点为球心，p1p2距离的一半为半径的球体--abc
		Cc3dVector4 c=(p1+p2)*0.5;
		Cc3dVector4 p1p2=p2-p1;
		float R=getLength(p1p2)/2;
		//求球体(c,R)的碰撞三角面集--abc
        vector<int> skipTagList;
        skipTagList.push_back(tag_collisionSubMesh);
		vector<CtriangleWithNorm> triWithNormList=m_octree->getCollisionTriangleList(c,R,skipTagList,this->getModel());
		return collisionTest_lineSeg_common(triWithNormList, p1, p2, c, R, p,collisionFaceNorm);
	}
    float collisionTestWithWall_multiPoint(const Cc3dVector4&c,const float Rc,const float RH,const Cc3dVector4&up,Cc3dVector4&e_back,
                                           const vector<int>&skipTagList,const int intended_nSP)
    //以_c为球心，Rc为长半径的扁球体与*this进行多点碰撞检测（_c为世界坐标)
    //up为球体的向上方向--abc
    //若碰撞，前端返回打入深度；若不碰撞，前端返回-1
    //若发生碰撞，则e_back带回反弹方向（单位向量，世界坐标)，否则e_back无效--abc
	{
		//求球体(c,R)的碰撞三角面集--abc
		vector<CtriangleWithNorm> triWithNormList=m_octree->getCollisionTriangleList(c,Rc,skipTagList,this->getModel());
        return collisionTestWithWall_multiPoint_common(triWithNormList, c, Rc,RH, up, e_back,intended_nSP);
		
	}
    float collisionTestWithWall_singlePoint(const Cc3dVector4&c,const float Rc,const float RH,const Cc3dVector4&up,Cc3dVector4&e_back,
                                           const vector<int>&skipTagList,
                                            const int intended_nSP)
    //以_c为球心，Rc为长半径的扁球体与*this进行多点碰撞检测（_c为世界坐标)
    //up为球体的向上方向--abc
    //若碰撞，前端返回打入深度；若不碰撞，前端返回-1
    //若发生碰撞，则e_back带回反弹方向（单位向量，世界坐标)，否则e_back无效--abc
	{
		//求球体(c,R)的碰撞三角面集--abc
		vector<CtriangleWithNorm> triWithNormList=m_octree->getCollisionTriangleList(c,Rc,skipTagList,this->getModel());
        return collisionTestWithWall_singlePoint_common(triWithNormList, c, Rc,RH, up, e_back,intended_nSP);
		
	}

    float getH_floor(const Cc3dVector4&c,const float Rc,Cc3dVector4&houseFloorNorm)
    //求点c处house地板高度，并带回c处地板法向--abc
    //如果返回-inf，则houseFloorNorm为(0,1,0)
	{
		//求球体(c,R)的碰撞三角面集--abc
        vector<int> skipTagList;
        skipTagList.push_back(tag_collisionSubMesh);
		vector<CtriangleWithNorm> triWithNormList=m_octree->getCollisionTriangleList(c,Rc,skipTagList,this->getModel());
        float H;
        H=getH_floor_common(triWithNormList, c, Rc, houseFloorNorm);
		return H;
	}
   
    float getH_ceil(const Cc3dVector4&c,const float Rc,Cc3dVector4&houseCeilNorm)
    //求点c处天花板高度，并带回c处天花板法向--abc
    //如果返回inf，则houseCeilNorm为(0,-1,0)
	{
		//求球体(c,R)的碰撞三角面集--abc
        vector<int> skipTagList;
        skipTagList.push_back(tag_collisionSubMesh);
		vector<CtriangleWithNorm> triWithNormList=m_octree->getCollisionTriangleList(c,Rc,skipTagList,this->getModel());
        float H;
        H=getH_ceil_common(triWithNormList,c, Rc, houseCeilNorm);
        return H;
    }
    

};
#endif /* defined(__HelloOpenGL__archScene__) */
