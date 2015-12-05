

#include "c3dMesh.h"
 
Cc3dRange Cc3dMesh::getRange()const {
    float xmin=c3d_INF;
    float xmax=-c3d_INF;
    float ymin=c3d_INF;
    float ymax=-c3d_INF;
    float zmin=c3d_INF;
    float zmax=-c3d_INF;
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        int nv=(int)m_subMeshList[i]->getSubMeshData()->getvlist().size();
        for(int j=0;j<nv;j++){
            const Cc3dVertex&v=m_subMeshList[i]->getVertexByIndex(j);//getSubMeshData()->vlist[j];
            const Cc3dVector4&pos=v.getPos();
            if(pos.x()<xmin)xmin=pos.x();
            if(pos.x()>xmax)xmax=pos.x();
            if(pos.y()<ymin)ymin=pos.y();
            if(pos.y()>ymax)ymax=pos.y();
            if(pos.z()<zmin)zmin=pos.z();
            if(pos.z()>zmax)zmax=pos.z();
        }
    }//得到xmin,xmax,ymin,ymax,zmin,zmax
    Cc3dRange range;
    range.init(xmin, xmax, ymin, ymax, zmin, zmax);
    return range;
}


void Cc3dMesh::genVBOBuffers(){
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh*psubMesh=m_subMeshList[i];
        psubMesh->getIndexVBO()->genBuffers();
    }
}void Cc3dMesh::setColor(float r,float g,float b,float a){
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh*psubMesh=m_subMeshList[i];
        if(psubMesh==NULL)continue;
        psubMesh->getMaterial()->setColor(r, g, b, a);
    }
    
}

void Cc3dMesh::setDiffuseAlpha(float alpha){
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh*psubMesh=m_subMeshList[i];
        if(psubMesh==NULL)continue;
        psubMesh->getMaterial()->setDiffuseAlpha(alpha);
    }
}
void Cc3dMesh::setDiffuseRGB(float r,float g,float b){
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh*psubMesh=m_subMeshList[i];
        if(psubMesh==NULL)continue;
        psubMesh->getMaterial()->setDiffuseRGB(r, g, b);
    }
}
void Cc3dMesh::setAmbient(float r,float g,float b){
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh*psubMesh=m_subMeshList[i];
        if(psubMesh==NULL)continue;
        psubMesh->getMaterial()->setAmbient(r, g, b);
    }
    
}
void Cc3dMesh::setSpecular(float r,float g,float b){
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh*psubMesh=m_subMeshList[i];
        if(psubMesh==NULL)continue;
        psubMesh->getMaterial()->setSpecular(r, g, b);
    }
    
}
void Cc3dMesh::setShininess(float shininess){
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh*psubMesh=m_subMeshList[i];
        if(psubMesh==NULL)continue;
        psubMesh->getMaterial()->setShininess(shininess);
    }
}
void Cc3dMesh::setIsHighlightUntransp(bool isHighlightUntransp){
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh*psubMesh=m_subMeshList[i];
        if(psubMesh==NULL)continue;
        psubMesh->getMaterial()->setIsHighlightUntransp(isHighlightUntransp);
    }
}
void Cc3dMesh::submit(GLenum usage)//提交数据--abc
{
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh*psubMesh=m_subMeshList[i];
        if(psubMesh)psubMesh->submit(usage);
    }
}
void Cc3dMesh::submitVertex(GLenum usage){
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh*psubMesh=m_subMeshList[i];
        if(psubMesh)psubMesh->submitVertex(usage);
    }
}
void Cc3dMesh::submitIndex(GLenum usage){
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh*psubMesh=m_subMeshList[i];
        if(psubMesh)psubMesh->submitIndex(usage);
    }
}
void Cc3dMesh::addSubMesh(Cc3dSubMesh*submesh){
    assert(submesh);
    m_subMeshList.push_back(submesh);
    //submesh->setName("?");
    this->addChild(submesh);
    
}

void Cc3dMesh::releaseIDtriList()//释放subMeshList中各submesh的IDtriList
{
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh&submesh=*m_subMeshList[i];
		submesh.getSubMeshData()->swapReleaseIDtriList();
    }
    
    
}
void Cc3dMesh::releasevList()//释放subMeshList中各submesh的vlist
{
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        Cc3dSubMesh&submesh=*m_subMeshList[i];
		submesh.getSubMeshData()->swapReleasevlist();
    }
}


void Cc3dMesh::scale_modifyVertexDirectly(float kx,float ky,float kz){//以直接修改顶点的方式进行放缩--abc
    int nSubMesh=(int)m_subMeshList.size();
    for(int i=0;i<nSubMesh;i++){
        int nv=(int)m_subMeshList[i]->getSubMeshData()->getVertexCount();
        for(int j=0;j<nv;j++){
            Cc3dVertex v=m_subMeshList[i]->getSubMeshData()->getVertexByIndex(j);
            const Cc3dVector4 pos=v.getPos();
            v.setPos(Cc3dVector4(pos.x()*kx, pos.y()*ky, pos.z()*kz,1));
            m_subMeshList[i]->getSubMeshData()->setVertexByIndex(j, v);
        }
    }
}


