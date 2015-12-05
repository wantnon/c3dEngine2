//
//  c3dALSource.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-23.
//
//

#ifndef __HelloOpenGL__c3dALSource__
#define __HelloOpenGL__c3dALSource__

#include <iostream>
#include "c3dALBuffer.h"
#include "c3dMath.h"
/*
//the max number of source play at the same time (maxSimultaneouslyPlay) have limit
//http://stackoverflow.com/questions/2871905/openal-determine-maximum-sources
//though it may allowed that to create more source than maxSimultaneouslyPlay,
//we'd better limit the number of source to maxSimultaneouslyPlay
const int maxSimultaneouslyPlay=32;//16
*/

class Cc3dALSource:public Cc3dObject
{//???source???????buffer
protected:
    ALuint m_source;
    string m_name;
    Cc3dALBuffer*m_buffer;
public:
    
    Cc3dALSource(){
        m_source=0;
        m_buffer=NULL;
        init_dft();
    }
    virtual~Cc3dALSource(){
        if(getIsValidSource()){
            stop();
            alDeleteSources(1, &m_source);
        }
        if(m_buffer)m_buffer->release();
        C3DCHECK_AL_ERROR_DEBUG();
    }
    void init_dft(){};
    Cc3dALBuffer*getBuffer()const{return m_buffer;}
    
    
    bool getIsValidSource()const{
        bool isValid=(m_source!=0&&alIsSource(m_source));
        return isValid;
    }
    void updatePosition(const Cc3dVector4&pos){
        if(m_source==0)return;
        alSourcefv(m_source, AL_POSITION,pos.getArray());
    }

    void play(){//??????????????????source????????????????????source????????
        if(m_source==0)return;
        alSourcePlay(m_source);
    }
    ALint getState()const {
        if(m_source==0){
            cout<<"source==0,????????!"<<endl;
            assert(false);
        };
        ALint sourceState;
        alGetSourcei(m_source, AL_SOURCE_STATE, &sourceState);
        return sourceState;
    }
    bool getIsPlaying()const {//??????????
        if(m_source==0)return false;
        ALint sourceState=getState();
        if(sourceState==AL_PLAYING){
            return true;
        }else{
            return false;
        }
    }
    bool getIsStoped()const {//???????
        if(m_source==0)return false;
        ALint sourceState=getState();
        if(sourceState==AL_STOPPED){
            return true;
        }else{
            return false;
        }
    }
    
    bool getIsPaused()const {
        if(m_source==0)return false;
        ALint sourceState=getState();
        if(sourceState==AL_PAUSED){
            return true;
        }else{
            return false;
        }
    }
    void playIfNotPlaying()
    //??????play????play
    {
        if(m_source==0)return;
        ALint sourceState;
        alGetSourcei(m_source, AL_SOURCE_STATE, &sourceState);
        if(sourceState!=AL_PLAYING){
            alSourcePlay(m_source);
        }
    }
    void setIsLooping(bool looping){
        if(m_source==0)return;
        if(looping){
            alSourcei(m_source, AL_LOOPING, AL_TRUE);
        }else{
            alSourcei(m_source, AL_LOOPING, AL_FALSE);
        }
    }
    
    void stop(){
        if(m_source==0)return;
        alSourceStop(m_source);
    }
    void setGain(float gain){
        if(m_source==0)return;
        gain=maxf(0,gain);//gain??��??0
        alSourcef(m_source, AL_GAIN, gain);
    }
    float getGain(){
        if(m_source==0)return 0;
        float gain;
        alGetSourcef(m_source, AL_GAIN, &gain);
        return gain;
    }
    void gainDec(float dGain){//??��????
        if(m_source==0)return;
        float gain;
        alGetSourcef(m_source, AL_GAIN, &gain);
        float newGain=maxf(0,gain-dGain);//??????��??0
        alSourcef(m_source, AL_GAIN, newGain);
    }
    void pause(){
        if(m_source==0)return;
        alSourcePause(m_source);
        
    }
    void initSource(Cc3dALBuffer*buffer);
    string getName()const {return m_name;}
    void setName(const string&name){m_name=name;}
protected:
    void setBuffer(Cc3dALBuffer*buffer){
        assert(buffer);
        if(m_buffer==NULL){
            m_buffer=buffer;
            buffer->retain();
        }else{
            m_buffer->release();
            m_buffer=buffer;
            m_buffer->retain();
        }
    }
};


#endif /* defined(__HelloOpenGL__c3dALSource__) */
