#ifndef _c3dSkinActor__
#define _c3dSkinActor__
#include<vector>
#include<string>
using namespace std;
#include "core/c3dActor.h"

static char tCharBuffer[1024]={0};
static void fskipOneStr(FILE * fp,bool valueOnly){
	assert(fp);
	if(valueOnly==false){
		fscanf(fp,"%s",tCharBuffer);
	}
}
static void fprintKey(FILE * fp,bool valueOnly,const string&key){
	assert(fp);
	if(valueOnly==false){
		fprintf(fp,"%s",key.c_str());
	}
}
static Cc3dVector4 fscanVector4(FILE * fp){
	assert(fp);
	float arr[4];
	for(int i=0;i<4;i++){
		fscanf(fp,"%f",&arr[i]);
	}
	return Cc3dVector4(arr);
}
class Cc3dAniFrame
{
protected:
	Cc3dMatrix4 m_transformMat;
	float m_time;
public:
	Cc3dAniFrame(){
		m_time=0;
	}
	virtual ~Cc3dAniFrame(){
	
	}

	Cc3dAniFrame(const Cc3dMatrix4&transformMat,float time){
		m_transformMat=transformMat;
		m_time=time;
	}

	void setVertexTransformMat(const Cc3dMatrix4&transformMat){m_transformMat=transformMat;}
	Cc3dMatrix4 getTransformMat()const{return m_transformMat;}
	void setTime(float time){m_time=time;}
	float getTime()const{return m_time;}
    
};
class Cc3dAniLayer:public Cc3dObject
{
protected:
	vector<Cc3dAniFrame>  m_aniFrameList;
public:
	void addAniFrame(const Cc3dAniFrame&aniFrame){
		m_aniFrameList.push_back(aniFrame);
	}
	const Cc3dAniFrame&getAniFrameByIndex(int index){
		assert(index>=0&&index<(int)m_aniFrameList.size());
		return m_aniFrameList[index];
	}
	int getAniFrameCount()const {return (int)m_aniFrameList.size();}
	Cc3dAniFrame getAniFrameByTime(float time){
		assert(m_aniFrameList.empty()==false);
		assert(time>=m_aniFrameList[0].getTime());
		assert(time<=m_aniFrameList[(int)m_aniFrameList.size()-1].getTime());
		Cc3dAniFrame aniFrameFoe;
		Cc3dAniFrame aniFrameNxt;
		int nAniFrame=(int)m_aniFrameList.size();
		for(int i=0;i<nAniFrame;i++){
			const Cc3dAniFrame&aniFrame=m_aniFrameList[i];
			if(aniFrame.getTime()==time){
				return aniFrame;
			}else if(aniFrame.getTime()>time){
				assert(i-1>=0);
				aniFrameFoe=m_aniFrameList[i-1];
				aniFrameNxt=m_aniFrameList[i];
				//calculate interpolated aniFrame
				float timeFoe=aniFrameFoe.getTime();
				const Cc3dMatrix4&vertexTransformMatFoe=aniFrameFoe.getTransformMat();
				float timeNxt=aniFrameNxt.getTime();
				const Cc3dMatrix4&vertexTransformMatNxt=aniFrameNxt.getTransformMat();
				float weightFoe=(timeNxt-time)/(timeNxt-timeFoe);
				float weightNxt=(time-timeFoe)/(timeNxt-timeFoe);
				Cc3dMatrix4 vertexTransformMat=vertexTransformMatFoe*weightFoe+vertexTransformMatNxt*weightNxt;
				Cc3dAniFrame aniFrame(vertexTransformMat,time);
				return aniFrame;
			}
		}
		assert(false);
	}
	void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);

};
class Cc3dSkinCluster:public Cc3dObject
{
protected:
	vector<int> m_vertexIndexList;
	vector<float> m_vertexWeightList;
	vector<Cc3dAniLayer*>  m_aniLayerList;
public:
	Cc3dSkinCluster(){
	
	}
	virtual~Cc3dSkinCluster(){
		for(int i=0;i<(int)m_aniLayerList.size();i++){
			Cc3dAniLayer*aniLayer=m_aniLayerList[i];
			if(aniLayer)aniLayer->release();
		}
	}
	void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
	void addAniLayer(Cc3dAniLayer*aniLayer){
		m_aniLayerList.push_back(aniLayer);
		aniLayer->retain();
	}
	Cc3dAniLayer* getAniLayerByIndex(int index){
		assert(index>=0&&index<(int)m_aniLayerList.size());
		return m_aniLayerList[index];
		
	}
	int getAniLayerCount()const{return (int)m_aniLayerList.size();}
	void setVertexIndexList(int*vertexIndexArray,int len){
		m_vertexIndexList.clear();
		for(int i=0;i<len;i++){
			m_vertexIndexList.push_back(vertexIndexArray[i]);
		}
	}
	void setVertexWeightList(double*vertexWeightArray,int len){
		m_vertexWeightList.clear();
		for(int i=0;i<len;i++){
			m_vertexWeightList.push_back(vertexWeightArray[i]);
		}
	}
	int getVertexIndexAt(int index){
		assert(index>=0&&index<(int)m_vertexIndexList.size());
		return m_vertexIndexList[index];
	}
	float getVertexWeightAt(int index){
		assert(index>=0&&index<(int)m_vertexWeightList.size());
		return m_vertexWeightList[index];
	}
	int getVertexIndexCount()const{return (int)m_vertexIndexList.size();}
	int getVertexWeightCount()const{
		assert((int)m_vertexWeightList.size()==(int)m_vertexIndexList.size());
		return (int)m_vertexWeightList.size();
	
	}


};

