//
//  sky.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-3-17.
//
//

#ifndef __HelloOpenGL__sky__
#define __HelloOpenGL__sky__

#include <iostream>
#include "c3d.h"
class CskySemiBall:public Cc3dActor
{
    int skyTexID;
    bool isExist;

public:
    CskySemiBall(){
        isExist=true;
        skyTexID=0;
    }
    void set_isExist(bool _isExist){
        isExist=_isExist;
    
    }
    bool init(){
        Cc3dMesh*mesh=new Cc3dMesh();
        mesh->autorelease();
        addMesh(mesh);
        return true;
    }
    Cc3dMesh*getMesh()const {
        assert((int)getModel()->getMeshCount()==1);
        return getModel()->getMeshByIndex(0);
    }
    bool get_isExist()const {
        return isExist;
    }
    int get_skyTexID() const{
        return skyTexID;
    }
    void scaleToFitR(float _R){//放缩，使半球的半径等于_R
        Cc3dRange range=getModel()->getMeshByIndex(0)->getRange();
        float r=range.getMaxY()-range.getMinY();
        //求由r放缩到_R的放缩比--abc
        float k=_R/r;
        //对模型进行放缩--abc
        this->getModel()->getMeshByIndex(0)->scale_modifyVertexDirectly(k, k, k);
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
        //------------------提取文件信息，并建立列模型列表--abc
        {
            char buffer[1024]={0};
            //提取skyTexID
            int skyTexID;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%d",&skyTexID);
            this->skyTexID=skyTexID;
            //提取centerY
            float centerY;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&centerY);
            this->m_transform->setPosY(centerY);
            //提取initRotAngle
            float A;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&A);
            float cosA=cosf(A*c3d_PI/180);
            float sinA=sinf(A*c3d_PI/180);
            this->m_transform->setRot(0, 1, 0, cosA, sinA);
        }
        //关闭文件--abc
        fclose(fp);
    }

      
};
#endif /* defined(__HelloOpenGL__sky__) */
