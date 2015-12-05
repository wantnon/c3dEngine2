//
//  initGame.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#include "initGame.h"
#include "c3d.h"
#include "c3dExtension.h"
#include "globalVars.h"
void initGame(){
	

	//----light
	{
		light0=new Cc3dLight();
		light0->autorelease();
		light0->retain();
		light0->setPos(1000,5000,1000);//(50,200,500);
		light0->setSpecular(0.4,0.4,0.4);
	}
	 //----camera2D
    {
        float left=0;
        float right=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getWidth();
        float top=Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getHeight();
        float bottom = 0;
        float _near = -1;//note: near and far is key word in vs2010, so here we use _near and _far
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
        camera->setzNear(0.5);
        camera->setzFar(10000);
        float eyePos[3]={0,30,50};
        float eyeTg[3]={0,30,0};
        float up[3]={0,1,0};
        camera->setEyePos(Cc3dVector3(eyePos).toV4(1));
        camera->setCenter(Cc3dVector3(eyeTg).toV4(1));
        camera->setUp(Cc3dVector3(up).toV4(0));
        camera->updateFrustum();
    }

	//----import _cfc model
	Cc3dSkinActor*actor=new Cc3dSkinActor();
	actor->autorelease();
	string path="export/girl_cfc";
	//string path="export/girl_readable_cfc";
	cout<<"loading from "<<path<<endl;
	actor->doImport(path.c_str());
	cout<<"loading done!"<<endl;
	
	actor->setPos(Cc3dVector4(0,0,0,1));
	actor->setLight(light0);
    actor->setCamera(camera);
	actor->setProgram(Cc3dProgramCache::sharedProgramCache()->getProgramByName("classicLighting"));
    actor->setPassUnifoCallback(buildinProgramPassUnifoCallback_classicLighting);

    actor->genVBOBuffers();
	actor->submit(GL_STATIC_DRAW);


	//----add to scene
    Cc3dSceneManager::sharedSceneManager()->getRoot()->addChild(actor);
}