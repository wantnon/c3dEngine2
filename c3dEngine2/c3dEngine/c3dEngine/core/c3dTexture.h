//
//  c3dTexture.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#ifndef __HelloOpenGL__c3dTexture__
#define __HelloOpenGL__c3dTexture__

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "common/c3dGL.h"
#include "core/c3dObject.h"
#include "core/c3dGLdebug.h"
#include "core/c3dAssert.h"

class Cc3dTexture:public Cc3dObject
{
protected:
    GLuint m_texture;
    float m_width,m_height;
    string m_filePath;//for the textures not load from file (created by program), m_filePath is ""
	bool m_isCubeTexture;
public:
    Cc3dTexture(){
        m_texture=0;
        m_width=m_height=0;
		m_isCubeTexture=false;
    }
    ~Cc3dTexture(){
        if(m_texture!=0){
            glDeleteTextures(1, &m_texture);
        }
    }
	bool getIsCubeTexture()const{return m_isCubeTexture;}
    bool getIsInvalid()const {
        return m_texture==0;
    }
    
    GLuint getID()const {
        return m_texture;
    }
    float getWidth()const {
        return m_width;
    }
    float getHeight()const {
        return m_height;
    }
    string getFilePath()const{
        return m_filePath;
    }
    bool init( const string&filePath,int wrapS,int wrapT,GLint minFilter,GLint magFilter,bool genMipmap);
    bool initDepthTexture(int width,int height,int wrapS,int wrapT,GLint minFilter,GLint magFilter);
    bool initColorTexture(int width,int height,int wrapS,int wrapT,GLint minFilter,GLint magFilter);
	bool initCubeTexture(const vector<string>&filePathList,int wrapS=GL_CLAMP_TO_EDGE,int wrapT=GL_CLAMP_TO_EDGE,int wrapR=GL_CLAMP_TO_EDGE,GLint minFilter=GL_LINEAR_MIPMAP_LINEAR,GLint magFilter=GL_LINEAR,bool isGenMipmap=true);
};

#endif /* defined(__HelloOpenGL__c3dTexture__) */
