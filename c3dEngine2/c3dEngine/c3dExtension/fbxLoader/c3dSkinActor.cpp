#include "c3dSkinActor.h"
#include "fbxsdk.h"

bool isTopologySame(const Cc3dSkinMesh*mesh1,const Cc3dSkinMesh*mesh2){
    const int mesh1VertexCount = (int)mesh1->getVertexDupCount();
    const int mesh2VertexCount = (int)mesh2->getVertexDupCount();
    if(mesh1VertexCount!=mesh2VertexCount){
        return false;
    }
    const int meshVertexCount=mesh1VertexCount;
    for (int i = 0; i < meshVertexCount; i++){
        const int meshVID=i;
        const vector<_CmeshIDvID>&vertexDup1=mesh1->getVertexDupByIndex(meshVID);
        const vector<_CmeshIDvID>&vertexDup2=mesh2->getVertexDupByIndex(meshVID);
        const int vertexDup1Size=vertexDup1.size();
        const int vertexDup2Size=vertexDup2.size();
        if(vertexDup1Size!=vertexDup2Size){
            return false;
        }
        const int vertexDupSize=vertexDup1Size;
        for(int j=0;j<vertexDupSize;j++){
            const int meshID1=vertexDup1[j].getMeshID();
            const int vID1=vertexDup1[j].getvID();
            const int meshID2=vertexDup2[j].getMeshID();
            const int vID2=vertexDup2[j].getvID();
            if(meshID1!=meshID2){
                return false;
            }
            if(vID1!=vID2){
                return false;
            }
        }
    }
    return true;
}
Cc3dSkinMesh* Cc3dSkinActor::findSkinMeshByFbxMeshPtr(void*fbxMeshPtr){
    int meshCount=this->getMeshCount();
    for(int i=0;i<meshCount;i++){
        Cc3dSkinMesh*mesh=(Cc3dSkinMesh*)this->getMeshByIndex(i);
        if(mesh->getFbxMeshPtr()==fbxMeshPtr){
            return mesh;
        }
    }
    return NULL;
    
}

Cc3dTargetShape::Cc3dTargetShape(){
    m_targetShapeMesh=NULL;
}
Cc3dTargetShape::~Cc3dTargetShape(){
    if(m_targetShapeMesh)m_targetShapeMesh->release();
}
void Cc3dTargetShape::setTargetShapeMesh(Cc3dSkinMesh*targetShapeMesh){
    m_targetShapeMesh=targetShapeMesh;
    m_targetShapeMesh->retain();
}
Cc3dSkinMesh*Cc3dTargetShape::getTargetShapeMesh(){
    return m_targetShapeMesh;
}