class Cc3dAnimCurveFrame
{
public:
    Cc3dAnimCurveFrame(){
        m_value=0;
        m_time=0;
    }
    virtual~Cc3dAnimCurveFrame(){}
    Cc3dAnimCurveFrame(float value,float time){
        m_value=value;
        m_time=time;
    }
    float getValue()const {return m_value;}
    float getTime()const {return m_time;}
    void setValue(float value){m_value=value;}
    void setTime(float time){m_time=time;}
    
protected:
    float m_value;
    float m_time;

};

class Cc3dAnimCurve:public Cc3dObject
{
public:
    Cc3dAnimCurve(){
    
    }
    virtual~Cc3dAnimCurve(){
        
    }
    void addAniCurveFrame(const Cc3dAnimCurveFrame&aniCurveFrame){
        m_animCurveFrameList.push_back(aniCurveFrame);
    }
    const Cc3dAnimCurveFrame&getAniCurveFrameByIndex(int index){
        assert(index>=0&&index<(int)m_animCurveFrameList.size());
        return m_animCurveFrameList[index];
    }
    int getAniFrameCount()const {return (int)m_animCurveFrameList.size();}
    Cc3dAnimCurveFrame getAniCurveFrameByTime(float time){
        assert(m_animCurveFrameList.empty()==false);
        assert(time>=m_animCurveFrameList[0].getTime());
        assert(time<=m_animCurveFrameList[(int)m_animCurveFrameList.size()-1].getTime());
        Cc3dAnimCurveFrame aniFrameFoe;
        Cc3dAnimCurveFrame aniFrameNxt;
        int nAniFrame=(int)m_animCurveFrameList.size();
        for(int i=0;i<nAniFrame;i++){
            const Cc3dAnimCurveFrame&aniFrame=m_animCurveFrameList[i];
            if(aniFrame.getTime()==time){
                return aniFrame;
            }else if(aniFrame.getTime()>time){
                assert(i-1>=0);
                aniFrameFoe=m_animCurveFrameList[i-1];
                aniFrameNxt=m_animCurveFrameList[i];
                //calculate interpolated aniFrame
                float timeFoe=aniFrameFoe.getTime();
                float valueFoe=aniFrameFoe.getValue();
                float timeNxt=aniFrameNxt.getTime();
                float valueNxt=aniFrameNxt.getValue();
                float weightFoe=(timeNxt-time)/(timeNxt-timeFoe);
                float weightNxt=(time-timeFoe)/(timeNxt-timeFoe);
                float value=valueFoe*weightFoe+valueNxt*weightNxt;
                Cc3dAnimCurveFrame animCurveFrame(value,time);
                return animCurveFrame;
            }
        }
        
        assert(false);
    }

protected:
    vector<Cc3dAnimCurveFrame> m_animCurveFrameList;
    

};
class Cc3dSkinMesh;
class Cc3dTargetShape:public Cc3dObject
{
public:
    Cc3dTargetShape();
    virtual ~Cc3dTargetShape();
    void setTargetShapeName(const string&name){m_targetShapeName=name;}
    string getTargetShapeName()const{return m_targetShapeName;}
    void setTargetShapeMesh(Cc3dSkinMesh*targetShapeMesh);
    Cc3dSkinMesh*getTargetShapeMesh();
public:
    vector<Cc3dVector4> m_controlPoints;
    vector<int> m_controlPointsIndices;
protected:
    string m_targetShapeName;
    Cc3dSkinMesh* m_targetShapeMesh;
};

