//
//  c3dDeviceAndOSInfoPlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-6.
//
//

#include "platform/win32/c3dDeviceAndOSInfoPlatformDepend.h"
#include "core/c3dAssert.h"
Cc3dVector2 getScreenSize_plat(){

	return Cc3dVector2(640*0.5,1136*0.5);
}
Cc3dVector2 getResolutionSize_plat(){

	//由于iOS上凡640x1136分辨率者只有高清,所以这里只能假装是高清以便能够使用资源--abc
	return Cc3dVector2(640,1136);
}
void getMemStatistics_plat(int&memUsed,int&memFree,int&memTotal){//获得内存统计数据--abc
 
	//no implemented yet

	C3DASSERT(false);
}

void showSystemInfo_plat(){
	//opengl version query
	//see: http://www.opengl.org/wiki/GlGetString
	const GLubyte* s_gl_vendor = glGetString(GL_VENDOR); 
	const GLubyte* s_gl_renderer = glGetString(GL_RENDERER);   
	const GLubyte* s_gl_version =glGetString(GL_VERSION);
	const GLubyte* s_glu_version= gluGetString(GLU_VERSION);
	cout<<endl;
	cout<<"OpenGL vendor: "<<s_gl_vendor<<endl; 
	cout<<"GPU:"<<s_gl_renderer<<endl;
	cout<<"OpenGL version: "<<s_gl_version<<endl;
	cout<<"GLU version: "<<s_glu_version<<endl; 
	//see if support opengl 2.0
	if (glewIsSupported("GL_VERSION_2_0"))
		cout<<"support openGL 2.0!"<<endl;
	else {
		cout<<"not support openGL 2.0!"<<endl;
		assert(false);
	}
	//GLSL version
    const GLubyte* s= glGetString(GL_SHADING_LANGUAGE_VERSION);
    cout<<s<<endl;
	cout<<endl;
}