//
//  c3dGLMath.h
//  HelloCpp
//
//  Created by wantnon (yang chao) on 14-1-5.
//
//

#ifndef __HelloCpp__c3dGLMath__
#define __HelloCpp__c3dGLMath__

#include <iostream>
using namespace std;
#include "c3dMath.h"
#include "c3dRect.h"
//OpenGL transform pipline: http://user.qzone.qq.com/350479720/blog/1310060277
//Basic 3D Math: Matrices: http://www.matrix44.net/cms/notes/opengl-3d-graphics/basic-3d-math-matrices
//ref to kmMat4PerspectiveProjection and kmMat4OrthographicProjection
Cc3dMatrix4 calculateViewMatrixInverse(const Cc3dVector4&eyePos,const Cc3dVector4&center,const Cc3dVector4&up);
Cc3dMatrix4 calculateViewMatrix(const Cc3dVector4&eyePos,const Cc3dVector4&center,const Cc3dVector4&up);
Cc3dMatrix4 calculateRotationMatrix(const Cc3dVector4&axis,float cosA,float sinA);
Cc3dMatrix4 calculateTranslateMatrix(const Cc3dVector4&translate);
Cc3dVector4 extractScaleFromRTSmat(const Cc3dMatrix4&mat);
Cc3dMatrix4 calculateScaleMatrix(float kx,float ky,float kz,float kw=1);
Cc3dMatrix4 calculatePerspectiveProjectionMatrix(float fovy,float aspect,float zNear,float zFar);
Cc3dMatrix4 calculateOrthoProjectionMatrix(float left,float right,float bottom,float top,float near,float far);
Cc3dVector4 convertFromViewportSpaceToWorldSpace(const Cc3dVector4&winPos,const Cc3dMatrix4&projectionViewInverse,const Cc3dRect&viewport);
Cc3dMatrix4 calculateWorldToViewportMatrix(const Cc3dMatrix4&viewMat,const Cc3dMatrix4&projection,const Cc3dRect&viewport);
Cc3dMatrix4 calculateWorldToViewportTexCoordMatrix(const Cc3dMatrix4&viewMat,const Cc3dMatrix4&projection);

#endif /* defined(__HelloCpp__c3dGLMath__) */
