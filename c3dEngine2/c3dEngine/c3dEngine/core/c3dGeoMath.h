//
//  c3dGeoMath.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-26.
//
//

#ifndef __HelloOpenGL__c3dGeoMath__
#define __HelloOpenGL__c3dGeoMath__

#include <iostream>
using namespace std;
#include "c3dMath.h"
#include "c3dPlane.h"
float directedDistanceFromPointToPlane(const Cc3dPlane&plane,const Cc3dVector4&point);
Cc3dVector4 calculateNormOfTri(const Cc3dVector4&v0,const Cc3dVector4&v1,const Cc3dVector4&v2);



#endif /* defined(__HelloOpenGL__c3dGeoMath__) */
