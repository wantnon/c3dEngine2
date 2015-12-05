//
//  c3dGLMath.cpp
//  HelloCpp
//
//  Created by wantnon (yang chao) on 14-1-5.
//
//

#include "c3dGLMath.h"

Cc3dMatrix4 calculateViewMatrixInverse(const Cc3dVector4&_eyePos,const Cc3dVector4&_center,const Cc3dVector4&_up)
{//up no need to be unit vector
    assert(_eyePos.w()==1);
    assert(_center.w()==1);
    assert(_up.w()==0);
    Cc3dVector4 forward,right,up;
    forward=_center-_eyePos;
    up=_up;
    forward=normalize(forward);
    /* Side = forward x up */
    right=cross(forward, up);
    right=normalize(right);
    /* Recompute up as: up = right x forward */
    up=cross(right, forward);
    //calculate back (use back as Z axis)
    Cc3dVector4 back=Cc3dVector4(0,0,0,0)-forward;
    //the matrix transform point from camera space to world space is
    float m[16]={
        right.x(),right.y(),right.z(),0,//col1
        up.x(),up.y(),up.z(),0,
        back.x(),back.y(),back.z(),0,
        _eyePos.x(),_eyePos.y(),_eyePos.z(),1
    };
    Cc3dMatrix4 viewMatInverse(m);
    return viewMatInverse;
}

Cc3dMatrix4 calculateViewMatrix(const Cc3dVector4&eyePos,const Cc3dVector4&center,const Cc3dVector4&up)
{//up no need to be unit vector
    assert(eyePos.w()==1);
    assert(center.w()==1);
    assert(up.w()==0);
    Cc3dMatrix4 viewMatInverse=calculateViewMatrixInverse(eyePos, center, up);
    //inverse viewMatInverse to get viewMat
    //notice that the left up 3x3 sub matrix of viewMatInverse is orthogonal matrix,
    //so we have easy way to inverse it, see:
    //http://user.qzone.qq.com/350479720/blog/1329478296
    //calculate the fourth col of view mat
    const float *mInv=viewMatInverse.getArray();
    float x=-(mInv[12]*mInv[0]+mInv[13]*mInv[1]+mInv[14]*mInv[2]);
    float y=-(mInv[12]*mInv[4]+mInv[13]*mInv[5]+mInv[14]*mInv[6]);
    float z=-(mInv[12]*mInv[8]+mInv[13]*mInv[9]+mInv[14]*mInv[10]);
    //view mat
    float m[16]={
        mInv[0],mInv[4],mInv[8],0,//col 1
        mInv[1],mInv[5],mInv[9],0,//col 2
        mInv[2],mInv[6],mInv[10],0,//col 3
        x,y,z,1//col 4
    };
    Cc3dMatrix4 viewMat(m);
    return viewMat;
}
Cc3dMatrix4 calculateRotationMatrix(const Cc3dVector4&axis,float cosA,float sinA){
    //axis must be a unit vector
    assert(axis.w()==0);
    assert(getLength2(axis)==1);
    //
    float nx=axis.x();
    float ny=axis.y();
    float nz=axis.z();
    
    float _cosA=1-cosA;
    float nx_cosA=nx*_cosA;
    float nz_cosA=nz*_cosA;
    float nxnx_cosA=nx*nx_cosA;
    float nxny_cosA=ny*nx_cosA;
    float nxnz_cosA=nz*nx_cosA;
    float nyny_cosA=ny*ny*_cosA;
    float nynz_cosA=ny*nz_cosA;
    float nznz_cosA=nz*nz_cosA;
    float nxsinA=nx*sinA;
    float nzsinA=nz*sinA;
    float nysinA=ny*sinA;
    float rotMat[16]={
        nxnx_cosA+cosA,nxny_cosA+nzsinA,nxnz_cosA-nysinA,0,//col 1
        nxny_cosA-nzsinA,nyny_cosA+cosA,nynz_cosA+nxsinA,0,//col 2
        nxnz_cosA+nysinA,nynz_cosA-nxsinA,nznz_cosA+cosA,0,//col 3
        0,0,0,1//col 4
    };
    return Cc3dMatrix4(rotMat);
}
Cc3dVector4 extractScaleFromRTSmat(const Cc3dMatrix4&mat){
	float scaleX=getLength(Cc3dVector4(mat.getAt(0),mat.getAt(1),mat.getAt(2),mat.getAt(3)));
	float scaleY=getLength(Cc3dVector4(mat.getAt(4),mat.getAt(5),mat.getAt(6),mat.getAt(7)));
	float scaleZ=getLength(Cc3dVector4(mat.getAt(8),mat.getAt(9),mat.getAt(10),mat.getAt(11)));
	float scaleW=mat.getAt(15);
	return Cc3dVector4(scaleX,scaleY,scaleZ,scaleW);
	

}

