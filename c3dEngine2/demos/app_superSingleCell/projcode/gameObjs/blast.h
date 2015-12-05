//
//  blast.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-3-1.
//
//

#ifndef __HelloOpenGL__blast__
#define __HelloOpenGL__blast__

#include <iostream>
#include "molecule.h"
#include "c3d.h"
#define BLASTTYPE_small 0
#define BLASTTYPE_blast_big0 1
class Cblast:public Cmolecule
{
public:
    int blastType;
    bool occQuery_haveVisiblePixel;
    virtual~Cblast(){};
    Cblast(){
        occQuery_haveVisiblePixel=true;
        blastType=BLASTTYPE_small;
    }
    static int blastTypeStrToblastTypeID(const string&blastTypeStr) {
        int _blastType=0;
        if(blastTypeStr=="blast_big0"){
            _blastType=BLASTTYPE_blast_big0;
        }else if(blastTypeStr=="blast_small"){
            _blastType=BLASTTYPE_small;
        }else{
            cout<<"error:未知的blastClipName!"<<endl;
			assert(false);
        }
        return _blastType;

    }
    void set_blastType(string blastTypeStr){
        blastType=blastTypeStrToblastTypeID(blastTypeStr);
    }
    int get_blastType()const {
        return blastType;
    }
};
class CblastSet:public CactorSet
{
public:
    
    void layBlast(const Cc3dVector4&_pos,Cc3dCamera&camera,int blastType,
                  const Cc3dVector4&pcollisionFaceNorm=Cc3dVector4(0,0,0,0))const//在pos处放一个爆炸--abc
    //collisionFaceNorm指明碰撞面的法向量--abc
    {
        assert(_pos.w()==1);
        Cc3dVector4 pos=_pos;
        //在pBurstList中找一个died==true且blastType传入的balstType者--abc
        Cblast*pblast=NULL;
        int n=actorList.size();
        for(int i=0;i<n;i++){
            Cblast*tpBlast=(Cblast*)actorList[i];
            if(tpBlast->getDied()&&tpBlast->get_blastType()==blastType){
                pblast=tpBlast;
                break;
            }
        }
        if(pblast){//如果找到了--abc
            //放在pos处--abc
            //为了避免公告牌被碰撞面裁切的问题，要将位置向视点拉近一些--abc
            {
                //获得pblast的宽度--abc
                float Rc=pblast->getRc();
                //将计算由pos向camera拉近rx后的位置--abc
                const Cc3dVector4&eyePos=camera.getEyePos();
                //由pos到eyePos的向量--abc
                Cc3dVector4 e_posToEyePos=eyePos-pos;
                e_posToEyePos=normalize(e_posToEyePos);
                if(isEqual(pcollisionFaceNorm,Cc3dVector4(0,0,0,0))==false){//如果提供了有反弹向量--abc
                    //由e_posToEyePos,collsionFaceNorm和Rc计算最终位置--abc
                    float cosA=dot(pcollisionFaceNorm, e_posToEyePos);
                    cosA=maxf(cosA, 0.5);//如果A小于60度则按60度算，这是为了防止cosA过小从而使下面计算出的pullBackLen过大--abc
                    float pullBackLen=Rc/cosA;//沿e_posToEyePos拉回的距离--abc
                    Cc3dVector4 pullBackVec;//拉近向量--abc
                    pullBackVec=e_posToEyePos*pullBackLen;
                    pos=pos+pullBackVec;//此时pos为拉近后的结果--abc
                }else{//如果没有提供反弹向量--abc
                    //由e_posToEyePos和Rc计算最终位置--abc
                    Cc3dVector4 pullBackVec;//拉近向量--abc
                    pullBackVec=e_posToEyePos*Rc;
                    pos=pullBackVec+pos;//此时pos为拉近后的结果--abc
                }
            }
            pblast->setPos(pos);
            pblast->adjustFace(camera.getEyePos());
            pblast->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STREAM_DRAW);
            pblast->setDied(false);
            pblast->setIsVisible(true);
            //声音--abc
            {
                //播放爆炸声音--abc
            //    pblast->pALSource->updatePosition(pblast->getPos());
            //    pblast->pALSource->play();
                if(pblast->getALSourceByName("bigBlast")){
                    pblast->getALSourceByName("bigBlast")->updatePosition(pblast->getPos());
                    pblast->getALSourceByName("bigBlast")->play();
                }
            }
        }
        
    }
    void doBlast(Cc3dCamera*camera){//所有died=false的blast播放动画，播放完后died置为true
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            Cblast*pblast=(Cblast*)actorList[i];
            if(pblast->getDied())continue;
            pblast->adjustFace(camera->getEyePos());
            pblast->getMesh()->getSubMeshByIndex(0)->submitVertex(GL_STREAM_DRAW);
            bool reachEnd=pblast->useNextFrameTex();
            if(reachEnd){
                pblast->setDied(true);
                pblast->setIsVisible(false);
            }
        }

    
    }
    
};
#endif /* defined(__HelloOpenGL__blast__) */