void Cc3dSkinMesh::deform(int aniLayerIndex,float time){
    //restore subMesh data for all subMeshes
    {
        int nSubMesh=this->getSubMeshCount();
        for(int i=0;i<nSubMesh;i++){
            Cc3dSkinSubMesh*subMesh=dynamic_cast<Cc3dSkinSubMesh*>(this->getSubMeshByIndex(i));
            subMesh->restoreSubMeshData();
        }
    }
    //mesh transform animation
    if(this->getAniLayerCount()!=0)
    {
        int aniLayerCount=this->getAniLayerCount();
        assert(aniLayerIndex<aniLayerCount);
        Cc3dAniLayer*aniLayer=this->getAniLayerByIndex(aniLayerIndex);
        const Cc3dAniFrame&aniFrame=aniLayer->getAniFrameByTime(time);
        Cc3dMatrix4 meshTransformMat=aniFrame.getTransformMat();//should change vertexTransformMat to transformMat!!!!!!!!!
        this->setRTSmat(meshTransformMat);
    }
    //shape deform animation
    const bool useSlerp=false;//slerp is slow
    {
        int nBlendShape=this->getBlendShapeCount();
        for(int i=0;i<nBlendShape;i++){
            Cc3dBlendShape*blendShape=this->getBlendShapeByIndex(i);
            int nBlendShapeChannel=blendShape->getBlendShapeChannelCount();
            for(int j=0;j<nBlendShapeChannel;j++){
                Cc3dBlendShapeChannel*blendShapeChannel=blendShape->getBlendShapeChannelByIndex(j);
                if(blendShapeChannel->getAnimCurveCount()==0){//if empty channel, skip
                    continue;
                }
                Cc3dAnimCurve*animCurve=blendShapeChannel->getAnimCurveByIndex(aniLayerIndex);
                
                if(animCurve){//animCurve exist
                    const Cc3dAnimCurveFrame&animCurveFrame=animCurve->getAniCurveFrameByTime(time);
                    const float weight=animCurveFrame.getValue();
                    const vector<float>&targetShapeFullWeights=blendShapeChannel->m_targetShapeFullWeights;
                    
                    // Find the two shape indices for influence calculation according to the weight.
                    // Consider index of base geometry as -1.
                    Cc3dBlendShapeChannel*lChannel=blendShapeChannel;
                    float lWeight=weight;
                    const int lShapeCount = (int)targetShapeFullWeights.size();
                    const vector<float>&lFullWeights = targetShapeFullWeights;
                    
                    // Find out which scope the lWeight falls in.
                    int lStartIndex = -1;
                    int lEndIndex = -1;
                    for(int lShapeIndex = 0; lShapeIndex<lShapeCount; ++lShapeIndex)
                    {
                        if(lWeight > 0 && lWeight <= lFullWeights[0])
                        {
                            lEndIndex = 0;
                            break;
                        }
                        if(lWeight > lFullWeights[lShapeIndex] && lWeight < lFullWeights[lShapeIndex+1])
                        {
                            
                            lStartIndex = lShapeIndex;
                            lEndIndex = lShapeIndex + 1;
                            break;
                        }
                    }
                    
                    Cc3dTargetShape* lStartShape = NULL;
                    Cc3dTargetShape* lEndShape = NULL;
                    if(lStartIndex > -1)
                    {
                        lStartShape = lChannel->getTargetShapeByIndex(lStartIndex);
                    }
                    if(lEndIndex > -1)
                    {
                        lEndShape = lChannel->getTargetShapeByIndex(lEndIndex);
                    }
                
                    //calculate the real weight
                    if(lStartIndex == -1 && lEndShape){//The weight percentage falls between base geometry and the first target shape.
                        float lEndWeight = lFullWeights[0];
                        lWeight = (lWeight/lEndWeight) * 100;
                    }else if(lStartShape && lEndShape){//The weight percentage falls between two target shapes.
                        float lStartWeight = lFullWeights[lStartIndex];
                        float lEndWeight = lFullWeights[lEndIndex];
                        lWeight = ((lWeight-lStartWeight)/(lEndWeight-lStartWeight)) * 100;
                    }else{
                        assert(false);
                    }
                    //Actually deform each vertices
                    int meshVertexCount = (int)m_vertexDupList.size();
                    for (int i = 0; i < meshVertexCount; i++)
                    {
                        const int meshVID=i;
                        const vector<_CmeshIDvID>&vertexDup=m_vertexDupList[meshVID];
                        int nDup=(int)vertexDup.size();
                        if(nDup==0)continue;
                        
                        //deform vertexes in vertexDup
                        for(int j=0;j<nDup;j++){
                            int meshID = vertexDup[j].getMeshID();
                            int vID = vertexDup[j].getvID();
                            Cc3dVertex curVertex=((Cc3dSkinSubMesh*)this->getSubMeshByIndex(meshID))->getVertexByIndex(vID);
                            Cc3dVertex newVertex=curVertex;
                            // deform pos
                            Cc3dVector4 newPos;
                            if(j==0){
                                //deform pos
                                Cc3dVector4 startPos,endPos;
                                if(lStartIndex == -1 && lEndShape){
                                    assert(meshVID<(int)lEndShape->m_controlPoints.size());
                                    startPos=curVertex.getPos();
                                    endPos=lEndShape->m_controlPoints[meshVID];
                                }else if(lStartShape && lEndShape){
                                    assert(meshVID<(int)lStartShape->m_controlPoints.size());
                                    assert(meshVID<(int)lEndShape->m_controlPoints.size());
                                    startPos=lStartShape->m_controlPoints[meshVID];
                                    endPos=lEndShape->m_controlPoints[meshVID];
                                }else{
                                    assert(false);
                                }
                                Cc3dVector4 lInfluence_pos = (endPos - startPos) * lWeight * 0.01;
                                newPos=startPos+lInfluence_pos;
                            }else{//j!=0
                                //same as vertexDup[0]
                                int meshID0=vertexDup[0].getMeshID();
                                int vID0=vertexDup[0].getvID();
                                newPos=this->getSubMeshByIndex(meshID0)->getVertexByIndex(vID0).getPos();
                            }
                            newVertex.setPos(newPos);
                       
                            // deform norm
                            if(lEndShape->getTargetShapeMesh()){
                                Cc3dVector4 startNorm,endNorm;
                                if(lStartIndex == -1 && lEndShape){
                                    startNorm=curVertex.getNorm();
                                    endNorm=((Cc3dSkinSubMesh*)lEndShape->getTargetShapeMesh()->getSubMeshByIndex(meshID))->getVertexByIndex(vID).getNorm();
                                }else if(lStartShape && lEndShape){
                                    startNorm=((Cc3dSkinSubMesh*)lStartShape->getTargetShapeMesh()->getSubMeshByIndex(meshID))->getVertexByIndex(vID).getNorm();
                                    endNorm=((Cc3dSkinSubMesh*)lEndShape->getTargetShapeMesh()->getSubMeshByIndex(meshID))->getVertexByIndex(vID).getNorm();
                                }else{
                                    assert(false);
                                }
                                
            
                                if(useSlerp){
                                    Cc3dVector4 newNorm=slerp(startNorm,endNorm,lWeight * 0.01);
                                    newVertex.setNorm(newNorm);
                                }else{
                                    Cc3dVector4 lInfluence_norm = (endNorm - startNorm) * lWeight * 0.01;
                                    Cc3dVector4 newNorm=(startNorm+lInfluence_norm);
                                    newVertex.setNorm(newNorm);
                                }
                            }
                            // replace vertex
                            this->getSubMeshByIndex(meshID)->setVertexByIndex(vID,newVertex);
                        }
                        
                    }
 
                }
            }
        }
    }
    
    //skin deform animation
    if(m_skin){
        int meshVertexCount = (int)m_vertexDupList.size();
        
        vector<Cc3dMatrix4> deformationList;//deformation of each vertex
        deformationList.resize(meshVertexCount);
        for(int i=0;i<(int)deformationList.size();i++)deformationList[i]=zeroMat();
        vector<float> weightList;//weight of each vertex
        weightList.resize(meshVertexCount);
        for(int i=0;i<(int)weightList.size();i++)weightList[i]=0;
        
        // For all clusters, accumulate their deformation and weight
        // on each vertices and store them in deformationList and weightList.
        int clusterCount = m_skin->getClusterCount();
        for ( int clusterIndex=0; clusterIndex<clusterCount; ++clusterIndex)
        {
            Cc3dSkinCluster*cluster=m_skin->getClusterByIndex(clusterIndex);
            
            int aniLayerCount=cluster->getAniLayerCount();
            assert(aniLayerIndex<aniLayerCount);
            Cc3dAniLayer*aniLayer=cluster->getAniLayerByIndex(aniLayerIndex);
            //int aniFrameCount=aniLayer->getAniFrameCount();//not used
            const Cc3dAniFrame&aniFrame=aniLayer->getAniFrameByTime(time);
            Cc3dMatrix4 vertexTransformMat=aniFrame.getTransformMat();
            //float time=aniFrame.getTime();//not used
            int clusterVertexIndexCount=cluster->getVertexIndexCount();
            
            for (int k = 0; k < clusterVertexIndexCount; ++k)
            {
                int meshVID = cluster->getVertexIndexAt(k);
                
                // Sometimes, the mesh can have less points than at the time of the skinning
                // because a smooth operator was active when skinning but has been deactivated during export.
                if (meshVID >= meshVertexCount)
                    continue;
                
                float weight = cluster->getVertexWeightAt(k);
                
                if (weight == 0.0)continue;
                
                // Compute the influence of the link on the vertex.
                Cc3dMatrix4 influenceMat = vertexTransformMat*weight;
                {
                    // Add to the sum of the deformations on the vertex.
                    deformationList[meshVID]=deformationList[meshVID]+influenceMat;
                    
                    // Add to the sum of weights to either normalize or complete the vertex.
                    weightList[meshVID]+=weight;
                }
            }
            
        }
        //Actually deform each vertices here by information stored in deformationList and weightList
        for (int i = 0; i < meshVertexCount; i++)
        {
            const int meshVID=i;
            vector<_CmeshIDvID> vertexDup=m_vertexDupList[meshVID];
            int nDup=(int)vertexDup.size();
            if(nDup==0)continue;
            //deform vertexes in vertexDup
            for(int j=0;j<nDup;j++){
                int meshID = vertexDup[j].getMeshID();
                int vID = vertexDup[j].getvID();
                Cc3dVertex newVertex=((Cc3dSkinSubMesh*)this->getSubMeshByIndex(meshID))->getVertexByIndex(vID);
                // Deform vertex
                if (weightList[meshVID] != 0.0)
                {
                    //deform pos
                    if(j==0){
                        newVertex.setPos(deformationList[meshVID]*newVertex.getPos());
                    }else{//j!=0
                        //same as vertexDup[0]
                        int meshID0=vertexDup[0].getMeshID();
                        int vID0=vertexDup[0].getvID();
                        newVertex.setPos(this->getSubMeshByIndex(meshID0)->getVertexByIndex(vID0).getPos());
                    }
                    //deform norm
                    newVertex.setNorm(deformationList[meshVID]*newVertex.getNorm());
                    
                }
                // replace vertexDup[j] with newVertex
                this->getSubMeshByIndex(meshID)->setVertexByIndex(vID,newVertex);
            }
            
        }
    }
    
}


