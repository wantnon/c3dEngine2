//
//  c3dProgram.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-16.
//
//

#include "core/c3dProgram.h"
#include "core/c3dAssert.h"
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
#include "platform/win32/c3dProgramPlatformDepend.h"
#else
#include "platform/iOS/c3dProgramPlatformDepend.h"
#endif
bool Cc3dProgram::initWithFile(const string&vertShaderFilePath,const string&fragShaderFilePath){
    m_vertShaderFilePath=vertShaderFilePath;
    m_fragShaderFilePath=fragShaderFilePath;
    //create shader
    vector<string> _vertShaderFileName=splitStrInTwoByLastDotBehindLastBar(vertShaderFilePath);
    vector<string> _fragShaderFileName=splitStrInTwoByLastDotBehindLastBar(fragShaderFilePath);
    //创建Program
    GLuint vertShader=createShader(_vertShaderFileName[0].c_str(), _vertShaderFileName[1].c_str());
    GLuint fragShader=createShader(_fragShaderFileName[0].c_str(), _fragShaderFileName[1].c_str());
    //Calls glCreateProgram, glAttachShader, and glLinkProgram to link the vertex and fragment shaders into a complete program.
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertShader);
    glAttachShader(programHandle, fragShader);
    
    //needs to be done prior to linking
    {
        glBindAttribLocation(programHandle, ATTRIB_LOC_position, "a_position");
        glBindAttribLocation(programHandle, ATTRIB_LOC_texCoord, "a_texCoord");
        glBindAttribLocation(programHandle, ATTRIB_LOC_normal, "a_normal");
        glBindAttribLocation(programHandle, ATTRIB_LOC_color, "a_color");
        glBindAttribLocation(programHandle, ATTRIB_LOC_texCoord2, "a_texCoord2");
    }
    glLinkProgram(programHandle);//after link, the index get by glGet**Location became fixed
    //check and see if there were any link errors
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[1024];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
		cout<<"vertShaderFilePath:"<<vertShaderFilePath<<endl;
		cout<<"fragShaderFilePath:"<<fragShaderFilePath<<endl;
		cout<<messages<<endl;
        C3DASSERT(false,messages);
    }
    m_program=programHandle;
    return true;
}
GLuint Cc3dProgram::createShader(const char*shaderName,const char*ext)
{
    return createShader_plat(shaderName,ext);
}

void Cc3dProgram::passUnifoValue1f(string unifoName,GLfloat v){
    glUniform1f(m_unifoMap[unifoName],v);
}
void Cc3dProgram::passUnifoValue1i(string unifoName,GLint v){
    glUniform1i(m_unifoMap[unifoName],v);
}
void Cc3dProgram::passUnifoValueMatrixNfv(string unifoName,const GLfloat *array,int arrayLen){
    assert(arrayLen==4||arrayLen==9||arrayLen==16);
    switch (arrayLen) {
        case 4:
            glUniformMatrix2fv(m_unifoMap[unifoName],1,GL_FALSE,array);
            break;
        case 9:
            glUniformMatrix3fv(m_unifoMap[unifoName],1,GL_FALSE,array);
            break;
        case 16:
            glUniformMatrix4fv(m_unifoMap[unifoName],1,GL_FALSE,array);
            break;
        default:
            assert(false);
            break;
    }
    
}
void Cc3dProgram::passUnifoValueNfv(string unifoName,const GLfloat*array,int arrayLen){
    assert(arrayLen>=1&&arrayLen<=4);
    switch (arrayLen) {
        case 1:
            glUniform1fv(m_unifoMap[unifoName],1,array);
            break;
        case 2:
            glUniform2fv(m_unifoMap[unifoName],1,array);
            break;
        case 3:
            glUniform3fv(m_unifoMap[unifoName],1,array);
            break;
        case 4:
            glUniform4fv(m_unifoMap[unifoName],1,array);
            break;
        default:
            assert(false);
            break;
    }
    
}
void Cc3dProgram::passUnifoValueNiv(string unifoName,const GLint*array,int arrayLen){
    assert(arrayLen>=1&&arrayLen<=4);
    switch (arrayLen) {
        case 1:
            glUniform1iv(m_unifoMap[unifoName],1,array);
            break;
        case 2:
            glUniform2iv(m_unifoMap[unifoName],1,array);
            break;
        case 3:
            glUniform3iv(m_unifoMap[unifoName],1,array);
            break;
        case 4:
            glUniform4iv(m_unifoMap[unifoName],1,array);
            break;
        default:
            assert(false);
            break;
    }
    
}
void Cc3dProgram::passUnifoValueMatrixNfv(string unifoName,const vector<GLfloat>&valueList){
    passUnifoValueMatrixNfv(unifoName, &valueList.front(), (int)valueList.size());
}
void Cc3dProgram::passUnifoValueNfv(string unifoName,const vector<GLfloat>&valueList){
    passUnifoValueNfv(unifoName, &valueList.front(), (int)valueList.size());
}
void Cc3dProgram::passUnifoValueNiv(string unifoName,const vector<GLint>&valueList){
    passUnifoValueNiv(unifoName, &valueList.front(), (int)valueList.size());
}

