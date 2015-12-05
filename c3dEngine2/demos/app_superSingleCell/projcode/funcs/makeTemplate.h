//
//  makeTemplate.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-15.
//
//

#ifndef __HelloOpenGL__makeTemplate__
#define __HelloOpenGL__makeTemplate__

#include <iostream>
#include "bossCell.h"
#include "ship.h"
#include "shotBall.h"
Cship*makeShipTemplate(int shipID);
CbossCell*makeCellTemplate(int cellID);
CshotBall*makeShotBallTemplate(int shotBallID,bool isReuseALSource);


#endif /* defined(__HelloOpenGL__makeTemplate__) */