void Cc3dAniLayer::doExport(string filePath,bool valueOnly){
//#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	//create a directory
	if (isFolderExist||Cc3dFileUtils::sharedFileUtils()->makeDirectory(folderFullPath.c_str()) ){//success
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dAniLayer.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"w");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}
		//------------------write file
		int nAniFrame=(int)m_aniFrameList.size();
		fprintKey(fp,valueOnly,"m_aniFrameList.size");fprintf(fp," %i\n",nAniFrame);
		for(int i=0;i<nAniFrame;i++){
			const Cc3dAniFrame&aniFrame=m_aniFrameList[i];
			fprintKey(fp,valueOnly,"aniFrame\n");
			float time=aniFrame.getTime();
			fprintKey(fp,valueOnly,"  m_time");fprintf(fp," %f\n",time);
			const float *m=aniFrame.getTransformMat().getArray();
			fprintKey(fp,valueOnly,"  m_vertexTransformMat");fprintf(fp," %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
																	m[0],m[1],m[2],m[3],
																	m[4],m[5],m[6],m[7],
																	m[8],m[9],m[10],m[11],
																	m[12],m[13],m[14],m[15]);
		}
		//--------------------------close file
		fclose(fp);
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
//#endif
}
void Cc3dAniLayer::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dAniLayer.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file
		//	cout<<"hi"<<endl;
		assert(m_aniFrameList.empty());
		int t_nAniFrame;
		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&t_nAniFrame);
		if(!valueOnly)assert(string(tCharBuffer)=="m_aniFrameList.size");
		for(int i=0;i<t_nAniFrame;i++){
			//		cout<<"i="<<i<<endl;
			fskipOneStr(fp,valueOnly);
			if(!valueOnly)assert(string(tCharBuffer)=="aniFrame");
			float time;
			fskipOneStr(fp,valueOnly);
			if(!valueOnly)assert(string(tCharBuffer)=="m_time");
			fscanf(fp,"%f",&time);
			float m[16];
			fskipOneStr(fp,valueOnly);
			if(!valueOnly)assert(string(tCharBuffer)=="m_vertexTransformMat");
			for(int j=0;j<16;j++){
				fscanf(fp,"%f",&m[j]);
			}
			Cc3dMatrix4 vertexTransformMat(m);
			Cc3dAniFrame aniFrame(vertexTransformMat,time);
			m_aniFrameList.push_back(aniFrame);

		}
		//	cout<<"ha"<<endl;
		//--------------------------close file
		fclose(fp);

	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}

}
void Cc3dSkinCluster::doExport(string filePath,bool valueOnly){
//#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	//create a directory
	if (isFolderExist||Cc3dFileUtils::sharedFileUtils()->makeDirectory(folderFullPath.c_str()) ){//success
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinCluster.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"w");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}
		//------------------write file
		int nVertexIndex=(int)m_vertexIndexList.size();
		fprintKey(fp,valueOnly,"m_vertexIndexList.size");fprintf(fp," %i\n",nVertexIndex);
		for(int i=0;i<nVertexIndex;i++){
			fprintf(fp,"%i ",m_vertexIndexList[i]);
		}fprintf(fp,"\n");
		int nVertexWeight=(int)m_vertexWeightList.size();
		fprintKey(fp,valueOnly,"m_vertexWeightList.size");fprintf(fp," %i\n",nVertexWeight);
		for(int i=0;i<nVertexWeight;i++){
			fprintf(fp,"%f ",m_vertexWeightList[i]);
		}fprintf(fp,"\n");
		fprintKey(fp,valueOnly,"m_aniLayerList.size");fprintf(fp," %i",(int)m_aniLayerList.size());
		//--------------------------close file
		fclose(fp);
		//------------------m_aniLayerList
		int nAniLayer=(int)m_aniLayerList.size();
		for(int i=0;i<nAniLayer;i++){
			string numStr=numberToStr(i);
			m_aniLayerList[i]->doExport(folderPath+"/m_aniLayer_"+numStr,valueOnly);	
		}
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
//#endif

}
void Cc3dSkinCluster::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinCluster.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file
		assert(m_vertexIndexList.empty());
		int t_nVertexIndex;
		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&t_nVertexIndex);
		if(!valueOnly)assert(string(tCharBuffer)=="m_vertexIndexList.size");
		for(int i=0;i<t_nVertexIndex;i++){
			int vertexIndex;
			fscanf(fp,"%i",&vertexIndex);
			m_vertexIndexList.push_back(vertexIndex);
		}
		assert(m_vertexWeightList.empty());
		int t_nVertexWeight;
		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&t_nVertexWeight);
		if(!valueOnly)assert(string(tCharBuffer)=="m_vertexWeightList.size");
		for(int i=0;i<t_nVertexWeight;i++){
			float vertexWeight;
			fscanf(fp,"%f ",&vertexWeight);
			m_vertexWeightList.push_back(vertexWeight);
		}
		int t_nAniLayer;
		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&t_nAniLayer);
		if(!valueOnly)assert(string(tCharBuffer)=="m_aniLayerList.size");

		//--------------------------close file
		fclose(fp);
		//------------------m_aniLayerList
		assert(m_aniLayerList.empty());
		for(int i=0;i<t_nAniLayer;i++){
			string numStr=numberToStr(i);
			Cc3dAniLayer*p=new Cc3dAniLayer();
			p->autorelease();
			this->addAniLayer(p);
			p->doImport(folderPath+"/m_aniLayer_"+numStr,valueOnly);	
		}
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
}

