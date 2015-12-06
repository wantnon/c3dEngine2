//
//  programSetUp.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-16.
//
//

#include "programSetUp.h"
#include "c3d.h"

void programSetUp(){
	string programFolder = "singleCellGameResource/shader";

    
    {//
        string programName="shader_diffuse_ambient_specular_noSelfShadow";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("projectionModelview",eUT_float4x4);
        program->addUniform("modelMat",eUT_float4x4);
        program->addUniform("normMat_toWorld",eUT_float4x4);
        program->addUniform("Texture",eUT_int);
        program->addUniform("lightPos_world",eUT_float3);
        program->addUniform("diffuseML",eUT_float4);
        program->addUniform("ambientML",eUT_float4);
        program->addUniform("specularML",eUT_float4);
        program->addUniform("shininess",eUT_float);
        program->addUniform("eyePos_world",eUT_float3);
        program->addUniform("isHighlightUntransp",eUT_int);
        program->addUniform("Texture_shadowMap",eUT_int);
        program->addUniform("worldToLightViewportTexCoord",eUT_float4x4);

    }
  

    {//
        string programName="shaderNoLight";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("projectionModelview",eUT_float4x4);
        program->addUniform("Texture",eUT_int);
        program->addUniform("color",eUT_float4);
        
    }

    {//
        string programName="shader_texOnly_rgb011Transp";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("projectionModelview",eUT_float4x4);
        program->addUniform("Texture",eUT_int);
        program->addUniform("transpColor",eUT_float4);
        
    }

  
    {//
        string programName="shader_texOnly_useTexAsAlpha";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("projectionModelview",eUT_float4x4);
        program->addUniform("Texture",eUT_int);
    }
    
   
    {//
        string programName="shader_diffuse_ambient_noTransf_shadowMap";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("projectionModelview",eUT_float4x4);
        program->addUniform("Texture",eUT_int);
        program->addUniform("color",eUT_float4);
        program->addUniform("lightPos_world",eUT_float3);
        program->addUniform("diffuseML",eUT_float4);
        program->addUniform("ambientML",eUT_float4);
        program->addUniform("Texture_shadowMap",eUT_int);
        program->addUniform("Texture_shadowMap2",eUT_int);
        program->addUniform("worldToLightViewportTexCoord",eUT_float4x4);
        program->addUniform("worldToLightViewportTexCoord2",eUT_float4x4);
    }

    {//
        string programName="shader_diffuse_ambient_noTransf_noSelfShadow";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("projectionModelview",eUT_float4x4);
        program->addUniform("Texture",eUT_int);
        program->addUniform("lightPos_world",eUT_float3);
        program->addUniform("diffuseML",eUT_float4);
        program->addUniform("ambientML",eUT_float4);
        program->addUniform("Texture_shadowMap",eUT_int);
        program->addUniform("Texture_shadowMap2",eUT_int);
        program->addUniform("worldToLightViewportTexCoord",eUT_float4x4);
        program->addUniform("worldToLightViewportTexCoord2",eUT_float4x4);
    }
    
 
    
    {//
        string programName="shader_diffuse_ambient_noSelfShadow";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("normMat_toWorld",eUT_float4x4);
        program->addUniform("modelMat",eUT_float4x4);
        program->addUniform("projectionModelview",eUT_float4x4);
        program->addUniform("Texture",eUT_int);
        program->addUniform("lightPos_world",eUT_float3);
        program->addUniform("diffuseML",eUT_float4);
        program->addUniform("ambientML",eUT_float4);
        program->addUniform("Texture_shadowMap",eUT_int);
        program->addUniform("Texture_shadowMap2",eUT_int);
        program->addUniform("worldToLightViewportTexCoord",eUT_float4x4);
        
    }

    {//
        string programName="shader_texOnly_noSelfShadow";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("projectionModelview",eUT_float4x4);
        program->addUniform("modelMat",eUT_float4x4);
        program->addUniform("Texture",eUT_int);
        program->addUniform("lightPos_world",eUT_float3);
        program->addUniform("Texture_shadowMap",eUT_int);
        program->addUniform("Texture_shadowMap2",eUT_int);
        program->addUniform("worldToLightViewportTexCoord",eUT_float4x4);
        
    }
    
    {//
        string programName="shader_water";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("projectionModelview",eUT_float4x4);
        program->addUniform("Texture",eUT_int);
        program->addUniform("maxAlpha",eUT_float);
    }

 
    {//
        string programName="shader_renderDepth";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->addUniform("projectionModelview",eUT_float4x4);
    }

    
    Cc3dProgramCache::sharedProgramCache()->printProgramNames();


}