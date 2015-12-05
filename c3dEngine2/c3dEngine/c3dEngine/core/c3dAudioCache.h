//
//  c3dAudioCache.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-2-8.
//
//

#ifndef __HelloOpenGL__c3dAudioCache__
#define __HelloOpenGL__c3dAudioCache__

#include <iostream>
#include <vector>
using namespace std;

#include "core/c3dALSource.h"


#include "common/c3dCommonFunc.h"




class Cc3dAudioCache
{
protected:
    vector<Cc3dALSource*> m_sourceList;
    vector<Cc3dALBuffer*> m_bufferList;
    bool m_isRemoveUnusedBuffersAndSourcesOnNextFrame;
public:
    static Cc3dAudioCache*sharedAudioCache();
    Cc3dAudioCache(){
        m_isRemoveUnusedBuffersAndSourcesOnNextFrame=false;
    }
    virtual ~Cc3dAudioCache(){
        //----注意,一定要先释放source再释放buffer,否则出现al_invalid_operation错误--abc
        //销毁source
        int nSource=(int)m_sourceList.size();
        for(int i=0;i<nSource;i++){
            m_sourceList[i]->release();
        }
        //销毁buffer
        int nBuffer=(int)m_bufferList.size();
        for(int i=0;i<nBuffer;i++){
            m_bufferList[i]->release();
        }
        m_sourceList.clear();
        m_bufferList.clear();
    }
    void removeUnusedBuffersAndSources(){
    //    stopAllPlayingSource();
        int sourceRemovedCount=0;
        //must release sources first and then release buffers
        int nSource=(int)m_sourceList.size();
        for(int i=0;i<nSource;i++){
            Cc3dALSource*source=m_sourceList[i];
            if(source->getRetainCount()==1){
                source->release();
                m_sourceList.erase(m_sourceList.begin()+i);
                i--;
                nSource--;
                sourceRemovedCount++;
            }
        }
    //    cout<<"source remove count:"<<sourceRemovedCount<<endl;
        int nBuffer=(int)m_bufferList.size();
        for(int i=0;i<nBuffer;i++){
            Cc3dALBuffer*buffer=m_bufferList[i];
            if(buffer->getRetainCount()==1){
                buffer->release();
                m_bufferList.erase(m_bufferList.begin()+i);
                i--;
                nBuffer--;
            }
        }
    }
    void removeUnusedBuffersAndSourcesOnNextFrame(){
        m_isRemoveUnusedBuffersAndSourcesOnNextFrame=true;
    }
    void performDelayRemoveUnusedBuffersAndSources(){
        if(m_isRemoveUnusedBuffersAndSourcesOnNextFrame){
            removeUnusedBuffersAndSources();
            m_isRemoveUnusedBuffersAndSourcesOnNextFrame=false;
        }
    }
    Cc3dALBuffer* getBufferByKey(const string&key)
    //看_filePathShort对应的buffer是否已经存在,如果存在返回指针,否则返回NULL
    {
        int nBuffer=(int)m_bufferList.size();
        for(int i=0;i<nBuffer;i++){
            assert(m_bufferList[i]);
            if(m_bufferList[i]->getFilePath()==key){
                return m_bufferList[i];
            }
        }
        return NULL;
    }
    int getBufferIndex(const Cc3dALBuffer*buffer)const{//return -1 if failed
        assert(buffer);
        int n=(int)m_bufferList.size();
        for(int i=0;i<n;i++){
            if(m_bufferList[i]==buffer){
                return i;
            }
        }
        return -1;
        
    }
    int getSourceCount()const{return (int)m_sourceList.size();}
    int getBufferCount()const{return (int)m_bufferList.size();}
    int getSourceIndex(const Cc3dALSource*source)const{//return -1 if failed
        assert(source);
        int n=(int)m_sourceList.size();
        for(int i=0;i<n;i++){
            if(m_sourceList[i]==source){
                return i;
            }
        }
        return -1;
        
    }
    vector<Cc3dALSource*>getSourcesByBuffer(const Cc3dALBuffer*buffer)
    {
        assert(buffer);
        assert(getBufferIndex(buffer)!=-1);
        vector<Cc3dALSource*> sourceList;
        int nSource=(int)sourceList.size();
        for(int i=0;i<nSource;i++){
            if(sourceList[i]->getBuffer()==buffer){
                sourceList.push_back(sourceList[i]);
            }
        }
        return sourceList;
    }
    Cc3dALSource*getSourceByName(const string&name){
        int nSource=(int)m_sourceList.size();
        for(int i=0;i<nSource;i++){
            Cc3dALSource*source=m_sourceList[i];
            if(source->getName()==name){
                return source;
            }
        }
        return NULL;

    
    }
 /*   void removeBuffer(Cc3dALBuffer*buffer)
    {
        int index=getBufferIndex(buffer);
        assert(index!=-1);
        buffer->release();
        m_bufferList.erase(m_bufferList.begin()+index);

    }
    void removeSource(Cc3dALSource*source)
    {
        int index=getSourceIndex(source);
        assert(index!=-1);
        source->release();
        m_sourceList.erase(m_sourceList.begin()+index);
        
        
    }*/
    

