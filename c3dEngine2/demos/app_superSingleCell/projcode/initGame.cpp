//
//  initGame.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-23.
//
//

#include "initGame.h"
//-------------------to do: include your headers
#include "c3d.h"
#include "funcs/programSetUp.h"
#include "global/define.h"
#include "otherObjs/userConfig.h"
#include "global/globalVars.h"
//pages
#include "pages/homePage.h"
#include "pages/menuPage.h"
#include "pages/startPage.h"
#include "pages/settingPage.h"
#include "pages/aboutPage.h"
#include "pages/endPage.h"
#include "pages/loadingPage.h"
#include "pages/succedPage.h"
#include "pages/descPage2.h"
#include "pages/descPage3.h"
#include "pages/descPage.h"
#include "pages/gameOverPage.h"
#include "pages/pausePage.h"
#include "pages/viewingTipPage.h"

bool initGame(){

	//---------------------------------to do: initialize your game
    //----初始化shader
    programSetUp();//new
    cout<<"shader setup done"<<endl;
    
    //xxx
    /*     //----地形纹理混合用到的各纹理--abc
     {
     program_diffuse_ambient_noTransf_noSelfShadow_texBlend.useProgram();//必须先useProgram然后才能传值--abc
     glUniform1i(program_diffuse_ambient_noTransf_noSelfShadow_texBlend.unifoSet.alphaMap.loc, 4);
     glUniform1i(program_diffuse_ambient_noTransf_noSelfShadow_texBlend.unifoSet.texture1.loc, 5);
     glUniform1i(program_diffuse_ambient_noTransf_noSelfShadow_texBlend.unifoSet.texture2.loc, 6);
     glUniform1i(program_diffuse_ambient_noTransf_noSelfShadow_texBlend.unifoSet.texture3.loc, 7);
     
     
     }
     */
    //----camera2D
	if(1)
    {
        float left=0;
        float right=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getWidth();
        float top=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getHeight();
        float bottom = 0;
        float _near = -1;//
        float _far = 1;
        Cc3dRange range(left,right,bottom,top,_near,_far);
        camera2D=new Cc3dCamera();
        camera2D->autorelease();
        camera2D->retain();//must retain
        camera2D->setRange(range);
        camera2D->setProjectionMode(ec3dOrthographicMode);
    }
    //----camera
    {
        camera=new Cc3dCamera();
        camera->autorelease();
        camera->retain();//must retain
        camera->setFovy(70);
        camera->setAspect((float)Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getWidth()/Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getHeight());
        camera->setzNear(2);
        camera->setzFar(10000);
        float eyePos[3]={0,0,0};
        float eyeTg[3]={0,0,-1};
        float up[3]={0,1,0};
        camera->setEyePos(Cc3dVector3(eyePos).toV4(1));
        camera->setCenter(Cc3dVector3(eyeTg).toV4(1));
        camera->setUp(Cc3dVector3(up).toV4(0));
        camera->updateFrustum();
    }
    //----设置听者gain
    alListenerf(AL_GAIN, 1.0);
    //----startPage
    CstartPage*startPage=new CstartPage();
    startPage->autorelease();
    startPage->init();
    Cc3dSceneManager::sharedSceneManager()->getRoot()->removeAllChildOnNextFrame();
    Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(startPage);
    //----加载用户设置--abc
    if(CuserConfig::sharedUserConfig()->getIsConfigFileExistInDocuments("userConfig_superSingleCell.txt")==false){
        CuserConfig::sharedUserConfig()->copyConfigFileToDocuments("singleCellGameResource/data/userConfig/userConfig_superSingleCell.txt");
    }
    CuserConfig::sharedUserConfig()->loadConfigFileFromDocuments("userConfig_superSingleCell.txt");
    
    return true;

}