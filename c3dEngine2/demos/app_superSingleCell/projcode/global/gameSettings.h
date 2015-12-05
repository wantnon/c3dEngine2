//
//  gameSettings.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-22.
//
//

#ifndef __HelloOpenGL__gameSettings__
#define __HelloOpenGL__gameSettings__

#include <iostream>
class CgameSettings{
public:
    CgameSettings(){
        reset();
    }
    static CgameSettings*sharedGameSettings();
    void setShowBubbleID(bool value){m_showBubbleID=value;}
    bool getShowBubbleID()const{return m_showBubbleID;}
    void setShowMobilePlatformID(bool value){m_showMobilePlatformID=value;}
    bool getShowMobilePlatformID()const{return m_showMobilePlatformID;}
    void setShowDepthTexs(bool value){m_showDepthTexs=value;}
    bool getShowDepthTexs()const{return m_showDepthTexs;}
    void setShowMusicCube(bool value){m_showMusicCube=value;}
    bool getShowMusicCube()const{return m_showMusicCube;}
    void setUnlockAll(bool value){m_unlockAll=value;}
    bool getUnlockAll()const{return m_unlockAll;}
    void setAudioOn(bool value){m_audioOn=value;}
    bool getAudioOn()const{return m_audioOn;}
	void setGroundIsWireMode(bool value){m_groundIsWireMode=value;}
	bool getGroundIsWireMode()const{return m_groundIsWireMode;}
	void reset(){
		m_showBubbleID=false;
        m_showMobilePlatformID=false;
        m_showDepthTexs=false;
        m_showMusicCube=false;
        m_unlockAll=false;
        m_audioOn=true;
		m_groundIsWireMode=false;
	}
protected:
    bool m_showBubbleID;
    bool m_showMobilePlatformID;
    bool m_showDepthTexs;
    bool m_showMusicCube;
    bool m_unlockAll;
    bool m_audioOn;
	bool m_groundIsWireMode;

};
#endif /* defined(__HelloOpenGL__gameSettings__) */
