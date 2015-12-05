//
//  myALSourceManager.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-2-8.
//
//

#include <string>
#include <iostream>
using namespace std;
#include "c3dAudioCache.h"
#include "c3dFileUtils.h"
static Cc3dAudioCache*s_audioCache=NULL;
Cc3dAudioCache*Cc3dAudioCache::sharedAudioCache(){
    if(s_audioCache==NULL){
        s_audioCache=new Cc3dAudioCache();
    }
    return s_audioCache;
    
}

Cc3dALBuffer* Cc3dAudioCache::createBuffer(const string&_filePathShort)
//?????????????buffer
{
 //   cout<<"buffer count:"<<(int)pBufferList.size()<<endl;
    //???????????????????buffer?????????????????????????????????????
    Cc3dALBuffer*pBuffer=getBufferByKey(_filePathShort);
    if(pBuffer!=NULL){//?????
   //     cout<<"hi"<<endl;
        //???????
        return pBuffer;
    }//??????
    //???buffer
    pBuffer=new Cc3dALBuffer();
    pBuffer->autorelease();
    pBuffer->retain();
    //cout<<"_filePathShort:"<<_filePathShort<<endl;
    pBuffer->initBuffer(_filePathShort);
    //??pBuffer????pBufferList
    m_bufferList.push_back(pBuffer);
    //????pBuffer
    return pBuffer;
}
Cc3dALSource* Cc3dAudioCache::createSource( Cc3dALBuffer*buffer,const string&name,bool isReuse)
//????buffer????source
{
 //   cout<<"source count: "<<(int)pSourceList.size()<<endl;
//    assert((int)pSourceList.size()<=maxSimultaneouslyPlay);
    assert(getBufferIndex(buffer)!=-1);
    if(isReuse){
        vector<Cc3dALSource*> sourceList=getSourcesByBuffer(buffer);
        if(sourceList.empty()==false){
        /*    cout<<"sourceName:"<<source->getName()<<endl;
            cout<<"bufferName:"<<buffer->getFilePathShort()<<endl;
            cout<<"sourceBufferName:"<<source->getBuffer()->getFilePathShort()<<endl;
         */
            int nSource=(int)sourceList.size();
            for(int i=0;i<nSource;i++){
                Cc3dALSource*source=sourceList[i];
                if(source->getName()==name)return source;
            }
        }
    }
    //???source
    Cc3dALSource*source=new Cc3dALSource();
    source->autorelease();
    source->retain();
    source->setName(name);
    source->initSource(buffer);
    //??pSource????pSourceList
    m_sourceList.push_back(source);
    //????pSource
    return source;
    
}