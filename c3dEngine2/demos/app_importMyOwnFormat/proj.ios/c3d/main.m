//
//  main.m
//


#import <UIKit/UIKit.h>
#import "c3dAppDelegate.h"
int main(int argc, char *argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, NSStringFromClass([c3dAppDelegate class]));
    //最后一个参数也可以写nil
    [pool release];
    return retVal;
}
