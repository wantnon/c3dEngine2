//
//  fbo.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-6.
//
//

#include "c3dFbo.h"
void Cc3dFbo::attachDepthTex(Cc3dTexture*depthTexture){
    assert(depthTexture);
    assert(m_depthTex==NULL);
    m_depthTex=depthTexture;
    m_depthTex->retain();
    GLuint depthTex=m_depthTex->getID();
    GLenum FBOstatus;
    GLint oldFbo=0;//???fbo?????????
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    //?????glDrawBuffer(GL_NONE)??glReadBuffer(GL_NONE)
    // attach the texture to FBO depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depthTex, 0);
	
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32) 
	//为了使glCheckFramebufferStatusEXT检验通过,当没有颜色纹理时要写下面两句--abc
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
#else

#endif
	
    // check FBO status
    FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(FBOstatus != GL_FRAMEBUFFER_COMPLETE){
        cout<<"error: GL_FRAMEBUFFER_COMPLETE failed!";
     /* switch(FBOstatus){
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                cout<<0<<endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                cout<<1<<endl;
                break ;
            case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                cout<<2<<endl;
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                cout<<3<<endl;
                break;
                
        }*/
        assert(false);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
}
void Cc3dFbo::attachColorTex(Cc3dTexture*colorTexture){
    assert(colorTexture);
    assert(m_colorTex==NULL);
    m_colorTex=colorTexture;
    m_colorTex->retain();
    GLuint colorTex=m_colorTex->getID();
    GLenum FBOstatus;
    GLint oldFbo=0;//???fbo?????????
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    //?????glDrawBuffer(GL_NONE)??glReadBuffer(GL_NONE)
    // attach the texture to FBO depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, colorTex, 0);//????????colorTex????????????bind(colorTex)
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
	//为了使glCheckFramebufferStatusEXT检验通过,当没有颜色纹理时要写下面两句--abc
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
#endif
	//opengl(es)?????????????????????????????
    //??????bind?????????????????????????????????bind
    // check FBO status
    FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(FBOstatus != GL_FRAMEBUFFER_COMPLETE){
        cout<<"error: GL_FRAMEBUFFER_COMPLETE failed!";
        assert(false);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
}