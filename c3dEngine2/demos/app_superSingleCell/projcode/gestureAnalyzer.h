//
//  gestureAnalyzer.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-13.
//
//

#ifndef __HelloOpenGL__gestureAnalyzer__
#define __HelloOpenGL__gestureAnalyzer__
#include <iostream>
#include <limits>
using namespace std;
#include "c3dVector.h"
#include "touchSequence.h"
#include "c3dDeviceAndOSInfo.h"
#include "c3dGlobalTimer.h"

class CgestureAnalyzer
{
    
public:
    CgestureAnalyzer(){
        
    }
    virtual~CgestureAnalyzer(){
        
    }
    static CgestureAnalyzer* sharedGestureAnalyzer();
    Cc3dVector2 getPoint()const{//math coordinate (origin at left down corner)
        Ctouch touch=CtouchSequence::sharedTouchSequence()->getLatestTouches()[0];
        Cc3dVector2 point=Cc3dVector2(touch.getPoint().x(),
                                      Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y()-touch.getPoint().y());
        return point;
    }
    
    Cc3dVector2 getEarlierPoint()const{
        Ctouch touch=CtouchSequence::sharedTouchSequence()->getLatestTouches()[0];
        long time=touch.getTime();
        long earlierTime=CtouchSequence::sharedTouchSequence()->getEarlierTime(time);
        vector<Ctouch> earlierTouches=CtouchSequence::sharedTouchSequence()->getTouchesAtTime(earlierTime);
        assert(earlierTouches.empty()==false);
        Cc3dVector2 point=Cc3dVector2(earlierTouches[0].getPoint().x(),
                                      Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y()-earlierTouches[0].getPoint().y());
        return point;
    }
    Cc3dVector2 getLatestTouchBeganPoint()const {
        Cc3dVector2 point=Cc3dVector2(CtouchSequence::sharedTouchSequence()->getLatestTouchesWithType(e_touchBegan)[0].getPoint().x(),
                                      Cc3dDeviceAndOSInfo::sharedDeviceAndOSInfo()->getScreenSize().y()-CtouchSequence::sharedTouchSequence()->getLatestTouchesWithType(e_touchBegan)[0].getPoint().y());
        return point;
    }
    long getLatestTouchBeganTime()const{
        return CtouchSequence::sharedTouchSequence()->getLatestTouchTypeTime(e_touchBegan);
    }
    
    
    bool getIsTapOnce(){
		//if current is up, and the latest down is not too long from crrent time, then juged as TapOnce
        if(CtouchSequence::sharedTouchSequence()->getTouchesAtTimeWithType(Cc3dFrameCounter::sharedFrameCounter()->getCount(), e_touchEnd).empty()==false){
            long latestTouchBeganTime=CtouchSequence::sharedTouchSequence()->getLatestTouchTypeTime(e_touchBegan);
            if(Cc3dFrameCounter::sharedFrameCounter()->getCount()-latestTouchBeganTime<20){//10
				//cout<<"isTapOnce"<<endl;
                return true;
            }
        }
        return false;
    }
    Cc3dVector2 getTouchMoveSpeed(){
        if(getIsDown()){
            Cc3dVector2 currentTouchPoint=this->getPoint();
            Cc3dVector2 earlierTouchPoint=CtouchSequence::sharedTouchSequence()->getTouchesAtEarlierTime(Cc3dFrameCounter::sharedFrameCounter()->getCount())[0].getPoint();
            return (currentTouchPoint-earlierTouchPoint)*(1.0/(0.01+Cc3dFrameCounter::sharedFrameCounter()->getCount()-CtouchSequence::sharedTouchSequence()->getEarlierTime(Cc3dFrameCounter::sharedFrameCounter()->getCount())));
        }else{
            return Cc3dVector2(0, 0);
        }
    }
    bool getIsDown()const {
		//by compare the order of latest touchBegan and touchend to judge whether in press down state
        Ctouch touchBegan=CtouchSequence::sharedTouchSequence()->getLatestTouchesWithType(e_touchBegan)[0];
        Ctouch touchEnd=CtouchSequence::sharedTouchSequence()->getLatestTouchesWithType(e_touchEnd)[0];
        if(touchBegan.getTime()<touchEnd.getTime()){
            
            return false;
        }else{
			//cout<<"isDown"<<endl;
            return true;
        }
    }
    
};
#endif /* defined(__HelloOpenGL__gestureAnalyzer__) */
