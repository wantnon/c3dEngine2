//
//  physicsActor.h
//  c3d
//
//  Created by ck02-084 on 14-4-23.
//
//

#ifndef __c3d__physicsActor__
#define __c3d__physicsActor__

#include <iostream>
using namespace std;
#include "physicsBody.h"
vector<Cc3dVector4> getPosListFromSubMeshData(const Cc3dSubMeshData&subMeshData);
vector<Cc3dIDTriangle> getIDtriListFromSubMeshData(const Cc3dSubMeshData&subMeshData);
Cc3dModel* makeBoxModel(float d,string texFilePath="");
Cc3dModel* makeBoxModel(float dx,float dy,float dz,string texFilePath="");
class CphysicsActor:public Cc3dActor
{
public:
    CphysicsActor(){
        m_body=NULL;
    }
    ~CphysicsActor(){
        if (m_body)m_body->release();
    }
    CphysicsBody*getBody()const {return m_body;}
    void setBody(CphysicsBody*body){
        if(m_body==NULL){
            m_body=body;
            m_body->retain();
        }else{
            m_body->release();
            m_body=body;
            m_body->retain();
        }
        body->setHostPointer(this);
    }
protected:
    CphysicsBody*m_body;
    
};
#endif /* defined(__c3d__physicsActor__) */
