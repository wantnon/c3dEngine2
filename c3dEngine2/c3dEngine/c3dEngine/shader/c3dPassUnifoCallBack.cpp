//
//  c3dPassUnifoCallBack.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-18.
//
//

#include "shader/c3dPassUnifoCallBack.h"
#include "core/c3dSubMesh.h"
void buildinProgramPassUnifoCallback_texColorOnly(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    assert(program->getName()=="shader_texColorOnly");
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=node->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=node->getCamera()->calculateViewMat();
    program->passUnifoValue1i("texture", 0);//texture attach point 0
    program->passUnifoValueMatrixNfv("projMat", projMat.getArray(), projMat.getArrayLen());
	program->passUnifoValueMatrixNfv("modelMat", modelMat.getArray(), modelMat.getArrayLen());
	program->passUnifoValueMatrixNfv("viewMat", viewMat.getArray(), viewMat.getArrayLen());
}
void buildinProgramPassUnifoCallback_classicLighting(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="classicLighting");
	assert(submesh->getLight());
	assert(submesh->getCamera());
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();

	//calculate normal mat
    //http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
    //http://www.songho.ca/opengl/gl_normaltransform.html
    //http://www.arcsynthesis.org/gltut/Illumination/Tut09%20Normal%20Transformation.html
    Cc3dMatrix4 normMat=transpose(inverse(modelMat));
	
	Cc3dVector4 lightPos=submesh->getLight()->getPos();
	Cc3dVector4 eyePos=submesh->getCamera()->getEyePos();
    Cc3dVector4 diffuseMaterial=submesh->getMaterial()->getDiffuse();
    Cc3dVector4 diffuseLight=submesh->getLight()->getDiffuse();
    Cc3dVector4 ambientMaterial=submesh->getMaterial()->getAmbient();
    Cc3dVector4 ambientLight=submesh->getLight()->getAmbient();
	Cc3dVector4 specularMaterial=submesh->getMaterial()->getSpecular();
	Cc3dVector4 specularLight=submesh->getLight()->getSpecular();
	float shininess=submesh->getMaterial()->getShininess();
    //
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
   
    // 
    program->passUnifoValue1i("texture", 0);//texture attach point 0
    program->passUnifoValueMatrixNfv("viewMat", viewMat.getArray(), viewMat.getArrayLen());
	program->passUnifoValueMatrixNfv("projMat", projMat.getArray(), projMat.getArrayLen());
	program->passUnifoValueMatrixNfv("modelMat", modelMat.getArray(), modelMat.getArrayLen());
	program->passUnifoValueMatrixNfv("normMat", normMat.getArray(), normMat.getArrayLen());
    program->passUnifoValueNfv("diffuseMaterial", diffuseMaterial.getArray(),diffuseMaterial.getArrayLen());
	program->passUnifoValueNfv("diffuseLight", diffuseLight.getArray(),diffuseLight.getArrayLen());
    program->passUnifoValueNfv("ambientMaterial", ambientMaterial.getArray(), ambientMaterial.getArrayLen());
	program->passUnifoValueNfv("ambientLight", ambientLight.getArray(), ambientLight.getArrayLen());
	program->passUnifoValueNfv("specularMaterial", specularMaterial.getArray(), specularMaterial.getArrayLen());
	program->passUnifoValueNfv("specularLight", specularLight.getArray(), specularLight.getArrayLen());
	program->passUnifoValue1f("shininess", shininess);
	program->passUnifoValueNfv("lightPos_world", lightPos.getArray(),lightPos.getArrayLen());
	program->passUnifoValueNfv("eyePos_world", eyePos.getArray(),eyePos.getArrayLen());
}