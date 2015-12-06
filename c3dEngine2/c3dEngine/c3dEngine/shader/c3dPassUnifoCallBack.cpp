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
    program->setUniform("texture", 0);//texture attach point 0
    program->setUniform("projMat", projMat);
	program->setUniform("modelMat", modelMat);
	program->setUniform("viewMat", viewMat);
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
    program->setUniform("texture", 0);//texture attach point 0
    program->setUniform("viewMat", viewMat);
	program->setUniform("projMat", projMat);
	program->setUniform("modelMat", modelMat);
	program->setUniform("normMat", normMat);
    program->setUniform("diffuseMaterial", diffuseMaterial);
	program->setUniform("diffuseLight", diffuseLight);
    program->setUniform("ambientMaterial", ambientMaterial);
	program->setUniform("ambientLight", ambientLight);
	program->setUniform("specularMaterial", specularMaterial);
	program->setUniform("specularLight", specularLight);
	program->setUniform("shininess", shininess);
	program->setUniform("lightPos_world", lightPos);
	program->setUniform("eyePos_world", eyePos);
}