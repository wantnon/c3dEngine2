//
//  physicsActor.cpp
//  c3d
//
//  Created by ck02-084 on 14-4-23.
//
//

#include "physicsActor.h"
vector<Cc3dVector4> getPosListFromSubMeshData(const Cc3dSubMeshData&subMeshData){
    vector<Cc3dVector4> posList;
    int nv=(int)subMeshData.getVertexCount();
    for(int i=0;i<nv;i++){
        const Cc3dVertex&vertex=subMeshData.getVertexByIndex(i);
        posList.push_back(vertex.getPos());
    }
    return posList;
}
vector<Cc3dIDTriangle> getIDtriListFromSubMeshData(const Cc3dSubMeshData&subMeshData){
    vector<Cc3dIDTriangle> IDtriList;
    int nIDtri=(int)subMeshData.getIDtriCount();
    for(int i=0;i<nIDtri;i++){
        const Cc3dIDTriangle&IDtri=subMeshData.getIDtriByIndex(i);
        IDtriList.push_back(IDtri);
    }
    return IDtriList;
}

Cc3dModel* makeBoxModel(float d,string texFilePath){
	return makeBoxModel(d,d,d,texFilePath);
}

Cc3dModel* makeBoxModel(float dx,float dy,float dz,string texFilePath){
	//----subMesh
	Cc3dSubMesh*subMesh=new Cc3dSubMesh();
	subMesh->autorelease();
	//--color
	//Cc3dVector4 color[8]={Cc3dVector4(1,0,0,1),Cc3dVector4(0,1,0,1),Cc3dVector4(0,0,1,1),Cc3dVector4(0,1,1,1),
    //    Cc3dVector4(1,0,1,1),Cc3dVector4(1,1,0,1),Cc3dVector4(1,0.5,0,1),Cc3dVector4(0,0.5,1,1)};
    Cc3dVector4 color[8]={Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1),
        Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1)};
	//--vertex
	//front face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,+dy,+dz,1),Cc3dVector2(0,0),Cc3dVector4(0,0,1,0),color[0]));//0
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,-dy,+dz,1),Cc3dVector2(0,1),Cc3dVector4(0,0,1,0),color[1]));//1
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,-dy,+dz,1),Cc3dVector2(1,1),Cc3dVector4(0,0,1,0),color[2]));//2
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,+dy,+dz,1),Cc3dVector2(1,0),Cc3dVector4(0,0,1,0),color[3]));//3
	//back face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,+dy,-dz,1),Cc3dVector2(0,0),Cc3dVector4(0,0,-1,0),color[4]));//4
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,+dy,-dz,1),Cc3dVector2(0,1),Cc3dVector4(0,0,-1,0),color[5]));//5
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,-dy,-dz,1),Cc3dVector2(1,1),Cc3dVector4(0,0,-1,0),color[6]));//6
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,-dy,-dz,1),Cc3dVector2(1,0),Cc3dVector4(0,0,-1,0),color[7]));//7
	//up face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,+dy,-dz,1),Cc3dVector2(0,0),Cc3dVector4(0,1,0,0),color[4]));//8
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,+dy,+dz,1),Cc3dVector2(0,1),Cc3dVector4(0,1,0,0),color[0]));//9
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,+dy,+dz,1),Cc3dVector2(1,1),Cc3dVector4(0,1,0,0),color[3]));//10
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,+dy,-dz,1),Cc3dVector2(1,0),Cc3dVector4(0,1,0,0),color[5]));//11
	//down face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,-dy,-dz,1),Cc3dVector2(0,0),Cc3dVector4(0,-1,0,0),color[7]));//12
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,-dy,-dz,1),Cc3dVector2(0,1),Cc3dVector4(0,-1,0,0),color[6]));//13
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,-dy,+dz,1),Cc3dVector2(1,1),Cc3dVector4(0,-1,0,0),color[2]));//14
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,-dy,+dz,1),Cc3dVector2(1,0),Cc3dVector4(0,-1,0,0),color[1]));//15
	//left face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,+dy,-dz,1),Cc3dVector2(0,0),Cc3dVector4(-1,0,0,0),color[4]));//16
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,-dy,-dz,1),Cc3dVector2(0,1),Cc3dVector4(-1,0,0,0),color[7]));//17
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,-dy,+dz,1),Cc3dVector2(1,1),Cc3dVector4(-1,0,0,0),color[1]));//18
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-dx,+dy,+dz,1),Cc3dVector2(1,0),Cc3dVector4(-1,0,0,0),color[0]));//19
	//right face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,+dy,+dz,1),Cc3dVector2(0,0),Cc3dVector4(1,0,0,0),color[3]));//20
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,-dy,+dz,1),Cc3dVector2(0,1),Cc3dVector4(1,0,0,0),color[2]));//21
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,-dy,-dz,1),Cc3dVector2(1,1),Cc3dVector4(1,0,0,0),color[6]));//22
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+dx,+dy,-dz,1),Cc3dVector2(1,0),Cc3dVector4(1,0,0,0),color[5]));//23
	//--IDTriangles
	//front face
	subMesh->addIDtri(Cc3dIDTriangle(0,1,2));
	subMesh->addIDtri(Cc3dIDTriangle(0,2,3));
	//back face
	subMesh->addIDtri(Cc3dIDTriangle(4,5,6));
	subMesh->addIDtri(Cc3dIDTriangle(4,6,7));
	//up face
	subMesh->addIDtri(Cc3dIDTriangle(8,9,10));
	subMesh->addIDtri(Cc3dIDTriangle(8,10,11));
	//down face
	subMesh->addIDtri(Cc3dIDTriangle(12,13,14));
	subMesh->addIDtri(Cc3dIDTriangle(12,14,15));
	//left face
	subMesh->addIDtri(Cc3dIDTriangle(16,17,18));
	subMesh->addIDtri(Cc3dIDTriangle(16,18,19));
	//right face
	subMesh->addIDtri(Cc3dIDTriangle(20,21,22));
	subMesh->addIDtri(Cc3dIDTriangle(20,22,23));
    //--texture
    if(texFilePath.empty()==false){
        Cc3dTexture*texture=Cc3dTextureCache::sharedTextureCache()->addImage(texFilePath,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
        subMesh->setTexture(texture);
    }
	//----mesh
	Cc3dMesh*mesh=new Cc3dMesh();
	mesh->autorelease();
    mesh->setName("box_mesh");
	mesh->addSubMesh(subMesh);
	//----model
    Cc3dModel*model=new Cc3dModel();
    model->autorelease();
    model->setName("box_model");
	model->addMesh(mesh);
    return model;
}

