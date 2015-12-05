//
//  terrain.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//

#ifndef __HelloOpenGL__terrain__
#define __HelloOpenGL__terrain__

#include <iostream>
using namespace std;
#include <math.h>
#include "core/c3dMesh.h"
#include "core/c3dActor.h"


class Cij{
protected:
    int m_i,m_j;
public:
    Cij(){
        initMembers();
    }
    virtual~Cij(){};
    Cij(int i,int j){
        initMembers();
        init(i, j);
    }
    void init(int i,int j){
        m_i=i;
        m_j=j;
    }
    int getI()const{return m_i;}
    int getJ()const{return m_j;}
protected:
    void initMembers(){
        m_i=0;
        m_j=0;
    }
};
class CterrainBlock{
public:
	CterrainBlock(){
		initMembers();
	}
	CterrainBlock(int imin,int imax,int jmin,int jmax){
		initMembers();
		init(imin,imax,jmin,jmax);
	}
	virtual~CterrainBlock(){
	
	}
	void init(int imin,int imax,int jmin,int jmax){
		m_imin=imin;m_imax=imax;
		m_jmin=jmin;m_jmax=jmax;
	}
	int getImin()const{return m_imin;}
	int getImax()const{return m_imax;}
	int getJmin()const{return m_jmin;}
	int getJmax()const{return m_jmax;}
protected:
	int m_imin,m_imax,m_jmin,m_jmax;
	void initMembers(){
		m_imin=m_imax=m_jmin=m_jmax=0;
	}
};

typedef unsigned char       BYTE;
//bmp每个像素对应一个grid,每个grid有四个顶点--abc
//markmat每个元素对应一个顶点--abc
class Cterrain:public Cc3dActor
{
protected:

	float reso;//LOD分辨率--abc
	vector<vector<bool> > markmat;//标记数组,用来标记各区域是否被分割--abc
    vector<Cij> markedElementIndexList;//记录markmat中被置1的元素的index,便于以后归零--abc
    string m_heightMapFileName;//高度图文件名--abc
    int m_quadtreeDepth;//四叉树深度（包含根节点),全部节点数为(pow(4,depth)-1)/3
    float m_heightScale;//地形高度缩放因子--abc
    Cc3dRange m_range;//地形范围--abc
    float gridSize;//markmat上一格的x或z跨度,x跨度与z跨度相等--abc
	float m_uvScale;//纹理缩放系数,u和v的放缩系数相等,都为kuv
    vector<vector<float> > landMat;//高程数组--abc
    vector<vector<Cc3dVector4> > normalMat;//法向数组--abc
	vector<CterrainBlock> m_blockList;
public:
	Cterrain(){
        m_quadtreeDepth=9;
        gridSize=0;
		m_uvScale=1;
		reso=10;
        m_heightScale=1;
	}
    bool initWithFile(const string&fileNameWithExt,const Cc3dRect&rect,float heightScale,int depth,Cc3dTexture*texture);
    Cc3dMesh*getMesh(){
        assert((int)getModel()->getMeshCount()==1);
        return getModel()->getMeshByIndex(0);
    }
    void setUVScale(float value){m_uvScale=value;}
    float getUVScale()const{return m_uvScale;}
    void setHeightScale(float value){m_heightScale=value;}
    float getHeightScale()const{return m_heightScale;}
    const Cc3dRange&getRange()const{return m_range;}
    void updateTerrain(const Cc3dCamera&camera);
    float getHAndNormal(float x,float z,Cc3dVector4&norm)const;
    float getH(float x,float z)const ;
	float getReso(){return reso;}
	void setReso(float _reso){reso=_reso;}
protected:
    void blocksToIDtris();
    void getBlocks(int jmin,int jmax,int imin,int imax,int curDepth);
    void readLandMat();
    void fillNormalMat();
    void makeMesh();
	//-----
	vector<Cij> getDivIJsOnDnEdge(int imin,int imax,int jmin,int jmax);
	vector<Cij> getDivIJsOnDnEdge_inn(int imin,int imax,int jmin,int jmax);
	vector<Cij> getDivIJsOnUpEdge(int imin,int imax,int jmin,int jmax);
	vector<Cij> getDivIJsOnUpEdge_inn(int imin,int imax,int jmin,int jmax);
	vector<Cij> getDivIJsOnLeftEdge(int imin,int imax,int jmin,int jmax);
	vector<Cij> getDivIJsOnLeftEdge_inn(int imin,int imax,int jmin,int jmax);
	vector<Cij> getDivIJsOnRightEdge(int imin,int imax,int jmin,int jmax);
	vector<Cij> getDivIJsOnRightEdge_inn(int imin,int imax,int jmin,int jmax);
	void blocksToIDtris_complex();

};



#endif /* defined(__HelloOpenGL__terrain__) */
