//
//  passUnifoCallBack.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-18.
//
//

#include "passUnifoCallBack.h"
void passUnifoCallback_diffuse_ambient_noTransf_shadowMap(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="shader_diffuse_ambient_noTransf_shadowMap");
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();
    Cc3dVector4 diffuseM=submesh->getMaterial()->getDiffuse();
    assert(submesh->getLight());
    Cc3dVector4 diffuseL=submesh->getLight()->getDiffuse();
    Cc3dVector4 ambientM=submesh->getMaterial()->getAmbient();
    Cc3dVector4 ambientL=submesh->getLight()->getAmbient();
    //
    Cc3dVector4 color=Cc3dVector4(1,1,1,1);
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    Cc3dVector4 diffuseML=componentProduct(diffuseM,diffuseL);
    diffuseML.seta(diffuseM.a());
    Cc3dVector4 ambientML=componentProduct(ambientM, ambientL);
    Cc3dVector3 lightPos=submesh->getLight()->getPos().toV3();
    //
    Cc3dMatrix4 worldToLightViewportTexCoord=calculateWorldToViewportTexCoordMatrix(submesh->getLight()->getLightViewCameraByIndex(0)->calculateViewMat(),
                                                                               submesh->getLight()->getLightViewCameraByIndex(0)->calculateProjectionMat());
    Cc3dMatrix4 worldToLightViewportTexCoord2=calculateWorldToViewportTexCoordMatrix(submesh->getLight()->getLightViewCameraByIndex(1)->calculateViewMat(),
                                                                                submesh->getLight()->getLightViewCameraByIndex(1)->calculateProjectionMat());

    //
    program->setUniform("Texture", 0);//texture attach point 0
    program->setUniform("color", color);
    program->setUniform("projectionModelview", PVMmat);
    program->setUniform("diffuseML", diffuseML);
    program->setUniform("lightPos_world", lightPos);
    program->setUniform("ambientML", ambientML);
    program->setUniform("Texture_shadowMap", 1);
    program->setUniform("Texture_shadowMap2", 2);
    program->setUniform("worldToLightViewportTexCoord",worldToLightViewportTexCoord);
    program->setUniform("worldToLightViewportTexCoord2",worldToLightViewportTexCoord2);

}
void passUnifoCallback_diffuse_ambient_noTransf_noSelfShadow(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="shader_diffuse_ambient_noTransf_noSelfShadow");
    Cc3dMatrix4 modelMat=submesh->getIsIgnorTransform()?unitMat():Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();
    Cc3dVector4 diffuseM=submesh->getMaterial()->getDiffuse();
    assert(submesh->getLight());
    Cc3dVector4 diffuseL=submesh->getLight()->getDiffuse();
    Cc3dVector4 ambientM=submesh->getMaterial()->getAmbient();
    Cc3dVector4 ambientL=submesh->getLight()->getAmbient();
    //
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    Cc3dVector4 diffuseML=componentProduct(diffuseM,diffuseL);
    diffuseML.seta(diffuseM.a());
    Cc3dVector4 ambientML=componentProduct(ambientM, ambientL);
    Cc3dVector3 lightPos=submesh->getLight()->getPos().toV3();
    //
    Cc3dMatrix4 worldToLightViewportTexCoord=calculateWorldToViewportTexCoordMatrix(submesh->getLight()->getLightViewCameraByIndex(0)->calculateViewMat(),
                                                                               submesh->getLight()->getLightViewCameraByIndex(0)->calculateProjectionMat());
    Cc3dMatrix4 worldToLightViewportTexCoord2=calculateWorldToViewportTexCoordMatrix(submesh->getLight()->getLightViewCameraByIndex(1)->calculateViewMat(),
                                                                                submesh->getLight()->getLightViewCameraByIndex(1)->calculateProjectionMat());
    
    //
    program->setUniform("Texture", 0);//texture attach point 0
    program->setUniform("projectionModelview", PVMmat);
    program->setUniform("diffuseML", diffuseML);
    program->setUniform("lightPos_world", lightPos);
    program->setUniform("ambientML", ambientML);
    program->setUniform("Texture_shadowMap", 1);
    program->setUniform("Texture_shadowMap2", 2);
    program->setUniform("worldToLightViewportTexCoord",worldToLightViewportTexCoord);
    program->setUniform("worldToLightViewportTexCoord2",worldToLightViewportTexCoord2);

}
void passUnifoCallback_diffuse_ambient_noSelfShadow(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="shader_diffuse_ambient_noSelfShadow");
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();
    Cc3dVector4 diffuseM=submesh->getMaterial()->getDiffuse();
    assert(submesh->getLight());
    Cc3dVector4 diffuseL=submesh->getLight()->getDiffuse();
    Cc3dVector4 ambientM=submesh->getMaterial()->getAmbient();
    Cc3dVector4 ambientL=submesh->getLight()->getAmbient();
    Cc3dVector4 specularL=submesh->getMaterial()->getSpecular();
    Cc3dVector4 specularM=submesh->getLight()->getSpecular();
    //
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    Cc3dMatrix4 normalMat=transpose(inverse(modelMat));
    Cc3dVector4 diffuseML=componentProduct(diffuseM,diffuseL);
    diffuseML.seta(diffuseM.a());
    Cc3dVector4 ambientML=componentProduct(ambientM, ambientL);
    Cc3dVector4 specularML=componentProduct(specularM, specularL);
    Cc3dVector3 lightPos=submesh->getLight()->getPos().toV3();
    //
    Cc3dMatrix4 worldToLightViewportTexCoord=calculateWorldToViewportTexCoordMatrix(submesh->getLight()->getLightViewCameraByIndex(0)->calculateViewMat(),
                                                                               submesh->getLight()->getLightViewCameraByIndex(0)->calculateProjectionMat());

    //
    program->setUniform("Texture", 0);//texture attach point 0
    program->setUniform("projectionModelview", PVMmat);
    program->setUniform("modelMat", modelMat);
    program->setUniform("lightPos_world", lightPos);
    program->setUniform("diffuseML", diffuseML);
    program->setUniform("ambientML", ambientML);
    program->setUniform("Texture_shadowMap", 1);
    program->setUniform("Texture_shadowMap2", 2);
    program->setUniform("worldToLightViewportTexCoord",worldToLightViewportTexCoord);
    program->setUniform("normMat_toWorld", normalMat);
}
void passUnifoCallback_water(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="shader_water");
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();
    //
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    program->setUniform("Texture", 0);//texture attach point 0
    program->setUniform("projectionModelview", PVMmat);
    program->setUniform("maxAlpha",0.5f);//0.6);
}
void passUnifoCallback_texOnly_useTexAsAlpha(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="shader_texOnly_useTexAsAlpha");
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();
    //
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    program->setUniform("Texture", 0);//texture attach point 0
    program->setUniform("projectionModelview", PVMmat);
}
void passUnifoCallback_diffuse_ambient_specular_noSelfShadow(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="shader_diffuse_ambient_specular_noSelfShadow");
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();
    Cc3dVector4 diffuseM=submesh->getMaterial()->getDiffuse();
    assert(submesh->getLight());
    Cc3dVector4 diffuseL=submesh->getLight()->getDiffuse();
    Cc3dVector4 ambientM=submesh->getMaterial()->getAmbient();
    Cc3dVector4 ambientL=submesh->getLight()->getAmbient();
    Cc3dVector4 specularL=submesh->getMaterial()->getSpecular();
    Cc3dVector4 specularM=submesh->getLight()->getSpecular();
    //
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    Cc3dMatrix4 normalMat=transpose(inverse(modelMat));
    Cc3dVector4 diffuseML=componentProduct(diffuseM,diffuseL);
    diffuseML.seta(diffuseM.a());
    Cc3dVector4 ambientML=componentProduct(ambientM, ambientL);
    Cc3dVector4 specularML=componentProduct(specularM, specularL);
    float shininess=submesh->getMaterial()->getShininess();
    Cc3dVector3 lightPos=submesh->getLight()->getPos().toV3();
    //
    Cc3dMatrix4 worldToLightViewportTexCoord=calculateWorldToViewportTexCoordMatrix(submesh->getLight()->getLightViewCameraByIndex(0)->calculateViewMat(),
                                                                               submesh->getLight()->getLightViewCameraByIndex(0)->calculateProjectionMat());
    //
    program->setUniform("Texture", 0);//texture attach point 0
    program->setUniform("projectionModelview", PVMmat);
    program->setUniform("diffuseML", diffuseML);
    program->setUniform("lightPos_world", lightPos);
    program->setUniform("ambientML", ambientML);
    program->setUniform("Texture_shadowMap", 1);
    program->setUniform("worldToLightViewportTexCoord",worldToLightViewportTexCoord);
    program->setUniform("specularML",specularML);
    program->setUniform("modelMat",modelMat);
    program->setUniform("normMat_toWorld",normalMat);
    program->setUniform("shininess",shininess);
    program->setUniform("isHighlightUntransp",(int)1);
    program->setUniform("eyePos_world",submesh->getCamera()->getEyePos().toV3());
}

