//
//  c3dTexture.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#include "core/c3dTexture.h"
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
#include "platform/win32/c3dTexturePlatformDepend.h"
#else
#include "platform/iOS/c3dTexturePlatformDepend.h"
#endif
bool Cc3dTexture::initDepthTexture(int width,int height,int wrapS,int wrapT,GLint minFilter,GLint magFilter)
//note, if used as target of render to texture, then wrapS and wrapT should be GL_CLAMP_TO_EDGE
{
    assert(m_texture==0);
    m_width=width;
    m_height=height;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    //GL_NEAREST,GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    //
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrapS);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrapT);
    //note, for depth texture
    //            1, use GL_DEPTH_COMPONENT, not GL_DEPTH_COMPONENT16
    //            2, use GL_UNSIGNED_INT, not GL_UNSIGNED_BYTE
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,m_width, m_height, 0,
                 GL_DEPTH_COMPONENT,GL_UNSIGNED_INT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    C3DCHECK_GL_ERROR_DEBUG();
    return true;
}

bool Cc3dTexture::initColorTexture(int width,int height,int wrapS,int wrapT,GLint minFilter,GLint magFilter)
//note, if used as target of render to texture, then wrapS and wrapT should be GL_CLAMP_TO_EDGE
{
    assert(m_texture==0);
    m_width=width;
    m_height=height;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    //GL_NEAREST或GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    C3DCHECK_GL_ERROR_DEBUG();
    return true;
}

bool Cc3dTexture::init(const string&filePath,int wrapS,int wrapT,GLint minFilter,GLint magFilter)
{
    GLuint texture;
	
    float width,height;
    texture=createGLTexture_plat(filePath,wrapS,wrapT,minFilter,magFilter,width,height);
	//cout<<"texture ID:"<<texture<<endl;
    //
    m_texture=texture;
    m_width=width;
    m_height=height;
    m_filePath=filePath;
    
    return true;

}
bool Cc3dTexture::initCubeTexture(const vector<string>&filePathList,int wrapS,int wrapT,int wrapR,GLint minFilter,GLint magFilter,bool isGenMipmap){
	m_isCubeTexture=true;
	//get the images' data
	vector<unsigned char*> imageDataList;
	vector<CCTexture2DPixelFormat> pixelFormatList;
	vector<float> widthList;
	vector<float> heightList;
	int nPath=(int)filePathList.size();
	for(int i=0;i<nPath;i++){
		string filePath=filePathList[i];
		CCTexture2DPixelFormat pixelFormat;
		float width,height;
		unsigned char* imageData=getImageData_plat(filePath,pixelFormat,width,height);
		imageDataList.push_back(imageData);
		pixelFormatList.push_back(pixelFormat);
		widthList.push_back(width);
		heightList.push_back(height);
	}
	for(int i=0;i<nPath-1;i++){
		C3DASSERT(widthList[i]==widthList[i+1]);
		C3DASSERT(heightList[i]==heightList[i+1]);
	}
	//create cubemap

	GLuint textureObject;
    glGenTextures(1, &textureObject);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureObject);


	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
    for (int f = 0; f < 6; ++f) {
        GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + f;
		unsigned char* imageData=imageDataList[f];
		CCTexture2DPixelFormat pixelFormat=pixelFormatList[f];
		float width=widthList[f];
		float height=heightList[f];
		if(pixelFormat==kCCTexture2DPixelFormat_RGB888){
		
			glTexImage2D(face, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		
		}else if(pixelFormat==kCCTexture2DPixelFormat_RGBA8888){
		
			glTexImage2D(face, 0, GL_RGBA,(GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		
		}else{
			C3DASSERT(false);
		}
    }
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,  wrapS );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,  wrapT );
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
    //there is no GL_TEXTURE_WRAP_R defined on mac opengles
    //http://www.gamedev.net/topic/520540-gl_texture_cube_map/
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,  wrapR );
#endif
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
	if(isGenMipmap){
	   glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    C3DCHECK_GL_ERROR_DEBUG();

	int nImageData=(int)imageDataList.size();
	for(int i=0;i<nImageData;i++){
		delete []imageDataList[i];
	}


	m_texture=textureObject;
    m_width=widthList[0];
    m_height=heightList[0];
    m_filePath="";
	return true;

}