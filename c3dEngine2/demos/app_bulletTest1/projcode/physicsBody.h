//
//  physicsBody.h
//  c3d
//
//  Created by ck02-084 on 14-4-24.
//
//

#ifndef __c3d__physicsBody__
#define __c3d__physicsBody__

#include <iostream>
#include "c3d.h"
#include "btBulletDynamicsCommon.h"
class CphysicsBody:public Cc3dObject
{
public:
    CphysicsBody(){
        m_body=NULL;
        m_shape=NULL;
        m_hostPointer=NULL;
    }
    ~CphysicsBody(){
        if (m_body)
        {
            delete m_body->getMotionState();
            delete m_body;
        }
        if (m_shape){
            delete m_shape;
        }
    }
    void setHostPointer(void*pointer){m_hostPointer=pointer;}
    void* getHostPointer()const {return m_hostPointer;}
    btRigidBody*getBody(){return m_body;}
    bool initWithMeshData(float bodyMass,
              const vector<Cc3dVector4>&posList,const vector<Cc3dIDTriangle>&IDtriList,
              bool isConvex,
              Cc3dMatrix4 RTmat){
        //rotation
        btMatrix3x3 rotMatrix=btMatrix3x3(RTmat.getAt(0),RTmat.getAt(4),RTmat.getAt(8),//row0
                                          RTmat.getAt(1),RTmat.getAt(5),RTmat.getAt(9),//row1
                                          RTmat.getAt(2),RTmat.getAt(6),RTmat.getAt(10)//row2
                                          );//Constructor with row major formatting
        btScalar yaw,pitch,roll;
        rotMatrix.getEulerZYX(yaw, pitch, roll);
        btQuaternion rotation;
        rotation.setEulerZYX(yaw, pitch, roll);
        //position
        btVector3 position = btVector3(RTmat.getAt(12), RTmat.getAt(13), RTmat.getAt(14));
        //shape
        createShapeWithMeshData(posList,IDtriList, isConvex);
        //motionState
        btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));
        //body mass
        btScalar	mass(bodyMass);
        //body inertia
        btVector3 bodyInertia;
        m_shape->calculateLocalInertia(mass, bodyInertia);
        //bodyCI
        btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, m_shape, bodyInertia);
        //body
        m_body=new btRigidBody(bodyCI);
        //save this to body
        m_body->setUserPointer(this);
        //linear factor
        m_body->setLinearFactor(btVector3(1,1,1));//if want no action on z, use (1,1,0)
        return true;
    }
    void setPosition(const Cc3dVector4&pos){
        btTransform trans = m_body->getWorldTransform();
        trans.setOrigin(btVector3(pos.x(), pos.y(), pos.z()));
        m_body->setWorldTransform(trans);
    }
    void setRotation(const Cc3dMatrix4&Rmat){
        assert(Rmat.getAt(12)==0&&Rmat.getAt(13)==0&&Rmat.getAt(14)==0);
        //calculate rotation quaternion
        btMatrix3x3 rotMatrix=btMatrix3x3(Rmat.getAt(0),Rmat.getAt(4),Rmat.getAt(8),//row0
                                          Rmat.getAt(1),Rmat.getAt(5),Rmat.getAt(9),//row1
                                          Rmat.getAt(2),Rmat.getAt(6),Rmat.getAt(10)//row2
                                          );//Constructor with row major formatting
        btScalar yaw,pitch,roll;
        rotMatrix.getEulerZYX(yaw, pitch, roll);
        btQuaternion rotation;
        rotation.setEulerZYX(yaw, pitch, roll);
        //set rotation quaternion to body
        btTransform trans = m_body->getWorldTransform();
        trans.setRotation(rotation);
        m_body->setWorldTransform(trans);
    }
    Cc3dVector4 getPosition(){
        btTransform trans = m_body->getWorldTransform();
        Cc3dVector4 pos(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z(),1);
        return pos;
    }
    Cc3dMatrix4 getRotation(){
        btMatrix3x3 rotMatrix = btMatrix3x3(m_body->getWorldTransform().getRotation());
        btVector3 col0=rotMatrix.getColumn(0);
        btVector3 col1=rotMatrix.getColumn(1);
        btVector3 col2=rotMatrix.getColumn(2);
        Cc3dMatrix4 mat(col0.x(),col0.y(),col0.z(),0,
                        col1.x(),col1.y(),col1.z(),0,
                        col2.x(),col2.y(),col2.z(),0,
                        0,0,0,1);
        return mat;
    }


protected:
    void createShapeWithMeshData(const vector<Cc3dVector4>&posList,const vector<Cc3dIDTriangle>&IDtriList,bool isConvex){
        assert(m_shape==NULL);
        if (isConvex)
        {
            m_shape = new btConvexHullShape();
            int vertexCount=(int)posList.size();
            for (int i = 0; i < vertexCount; i++)
            {
                const Cc3dVector4&pos=posList[i];
                btVector3 btv = btVector3(pos.x(), pos.y(), pos.z());
                ((btConvexHullShape*)m_shape)->addPoint(btv);
            }
        }
        else
        {
            btTriangleMesh* mesh = new btTriangleMesh();
            int IDtriCount=(int)IDtriList.size();
            for (int i=0; i < IDtriCount; i++)
            {
                const Cc3dIDTriangle&IDtri=IDtriList[i];
                const Cc3dVector4&v0=posList[IDtri.vID[0]];
                const Cc3dVector4&v1=posList[IDtri.vID[1]];
                const Cc3dVector4&v2=posList[IDtri.vID[2]];
                
                btVector3 bv0 = btVector3(v0.x(), v0.y(), v0.z());
                btVector3 bv1 = btVector3(v1.x(), v1.y(), v1.z());
                btVector3 bv2 = btVector3(v2.x(), v2.y(), v2.z());
                
                mesh->addTriangle(bv0, bv1, bv2);
            }
            m_shape = new btBvhTriangleMeshShape(mesh, true);
        }
        
    }

protected:
    
    btRigidBody* m_body;
    btCollisionShape* m_shape;
    void*m_hostPointer;
};

#endif /* defined(__c3d__physicsBody__) */
