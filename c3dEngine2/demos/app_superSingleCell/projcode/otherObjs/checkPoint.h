//
//  checkPoint.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-3-23.
//
//

#ifndef __HelloOpenGL__checkPoint__
#define __HelloOpenGL__checkPoint__

#include <iostream>
#include "bossCell.h"
#include "bubble.h"
#include "c3d.h"
class CcheckPoint
{
protected:
    bool isEmpty;
    Cc3dTransform m_transform;
public:
    
    vector<Cc3dALSource*> playingALSourcePtrList;//正在播放的alsource指针列表--abc
    CcheckPoint(){
        isEmpty=true;
    }
    virtual~CcheckPoint(){}
    void reSetToEmpty(){
        isEmpty=true;
    }
    bool getIsEmpty()const {
        return isEmpty;
    }
    void save(CbossCell &_cell){
        isEmpty=false;
        m_transform=*_cell.getTransform();
        playingALSourcePtrList=Cc3dAudioCache::sharedAudioCache()->getPlayingSourceList();
    }
    void recover(CbossCell &_cell)const {
        Cc3dTransform*transform=new Cc3dTransform();
        transform->autorelease();
        *transform=m_transform;
        _cell.setTransform(transform);
        //重启playingALSourcePtrList中各source
        int nSource=(int)playingALSourcePtrList.size();
        for(int i=0;i<nSource;i++){
            playingALSourcePtrList[i]->play();
        }
        
    }
    
};
#endif /* defined(__HelloOpenGL__checkPoint__) */
