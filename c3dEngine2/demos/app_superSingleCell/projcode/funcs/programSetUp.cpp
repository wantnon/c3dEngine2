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
   /* {//x
        string programName="shader0";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programName+".vert", programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("modelMat");
        program->attachUniform("normMat_toWorld");
        program->attachUniform("Texture");
        program->attachUniform("lightPos_world");
        program->attachUniform("diffuseML");
        program->attachUniform("ambientML");
        program->attachUniform("specularML");
        program->attachUniform("shininess");
        program->attachUniform("eyePos_world");
        program->attachUniform("isHighlightUntransp");
        
        program->printUnifoMap();
        
    }*/
    
    {//
        string programName="shader_diffuse_ambient_specular_noSelfShadow";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("modelMat");
        program->attachUniform("normMat_toWorld");
        program->attachUniform("Texture");
        program->attachUniform("lightPos_world");
        program->attachUniform("diffuseML");
        program->attachUniform("ambientML");
        program->attachUniform("specularML");
        program->attachUniform("shininess");
        program->attachUniform("eyePos_world");
        program->attachUniform("isHighlightUntransp");
        program->attachUniform("Texture_shadowMap");
        program->attachUniform("worldToLightViewportTexCoord");

    }
  

    {//
        string programName="shaderNoLight";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("Texture");
        program->attachUniform("color");
        
        program->printUnifoMap();
    }

    {//
        string programName="shader_texOnly_rgb011Transp";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("Texture");
        program->attachUniform("transpColor");
        
    }

   /* {//x
        string programName="shader_points";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("Texture");
        program->attachUniform("pointSize");
    }*/

    {//
        string programName="shader_texOnly_useTexAsAlpha";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("Texture");
    }
    
   
    {//
        string programName="shader_diffuse_ambient_noTransf_shadowMap";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("Texture");
        program->attachUniform("color");
        program->attachUniform("lightPos_world");
        program->attachUniform("diffuseML");
        program->attachUniform("ambientML");
        program->attachUniform("Texture_shadowMap");
        program->attachUniform("Texture_shadowMap2");
        program->attachUniform("worldToLightViewportTexCoord");
        program->attachUniform("worldToLightViewportTexCoord2");
    }

    {//
        string programName="shader_diffuse_ambient_noTransf_noSelfShadow";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("Texture");
        program->attachUniform( "lightPos_world");
        program->attachUniform("diffuseML");
        program->attachUniform("ambientML");
        program->attachUniform("Texture_shadowMap");
        program->attachUniform("Texture_shadowMap2");
        program->attachUniform("worldToLightViewportTexCoord");
        program->attachUniform("worldToLightViewportTexCoord2");
    }
    
  /*  {//x
        string programName="shader_diffuse_ambient_noTransf_noSelfShadow_texBlend";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("lightPos_world");
        program->attachUniform("diffuseML");
        program->attachUniform("ambientML");
        program->attachUniform("Texture_shadowMap");
        program->attachUniform("Texture_shadowMap2");
        program->attachUniform("worldToLightViewportTexCoord");
        program->attachUniform("worldToLightViewportTexCoord2");
        program->attachUniform("Texture_alpha");
        program->attachUniform("Texture1");
        program->attachUniform("Texture2");
        program->attachUniform("Texture3");
    }*/

    
    {//
        string programName="shader_diffuse_ambient_noSelfShadow";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("normMat_toWorld");
        program->attachUniform("modelMat");
        program->attachUniform("projectionModelview");
        program->attachUniform("Texture");
        program->attachUniform("lightPos_world");
        program->attachUniform("diffuseML");
        program->attachUniform("ambientML");
        program->attachUniform("Texture_shadowMap");
        program->attachUniform("Texture_shadowMap2");
        program->attachUniform("worldToLightViewportTexCoord");
        
    }

    {//
        string programName="shader_texOnly_noSelfShadow";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("modelMat");
        program->attachUniform("Texture");
        program->attachUniform("lightPos_world");
        program->attachUniform("Texture_shadowMap");
        program->attachUniform("Texture_shadowMap2");
        program->attachUniform("worldToLightViewportTexCoord");
        
    }
    
    {//
        string programName="shader_water";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("Texture");
        program->attachUniform("maxAlpha");
    }

  /*  {//x
        string programName="shader_showDepthTex";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
        program->attachUniform("Texture");
        
    }*/

    {//
        string programName="shader_renderDepth";
        Cc3dProgram*program=Cc3dProgramCache::sharedProgramCache()->createProgram(programFolder+"/"+programName+".vert", programFolder+"/"+programName+".frag", programName);
        program->attachUniform("projectionModelview");
    }

    
    Cc3dProgramCache::sharedProgramCache()->printProgramNames();


}