void Cc3dSkin::doExport(string filePath,bool valueOnly){
//#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	//create a directory
	if (isFolderExist||Cc3dFileUtils::sharedFileUtils()->makeDirectory(folderFullPath.c_str()) ){//success
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkin.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"w");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}
		//------------------write file
		fprintKey(fp,valueOnly,"m_skinType");fprintf(fp," %i\n",m_skinType);
		fprintKey(fp,valueOnly,"m_clusterList.size");fprintf(fp," %i",(int)m_clusterList.size());
		//--------------------------close file
		fclose(fp);
		//------------------m_clusterList
		int nCluster=(int)m_clusterList.size();
		for(int i=0;i<nCluster;i++){
			string numStr=numberToStr(i);
			m_clusterList[i]->doExport(folderPath+"/m_skinCluster_"+numStr,valueOnly);	
		}
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
//#endif
}
void Cc3dSkin::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkin.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&m_skinType);
		if(!valueOnly)assert(string(tCharBuffer)=="m_skinType");

		fskipOneStr(fp,valueOnly);
		int t_nCluster;
		fscanf(fp,"%i",&t_nCluster);
		if(!valueOnly)assert(string(tCharBuffer)=="m_clusterList.size");

		//--------------------------close file
		fclose(fp);
		//------------------m_clusterList
		assert(m_clusterList.empty());
		for(int i=0;i<t_nCluster;i++){
			string numStr=numberToStr(i);
			Cc3dSkinCluster* cluster=new Cc3dSkinCluster();
			cluster->autorelease();
			addCluster(cluster);
			cluster->doImport(folderPath+"/m_skinCluster_"+numStr,valueOnly);	
		}
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
}
void Cc3dSkinSubMeshData::doExport(string filePath,bool valueOnly){
//#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	//create a directory
	if (isFolderExist||Cc3dFileUtils::sharedFileUtils()->makeDirectory(folderFullPath.c_str()) ){//success
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinSubMeshData.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"w");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}
		//------------------write file	
		int nv=(int)vlist.size();
		fprintKey(fp,valueOnly,"m_vlist.size");fprintf(fp," %i\n",nv);
		for(int i=0;i<nv;i++){
			const Cc3dVertex&vertex=vlist[i];
			fprintKey(fp,valueOnly,"vertex\n");
			const Cc3dVector4&pos=vertex.getPos();
			const Cc3dVector2&texCoord=vertex.getTexCoord();
			const Cc3dVector4&norm=vertex.getNorm();
			const Cc3dVector4&color=vertex.getColor();
			fprintKey(fp,valueOnly,"  pos");fprintf(fp," %f %f %f\n",pos.x(),pos.y(),pos.z());
			fprintKey(fp,valueOnly,"  texCoord");fprintf(fp," %f %f\n",texCoord.x(),texCoord.y());
			fprintKey(fp,valueOnly,"  norm");fprintf(fp," %f %f %f\n",norm.x(),norm.y(),norm.z());
			fprintKey(fp,valueOnly,"  color");fprintf(fp," %f %f %f %f\n",color.x(),color.y(),color.z(),color.w());
		}
		int nIDtri=(int)IDtriList.size();
		fprintKey(fp,valueOnly,"m_IDtriList.size");fprintf(fp," %i\n",nIDtri);
		for(int i=0;i<nIDtri;i++){
			const Cc3dIDTriangle&IDtri=IDtriList[i];
			fprintKey(fp,valueOnly,"IDtri");fprintf(fp," %i %i %i\n",IDtri.vID[0],IDtri.vID[1],IDtri.vID[2]);
		}

		//--------------------------close file
		fclose(fp);
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
	}
