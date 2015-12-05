#include "c3dFbxOneLoad.h"
#ifdef IOS_REF
	#undef  IOS_REF
	#define IOS_REF (*(pManager->GetIOSettings()))
#endif

Cc3dMatrix4 FbxAMatrixToCc3dMatrix4(const FbxAMatrix&m){
	//设m为FbxAMatrix
	//double*pm=(double*)m
	//则pm完全等同于opengl中的double[16],遵从如下布局:
	// pm[0] pm[4] pm[8]  pm[12]
	// pm[1] pm[5] pm[9]  pm[13]
	// pm[2] pm[6] pm[10] pm[14]
	// pm[3] pm[7] pm[11] pm[15]
	//其中(pm[12],pm[13],pm[14],pm[15])为平移分量(x,y,z,1)
	//而如果不进行类型转化,直接使用m,则：
	//               m[0][0] m[0][1] m[0][2] m[0][3]  pm[0]  pm[1]  pm[2]  pm[3]
	// FbxAMatrix m= m[1][0] m[1][1] m[1][2] m[1][3]= pm[4]  pm[5]  pm[6]  pm[7]
	//				 m[2][0] m[2][1] m[2][2] m[2][3]  pm[8]  pm[9]  pm[10] pm[11]
	//               m[3][0] m[3][1] m[3][2] m[3][3]  pm[12] pm[13] pm[14] pm[15]
	//注意此处pm矩阵与上面的pm矩阵行列正好相反--abc
	const double*pm=(const double*)m;
	Cc3dMatrix4 mat(pm[0],pm[1],pm[2],pm[3],//col1
					pm[4],pm[5],pm[6],pm[7],//col2
					pm[8],pm[9],pm[10],pm[11],//col3
					pm[12],pm[13],pm[14],pm[15]);//col4
	return mat;

}
	
	void Cc3dFbxOneLoad::Init_and_load(const char* _fbxFileName){
        
		fbxFileName=(char*)_fbxFileName;
		cout<<"load "<<fbxFileName<<endl;
		//加载fbx场景--abc
		InitializeSdkObjects(lSdkManager,lScene);
		LoadScene(lSdkManager,lScene,/*fbxFileName*/Cc3dFileUtils::sharedFileUtils()->getFullPath(fbxFileName).c_str());
	}
	/*		
     void Cc3dFbxOneLoad::InitializeSdkObjects(FbxManager*& pSdkManager, FbxScene*& pScene)
	{
// The first thing to do is to create the FBX SDK manager which is the
		// object allocator for almost all the classes in the SDK.
		pSdkManager = FbxManager::Create();
		if( !pSdkManager )
		{
			FBXSDK_printf("Unable to create the FBX SDK manager\n");
			system("pause");
			exit(0);
		}
		else
		{
			FBXSDK_printf("FBX SDK version %s\n\n", pSdkManager->GetVersion());
		}

		// create an IOSettings object
		FbxIOSettings * ios = FbxIOSettings::Create(pSdkManager, IOSROOT );
		pSdkManager->SetIOSettings(ios);

		// Load plugins from the executable directory
		FbxString lPath = FbxGetApplicationDirectory();
		pSdkManager->LoadPluginsDirectory(lPath.Buffer());

		// Create the entity that will hold the scene.
		pScene = FbxScene::Create(pSdkManager,"");
     }
     */
void Cc3dFbxOneLoad::InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
    //The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
    pManager = FbxManager::Create();
    if( !pManager )
    {
        FBXSDK_printf("Error: Unable to create FBX Manager!\n");
        exit(1);
    }
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());
    
	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);
    
	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());
    
    //Create an FBX scene. This object holds most objects imported/exported from/to files.
    pScene = FbxScene::Create(pManager, "My Scene");
	if( !pScene )
    {
        FBXSDK_printf("Error: Unable to create FBX scene!\n");
        exit(1);
    }
}

/*	bool Cc3dFbxOneLoad::LoadScene(FbxManager* pSdkManager, FbxDocument* pScene,char* pFilename)
	{
#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pSdkManager->GetIOSettings()))
#endif

		int lFileMajor, lFileMinor, lFileRevision;
		int lSDKMajor,  lSDKMinor,  lSDKRevision;
		//int lFileFormat = -1;
		int i, lAnimStackCount;
		bool lStatus;
		char lPassword[1024];

		// Get the file version number generate by the FBX SDK.
		FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

		// Create an importer.
		FbxImporter* lImporter = FbxImporter::Create(pSdkManager,"");

		// Initialize the importer by providing a filename.
		const bool lImportStatus = lImporter->Initialize(pFilename, -1, pSdkManager->GetIOSettings());
		lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

		if( !lImportStatus )
		{
			FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
			FBXSDK_printf("Error returned: %s\n\n", lImporter->GetLastErrorString());

			if (lImporter->GetLastErrorID() == FbxIOBase::eFileVersionNotSupportedYet ||
				lImporter->GetLastErrorID() == FbxIOBase::eFileVersionNotSupportedAnymore)
			{
				FBXSDK_printf("FBX version number for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
				FBXSDK_printf("FBX version number for file %s is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
			}

			return false;
		}



		FBXSDK_printf("FBX version number for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

		if (lImporter->IsFBX())
		{
			FBXSDK_printf("FBX version number for file %s is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

			// From this point, it is possible to access animation stack information without
			// the expense of loading the entire file.

			FBXSDK_printf("Animation Stack Information\n");

			lAnimStackCount = lImporter->GetAnimStackCount();

			FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
			FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
			FBXSDK_printf("\n");

			for(i = 0; i < lAnimStackCount; i++)
			{
				FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

				FBXSDK_printf("    Animation Stack %d\n", i);
				FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
				FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

				// Change the value of the import name if the animation stack should be imported 
				// under a different name.
				FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

				// Set the value of the import state to false if the animation stack should be not
				// be imported. 
				FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
				FBXSDK_printf("\n");
			}

			// Set the import states. By default, the import states are always set to 
			// true. The code below shows how to change these states.
			IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
			IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
			IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
			IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
			IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
			IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
			IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
		}

		// Import the scene.
		lStatus = lImporter->Import(pScene);

		if(lStatus == false && lImporter->GetLastErrorID() == FbxIOBase::ePasswordError)
		{
			FBXSDK_printf("Please enter password: ");

			lPassword[0] = '\0';

			FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
				scanf("%s", lPassword);
			FBXSDK_CRT_SECURE_NO_WARNING_END

				FbxString lString(lPassword);

			IOS_REF.SetStringProp(IMP_FBX_PASSWORD,      lString);
			IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

			lStatus = lImporter->Import(pScene);

			if(lStatus == false && lImporter->GetLastErrorID() == FbxIOBase::ePasswordError)
			{
				FBXSDK_printf("\nPassword is wrong, import aborted.\n");
			}
		}

		// Destroy the importer.
		lImporter->Destroy();

		return lStatus;
	}
	*/

