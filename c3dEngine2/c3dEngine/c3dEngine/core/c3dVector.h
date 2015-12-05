//
//  c3dVector.h
//  HelloCpp
//
//  Created by wantnon (yang chao) on 14-1-1.
//
//

#ifndef __HelloCpp__c3dVector__
#define __HelloCpp__c3dVector__

#include <iostream>
#include <vector>
using namespace std;
#include "assert.h"
#include "c3dLog.h"
#include "c3dDefine.h"
#include "c3dLog.h"
#define cc3dv4(x,y,z,w) Cc3dVector4(x,y,z,w)
class Cc3dVector3;
class Cc3dVector4;
class Cc3dVector2 USED_FOR_SUBMIT_TO_VBO
{
public:
    Cc3dVector2(){
        initMembers();
    }
    Cc3dVector2(float a0,float a1);
    Cc3dVector2(const float array[2]);
    ~Cc3dVector2(){};
    const float* getArray()const{return m_array;}
    int getArrayLen()const{return 2;}
    vector<float> convertToVector()const;
    Cc3dVector2 operator +(const Cc3dVector2&right)const;
    Cc3dVector2 operator -(const Cc3dVector2&right)const;
    Cc3dVector2 operator *(float k)const;
    void init(const float array[2]);
    void init(float a0,float a1);
    float x()const {return m_array[0];}
    float y()const {return m_array[1];}
    void setx(float x) { m_array[0]=x;}
    void sety(float y) { m_array[1]=y;}
    void print()const;
    Cc3dVector3 toV3(float z)const;
    Cc3dVector4 toV4(float z,float w)const;
protected:
    void initMembers();
protected:
    float m_array[2];
    
};


class Cc3dVector4 USED_FOR_SUBMIT_TO_VBO
{
public:
    Cc3dVector4(){
        initMembers();
    }
    Cc3dVector4(float a0,float a1,float a2,float a3);
    Cc3dVector4(const float array[4]);
    ~Cc3dVector4(){};
    const float* getArray()const{return m_array;}
    float getAt(int index)const;
    int getArrayLen()const{return 4;}
    vector<float> convertToVector()const;
    Cc3dVector4 operator +(const Cc3dVector4&right)const;
    Cc3dVector4 operator -(const Cc3dVector4&right)const;
    Cc3dVector4 operator *(float k)const;
    void init(const float array[4]);
    void init(float a0,float a1,float a2,float a3);
    float x()const {return m_array[0];}
    float y()const {return m_array[1];}
    float z()const {return m_array[2];}
    float w()const {return m_array[3];}
    float r()const {return m_array[0];}
    float g()const {return m_array[1];}
    float b()const {return m_array[2];}
    float a()const {return m_array[3];}
    void setx(float x) { m_array[0]=x;}
    void sety(float y) { m_array[1]=y;}
    void setz(float z) { m_array[2]=z;}
    void setw(float w) { m_array[3]=w;}
    void setr(float r) { m_array[0]=r;}
    void setg(float g) { m_array[1]=g;}
    void setb(float b) { m_array[2]=b;}
    void seta(float a) { m_array[3]=a;}
    void print();
    Cc3dVector2 toV2()const;
    Cc3dVector3 toV3()const;
protected:
    void initMembers(){
        memset(&m_array, 0, sizeof(m_array));
    }
protected:
    float m_array[4];

};



class Cc3dVector3 USED_FOR_SUBMIT_TO_VBO
{
public:
    Cc3dVector3(){
        initMembers();
    }
    Cc3dVector3(float a0,float a1,float a2);
    Cc3dVector3(const float array[3]);
    ~Cc3dVector3(){};
    const float* getArray()const{return m_array;}
    float getAt(int index)const;
    int getArrayLen()const{return 3;}
    vector<float> convertToVector()const;
    Cc3dVector3 operator +(const Cc3dVector3&right)const;
    Cc3dVector3 operator -(const Cc3dVector3&right)const;
    Cc3dVector3 operator *(float k)const;
    void init(const float array[3]);
    void init(float a0,float a1,float a2);
    float x()const {return m_array[0];}
    float y()const {return m_array[1];}
    float z()const {return m_array[2];}
    float r()const {return m_array[0];}
    float g()const {return m_array[1];}
    float b()const {return m_array[2];}
    void setx(float x) { m_array[0]=x;}
    void sety(float y) { m_array[1]=y;}
    void setz(float z) { m_array[2]=z;}
    void setr(float r) { m_array[0]=r;}
    void setg(float g) { m_array[1]=g;}
    void setb(float b) { m_array[2]=b;}
    void print();
    Cc3dVector2 toV2()const;
    Cc3dVector4 toV4(float w)const;
protected:
    void initMembers(){
        memset(&m_array, 0, sizeof(m_array));
    }
protected:
    float m_array[3];
    
};
#endif /* defined(__HelloCpp__c3dVector__) */
