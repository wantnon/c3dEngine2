//
//  c3dMaterial.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-25.
//
//

#ifndef __HelloOpenGL__c3dMaterial__
#define __HelloOpenGL__c3dMaterial__

#include <iostream>
using namespace std;
#include "c3dObject.h"
#include "c3dVector.h"
class Cc3dMaterial:public Cc3dObject
{
protected:
    Cc3dVector4 m_diffuse;
    Cc3dVector4 m_ambient;
    Cc3dVector4 m_specular;
    Cc3dVector4 m_color;//only used by 2d element
    float m_shininess;
    bool m_isHighlightUntransp;//whether highlight is untransparent
public:
    Cc3dMaterial(){
        m_isHighlightUntransp=false;
        m_diffuse.init(1,1,1,1);
        m_ambient.init(1, 1, 1, 1);
        m_specular.init(0,0,0,1);
        m_color.init(1, 1, 1, 1);
        m_shininess=0;
        init_dft();
    }
    void init_dft(){};
    Cc3dVector4 getColor()const{return m_color;}
    Cc3dVector4 getSpecular()const{return m_specular;}
    float getShininess()const{return m_shininess;}
    Cc3dVector4 getDiffuse()const{return m_diffuse;}
    Cc3dVector4 getAmbient()const{return m_ambient;}
    void setIsHighlightUntransp(bool isHighlightUntransp){
        m_isHighlightUntransp=isHighlightUntransp;
    }
    void setSpecular(float r,float g,float b){
        m_specular.setr(r);
        m_specular.setg(g);
        m_specular.setb(b);
    }
    void setShininess(float shininess){
        m_shininess=shininess;
    }
    void setDiffuseRGB(float r,float g,float b){
        m_diffuse.setr(r);
        m_diffuse.setg(g);
        m_diffuse.setb(b);

    }
    void setDiffuseAlpha(float alpha){
        m_diffuse.seta(alpha);
    }
    void setAmbient(float r,float g,float b){
        m_ambient.setr(r);
        m_ambient.setg(g);
        m_ambient.setb(b);
    }
    void setColor(float r,float g,float b,float a){
        m_color.init(r, g, b, a);
    }
	void setSpecular(const Cc3dVector4&specular){m_specular=specular;}
	void setDiffuse(const Cc3dVector4&diffuse){m_diffuse=diffuse;}
	void setAmbient(const Cc3dVector4&ambient){m_ambient=ambient;}
	void setColor(const Cc3dVector4&color){m_color=color;}
};

#endif /* defined(__HelloOpenGL__c3dMaterial__) */