/*
bool Cc3dFbxOneLoad::LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
    cout<<"-------"<<pFilename<<endl;
    int lFileMajor, lFileMinor, lFileRevision;
    int lSDKMajor,  lSDKMinor,  lSDKRevision;
    //int lFileFormat = -1;
    int i, lAnimStackCount;
    bool lStatus;
    char lPassword[1024];

    // Get the file version number generate by the FBX SDK.
    FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

    // Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(pManager,"");

    // Initialize the importer by providing a filename.
    const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
    lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

    if( !lImportStatus )
    {
        FbxString error = lImporter->GetStatus().GetErrorString();
        FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
        FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

        if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
        {
            FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
            FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
        }

        return false;
    }

    FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

    if (lImporter->IsFBX())
    {
        FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

        // From this point, it is possible to access animation stack information without
        // the expense of loading the entire file.

        FBXSDK_printf("Animation Stack Information\n");

        lAnimStackCount = lImporter->GetAnimStackCount();

        FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
        FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
        FBXSDK_printf("\n");

        for(i = 0; i < lAnimStackCount; i++)
        {
            FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

            FBXSDK_printf("    Animation Stack %d\n", i);
            FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
            FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

            // Change the value of the import name if the animation stack should be imported 
            // under a different name.
            FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

            // Set the value of the import state to false if the animation stack should be not
            // be imported. 
            FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
            FBXSDK_printf("\n");
        }

        // Set the import states. By default, the import states are always set to 
        // true. The code below shows how to change these states.
        IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
        IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
        IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
        IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
        IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
        IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
        IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }

    // Import the scene.
    lStatus = lImporter->Import(pScene);

    if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
    {
        FBXSDK_printf("Please enter password: ");

        lPassword[0] = '\0';

        FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
        scanf("%s", lPassword);
        FBXSDK_CRT_SECURE_NO_WARNING_END

        FbxString lString(lPassword);

        IOS_REF.SetStringProp(IMP_FBX_PASSWORD,      lString);
        IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

        lStatus = lImporter->Import(pScene);

        if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
        {
            FBXSDK_printf("\nPassword is wrong, import aborted.\n");
        }
    }

    // Destroy the importer.
    lImporter->Destroy();

    return lStatus;
}*/
bool Cc3dFbxOneLoad::LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
    int lFileMajor, lFileMinor, lFileRevision;
    int lSDKMajor,  lSDKMinor,  lSDKRevision;
    //int lFileFormat = -1;
    int i, lAnimStackCount;
    bool lStatus;
    char lPassword[1024];
    
    // Get the file version number generate by the FBX SDK.
    FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
    
    // Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(pManager,"");
    
    // Initialize the importer by providing a filename.
    const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
    lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
    
    if( !lImportStatus )
    {
        FbxString error = lImporter->GetStatus().GetErrorString();
        FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
        FBXSDK_printf("Error returned: %s\n\n", error.Buffer());
        
        if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
        {
            FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
            FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
        }
        
        return false;
    }
    
    FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
    
    if (lImporter->IsFBX())
    {
        FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
        
        // From this point, it is possible to access animation stack information without
        // the expense of loading the entire file.
        
        FBXSDK_printf("Animation Stack Information\n");
        
        lAnimStackCount = lImporter->GetAnimStackCount();
        
        FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
        FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
        FBXSDK_printf("\n");
        
        for(i = 0; i < lAnimStackCount; i++)
        {
            FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);
            
            FBXSDK_printf("    Animation Stack %d\n", i);
            FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
            FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());
            
            // Change the value of the import name if the animation stack should be imported
            // under a different name.
            FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());
            
            // Set the value of the import state to false if the animation stack should be not
            // be imported.
            FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
            FBXSDK_printf("\n");
        }
        
        // Set the import states. By default, the import states are always set to
        // true. The code below shows how to change these states.
        IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
        IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
        IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
        IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
        IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
        IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
        IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }
    
    // Import the scene.
    lStatus = lImporter->Import(pScene);
    
    if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
    {
        FBXSDK_printf("Please enter password: ");
        
        lPassword[0] = '\0';
        
        FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
        scanf("%s", lPassword);
        FBXSDK_CRT_SECURE_NO_WARNING_END
        
        FbxString lString(lPassword);
        
        IOS_REF.SetStringProp(IMP_FBX_PASSWORD,      lString);
        IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);
        
        lStatus = lImporter->Import(pScene);
        
        if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
        {
            FBXSDK_printf("\nPassword is wrong, import aborted.\n");
        }
    }
    
    // Destroy the importer.
    lImporter->Destroy();
    
    return lStatus;
}


	void Cc3dFbxOneLoad::DestroySdkObjects(FbxManager* &pSdkManager)//一定要传指针的引用,因为需要将指针置NULL
	{
		// Delete the FBX SDK manager. All the objects that have been allocated 
		// using the FBX SDK manager and that haven't been explicitly destroyed 
		// are automatically destroyed at the same time.
		if (pSdkManager) pSdkManager->Destroy();
		pSdkManager = NULL;
	}
	void Cc3dFbxOneLoad::makeSubMeshSetForThisNode(FbxNode* pNode){
		FbxMesh*lMesh=pNode->GetMesh();
		const int lVertexCount = lMesh->GetControlPointsCount();
		if(!lMesh){
			cout<<"error: lMesh==NULL!"<<endl;
			assert(false);
			return;
		}
		Cc3dSkinMesh*mesh=new Cc3dSkinMesh();
		mesh->autorelease();
		mesh->setName(pNode->GetName());//use node name as mesh name
		mesh->setFbxMeshPtr(lMesh);
		//制作mesh
		//每个材质对应一个subMesh
		{
			
			//获得材质索引表（各多边形对应的材质索引)
			FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
			{
				if (lMesh->GetElementMaterial())
				{
					lMaterialIndice = &lMesh->GetElementMaterial()->GetIndexArray();
				}
			}
			if(!lMaterialIndice){
				cout<<"error: lMaterialIndice==NULL!"<<endl;
				return;
			}
			//获得材质索引表长度--abc
			const int MaterialIndiceCount=lMaterialIndice->GetCount();
			if(MaterialIndiceCount==0){
				cout<<"error: MaterialIndiceCount==0 !"<<endl;
				return;
			}
			//获得材质映射模式--abc
			FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
			lMaterialMappingMode = lMesh->GetElementMaterial()->GetMappingMode();
			
	
			//-------------------------------判断uv映射模式及获得lUVName
			const char * lUVName = NULL;
			FbxStringList lUVNames;//这个不能放到下面的括号里面去,因为将来lUVName是要指向lUVNames[0]的--abc
			//如果lUVNames放在了下面的括号里面,括号结束时lUVName被析构,造成lUVName成空引用--abc
			{
				//是否有uv
				bool mHasUV = lMesh->GetElementUVCount() > 0;
				if(mHasUV==false){
					cout<<"error: there is no uv!"<<endl;
					assert(false);
				}else{
					//uv映射模式--abc
					const FbxGeometryElementUV * lUVElement = lMesh->GetElementUV(0);
					FbxGeometryElement::EMappingMode lUVMappingMode=lUVElement->GetMappingMode();
					if(lUVMappingMode!=FbxGeometryElement::eByPolygonVertex){
						cout<<"error: currently, only uv mapping mode : eByPolygonVertex is supported!"<<endl;
						assert(false);
					}else{
						//获得uvSet名称--abc
						lMesh->GetUVSetNames(lUVNames);
						if (lUVNames.GetCount())
						{
							lUVName = lUVNames[0];
						}else{
							cout<<"lUVName==NULL!"<<endl;
							assert(false);
						}
					}	
				}
			}//得到lUVName
			//cout<<"lUVName:"<<lUVName<<endl;
			//---------------------判断norm映射模式--abc
			bool mHasNormal = lMesh->GetElementNormalCount() > 0;
			if (mHasNormal)
			{
				FbxGeometryElement::EMappingMode lNormalMappingMode = lMesh->GetElementNormal(0)->GetMappingMode();
				if (lNormalMappingMode == FbxGeometryElement::eNone)
				{
					cout<<"error:没有法向数据!"<<endl;
					assert(false);
				}
				if (mHasNormal && lNormalMappingMode != FbxGeometryElement::eByPolygonVertex)
				{
					cout<<"error:norm映射模式不是eByPolygonVertex,目前尚不支持其它模式!"<<endl;
					assert(false);
				}
			}
			//-------------
			//求材质索引集--abc
			vector<int> materialIndexSet;
			for(int i=0;i<MaterialIndiceCount;i++){
				int MaterialIndex=lMaterialIndice->GetAt(i);
				bool isExist=false;
				for(int j=0;j<(int)materialIndexSet.size();j++){
					if(materialIndexSet[j]==MaterialIndex){
						isExist=true;
						break;
					}
				}//got isExist
				if(isExist==false){
					materialIndexSet.push_back(MaterialIndex);
				}
			}//got materialIndexSet
			int materialIndexSetSize=(int)materialIndexSet.size();

			//生成subMesh列表（每个材质生成一个subMesh)
			vector<Cc3dSkinSubMesh*> subMeshList;
			for(int i=0;i<materialIndexSetSize;i++){
				Cc3dSkinSubMesh*subMesh=new Cc3dSkinSubMesh();
				subMesh->autorelease();
				subMeshList.push_back(subMesh);
			}
			//为各subMesh填充纹理--abc
			string modelFolderPath;
			{
				modelFolderPath=fbxFileName;
				vector<string> strList=splitStrInTwoByLastBar(modelFolderPath);
				modelFolderPath=strList[0];
			}//got modelFolderPath
			for(int i=0;i<materialIndexSetSize;i++){
				int lMaterialIndex=materialIndexSet[i];
				Cc3dTexture* texture=NULL;
				const FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
				// Phong material 
				//see: http://blog.csdn.net/bugrunner/article/details/7211515
				if(lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId)){
				
					// Ambient Color   
					FbxDouble3 ambient= ((FbxSurfacePhong*)lMaterial)->Ambient;  
					//cout<<"ambient:"<<ambient[0]<<" "<<ambient[1]<<" "<<ambient[2]<<endl; 

					// Diffuse Color   
					FbxDouble3 diffuse=((FbxSurfacePhong*)lMaterial)->Diffuse;  
					//cout<<"diffuse:"<<diffuse[0]<<" "<<diffuse[1]<<" "<<diffuse[2]<<endl;

					// Specular Color   
					FbxDouble3 specular=((FbxSurfacePhong*)lMaterial)->Specular;  
					//cout<<"specular:"<<specular[0]<<" "<<specular[1]<<" "<<specular[2]<<endl;

					// Emissive Color   
					FbxDouble3 emissive=((FbxSurfacePhong*)lMaterial)->Emissive;  
					//cout<<"emissive:"<<emissive[0]<<" "<<emissive[1]<<" "<<emissive[2]<<endl;

					// Opacity   
					//0 = opaque, 1 = transparent
					//see fbxsurfacelambert.h
					FbxDouble transparencyFactor=((FbxSurfacePhong*)lMaterial)->TransparencyFactor;  
					//cout<<"transparencyFactor:"<<transparencyFactor<<endl;

					// Shininess   
					FbxDouble shininess=((FbxSurfacePhong*)lMaterial)->Shininess;  
					//cout<<"shininess:"<<shininess<<endl; 

					// Reflectivity   
					FbxDouble reflectionFactor=((FbxSurfacePhong*)lMaterial)->ReflectionFactor;  
					//cout<<"reflectionFactor:"<<reflectionFactor<<endl;

					Cc3dMaterial*material=new Cc3dMaterial();
					material->autorelease();
					//not all material properties are used
					material->setAmbient(ambient[0],ambient[1],ambient[2]);
					material->setDiffuseRGB(diffuse[0],diffuse[1],diffuse[2]);
					material->setSpecular(specular[0],specular[1],specular[2]);
					material->setDiffuseAlpha(1-transparencyFactor);//note: should be alpha=1-fbxTransprencyFactor
					material->setShininess(shininess);
	         		subMeshList[lMaterialIndex]->setMaterial(material);
					

				}else if(lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)){
					cout<<"warning: the material type is lambert, currently we only support phong, will ignore."<<endl;
				}else{
					cout<<"warning: unknown material type, will ignore."<<endl;
				}
		
				//texture
				const char * pPropertyName=FbxSurfaceMaterial::sDiffuse;
				const FbxProperty lProperty = lMaterial->FindProperty(pPropertyName);
				if (lProperty.IsValid())
				{
					const int lTextureCount = lProperty.GetSrcObjectCount(FbxFileTexture::ClassId);
					if (lTextureCount)
					{
                        const FbxFileTexture* lFileTexture = lProperty.GetSrcObject<FbxFileTexture>();//lProperty.GetSrcObject(FBX_TYPE(FbxFileTexture), 0);//2014-9-27

						// Try to load the texture from absolute path
						string fullPath=lFileTexture->GetFileName();
						vector<string> strList=splitStrInTwoByLastBar(fullPath);
						assert((int)strList.size()==2);
						string texFileName=strList[1];
						string texPath=modelFolderPath+"/"+texFileName;
						//cout<<"texPath:"<<texPath<<endl;
						//以texPath生成纹理--abc
						texture=Cc3dTextureCache::sharedTextureCache()->addImage(texPath.c_str());
                        assert(isPowOfTwo((int)texture->getWidth()));
                        assert(isPowOfTwo((int)texture->getHeight()));
						if(texture==NULL){
							cout<<"error: create texture failed! "<<endl;
							assert(false);
						}

					}
				}//got texture
				if(texture==NULL){
					cout<<"warning: texture==NULL, use default texture instead"<<endl;
					texture=Cc3dTextureCache::sharedTextureCache()->addImage("c3dEngineResource/tex/white.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
				}
				subMeshList[lMaterialIndex]->setTexture(texture);
			}
			//遍历各多边形,填充到subMesh
			vector<vector<_CmeshIDvID> > vertexDupList;
			vertexDupList.resize(lVertexCount);//support controlPointID is continuous (it is right)
			const int triangleCount = lMesh->GetPolygonCount();
			for (int i = 0; i < triangleCount; i++)
			{
				//当前多边形号：i
				//...
				//----顶点--abc
				int controlPointID0=lMesh->GetPolygonVertex(i,0);
				int controlPointID1=lMesh->GetPolygonVertex(i,1);
				int controlPointID2=lMesh->GetPolygonVertex(i,2);
				// Sometimes, the mesh can have less points than at the time of the skinning
				// because a smooth operator was active when skinning but has been deactivated during export.
				if (controlPointID0 >= lVertexCount)continue;
				if (controlPointID1 >= lVertexCount)continue;
				if (controlPointID2 >= lVertexCount)continue;
				FbxVector4 pos0=lMesh->GetControlPoints()[controlPointID0];
				FbxVector4 pos1=lMesh->GetControlPoints()[controlPointID1];
				FbxVector4 pos2=lMesh->GetControlPoints()[controlPointID2];
				pos0[3]=1;
				pos1[3]=1;
				pos2[3]=1;
				//----获得uv
				FbxVector2 uv0;
				FbxVector2 uv1;
				FbxVector2 uv2;
				bool isUnmapped0;
				bool isUnmapped1;
				bool isUnmapped2;
				//Added a new parameter to FbxMesh::GetPolygonVertexUV() to allow it to return an array of unmapped polygons. This is extremely useful to determine which vertex has no associated UV.
				//see: http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html
				lMesh->GetPolygonVertexUV(i, 0, lUVName, uv0,isUnmapped0);
				lMesh->GetPolygonVertexUV(i, 1, lUVName, uv1,isUnmapped1);
				lMesh->GetPolygonVertexUV(i, 2, lUVName, uv2,isUnmapped2);
				//----获得法向量--abc
				FbxVector4 norm0;
				FbxVector4 norm1;
				FbxVector4 norm2;
				lMesh->GetPolygonVertexNormal(i, 0, norm0);
				lMesh->GetPolygonVertexNormal(i, 1, norm1);
				lMesh->GetPolygonVertexNormal(i, 2, norm2);
				norm0[3]=0;
				norm1[3]=0;
				norm2[3]=0;
				//当前多边形材质索引--abc
				//ref to "Layer element for mapping materials (FbxSurfaceMaterial) to a geometry" in fbxlayer.h
				int lMaterialIndex=0;
				if(lMaterialMappingMode == FbxGeometryElement::eByPolygon){
					lMaterialIndex=lMaterialIndice->GetAt(i);
				}else if( lMaterialMappingMode == FbxGeometryElement::eAllSame){
					lMaterialIndex=0;
				}else{
					cout<<"error: unknown material mapping mode!"<<endl;
					assert(false);
				}//得到lMaterialIndex
				//用材质索引作为meshID
				int meshID=lMaterialIndex;
				//当前subMesh
				Cc3dSubMesh*subMesh=subMeshList[meshID];
				//----由uv,norm,pos合成顶点--abc
				Cc3dVertex vertex0(Cc3dVector4(pos0[0],pos0[1],pos0[2],pos0[3]),Cc3dVector2(uv0[0],1-uv0[1]),Cc3dVector4(norm0[0],norm0[1],norm0[2],norm0[3]));
				Cc3dVertex vertex1(Cc3dVector4(pos1[0],pos1[1],pos1[2],pos1[3]),Cc3dVector2(uv1[0],1-uv1[1]),Cc3dVector4(norm1[0],norm1[1],norm1[2],norm1[3]));
				Cc3dVertex vertex2(Cc3dVector4(pos2[0],pos2[1],pos2[2],pos2[3]),Cc3dVector2(uv2[0],1-uv2[1]),Cc3dVector4(norm2[0],norm2[1],norm2[2],norm2[3]));
				//----addIDtri first
				int vID0=subMesh->getSubMeshData()->getVertexCount();//vlist.size();
				int vID1=vID0+1;
				int vID2=vID0+2;
				subMesh->addIDtri(Cc3dIDTriangle(vID0,vID1,vID2));
				//----addVertex second (must after addIDtri)
				subMesh->addVertex(vertex0);
				subMesh->addVertex(vertex1);
				subMesh->addVertex(vertex2);
				//----make vertexDupList
				vertexDupList[controlPointID0].push_back(_CmeshIDvID(meshID,vID0));
				vertexDupList[controlPointID1].push_back(_CmeshIDvID(meshID,vID1));
				vertexDupList[controlPointID2].push_back(_CmeshIDvID(meshID,vID2));
				
			}
			//backup subMeshData for all subMeshes
			int nSubMesh=(int)subMeshList.size();
			for(int i=0;i<nSubMesh;i++){
				subMeshList[i]->backupSubMeshData();
			}
			//将subMeshList中所有mesh添加到mesh
			for(int i=0;i<nSubMesh;i++){
				mesh->addSubMesh(subMeshList[i]);
			}
			//将vertexDupList添加到mesh
			mesh->setVertexDupList(vertexDupList);
		}
		//将mesh添加到this
		m_actor->addMesh(mesh);
	

		

	}

    void Cc3dFbxOneLoad::attachMeshToTargetShapes(bool isHideTargetShapeMeshes){
        const int nMeshCount=m_actor->getMeshCount();
        for(int i=0;i<nMeshCount;i++){
            Cc3dSkinMesh*skinMesh=(Cc3dSkinMesh*)m_actor->getMeshByIndex(i);
            if(skinMesh==NULL)continue;
            const int nBlendShape=skinMesh->getBlendShapeCount();
            for(int j=0;j<nBlendShape;j++){
                Cc3dBlendShape*blendShape=skinMesh->getBlendShapeByIndex(j);
                const int nBlendShapeChannel=blendShape->getBlendShapeChannelCount();
                for(int k=0;k<nBlendShapeChannel;k++){
                    Cc3dBlendShapeChannel*blendShapeChannel=blendShape->getBlendShapeChannelByIndex(k);
                    const int nTargetShape=blendShapeChannel->getTargetShapeCount();
                    for(int u=0;u<nTargetShape;u++){
                        Cc3dTargetShape*targetShape=blendShapeChannel->getTargetShapeByIndex(u);
                        const string&targetShapeName=targetShape->getTargetShapeName();
                        FbxNode*lNode=lScene->FindNodeByName(FbxString(targetShapeName.c_str()));
                        if(lNode==NULL){
                            cout<<"warning: can not find mesh for targetShape "<<"'"<<targetShapeName<<"'"<<", it's normal data can not be obtained!"<<endl;
                        }else{
                            assert(lNode->GetMesh());
                            Cc3dSkinMesh* mesh=(Cc3dSkinMesh*)m_actor->findSkinMeshByFbxMeshPtr(lNode->GetMesh());
                            assert(mesh);
                            targetShape->setTargetShapeMesh(mesh);
                            //topology consistency check : mesh and skinMesh have same topology?
                            bool topologyIsSame=isTopologySame(skinMesh, mesh);
                            if(topologyIsSame==false){
                                cout<<"error: target shape mesh's topology is not same with original mesh!"<<endl;
                                assert(false);
                            }
                            if(isHideTargetShapeMeshes){
                                mesh->setIsVisibleRecursively(false);
                                mesh->setIsDoUpdateRecursively(false);
                            }
                        }
                    }
                }
            }
        }
        
    }
	void Cc3dFbxOneLoad::makeSubMeshSetForEachNode(FbxNode* pNode)
	{
		FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
		if (lNodeAttribute//有属性节点--abc
			&&lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh//且属性节点为的类型为eMesh
			)//为pNode制作并绑定idSubMeshSet
		{	
			makeSubMeshSetForThisNode(pNode);
		}
		const int lChildCount = pNode->GetChildCount();
		for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
		{
			makeSubMeshSetForEachNode(pNode->GetChild(lChildIndex));
		}
	}
	bool Cc3dFbxOneLoad::getHasDeformer(FbxMesh*lMesh){
		//	// If it has some deformer connection, update the vertices position
		//	const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) &&
		//		(static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->IsActive();
		//	const bool lHasShape = lMesh->GetShapeCount() > 0;
		const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		//	const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;
		return lHasSkin;
	}



