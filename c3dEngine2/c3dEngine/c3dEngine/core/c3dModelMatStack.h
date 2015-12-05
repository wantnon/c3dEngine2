//
//  c3dModelMatStack.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-16.
//
//

#ifndef __HelloOpenGL__c3dModelMatStack__
#define __HelloOpenGL__c3dModelMatStack__

#include <iostream>
#include <vector>
using namespace std;
#include "c3dMatrix.h"
#include "c3dMath.h"
class Cc3dModelMatStack{
public:
    static Cc3dModelMatStack*sharedModelMatStack();
    Cc3dModelMatStack(){
        m_stack.push_back(unitMat());//inital element
    }
    virtual~Cc3dModelMatStack(){}
    void pushMatrix(){
        assert(m_stack.empty()==false);
        Cc3dMatrix4 topMat=m_stack[(int)m_stack.size()-1];//copy top mat
        m_stack.push_back(topMat);
        
    }
    void popMatrix(){
        assert(m_stack.empty()==false);
        int n=(int)m_stack.size();
        m_stack.resize(n-1);
        assert((int)m_stack.size()!=0);//m_stack at least have one element (the inital element)
    }
    void mulMatrix(const Cc3dMatrix4&mat){
        assert(m_stack.empty()==false);
        int n=(int)m_stack.size();
        Cc3dMatrix4 topMat=m_stack[(int)m_stack.size()-1];
        Cc3dMatrix4 newTopMat=topMat*mat;
        //use newTopMat replace top mat
        m_stack[(int)m_stack.size()-1]=newTopMat;
        assert(n==(int)m_stack.size());//m_stack size should not change
    }
    void loadIdentity(){
        assert(m_stack.empty()==false);
        m_stack[(int)m_stack.size()-1]=unitMat();
    }
    void loadMatrix(const Cc3dMatrix4&mat){
        assert(m_stack.empty()==false);
        m_stack[(int)m_stack.size()-1]=mat;
    }
    Cc3dMatrix4 getTopMat()const{
        assert(m_stack.empty()==false);
        return m_stack[(int)m_stack.size()-1];
    }
protected:
    vector<Cc3dMatrix4> m_stack;

};
#endif /* defined(__HelloOpenGL__c3dModelMatStack__) */
