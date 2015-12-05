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
        program->attachUniform("projMat");
		program->attachUniform("modelMat");
		program->attachUniform("viewMat");
        program->attachUniform("texture");
    }
	//
    {
        string programName="classicLighting";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projMat");
		program->attachUniform("viewMat");
        program->attachUniform("modelMat");
        program->attachUniform("normMat");
        program->attachUniform("texture");
        program->attachUniform("lightPos_world");
		program->attachUniform("eyePos_world");
        program->attachUniform("diffuseMaterial");
		program->attachUniform("diffuseLight");
		program->attachUniform("ambientMaterial");
		program->attachUniform("ambientLight");
		program->attachUniform("specularMaterial");
		program->attachUniform("specularLight");
        program->attachUniform("shininess");
        

    }
  
	{
		//...
	}
    Cc3dProgramCache::sharedProgramCache()->printProgramNames();


}