//get mesh smoothing info
//set pCompute true to compute smoothing from normals by default 
//set pConvertToSmoothingGroup true to convert hard/soft edge info to smoothing group info by default
void Cc3dFbxOneLoad::GetSmoothing(FbxManager* pSdkManager, FbxNode* pNode, bool pCompute, bool pConvertToSmoothingGroup)
{
    if(!pNode || !pSdkManager)
        return;

    //get mesh
    FbxMesh* lMesh = pNode->GetMesh();
    if(lMesh)
    {
        //print mesh node name
        FBXSDK_printf("current mesh node: %s\n", pNode->GetName());

        //if there's no smoothing info in fbx file, but you still want to get smoothing info.
        //please compute smoothing info from normals.
        //Another case to recompute smoothing info from normals is:
        //If users edit normals manually in 3ds Max or Maya and export the scene to FBX with smoothing info,
        //The smoothing info may NOT match with normals.
        //the mesh called "fbx_customNormals" in Normals.fbx is the case. All edges are hard, but normals actually represent the "soft" looking.
        //Generally, the normals in fbx file holds the smoothing result you'd like to get.
        //If you want to get correct smoothing info(smoothing group or hard/soft edges) which match with normals,
        //please drop the original smoothing info of fbx file, and recompute smoothing info from normals.
        //if you want to get soft/hard edge info, please call FbxGeometryConverter::ComputeEdgeSmoothingFromNormals().
        //if you want to get smoothing group info, please get soft/hard edge info first by ComputeEdgeSmoothingFromNormals() 
        //And then call FbxGeometryConverter::ComputePolygonSmoothingFromEdgeSmoothing().
        if(pCompute)
        {
            FbxGeometryConverter lGeometryConverter(pSdkManager);
            lGeometryConverter.ComputeEdgeSmoothingFromNormals(lMesh);
            //convert soft/hard edge info to smoothing group info
            if(pConvertToSmoothingGroup)
                lGeometryConverter.ComputePolygonSmoothingFromEdgeSmoothing(lMesh);
        }

        //if there is smoothing groups info in your fbx file, but you want to get hard/soft edges info
        //please use following code:
        //FbxGeometryConverter lGeometryConverter(lSdkManager);
        //lGeometryConverter.ComputeEdgeSmoothingFromPolygonSmoothing(lMesh);

        //get smoothing info
        FbxGeometryElementSmoothing* lSmoothingElement = lMesh->GetElementSmoothing();
        if(lSmoothingElement)
        {
            //mapping mode is by edge. The mesh usually come from Maya, because Maya can set hard/soft edges.
            //we can get smoothing info(which edges are soft, which edges are hard) by retrieving each edge. 
            if( lSmoothingElement->GetMappingMode() == FbxGeometryElement::eByEdge )
            {
                //Let's get smoothing of each edge, since the mapping mode of smoothing element is by edge
                for(int lEdgeIndex = 0; lEdgeIndex < lMesh->GetMeshEdgeCount(); lEdgeIndex++)
                {
                    int lSmoothingIndex = 0;
                    //reference mode is direct, the smoothing index is same as edge index.
                    //get smoothing by the index of edge
                    if( lSmoothingElement->GetReferenceMode() == FbxGeometryElement::eDirect )
                        lSmoothingIndex = lEdgeIndex;

                    //reference mode is index-to-direct, get smoothing by the index-to-direct
                    if(lSmoothingElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                        lSmoothingIndex = lSmoothingElement->GetIndexArray().GetAt(lEdgeIndex);

                    //Got smoothing of each vertex.
                    int lSmoothingFlag = lSmoothingElement->GetDirectArray().GetAt(lSmoothingIndex);
                    FBXSDK_printf("hard/soft value for edge[%d]: %d \n", lEdgeIndex, lSmoothingFlag);
                    //add your custom code here, to output smoothing or get them into a list, such as KArrayTemplate<int>
                    //. . .
                }//end for lEdgeIndex
            }//end eByEdge
            //mapping mode is by polygon. The mesh usually come from 3ds Max, because 3ds Max can set smoothing groups for polygon.
            //we can get smoothing info(smoothing group ID for each polygon) by retrieving each polygon. 
            else if(lSmoothingElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
            {
                //Let's get smoothing of each polygon, since the mapping mode of smoothing element is by polygon.
                for(int lPolygonIndex = 0; lPolygonIndex < lMesh->GetPolygonCount(); lPolygonIndex++)
                {
                    int lSmoothingIndex = 0;
                    //reference mode is direct, the smoothing index is same as polygon index.
                    if( lSmoothingElement->GetReferenceMode() == FbxGeometryElement::eDirect )
                        lSmoothingIndex = lPolygonIndex;

                    //reference mode is index-to-direct, get smoothing by the index-to-direct
                    if(lSmoothingElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                        lSmoothingIndex = lSmoothingElement->GetIndexArray().GetAt(lPolygonIndex);

                    //Got smoothing of each polygon.
                    int lSmoothingFlag = lSmoothingElement->GetDirectArray().GetAt(lSmoothingIndex);
                    FBXSDK_printf("smoothing group ID for polygon[%d]: %d \n", lPolygonIndex, lSmoothingFlag);
                    //add your custom code here, to output normals or get them into a list, such as KArrayTemplate<int>
                    //. . .

                }//end for lPolygonIndex //PolygonCount

            }//end eByPolygonVertex
        }//end if lSmoothingElement
    }//end if lMesh

    //recursively traverse each node in the scene
    int i, lCount = pNode->GetChildCount();
    for (i = 0; i < lCount; i++)
    {
        GetSmoothing(pSdkManager, pNode->GetChild(i), pCompute, pConvertToSmoothingGroup);
    }
}

	void Cc3dFbxOneLoad::destroyManager(){
		DestroySdkObjects(lSdkManager);//删除manager
	}
	void Cc3dFbxOneLoad::bakeAnimation(float aniFrameInterval)
	//if set aniFrameInterval to ZERO, will bake static model (the very first frame)
	{
		m_actor->setInterval(aniFrameInterval);
		//animStack
		FbxArray<FbxString*> mAnimStackNameArray;
		lScene->FillAnimStackNameArray(mAnimStackNameArray);
		const int lAnimStackCount = mAnimStackNameArray.GetCount();
        if(m_isNoAnimation){
            //bake static model (the very first frame)
			assert(lAnimStackCount>0);
			int animStackIndex=0;
			FbxAnimStack *lCurrentAnimationStack;
			lCurrentAnimationStack = lScene->FindMember<FbxAnimStack>(mAnimStackNameArray[animStackIndex]->Buffer());// lScene->FindMember(FBX_TYPE(FbxAnimStack), mAnimStackNameArray[animStackIndex]->Buffer());//2014-9-27
			FbxTime fbxTime(0);//2014-9-27
            this->updateSkin(fbxTime,lCurrentAnimationStack,animStackIndex);
			return;

        
        }
		for(int i=0;i<lAnimStackCount;i++){
			int animStackIndex=i;
	
			// select the base layer from the animation stack
			FbxAnimStack *lCurrentAnimationStack;
			lCurrentAnimationStack = lScene->FindMember<FbxAnimStack>(mAnimStackNameArray[animStackIndex]->Buffer());//lScene->FindMember(FBX_TYPE(FbxAnimStack), mAnimStackNameArray[animStackIndex]->Buffer());//2014-9-27
			if (lCurrentAnimationStack == NULL)
			{
				// this is a problem. The anim stack should be found in the scene!
				//销毁mAnimStackNameArray
				FbxArrayDelete(mAnimStackNameArray);
				assert(false);
			}
			//设置当前AnimationStack为lCurrentAnimationStack
			//Now to retrieve the current animation stack, please use FbxScene::GetCurrentAnimationStack instead of FbxAnimEvaluator::GetContext.
			//see fbxsdk2014.2.1 readme.txt
			//and see fbxsdk2014.2.1/samples/viewScene/SceneContext.ccx
			
			///lScene->GetEvaluator()->SetContext(lCurrentAnimationStack);//fbxsdk 2013.1
			lScene->SetCurrentAnimationStack(lCurrentAnimationStack);
			//计算此动画的起止时间--abc
			FbxTime startTime,stopTime;
			{
				
				FbxTakeInfo* lCurrentTakeInfo = lScene->GetTakeInfo(*(mAnimStackNameArray[animStackIndex]));
				if (lCurrentTakeInfo)
				{
					startTime = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
					stopTime = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
				}
				if(lCurrentTakeInfo==NULL||startTime.GetMilliSeconds()<0)//没得到lCurrentTakeInfo或者由lCurrentTakeInfo中得到的Start为负值--abc
				{
					if(lCurrentTakeInfo==NULL){
						cout<<"warning:no currentTakeInfo! take the time line value instead"<<endl;
					}
					if(lCurrentTakeInfo&&startTime.GetMilliSeconds()<0){
						cout<<"warning:value in currentTakeInfo is negative! take the time line value instead"<<endl;
					}
					
					// Take the time line value
					FbxTimeSpan lTimeLineTimeSpan;
					lScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

					startTime = lTimeLineTimeSpan.GetStart();
					stopTime  = lTimeLineTimeSpan.GetStop();
				}//得到startTime和stopTime
			}
				cout<<"startTime (milliSeconds):"<<startTime.GetMilliSeconds()<<endl;
				cout<<"stopTime (milliSeconds):"<<stopTime.GetMilliSeconds()<<endl;
				cout<<"startTime (seconds):"<<startTime.GetSecondDouble()<<endl;
				cout<<"stopTime (seconds):"<<stopTime.GetSecondDouble()<<endl;
			//
			//create aniLayer info
			Cc3dAniLayerInfo*aniLayerInfo=new Cc3dAniLayerInfo();
			aniLayerInfo->autorelease();
			aniLayerInfo->setInterval(m_actor->getInterval());
			aniLayerInfo->setStartTime((float)startTime.GetMilliSeconds()/1000);
			aniLayerInfo->setEndTime((float)stopTime.GetMilliSeconds()/1000);
			aniLayerInfo->setCurTime(aniLayerInfo->getStartTime());
			//calculate animation frames of this aniLayer
			FbxTime dTime;
			dTime.SetMilliSeconds(aniLayerInfo->getInterval()*1000);
			FbxTime curTime=startTime;
			while(1){
				if(curTime>stopTime){
					updateSkin(stopTime,lCurrentAnimationStack,animStackIndex);
					break;
				}
				this->updateSkin(curTime,lCurrentAnimationStack,animStackIndex);
				curTime+=dTime;
			}
			//add aniLayerInfo to this actor
			m_actor->addAniLayerInfo(aniLayerInfo);
		}
		//销毁mAnimStackNameArray
		FbxArrayDelete(mAnimStackNameArray);
	}

	
	
  /*  // Triangulate all NURBS, patch and mesh under this node recursively.
    void Cc3dFbxOneLoad::TriangulateRecursive(FbxNode* pNode)
    {
        FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

        if (lNodeAttribute)
        {
            if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
                lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
                lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbsSurface ||
                lNodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
            {
                FbxGeometryConverter lConverter(pNode->GetFbxManager());
                lConverter.TriangulateInPlace(pNode);
            }
        }

        const int lChildCount = pNode->GetChildCount();
        for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
        {
            TriangulateRecursive(pNode->GetChild(lChildIndex));
        }
    }
	*/
	void Cc3dFbxOneLoad::updateSkin(FbxTime&Time,FbxAnimStack *lCurrentAnimationStack,int animStackIndex){
		//更新fbx场景--abc

		if(lCurrentAnimationStack!=NULL){
			//获得mCurrentAnimLayer
			// we assume that the first animation layer connected to the animation stack is the base layer
			// (this is the assumption made in the FBXSDK)
			FbxAnimLayer *mCurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();// lCurrentAnimationStack->GetMember(FBX_TYPE(FbxAnimLayer), 0);//2014-9-27
			//获得pose
			int mPoseIndex=-1;//就想让lPose得NULL,所以设定此处为-1
			FbxPose * lPose = NULL;
			if (mPoseIndex != -1)
			{
				lPose = lScene->GetPose(mPoseIndex);
			}
			//定义lDummyGlobalPosition
			FbxAMatrix lDummyGlobalPosition;//模型的世界位置（默认构造为单位矩阵)
			DrawNodeRecursive(lScene->GetRootNode(), Time, mCurrentAnimLayer,animStackIndex, lDummyGlobalPosition,lPose);
		
		}
	
	}
	
	// Draw recursively each node of the scene. To avoid recomputing 
	// uselessly the global positions, the global position of each 
	// node is passed to it's children while browsing the node tree.
	// If the node is part of the given pose for the current scene,
	// it will be drawn at the position specified in the pose, Otherwise
	// it will be drawn at the given time.
	void Cc3dFbxOneLoad::DrawNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer,int animStackIndex,
		FbxAMatrix& pParentGlobalPosition, FbxPose* pPose)
	{
		//{the scale component of globalPosition matrix is equals to the product of scale factors when fbx file import and export via 3dmax }
		FbxAMatrix lGlobalPosition = GetGlobalPosition(pNode, pTime,pPose, &pParentGlobalPosition);
		
        
        
        if (pNode->GetNodeAttribute())
		{
			// Geometry offset.
			// it is not inherited by the children.
			  FbxAMatrix lGeometryOffset = GetGeometry(pNode);
			  FbxAMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;

			DrawNode(pNode, pTime, pAnimLayer,animStackIndex, pParentGlobalPosition,lGlobalOffPosition,pPose);
		}

		const int lChildCount = pNode->GetChildCount();
		for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
		{
			FbxNode* pChild=pNode->GetChild(lChildIndex);
			assert(pChild);
			DrawNodeRecursive(pChild, pTime, pAnimLayer,animStackIndex, lGlobalPosition,pPose);
		}
	}
	

	// Get the global position of the node for the current pose.
	// If the specified node is not part of the pose or no pose is specified, get its
	// global position at the current time.
	FbxAMatrix Cc3dFbxOneLoad::GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime,FbxPose* pPose, FbxAMatrix* pParentGlobalPosition)
	{
		FbxAMatrix lGlobalPosition;
		bool        lPositionFound = false;

		if (pPose)
		{
			int lNodeIndex = pPose->Find(pNode);

			if (lNodeIndex > -1)
			{
				// The bind pose is always a global matrix.
				// If we have a rest pose, we need to check if it is
				// stored in global or local space.
				if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
				{
					lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
				}
				else
				{
					// We have a local matrix, we need to convert it to
					// a global space matrix.
					FbxAMatrix lParentGlobalPosition;

					if (pParentGlobalPosition)
					{
						lParentGlobalPosition = *pParentGlobalPosition;
					}
					else
					{
						if (pNode->GetParent())
						{
							lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
						}
					}

					FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
					lGlobalPosition = lParentGlobalPosition * lLocalPosition;
				}

				lPositionFound = true;
			}
		}

		if (!lPositionFound)
		{
			// There is no pose entry for that node, get the current global position instead.

			// Ideally this would use parent global position and local position to compute the global position.
			// Unfortunately the equation 
			//    lGlobalPosition = pParentGlobalPosition * lLocalPosition
			// does not hold when inheritance type is other than "Parent" (RSrs).
			// To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
			lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
		}

		return lGlobalPosition;
	}

	// Get the matrix of the given pose
	FbxAMatrix Cc3dFbxOneLoad::GetPoseMatrix(FbxPose* pPose, int pNodeIndex)
	{
		FbxAMatrix lPoseMatrix;
		FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);

		memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

		return lPoseMatrix;
	}

	// Get the geometry offset to a node. It is never inherited by the children.
	FbxAMatrix Cc3dFbxOneLoad  ::GetGeometry(FbxNode* pNode)
	{
		const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

		return FbxAMatrix(lT, lR, lS);
	}
	// Draw the node following the content of it's node attribute.
	void Cc3dFbxOneLoad::DrawNode(FbxNode* pNode, 
		FbxTime& pTime, 
		FbxAnimLayer* pAnimLayer,
		int animStackIndex,
		FbxAMatrix& pParentGlobalPosition,
		FbxAMatrix& pGlobalPosition,
		FbxPose* pPose)
	{
		FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

		if (lNodeAttribute)
		{
			const FbxNodeAttribute::EType&attributeType=lNodeAttribute->GetAttributeType();
			// All lights has been processed before the whole scene because they influence every geometry.
			switch(attributeType){
			case FbxNodeAttribute::eMesh:
				// NURBS and patch have been converted into triangluation meshes.
				{
					DrawMesh(pNode, pTime, pAnimLayer,animStackIndex, pGlobalPosition, pPose);
				}
				break;
		/*
			case FbxNodeAttribute::eMarker:
				{
				//	DrawMarker(pGlobalPosition);
				}
				break;
			case FbxNodeAttribute::eSkeleton:
				{
					//这个在调试模型时应开启,以便观察骨骼与蒙皮是否吻合,但最终显示时应关闭以节省计算量--abc
					DrawSkeleton(pNode, pParentGlobalPosition, pGlobalPosition);
				}
				break;
			case FbxNodeAttribute::eCamera:
				{
					DrawCamera(pNode, pTime, pAnimLayer, pGlobalPosition);
				}
				break;
			case FbxNodeAttribute::eNull:
				{
					DrawNull(pGlobalPosition);
				}
				break;*/
			}
		}
		else
		{
			// Draw a Null for nodes without attribute.
	//		DrawNull(pGlobalPosition);
		}
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Skeleton
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Draw a limb between the node and its parent.
	void Cc3dFbxOneLoad::DrawSkeleton(FbxNode* pNode, FbxAMatrix& pParentGlobalPosition, FbxAMatrix& pGlobalPosition)
	{
		FbxSkeleton* lSkeleton = (FbxSkeleton*) pNode->GetNodeAttribute();

		// Only draw the skeleton if it's a limb node and if 
		// the parent also has an attribute of type skeleton.
		if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode &&
			pNode->GetParent() &&
			pNode->GetParent()->GetNodeAttribute() &&
			pNode->GetParent()->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			GlDrawLimbNode(pParentGlobalPosition, pGlobalPosition); 
		}
	}
	void Cc3dFbxOneLoad::GlDrawLimbNode(FbxAMatrix& pGlobalBasePosition, FbxAMatrix& pGlobalEndPosition)
	{
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)

  
		glColor3f(1.0, 0.0, 0.0);
		glLineWidth(2.0);

		glBegin(GL_LINES);

		glVertex3dv((GLdouble *)pGlobalBasePosition.GetT());
		glVertex3dv((GLdouble *)pGlobalEndPosition.GetT());

		glEnd();
