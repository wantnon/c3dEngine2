//
//  c3dActor.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#include "c3dModel.h"
void Cc3dModel::addMesh(Cc3dMesh*mesh){
    assert(mesh);
    m_meshList.push_back(mesh);
  //  mesh->setName("?");
    this->addChild(mesh);
    
}

void Cc3dModel::submitVertex(GLenum usage){
    int nMesh=(int)getMeshCount();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*mesh=m_meshList[i];
        int nSubMesh=(int)mesh->getSubMeshCount();
        for(int j=0;j<nSubMesh;j++){
            Cc3dSubMesh*psubMesh=mesh->getSubMeshByIndex(j);
            psubMesh->getIndexVBO()->submitVertex(psubMesh->getSubMeshData()->getvlist(), usage);
        }
    }
    
}
void Cc3dModel::submitIndex(GLenum usage){
    int nMesh=(int)getMeshCount();
    for(int i=0;i<nMesh;i++){
        Cc3dMesh*mesh=m_meshList[i];
        int nSubMesh=(int)mesh->getSubMeshCount();
        for(int j=0;j<nSubMesh;j++){
            Cc3dSubMesh*psubMesh=mesh->getSubMeshByIndex(j);
            psubMesh->getIndexVBO()->submitIndex(psubMesh->getSubMeshData()->getIDtriList(), usage);
        }
    }
    
}

