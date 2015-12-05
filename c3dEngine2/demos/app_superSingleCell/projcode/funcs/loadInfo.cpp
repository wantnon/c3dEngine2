//
//  loadInfo.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-25.
//
//

#include "loadInfo.h"
#include "c3d.h"
#include "myFunc.h"
void loadInfo_light(Cc3dLight*light,const string&fileNameWithExt){
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
        //提取光源位置--abc
        float pos[3];
        fscanf(fp, "%f",&pos[0]);
        fscanf(fp, "%f",&pos[1]);
        fscanf(fp, "%f",&pos[2]);
        light->setPos(pos[0], pos[1], pos[2]);
    }
    //关闭文件--abc
    fclose(fp);
}


void loadInfo_subMesh(Cc3dSubMesh*submesh,const string&meshFilePathShort,const string&texClipPathShort,float scale,Cc3dVector4 offsetVec)
//加载网格--abc
//scale为放缩系数，可以在导入时对顶点坐标进行等比方缩--abc
//offsetVec为偏移量（缩放以后的偏移量)，可在导入时对顶点坐标进行偏移--abc
//由于是等比放缩，所以法线和纹理坐标不受影响--abc
{
    if(meshFilePathShort.empty()==false)assert(meshFilePathShort[(int)meshFilePathShort.size()-1]!='/');
    if(texClipPathShort.empty()==false)assert(texClipPathShort[(int)texClipPathShort.size()-1]!='/');
    assert(submesh->getSubMeshData()->isEmpty());
    //------------------获得纹理文件夹相对路径--abc
    string meshFileName_part1;//相对路径名，不包含文件名--abc
    string meshFileName_part2;//文件名--abc
    vector<string> rs=splitStrInTwoByLastBar(meshFilePathShort);
    meshFileName_part1=rs[0];
    meshFileName_part2=rs[1];
    //------------------获得绝对路径--abc
    string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(meshFilePathShort);
    //打开文件--abc
    FILE  *fp=NULL;
    fp=fopen(pathName.c_str(),"r");
    if(fp==NULL)
    {
        cout<<"Open "<<pathName<<" failed!"<<endl;
       assert(false);
    }
    
    //------------------提取文件信息到submesh
    {
        //提取顶点数--abc
        int nv;
        fscanf(fp, "%d",&nv);
        //提取各顶点--abc
        for(int i=0;i<nv;i++){
            
            //提取pos
            float pos[4]={0,0,0,1};
            fscanf(fp, "%f",&pos[0]);
            fscanf(fp, "%f",&pos[1]);
            fscanf(fp, "%f",&pos[2]);
            //提取texCoord
            float texCoord[2];
            fscanf(fp, "%f",&texCoord[0]);
            fscanf(fp, "%f",&texCoord[1]);
            //提取norm
            float norm[4]={0,0,0,0};
            fscanf(fp, "%f",&norm[0]);
            fscanf(fp, "%f",&norm[1]);
            fscanf(fp, "%f",&norm[2]);
            //将texCoord原点由左下角变为左上角--abc
            texCoord[1]=1-texCoord[1];
            //对顶点进行放缩--abc
            Cc3dVector4 posWithScale=Cc3dVector4(pos)*scale;
            posWithScale.setw(1);
            //对缩放后的顶点进行偏移（注意，一定要放在缩放后面)
            posWithScale=posWithScale+offsetVec;
            //组成顶点--abc
            Cc3dVertex vertex(posWithScale,Cc3dVector2(texCoord),Cc3dVector4(norm));
            //将此顶点加入vlist
            submesh->getSubMeshData()->addVertex(vertex);
        }
        //提取IDtri数--abc
        int nIDtri;
        fscanf(fp, "%d",&nIDtri);
        
        //提取各IDtri
        for(int i=0;i<nIDtri;i++){
            Cc3dIDTriangle IDtri;
            //提取ID0,ID1,ID2
            fscanf(fp, "%d",&IDtri.vID[0]);
            fscanf(fp, "%d",&IDtri.vID[1]);
            fscanf(fp, "%d",&IDtri.vID[2]);
            submesh->getSubMeshData()->addIDtri(IDtri);
        }
        //提取纹理图片名并加载纹理--abc
        {
            char t_imageName[1024];
            fscanf(fp, "%s",t_imageName);
            //将texFileName的后缀名改成.png
            string part1,part2;
            vector<string> rs=splitStrInTwoByLastDotBehindLastBar(t_imageName);
            part1=rs[0];
            part2=rs[1];
            string imageName=part1+".png";
            //生成相对路径--abc
            string texPathName=texClipPathShort+"/"+imageName;
            //根据纹理图片路径加载纹理，并将纹理句柄存入submesh
            submesh->setTexture(Cc3dTextureCache::sharedTextureCache()->addImage(texPathName.c_str()));
        }
        //提取backVisible
        char buffer[1024]={0};
        int backVisible;
        fscanf(fp, "%s",buffer);
        fscanf(fp, "%d",&backVisible);
        if(backVisible){
            submesh->setCullFace(ec3dNotCull);
        }else{
            submesh->setCullFace(ec3dCullBack);
        }
        
    }
    //关闭文件--abc
    fclose(fp);
    
    
}