#endif
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//mesh
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Draw the vertices of a mesh.
	void Cc3dFbxOneLoad::DrawMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer,int animStackIndex,
		FbxAMatrix& pGlobalPosition, FbxPose* pPose)
	{
		FbxMesh* lMesh = pNode->GetMesh();
		assert(lMesh);
		{
			Cc3dSkinMesh* mesh=(Cc3dSkinMesh*)m_actor->findSkinMeshByFbxMeshPtr(lMesh);
			if(mesh){
                if(m_isNoAnimation){
                    Cc3dMatrix4 globalPositionMat=FbxAMatrixToCc3dMatrix4(pGlobalPosition);
                    ////    globalPositionMat.print();
                    mesh->setRTSmat(globalPositionMat);
                }else{//animation model
                    Cc3dMatrix4 globalPositionMat=FbxAMatrixToCc3dMatrix4(pGlobalPosition);
                    ////    globalPositionMat.print();
                    int meshAniLayerCount=mesh->getAniLayerCount();
                    if(animStackIndex>=meshAniLayerCount){//aniLayer not exist
                        //create aniLayer
                        Cc3dAniLayer*aniLayer=new Cc3dAniLayer();
                        aniLayer->autorelease();
                        mesh->addAniLayer(aniLayer);
                        assert((int)mesh->getAniLayerCount()==animStackIndex+1);
                    }
                    Cc3dAniLayer*aniLayer=mesh->getAniLayerByIndex(animStackIndex);
                    aniLayer->addAniFrame(Cc3dAniFrame(globalPositionMat,(float)pTime.GetMilliSeconds()/1000));

                }
            }
		}
		const int lVertexCount = lMesh->GetControlPointsCount();
		const int triangleCount = lMesh->GetPolygonCount();
		// No vertex to draw.
		if (lVertexCount == 0)
		{
			return;
		}
		const bool lHasShape = lMesh->GetShapeCount() > 0;
		if(lHasShape){
	//		cout<<"warning: not support shape deform yet!"<<endl;
            //Deform the vertex array with the shapes
            ComputeShapeDeformation(lMesh, pTime, pAnimLayer, animStackIndex);
            
		}
		//获得hasSkin
		bool HasSkin=getHasDeformer(lMesh);
		//获得CLusterCount
		int ClusterCount = 0;
		{
			//we need to get the number of clusters
			const int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);
			
			for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
			{
				ClusterCount += ((FbxSkin *)(lMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
			}//得到lClusterCount

		}

		//{compute deformation}
		if (HasSkin)//{if have skin}
		{
			if (ClusterCount)
			{
				// Deform the vertex array with the skin deformer.
				ComputeSkinDeformation(pGlobalPosition, lMesh, pTime, pPose,animStackIndex);
			}


		}
	}
	// Deform the vertex array according to the links contained in the mesh and the skinning type.
	void Cc3dFbxOneLoad::ComputeSkinDeformation(FbxAMatrix& pGlobalPosition, 
		FbxMesh* pMesh, 
		FbxTime& pTime, 
		FbxPose* pPose,
		int animStackIndex)
	{
		FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(0, FbxDeformer::eSkin);
		FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();


		if(lSkinningType == FbxSkin::eLinear || lSkinningType == FbxSkin::eRigid)
		{
			Cc3dSkinMesh* mesh=(Cc3dSkinMesh*)m_actor->findSkinMeshByFbxMeshPtr(pMesh);
			assert(mesh);
			if(mesh->getSkin()==NULL){
				Cc3dSkin*skin=new Cc3dSkin();
				skin->autorelease();
				skin->setSkinType(lSkinningType);
				mesh->setSkin(skin);
			}
			
			
		

			ComputeLinearDeformation_simplify(pGlobalPosition, pMesh, pTime, pPose,animStackIndex);
		//	//如果想适用于最广的情况,应该用下面这句,但效率会有所降低--abc
		//	ComputeLinearDeformation_unsimplify(pGlobalPosition, pMesh, pTime, pVertexArray,pNormalArray, pPose);
			
		}
	/*	else if(lSkinningType == FbxSkin::eDualQuaternion)
		{
			ComputeDualQuaternionDeformation(pGlobalPosition, pMesh, pTime, pVertexArray, pPose);
		}
		else if(lSkinningType == FbxSkin::eBlend)
		{
			int lVertexCount = pMesh->GetControlPointsCount();

			FbxVector4* lVertexArrayLinear = new FbxVector4[lVertexCount];
			memcpy(lVertexArrayLinear, pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

			FbxVector4* lVertexArrayDQ = new FbxVector4[lVertexCount];
			memcpy(lVertexArrayDQ, pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

			ComputeLinearDeformation(pGlobalPosition, pMesh, pTime, lVertexArrayLinear, pPose);
			ComputeDualQuaternionDeformation(pGlobalPosition, pMesh, pTime, lVertexArrayDQ, pPose);

			// To blend the skinning according to the blend weights
			// Final vertex = DQSVertex * blend weight + LinearVertex * (1- blend weight)
			// DQSVertex: vertex that is deformed by dual quaternion skinning method;
			// LinearVertex: vertex that is deformed by classic linear skinning method;
			int lBlendWeightsCount = lSkinDeformer->GetControlPointIndicesCount();
			for(int lBWIndex = 0; lBWIndex<lBlendWeightsCount; ++lBWIndex)
			{
				double lBlendWeight = lSkinDeformer->GetControlPointBlendWeights()[lBWIndex];
				pVertexArray[lBWIndex] = lVertexArrayDQ[lBWIndex] * lBlendWeight + lVertexArrayLinear[lBWIndex] * (1 - lBlendWeight);
			}
		}*/
	}
	


	
	// Deform the vertex array in classic linear way.{simplified version}
	void Cc3dFbxOneLoad::ComputeLinearDeformation_simplify(FbxAMatrix& pGlobalPosition, 
		FbxMesh* pMesh, 
		FbxTime& pTime, 
		FbxPose* pPose,
		int animStackIndex)
	{
		
		{   //{we only support normalized cluster mode}
			// All the links must have the same link mode.
			FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
			if(lClusterMode!=FbxCluster::eNormalize){
				cout<<"error: currently, only eNormalize cluster mode is supported!"<<endl;
				assert(false);
				//  (1) lClusterMode == FbxCluster::eNormalize:
				//	normalized link mode, a vertex is always totally influenced by the links.
				//  所有link（骨骼)对此顶点的影响权重之和为1
				//  (2) lClusterMode == FbxCluster::eTotalOne:
				//	total 1 link mode, a vertex can be partially influenced by the links. 
				//  所有link（骨骼)对此顶点的影响权重之和totalLinksWeight不足1,其余1-totalLinksWeight由自身贡献--abc
				
			}
		}
		int lVertexCount = pMesh->GetControlPointsCount();
	

		// For all skins and all clusters, accumulate their deformation and weight
		// on each vertices and store them in lClusterDeformation and lClusterWeight.
		int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
		//其实只处理第一个skin即可--abc
		if(lSkinCount>1){//如果多于一个skin,给一个警报--abc
			cout<<"warning: here is more than one skin, only consider the first one."<<endl;
			lSkinCount=1;//强制只处理第一个skin,这样节省计算量--abc
		}
		
		for ( int lSkinIndex=0; lSkinIndex<lSkinCount; ++lSkinIndex)
		{
			FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);

			int lClusterCount = lSkinDeformer->GetClusterCount();//每个cluster对应一个骨骼,所以此值也是骨骼数--abc
			for ( int lClusterIndex=0; lClusterIndex<lClusterCount; ++lClusterIndex)
			{
				FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
				if (!lCluster->GetLink())
					continue;

				FbxAMatrix lVertexTransformMatrix;
				ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);
				
				int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
				int* clusterControlPointIndices=lCluster->GetControlPointIndices();//cluster的顶点索引表--abc
				double* clusterControlPointWeights= lCluster->GetControlPointWeights();//cluster的顶点权重表--abc

				
				Cc3dSkinMesh* mesh=(Cc3dSkinMesh*)m_actor->findSkinMeshByFbxMeshPtr(pMesh);
				assert(mesh);
				if(lClusterIndex>=(int)mesh->getSkin()->getClusterCount()){//cluster not exist
					//create cluster
					Cc3dSkinCluster*cluster=new Cc3dSkinCluster();
					cluster->autorelease();
					mesh->getSkin()->addCluster(cluster);
					assert((int)mesh->getSkin()->getClusterCount()==lClusterIndex+1);
				}
				Cc3dSkinCluster*cluster=mesh->getSkin()->getClusterByIndex(lClusterIndex);
				
				int clusterAniLayerCount=cluster->getAniLayerCount();
				if(animStackIndex>=clusterAniLayerCount){//aniLayer not exist
					//create aniLayer
					Cc3dAniLayer*aniLayer=new Cc3dAniLayer();
					aniLayer->autorelease();
					cluster->addAniLayer(aniLayer);
					assert((int)cluster->getAniLayerCount()==animStackIndex+1);
				}
				Cc3dAniLayer*aniLayer=cluster->getAniLayerByIndex(animStackIndex);
				Cc3dMatrix4 vertexTransformMat=FbxAMatrixToCc3dMatrix4(lVertexTransformMatrix);
				aniLayer->addAniFrame(Cc3dAniFrame(vertexTransformMat,(float)pTime.GetMilliSeconds()/1000));
				if(cluster->getVertexIndexCount()==0){
					cluster->setVertexIndexList(clusterControlPointIndices,lVertexIndexCount);
					cluster->setVertexWeightList(clusterControlPointWeights,lVertexIndexCount);
				}
				              
			}//lClusterCount
		}
		
	}