//#endif
}
void Cc3dSkinSubMeshData::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinSubMeshData.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file
		//vlist
		assert(vlist.empty());
		int t_nv;
		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="m_vlist.size");
		fscanf(fp,"%i",&t_nv);
		for(int i=0;i<t_nv;i++){
			fskipOneStr(fp,valueOnly);
			if(!valueOnly)assert(string(tCharBuffer)=="vertex");
			Cc3dVector4 pos;
			Cc3dVector2 texCoord;
			Cc3dVector4 norm;
			Cc3dVector4 color;
			float posx,posy,posz;
			float s,t;
			float normx,normy,normz;
			float r,g,b,a;
			fskipOneStr(fp,valueOnly);
			fscanf(fp,"%f",&posx);
			fscanf(fp,"%f",&posy);
			fscanf(fp,"%f",&posz);
			fskipOneStr(fp,valueOnly);
			fscanf(fp,"%f",&s);
			fscanf(fp,"%f",&t);
			fskipOneStr(fp,valueOnly);
			fscanf(fp,"%f",&normx);
			fscanf(fp,"%f",&normy);
			fscanf(fp,"%f",&normz);
			fskipOneStr(fp,valueOnly);
			fscanf(fp,"%f",&r);
			fscanf(fp,"%f",&g);
			fscanf(fp,"%f",&b);
			fscanf(fp,"%f",&a);
			pos.init(posx,posy,posz,1);
			texCoord.init(s,t);
			norm.init(normx,normy,normz,0);
			color.init(r,g,b,a);
			Cc3dVertex vertex(pos,texCoord,norm,color);
			vlist.push_back(vertex);
		}
		assert(IDtriList.empty());
		int t_nIDtri;
		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="m_IDtriList.size");
		fscanf(fp,"%i",&t_nIDtri);
		for(int i=0;i<t_nIDtri;i++){
			fskipOneStr(fp,valueOnly);
			if(!valueOnly)assert(string(tCharBuffer)=="IDtri");
			int vID0,vID1,vID2;
			fscanf(fp,"%i",&vID0);
			fscanf(fp,"%i",&vID1);
			fscanf(fp,"%i",&vID2);
			Cc3dIDTriangle IDtri(vID0,vID1,vID2);
			IDtriList.push_back(IDtri);

		}

		//--------------------------close file
		fclose(fp);
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
}


