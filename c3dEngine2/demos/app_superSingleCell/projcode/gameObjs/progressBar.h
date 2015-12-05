//
//  progressBar.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-31.
//
//

#ifndef __HelloOpenGL__progressBar__
#define __HelloOpenGL__progressBar__

#include <iostream>
#include "key.h"
class CprogressBar:public Cc3dNode
{
protected:
    Ckey*m_quad_back;
    Ckey*m_quad_front;
public:
    
    float percentage;//百分比--abc
    CprogressBar(){
        percentage=0.5;
        m_quad_back=NULL;
        m_quad_front=NULL;
    }
    
    
    void reSetLeftDownCorner(float x,float y){
        m_quad_back->reSetLeftDownCorner(x, y);
        m_quad_front->reSetLeftDownCorner(x, y);
    
    }
    Ckey*getQuadBack()const{return m_quad_back;}
    Ckey*getQuadFront()const{return m_quad_front;}
    void genAndInit(float bx,float by,float width,float height){
        assert(m_quad_back==NULL);
        assert(m_quad_front==NULL);
        m_quad_back=new Ckey();
        m_quad_back->autorelease();
        addChild(m_quad_back);
        m_quad_front=new Ckey();
        m_quad_front->autorelease();
        addChild(m_quad_front);
        m_quad_back->genAndInit(bx, bx+width, by, by+height,
                          -0.04,1.04,-0.04,1.04//注意iphone中t轴向下。另外为了边框能完整显示，使用纹理坐标缩放的技巧--abc
                            );
        m_quad_front->genAndInit(bx, bx+width*percentage, by, by+height,
                          -0.04,1.04,-0.04,1.04
                            );

    }
    void update(float _percentage,GLenum usage){
        percentage=_percentage;
        //根据percentage计算新的宽度，更新quad_front的宽度--abc
        m_quad_front->reSetWidth((m_quad_back->xmax-m_quad_back->xmin)*percentage);
        //重新提交顶点--abc
        m_quad_front->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getIndexVBO()->submitVertex(m_quad_front->getModel()->getMeshList()[0]->getSubMeshByIndex(0)->getSubMeshData()->getvlist(),usage);
    }

};
#endif /* defined(__HelloOpenGL__progressBar__) */
