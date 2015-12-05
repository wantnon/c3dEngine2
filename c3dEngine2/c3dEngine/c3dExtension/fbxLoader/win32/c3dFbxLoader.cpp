#include "c3dFbxLoader.h"

static Cc3dFbxLoader* s_fbxLoader=NULL;
Cc3dFbxLoader*Cc3dFbxLoader::sharedFbxLoader(){
	if(s_fbxLoader==NULL){
		s_fbxLoader=new Cc3dFbxLoader();
	}
	return s_fbxLoader;
}
Cc3dSkinActor*Cc3dFbxLoader::load(string fbxFilePath,float aniFrameInterval) 
{
    cout<<"aniFrameInterval:"<<aniFrameInterval<<endl;
	//create fbxOneLoad object
	Cc3dFbxOneLoad*fbxOneLoad=new Cc3dFbxOneLoad();
	fbxOneLoad->autorelease();
    if(aniFrameInterval==0){
        fbxOneLoad->m_isNoAnimation=true;
    }else{
        fbxOneLoad->m_isNoAnimation=false;
    }
	//load
	fbxOneLoad->Init_and_load(fbxFilePath.c_str());
	//convert to Cc3dSkinActor
	return fbxOneLoad->convertToSkinActor(aniFrameInterval);
}