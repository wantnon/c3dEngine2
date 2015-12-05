//
//  ground.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#include "ground.h"

void Cground::update(float dt){
	this->updateTerrain(*this->getCamera());
	this->submitIndex(GL_DYNAMIC_DRAW);
}