void Cc3dSkinSubMesh::doExport(string filePath,bool valueOnly){
//#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	//create a directory
	if (isFolderExist||Cc3dFileUtils::sharedFileUtils()->makeDirectory(folderFullPath.c_str()) ){//success
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinSubMesh.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"w");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}
		//------------------write file
		string texFilePath=this->getTexture()->getFilePath();
		fprintKey(fp,valueOnly,"texFilePath");fprintf(fp," %s\n",texFilePath.c_str());
		Cc3dVector4 ambient=this->getMaterial()->getAmbient();
		Cc3dVector4 diffuse=this->getMaterial()->getDiffuse();
		Cc3dVector4 specular=this->getMaterial()->getSpecular();
		float shininess=this->getMaterial()->getShininess();

		fprintKey(fp,valueOnly,"ambient");fprintf(fp," %f %f %f %f\n",ambient.x(),ambient.y(),ambient.z(),ambient.w());
		fprintKey(fp,valueOnly,"diffuse");fprintf(fp," %f %f %f %f\n",diffuse.x(),diffuse.y(),diffuse.z(),diffuse.w());
		fprintKey(fp,valueOnly,"specular");fprintf(fp," %f %f %f %f\n",specular.x(),specular.y(),specular.z(),specular.w());
		fprintKey(fp,valueOnly,"shininess");fprintf(fp," %f\n",shininess);

		//--------------------------close file
		fclose(fp);
		//-----------------m_subMeshData_backup
		m_subMeshData_backup->doExport(folderPath+"/m_subMeshData_backup",valueOnly);
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
//#endif
}
void Cc3dSkinSubMesh::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinSubMesh.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file
		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="texFilePath");
		char t_texFilePath[1024];
		fscanf(fp,"%s",t_texFilePath);
		string texFilePath=string(t_texFilePath);
		Cc3dTexture*texture=Cc3dTextureCache::sharedTextureCache()->addImage(texFilePath.c_str());
		this->setTexture(texture);

		Cc3dVector4 ambient;
		Cc3dVector4 diffuse;
		Cc3dVector4 specular;
		float shininess;

		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="ambient");
		ambient=fscanVector4(fp);

		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="diffuse");
		diffuse=fscanVector4(fp);

		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="specular");
		specular=fscanVector4(fp);

		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="shininess");
		fscanf(fp,"%f",&shininess);

		Cc3dMaterial*material=new Cc3dMaterial();
		material->autorelease();
		material->setAmbient(ambient);
		material->setDiffuse(diffuse);
		material->setSpecular(specular);
		material->setShininess(shininess);
		this->setMaterial(material);

		//--------------------------close file
		fclose(fp);
		//------------------m_subMeshData_backup
		assert(m_subMeshData_backup==NULL);
		m_subMeshData_backup=new Cc3dSkinSubMeshData();
		m_subMeshData_backup->autorelease();
		m_subMeshData_backup->retain();
		m_subMeshData_backup->doImport(folderPath+"/m_subMeshData_backup",valueOnly);
		//------------------m_subMeshData
		Cc3dSkinSubMeshData* subMeshData=new Cc3dSkinSubMeshData();
		subMeshData->autorelease();
		this->setMeshData(subMeshData);
		*subMeshData=*m_subMeshData_backup;
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
}


