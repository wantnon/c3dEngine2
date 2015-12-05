//
//  geoMath.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-29.
//
//

#ifndef __HelloOpenGL__geoMath__
#define __HelloOpenGL__geoMath__

#include <iostream>
#include <vector>
using namespace std;
#include "core/c3dPlane.h"
#include "core/c3dMath.h"
#include "core/c3dGeoMath.h"


static inline bool point_in_triVolum(const Cc3dVector4&norm,const Cc3dVector4&v0,const Cc3dVector4&v1,const Cc3dVector4&v2,const Cc3dVector4&p)
//判断点是否位于三角形为底的直角柱体内(包括边上)
//必须保证norm为v[0~2]所成三角形的法向量--abc
{
    Cc3dVector4 v[3]={v0,v1,v2};
    //norm是三角形的法向量--abc
    //求三角形各边的垂直于三角形所在平面的平面--abc
    Cc3dPlane planeList[3];
    for(int i=0;i<3;i++){
        const Cc3dVector4&p1=v[i];
        const Cc3dVector4&p2=v[(i+1)%3];
        Cc3dVector4 t=p2-p1;//边的方向向量--abc
        //求边t的垂直于三角形所在平面的平面的法向量(方向指向三角形内部)
        Cc3dVector4 norm2=cross(norm, t);
        //求边t的垂直于多边形所在平面的平面--abc
        planeList[i].init(p1,norm2);
    }//得到planeList
    //根据点p到planeList中各平面的有向距离判断点是否在多边形内--abc
    for(int i=0;i<3;i++){
        Cc3dPlane&plane=planeList[i];
        float side=directedDistanceFromPointToPlane(plane,p);
        if(side<0)return false;
    }
    return true;
}

static inline vector<Cc3dPlane> getLeanedVolum(const Cc3dVector4&dir,const vector<Cc3dVector4> &polygon)
//求以polygon为底dir为方向向量的斜棱柱--abc
//dir为柱体的方向向量,不必为单位向量--abc
{
    //dir和polygon构成一个斜棱柱,看c是否在此柱体之内--abc
    int nv=(int)polygon.size();//polygon顶点数--abc
    //制作柱体的nv个侧壁（法向量指向内)
    vector<Cc3dPlane> planeList(nv);
    //开始制作nv个侧壁--abc
    for(int j=0;j<nv;j++){
        //求过v[j],v[(j+1)%nv]边的侧壁--abc
        //求此侧壁的法向量--abc
        Cc3dVector4 norm;//侧壁法向量--abc
        Cc3dVector4 VjVnext;//边v[j]v[(j+1)%nv]
        VjVnext=polygon[(j+1)%nv]-polygon[j];//得到VjVnext
        norm=cross(dir, VjVnext);
        norm=normalize(norm);
        //生成侧壁平面--abc
        planeList[j].init(polygon[j],norm);
        
    }//得到planeList
    return planeList;
}
static inline bool isPointInVolum(const vector<Cc3dPlane>&planeList,const Cc3dVector4&p,const float margin=0)
//判断点p是否在planeList形成的volume（再放大margin)内（包括边上)
{
    int nPlane=(int)planeList.size();
    for(int i=0;i<nPlane;i++){
        const Cc3dPlane&plane=planeList[i];
        float side=directedDistanceFromPointToPlane(plane,p);
        if(side<-margin)return false;
    }
    return true;
}
static inline bool isPointInLeanedVolum(const Cc3dVector4&dir,const vector<Cc3dVector4> &polygon,const Cc3dVector4&p,const float margin=0)
//判断点p是否位于以polygon为底dir为方向的斜棱柱(再放大margin)内(包括边上)
{
    const vector<Cc3dPlane> planeList=getLeanedVolum(dir, polygon);
    return isPointInVolum(planeList, p,margin);
}



static inline bool getPointFootOnTri(const Cc3dVector4&p,
                       const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2,const Cc3dVector4&norm,//norm为三角面p0p1p2的法向量--abc
                       Cc3dVector4&foot
                       ){//求点p到三角面p0p1p2的垂足,前端返回垂足是否落在三角面内--abc
    //求p0p1p2所在平面--abc
    Cc3dPlane plane;
    plane.init(p0,norm);
    //求垂足--abc
    float PND=directedDistanceFromPointToPlane(plane,p);//p到plane的有向距离--abc
    foot=p+plane.getNorm()*(-PND);
    foot.setw(1);//得到foot
    //判断foot是否在三角面内--abc
    return point_in_triVolum(norm,p0,p1,p2,p);
}

