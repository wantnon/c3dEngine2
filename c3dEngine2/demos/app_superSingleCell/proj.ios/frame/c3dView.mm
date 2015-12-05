#import "c3dView.h"
//
#include <iostream>
using namespace std;

#include "c3d.h"
//
#include "mainWindowFuncs.h"
//
#include "initGame.h"



@implementation Cc3dView

+ (Class)layerClass {
    return [CAEAGLLayer class];
}
- (void)setupLayer {
    _eaglLayer = (CAEAGLLayer*) self.layer;
    _eaglLayer.opaque = YES;
}

- (void)setupContext {   
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    _context = [[EAGLContext alloc] initWithAPI:api];
    if (!_context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context");
        assert(false);
    }
    
    if (![EAGLContext setCurrentContext:_context]) {
        NSLog(@"Failed to set current OpenGL context");
        assert(false);
    }
}

- (void)setupDisplayLink {
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)setupRenderBuffer {
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
    /*Note: When the Core Animation layer’s bounds or properties change, 
     your application should reallocate the renderbuffer’s storage.
     If you do not reallocate the renderbuffers,
     the renderbuffer size won’t match the size of the view;
     in this case, Core Animation may scale the image’s contents to fit in the view. 
     To prevent this, the Xcode template reallocates the framebuffer and renderbuffer whenever the view layout changes.
     http://developer.apple.com/library/ios/#documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/WorkingwithEAGLContexts/WorkingwithEAGLContexts.html
     */
}

- (void)setupDepthBuffer {
    glGenRenderbuffers(1, &_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16 /*24_OES*/, self.frame.size.width, self.frame.size.height);
}

- (void)setupFrameBuffer {
    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
    //Test the framebuffer for completeness. This test only needs to be performed when the framebuffer's configuration changes.
    GLenum status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status!=GL_FRAMEBUFFER_COMPLETE){
        cout<<"failed to make complete framebuffer object!"<<endl;
        exit(0);
    }

}



- (void)render:(CADisplayLink*)displayLink {
    render(_context,_frameBuffer,_colorRenderBuffer);
}


- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    assert(self);
    [self setupLayer];
    [self setupContext];
    [self setupDepthBuffer];
    [self setupRenderBuffer];
    [self setupFrameBuffer];
    [self setupDisplayLink];//其中定义了render回调
    
    initWithFrame();
    //initGame
    initGame();
    
    return self;
}
-(BOOL) CheckForExtension:(NSString *)searchName
{
    // For performance, the array can be created once and cached.
    NSString *extensionsString = [NSString stringWithCString:(const char*)glGetString(GL_EXTENSIONS) encoding: NSASCIIStringEncoding];
    NSArray *extensionsNames = [extensionsString componentsSeparatedByString:@" "];
    return [extensionsNames containsObject: searchName];
}
- (void)dealloc
{
    teardownOpenAL();
    [_context release];
    _context = nil;
    [super dealloc];
}
@end