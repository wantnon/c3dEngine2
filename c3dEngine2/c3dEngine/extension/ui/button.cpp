//
//  button.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#include "button.h"
bool Cbutton::init(float width,float height,string imagePath_up,string imagePath_dn,string imagePath_text){
	m_width=width;
	m_height=height;
	//--texture
	m_textureUp=Cc3dTextureCache::sharedTextureCache()->addImage(imagePath_up,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
	m_textureUp->retain();
	m_textureDn=Cc3dTextureCache::sharedTextureCache()->addImage(imagePath_dn,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR,false);
	m_textureDn->retain();

	//meshes
	for(int i=0;i<2;i++){
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
	this->getMeshByIndex(0)->getSubMeshByIndex(0)->setTexture(m_textureUp);
	setText(imagePath_text);
	this->setIsDoDepthTest(false);
	return true;
}
bool Cbutton::isContainPoint(const Cc3dVector2&point){
	Cc3dVector4 pos=this->getPos();
	float x=pos.x();
	float y=pos.y();
	float xmin=x-m_width/2;
	float xmax=x+m_width/2;
	float ymin=y-m_height/2;
	float ymax=y+m_height/2;
	if(point.x()>=xmin&&point.x()<=xmax&&point.y()>=ymin&&point.y()<=ymax){
		return true;
	}else{
		return false;
	}
}
void Cbutton::setText(string imagePath_text){
	Cc3dTexture*textureText=Cc3dTextureCache::sharedTextureCache()->addImage(imagePath_text,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
	this->getMeshByIndex(1)->getSubMeshByIndex(0)->setTexture(textureText);
}
void Cbutton::update(float dt){
	/*
	//backKey响应按下和抬起--abc
	if(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getIsDown()&&this->isContainPoint(Cc3dGestureAnalyzer::sharedGestureAnalyzer()->getPoint())){
		this->getMeshByIndex(0)->getSubMeshByIndex(0)->setTexture(m_textureDn);
	}else{
		this->getMeshByIndex(0)->getSubMeshByIndex(0)->setTexture(m_textureUp);
	}*/
}