void passUnifoCallback_texOnly_noSelfShadow(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="shader_texOnly_noSelfShadow");
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();
    Cc3dVector4 diffuseM=submesh->getMaterial()->getDiffuse();
    assert(submesh->getLight());
    Cc3dVector4 diffuseL=submesh->getLight()->getDiffuse();
    Cc3dVector4 ambientM=submesh->getMaterial()->getAmbient();
    Cc3dVector4 ambientL=submesh->getLight()->getAmbient();
    Cc3dVector4 specularL=submesh->getMaterial()->getSpecular();
    Cc3dVector4 specularM=submesh->getLight()->getSpecular();
    //
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    Cc3dMatrix4 normalMat=transpose(inverse(modelMat));
    Cc3dVector3 lightPos=submesh->getLight()->getPos().toV3();
    //
    Cc3dMatrix4 worldToLightViewportTexCoord=calculateWorldToViewportTexCoordMatrix(submesh->getLight()->getLightViewCameraByIndex(0)->calculateViewMat(),
                                                                               submesh->getLight()->getLightViewCameraByIndex(0)->calculateProjectionMat());
    //
    program->setUniform("Texture", 0);//texture attach point 0
    program->setUniform("projectionModelview", PVMmat);
    program->setUniform("lightPos_world", lightPos);
    program->setUniform("Texture_shadowMap", 1);
    program->setUniform("worldToLightViewportTexCoord",worldToLightViewportTexCoord);
    program->setUniform("modelMat",modelMat);
    program->setUniform("Texture_shadowMap2", 2);
}