    Cc3dALSource *createBufferAndSource(const string&filePath,const string&name,bool isReuseSource=true){
        Cc3dALBuffer*pBuffer=createBuffer(filePath);
        return createSource(pBuffer,name,isReuseSource);
    }
    void stopAllPlayingSource(){
        int nSource=(int)m_sourceList.size();
        for(int i=0;i<nSource;i++){
            if(m_sourceList[i]->getIsPlaying()){
                m_sourceList[i]->stop();
            }
        }
    }
    void pauseAllPlayingSource(){
        int nSource=(int)m_sourceList.size();
        for(int i=0;i<nSource;i++){
            if(m_sourceList[i]->getIsPlaying()){
                m_sourceList[i]->pause();
            }
        }
    }
    vector<Cc3dALSource*> getPlayingSourceList()const {
        vector<Cc3dALSource*> playingSourceList;
        int nSource=(int)m_sourceList.size();
        for(int i=0;i<nSource;i++){
            if(m_sourceList[i]->getIsPlaying()){
                playingSourceList.push_back(m_sourceList[i]);
            }
        }
        return playingSourceList;
    }
    void continuePausedSource(){//继续播放所有暂停的source
        int nSource=(int)m_sourceList.size();
        for(int i=0;i<nSource;i++){
            if(m_sourceList[i]->getIsPaused()){
                m_sourceList[i]->play();
            }
        }
    }
    void print(){
        //print all buffers and related sources
        cout<<"-----------------------------------"<<endl;
#if DEBUG == 1
        int nBuffer=(int)m_bufferList.size();
        for(int i=0;i<nBuffer;i++){
            Cc3dALBuffer*buffer=m_bufferList[i];
            cout<<"buffer:"<<buffer->getFilePath()<<endl;
            cout<<"related sources: ";
            int nSource=(int)m_sourceList.size();
            for(int j=0;j<nSource;j++){
                Cc3dALSource*source=m_sourceList[j];
                if(source->getBuffer()==buffer){
                    cout<<source->getName()<<" ";
                }
            }cout<<endl;
        }
#endif
        cout<<"total buffer:"<<(int)m_bufferList.size()<<endl;
        cout<<"total source:"<<(int)m_sourceList.size()<<endl;
        cout<<"-----------------------------------"<<endl;
        
    }
protected:
    Cc3dALBuffer *createBuffer(const string&_filePathShort);
    Cc3dALSource *createSource(Cc3dALBuffer*pBuffer,const string&name,bool isReuse=true);
    
    
    
};




#endif /* defined(__HelloOpenGL__c3dAudioCache__) */
