//
//  c3dGeoMath.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-26.
//
//

#include "c3dGeoMath.h"
float directedDistanceFromPointToPlane(const Cc3dPlane&plane,const Cc3dVector4&point){
    Cc3dVector4 planePointToPoint=point-plane.getPoint();
    return dot(planePointToPoint, plane.getNorm());
}
Cc3dVector4 calculateNormOfTri(const Cc3dVector4&v0,const Cc3dVector4&v1,const Cc3dVector4&v2){
    Cc3dVector4 v01=v1-v0;
    Cc3dVector4 v02=v2-v0;
    Cc3dVector4 norm=cross(v01, v02);
    norm=normalize(norm);
    return norm;
}