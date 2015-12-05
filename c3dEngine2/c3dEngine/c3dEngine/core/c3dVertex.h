//
//  vertex.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-25.
//
//

#ifndef __HelloOpenGL__vertex__
#define __HelloOpenGL__vertex__

#include <iostream>
using namespace std;
#include "c3dDefine.h"
#include "c3dVector.h"
class Cc3dVertex USED_FOR_SUBMIT_TO_VBO
{
protected:
    Cc3dVector4 m_pos;
    Cc3dVector2 m_texCoord;
    Cc3dVector4 m_norm;
	Cc3dVector4 m_color;
    Cc3dVector2 m_texCoord2;
public:
    Cc3dVertex(){
		initMembers();
	};
    ~Cc3dVertex(){};//used for submit to vbo, so destructor must not be virtual
    Cc3dVertex(const Cc3dVector4&pos,const Cc3dVector2&texCoord,const Cc3dVector4&norm,const Cc3dVector4&color=Cc3dVector4(1,1,1,1),const Cc3dVector2&texCoord2=Cc3dVector2(0,0)){
        initMembers();
		init(pos,texCoord,norm,color,texCoord2);
    }
	void init(const Cc3dVector4&pos,const Cc3dVector2&texCoord,const Cc3dVector4&norm,const Cc3dVector4&color=Cc3dVector4(1,1,1,1),const Cc3dVector2&texCoord2=Cc3dVector2(0,0)){
		m_pos=pos;
		m_texCoord=texCoord;
		m_norm=norm;
		m_color=color;
		m_texCoord2=texCoord2;
	}
    Cc3dVector4 getPos()const {
        return m_pos;
    }
    Cc3dVector4 getNorm()const {
        return m_norm;
    }
	Cc3dVector4 getColor()const{
		return m_color;
	}
    Cc3dVector2 getTexCoord()const {
        return m_texCoord;
    }
    void setPos(const Cc3dVector4&pos){
        m_pos=pos;

    }

    void setTexCoord(const Cc3dVector2&texCoord){
        m_texCoord=texCoord;
    }
    void setNorm(const Cc3dVector4&norm){
        m_norm=norm;
    }
	void setColor(const Cc3dVector4&color){
		m_color=color;
	}
    void setTexCoord2(const Cc3dVector2&texCoord2){
        m_texCoord2=texCoord2;
    }
protected:
	void initMembers(){
		m_color.init(1,1,1,1);
	}
    
};


#endif /* defined(__HelloOpenGL__vertex__) */
