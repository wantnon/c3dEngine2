//
//  c3dCamera.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-8.
//
//
#include <math.h>
#include "c3dCamera.h"
#include "c3dGeoMath.h"
//--------frustum
bool Cc3dFrustum::ballIsPotentiallyVisible(const Cc3dVector4&c,float R){//?��?????(c,R)??????????

    for(int i=0;i<6;i++){
        float PND=directedDistanceFromPointToPlane(planeList[i], c);
        if(PND<-R){//????????��????��????????????????????????
            return false;
        }
    }
    return true;
    
}
void Cc3dFrustum::updateFrustum(const Cc3dMatrix4&projectionMatrix,const Cc3dMatrix4&viewMatrix)
//?????????????????????????
{
    //??????????????viewport????????????????????????????????viewport??
    //?????��??????????viewport?????????????viewport??????????????????????viewport=Cc3dRect(0,1,0,1)???��?
    Cc3dRect viewport(0,1,0,1);
    //
    float left=viewport.getMinX();
    float down=viewport.getMinY();
    float right=left+viewport.getWidth();
    float up=down+viewport.getHeight();
    //?????
    //   p3(left,up,0)------p2(right,up,0)
    //      |                   |
    //      |                   |
    //   p0(left,down,0)----p1(right,down,0)
    //????
    //   q3(left,up,1)------q2(right,up,1)
    //      |                   |
    //      |                   |
    //   q0(left,down,1)----q1(right,down,1)
    Cc3dVector4 p[4];//??????????????????????
    p[0].init(left, down, 0, 1);
    p[1].init(right,down,0,1);
    p[2].init(right,up,0,1);
    p[3].init(left,up,0,1);
    Cc3dVector4 q[4];//?????????????????????
    q[0].init(left,down,1,1);
    q[1].init(right,down,1,1);
    q[2].init(right,up,1,1);
    q[3].init(left,up,1,1);
    //??invProjectionView
    Cc3dMatrix4 projectionInverse=inverse(projectionMatrix);
    Cc3dMatrix4 viewInverse=inverse(viewMatrix);
    Cc3dMatrix4 projectionViewInverse=inverse(Cc3dMatrix4(projectionMatrix)*Cc3dMatrix4(viewMatrix));//viewInverse*projectionInverse;
    //??p???????????
    for(int i=0;i<=3;i++){
        //??p[i]???????????
        p[i]=convertFromViewportSpaceToWorldSpace(p[i],projectionViewInverse,viewport);
    }//p??????????????
    //??q???????????
    for(int i=0;i<=3;i++){
        //??q[i]???????????
        q[i]=convertFromViewportSpaceToWorldSpace(q[i],projectionViewInverse,viewport);
    }//q??????????????
    //????????��???????????????
    //????????????
    Cc3dVector4 norm[6];
    norm[0]=calculateNormOfTri(p[2], p[1], p[0]);//????�Q??
    norm[1]=calculateNormOfTri(p[1], q[1], q[0]);
    norm[2]=calculateNormOfTri(p[1], q[2], q[1]);
    norm[3]=calculateNormOfTri(p[2], q[3], q[2]);
    norm[4]=calculateNormOfTri(p[0], q[0], q[3]);
    norm[5]=Cc3dVector4(0,0,0,0)-norm[0];//???�Q????????????
    //????????
    planeList[0].init(p[0],norm[0]);//?????
    planeList[1].init(p[1],norm[1]);
    planeList[2].init(p[1],norm[2]);
    planeList[3].init(p[2],norm[3]);
    planeList[4].init(p[0],norm[4]);
    planeList[5].init(q[0],norm[5]);//????
}
//--------camera
void Cc3dCamera::updateFrustum(){
    getFrustum().updateFrustum(calculateProjectionMat(), calculateViewMat());
}
Cc3dMatrix4 Cc3dCamera::calculateViewMat() {
    if(m_viewMatDirty){
        m_viewMat=calculateViewMatrix(m_eyePos, m_center, m_up);
        m_viewMatDirty=false;
    }
    return m_viewMat;
    
}
Cc3dMatrix4 Cc3dCamera::calculateViewMatInverse(){
    if(m_viewMatInverseDirty){
        m_viewMatInverse=calculateViewMatrixInverse(m_eyePos, m_center, m_up);
        m_viewMatInverseDirty=false;
    }
    return m_viewMatInverse;
    
}
Cc3dMatrix4 Cc3dCamera::calculateProjectionMat() {
    if(m_projMatDirty){
        switch (m_projectionMode) {
            case ec3dPerspectiveMode:
                m_projMat=::calculatePerspectiveProjectionMatrix(m_fovy, m_aspect, m_zNear, m_zFar);
                break;
            case ec3dOrthographicMode:
                m_projMat=::calculateOrthoProjectionMatrix(m_range.getMinX(), m_range.getMaxX(), m_range.getMinY(), m_range.getMaxY(), m_range.getMinZ(), m_range.getMaxZ());
                break;
            default:
                assert(false);
                break;
        }
        m_projMatDirty=false;
    }
    return m_projMat;
}
