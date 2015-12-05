//
//  terrain.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//
#include "geometry/geoMath.h"
#include "terrain/terrain.h"
#include "core/c3dFileUtils.h"
inline void Cterrain::readLandMat(){
    int bmpSideLength=pow(2.0,(int)(m_quadtreeDepth-1));//do not use 2, should use 2.0, see: http://www.360doc.com/content/11/0826/15/7566064_143451209.shtml
    //landMat开辟空间--abc
    landMat.resize(bmpSideLength);
    for(int i=0;i<(int)landMat.size();i++){
        landMat[i].resize(bmpSideLength);
    }
    //读取高程数据--abc
    const int MAKEUP=(4-(bmpSideLength*3)%4)%4;
    //data和_data只是两个临时中转用的数组,如果申请静态数组,会使栈空间不够用,
    //所以这里改用申请动态空间--abc
    BYTE *data=new BYTE[bmpSideLength*(bmpSideLength*3+MAKEUP)];//位图数据缓冲--abc
    BYTE *_data=new BYTE[bmpSideLength*(bmpSideLength*3)];//提取数据的数组--abc
    //读取位图--abc
    FILE  *infile=NULL;
    //打开文件--abc
    string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(m_heightMapFileName);
    infile=fopen(pathName.c_str(),"rb");//Open 24 bit bitmap file
    assert(infile);
    //读文件头--abc
    BYTE  Head[54];
    fread(Head,54,1,infile);
    //读数据--abc
    int len=bmpSideLength*(bmpSideLength*3+MAKEUP);
    fread(data,len,1,infile);
    fclose(infile);
    //分析文件头--abc
    int bmpWidth =Head[18]+Head[19]*256;
    int bmpHeight=Head[22]+Head[23]*256;
    if(bmpWidth!=bmpSideLength||bmpHeight!=bmpSideLength){
        cout<<"bmpWidth:"<<bmpWidth<<endl;
        cout<<"bmpHeight:"<<bmpHeight<<endl;
        cout<<"bmpSideLength:"<<bmpSideLength<<endl;
        cout<<"not match!"<<endl;
		assert(false);
    }
    int makeup=(4-(bmpWidth*3)%4)%4;
    if(makeup!=MAKEUP){
        cout<<"nt match!"<<endl;
		assert(false);
    }
    //提取数据--abc
    
    int p=0;
    int _p=0;
    for(int i=0;i<bmpHeight;i++){
        for(int j=0;j<bmpWidth;j++){
            _data[_p++]=data[p++];
            _data[_p++]=data[p++];
            _data[_p++]=data[p++];
        }
        p+=MAKEUP;
    }
    //将_data填充到landMat并放缩--abc
    //注意bmp的通道顺序是bgr
    //另外像素序列是反的--abc
    _p=0;
    for(int i=bmpSideLength-1;i>=0;i--){//倒序--abc
        for(int j=0;j<bmpSideLength;j++){//正序--abc
            float b=_data[_p++];//blue表附加凹陷--abc
            float g=_data[_p++];//g表基础凸起--abc
            float r=_data[_p++];//r表附加凸起--abc
            landMat[i][j]=m_heightScale*(g-b+r);
        }
    }

    delete []_data;
    delete []data;
}
inline void Cterrain::fillNormalMat()//根据高程数据landMat填充法向数据normalMat
{
    //为normalMat开辟空间--abc
    normalMat.resize((int)landMat.size());
    for(int i=0;i<(int)normalMat.size();i++){
        normalMat[i].resize(landMat[i].size());
    }
    //根据landMat填充normalMat
    float dw=m_range.getSpanX()/normalMat[0].size();
    float dh=m_range.getSpanZ()/normalMat.size();
    for(int i=0;i<(int)normalMat.size();i++){
        for(int j=0;j<(int)normalMat[i].size();j++){
            //填充normalMatMat[I][J][i][j]
            float hct=landMat[i][j];//本格高度(中心高度)
            float hrt;//右格高度--abc
            float hdn;//下格高度--abc
            if(j+1<(int)normalMat[i].size()){//j+1不越界--abc
                hrt=landMat[i][j+1];
            }else{//j+1越界--abc
                normalMat[i][j].init(0,1,0,0);
                continue;
            }
            if(i+1<(int)normalMat.size()){//i+1不越界--abc
                hdn=landMat[i+1][j];
            }else{//i+1越界--abc
                normalMat[i][j].init(0,1,0,0);
                continue;
            }
            //      (0,hct,0)
            //         *-------> (w,hrt,0)
            //    vec1 |   vec2
            //        \|/
            //     (0,hdn,h)
            Cc3dVector4 vec1(0,hdn-hct,dh,0);
            Cc3dVector4 vec2(dw,hrt-hct,0,0);
            Cc3dVector4 norm=cross(vec1, vec2);
            norm=normalize(norm);
            normalMat[i][j]=norm;
        }
    }
}
bool Cterrain::initWithFile(const string&heightMapFileName,const Cc3dRect&rect,float heightScale,int quadtreeDepth,
                   Cc3dTexture*texture){
    assert(rect.getWidth()==rect.getHeight());
    m_heightScale=heightScale;
    m_quadtreeDepth=quadtreeDepth;
    m_heightMapFileName=heightMapFileName;
    //生成mesh
    Cc3dMesh*mesh=new Cc3dMesh();
    mesh->autorelease();
    addMesh(mesh);
    //读取高程数据--abc
    readLandMat();
    //求高度范围--abc
    float Hmin=c3d_INF;
    float Hmax=-c3d_INF;
    for(int i=0;i<(int)landMat.size();i++){//倒序--abc
        for(int j=0;j<(int)landMat[i].size();j++){//正序--abc
            if(landMat[i][j]>Hmax){
                Hmax=landMat[i][j];
            }
            if(landMat[i][j]<Hmin){
                Hmin=landMat[i][j];
            }
        }
    }//得到Hmin和Hmax
    //设置m_range
    m_range.init(rect.getMinX(), rect.getMaxX(), Hmin, Hmax, rect.getMinY(), rect.getMaxY());
    //计算步长--abc
    int markMatSideLength=pow(2.0,m_quadtreeDepth-1)+1;//do not use 2, should use 2.0, see: http://www.360doc.com/content/11/0826/15/7566064_143451209.shtml
    gridSize=m_range.getSpanX()/(markMatSideLength-1);
    //填充法向数据--abc
    fillNormalMat();
    //markmat开辟空间--abc
    markmat.resize(markMatSideLength);
    for(int i=0;i<(int)markmat.size();i++){
        markmat[i].resize(markMatSideLength);
        for(int j=0;j<(int)markmat[i].size();j++){
            markmat[i][j]=0;
        }
    }
    //制作ground submesh
    Cc3dSubMesh*submesh=new Cc3dSubMesh();
    submesh->autorelease();
    submesh->setTexture(texture);
    submesh->getIndexVBO()->genBuffers();
    this->getMesh()->addSubMesh(submesh);
    //make submesh
    makeMesh();
    
    return true;
    
}
void Cterrain::makeMesh(){
    int nVertex=(int)markmat.size()*(int)markmat[0].size();
    int nStep=4+2+4+4+2;
    float *vertexArray=new float[nVertex*nStep];
    //填充vertexArray
    for(int i=0;i<(int)markmat.size();i++){
        for(int j=0;j<(int)markmat[i].size();j++){
            float x,y,z;
            x=m_range.getMinX()+j*gridSize;
            y=landMat[min(i,(int)landMat.size()-1)][min(j,(int)landMat[0].size()-1)];
            z=m_range.getMinZ()+i*gridSize;
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+0)=x;
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+1)=y;
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+2)=z;
			*(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+3)=1;
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+4)=m_uvScale*j/(int)markmat[0].size();
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+5)=m_uvScale*i/(int)markmat.size();
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+6)=normalMat[min(i,(int)landMat.size()-1)][min(j,(int)landMat[0].size()-1)].x();
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+7)=normalMat[min(i,(int)landMat.size()-1)][min(j,(int)landMat[0].size()-1)].y();
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+8)=normalMat[min(i,(int)landMat.size()-1)][min(j,(int)landMat[0].size()-1)].z();
			*(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+9)=0;
			*(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+10)=1;
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+11)=1;
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+12)=1;
			*(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+13)=1;
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+14)=(x-m_range.getMinX())/m_range.getSpanX();//alpha map texCoord s
            *(vertexArray+i*(int)markmat[0].size()*nStep+j*nStep+15)=(z-m_range.getMinZ())/m_range.getSpanZ();//alpha map texCoord t
        }
    }
    this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->clearvlist();//vlist.clear();
    for(int i=0;i<nVertex;i++){
        Cc3dVertex vertex;
        vertex.setPos(vertexArray+i*nStep);
        vertex.setTexCoord(vertexArray+i*nStep+4);
        vertex.setNorm(vertexArray+i*nStep+4+2);
		vertex.setColor(vertexArray+i*nStep+4+2+4);
        vertex.setTexCoord2(vertexArray+i*nStep+4+2+4+4);
        this->getMesh()->getSubMeshByIndex(0)->addVertex(vertex);//getSubMeshData()->vlist.push_back(vertex);
    }
    //释放vertexArray
    delete []vertexArray;
}