class Cc3dBlendShapeChannel:public Cc3dObject
{
public:
    Cc3dBlendShapeChannel(){
    }
    virtual~Cc3dBlendShapeChannel(){
        for(int i=0;i<(int)m_animCurveList.size();i++){
            if(m_animCurveList[i])m_animCurveList[i]->release();
        }
        for(int i=0;i<(int)m_targetShapeList.size();i++){
            if(m_targetShapeList[i])m_targetShapeList[i]->release();
        }
    }
    void addAnimCurve(Cc3dAnimCurve*aniCurve){
        m_animCurveList.push_back(aniCurve);
        aniCurve->retain();
        
    }
    Cc3dAnimCurve* getAnimCurveByIndex(int index){
        assert(index>=0&&index<(int)m_animCurveList.size());
        return m_animCurveList[index];
    }
    int getAnimCurveCount()const{return m_animCurveList.size();}
    
    void addTargetShape(Cc3dTargetShape*targetShape){
        m_targetShapeList.push_back(targetShape);
        targetShape->retain();
    }
    Cc3dTargetShape*getTargetShapeByIndex(int index){
        assert(index>=0&&index<(int)m_targetShapeList.size());
        return m_targetShapeList[index];
    }
    int getTargetShapeCount()const{return m_targetShapeList.size();}
  
protected:
    vector<Cc3dAnimCurve*> m_animCurveList;
    vector<Cc3dTargetShape*> m_targetShapeList;

public:
    vector<float> m_targetShapeFullWeights;
    
};

class Cc3dBlendShape:public Cc3dObject
{
protected:
    vector<Cc3dBlendShapeChannel*> m_blendShapeChannelList;
public:
    Cc3dBlendShape(){
    }
    virtual ~Cc3dBlendShape(){
        for(int i=0;i<(int)m_blendShapeChannelList.size();i++){
            if(m_blendShapeChannelList[i])m_blendShapeChannelList[i]->release();
        }
    }
    void addBlendShapeChannel(Cc3dBlendShapeChannel*blendShapeChannel){
        m_blendShapeChannelList.push_back(blendShapeChannel);
        blendShapeChannel->retain();
    }
    Cc3dBlendShapeChannel* getBlendShapeChannelByIndex(int index){
        assert(index>=0&&index<(int)m_blendShapeChannelList.size());
        return m_blendShapeChannelList[index];
    }
    int getBlendShapeChannelCount()const{return (int)m_blendShapeChannelList.size();}

    
    

};


class Cc3dSkin:public Cc3dObject
{
protected:
	int m_skinType;
	vector<Cc3dSkinCluster*> m_clusterList;
public:
	Cc3dSkin(){
		m_skinType=0;
	}
	virtual ~Cc3dSkin(){
		for(int i=0;i<(int)m_clusterList.size();i++){
			m_clusterList[i]->release();
		}
	}
	void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
	void setSkinType(int skinType){m_skinType=skinType;} 
	int getSkinType()const{return m_skinType;}
	void addCluster(Cc3dSkinCluster*cluster){
		m_clusterList.push_back(cluster);
		cluster->retain();
	}
	Cc3dSkinCluster* getClusterByIndex(int index){
		assert(index>=0&&index<(int)m_clusterList.size());
		return m_clusterList[index];
	}
	int getClusterCount(){
		return (int)m_clusterList.size();
	}

};
class _CmeshIDvID{
protected:
	int m_meshID;
	int m_vID;
public:
	_CmeshIDvID(){
		initMembers();
	}
	_CmeshIDvID(int meshID,int vID){
		init(meshID,vID);
	}
	virtual ~_CmeshIDvID(){
		
	}

