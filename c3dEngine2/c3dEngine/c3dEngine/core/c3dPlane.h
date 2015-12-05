//
//  c3dPlane.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-2.
//
//

#ifndef __HelloOpenGL__c3dPlane__
#define __HelloOpenGL__c3dPlane__

#include <iostream>
using namespace std;
#include "c3dVector.h"
class Cc3dPlane//用点法式描述--abc
{
protected:
	Cc3dVector4 m_point;
	Cc3dVector4 m_norm;
public:
	void init(const Cc3dVector4 &point,const Cc3dVector4&norm)
	//由点法构造平面,norm须确保为单位向量--abc
	{
        assert(point.w()==1);
        assert(norm.w()==0);
        m_point=point;
        m_norm=norm;
	}
    Cc3dVector4 getPoint()const{return m_point;}
    Cc3dVector4 getNorm()const{return m_norm;}
    
};


#endif /* defined(__HelloOpenGL__c3dPlane__) */
