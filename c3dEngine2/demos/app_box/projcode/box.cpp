//
//  box.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#include "box.h"
void Cbox::makeBox(float d,string texFilePath){
	//----subMesh
	Cc3dSubMesh*subMesh=new Cc3dSubMesh();
	subMesh->autorelease();
	//--color
	Cc3dVector4 color[8]={Cc3dVector4(1,0,0,1),Cc3dVector4(0,1,0,1),Cc3dVector4(0,0,1,1),Cc3dVector4(0,1,1,1),
						  Cc3dVector4(1,0,1,1),Cc3dVector4(1,1,0,1),Cc3dVector4(1,0.5,0,1),Cc3dVector4(0,0.5,1,1)};
	//--vertex
	//front face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,+d,+d,1),Cc3dVector2(0,0),Cc3dVector4(0,0,1,0),color[0]));//0
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,-d,+d,1),Cc3dVector2(0,1),Cc3dVector4(0,0,1,0),color[1]));//1
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,-d,+d,1),Cc3dVector2(1,1),Cc3dVector4(0,0,1,0),color[2]));//2
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,+d,+d,1),Cc3dVector2(1,0),Cc3dVector4(0,0,1,0),color[3]));//3
	//back face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,+d,-d,1),Cc3dVector2(0,0),Cc3dVector4(0,0,-1,0),color[4]));//4
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,+d,-d,1),Cc3dVector2(0,1),Cc3dVector4(0,0,-1,0),color[5]));//5
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,-d,-d,1),Cc3dVector2(1,1),Cc3dVector4(0,0,-1,0),color[6]));//6
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,-d,-d,1),Cc3dVector2(1,0),Cc3dVector4(0,0,-1,0),color[7]));//7
	//up face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,+d,-d,1),Cc3dVector2(0,0),Cc3dVector4(0,1,0,0),color[4]));//8
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,+d,+d,1),Cc3dVector2(0,1),Cc3dVector4(0,1,0,0),color[0]));//9
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,+d,+d,1),Cc3dVector2(1,1),Cc3dVector4(0,1,0,0),color[3]));//10
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,+d,-d,1),Cc3dVector2(1,0),Cc3dVector4(0,1,0,0),color[5]));//11
	//down face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,-d,-d,1),Cc3dVector2(0,0),Cc3dVector4(0,-1,0,0),color[7]));//12
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,-d,-d,1),Cc3dVector2(0,1),Cc3dVector4(0,-1,0,0),color[6]));//13
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,-d,+d,1),Cc3dVector2(1,1),Cc3dVector4(0,-1,0,0),color[2]));//14
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,-d,+d,1),Cc3dVector2(1,0),Cc3dVector4(0,-1,0,0),color[1]));//15
	//left face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,+d,-d,1),Cc3dVector2(0,0),Cc3dVector4(-1,0,0,0),color[4]));//16
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,-d,-d,1),Cc3dVector2(0,1),Cc3dVector4(-1,0,0,0),color[7]));//17
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,-d,+d,1),Cc3dVector2(1,1),Cc3dVector4(-1,0,0,0),color[1]));//18
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(-d,+d,+d,1),Cc3dVector2(1,0),Cc3dVector4(-1,0,0,0),color[0]));//19
	//right face
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,+d,+d,1),Cc3dVector2(0,0),Cc3dVector4(1,0,0,0),color[3]));//20
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,-d,+d,1),Cc3dVector2(0,1),Cc3dVector4(1,0,0,0),color[2]));//21
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,-d,-d,1),Cc3dVector2(1,1),Cc3dVector4(1,0,0,0),color[6]));//22
	subMesh->addVertex(Cc3dVertex(Cc3dVector4(+d,+d,-d,1),Cc3dVector2(1,0),Cc3dVector4(1,0,0,0),color[5]));//23
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
	Cc3dTexture*texture=Cc3dTextureCache::sharedTextureCache()->addImage(texFilePath,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
	subMesh->setTexture(texture);
	//----mesh
	Cc3dMesh*mesh=new Cc3dMesh();
	mesh->autorelease();
	mesh->addSubMesh(subMesh);
	//----model
	this->addMesh(mesh);
}