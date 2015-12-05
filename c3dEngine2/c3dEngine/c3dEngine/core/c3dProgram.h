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
    string getName()const{return m_name;}
    void setName(string name){m_name=name;}
    void attachUniform(string unifoName){
        m_unifoMap[unifoName] = glGetUniformLocation(m_program,unifoName.c_str());
    }
    GLuint getProgram(){return m_program;}
    void passUnifoValue1f(string unifoName,GLfloat v);
    void passUnifoValue1i(string unifoName,GLint v);
    void passUnifoValueMatrixNfv(string unifoName,const GLfloat *array,int arrayLen);
    void passUnifoValueNfv(string unifoName,const GLfloat *array,int arrayLen);
    void passUnifoValueNiv(string unifoName,const GLint *array,int arrayLen);
    void passUnifoValueMatrixNfv(string unifoName,const vector<GLfloat>&valueList);
    void passUnifoValueNfv(string unifoName,const vector<GLfloat>&valueList);
    void passUnifoValueNiv(string unifoName,const vector<GLint>&valueList);
    void printUnifoMap(){
        cout<<"unifoMap: ";
      /*  typename*/ map<string,GLint>::iterator iter;
        for ( iter = m_unifoMap.begin(); iter != m_unifoMap.end(); iter++ ){
            cout<<iter->first<<":"<<iter->second<<" ";
        }
        cout<<endl;
    }
    string getVertShaderFilePath()const {
        return m_vertShaderFilePath;
    }
    string getFragShaderFilePath()const {
        return m_fragShaderFilePath;
    }
protected:
    GLuint createShader(const char*shaderName,const char*ext);
protected:
    map<string,GLint> m_unifoMap;
    string m_vertShaderFilePath;
    string m_fragShaderFilePath;
    string m_name;
    GLuint m_program;
};
#endif /* defined(__HelloOpenGL__c3dProgram__) */