// Deform the vertex array with the shapes contained in the mesh.
void Cc3dFbxOneLoad::ComputeShapeDeformation(FbxMesh* pMesh, FbxTime& pTime, FbxAnimLayer * pAnimLayer, int animStackIndex)
{
    
    int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);
    for(int lBlendShapeIndex = 0; lBlendShapeIndex<lBlendShapeDeformerCount; ++lBlendShapeIndex)
    {
        FbxBlendShape* lBlendShape = (FbxBlendShape*)pMesh->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);
        

        Cc3dSkinMesh* mesh=(Cc3dSkinMesh*)m_actor->findSkinMeshByFbxMeshPtr(pMesh);
        assert(mesh);
        if(lBlendShapeIndex>=mesh->getBlendShapeCount())
        {
            Cc3dBlendShape*blendShape=new Cc3dBlendShape();
            blendShape->autorelease();
            mesh->addBlendShape(blendShape);
        }

        
        int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
        for(int lChannelIndex = 0; lChannelIndex<lBlendShapeChannelCount; ++lChannelIndex)
        {
            FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
            
            Cc3dBlendShape*blendShape=mesh->getBlendShapeByIndex(lBlendShapeIndex);
            if(lChannelIndex>=blendShape->getBlendShapeChannelCount())
            {
                Cc3dBlendShapeChannel*blendShapeChannel=new Cc3dBlendShapeChannel();
                blendShapeChannel->autorelease();
                blendShape->addBlendShapeChannel(blendShapeChannel);
            }
            
            
            if(lChannel)
            {
                // Get the percentage of influence on this channel.
                FbxAnimCurve* lFCurve = pMesh->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer);
                if (!lFCurve) continue;
                double lWeight = lFCurve->Evaluate(pTime);
                Cc3dBlendShapeChannel*blendShapeChannel=blendShape->getBlendShapeChannelByIndex(lChannelIndex);
                if(animStackIndex>=blendShapeChannel->getAnimCurveCount())
                {
                    Cc3dAnimCurve*aniCurve=new Cc3dAnimCurve();
                    aniCurve->autorelease();
                    blendShapeChannel->addAnimCurve(aniCurve);
                }
                const float weight=maxf(0.000001,minf(100,lWeight));
                blendShapeChannel->getAnimCurveByIndex(animStackIndex)->addAniCurveFrame(Cc3dAnimCurveFrame(weight, (float)pTime.GetMilliSeconds()/1000));
             ///   cout<<"----"<<endl;
             ///   cout<<"weight:"<<weight<<endl;
                
//                 If there is only one targetShape on this channel, the influence is easy to calculate:
//                 influence = (targetShape - baseGeometry) * weight * 0.01
//                 dstGeometry = baseGeometry + influence
//                 
//                 But if there are more than one targetShapes on this channel, this is an in-between
//                 blendshape, also called progressive morph. The calculation of influence is different.
//                 
//                 For example, given two in-between targets, the full weight percentage of first target
//                 is 50, and the full weight percentage of the second target is 100.
//                 When the weight percentage reach 50, the base geometry is already be fully morphed
//                 to the first target shape. When the weight go over 50, it begin to morph from the
//                 first target shape to the second target shape.
//                 
//                 To calculate influence when the weight percentage is 25:
//                 1. 25 falls in the scope of 0 and 50, the morphing is from base geometry to the first target.
//                 2. And since 25 is already half way between 0 and 50, so the real weight percentage change to
//                 the first target is 50.
//                 influence = (firstTargetShape - baseGeometry) * (25-0)/(50-0) * 100
//                 dstGeometry = baseGeometry + influence
//                 
//                 To calculate influence when the weight percentage is 75:
//                 1. 75 falls in the scope of 50 and 100, the morphing is from the first target to the second.
//                 2. And since 75 is already half way between 50 and 100, so the real weight percentage change 
//                 to the second target is 50.
//                 influence = (secondTargetShape - firstTargetShape) * (75-50)/(100-50) * 100
//                 dstGeometry = firstTargetShape + influence
 
                // Find the two shape indices for influence calculation according to the weight.
                // Consider index of base geometry as -1.
                
                int lShapeCount = lChannel->GetTargetShapeCount();
                double* lFullWeights = lChannel->GetTargetShapeFullWeights();
                blendShapeChannel->m_targetShapeFullWeights.resize(lShapeCount);
                for(int i=0;i<(int)blendShapeChannel->m_targetShapeFullWeights.size();i++){
                    blendShapeChannel->m_targetShapeFullWeights[i]=lFullWeights[i];
                    if(blendShapeChannel->m_targetShapeFullWeights[i]>=99.99999){
                        blendShapeChannel->m_targetShapeFullWeights[i]=100.00001;
                        
                    }
                    if(blendShapeChannel->m_targetShapeFullWeights[i]<0.00001){
                        blendShapeChannel->m_targetShapeFullWeights[i]=-0.00001;
                    }
                }
                
                
                const int lTargetShapeCount=lChannel->GetTargetShapeCount();
                for(int lTargetShapeIndex=0;lTargetShapeIndex<lTargetShapeCount;lTargetShapeIndex++){
                    FbxShape* lTargetShape=lChannel->GetTargetShape(lTargetShapeIndex);
                    string targetShapeName=lTargetShape->GetName();
                    if(lTargetShapeIndex>=blendShapeChannel->getTargetShapeCount()){
                        Cc3dTargetShape*targetShape=new Cc3dTargetShape();
                        targetShape->autorelease();
                        blendShapeChannel->addTargetShape(targetShape);
                        targetShape->setTargetShapeName(targetShapeName);
                        //control points
                        const int controlPointsCount=lTargetShape->GetControlPointsCount();
                        for(int j=0;j<controlPointsCount;j++){
                            FbxVector4 lPos=lTargetShape->GetControlPoints()[j];
                            Cc3dVector4 pos(lPos[0],lPos[1],lPos[2],1);
                            targetShape->m_controlPoints.push_back(pos);
                        }
               
                        
                        
                        //controlPoints indices
                        //currently, targetShape->m_controlPointsIndices is no use
                        const int controlPointIndicesCount=lTargetShape->GetControlPointIndicesCount();
                        for(int j=0;j<controlPointIndicesCount;j++){
                            int lIndex=lTargetShape->GetControlPointIndices()[j];
                            targetShape->m_controlPointsIndices.push_back(lIndex);
                            
                        }
                    }
                
                }
            
            }//If lChannel is valid
        }//For each blend shape channel
    }//For each blend shape deformer
}
/*
// Deform the vertex array with the shapes contained in the mesh.
void Cc3dFbxOneLoad::ComputeShapeDeformation(FbxMesh* pMesh, FbxTime& pTime, FbxAnimLayer * pAnimLayer, int animStackIndex)
{
    
    int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);
    for(int lBlendShapeIndex = 0; lBlendShapeIndex<lBlendShapeDeformerCount; ++lBlendShapeIndex)
    {
        FbxBlendShape* lBlendShape = (FbxBlendShape*)pMesh->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);
        
        int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
        for(int lChannelIndex = 0; lChannelIndex<lBlendShapeChannelCount; ++lChannelIndex)
        {
            FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
            if(lChannel)
            {
                // Get the percentage of influence on this channel.
                FbxAnimCurve* lFCurve = pMesh->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer);
                if (!lFCurve) continue;
                double lWeight = lFCurve->Evaluate(pTime);
                
                
                //                 If there is only one targetShape on this channel, the influence is easy to calculate:
                //                 influence = (targetShape - baseGeometry) * weight * 0.01
                //                 dstGeometry = baseGeometry + influence
                //
                //                 But if there are more than one targetShapes on this channel, this is an in-between
                //                 blendshape, also called progressive morph. The calculation of influence is different.
                //
                //                 For example, given two in-between targets, the full weight percentage of first target
                //                 is 50, and the full weight percentage of the second target is 100.
                //                 When the weight percentage reach 50, the base geometry is already be fully morphed
                //                 to the first target shape. When the weight go over 50, it begin to morph from the
                //                 first target shape to the second target shape.
                //
                //                 To calculate influence when the weight percentage is 25:
                //                 1. 25 falls in the scope of 0 and 50, the morphing is from base geometry to the first target.
                //                 2. And since 25 is already half way between 0 and 50, so the real weight percentage change to
                //                 the first target is 50.
                //                 influence = (firstTargetShape - baseGeometry) * (25-0)/(50-0) * 100
                //                 dstGeometry = baseGeometry + influence
                //
                //                 To calculate influence when the weight percentage is 75:
                //                 1. 75 falls in the scope of 50 and 100, the morphing is from the first target to the second.
                //                 2. And since 75 is already half way between 50 and 100, so the real weight percentage change
                //                 to the second target is 50.
                //                 influence = (secondTargetShape - firstTargetShape) * (75-50)/(100-50) * 100
                //                 dstGeometry = firstTargetShape + influence
                
                // Find the two shape indices for influence calculation according to the weight.
                // Consider index of base geometry as -1.
                
                int lShapeCount = lChannel->GetTargetShapeCount();
                double* lFullWeights = lChannel->GetTargetShapeFullWeights();
                
                // Find out which scope the lWeight falls in.
                int lStartIndex = -1;
                int lEndIndex = -1;
                for(int lShapeIndex = 0; lShapeIndex<lShapeCount; ++lShapeIndex)
                {
                    if(lWeight > 0 && lWeight <= lFullWeights[0])
                    {
                        lEndIndex = 0;
                        break;
                    }
                    if(lWeight > lFullWeights[lShapeIndex] && lWeight < lFullWeights[lShapeIndex+1])
                    {
                        lStartIndex = lShapeIndex;
                        lEndIndex = lShapeIndex + 1;
                        break;
                    }
                }
                
                FbxShape* lStartShape = NULL;
                FbxShape* lEndShape = NULL;
                if(lStartIndex > -1)
                {
                    lStartShape = lChannel->GetTargetShape(lStartIndex);
                }
                if(lEndIndex > -1)
                {
                    lEndShape = lChannel->GetTargetShape(lEndIndex);
                }
                
                //The weight percentage falls between base geometry and the first target shape.
                if(lStartIndex == -1 && lEndShape)
                {
                    double lEndWeight = lFullWeights[0];
                    // Calculate the real weight.
                    lWeight = (lWeight/lEndWeight) * 100;
                    // Initialize the lDstVertexArray with vertex of base geometry.
                    memcpy(lDstVertexArray, lSrcVertexArray, lVertexCount * sizeof(FbxVector4));
                    for (int j = 0; j < lVertexCount; j++)
                    {
                        // Add the influence of the shape vertex to the mesh vertex.
                        FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lSrcVertexArray[j]) * lWeight * 0.01;
                        lDstVertexArray[j] += lInfluence;
                    }
                }
                //The weight percentage falls between two target shapes.
                else if(lStartShape && lEndShape)
                {
                    double lStartWeight = lFullWeights[lStartIndex];
                    double lEndWeight = lFullWeights[lEndIndex];
                    // Calculate the real weight.
                    lWeight = ((lWeight-lStartWeight)/(lEndWeight-lStartWeight)) * 100;
                    // Initialize the lDstVertexArray with vertex of the previous target shape geometry.
                    memcpy(lDstVertexArray, lStartShape->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
                    for (int j = 0; j < lVertexCount; j++)
                    {
                        // Add the influence of the shape vertex to the previous shape vertex.
                        FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lStartShape->GetControlPoints()[j]) * lWeight * 0.01;
                        lDstVertexArray[j] += lInfluence;
                    }	
                }
            }//If lChannel is valid
        }//For each blend shape channel
    }//For each blend shape deformer
    
    memcpy(pVertexArray, lDstVertexArray, lVertexCount * sizeof(FbxVector4));
    
    delete [] lDstVertexArray;
}
*/

	// Scale all the elements of a matrix.
	void Cc3dFbxOneLoad::MatrixScale(FbxAMatrix& pMatrix, double pValue)
	{
		int i,j;

		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				pMatrix[i][j] *= pValue;
			}
		}
	}

	// Add a value to all the elements in the diagonal of the matrix.
	void Cc3dFbxOneLoad::MatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue)
	{
		pMatrix[0][0] += pValue;
		pMatrix[1][1] += pValue;
		pMatrix[2][2] += pValue;
		pMatrix[3][3] += pValue;
	}


	// Sum two matrices element by element.
	void Cc3dFbxOneLoad::MatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix)
	{
		int i,j;

		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				pDstMatrix[i][j] += pSrcMatrix[i][j];
			}
		}
	}