void Cc3dSkinMesh::doExport(string filePath,bool valueOnly){
//#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	//create a directory
	if (isFolderExist||Cc3dFileUtils::sharedFileUtils()->makeDirectory(folderFullPath.c_str()) ){//success
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinMesh.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"w");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}
		//------------------write file
		fprintKey(fp,valueOnly,"m_skinSubMesh.size");fprintf(fp," %i\n",this->getSubMeshCount());
        if(m_skin){
            fprintKey(fp,valueOnly,"m_vertexDupList.size");fprintf(fp," %i\n",(int)m_vertexDupList.size());
            int nVertexDup=(int)m_vertexDupList.size();
            for(int i=0;i<nVertexDup;i++){
                int nMeshIDvID=(int)m_vertexDupList[i].size();
                fprintKey(fp,valueOnly,"nMeshIDvID");fprintf(fp," %i\n",nMeshIDvID);
                for(int j=0;j<nMeshIDvID;j++){
                    const _CmeshIDvID&meshIDvID=m_vertexDupList[i][j];
                    fprintKey(fp,valueOnly,"  meshIDvID");fprintf(fp," %i %i\n",meshIDvID.getMeshID(),meshIDvID.getvID());
                }
            }
        }else{//if there is no skin, no need to export m_vertexDupList, just print "m_vertexDupList.size 0"
            fprintKey(fp,valueOnly,"m_vertexDupList.size");fprintf(fp," %i\n",0);
        }
		//RTmat and Scale
		Cc3dMatrix4 RTmat=this->getTransform()->getRTmat();
		const float *m=RTmat.getArray();
		fprintKey(fp,valueOnly,"RTmat");fprintf(fp," %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			m[0],m[1],m[2],m[3],
			m[4],m[5],m[6],m[7],
			m[8],m[9],m[10],m[11],
			m[12],m[13],m[14],m[15]);
		float scaleX,scaleY,scaleZ;
		scaleX=this->getTransform()->getScaleX();
		scaleY=this->getTransform()->getScaleY();
		scaleZ=this->getTransform()->getScaleZ();
		fprintKey(fp,valueOnly,"scale");fprintf(fp," %f %f %f",scaleX,scaleY,scaleZ);fprintf(fp,"\n");
        fprintKey(fp,valueOnly,"m_aniLayerList.size");fprintf(fp," %i",(int)m_aniLayerList.size());
		//--------------------------close file
		fclose(fp);
		//------------------m_skin
        if(m_skin){
            m_skin->doExport(folderPath+"/m_skin",valueOnly);
        }
		//------------------skinSubMeshes
		int nSkinSubMesh=this->getSubMeshCount();
		for(int i=0;i<nSkinSubMesh;i++){
			Cc3dSkinSubMesh*skinSubMesh=(Cc3dSkinSubMesh*)this->getSubMeshByIndex(i);
			string numStr=numberToStr(i);
			skinSubMesh->doExport(folderPath+"/m_skinSubMesh_"+numStr,valueOnly);
		}
        //------------------m_aniLayerList
        int nAniLayer=(int)m_aniLayerList.size();
        for(int i=0;i<nAniLayer;i++){
            string numStr=numberToStr(i);
            m_aniLayerList[i]->doExport(folderPath+"/m_aniLayer_"+numStr,valueOnly);
        }

	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
