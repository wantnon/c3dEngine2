//
//  c3dRange.h
//  HelloCpp
//
//  Created by wantnon (yang chao) on 14-1-31.
//
//

#ifndef __HelloCpp__c3dRange__
#define __HelloCpp__c3dRange__

#include <iostream>
#include <assert.h>
using namespace std;
#include "c3dVector.h"
#include "c3dSpan.h"
class Cc3dRange{
public:
    Cc3dRange(){
        initMembers();
    }
    Cc3dRange(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax){
        initMembers();
        init(xmin, xmax, ymin, ymax, zmin, zmax);
    }
    virtual~Cc3dRange(){}
    bool init(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax){
        assert(xmax>=xmin);
        assert(ymax>=ymin);
        assert(zmax>=zmin);
        m_xmin=xmin;
        m_xmax=xmax;
        m_ymin=ymin;
        m_ymax=ymax;
        m_zmin=zmin;
        m_zmax=zmax;
        return true;
    }
    float getMinX()const{return m_xmin;}
    float getMaxX()const{return m_xmax;}
    float getMinY()const{return m_ymin;}
    float getMaxY()const{return m_ymax;}
    float getMinZ()const{return m_zmin;}
    float getMaxZ()const{return m_zmax;}
    void setXmin(float xmin) {m_xmin=xmin;}
    void setXmax(float xmax) {m_xmax=xmax;}
    void setYmin(float ymin) {m_ymin=ymin;}
    void setYmax(float ymax) {m_ymax=ymax;}
    void setZmin(float zmin) {m_zmin=zmin;}
    void setZmax(float zmax) {m_zmax=zmax;}
    float getSpanX()const{return m_xmax-m_xmin;}
    float getSpanY()const{return m_ymax-m_ymin;}
    float getSpanZ()const{return m_zmax-m_zmin;}
    Cc3dVector4 getCenter()const{
        Cc3dVector4 center((m_xmin+m_xmax)/2,(m_ymin+m_ymax)/2,(m_zmin+m_zmax)/2,1);
        return center;
    }
    bool containPoint(const Cc3dVector4&point){
        float x=point.x();
        float y=point.y();
        float z=point.z();
        if(x>m_xmin&&x<m_xmax&&y>m_ymin&&y<m_ymax&&z>m_zmin&&z<m_zmax){
            return true;
        }else{
            return false;
        }
    }
    void expand(float marginx,float marginy,float marginz){
        m_xmin-=marginx;
        m_xmax+=marginx;
        m_ymin-=marginy;
        m_ymax+=marginy;
        m_zmin-=marginz;
        m_zmax+=marginz;
    }
    void mov(float dx,float dy,float dz){
        m_xmin+=dx;
        m_xmax+=dx;
        m_ymin+=dy;
        m_ymax+=dy;
        m_zmin+=dz;
        m_zmax+=dz;
    }
    void mov(const Cc3dVector4&d){
        mov(d.x(),d.y(),d.z());
    }
    void add(const Cc3dRange&range){
        m_xmin=min(m_xmin, range.getMinX());
        m_xmax=max(m_xmax, range.getMaxX());
        m_ymin=min(m_ymin, range.getMinY());
        m_ymax=max(m_ymax, range.getMaxY());
        m_zmin=min(m_zmin, range.getMinZ());
        m_zmax=max(m_zmax, range.getMaxZ());
    }
    Cc3dSpan getSpan()const{
        Cc3dSpan span(m_xmax-m_xmin,m_ymax-m_ymin,m_zmax-m_zmin);
        return span;
    }
protected:
    void initMembers(){
        m_xmin=m_xmax=0;
        m_ymin=m_ymax=0;
        m_zmin=m_zmax=0;
    }
protected:
    float m_xmin,m_xmax;
    float m_ymin,m_ymax;
    float m_zmin,m_zmax;
};

#endif /* defined(__HelloCpp__c3dRange__) */