/*
//Compute the transform matrix that the cluster will transform the vertex.
	void Cc3dFbxOneLoad::ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, 
		FbxMesh* pMesh,
		FbxCluster* pCluster, 
		FbxAMatrix& pVertexTransformMatrix,
		FbxTime pTime,
		FbxPose* pPose)
	{
		FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();

		FbxAMatrix lReferenceGlobalInitPosition;
		FbxAMatrix lReferenceGlobalCurrentPosition;
		
		FbxAMatrix lClusterGlobalInitPosition;
		FbxAMatrix lClusterGlobalCurrentPosition;

		FbxAMatrix lReferenceGeometry;

		FbxAMatrix lClusterRelativeInitPosition;
		FbxAMatrix lClusterRelativeCurrentPositionInverse;
		//{calculate lClusterRelativeInitPosition}
		{
				pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
				//Multiply lReferenceGlobalInitPosition by Geometric Transformation
				lReferenceGeometry = GetGeometry(pMesh->GetNode());
				lReferenceGlobalInitPosition *= lReferenceGeometry;
				// Get the link initial global position.
				pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
				// Compute the initial position of the link relative to the reference.
				lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;
		}
		//{calculate lClusterRelativeCurrentPositionInverse}
		{
			lReferenceGlobalCurrentPosition = pGlobalPosition;
			// Get the link current global position.
			lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose,NULL);
			// Compute the current position of the link relative to the reference.
			lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

		}
		// Compute the shift of the link relative to the reference.
		pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;


	}    
 */
