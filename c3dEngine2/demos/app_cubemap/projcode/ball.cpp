//
//  ball.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#include "ball.h"
void Cball::makeBall(float r,int nSlice,int nStack,string texFilePath){
	
	//make ball
	float dA=360.0/nSlice;
    float dB=180.0/nStack;
    const float PIDIV180=c3d_PI/180;
    vector<Cc3dVector4> positionList;
    vector<Cc3dVector2> texCoordList;
    vector<Cc3dVector4> normalList;
    vector<Cc3dIDTriangle> IDtriList;
    //generate positionList, texCoordList, normalList, colorList
    for(int i=0;i<=nStack;i++){
        float B=-90+i*dB;
        float y=r*sinf(B*PIDIV180);
        float cosB=cosf(B*PIDIV180);
        for (int j=0; j<=nSlice; j++) {
            float A=j*dA;
            float R=r*cosB;
            float x=R*cosf(A*PIDIV180);
            float z=R*sinf(A*PIDIV180);
            Cc3dVector4 position(x,y,z,1);
            float s=(float)j/nSlice+0.25;
            float t=1-(float)i/nStack;
            Cc3dVector2 texCoord(s,t);
            Cc3dVector4 normal=normalize(cc3dv4(position.x(), position.y(), position.z(), 0));
            positionList.push_back(position);
            texCoordList.push_back(texCoord);
            normalList.push_back(normal);
        }
    }
    //generate IDtriList
    for(int i=0;i<nStack;i++){
        for(int j=0;j<nSlice;j++){
            int vID_ld=(nSlice+1)*i+j;
            int vID_rd=vID_ld+1;
            int vID_ru=vID_rd+(nSlice+1);
            int vID_lu=vID_ru-1;
            Cc3dIDTriangle IDtri0(vID_ld,vID_rd,vID_ru);
            Cc3dIDTriangle IDtri1(vID_ld,vID_ru,vID_lu);
            IDtriList.push_back(IDtri0);
            IDtriList.push_back(IDtri1);
        }
    }
    Cc3dSubMesh*subMesh=new Cc3dSubMesh();
	subMesh->autorelease();
	int nPos=(int)positionList.size();
	for(int i=0;i<nPos;i++){
		Cc3dVector4&pos=positionList[i];
		Cc3dVector2&texCoord=texCoordList[i];
		Cc3dVector4&norm=normalList[i];
		subMesh->addVertex(Cc3dVertex(pos,texCoord,norm));
	}
	int nIDtri=(int)IDtriList.size();
	for(int i=0;i<nIDtri;i++){
		Cc3dIDTriangle&IDtri=IDtriList[i];
		subMesh->addIDtri(IDtri);
	}


	//--texture
	if(texFilePath.empty()==false){
		Cc3dTexture*texture=Cc3dTextureCache::sharedTextureCache()->addImage(texFilePath,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
		subMesh->setTexture(texture);
	}

	//----mesh
	Cc3dMesh*mesh=new Cc3dMesh();
	mesh->autorelease();
	mesh->addSubMesh(subMesh);
	//----model
	this->addMesh(mesh);
}