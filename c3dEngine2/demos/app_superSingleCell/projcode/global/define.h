//
//  define.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-15.
//
//

#ifndef __HelloOpenGL__define__
#define __HelloOpenGL__define__

#include <iostream>
using namespace std;
//----天空球半径--abc
const float r_sky=4572.9198;
//----获得的金币数位数和消灭的敌人数位数--abc
const int nCoinPositivePlaceCount=3;
const int nKillEnemyPostivePlaceCount=3;
//----一些quad的持续时间--abc
const float duringTime_quad_out_of_fuel=40;
const float duringTime_quad_halfFuelHaveBeenUsed=120;
//----无伤害持续时间（重生后一定时间内无伤害)
const long noHurtDuringTime=200;
//----按钮大小--abc
const float r_key=28;
//----敌人参数--abc
const float MHD_bossCellMidFar=600;//距主角的曼哈顿距离超过此值，认为bossCell中等远--abc
const float MHD_bossCellTooFar=900;//距主角的曼哈顿距离越过此值，认为bossCell很远--abc
//----地形尺寸--abc
const float blockWidth=331.371*8.67;
const float blockHeight=331.371*8.67;
//----相机相对于cell的位置--abc
const float eyePosInCell_normal[4]={0,32*2,60*2,1};//正常情况下cell坐标系下相机的位置--abc
const float k_eyePosInCell_zoomOut=10;//在拉远情况下相机相对于cell的位置的放大倍数--abc
//----共享纹理挂接点--abc
//GL_TEXTURE1: shadow map1
//GL_TEXTURE2: 挂shadow map2
//GL_TEXTURE3: 挂水面alpha纹理--abc
//GL_TEXTURE4: 挂地面alpha纹理--abc
//GL_TEXTURE5: 挂地面texture1
//GL_TEXTURE6: 挂地面texture2
//GL_TEXTURE7: 挂地面texture3

#endif /* defined(__HelloOpenGL__define__) */
