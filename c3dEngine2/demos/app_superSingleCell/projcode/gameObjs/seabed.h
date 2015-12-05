//
//  seabed.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-4.
//
//

#ifndef __HelloOpenGL__seabed__
#define __HelloOpenGL__seabed__

#include <iostream>
using namespace std;
#include "c3d.h"
class Cseabed:public Cc3dActor
{
protected:
    float m_height;
public:
    Cseabed(){
        m_height=-c3d_INF;
    }
    virtual~Cseabed(){};
    bool init(float r,Cc3dTexture*texture){
        Cc3dMesh*mesh=new Cc3dMesh();
        mesh->autorelease();
        addMesh(mesh);
        Cc3dSubMesh*submesh=new Cc3dSubMesh();
        submesh->autorelease();
        submesh->setTexture(texture);
        submesh->getIndexVBO()->genBuffers();
        
        const float h=m_height;
        const float d=r;
        //v2(-d,h,-d)  v1(d,h,-d)
        //     ---------
        //     |                    |
        //     |                    |
        //     ---------
        //v3(-d,h,d)   v0(d,h,d)
        Cc3dVertex v0=Cc3dVertex(Cc3dVector4(d,h,d,1),Cc3dVector2(1,0),Cc3dVector4(0,1,0,0));
        Cc3dVertex v1=Cc3dVertex(Cc3dVector4(d,h,-d,1),Cc3dVector2(1,1),Cc3dVector4(0,1,0,0));
        Cc3dVertex v2=Cc3dVertex(Cc3dVector4(-d,h,-d,1),Cc3dVector2(0,1),Cc3dVector4(0,1,0,0));
        Cc3dVertex v3=Cc3dVertex(Cc3dVector4(-d,h,d,1),Cc3dVector2(0,0),Cc3dVector4(0,1,0,0));
        submesh->clearMeshData();
        submesh->getSubMeshData()->addVertex(v0);
        submesh->getSubMeshData()->addVertex(v1);
        submesh->getSubMeshData()->addVertex(v2);
        submesh->getSubMeshData()->addVertex(v3);
        submesh->addIDtri(Cc3dIDTriangle(0, 1, 2));
        submesh->addIDtri(Cc3dIDTriangle(0, 2, 3));
        
        this->getModel()->getMeshByIndex(0)->addSubMesh(submesh);
        return true;
    }

    void loadConfig(const string&fileNameWithExt){
        //------------------获得绝对路径--abc
        string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(fileNameWithExt);
        //打开文件--abc
        FILE  *fp=NULL;
        fp=fopen(pathName.c_str(),"r");
        if(fp==NULL)
        {
            cout<<"Open "<<pathName<<" failed!"<<endl;
            assert(false);
        }
        //------------------提取文件信息--abc
        {
            char buffer[1024]={0};
            //提取seabed高度--abc
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&m_height);
            
        }
        //关闭文件--abc
        fclose(fp);
    }

    
};
#endif /* defined(__HelloOpenGL__seabed__) */