void Cterrain::getBlocks(int jmin,int jmax,int imin,int imax,int curDepth)
{
    //检查当前节点是否与视截体相交--abc
    //求节点p所表示区域的保守包围盒--abc
    //    上面--abc
    //    p[0]--p[3]
    //     |     |
    //    p[1]--p[2]
    //    下面--abc
    //    p[4]--p[7]
    //     |     |
    //    p[5]--p[6]
    float xmin=m_range.getMinX()+gridSize*jmin;
    float xmax=m_range.getMinX()+gridSize*jmax;
    float zmin=m_range.getMinZ()+gridSize*imin;
    float zmax=m_range.getMinZ()+gridSize*imax;
    float ymin=m_range.getMinY();
    float ymax=m_range.getMaxY();
    float c[3]={(xmax+xmin)/2,(ymin+ymax)/2,(zmin+zmax)/2};
    float r=max(xmax-xmin,ymax-ymin)*0.86602540378443864676372317075294;//由于zmax-zmin与xmax-xmin相等,所以不用考虑--abc
    //看球体(c,r)是否都在planeList中某个面的反面,如果是则可剔除--abc
    bool visible=true;
    for(int i=0;i<5;i++){//不考虑远平面--abc
        const Cc3dPlane&plane=this->getMesh()->getSubMeshByIndex(0)->getCamera()->getFrustum().getPlaneByIndex(i);
        //看球体(c,r)是否在plane的背面--abc
        float PND=directedDistanceFromPointToPlane(plane, c);
        if(PND<-r){//如果在背面--abc
            //断定为不可见,不用再继续检测--abc
            visible=false;
            break;
        }
    }//得到visible
    if(visible){//如果可见--abc
        bool needDiv=false;//是否需要再分--abc
        //求needDiv
        if(imin+1==imax){//无须再分,因为已经无法再分--abc
            needDiv=false;
        }else{//进一步判断--abc
            //求c到视点的距离--abc
            float d2=square(this->getMesh()->getSubMeshByIndex(0)->getCamera()->getEyePos().x()-c[0])
         ///   +square(this->getMesh()->getSubMeshByIndex(0)->getCamera()->getEyePos().y()-c[1])
            +square(this->getMesh()->getSubMeshByIndex(0)->getCamera()->getEyePos().z()-c[2]);
            float e=xmax-xmin;//边长--abc
            if(d2<square(e*reso))needDiv=true;
        }//得到needDiv
        if(needDiv){//继续分--abc
            int imid=(imin+imax)>>1;//除2
            int jmid=(jmin+jmax)>>1;
            markmat[imid][jmid]=true;
            markedElementIndexList.push_back(Cij(imid,jmid));
            //对四个孩子继续递归--abc
            getBlocks(jmin,jmid,imin,imid,curDepth+1);
            getBlocks(jmin,jmid,imid,imax,curDepth+1);
            getBlocks(jmid,jmax,imid,imax,curDepth+1);
            getBlocks(jmid,jmax,imin,imid,curDepth+1);
        }else{//不分--abc
			CterrainBlock block(imin,imax,jmin,jmax);
			m_blockList.push_back(block);
   
        }
    }
}


