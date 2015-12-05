//
//  c3dProgramCache.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-16.
//
//

#ifndef __HelloOpenGL__c3dProgramCache__
#define __HelloOpenGL__c3dProgramCache__

#include <iostream>
using namespace std;
#include "c3dProgram.h"
class Cc3dProgramCache{
public:
    static Cc3dProgramCache* sharedProgramCache();
    Cc3dProgram*createProgram(const string&vertShaderFilePath,const string&fragShaderFilePath,const string&name){
        cout<<"create program: "<<"vsh: "<<vertShaderFilePath<<" "<<"fsh: "<<fragShaderFilePath<<endl;
        for(int i=0;i<(int)m_programList.size();i++){
            Cc3dProgram*program=m_programList[i];
            if(program->getVertShaderFilePath()==vertShaderFilePath
               &&program->getFragShaderFilePath()==fragShaderFilePath
               &&program->getName()==name){
                return program;
            }
        }
        Cc3dProgram*program=new Cc3dProgram();
        program->autorelease();
        program->initWithFile(vertShaderFilePath, fragShaderFilePath);
        program->setName(name);
        m_programList.push_back(program);
        program->retain();
        return program;
    }
    Cc3dProgram* getProgramByName(string name){
        for(int i=0;i<(int)m_programList.size();i++){
            Cc3dProgram*program=m_programList[i];
            if(program->getName()==name){
                return program;
            }
        }
	
        return NULL;
    }
    void printProgramNames(){
        cout<<"-----------------------------"<<endl;
        cout<<"programs:"<<endl;
        for(int i=0;i<(int)m_programList.size();i++){
            Cc3dProgram*program=m_programList[i];
            cout<<program->getName()<<" ";
        }cout<<endl;
        cout<<"-----------------------------"<<endl;
    }
protected:
    vector<Cc3dProgram*> m_programList;
};
#endif /* defined(__HelloOpenGL__c3dProgramCache__) */
