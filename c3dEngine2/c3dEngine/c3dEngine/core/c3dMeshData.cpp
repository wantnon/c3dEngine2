//
//  c3dMeshData.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-25.
//
//

#include "c3dMeshData.h"
void Cc3dSubMeshData::addVertex(const Cc3dVertex&vertex){
    vlist.push_back(vertex);
}
void Cc3dSubMeshData::addIDtri(const Cc3dIDTriangle&IDtri){
    IDtriList.push_back(IDtri);
}