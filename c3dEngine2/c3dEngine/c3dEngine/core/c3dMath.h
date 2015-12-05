//
//  c3dMath.h
//  HelloCpp
//
//  Created by wantnon (yang chao) on 14-1-4.
//
//

#ifndef __HelloCpp__c3dMath__
#define __HelloCpp__c3dMath__

#include <iostream>
using namespace std;
#include "c3dMatrix.h"
#include "c3dVector.h"
#include <math.h>
#define square(x) ((x)*(x)) //must with the brackets
float maxf(float a,float b);
float minf(float a,float b);
float isEqual(float a,float b,float eps=0);
Cc3dMatrix4 unitMat();
Cc3dMatrix4 zeroMat();
Cc3dVector4 normalize(const Cc3dVector4&v);
Cc3dVector4 cross(const Cc3dVector4&v1,const Cc3dVector4&v2);
Cc3dVector4 componentProduct(const Cc3dVector4&v1,const Cc3dVector4&v2);
float dot(const Cc3dVector4&v1,const Cc3dVector4&v2);
bool isUnitMat(const Cc3dMatrix4&mat);
bool isEqual(const Cc3dMatrix4&mat1,const Cc3dMatrix4&mat2,float eps=0);
bool isEqual(const Cc3dVector4&v1,const Cc3dVector4&v2,float eps=0);
Cc3dMatrix4 inverse(const Cc3dMatrix4&mat);
Cc3dMatrix4 transpose(const Cc3dMatrix4&mat);
float getLength2(const Cc3dVector4&v);
float getLength(const Cc3dVector4&v);
Cc3dMatrix4 orthogonalization3x3(const Cc3dMatrix4&mat);
bool isPowOfTwo(int n);
Cc3dVector4 slerp(const Cc3dVector4&v1,const Cc3dVector4&v2,float t);
#endif /* defined(__HelloCpp__c3dMath__) */
