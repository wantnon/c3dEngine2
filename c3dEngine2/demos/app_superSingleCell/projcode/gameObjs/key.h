//
//  key.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-17.
//
//

#ifndef __HelloOpenGL__key__
#define __HelloOpenGL__key__

#include <iostream>
#include "c3d.h"
#include "timeCounter.h"
class Ckey:public Cc3dActor
{
public:
    float keyDownColor[4];
    float keyUpColor[4];
    float xmin,xmax,ymin,ymax;
    float transpColor[4];
    bool isGrey;//变灰--abc
 //   bool invisible;//不可见（同时也不再起作用)
    bool haveBeenDisplayed;//已经显示过--abc
    CtimeCounter disapearCounter;//多长时间消失--abc
    bool isDown;
    Ckey(){
        haveBeenDisplayed=false;
        isGrey=false;
   //     invisible=false;
        isDown=false;
        transpColor[0]=0;
        transpColor[1]=0;
        transpColor[2]=0;
        transpColor[3]=0;
        
        keyDownColor[0]=0;
        keyDownColor[1]=0;
        keyDownColor[2]=0;
        keyDownColor[3]=1;
   
        keyUpColor[0]=1;
        keyUpColor[1]=1;
        keyUpColor[2]=1;
        keyUpColor[3]=1;

        xmin=xmax=ymin=ymax=0;
    }
    void setIsDown(bool value){
        isDown=value;
    }
    bool getIsDown()const{return isDown;}
    void disapearCount(){
        disapearCounter.doCount_dec();
        if(disapearCounter.getCount()==0){
      //      invisible=true;
            setIsVisible(false);
        }
    }
    void setDisapearCountValue(long disapearCountValue){
        disapearCounter.setValue(disapearCountValue);
    }
    void set_isGrey(bool _isGrey){
        isGrey=_isGrey;
    }
    bool get_isGrey()const{
        return isGrey;
    }
 /*   void set_invisible(bool _invisible){
        invisible=_invisible;
    }
    bool get_invisible()const{
        return invisible;
    }*/

    void reSetWidth(float _width){//重设宽度--abc
        xmax=xmin+_width;
        //更新顶点坐标(1号顶点和2号顶点)
        Cc3dVertex v1=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(1);
        Cc3dVertex v2=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(2);
        Cc3dVector4 pos1=v1.getPos();
        Cc3dVector4 pos2=v2.getPos();
        pos1.setx(xmax);
        pos2.setx(xmax);
        v1.setPos(pos1);
        v2.setPos(pos2);
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(1,v1);
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(2,v2);
        
    
    }
    void reSetHeight(float _height){//重设高度--abc
        ymax=ymin+_height;
        //更新顶点坐标(0号顶点和1号顶点)
        Cc3dVertex v2=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(2);
        Cc3dVertex v3=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(3);
        Cc3dVector4 pos2=v2.getPos();
        Cc3dVector4 pos3=v3.getPos();
        pos2.sety(ymax);
        pos3.sety(ymax);
        v2.setPos(pos2);
        v3.setPos(pos3);
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(2,v2);
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(3,v3);
        
    }
    float getCenterX()const {//获得中心x坐标--abc
        return (xmin+xmax)/2;
    }
    float getCenterY()const {//获得中心y坐标--abc
        return (ymin+ymax)/2;
    }
    void reSetCenter(float x,float y){//重设位置--abc
        //求原中心位置--abc
        float xf=(xmin+xmax)/2;
        float yf=(ymin+ymax)/2;
        //求平移量--abc
        float dx=x-xf;
        float dy=y-yf;
        //更新范围--abc
        xmin+=dx;
        xmax+=dx;
        ymin+=dy;
        ymax+=dy;
        //更新顶点--abc
        int nv=(int)getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->getVertexCount();
        for(int i=0;i<nv;i++){
            Cc3dVertex v=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(i);
            Cc3dVector4 pos=v.getPos();
            pos.setx(pos.x()+dx);
            pos.sety(pos.y()+dy);
            v.setPos(pos);
            getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(i,v);
        }
    
    }
    void reSetLeftDownCorner(float x,float y){//重设位置--abc
        //求原左下角位置--abc
        float xf=xmin;
        float yf=ymin;
        //求平移量--abc
        float dx=x-xf;
        float dy=y-yf;
        //更新范围--abc
        xmin+=dx;
        xmax+=dx;
        ymin+=dy;
        ymax+=dy;
        //更新顶点--abc
        int nv=(int)getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->getVertexCount();
        for(int i=0;i<nv;i++){
            Cc3dVertex v=getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->getVertexByIndex(i);
            Cc3dVector4 pos=v.getPos();
            pos.setx(pos.x()+dx);
            pos.sety(pos.y()+dy);
            v.setPos(pos);
            getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->setVertexByIndex(i,v);
        }
        
    }

