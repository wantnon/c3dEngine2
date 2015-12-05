#ifndef _c3dFbxOneLoad__
#define _c3dFbxOneLoad__
#include <string>
using namespace std;
#include "core/c3dObject.h"

#define FBXSDK_NEW_API 0
#include "fbxsdk.h"
#include "core/c3dMatrix.h"
#include "fbxLoader/c3dSkinActor.h"

//about fbx skin animation principle, see: http://user.qzone.qq.com/350479720/blog/1349982989
//bug:
//models in 3dmax should not have redundant material, Multi/Sub-Object material should only contain material balls that actually in use. or there will be errors when export fbx to my own formate.
Cc3dMatrix4 FbxAMatrixToCc3dMatrix4(const FbxAMatrix&m);

class Cc3dFbxOneLoad:public Cc3dObject
{
public:
	char*fbxFileName;
	FbxManager* lSdkManager;
	FbxScene* lScene;
	Cc3dSkinActor* m_actor;
    bool m_isNoAnimation;
	//
	Cc3dFbxOneLoad(){
		fbxFileName=NULL;
		lSdkManager = NULL;
	    lScene = NULL;
		m_actor=NULL;
		{
			m_actor=new Cc3dSkinActor();
			m_actor->autorelease();
			m_actor->retain();
		}
        m_isNoAnimation=false;
	}
	~Cc3dFbxOneLoad(){
		assert(m_actor);
		m_actor->release();
		if(lSdkManager!=NULL)destroyManager();
	}
	Cc3dSkinActor* convertToSkinActor(float aniFrameInterval){
       

		//trianglulate
		//TriangulateRecursive(lScene->GetRootNode());// Convert mesh, NURBS and patch into triangle mesh

		// Convert mesh, NURBS and patch into triangle mesh //2014-9-27
		FbxGeometryConverter lGeomConverter(lSdkManager);
		lGeomConverter.Triangulate(lScene, /*replace*/true);
        
		
		//makeSubMeshes
		makeSubMeshSetForEachNode(lScene->GetRootNode());
		//bake animation
		bakeAnimation(aniFrameInterval);
        //attach target mesh to blendShapeChannel
        attachMeshToTargetShapes(true);
        
		//m_actor done
		return m_actor;
		
	}
    void attachMeshToTargetShapes(bool isHideTargetShapeMeshes);
	void bakeAnimation(float aniFrameInterval);
	void Init_and_load(const char* _fbxFileName);
	void makeSubMeshSetForThisNode(FbxNode* pNode);
	void makeSubMeshSetForEachNode(FbxNode* pNode);
	bool getHasDeformer(FbxMesh*lMesh);
	void updateSkin(FbxTime&Time,FbxAnimStack *lCurrentAnimationStack,int animStackIndex);
	void destroyManager();
	void ComputeLinearDeformation_simplify(FbxAMatrix& pGlobalPosition, 
		FbxMesh* pMesh, 
		FbxTime& pTime, 
		FbxPose* pPose,
		int animStackIndex);
    void ComputeShapeDeformation(FbxMesh* pMesh, FbxTime& pTime, FbxAnimLayer * pAnimLayer, int animStackIndex);
	void InitializeSdkObjects(FbxManager*& pSdkManager, FbxScene*& pScene);
	bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);
	void DestroySdkObjects(FbxManager* &pSdkManager);
	// Draw the vertices of a mesh.
	void DrawMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer,int animStackIndex,
		FbxAMatrix& pGlobalPosition, FbxPose* pPose);
	//get mesh smoothing info
	//set pCompute true to compute smoothing from normals by default 
	//set pConvertToSmoothingGroup true to convert hard/soft edge info to smoothing group info by default
	void GetSmoothing(FbxManager* pSdkManager, FbxNode* pNode, bool pCompute, bool pConvertToSmoothingGroup);
    // Triangulate all NURBS, patch and mesh under this node recursively.
    void TriangulateRecursive(FbxNode* pNode);	
	// Draw recursively each node of the scene. To avoid recomputing 
	// uselessly the global positions, the global position of each 
	// node is passed to it's children while browsing the node tree.
	// If the node is part of the given pose for the current scene,
	// it will be drawn at the position specified in the pose, Otherwise
	// it will be drawn at the given time.
	void DrawNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer,int animStackIndex,
		FbxAMatrix& pParentGlobalPosition, FbxPose* pPose);
	// Get the global position of the node for the current pose.
	// If the specified node is not part of the pose or no pose is specified, get its
	// global position at the current time.
	FbxAMatrix GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime,FbxPose* pPose, FbxAMatrix* pParentGlobalPosition=NULL);
	// Get the matrix of the given pose
	FbxAMatrix GetPoseMatrix(FbxPose* pPose, int pNodeIndex);
	// Get the geometry offset to a node. It is never inherited by the children.
	FbxAMatrix GetGeometry(FbxNode* pNode);
	// Draw the node following the content of it's node attribute.
	void DrawNode(FbxNode* pNode, 
		FbxTime& pTime, 
		FbxAnimLayer* pAnimLayer,
		int animStackIndex,
		FbxAMatrix& pParentGlobalPosition,
		FbxAMatrix& pGlobalPosition,
		FbxPose* pPose);
	// Draw a limb between the node and its parent.
	void DrawSkeleton(FbxNode* pNode, FbxAMatrix& pParentGlobalPosition, FbxAMatrix& pGlobalPosition);
	void GlDrawLimbNode(FbxAMatrix& pGlobalBasePosition, FbxAMatrix& pGlobalEndPosition);
	// Deform the vertex array according to the links contained in the mesh and the skinning type.
	void ComputeSkinDeformation(FbxAMatrix& pGlobalPosition, 
		FbxMesh* pMesh, 
		FbxTime& pTime, 
		FbxPose* pPose,
		int animStackIndex);
	// Scale all the elements of a matrix.
	void MatrixScale(FbxAMatrix& pMatrix, double pValue);
	// Add a value to all the elements in the diagonal of the matrix.
	void MatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue);
	// Sum two matrices element by element.
	void MatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix);
	//Compute the transform matrix that the cluster will transform the vertex.
	void ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, 
		FbxMesh* pMesh,
		FbxCluster* pCluster, 
		FbxAMatrix& pVertexTransformMatrix,
		FbxTime pTime,
		FbxPose* pPose);

};
#endif