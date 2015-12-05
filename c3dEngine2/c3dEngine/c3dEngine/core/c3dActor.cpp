//
//  c3dActor.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#include "c3dActor.h"
void Cc3dActor::addMesh(Cc3dMesh*mesh){
    assert(mesh);
	m_model->addMesh(mesh);
    
}

void Cc3dActor::submitVertex(GLenum usage){
    m_model->submitVertex(usage);
    
    
}
void Cc3dActor::submitIndex(GLenum usage){
    m_model->submitIndex(usage);
    
}

