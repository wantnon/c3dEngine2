//
//  c3dProgram.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-16.
//
//

#ifndef __HelloOpenGL__c3dProgram__
#define __HelloOpenGL__c3dProgram__

#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;
#include "common/c3dGL.h"

#include "core/c3dObject.h"
#include "core/c3dFileUtils.h"
#include "core/c3dDefine.h"
#include "common/c3dPathFunc.h"
#include "core/c3dVector.h"
#include "core/c3dMatrix.h"
bool isListEqual(const vector<float>&list,const float arr[],int n);

enum eUnifoType{
    eUT_float=0,
    eUT_float2,
    eUT_float3,
    eUT_float4,
    eUT_float4x4,
    eUT_int,
};


class Cc3dUniform{
    
public:
    Cc3dUniform(){
        m_inited=false;
        m_isDirty=false;
        m_inited=false;
        m_type=(eUnifoType)0;
    }
    
    int getInt()const ;
    float getFloat()const ;
    Cc3dVector2 getV2()const ;
    Cc3dVector3 getV3()const ;
    Cc3dVector4 getV4()const ;
    Cc3dMatrix4 getM4()const ;
    void set(int x);
    void set(float x);
    void set(const Cc3dVector2&v);
    void set(const Cc3dVector3&v);
    void set(const Cc3dVector4&v);
    void set(const Cc3dMatrix4&m);
    
public:
    string m_name;
    eUnifoType m_type;
    bool m_isDirty;
    bool m_inited;
    vector<float> m_floatList;
    vector<bool> m_boolList;
    vector<int> m_intList;
    
    GLint m_handle;
    
    
    
    
};


class Cc3dProgram:public Cc3dObject
{
public:
    Cc3dProgram(){
        m_program=0;
        init_dft();
    }
    virtual~Cc3dProgram(){
        if(m_program!=0)glDeleteProgram( m_program );
    }
    void init_dft(){};
    bool initWithFile(const string&vertShaderFilePath,const string&fragShaderFilePath);
    
    GLuint getProgram(){return m_program;}
    
    void setName(string name){m_name=name;}
    string getName()const{return m_name;}
    string getVertShaderFilePath()const {return m_vertShaderFilePath;}
    string getFragShaderFilePath()const {return m_fragShaderFilePath;}
    
    
    void addUniform(const string&name,eUnifoType type){
        Cc3dUniform unifo;
        unifo.m_name=name;
        unifo.m_type=type;
        unifo.m_handle=glGetUniformLocation(m_program,unifo.m_name.c_str());
        m_uniformList.push_back(unifo);
    }
    
    void setSampler(const string&samplerName,int hangPoint);
    void setUniform(const string&unifoName,int x);
    void setUniform(const string&unifoName,float x);
    void setUniform(const string&unifoName,const Cc3dVector2&v2);
    void setUniform(const string&unifoName,const Cc3dVector3&v3);
    void setUniform(const string&unifoName,const Cc3dVector4&v4);
    void setUniform(const string&unifoName,const Cc3dMatrix4&m4);
    Cc3dUniform*getpUnifomByName(const string&unifoName);
    void applyAllDirtyUniforms();
protected:
    void applyUniform(const Cc3dUniform&unifo);
    
    GLuint createShader(const char*shaderName,const char*ext);
protected:

    vector<Cc3dUniform> m_uniformList;
    string m_vertShaderFilePath;
    string m_fragShaderFilePath;
    string m_name;
    GLuint m_program;
};
#endif /* defined(__HelloOpenGL__c3dProgram__) */