void passUnifoCallback_texOnly_blackTransp(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="shader_texOnly_rgb011Transp");
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();
    //
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    Cc3dVector4 blackColor(0,0,0,1);

    program->setUniform("Texture", 0);//texture attach point 0
    program->setUniform("projectionModelview", PVMmat);
    program->setUniform("transpColor", blackColor);

}

void passUnifoCallback_texOnly_brightBlueTransp(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="shader_texOnly_rgb011Transp");
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();
    //
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    Cc3dVector4 brightBlueColor(0,1,1,1);
    
    program->setUniform("Texture", 0);//texture attach point 0
    program->setUniform("projectionModelview", PVMmat);
    program->setUniform("transpColor", brightBlueColor);
    
}

void passUnifoCallback_renderDepth(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="shader_renderDepth");
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();
    //
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    program->setUniform("projectionModelview", PVMmat);

}
void passUnifoCallback_noLight(Cc3dNode*node, Cc3dProgram*program){
    assert(node);
    assert(program);
    Cc3dSubMesh*submesh=(Cc3dSubMesh*)node;
    assert(program->getName()=="shaderNoLight");
    Cc3dMatrix4 modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    Cc3dMatrix4 projMat=submesh->getCamera()->calculateProjectionMat();
    Cc3dMatrix4 viewMat=submesh->getCamera()->calculateViewMat();
    //
    Cc3dMatrix4 PVMmat=projMat*viewMat*modelMat;
    Cc3dVector4 color=submesh->getMaterial()->getColor();
    
    program->setUniform("Texture", 0);//texture attach point 0
    program->setUniform("projectionModelview", PVMmat);
    program->setUniform("color", color);
    

}