void Cterrain::updateTerrain(const Cc3dCamera&camera){

    //清除四叉树上的标记--abc
    int nMarked=(int)markedElementIndexList.size();
    for(int i=0;i<nMarked;i++){
        const Cij&index=markedElementIndexList[i];
        markmat[index.getI()][index.getJ()]=0;
    }
    //清空markedElementIndexList
    markedElementIndexList.clear();
    //清空model_ground的indexList
    this->getMesh()->getSubMeshByIndex(0)->getSubMeshData()->clearIDtriList();//IDtriList.clear();
	//清空m_blockList
	m_blockList.clear();
    //获得所有block并填充markmat
    getBlocks(0,(int)markmat.size()-1,0,(int)markmat[0].size()-1,1);//进入第一层（根节点规定为第一层)
	//将blocks转化为IDtris
	blocksToIDtris_complex();//blocksToIDtris();

}
//--------------------------------dn
vector<Cij> Cterrain::getDivIJsOnDnEdge(int imin,int imax,int jmin,int jmax){
	vector<Cij> divIJList;
	//左端点--abc
	divIJList.push_back(Cij(imax,jmin));
	//中间--abc
	vector<Cij> divIJListMid=getDivIJsOnDnEdge_inn(imin,imax,jmin,jmax);
	int n=(int)divIJListMid.size();
	for(int i=0;i<n;i++){
		divIJList.push_back(divIJListMid[i]);
	}
	//右端点--abc
	divIJList.push_back(Cij(imax,jmax));
	return divIJList;
}
vector<Cij> Cterrain::getDivIJsOnDnEdge_inn(int imin,int imax,int jmin,int jmax){
	vector<Cij> divIJList;
	if(imin+1==imax)return divIJList;
	//判断本区域是否分割--abc
	int imid=(imin+imax)>>1;//除2
    int jmid=(jmin+jmax)>>1;
	int markmatW=(int)markmat[0].size();
	int markmatH=(int)markmat.size();
	assert(imid>=0&&imid<markmatH);
	assert(jmid>=0&&jmid<markmatW);
	if(markmat[imid][jmid]){
		//左--abc
		vector<Cij> divIJList_left=getDivIJsOnDnEdge_inn(imid,imax,jmin,jmid);
		int nDivIJLeft=(int)divIJList_left.size();
		for(int i=0;i<nDivIJLeft;i++){
			divIJList.push_back(divIJList_left[i]);
		}
		//中--abc
		divIJList.push_back(Cij(imax,jmid));
		//右--abc
		vector<Cij> divIJList_right=getDivIJsOnDnEdge_inn(imid,imax,jmid,jmax);
		int nDivIJRight=(int)divIJList_right.size();
		for(int i=0;i<nDivIJRight;i++){
			divIJList.push_back(divIJList_right[i]);
		}
	}else{
		return divIJList;
	}
	return divIJList;
}
//---------------------------------up

