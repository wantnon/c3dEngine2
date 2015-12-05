//
//  OpenGLView.h
//  HelloOpenGL
//
//  Created by Ray Wenderlich on 5/24/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#include <OpenGLES/ES2/glext.h>
#import <OpenAL/alc.h>  
@interface Cc3dView :UIView
{
    CAEAGLLayer* _eaglLayer;
    EAGLContext* _context;
  
    //帧句柄
    GLuint _frameBuffer;
    GLuint _colorRenderBuffer;
    GLuint _depthRenderBuffer;

   
}

@end
