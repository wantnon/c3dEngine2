//
//  collisonGrid.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-28.
//
//

#ifndef __HelloOpenGL__collisonGrid__
#define __HelloOpenGL__collisonGrid__

#include <iostream>
#include <vector>
using namespace std;
#include "c3d.h"
class Cgridcell
{
public:
    vector<Cc3dNode*> pModelList;
    
};
class CcollisionGrid
{
public:
    vector<vector<vector<Cgridcell> > > grid;
    vector<Cgridcell*> pdirtyList;//脏格子列表（如果一个格子含有多个node，则此格指针在pdirtyList中出现多次)
    float dx,dy,dz;//每个格子的尺寸--abc
    float xmin,xmax,ymin,ymax,zmin,zmax;//grid范围，应保证被管理的模型均不超出grid范围，超出者将被忽略--abc
    CcollisionGrid(){
        setDefaultValueForMembers();
    }
    void setDefaultValueForMembers(){
        dx=dy=dz=0;
        xmin=xmax=ymin=ymax=zmin=zmax=0;
    }
    void clear(){
        grid.clear();
        pdirtyList.clear();
        setDefaultValueForMembers();
    
    }
    void init(float _xmin,float _xmax,float _ymin,float _ymax,float _zmin,float _zmax,
              float _dx,float _dy,float _dz,
              int nxMax=-1,int nyMax=-1,int nzMax=-1)
    //nxMax,nyMax,nzMax分别为x,y,z方向上的最大段数--abc
    
    {
        dx=_dx;
        dy=_dy;
        dz=_dz;
        xmin=_xmin;
        xmax=_xmax;
        ymin=_ymin;
        ymax=_ymax;
        zmin=_zmin;
        zmax=_zmax;
        float xSpan=_xmax-_xmin;
        float ySpan=_ymax-_ymin;
        float zSpan=_zmax-_zmin;
        int nx=ceil(xSpan/_dx);
        int ny=ceil(ySpan/_dy);
        int nz=ceil(zSpan/_dz);
        if(nxMax!=-1&&nx>nxMax){//如果越过nxMax，则重设置dx和nx
            dx=xSpan/nxMax;
            nx=nxMax;
        }
        if(nyMax!=-1&&ny>nyMax){//如果越过nyMax，则重设置dy和ny
            dy=ySpan/nyMax;
            ny=nyMax;
        }
        if(nzMax!=-1&&nz>nzMax){//如果越过nzMax，则重设置dz和nz
            dz=zSpan/nzMax;
            nz=nzMax;
        }
        //重新计算xmax,ymax,zmax
        xmax=xmin+nx*dx;
        ymax=ymin+ny*dy;
        zmax=zmin+nz*dz;
        //用nx,ny,nz初始化grid
        {
            grid.resize(nx);
            for(int i=0;i<nx;i++){
                grid[i].resize(ny);
                for(int j=0;j<ny;j++){
                    grid[i][j].resize(nz);
                }
            }
        }
    
    }
    
    void print(){
        int m1=grid.size();
        int m2=grid[0].size();
        int m3=grid[0][0].size();
        for(int i=0;i<m1;i++){
            for(int j=0;j<m2;j++){
                for(int k=0;k<m3;k++){
                    cout<<(int)grid[i][j][k].pModelList.size()<<" ";
                }
                cout<<endl;
            }
            cout<<endl;
        }
    }
    vector<Cc3dNode*> fillGrid(const vector<Cc3dNode*>&pModelBaseList,const vector<bool>&maskList)
    //maskList为屏蔽列表，如果maskList[i]==false，则跳过第i个model，不填充到grid
    //返回未mask且出界的元素列表--abc
    {
        if((int)maskList.size()!=(int)pModelBaseList.size()){
            cout<<"error:maskList元素数不正确!"<<endl;
            assert(false);
        }
        //----清空pdirtyList，因为此时已没有boss相关的脏格子了--abc
        for(int i=0;i<(int)grid.size();i++){
            for(int j=0;j<(int)grid[i].size();j++){
                for(int k=0;k<(int)grid[i][j].size();k++){
                    grid[i][j][k].pModelList.clear();
                }
            }
        }
        //----
        vector<Cc3dNode*> outRangeList;//出界列表--abc
        if((int)grid.size()==0||(int)grid[0].size()==0||(int)grid[0][0].size()==0)return outRangeList;
        int Imax=(int)grid.size()-1;
        int Jmax=(int)grid[0].size()-1;
        int Kmax=(int)grid[0][0].size()-1;
        int n=(int)pModelBaseList.size();
        for(int i=0;i<n;i++){
            if(maskList[i])continue;//跳过屏蔽的node
            Cc3dNode*p=pModelBaseList[i];
            Cc3dVector4 c=p->getPos();
            //看c落在哪个格子里，落在grid[I][J][K]
            int I=(int)((c.x()-xmin)/dx);
            int J=(int)((c.y()-ymin)/dy);
            int K=(int)((c.z()-zmin)/dz);
            if(I<0||I>Imax||J<0||J>Jmax||K<0||K>Kmax){//如果出界--abc
                //加入到出界列表--abc
                outRangeList.push_back(p);
                continue;//跳过--abc
            }
            grid[I][J][K].pModelList.push_back(p);
    //        pdirtyList.push_back(&grid[I][J][K]);
        }
        return outRangeList;
    }
    vector<Cc3dNode*> getCollisonList(const Cc3dVector4&pos)const
    //必须确保传入的pCollisionBossCellList为空--abc
    {
        vector<Cc3dNode*> pCollisionModelList;
        if((int)grid.size()==0||(int)grid[0].size()==0||(int)grid[0][0].size()==0){
            return pCollisionModelList;
        }
        int Imax=(int)grid.size()-1;
        int Jmax=(int)grid[0].size()-1;
        int Kmax=(int)grid[0][0].size()-1;
        //将pos所在格及周周围26个邻格中的bossCell加入到pCollisionBossCellList
        //求pos所在格--abc
        int I=(int)((pos.x()-xmin)/dx);
        int J=(int)((pos.y()-ymin)/dy);
        int K=(int)((pos.z()-zmin)/dz);
        for(int i=I-1;i<=I+1;i++){
            for(int j=J-1;j<=J+1;j++){
                for(int k=K-1;k<=K+1;k++){
                    if(i<0||i>Imax||j<0||j>Jmax||k<0||k>Kmax)continue;
                    int nMesh=(int)grid[i][j][k].pModelList.size();
                    for(int u=0;u<nMesh;u++){
                        Cc3dNode*mesh=grid[i][j][k].pModelList[u];
                        pCollisionModelList.push_back(mesh);
                    }
                }
            }
        }
        return pCollisionModelList;
    }
};
#endif /* defined(__HelloOpenGL__collisonGrid__) */
