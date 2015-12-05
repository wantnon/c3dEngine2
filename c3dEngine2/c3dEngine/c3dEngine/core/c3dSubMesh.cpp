//
//  c3dSubMesh.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//

#include "c3dSubMesh.h"
#include "c3dMatrix.h"
#include "c3dVector.h"
static Cc3dGLState*s_GLState=NULL;
Cc3dGLState*Cc3dGLState::getInstance(){
    if(s_GLState==NULL){
        s_GLState=new Cc3dGLState();
    }
    return s_GLState;
}

void Cc3dSubMesh::submit(GLenum usage){
    this->m_indexVBO->submitVertex(this->getSubMeshData()->getvlist(), usage);
    this->m_indexVBO->submitIndex(this->getSubMeshData()->getIDtriList() , usage);
}
void Cc3dSubMesh::submitVertex(GLenum usage){
    this->m_indexVBO->submitVertex(this->getSubMeshData()->getvlist(), usage);
}
void Cc3dSubMesh::submitIndex(GLenum usage){
    this->m_indexVBO->submitIndex(this->getSubMeshData()->getIDtriList() , usage);
}
void Cc3dSubMesh::clearMeshData(){
    m_subMeshData->clear();
}
void Cc3dSubMesh::addVertex(const Cc3dVertex&vertex){
    m_subMeshData->addVertex(vertex);
}
void Cc3dSubMesh::addIDtri(const Cc3dIDTriangle&IDtri){
    m_subMeshData->addIDtri(IDtri);
}