	int getMeshID()const{return m_meshID;}
	int getvID()const{return m_vID;}
	void init(int meshID,int vID){
		m_meshID=meshID;
		m_vID=vID;
	}
protected:
	void initMembers(){
		m_meshID=-1;
		m_vID=-1;
	}

};
class Cc3dSkinSubMeshData:public Cc3dSubMeshData
{
public:
	void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
};
class Cc3dSkinSubMesh:public Cc3dSubMesh
{
protected:
	Cc3dSkinSubMeshData*m_subMeshData_backup;
public:
	Cc3dSkinSubMesh(){
		m_subMeshData_backup=NULL;
	}
	virtual ~Cc3dSkinSubMesh(){
		if(m_subMeshData_backup)m_subMeshData_backup->release();
	}
	void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
	void backupSubMeshData(){//call after m_subMeshData has established
		m_subMeshData_backup=new Cc3dSkinSubMeshData();
		m_subMeshData_backup->autorelease();
		m_subMeshData_backup->retain();
		//copy m_subMeshData
		*m_subMeshData_backup=*(Cc3dSkinSubMeshData*)m_subMeshData;
	}
    void restoreSubMeshData(){
        *m_subMeshData=*m_subMeshData_backup;
        
    }
	Cc3dVertex getBackupVertexByIndex(int index){
		return m_subMeshData_backup->getVertexByIndex(index); 
	}

};

class Cc3dSkinMesh:public Cc3dMesh
{
protected:
	void* m_fbxMeshPtr;//this pointer only used for loading fbx data from fbx
	Cc3dSkin*m_skin;
	vector<vector<_CmeshIDvID> > m_vertexDupList;//m_vertexDupList[meshVID] is duplicated to {(meshID,vID),(meshID,vID),...}
    vector<Cc3dAniLayer*>  m_aniLayerList;
    vector<Cc3dBlendShape*> m_blendShapeList;
public:
	Cc3dSkinMesh(){
		m_fbxMeshPtr=NULL;
		m_skin=NULL;
	}
	virtual ~Cc3dSkinMesh(){
		if(m_skin)m_skin->release();
        for(int i=0;i<(int)m_aniLayerList.size();i++){
            if(m_aniLayerList[i])m_aniLayerList[i]->release();
        }
        for(int i=0;i<(int)m_blendShapeList.size();i++){
            if(m_blendShapeList[i])m_blendShapeList[i]->release();
        }
	}
    void addAniLayer(Cc3dAniLayer*aniLayer){
        m_aniLayerList.push_back(aniLayer);
        aniLayer->retain();
    }
    Cc3dAniLayer* getAniLayerByIndex(int index){
        assert(index>=0&&index<(int)m_aniLayerList.size());
        return m_aniLayerList[index];
        
    }
    int getAniLayerCount()const{return (int)m_aniLayerList.size();}

    void addBlendShape(Cc3dBlendShape*blendShape){
        m_blendShapeList.push_back(blendShape);
        blendShape->retain();
    }
    Cc3dBlendShape* getBlendShapeByIndex(int index){
        assert(index>=0&&index<(int)m_blendShapeList.size());
        return m_blendShapeList[index];
    }
    int getBlendShapeCount()const{return (int)m_blendShapeList.size();}
    
