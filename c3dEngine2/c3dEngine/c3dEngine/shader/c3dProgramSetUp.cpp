//
//  c3dProgramSetUp.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-16.
//
//

#include "shader/c3dProgramSetUp.h"
#include "core/c3dProgram.h"
#include "core/c3dProgramCache.h"

void buildinProgramSetUp(){
	string programFolder = "c3dEngineResource/shader";

	//
    {
        string programName="shader_texColorOnly";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("projMat",eUT_float4x4);
		program->addUniform("modelMat",eUT_float4x4);
		program->addUniform("viewMat",eUT_float4x4);
        program->addUniform("texture",eUT_int);
    }
	//
    {
        string programName="classicLighting";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("projMat",eUT_float4x4);
		program->addUniform("viewMat",eUT_float4x4);
        program->addUniform("modelMat",eUT_float4x4);
        program->addUniform("normMat",eUT_float4x4);
        program->addUniform("lightPos_world",eUT_float4);
		program->addUniform("eyePos_world",eUT_float4);
        program->addUniform("diffuseMaterial",eUT_float4);
		program->addUniform("diffuseLight",eUT_float4);
		program->addUniform("ambientMaterial",eUT_float4);
		program->addUniform("ambientLight",eUT_float4);
		program->addUniform("specularMaterial",eUT_float4);
		program->addUniform("specularLight",eUT_float4);
        program->addUniform("shininess",eUT_float);
        
        program->addUniform("texture",eUT_int);
        

    }
  
	{
		//...
	}
    Cc3dProgramCache::sharedProgramCache()->printProgramNames();


}