void loadInfo_mesh(Cc3dMesh*mesh,const string&modelClipPathShort,float scale,Cc3dVector4 offsetVec){//输入模型文件夹短路径名--abc
    if(modelClipPathShort.empty()==false)assert(modelClipPathShort[(int)modelClipPathShort.size()-1]!='/');
  //  mesh->modelClipPathShort=modelClipPathShort;
    //----加载各mesh_normByFace
    if(Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(modelClipPathShort+"/mesh")){
        int i=0;
        while (1) {
            bool pathExists=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(modelClipPathShort+"/mesh/mesh"+numberToStr(i)+".ysm");
            if(pathExists==false)break;
            Cc3dSubMesh *subMesh=new Cc3dSubMesh();
            subMesh->autorelease();
            
            string meshFilePathShort=modelClipPathShort+"/mesh/mesh"+numberToStr(i)+".ysm";
            string texClipPathShort=modelClipPathShort+"/tex";
            loadInfo_subMesh(subMesh,meshFilePathShort,texClipPathShort,scale,offsetVec);
            mesh->addSubMesh(subMesh);
            i++;
        }
        
    }
    //----加载各mesh_normByVertex
    if(Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(modelClipPathShort+"/mesh_normByVertex")){
        int i=0;
        while (1) {
            bool pathExists=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(modelClipPathShort+"/mesh_normByVertex/mesh"+numberToStr(i)+"_normByVertex.ysm");
            if(pathExists==false)break;
            Cc3dSubMesh *subMesh=new Cc3dSubMesh();
            subMesh->autorelease();
            string meshFilePathShort=modelClipPathShort+"/mesh_normByVertex/mesh"+numberToStr(i)+"_normByVertex.ysm";
            string texClipPathShort=modelClipPathShort+"/tex";
            loadInfo_subMesh(subMesh, meshFilePathShort,texClipPathShort,scale,offsetVec );
            mesh->addSubMesh(subMesh);
            reCalNorms_precise_normByVertex(subMesh->getSubMeshData());
            i++;
        }
    }
    //----加载mesh_bossAndCBLoc
    if(Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(modelClipPathShort+"/mesh_bossAndCBLoc")){
        int i=0;
        while (1) {
            bool pathExists=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(modelClipPathShort+"/mesh_bossAndCBLoc/mesh"+numberToStr(i)+"_bossAndCBLoc.ysm");
            if(pathExists==false)break;
            Cc3dSubMesh *subMesh=new Cc3dSubMesh();
            subMesh->autorelease();
            string meshFilePathShort=modelClipPathShort+"/mesh_bossAndCBLoc/mesh"+numberToStr(i)+"_bossAndCBLoc.ysm";
            string texClipPathShort=modelClipPathShort+"/tex";
            loadInfo_subMesh(subMesh, meshFilePathShort,texClipPathShort,scale,offsetVec);
            mesh->addSubMesh(subMesh);
            reCalNorms_precise_normByVertex(subMesh->getSubMeshData());
            i++;
        }
    }
    
    
}