vector<Cij> Cterrain::getDivIJsOnUpEdge(int imin,int imax,int jmin,int jmax){
	//cout<<"spans:"<<imin<<" "<<imax<<" "<<jmin<<" "<<jmax<<endl;
	vector<Cij> divIJList;
	//左端点--abc
	divIJList.push_back(Cij(imin,jmin));
	//中间--abc
	vector<Cij> divIJListMid=getDivIJsOnUpEdge_inn(imin,imax,jmin,jmax);
	int n=(int)divIJListMid.size();
	for(int i=0;i<n;i++){
		divIJList.push_back(divIJListMid[i]);
	}
	//右端点--abc
	divIJList.push_back(Cij(imin,jmax));
	return divIJList;
}
vector<Cij> Cterrain::getDivIJsOnUpEdge_inn(int imin,int imax,int jmin,int jmax){
	vector<Cij> divIJList;
	if(imin+1==imax)return divIJList;
	//判断本区域是否分割--abc
	int imid=(imin+imax)>>1;//除2
    int jmid=(jmin+jmax)>>1;
	int markmatW=(int)markmat[0].size();
	int markmatH=(int)markmat.size();
	assert(imid>=0&&imid<markmatH);
	assert(jmid>=0&&jmid<markmatW);
	if(markmat[imid][jmid]){
		//左--abc
		vector<Cij> divIJList_left=getDivIJsOnUpEdge_inn(imin,imid,jmin,jmid);
		int nDivIJLeft=(int)divIJList_left.size();
		for(int i=0;i<nDivIJLeft;i++){
			divIJList.push_back(divIJList_left[i]);
		}
		//中--abc
		divIJList.push_back(Cij(imin,jmid));
		//右--abc
		vector<Cij> divIJList_right=getDivIJsOnUpEdge_inn(imin,imid,jmid,jmax);
		int nDivIJRight=(int)divIJList_right.size();
		for(int i=0;i<nDivIJRight;i++){
			divIJList.push_back(divIJList_right[i]);
		}
	}else{
		return divIJList;
	}
	return divIJList;
}
//-------------------------------------left
vector<Cij> Cterrain::getDivIJsOnLeftEdge(int imin,int imax,int jmin,int jmax){
	vector<Cij> divIJList;
	//上端点--abc
	divIJList.push_back(Cij(imin,jmin));
	//中间--abc
	vector<Cij> divIJListMid=getDivIJsOnLeftEdge_inn(imin,imax,jmin,jmax);
	int n=(int)divIJListMid.size();
	for(int i=0;i<n;i++){
		divIJList.push_back(divIJListMid[i]);
	}
	//下端点--abc
	divIJList.push_back(Cij(imax,jmin));
	return divIJList;
}
vector<Cij> Cterrain::getDivIJsOnLeftEdge_inn(int imin,int imax,int jmin,int jmax){
	vector<Cij> divIJList;
	if(imin+1==imax)return divIJList;
	//判断本区域是否分割--abc
	int imid=(imin+imax)>>1;//除2
    int jmid=(jmin+jmax)>>1;
	int markmatW=(int)markmat[0].size();
	int markmatH=(int)markmat.size();
	assert(imid>=0&&imid<markmatH);
	assert(jmid>=0&&jmid<markmatW);
	if(markmat[imid][jmid]){
		//上--abc
		vector<Cij> divIJList_up=getDivIJsOnLeftEdge_inn(imin,imid,jmin,jmid);
		int nDivIJUp=(int)divIJList_up.size();
		for(int i=0;i<nDivIJUp;i++){
			divIJList.push_back(divIJList_up[i]);
		}
		//中--abc
		divIJList.push_back(Cij(imid,jmin));
		//下--abc
		vector<Cij> divIJList_dn=getDivIJsOnLeftEdge_inn(imid,imax,jmin,jmid);
		int nDivIJDn=(int)divIJList_dn.size();
		for(int i=0;i<nDivIJDn;i++){
			divIJList.push_back(divIJList_dn[i]);
		}
	}else{
		return divIJList;
	}
	return divIJList;
}
//--------------------------right
vector<Cij> Cterrain::getDivIJsOnRightEdge(int imin,int imax,int jmin,int jmax){
	vector<Cij> divIJList;
	//上端点--abc
	divIJList.push_back(Cij(imin,jmax));
	//中间--abc
	vector<Cij> divIJListMid=getDivIJsOnRightEdge_inn(imin,imax,jmin,jmax);
	int n=(int)divIJListMid.size();
	for(int i=0;i<n;i++){
		divIJList.push_back(divIJListMid[i]);
	}
	//下端点--abc
	divIJList.push_back(Cij(imax,jmax));
	return divIJList;
}
vector<Cij> Cterrain::getDivIJsOnRightEdge_inn(int imin,int imax,int jmin,int jmax){
	vector<Cij> divIJList;
	if(imin+1==imax)return divIJList;
	//判断本区域是否分割--abc
	int imid=(imin+imax)>>1;//除2
    int jmid=(jmin+jmax)>>1;
	int markmatW=(int)markmat[0].size();
	int markmatH=(int)markmat.size();
	assert(imid>=0&&imid<markmatH);
	assert(jmid>=0&&jmid<markmatW);
	if(markmat[imid][jmid]){
		//上--abc
		vector<Cij> divIJList_up=getDivIJsOnRightEdge_inn(imin,imid,jmid,jmax);
		int nDivIJUp=(int)divIJList_up.size();
		for(int i=0;i<nDivIJUp;i++){
			divIJList.push_back(divIJList_up[i]);
		}
		//中--abc
		divIJList.push_back(Cij(imid,jmax));
		//下--abc
		vector<Cij> divIJList_dn=getDivIJsOnDnEdge_inn(imid,imax,jmid,jmax);
		int nDivIJDn=(int)divIJList_dn.size();
		for(int i=0;i<nDivIJDn;i++){
			divIJList.push_back(divIJList_dn[i]);
		}
	}else{
		return divIJList;
	}
	return divIJList;
}
//---------------

