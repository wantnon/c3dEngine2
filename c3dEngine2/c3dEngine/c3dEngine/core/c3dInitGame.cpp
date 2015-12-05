//
//  c3dInitGame.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#include "core/c3dInitGame.h"
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
#include "platform/win32/c3dALPlatformDepend.h"
#include "platform/win32/c3dGLPlatformDepend.h"
#else
#include "platform/iOS/c3dALPlatformDepend.h"
#include "platform/iOS/c3dGLPlatformDepend.h"
#endif
//
void initOpenGL(){
	initOpenGL_plat();
}
void initOpenAL(){
	initOpenAL_plat();
}
void teardownOpenAL(){
	teardownOpenAL_plat();
}
void c3dInitGame(){
	//----start global timer
	Cc3dGlobalTimer::sharedGlobalTimer()->start();
	//----随机数--abc
    srand(time(0));
	//----初始化openal
	initOpenAL();
	//----初始化opengl
	initOpenGL();
	//----openal状态--abc
	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);//设置距离模型--abc
    alListenerf(AL_GAIN, 1.0);//set listener gain
    //----opengl状态--abc
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
	glEnable(GL_TEXTURE_2D);//on windows, do not forget this!!!
	glEnable(GL_TEXTURE_CUBE_MAP);
#endif
    ////glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);// Enable point size by default on windows.(wait, it is not support on iOS!)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);//指定清理color buffers时所用的颜色,默认值为(0,0,0,0),见:http://msdn.microsoft.com/en-us/library/windows/desktop/dd318377(v=vs.85).aspx
    glEnableVertexAttribArray(ATTRIB_LOC_position);
    glEnableVertexAttribArray(ATTRIB_LOC_texCoord);
    glEnableVertexAttribArray(ATTRIB_LOC_normal);
	glEnableVertexAttribArray(ATTRIB_LOC_color);
    glEnableVertexAttribArray(ATTRIB_LOC_texCoord2);
	//----show system info
    Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->showSystemInfo();
	//----获取设备信息--abc
    cout<<"screenSize: "<<Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().x()<<" "<<Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y()<<endl;
    cout<<"resolution: "<<Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().x()<<" "<<Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getResolutionSize().y()<<endl;
    //----创建默认shaderProgam
    buildinProgramSetUp();
   

}
