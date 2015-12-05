#import "c3dViewController.h"
//
#include <iostream>
using namespace std;
#include "c3dTextureCache.h"
#include "c3dAudioCache.h"
#include "c3dCamera.h"
#include "c3dModel.h"
#include "c3dGlobalTimer.h"
#include "c3dSceneManger.h"


@implementation Cc3dViewController
@synthesize accelerometer;
//创建view的代码可以写在loadView中也可以写在viewDidLoad中
//在使用interface builder的情况下loadView不被调用，只调用viewDidLoad
//在不使用interface builder的情况下先调用loadView再调用viewDidLoad
//在不使用interface builder的情况下loadView必须实现，即使没有内容，也必须写出来，否则viewController无法构造成功
//可以将创建view的代码写在loadView，将设置view的代码写在viewDidLoad，也可以全写在一个里。
-(void)loadView
{
    
}
-(void)viewDidLoad
{
    [super viewDidLoad];
    //创建
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    Cc3dView* glView = [[[Cc3dView alloc] initWithFrame:screenBounds] autorelease];
    self.view=glView;
    //设置多点触摸
    [self.view setMultipleTouchEnabled:(BOOL)true];
    //重力感应
    self.accelerometer = [UIAccelerometer sharedAccelerometer];
    self.accelerometer.updateInterval = 1.0f/20.0f;//每秒读取20次
    self.accelerometer.delegate = self;
    
 /*   //target - what object is going to handle the gesture when it gets recognised
    //the argument for tap: is the gesture that caused this message to be sent
   UITapGestureRecognizer* tapOnce = [[[UITapGestureRecognizer alloc] initWithTarget:self  action:@selector(tapOnce:)] autorelease];
   UITapGestureRecognizer* tapTwice =[[[UITapGestureRecognizer alloc] initWithTarget:self  action:@selector(tapTwice:)] autorelease];
    
    tapOnce.numberOfTapsRequired = 1;
    tapTwice.numberOfTapsRequired = 2;
    
    //stops tapOnce from overriding tapTwice
    [tapOnce requireGestureRecognizerToFail:tapTwice];
    
    //then need to add the gesture recogniser to a view - this will be the view that recognises the gesture
    [self.view addGestureRecognizer:tapOnce];
    [self.view addGestureRecognizer:tapTwice];
*/
  }

- (void)dealloc
{

    [super dealloc];
}

//----处理低内存警告
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    cout<<"低内存警告"<<endl;
    // Add code to clean up any of your own resources that are no longer necessary.
    Cc3dAudioCache::sharedAudioCache()->removeUnusedBuffersAndSources();
    Cc3dTextureCache::sharedTextureCache()->removeUnusedTextures();
    
}
/*
 //----实现重力感应
- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
    if(supportAccel==false)return;
    if(useAccel){
        myAccel.pushAccel((float)acceleration.x,(float)acceleration.y,(float)acceleration.z);
    }

}
*/


// Handles the start of a touch
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
    // Enumerate through all the touch objects.
    vector<Cc3dVector2> points;
	for (UITouch *touch in touches) {
        CGPoint touchPoint=[touch locationInView:self.view] ;
        //     NSLog(@"touchesBegan:(%f,%f)",touchPoint.x,touchPoint.y);
        points.push_back(Cc3dVector2(touchPoint.x,touchPoint.y));
	}
    
    Cc3dSceneManager::sharedSceneManager()->getRoot()->visitTouchesBegan(points);
    
}
// Handles the continuation of a touch.
-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Enumerates through all touch objects
    vector<Cc3dVector2> points;
	for (UITouch *touch in touches) {
		CGPoint touchPoint=[touch locationInView:self.view] ;
         //       NSLog(@"touchesMoved:(%f,%f)",touchPoint.x,touchPoint.y);
        
        points.push_back(Cc3dVector2(touchPoint.x,touchPoint.y));
	}
    
    Cc3dSceneManager::sharedSceneManager()->getRoot()->visitTouchesMoved(points);
    
    
    
}
// Handles the end of a touch event.
-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Enumerates through all touch object
    vector<Cc3dVector2> points;
	for (UITouch *touch in touches) {
		CGPoint touchPoint=[touch locationInView:self.view] ;
         //  NSLog(@"touchesEnded:(%f,%f)",touchPoint.x,touchPoint.y);
        points.push_back(Cc3dVector2(touchPoint.x,touchPoint.y));
	}
    Cc3dSceneManager::sharedSceneManager()->getRoot()->visitTouchesEnded(points);
    
}

/*
 //-------------------实现landscape
 // -------------------------------------------------------------------------------
 //  supportedInterfaceOrientations
 //  Support either landscape orientation (iOS 6).
 // -------------------------------------------------------------------------------
 - (NSUInteger)supportedInterfaceOrientations
 {
 return UIInterfaceOrientationMaskLandscape;
 }
 
 // -------------------------------------------------------------------------------
 //  shouldAutorotateToInterfaceOrientation
 //  Support either landscape orientation (IOS 5 and below).
 // -------------------------------------------------------------------------------
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
 {
 return UIInterfaceOrientationIsLandscape(interfaceOrientation);
 }
 */
//------------------------------------------------------------------------
/*- (void)tapOnce:(UIGestureRecognizer *)gesture
 {
 NSLog(@"tapOnce");
 }
 - (void)tapTwice:(UIGestureRecognizer *)gesture
 {
 NSLog(@"tapTwice");
 }*/
//------------------------------------------------------------------------
@end
