//
//  button.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#include "board.h"

bool Cboard::init(float width,float height,Cc3dTexture*texture){
	m_width=width;
	m_height=height;

	//meshes
	{
		//----subMesh
		Cc3dSubMesh* subMesh=new Cc3dSubMesh();
		subMesh->autorelease();
		//--color
		Cc3dVector4 color[4]={Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1)};
		//--vertex
		//front face
		subMesh->addVertex(Cc3dVertex(Cc3dVector4(-width/2,+height/2,0,1),Cc3dVector2(0,0),Cc3dVector4(0,0,1,0),color[0]));//0
		subMesh->addVertex(Cc3dVertex(Cc3dVector4(-width/2,-height/2,0,1),Cc3dVector2(0,1),Cc3dVector4(0,0,1,0),color[1]));//1
		subMesh->addVertex(Cc3dVertex(Cc3dVector4(+width/2,-height/2,0,1),Cc3dVector2(1,1),Cc3dVector4(0,0,1,0),color[2]));//2
		subMesh->addVertex(Cc3dVertex(Cc3dVector4(+width/2,+height/2,0,1),Cc3dVector2(1,0),Cc3dVector4(0,0,1,0),color[3]));//3
		//--IDTriangles
		//front face
		subMesh->addIDtri(Cc3dIDTriangle(0,1,2));
		subMesh->addIDtri(Cc3dIDTriangle(0,2,3));
		//----mesh
		Cc3dMesh*mesh=new Cc3dMesh();
		mesh->autorelease();
		mesh->addSubMesh(subMesh);
		//----model
		this->addMesh(mesh);
	}
	//----texture
	this->getMeshByIndex(0)->getSubMeshByIndex(0)->setTexture(texture);
	this->setIsDoDepthTest(false);
	return true;
}
bool Cboard::init(float width,float height,string imagePath){
	m_width=width;
	m_height=height;
	//--texture
	Cc3dTexture*texture=Cc3dTextureCache::sharedTextureCache()->addImage(imagePath,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

	//meshes
	{
		//----subMesh
		Cc3dSubMesh* subMesh=new Cc3dSubMesh();
		subMesh->autorelease();
		//--color
		Cc3dVector4 color[4]={Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1),Cc3dVector4(1,1,1,1)};
		//--vertex
		//front face
		subMesh->addVertex(Cc3dVertex(Cc3dVector4(-width/2,+height/2,0,1),Cc3dVector2(0,0),Cc3dVector4(0,0,1,0),color[0]));//0
		subMesh->addVertex(Cc3dVertex(Cc3dVector4(-width/2,-height/2,0,1),Cc3dVector2(0,1),Cc3dVector4(0,0,1,0),color[1]));//1
		subMesh->addVertex(Cc3dVertex(Cc3dVector4(+width/2,-height/2,0,1),Cc3dVector2(1,1),Cc3dVector4(0,0,1,0),color[2]));//2
		subMesh->addVertex(Cc3dVertex(Cc3dVector4(+width/2,+height/2,0,1),Cc3dVector2(1,0),Cc3dVector4(0,0,1,0),color[3]));//3
		//--IDTriangles
		//front face
		subMesh->addIDtri(Cc3dIDTriangle(0,1,2));
		subMesh->addIDtri(Cc3dIDTriangle(0,2,3));
		//----mesh
		Cc3dMesh*mesh=new Cc3dMesh();
		mesh->autorelease();
		mesh->addSubMesh(subMesh);
		//----model
		this->addMesh(mesh);
	}
	//----texture
	this->getMeshByIndex(0)->getSubMeshByIndex(0)->setTexture(texture);
	this->setIsDoDepthTest(false);
	return true;
}