//
//  c3dGlobalTimer.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-19.
//
//

#ifndef __HelloOpenGL__c3dGlobalTimer__
#define __HelloOpenGL__c3dGlobalTimer__

#include <iostream>
using namespace std;
#include <time.h>
#include <assert.h>

#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
#else
#include <sys/time.h>
#endif

class Cc3dGlobalTimer
{
protected:
	//all time is second
	double m_startTime;
    double m_curTime;
	bool m_isStarted;
public:
    static Cc3dGlobalTimer*sharedGlobalTimer();
    Cc3dGlobalTimer(){
        m_startTime=0;
		m_curTime=0;
		m_isStarted=false;
    }
    virtual~Cc3dGlobalTimer(){};
	void start(){
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
		m_startTime=(double)clock()/CLOCKS_PER_SEC;
#else
        //ref to cocos2dx CCDirector::calculateDeltaTime
        struct timeval t_time;
        gettimeofday(&t_time,  0);
        m_startTime=t_time.tv_sec+t_time.tv_usec*0.000001;
#endif
		m_curTime=m_startTime;
		m_isStarted=true;
	}
    double getTimeFromStart(){
		assert(m_isStarted);
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
		m_curTime=(double)clock()/CLOCKS_PER_SEC;
#else
        struct timeval t_time;
        gettimeofday(&t_time,  0);
        m_curTime=t_time.tv_sec+t_time.tv_usec*0.000001;
#endif
        return max(0.0,m_curTime-m_startTime);//must use 0.0, not 0
    }
};



//------------
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32)
const long maxCount=0.5*(numeric_limits<long>::max)();//see:http://blog.chinaunix.net/uid-17102734-id-2830143.html
#else
const long maxCount=0.5*(numeric_limits<long>::max());
#endif


class Cc3dFrameCounter
{
protected:
    long m_count;
public:
    static Cc3dFrameCounter*sharedFrameCounter();
    Cc3dFrameCounter(){
        m_count=0;
    }
    virtual~Cc3dFrameCounter(){};
    long getCount()const{
        return m_count;
    }
    void doCount(){
        m_count++;
        if(m_count>maxCount)m_count=0;//wrap
    }
};

#endif /* defined(__HelloOpenGL__c3dGlobalTimer__) */