Cc3dMatrix4 calculateTranslateMatrix(const Cc3dVector4&translate){
    assert(translate.w()==0);
    //1 0 0 dx
    //0 1 0 dy
    //0 0 1 dz
    //0 0 0  1
    float dx=translate.x();
    float dy=translate.y();
    float dz=translate.z();
    float TMat[16]={
        1,0,0,0,//col 1
        0,1,0,0,
        0,0,1,0,
        dx,dy,dz,1
    };
    return Cc3dMatrix4(TMat);
}
Cc3dMatrix4 calculateScaleMatrix(float kx,float ky,float kz,float kw){
    //1 0 0 0
    //0 1 0 0
    //0 0 1 0
    //0 0 0 1
    float SMat[16]={
        kx,0,0,0,//col 1
        0,ky,0,0,
        0,0,kz,0,
        0,0,0,kw
    };
    return Cc3dMatrix4(SMat);
}
Cc3dMatrix4 calculatePerspectiveProjectionMatrix(float fovy,float aspect,float zNear,float zFar)
//if calculation failed, return zero matrix
{
    double sine, cotangent, deltaZ;
    double radians = (fovy/2) * c3d_PI/180;
    deltaZ = zFar - zNear;
    sine = sinf(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
        return zeroMat();
    }
    cotangent = cosf(radians) / sine;
    float m[16]={
        cotangent / aspect,0,0,0,//col 1
        0,cotangent,0,0,//col 2
        0,0,-(zFar + zNear) / deltaZ,-1,//col 3
        0,0, -2 * zNear * zFar / deltaZ,0};//col 4
    return Cc3dMatrix4(m);
}
Cc3dMatrix4 calculateOrthoProjectionMatrix(float left,float right,float bottom,float top,float _near,float _far){
	//"near" and "far" have been defined in windows, so we use other var name 
    float tx=-((right+left)/(right-left));
    float ty=-((top+bottom)/(top-bottom));
	float tz=-((_far+_near)/(_far-_near));
    float m[16]={
        2.0/(right-left),0,0,0,//col 1
        0,2.0/(top-bottom),0,0,//col 2
        0,0,(-2.0)/(_far-_near),0,//col 3
        tx,ty,tz,1//col 4
    };
    return Cc3dMatrix4(m);

}

Cc3dVector4 convertFromViewportSpaceToWorldSpace(const Cc3dVector4&winPos,
                                                 const Cc3dMatrix4&projectionViewInverse,// inverse(projectionMat*viewMat)
                                                 const Cc3dRect&viewport){
    Cc3dVector4 t_winPos=winPos;
    /* Map x and y from window coordinates */
    t_winPos.setx((t_winPos.x()-viewport.getMinX())/viewport.getWidth());
    t_winPos.sety((t_winPos.y()-viewport.getMinY())/viewport.getHeight());
    /* Map to range -1 to 1 */
    //??????????��???????
    t_winPos.setx(t_winPos.x()*2-1);
    t_winPos.sety(t_winPos.y()*2-1);
    t_winPos.setz(t_winPos.z()*2-1);
    //???????????????
    Cc3dVector4 worldPos=projectionViewInverse*t_winPos;
    if (worldPos.w() == 0.0){
        return false;
    }
    //??????
    worldPos.setx(worldPos.x()/worldPos.w());
    worldPos.sety(worldPos.y()/worldPos.w());
    worldPos.setz(worldPos.z()/worldPos.w());
    worldPos.setw(1);
    return worldPos;
}

Cc3dMatrix4 calculateWorldToViewportMatrix(const Cc3dMatrix4&viewMat,
                                           const Cc3dMatrix4&projection,
                                           const Cc3dRect&viewport)
//???????????mat_worldToViewport??
//????????????????????????mat_worldToViewport??????????????
//?????????????????
{
    //????viewport???????��????
    float vx=viewport.getMinX();
    float vy=viewport.getMinY();
    float width=viewport.getWidth();
    float height=viewport.getHeight();
    Cc3dMatrix4 viewportMat(
                            width/2,0,0,0,//?????
                            0,height/2,0,0,//?????
                            0,0,0.5,0,//??????
                            vx+width/2,vy+height/2,0.5,1//??????
                            );
    //??projectionModelView
    Cc3dMatrix4 worldToViewportMat=viewportMat*projection*viewMat;
    return worldToViewportMat;
}
Cc3dMatrix4 calculateWorldToViewportTexCoordMatrix(const Cc3dMatrix4&viewMat,
                                                   const Cc3dMatrix4&projection)
//???????????mat_worldToViewportTexCoord??
//????????????????????????mat_worldToViewportTexCoord??????????????????
//?????????????????
{
    const Cc3dRect viewport(0,0,1,1);//?????????????????????????????????width??height???????????��?????
    return calculateWorldToViewportMatrix(viewMat,projection,viewport);
}

