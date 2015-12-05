//
//  cell.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//

#include "cell.h"
#include "ship.h"
void Ccell::set_ship(Cship*_pShip){
    assert(_pShip);
    if(pShip==NULL){
        pShip=_pShip;
        pShip->retain();
    }else{
        pShip->release();
        pShip=_pShip;
        pShip->retain();
        pShip_latest=_pShip;//同时更新pShip_latest
        
        
        
    }
    
    
}