//Compute the transform matrix that the cluster will transform the vertex.
void Cc3dFbxOneLoad::ComputeClusterDeformation(FbxAMatrix& pGlobalPosition,
                               FbxMesh* pMesh,
                               FbxCluster* pCluster,
                               FbxAMatrix& pVertexTransformMatrix,
                               FbxTime pTime,
                               FbxPose* pPose)
{
    FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();
    
    FbxAMatrix lReferenceGlobalInitPosition;
    FbxAMatrix lReferenceGlobalCurrentPosition;
    FbxAMatrix lAssociateGlobalInitPosition;
    FbxAMatrix lAssociateGlobalCurrentPosition;
    FbxAMatrix lClusterGlobalInitPosition;
    FbxAMatrix lClusterGlobalCurrentPosition;
    
    FbxAMatrix lReferenceGeometry;
    FbxAMatrix lAssociateGeometry;
    FbxAMatrix lClusterGeometry;
    
    FbxAMatrix lClusterRelativeInitPosition;
    FbxAMatrix lClusterRelativeCurrentPositionInverse;
    
    if (lClusterMode == FbxCluster::eAdditive && pCluster->GetAssociateModel())
    {
        pCluster->GetTransformAssociateModelMatrix(lAssociateGlobalInitPosition);
        // Geometric transform of the model
        lAssociateGeometry = GetGeometry(pCluster->GetAssociateModel());
        lAssociateGlobalInitPosition *= lAssociateGeometry;
        lAssociateGlobalCurrentPosition = GetGlobalPosition(pCluster->GetAssociateModel(), pTime, pPose);
        
        pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
        // Multiply lReferenceGlobalInitPosition by Geometric Transformation
        lReferenceGeometry = GetGeometry(pMesh->GetNode());
        lReferenceGlobalInitPosition *= lReferenceGeometry;
        lReferenceGlobalCurrentPosition = pGlobalPosition;
        
        // Get the link initial global position and the link current global position.
        pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
        // Multiply lClusterGlobalInitPosition by Geometric Transformation
        lClusterGeometry = GetGeometry(pCluster->GetLink());
        lClusterGlobalInitPosition *= lClusterGeometry;
        lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);
        
        // Compute the shift of the link relative to the reference.
        //ModelM-1 * AssoM * AssoGX-1 * LinkGX * LinkM-1*ModelM
        pVertexTransformMatrix = lReferenceGlobalInitPosition.Inverse() * lAssociateGlobalInitPosition * lAssociateGlobalCurrentPosition.Inverse() *
        lClusterGlobalCurrentPosition * lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;
    }
    else
    {
        pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
        lReferenceGlobalCurrentPosition = pGlobalPosition;
        // Multiply lReferenceGlobalInitPosition by Geometric Transformation
        lReferenceGeometry = GetGeometry(pMesh->GetNode());
        lReferenceGlobalInitPosition *= lReferenceGeometry;
        
        // Get the link initial global position and the link current global position.
        pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
        lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);
        
        // Compute the initial position of the link relative to the reference.
        lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;
        
        // Compute the current position of the link relative to the reference.
        lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;
        
        // Compute the shift of the link relative to the reference.
        pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;
    }
}