    void genAndInit(
                    float _xmin,float _xmax,float _ymin,float _ymax,//坐标范围--abc
                    float _smin,float _smax,float _tmin,float _tmax//纹理坐标范围--abc
               ){
                   assert((int)this->getModel()->getMeshCount()==0);
        xmin=_xmin;
        xmax=_xmax;
        ymin=_ymin;
        ymax=_ymax;
        //制作网格--abc
        Cc3dMesh*mesh=new Cc3dMesh();
                   mesh->autorelease();
        this->addMesh(mesh);
        Cc3dSubMesh*subMesh=new Cc3dSubMesh();
                   subMesh->autorelease();
        this->getModel()->getMeshByIndex(0)->addSubMesh(subMesh);
        Cc3dVertex v0=Cc3dVertex(Cc3dVector4(xmin,ymin,1,1),Cc3dVector2(_smin,_tmax),Cc3dVector4(0,0,0,0));
        Cc3dVertex v1=Cc3dVertex(Cc3dVector4(xmax,ymin,1,1),Cc3dVector2(_smax,_tmax),Cc3dVector4(0,0,0,0));
        Cc3dVertex v2=Cc3dVertex(Cc3dVector4(xmax,ymax,1,1),Cc3dVector2(_smax,_tmin),Cc3dVector4(0,0,0,0));
        Cc3dVertex v3=Cc3dVertex(Cc3dVector4(xmin,ymax,1,1),Cc3dVector2(_smin,_tmin),Cc3dVector4(0,0,0,0));
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->addVertex(v0);
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->addVertex(v1);
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->addVertex(v2);
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->addVertex(v3);
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->clearIDtriList();
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->addIDtri(Cc3dIDTriangle(0, 1, 2));
        getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getSubMeshData()->addIDtri(Cc3dIDTriangle(0, 2, 3));
    }
    bool isContainPoint(const Cc3dVector2&point)
    {
        float x=point.x();
        float y=point.y();
        if (x>=xmin&&x<=xmax&&y>=ymin&&y<=ymax){
            return true;
        }else{
            return false;
        }
    }
    void setKeyDownColor(float r,float g,float b,float a){
        keyDownColor[0]=r;
        keyDownColor[1]=g;
        keyDownColor[2]=b;
        keyDownColor[3]=a;
        
    }
    void setKeyUpColor(float r,float g,float b,float a){
        keyUpColor[0]=r;
        keyUpColor[1]=g;
        keyUpColor[2]=b;
        keyUpColor[3]=a;
        
    }
    void useKeyDownColor(){
        this->getModel()->getMeshByIndex(0)->setColor(keyDownColor[0],keyDownColor[1],keyDownColor[2],keyDownColor[3]);
        
    }
    void useKeyUpColor(){
        this->getModel()->getMeshByIndex(0)->setColor(keyUpColor[0],keyUpColor[1],keyUpColor[2],keyUpColor[3]);
    }

    
};
#endif /* defined(__HelloOpenGL__key__) */