void Cterrain::blocksToIDtris_complex(){
	//
	int markmatW=(int)markmat[0].size();
	int markmatH=(int)markmat.size();
	//
	int nBlock=(int)m_blockList.size();
	for(int i=0;i<nBlock;i++){
		const CterrainBlock&block=m_blockList[i];
		int imin=block.getImin();
		int imax=block.getImax();
		int jmin=block.getJmin();
		int jmax=block.getJmax();
		if(imin+1==imax){//不再分--abc
			//   ID0-ID1
			//    | \ | 
			//   ID2-ID3
			int ID0=markmatW*imin+jmin;
			int ID1=markmatW*imin+jmax;
			int ID2=markmatW*imax+jmin;
			int ID3=markmatW*imax+jmax;
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID0, ID2, ID3));
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID0, ID3, ID1));
			continue;
		}
		//本区域中心--abc
        int imid=(imin+imax)>>1;//除2
        int jmid=(jmin+jmax)>>1;
		
		//block划分--abc
		int iSpan=imax-imin;
		int jSpan=jmax-jmin;
	    vector<Cij> divIJsUp,divIJsDn,divIJsLeft,divIJsRight;
		if(imin-iSpan>=0){
			divIJsUp=getDivIJsOnDnEdge(imin-iSpan,imax-iSpan,jmin,jmax);
		}else{
			divIJsUp.push_back(Cij(imin,jmin));
			divIJsUp.push_back(Cij(imin,jmax));
		}
		if(imax+iSpan<markmatH){
			divIJsDn=getDivIJsOnUpEdge(imin+iSpan,imax+iSpan,jmin,jmax);
		}else{
			divIJsDn.push_back(Cij(imax,jmin));
			divIJsDn.push_back(Cij(imax,jmax));
		}
		if(jmin-jSpan>=0){
			divIJsLeft=getDivIJsOnRightEdge(imin,imax,jmin-jSpan,jmax-jSpan);
		}else{
			divIJsLeft.push_back(Cij(imin,jmin));
			divIJsLeft.push_back(Cij(imax,jmin));
		}
		if(jmax+jSpan<markmatW){
			divIJsRight=getDivIJsOnLeftEdge(imin,imax,jmin+jSpan,jmax+jSpan);
		}else{
			divIJsRight.push_back(Cij(imin,jmax));
			divIJsRight.push_back(Cij(imax,jmax));
		}
		int nIJsUp=(int)divIJsUp.size();
		int nIJsDn=(int)divIJsDn.size();
		int nIJsLeft=(int)divIJsLeft.size();
		int nIJsRight=(int)divIJsRight.size();
		if(nIJsUp==2&&nIJsDn==2&&nIJsLeft==2&&nIJsRight==2){
			//   ID0-ID1
			//    | \ | 
			//   ID2-ID3
			int ID0=markmatW*imin+jmin;
			int ID1=markmatW*imin+jmax;
			int ID2=markmatW*imax+jmin;
			int ID3=markmatW*imax+jmax;
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID0, ID2, ID3));
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID0, ID3, ID1));
			continue;
		}
		//block中心vID
		int vID_center=markmatW*imid+jmid;
		//block上四分之一生成IDtri
		for(int k=0;k<nIJsUp-1;k++){
			int i=divIJsUp[k].getI();
			int j=divIJsUp[k].getJ();
			int in=divIJsUp[k+1].getI();
			int jn=divIJsUp[k+1].getJ();
			int vID=markmatW*i+j;
			int vIDn=markmatW*in+jn;
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(vID_center, vIDn, vID));
		}
		//block下四分之一生成IDtri
		for(int k=0;k<nIJsDn-1;k++){
			int i=divIJsDn[k].getI();
			int j=divIJsDn[k].getJ();
			int in=divIJsDn[k+1].getI();
			int jn=divIJsDn[k+1].getJ();
			int vID=markmatW*i+j;
			int vIDn=markmatW*in+jn;
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(vID_center, vID, vIDn));
		}
		//block左四分之一生成IDtri
		for(int k=0;k<nIJsLeft-1;k++){
			int i=divIJsLeft[k].getI();
			int j=divIJsLeft[k].getJ();
			int in=divIJsLeft[k+1].getI();
			int jn=divIJsLeft[k+1].getJ();
			int vID=markmatW*i+j;
			int vIDn=markmatW*in+jn;
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(vID_center, vID, vIDn));
		}
		//block右四分之一生成IDtri
		for(int k=0;k<nIJsRight-1;k++){
			int i=divIJsRight[k].getI();
			int j=divIJsRight[k].getJ();
			int in=divIJsRight[k+1].getI();
			int jn=divIJsRight[k+1].getJ();
			int vID=markmatW*i+j;
			int vIDn=markmatW*in+jn;
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(vID_center, vIDn, vID));
		}
	}
}
void Cterrain::blocksToIDtris(){
	//
	int markmatW=(int)markmat[0].size();
	int markmatH=(int)markmat.size();
	//
	int nBlock=(int)m_blockList.size();
	for(int i=0;i<nBlock;i++){
		const CterrainBlock&block=m_blockList[i];
		int imin=block.getImin();
		int imax=block.getImax();
		int jmin=block.getJmin();
		int jmax=block.getJmax();
		if(imin+1==imax){//不再分--abc
			//   ID0-ID1
			//    | \ | 
			//   ID2-ID3
			int ID0=markmatW*imin+jmin;
			int ID1=markmatW*imin+jmax;
			int ID2=markmatW*imax+jmin;
			int ID3=markmatW*imax+jmax;
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID0, ID2, ID3));
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID0, ID3, ID1));
			continue;
		}
		//本block四周等面积区域分割情况--abc
		//本区域中心--abc
        int imid=(imin+imax)>>1;//除2
        int jmid=(jmin+jmax)>>1;
		//上方等面积区域是否分割--abc
		bool upDivided=false;
		{
			int imid_up=imid-(imax-imin);
			int jmid_up=jmid;
			upDivided=(imid_up>=0&&markmat[imid_up][jmid_up]);
		}
		//下方等面积区域是否分割--abc
		bool dnDivided=false;
		{
			int imid_dn=imid+(imax-imin);
            int jmid_dn=jmid;
			dnDivided=(imid_dn<markmatH&&markmat[imid_dn][jmid_dn]);
		}
		//左方等面积区域是否分割--abc
		bool leftDivided=false;
		{
			 int imid_left=imid;
             int jmid_left=jmid-(jmax-jmin);
			 leftDivided=(jmid_left>=0&&markmat[imid_left][jmid_left]);
		}
		//右方等面积区域是否分割--abc
		bool rightDivided=false;
		{
			int imid_right=imid;
            int jmid_right=jmid+(jmax-jmin);
			rightDivided=(jmid_right<markmatW&&markmat[imid_right][jmid_right]);
		}
		if(upDivided==false&&dnDivided==false&&leftDivided==false&&rightDivided==false){//不再分--abc
			//   ID0-ID1
			//    | \ | 
			//   ID2-ID3
			int ID0=markmatW*imin+jmin;
			int ID1=markmatW*imin+jmax;
			int ID2=markmatW*imax+jmin;
			int ID3=markmatW*imax+jmax;
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID0, ID2, ID3));
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID0, ID3, ID1));
			continue;
		}
		//block划分--abc
		//   ID0-ID1-ID2
		//    | \ | / |
		//   ID3-ID4-ID5
		//    | / | \ |
		//   ID6-ID7-ID8
		int ID0=markmatW*imin+jmin;
		int ID1=markmatW*imin+jmid;
		int ID2=markmatW*imin+jmax;
		int ID3=markmatW*imid+jmin;
		int ID4=markmatW*imid+jmid;
		int ID5=markmatW*imid+jmax;
		int ID6=markmatW*imax+jmin;
		int ID7=markmatW*imax+jmid;
		int ID8=markmatW*imax+jmax;
		//block上四分之一生成IDtri
		if(upDivided){
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID0, ID4, ID1));
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID1, ID4, ID2));
		}else{
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID0, ID4, ID2));
		}
		//block下四分之一生成IDtri
		if(dnDivided){
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID4, ID6, ID7));
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID4, ID7, ID8));
		}else{
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID4, ID6, ID8));
		}
		//block左四分之一生成IDtri
		if(leftDivided){
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID4, ID0, ID3));
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID4, ID3, ID6));
		}else{
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID4, ID0, ID6));
		}
		//block右四分之一生成IDtri
		if(rightDivided){
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID4, ID5, ID2));
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID4, ID8, ID5));
		}else{
			this->getMesh()->getSubMeshByIndex(0)->addIDtri(Cc3dIDTriangle(ID4, ID8, ID2));
		}

	}
}
float Cterrain::getHAndNormal(float x,float z,Cc3dVector4&norm)const{
    int j=(x-m_range.getMinX())/gridSize+0.5;//加0.5为实现四舍五入--abc
    int i=(z-m_range.getMinZ())/gridSize+0.5;
    if(i>=0&&j>=0&&i<(int)landMat.size()&&j<(int)landMat[0].size()){
        norm=normalMat[i][j];
        return landMat[i][j];
    }else{
        norm.init(0,1,0,0);
        return -c3d_INF;
    }
}
float Cterrain::getH(float x,float z)const {

    int j=(x-m_range.getMinX())/gridSize+0.5;//加0.5为实现四舍五入--abc
    int i=(z-m_range.getMinZ())/gridSize+0.5;
    if(i>=0&&j>=0&&i<(int)landMat.size()&&j<(int)landMat[0].size()){
        return landMat[i][j];
    }else{
        return -c3d_INF;
    }
    
}