	void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
	void setFbxMeshPtr(void*fbxMeshPtr){m_fbxMeshPtr=fbxMeshPtr;}
	void* getFbxMeshPtr(){return m_fbxMeshPtr;}

	
	void setSkin(Cc3dSkin*skin){
		if(m_skin==NULL){
			m_skin=skin;
			m_skin->retain();
		}else{
			m_skin->release();
			m_skin=skin;
			m_skin->retain();
		}
	}
	Cc3dSkin*getSkin(){return m_skin;}
	void setVertexDupList(const vector<vector<_CmeshIDvID> >&vertexDupList){
		m_vertexDupList=vertexDupList;
	}
	vector<_CmeshIDvID> getVertexDupByIndex(int index)const {
		assert(index>=0&&index<(int)m_vertexDupList.size());
		return m_vertexDupList[index];
	}
    int getVertexDupCount()const{return (int)m_vertexDupList.size();}
    void deform(int aniLayerIndex,float time);
};
class Cc3dAniLayerInfo:public Cc3dObject
{
protected:
	//all time is second
	float m_interval;
	float m_startTime;
	float m_endTime;
	float m_curTime;
public:
	Cc3dAniLayerInfo(){
		m_interval=0;
		m_startTime=0;
		m_endTime=0;
		m_curTime=0;
	}
	void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
	void setInterval(float interval){m_interval=interval;}
	void setStartTime(float startTime){m_startTime=startTime;}
	void setEndTime(float endTime){m_endTime=endTime;}
	void setCurTime(float curTime){m_curTime=curTime;}
	float getInterval()const{return m_interval;}
	float getStartTime()const{return m_startTime;}
	float getEndTime()const{return m_endTime;}
	float getCurTime()const{return m_curTime;}
	void progress(float dt){
		if((m_endTime>m_startTime)==false)return;
		m_curTime+=dt;
		if(m_curTime>m_endTime)m_curTime=0;//m_curTime-=(m_endTime-m_startTime);//wrap
	}


};
class Cc3dSkinActor:public Cc3dActor
{
protected:
	int m_aniLayerIndex;
	float m_interval;
	vector<Cc3dAniLayerInfo*> m_aniLayerInfoList;
public:
	Cc3dSkinActor(){
		m_interval=1.0f/10; //1.0/60;
		m_aniLayerIndex=0;
	}
	virtual ~Cc3dSkinActor(){
		for(int i=0;i<(int)m_aniLayerInfoList.size();i++){
			Cc3dAniLayerInfo*aniLayerInfo=m_aniLayerInfoList[i];
			if(aniLayerInfo)aniLayerInfo->release();
		}
	}
	void doExport(string filePath,bool valueOnly);
	void doImport(string filePath);
	void addAniLayerInfo(Cc3dAniLayerInfo*aniLayerInfo){
		m_aniLayerInfoList.push_back(aniLayerInfo);
		aniLayerInfo->retain();
	}
	Cc3dAniLayerInfo*getAniLayerInfoByIndex(int index){
		assert(index>=0&&index<(int)m_aniLayerInfoList.size());
		return m_aniLayerInfoList[index];
	}
    int getAniLayerInfoCount()const{return (int)m_aniLayerInfoList.size();}
	void update(float dt){
	//	cout<<"dt:"<<dt<<endl;
		if(m_interval>0){
			playAnimation(dt);
		}
		

	}
	void playAnimation(float dt){
		if((int)m_aniLayerInfoList.size()==0)return;
		//update current aniLayerInfo
		Cc3dAniLayerInfo*curAniLayerInfo=m_aniLayerInfoList[m_aniLayerIndex];
		curAniLayerInfo->progress(dt);
		//calculate animation frame index
		float time = curAniLayerInfo->getCurTime();
		//deform
		deform(m_aniLayerIndex,time);
		this->submitVertex(GL_DYNAMIC_DRAW);//only need to submitVertex (actually only pos and norm)
	}
	void setAniLayerIndex(int aniLayerIndex){m_aniLayerIndex=aniLayerIndex;}
	int getAniLayerIndex()const{return m_aniLayerIndex;}
	void setInterval(float interval){m_interval=interval;}
	float getInterval()const{return m_interval;}
    Cc3dSkinMesh* findSkinMeshByFbxMeshPtr(void*fbxMeshPtr);
    void deform(int aniLayerIndex,float time){
		int nMeshCount=this->getMeshCount();
		for(int i=0;i<nMeshCount;i++){
			((Cc3dSkinMesh*)this->getMeshByIndex(i))->deform(aniLayerIndex,time);
		}
	}

};
bool isTopologySame(const Cc3dSkinMesh*mesh1,const Cc3dSkinMesh*mesh2);
#endif