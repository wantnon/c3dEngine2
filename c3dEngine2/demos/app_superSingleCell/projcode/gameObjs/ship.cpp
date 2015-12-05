//
//  ship.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-2-11.
//
//

#include "ship.h"
#include "makeTemplate.h"
#include "c3d.h"
void CshipSet::loadConfig(const string&fileNameWithExt){
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
    //------------------提取文件信息，并建立模型列表--abc
    {
        char buffer[1024]={0};
        //提取项目数--abc
        int n;
        fscanf(fp, "%d",&n);
        //提取项目--abc
        for(int i=0;i<n;i++){
            //  Cship*pship=new Cship();
            //----pos
            float initPos[4]={0,0,0,0};
            fscanf(fp,"%s",buffer);
            if(buffer[0]=='#'){
                cout<<"error:提前遇到#!"<<endl;
                assert(false);
            }
            
            fscanf(fp, "%f",&initPos[0]);
            fscanf(fp, "%f",&initPos[1]);
            fscanf(fp, "%f",&initPos[2]);
            //提取初始旋转矩阵--abc
            float initRmat[16];
            for(int j=0;j<16;j++){
                fscanf(fp, "%f",&initRmat[j]);
            }
            
            //提取模板ID
            int templateID;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%d",&templateID);
            //ship拷贝相应的模板--abc
            Cship*pship=makeShipTemplate(templateID);
            //根据spos和initRmat设置transf.RTmat
            Cc3dTransform transf;
            transf.setRmat(initRmat);
            transf.setPos(initPos[0], initPos[1], initPos[2]);
            pship->getTransform()->setRTmat(transf.getRTmat());
            pship->templateID=templateID;
            //将pship加入到pModelBaseList
            this->addActor(pship);
        }
        fscanf(fp, "%s",buffer);
        if(buffer[0]!='#'){
            cout<<"error:缺少#或实际条目数比n值大!"<<endl;
            assert(false);
        }
    }
    //关闭文件--abc
    fclose(fp);
}
