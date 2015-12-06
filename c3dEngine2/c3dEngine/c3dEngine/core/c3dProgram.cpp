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

bool isListEqual(const vector<float>&list,const float arr[],int n){
    if((int)list.size()!=n)return false;
    for(int i=0;i<n;i++){
        if(list[i]!=arr[i])return false;
    }
    return true;
}

float Cc3dUniform::getFloat()const {
    assert(m_type==eUT_float);
    if(m_inited){
        return m_floatList[0];
    }else{
        return 0;
    }
}
int Cc3dUniform::getInt()const {
    assert(m_type==eUT_int);
    if(m_inited){
        return m_intList[0];
    }else{
        return 0;
    }
}
Cc3dVector2 Cc3dUniform::getV2()const {
    assert(m_type==eUT_float2);
    if(m_inited){
        return Cc3dVector2(m_floatList[0],m_floatList[1]);
    }else{
        return Cc3dVector2();
    }
}
Cc3dVector3 Cc3dUniform::getV3()const {
    assert(m_type==eUT_float3);
    if(m_inited){
        return Cc3dVector3(m_floatList[0],m_floatList[1],m_floatList[2]);
    }else{
        return Cc3dVector3();
    }
}
Cc3dVector4 Cc3dUniform::getV4()const {
    assert(m_type==eUT_float4);
    if(m_inited){
        return Cc3dVector4(m_floatList[0],m_floatList[1],m_floatList[2],m_floatList[3]);
    }else{
        return Cc3dVector4();
    }
}
Cc3dMatrix4 Cc3dUniform::getM4()const {
    assert(m_type==eUT_float4x4);
    if(m_inited){
        return Cc3dMatrix4(m_floatList[0],m_floatList[1],m_floatList[2],m_floatList[3],
                           m_floatList[4],m_floatList[5],m_floatList[6],m_floatList[7],
                           m_floatList[8],m_floatList[9],m_floatList[10],m_floatList[11],
                           m_floatList[12],m_floatList[13],m_floatList[14],m_floatList[15]);
    }else{
        return Cc3dMatrix4();
    }
}

void Cc3dUniform::set(int x){
    assert(m_type==eUT_int);
    if((int)m_intList.size()!=1||x!=m_intList[0]){
        m_intList.resize(1);
        m_intList[0]=x;
        m_isDirty=true;
    }
    if(m_inited==false){
        m_inited=true;
        m_isDirty=true;
    }
}

void Cc3dUniform::set(float x){
    assert(m_type==eUT_float);
    if((int)m_floatList.size()!=1||x!=m_floatList[0]){
        m_floatList.resize(1);
        m_floatList[0]=x;
        m_isDirty=true;
    }
    if(m_inited==false){
        m_inited=true;
        m_isDirty=true;
    }
}
void Cc3dUniform::set(const Cc3dVector2&v){
    assert(m_type==eUT_float2);
    if(isListEqual(m_floatList,v.getArray(),v.getArrayLen())==false){
        m_floatList=v.convertToVector();
        m_isDirty=true;
    }
    if(m_inited==false){
        m_inited=true;
        m_isDirty=true;
    }
}
void Cc3dUniform::set(const Cc3dVector3&v){
    assert(m_type==eUT_float3);
    if(isListEqual(m_floatList,v.getArray(),v.getArrayLen())==false){
        m_floatList=v.convertToVector();
        m_isDirty=true;
    }
    if(m_inited==false){
        m_inited=true;
        m_isDirty=true;
    }
}
void Cc3dUniform::set(const Cc3dVector4&v){
    assert(m_type==eUT_float4);
    if(isListEqual(m_floatList,v.getArray(),v.getArrayLen())==false){
        m_floatList=v.convertToVector();
        m_isDirty=true;
    }
    if(m_inited==false){
        m_inited=true;
        m_isDirty=true;
    }
}
void Cc3dUniform::set(const Cc3dMatrix4&m){
    assert(m_type==eUT_float4x4);
    if(isListEqual(m_floatList,m.getArray(),m.getArrayLen())==false){
        m_floatList=m.convertToVector();
        m_isDirty=true;
    }
    if(m_inited==false){
        m_inited=true;
        m_isDirty=true;
    }
}




//------------------------------------------------------------------------------------------
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

Cc3dUniform*Cc3dProgram::getpUnifomByName(const string&unifoName){
    const int unifoCount=(int)m_uniformList.size();
    for(int i=0;i<unifoCount;i++){
        Cc3dUniform&unifo=m_uniformList[i];
        if(unifo.m_name==unifoName){
            return &unifo;
        }
    }
    return NULL;
}
void Cc3dProgram::setUniform(const string&unifoName,int x){
    Cc3dUniform*p=getpUnifomByName(unifoName);
    if(p)p->set(x);
}
void Cc3dProgram::setUniform(const string&unifoName,float x){
    Cc3dUniform*p=getpUnifomByName(unifoName);
    if(p)p->set(x);
}
void Cc3dProgram::setUniform(const string&unifoName,const Cc3dVector2&v2){
    Cc3dUniform*p=getpUnifomByName(unifoName);
    if(p)p->set(v2);
}
void Cc3dProgram::setUniform(const string&unifoName,const Cc3dVector3&v3){
    Cc3dUniform*p=getpUnifomByName(unifoName);
    if(p)p->set(v3);
}
void Cc3dProgram::setUniform(const string&unifoName,const Cc3dVector4&v4){
    Cc3dUniform*p=getpUnifomByName(unifoName);
    if(p)p->set(v4);
}
void Cc3dProgram::setUniform(const string&unifoName,const Cc3dMatrix4&m4){
    Cc3dUniform*p=getpUnifomByName(unifoName);
    if(p)p->set(m4);
}

void Cc3dProgram::applyAllDirtyUniforms(){
    const int unifoCount=(int)m_uniformList.size();
    for(int i=0;i<unifoCount;i++){
        Cc3dUniform&unifo=m_uniformList[i];
        if(unifo.m_isDirty){//optim
            applyUniform(unifo);
            unifo.m_isDirty=false;//m_isDirty consumed
        }
    }
}

void Cc3dProgram::applyUniform(const Cc3dUniform&unifo)
//shader can preserve uniform value even though it is unbinded and bind again
//ref: https://www.opengl.org/discussion_boards/showthread.php/185522-glUniform-values-are-preserved-when-shader-is-uninstalled
//and ref: http://www.opengpu.org/bbs/forum.php?mod=viewthread&tid=17637
{
    eUnifoType type=unifo.m_type;
    string name=unifo.m_name;
    switch(type){
        case eUT_int:
            glUniform1i(unifo.m_handle,unifo.getInt());
            break;
        case eUT_float:
            glUniform1f(unifo.m_handle,unifo.getFloat());
            break;
        case eUT_float2:
            glUniform2fv(unifo.m_handle,1,unifo.getV2().getArray());
            break;
        case eUT_float3:
            glUniform3fv(unifo.m_handle,1,unifo.getV3().getArray());
            break;
        case eUT_float4:
            glUniform4fv(unifo.m_handle,1,unifo.getV4().getArray());
            break;
        case eUT_float4x4:
            glUniformMatrix4fv(unifo.m_handle,1,GL_FALSE,unifo.getM4().getArray());
            break;
        default:
            assert(false);
            break;
    }
    
}