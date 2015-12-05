//
//  touchSequence.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-21.
//
//

#ifndef __HelloOpenGL__touchSequence__
#define __HelloOpenGL__touchSequence__

#include <iostream>
using namespace std;
#include "c3d.h"
enum eTouchType{
    e_touchBegan=0,
    e_touchMove,
    e_touchEnd,
};
class Ctouch{
public:
    Cc3dVector2 m_point;
    eTouchType m_type;
    double m_time;
    Ctouch(){
        initMembers();
    }
    Ctouch(float x,float y,eTouchType touchType, double time){
        initMembers();
        init(x, y, touchType, time);
    }
    void init(float x,float y,eTouchType touchType, double time){
        m_point.init(x, y);
        m_type=touchType;
        m_time=time;
    }
    Cc3dVector2 getPoint()const{return m_point;}
    double getTime()const{return m_time;}
    eTouchType getType()const{return m_type;}
    void print(){
        cout<<"touch: "<<m_point.x()<<" "<<m_point.y()<<" "<<m_type<<" "<<m_time<<endl;
    }
protected:
    void initMembers(){
        m_time=0;
        m_type=e_touchBegan;
    }
    
};
const int touchSequenceMaxLength=100;
const double timeLongLongAgo=-100000;
class CtouchSequence{
public:
    static CtouchSequence*sharedTouchSequence();
    CtouchSequence(){
        //put 3 guard
        m_touchList.push_back(Ctouch(0,0,e_touchBegan,timeLongLongAgo));
        m_touchList.push_back(Ctouch(0,0,e_touchMove,timeLongLongAgo+1));
        m_touchList.push_back(Ctouch(0,0,e_touchEnd,timeLongLongAgo+2));
        
    }
    void addTouch(const Ctouch&touch){
        //limit the length of touchList not exceed touchSequenceMaxLength
        int n=(int)m_touchList.size();
        if(n<touchSequenceMaxLength){//add directly
            m_touchList.push_back(touch);
        }else{
            //move left
            for(int i=0;i<n-1;i++){
                m_touchList[i]=m_touchList[i+1];
            }
            //fill touch to tail element
            m_touchList[n-1]=touch;
			//the first three element set as guard
            m_touchList[0]=Ctouch(0,0,e_touchBegan,timeLongLongAgo);
            m_touchList[1]=Ctouch(0,0,e_touchMove,timeLongLongAgo+1);
            m_touchList[2]=Ctouch(0,0,e_touchEnd,timeLongLongAgo+2);
        }
    }
    vector<Ctouch> getTouchList() {
        return m_touchList;
    }
    vector<Ctouch> getLatestTouches(){
        double latestTouchTime=getLatestTouchTime();
        return getTouchesAtTime(latestTouchTime);
    }
    double getLatestTouchTime(){
        assert(m_touchList.empty()==false);
        return m_touchList[(int)m_touchList.size()-1].getTime();
    }
    vector<Ctouch> getTouchesAtTime(double time){
        vector<Ctouch> touches;
        int n=(int)m_touchList.size();
        for(int i=0;i<n;i++){
            const Ctouch&touch=m_touchList[i];
            if(touch.getTime()==time){
                touches.push_back(touch);
            }
        }
        return touches;
    }
    double getLatestTouchTypeTime(eTouchType type){
        int n=(int)m_touchList.size();
        for(int i=n-1;i>=0;i--){
            const Ctouch&touch=m_touchList[i];
            if(touch.getType()==type){
                return touch.getTime();
            }
        }
        assert(false);
    }
    vector<Ctouch> getLatestTouchesWithType(eTouchType type){
        double latestTouchBeganTime=getLatestTouchTypeTime(type);
        return getTouchesAtTimeWithType(latestTouchBeganTime, type);
    }
    vector<Ctouch> getTouchesAtTimeWithType(double time,eTouchType type){
        vector<Ctouch> touches;
        int n=(int)m_touchList.size();
        for(int i=0;i<n;i++){
            const Ctouch&touch=m_touchList[i];
            if(touch.getTime()==time&&touch.getType()==type){
                touches.push_back(touch);
            }
        }
        return touches;
    }
	vector<Ctouch> getTouchesInTimeSpanWithType(double time0,double time1,eTouchType type){//time span is [time0,time1]
		assert(time0<=time1);
        vector<Ctouch> touches;
        int n=(int)m_touchList.size();
        for(int i=0;i<n;i++){
            const Ctouch&touch=m_touchList[i];
            if(touch.getTime()>=time0&&touch.getTime()<=time1&&touch.getType()==type){
                touches.push_back(touch);
            }
        }
        return touches;
    }
    double getEarlierTime(double time){
        assert(time>=0);
        int n=(int)m_touchList.size();
        for(int i=n-1;i>=0;i--){
            const Ctouch&touch=m_touchList[i];
            if(touch.getTime()<time){
                return touch.getTime();
            }
        }
        assert(false);
    }
    vector<Ctouch> getTouchesAtEarlierTime(double time){
        double earlierTime=getEarlierTime(time);
        return getTouchesAtTime(earlierTime);
    }
    vector<Ctouch> getTouchesAtEarlierTimeWithType(double time,eTouchType type){
        double earlierTime=getEarlierTime(time);
        return getTouchesAtTimeWithType(earlierTime,type);
    }
    int getTouchCount()const{return (int)m_touchList.size();}
protected:
    vector<Ctouch> m_touchList;
    
};
#endif /* defined(__HelloOpenGL__touchSequence__) */
