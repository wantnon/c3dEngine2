
#import <UIKit/UIKit.h>
#import "c3dView.h"

@interface Cc3dViewController : UIViewController <UIGestureRecognizerDelegate,UIAccelerometerDelegate>
{
     UIAccelerometer *accelerometer;

}
@property (nonatomic, retain) UIAccelerometer *accelerometer;
@end
