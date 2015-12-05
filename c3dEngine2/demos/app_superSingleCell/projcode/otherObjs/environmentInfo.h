//
//  environmentInfo.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-2-12.
//
//

#ifndef __HelloOpenGL__environmentInfo__
#define __HelloOpenGL__environmentInfo__

#include <iostream>
#include "c3d.h"
#include "mobilePlatform.h"
#include "archScene.h"
#include "ground.h"
#include "water.h"
//surface type
#define SURFACETYPE_WATER 0
#define SURFACETYPE_GROUND 1
#define SURFACETYPE_ARCH 2
#define SURFACETYPE_MOBILEPLATFORM 3
//ceil type
#define CEILTYPE_NON 0
#define CEILTYPE_ARCH 1
#define CEILTYPE_MOBILEPLATFORM 2
class CenvironmentInfo
{
public:
    //最近上下表面高度，法向和类型--abc
    float ceilH;//天花板高度--abc
    Cc3dVector4 ceilNorm;//天花板法向--abc
    float surfaceH;//表面高度--abc
    Cc3dVector4 surfaceNorm;//表面法向--abc
    int surfaceType;//表面类型--abc
    int ceilType;//天花板类型--abc
    //地面，水面，建筑，移动平台的各表面及法向数据也保存下来--abc
    float groundH;
    float waterH;
    float archSurfaceH;
    float archCeilH;
    float mobilePlatformSurfaceH;
    float mobilePlatformCeilH;
    Cc3dVector4 groundNorm;
    Cc3dVector4 waterNorm;
    Cc3dVector4 archSurfaceNorm;
    Cc3dVector4 archCeilNorm;
    Cc3dVector4 mobilePlatformSurfaceNorm;
    Cc3dVector4 mobilePlatformCeilNorm;
    CmobilePlatform*pSurfaceMobilePlatform;//指向当前所在surface的平台，仅surfaceType==SURFACETYPE_MOBILEPLATFORM时有效--abc
    CmobilePlatform*pCeilMobilePlatform;//指向当前所在ceil所属平台，仅ceilType==CEILTYPE_MOBILEPLATFORM时有效--abc
    CenvironmentInfo(){
        ceilH=c3d_INF;
        surfaceH=0;
        surfaceNorm.init(0,1,0,0);
        ceilNorm.init(0, -1, 0, 0);
        surfaceType=0;
        ceilType=0;
        groundH=0;
        waterH=0;
        archCeilH=0;
        archSurfaceH=0;
        mobilePlatformSurfaceH=0;
        mobilePlatformCeilH=0;
        groundNorm.init(0, 1, 0, 0);
        waterNorm.init(0,1,0,0);
        archSurfaceNorm.init(0,1,0,0);
        archCeilNorm.init(0,-1,0,0);
        mobilePlatformSurfaceNorm.init(0, 1, 0, 0);
        mobilePlatformCeilNorm.init(0,-1,0,0);
        pCeilMobilePlatform=NULL;
        pSurfaceMobilePlatform=NULL;
        
    }
    
    void updateInfo(const Cc3dVector4&pos,float Rc,float Rc_small,const Cground*ground,const Cwater*water,CarchScene*archScene,
                    const CmobilePlatformSet*mobilePlatformSet){
        //获得地面高度和法向--abc
        if(ground){
            groundH=ground->getHAndNormal(pos.x(), pos.z(), groundNorm);
        }else{
            groundH=-c3d_INF;
            //groundNorm不作处理--abc
        }
        //获得水面高度和法向--abc
        if(water){
           waterH=water->getHorizon();
            waterNorm.init(0, 1, 0, 0);
        }else{
            waterH=-c3d_INF;
            //waterNorm不作处理--abc
        }
     
        //获得arch表面高度和法线，天花板高度和法线--abc
        if(archScene){
            //（要用Rc而不是Rc_small，否则导致ship着陆下陷)
            archSurfaceH=archScene->getH_floor(pos,Rc,archSurfaceNorm);
            archCeilH=archScene->getH_ceil(pos, Rc, archCeilNorm);
        }else{
            archSurfaceH=-c3d_INF;
            archCeilH=c3d_INF;
        }
        //获得mobilePlatformSet的表面高度和法线，天花板高度和法线，及相应的mobilePlatform
        if(mobilePlatformSet){
            //（要用Rc而不是Rc_small，否则导致ship着陆下陷)
            mobilePlatformSurfaceH=mobilePlatformSet->getH_floor(pos,Rc,mobilePlatformSurfaceNorm,pSurfaceMobilePlatform);
            mobilePlatformCeilH=mobilePlatformSet->getH_ceil(pos, Rc, mobilePlatformCeilNorm,pCeilMobilePlatform);
        }else{
            mobilePlatformSurfaceH=-c3d_INF;
            mobilePlatformCeilH=c3d_INF;
        }
        //----ceilH取archCeil和mobilePlatformCeil中较低者，ceilNorm取较低者的norm
        if(archCeilH<=mobilePlatformCeilH){//如果archCeil低--abc
            ceilH=archCeilH;
            ceilNorm=archCeilNorm;
            ceilType=CEILTYPE_ARCH;
        }else{//如果mobilePlatformCeilH低--abc
            ceilH=mobilePlatformCeilH;
            ceilNorm=mobilePlatformCeilNorm;
            ceilType=CEILTYPE_MOBILEPLATFORM;
        }
        //----比较seabedH,groundH,waterH,archSurfaceH,mobilePlatformSurfaceH几者哪个高，则surfaceH就取谁--abc
        const int nSurface=4;
        float surfaceHList[nSurface]={groundH,waterH,archSurfaceH,mobilePlatformSurfaceH};
        int surfaceTypeList[nSurface]={SURFACETYPE_GROUND,SURFACETYPE_WATER,SURFACETYPE_ARCH,SURFACETYPE_MOBILEPLATFORM};
        Cc3dVector4 surfaceNormList[nSurface]={groundNorm,
            waterNorm,archSurfaceNorm,
            mobilePlatformSurfaceNorm};
        int imax=0;//surfaceH最高者的索引号--abc
        float Hmax=surfaceHList[0];//surfaceH的最大值--abc
        for(int i=1;i<nSurface;i++){//从1开始--abc
            if(surfaceHList[i]>Hmax){
                Hmax=surfaceHList[i];
                imax=i;
            }
        }//得到imax
        surfaceH=surfaceHList[imax];
        surfaceNorm=surfaceNormList[imax];
        surfaceType=surfaceTypeList[imax];
    }
};

#endif /* defined(__HelloOpenGL__environmentInfo__) */
