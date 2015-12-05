//
//  mainWindowFuncs.cpp
//  c3d
//
//  Created by apple on 14-5-3.
//
//

#include "mainWindowFuncs.h"
void initWithFrame(){
    c3dInitGame();
}
void render(void*_context, GLuint frameBuffer,GLuint colorRenderBuffer){
    EAGLContext* context=(EAGLContext*)_context;
    //switch back to screen fbo
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);//tile-base architecture要求bind之后立即clear更快些
    glViewport(Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getMinX(),
               Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getMinY(),
               Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getWidth() ,
               Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenRect().getHeight());//屏幕窗口
    
    if(Cc3dAppState::sharedAppState()->getIsInBackGround())return;
    
    
    //remove nodes marked as isRemoveOnNextFrame
    Cc3dSceneManager::sharedSceneManager()->performDelayRemove();
    //remove unused resource in caches (must after performDelayRemove)
    Cc3dTextureCache::sharedTextureCache()->performDelayRemoveUnusedTextures();
    Cc3dAudioCache::sharedAudioCache()->performDelayRemoveUnusedBuffersAndSources();
    //visitUpdate
    Cc3dSceneManager::sharedSceneManager()->getRoot()->visitUpdate();
    //visitDraw
    Cc3dSceneManager::sharedSceneManager()->getRoot()->visitDraw();
    //autorelease
    Cc3dAutoreleasePool::sharedAutoreleasePool()->releaseAll();
    //refresh
    //[self discardDepthBuffer];
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER];
    
    C3DCHECK_GL_ERROR_DEBUG() ;
    
    C3DCHECK_AL_ERROR_DEBUG();
    
    Cc3dFrameCounter::sharedFrameCounter()->doCount();
}