static inline bool getPointFootOnLine(const Cc3dVector4&p,const Cc3dVector4&p0,const Cc3dVector4&p1,Cc3dVector4&foot){//求点p到直线p0p1的垂足,前端返回垂足是否落在线段p0p1上--abc
    assert(p.w()==1);
    assert(p0.w()==1);
    assert(p1.w()==1);
    //公式为foot=p0+p0p1/|p0p1|*p0p*p0p1/|p0p1|
    Cc3dVector4 p0p1=p1-p0;
    Cc3dVector4 p0p=p-p0;
    Cc3dVector4 e_p0p1=normalize(p0p1);//p0p1的单位向量--abc
    Cc3dVector4 p0foot;
    float t=dot(p0p,e_p0p1);//p0沿p0p1到foot的有向长度--abc
    p0foot=e_p0p1*t;
    foot=p0+p0foot;//得到foot
    //判断foot是否在p0,p1之间--abc
    if(t<0){
        return false;
    }else{
        if(t*t>getLength(p0p1)){
            return false;
        }else{
            return true;
        }
    }
    
}

static inline float minDis_point_Tri(const Cc3dVector4&point,
                       const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2,const Cc3dVector4&norm,//norm为三角面p0p1p2的法向量--abc
                       Cc3dVector4&minp){//求点point到三角面p0p1p2的最短距离,前端返回最小距离值,minp带回取得最小距离的点--abc
    //----制作关键点列表和有效性列表--abc
    Cc3dVector4 p[7];//关键点列表（考察的7个关键点是：point到三角面垂足,point到三角面的三条边的垂足,三角面三个顶点)
    bool valid[7];//垂足的有效性列表--abc
    //point在平面上的垂足--abc
    valid[0]=getPointFootOnTri(point,p0,p1,p2,norm,p[0]);
    if(valid[0]==true){//如果垂足在三角面内,则此必为最短距离,不用再往下算,此为一优化--abc
        Cc3dVector4 vec=point-p[0];
        return  getLength(vec);
    }
    //point到边p0p1的垂足--abc
    valid[1]=getPointFootOnLine(point,p0,p1,p[1]);
    //求point到边p1p2的垂足--abc
    valid[2]=getPointFootOnLine(point,p1,p2,p[2]);
    //求point到边p2p0的垂足--abc
    valid[3]=getPointFootOnLine(point,p2,p0,p[3]);
    //p0
    valid[4]=true;
    p[4]=p0;
    //p1
    valid[5]=true;
    p[5]=p1;
    //p2
    valid[6]=true;
    p[6]=p2;
    //----制作距离列表--abc
    float d[7];//距离列表--abc
    for(int i=0;i<=6;i++){
        Cc3dVector4 vec=point-p[i];
        d[i]=getLength(vec);
    }//得到d
    //----求最短距离及取得此距离的点--abc
    float minDis=c3d_INF;
    for(int i=0;i<=6;i++){
        if(valid[i]==true&&d[i]<minDis){
            minDis=d[i];
            minp=p[i];
        }
    }//得到minDis和minp
    return minDis;
}
static inline int getPointOfIntersect_line_plane_np(const Cc3dVector4&p1,const Cc3dVector4&p2,const Cc3dPlane&plane,Cc3dVector4&p3)
//getPointOfIntersect_line_plane的欠精确版本--abc
//-----------------------------------------------------------------
//求p1p2所确定的直线与平面plane的交点p3（注意是直线而非线段)
//前端返回0表示无交点--abc
//返回1表示恰有一个交点--abc
//返回2表示有无穷多交点--abc
//有无穷多交点时p3直接等于p1
{
    assert(p1.w()==1);
    assert(p2.w()==1);
    //求直线p1p2与平面的交点p3
    const Cc3dVector4&norm=plane.getNorm();
    const Cc3dVector4&p=plane.getPoint();
    Cc3dVector4 v=normalize(p2-p1);//直线的单位方向向量--abc
    Cc3dVector4 p1p=p-p1;
    //p0到平面的有向距离t
    double a=dot(p1p,norm);
    double b=dot(norm,v);
    if(a==0&&b==0){//无穷多交点--abc
        p3=p1;
        return 2;
    }else if(a!=0&&b==0){//无交点--abc
        return 0;
    }else{//恰有一交点--abc
        float t=a/b;
        //求交点p3
        Cc3dVector4 vt=v*t;
        p3=p1+vt;//得到p3
        return 1;
    }
}

static inline float d2_pointToRay(const Cc3dVector4&p,const Cc3dVector4&dir,const Cc3dVector4&c)
//求空间点c到射线(p,dir)的距离的平方,如果点到射线所在直线的垂足落在射线外,则规定距离为无穷--abc
{
    Cc3dVector4 pc=c-p;
    if(dot(dir, pc)<0){
        return c3d_INF;
    }else{
        Cc3dVector4 crossProduct=cross(dir, pc);
        return getLength2(crossProduct);
    }

}

#endif /* defined(__HelloOpenGL__geoMath__) */