//#endif
}
void Cc3dSkinMesh::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinMesh.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}
		//------------------read file

		fskipOneStr(fp,valueOnly);
		int t_nSkinSubMesh;
		fscanf(fp,"%i",&t_nSkinSubMesh);
		if(!valueOnly)assert(string(tCharBuffer)=="m_skinSubMesh.size");

		fskipOneStr(fp,valueOnly);
		int t_nVertexDup;
		fscanf(fp,"%i",&t_nVertexDup);
		if(!valueOnly)assert(string(tCharBuffer)=="m_vertexDupList.size");
		//m_vertexDupList
		assert(m_vertexDupList.empty());
		for(int i=0;i<t_nVertexDup;i++){
			vector<_CmeshIDvID> meshIDvIDList;
			int t_nMeshIDvID;
			fskipOneStr(fp,valueOnly);
			fscanf(fp,"%i",&t_nMeshIDvID);
			if(!valueOnly)assert(string(tCharBuffer)=="nMeshIDvID");
			for(int j=0;j<t_nMeshIDvID;j++){
				_CmeshIDvID meshIDvID;
				int meshID,vID;
				fskipOneStr(fp,valueOnly);
				fscanf(fp,"%i",&meshID);
				fscanf(fp,"%i",&vID);
				if(!valueOnly)assert(string(tCharBuffer)=="meshIDvID");
				meshIDvID.init(meshID,vID);
				meshIDvIDList.push_back(meshIDvID);
			}
			m_vertexDupList.push_back(meshIDvIDList);
		}
		//RTmat and Scale
		float m[16];
		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="RTmat");
		for(int j=0;j<16;j++){
			fscanf(fp,"%f",&m[j]);
		}
		Cc3dMatrix4 RTmat(m);
		float scaleX,scaleY,scaleZ;
		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="scale");
		fscanf(fp,"%f",&scaleX);
		fscanf(fp,"%f",&scaleY);
		fscanf(fp,"%f",&scaleZ);
		Cc3dTransform*transform=new Cc3dTransform();
        transform->autorelease();
		transform->init(RTmat,scaleX,scaleY,scaleZ);
		this->setTransform(transform);
        //aniLayer count
        int t_nAniLayer;
        fskipOneStr(fp,valueOnly);
        fscanf(fp,"%i",&t_nAniLayer);
        if(!valueOnly)assert(string(tCharBuffer)=="m_aniLayerList.size");
        //meshName
        fskipOneStr(fp,valueOnly);
        char t_str[1024];
        fscanf(fp,"%s",t_str);
        this->setName(t_str);
        if(!valueOnly)assert(string(tCharBuffer)=="meshName");
		//--------------------------close file
		fclose(fp);
		//------------------m_skin
        if(Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath+"/m_skin")){
            Cc3dSkin*skin=new Cc3dSkin();
            skin->autorelease();
            setSkin(skin);
            skin->doImport(folderPath+"/m_skin",valueOnly);
        }
		//------------------skinSubMeshes
		assert(this->getSubMeshCount()==0);
		for(int i=0;i<t_nSkinSubMesh;i++){
			string numStr=numberToStr(i);
			Cc3dSkinSubMesh*p=new Cc3dSkinSubMesh();
			p->autorelease();
			this->addSubMesh(p);
			p->doImport(folderPath+"/m_skinSubMesh_"+numStr,valueOnly);
		}
        //------------------m_aniLayerList
        assert(m_aniLayerList.empty());
        for(int i=0;i<t_nAniLayer;i++){
            string numStr=numberToStr(i);
            Cc3dAniLayer*p=new Cc3dAniLayer();
            p->autorelease();
            this->addAniLayer(p);
            p->doImport(folderPath+"/m_aniLayer_"+numStr,valueOnly);
        }
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
}
void Cc3dAniLayerInfo::doExport(string filePath,bool valueOnly){
//#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	//create a directory
  
	if (isFolderExist||Cc3dFileUtils::sharedFileUtils()->makeDirectory(folderFullPath.c_str()) ){//success
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dAniLayerInfo.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"w");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}
		//------------------write file
		{
			fprintKey(fp,valueOnly,"m_interval");fprintf(fp," %f\n",m_interval);
			fprintKey(fp,valueOnly,"m_startTime");fprintf(fp," %f\n",m_startTime);
			fprintKey(fp,valueOnly,"m_endTime");fprintf(fp," %f\n",m_endTime);
		}
		//--------------------------close file
		fclose(fp);
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
//#endif
}
void Cc3dAniLayerInfo::doImport(string filePath,bool valueOnly){

	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dAniLayerInfo.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%f",&m_interval);
		if(!valueOnly)assert(string(tCharBuffer)=="m_interval");

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%f",&m_startTime);
		if(!valueOnly)assert(string(tCharBuffer)=="m_startTime");

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%f",&m_endTime);
		if(!valueOnly)assert(string(tCharBuffer)=="m_endTime");


		m_curTime=0;

		//--------------------------close file
		fclose(fp);
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}

}
void Cc3dSkinActor::doExport(string filePath,bool valueOnly){
//#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	//create a directory
	if (isFolderExist||Cc3dFileUtils::sharedFileUtils()->makeDirectory(folderFullPath.c_str()) ){//success
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinActor.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"w");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}
		//------------------write file
		{
			if(valueOnly){
				fprintf(fp,"%s","valueOnly\n");
			}else{
				fprintf(fp,"%s","notValueOnly\n");
		
			}
		
			fprintKey(fp,valueOnly,"m_aniLayerIndex");
			fprintf(fp," %i\n",m_aniLayerIndex);
			fprintKey(fp,valueOnly,"m_interval");fprintf(fp," %f\n",m_interval);
			fprintKey(fp,valueOnly,"m_aniLayerInfoList.size");fprintf(fp," %i\n",(int)m_aniLayerInfoList.size());
			fprintKey(fp,valueOnly,"m_skinMeshList.size");fprintf(fp," %i\n",(int)this->getMeshCount());
		}
		//--------------------------close file
		fclose(fp);
		//------------------m_aniLayerInfoList
		int nAniLayerInfo=(int)m_aniLayerInfoList.size();
		for(int i=0;i<nAniLayerInfo;i++){
			string numStr=numberToStr(i);
			m_aniLayerInfoList[i]->doExport(folderPath+"/m_aniLayerInfo_"+numStr,valueOnly);
		}
		//------------------skinMeshes
		int meshCount=this->getMeshCount();
		for(int i=0;i<meshCount;i++){
			string numStr=numberToStr(i);
			Cc3dSkinMesh*mesh=(Cc3dSkinMesh*)this->getMeshByIndex(i);
			mesh->doExport(folderPath+"/m_skinMesh_"+numStr,valueOnly);
		}
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
//#endif
}
void Cc3dSkinActor::doImport(string filePath){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
	bool isFolderExist=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
	string folderFullPath=Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
	if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinActor.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file
		bool valueOnly;

		char t_str[1024];
		fscanf(fp,"%s",t_str);
		string str=t_str;
		if(str=="valueOnly"){
			valueOnly=true;
		}else if(str=="notValueOnly"){
			valueOnly=false;
		}else{
			assert(false);
		}
		cout<<"valueOnly:"<<valueOnly<<endl;

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&m_aniLayerIndex);

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%f",&m_interval);

		fskipOneStr(fp,valueOnly);
		int t_nAniLayerInfo;
		fscanf(fp,"%i",&t_nAniLayerInfo);

		fskipOneStr(fp,valueOnly);
		int t_nSkinMesh;
		fscanf(fp,"%i",&t_nSkinMesh);

		//--------------------------close file
		fclose(fp);
		//------------------m_aniLayerInfoList
		assert(m_aniLayerInfoList.empty());
		for(int i=0;i<t_nAniLayerInfo;i++){
			string numStr=numberToStr(i);
			Cc3dAniLayerInfo*p=new Cc3dAniLayerInfo();
			p->autorelease();
			this->addAniLayerInfo(p);
			p->doImport(folderPath+"/m_aniLayerInfo_"+numStr,valueOnly);
		}
		//------------------skinMeshes
		assert(this->getMeshCount()==0);
		for(int i=0;i<t_nSkinMesh;i++){
			string numStr=numberToStr(i);
			Cc3dSkinMesh*p=new Cc3dSkinMesh();
			p->autorelease();
			this->addMesh(p);
			p->doImport(folderPath+"/m_skinMesh_"+numStr,valueOnly);
		}
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}

}