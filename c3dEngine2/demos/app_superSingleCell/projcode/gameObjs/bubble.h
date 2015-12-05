//
//  bubble.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-1-2.
//
//

#ifndef __HelloOpenGL__bubble__
#define __HelloOpenGL__bubble__

#include <iostream>
#include <vector>
using namespace std;
#include "sprite.h"
#include "molecule.h"
#include "life.h"
#include "c3d.h"
class Cbubble:public Csprite
{
protected:
    Cmolecule* m_molecule;
    Csprite* m_IDBoard0;//显示ID的公告牌，ID第0位--abc
    Csprite* m_IDBoard1;//显示ID的公告牌，ID第1位--abc
public:
    string fileName_sound_eaten;
public:
    int ID;
    string name;
    
    Clife life;

    Cbubble(){
        ID=0;
        m_molecule=NULL;
        m_IDBoard0=NULL;
        m_IDBoard1=NULL;
        
    }
    virtual~Cbubble(){
    }
    void setIsVisible(bool value){
        this->Csprite::setIsVisible(value);
        if(m_molecule)m_molecule->setIsVisible(value);
        if(m_IDBoard0)m_IDBoard0->setIsVisible(value);
        if(m_IDBoard1)m_IDBoard1->setIsVisible(value);
        
    }
    Csprite*getIDBoard0()const {return m_IDBoard0;}
    Csprite*getIDBoard1()const {return m_IDBoard1;}
    void setIDBoard0(Csprite*IDBoard0){
        assert(IDBoard0);
        if(m_IDBoard0==NULL){
            m_IDBoard0=IDBoard0;
            addChild(m_IDBoard0);
        }else{
            m_IDBoard0->removeFromParent();
            m_IDBoard0=IDBoard0;
            addChild(m_IDBoard0);
        }
    }
    void setIDBoard1(Csprite*IDBoard1){
        assert(IDBoard1);
        if(m_IDBoard1==NULL){
            m_IDBoard1=IDBoard1;
            addChild(m_IDBoard1);
        }else{
            m_IDBoard1->removeFromParent();
            m_IDBoard1=IDBoard1;
            addChild(m_IDBoard1);
        }
        
    }

    Cmolecule*getMolecule()const{
        return m_molecule;
    }
    void setMolecule(Cmolecule*molecule){
        assert(molecule);
        if(m_molecule==NULL){
            m_molecule=molecule;
            addChild(m_molecule);
        }else{
            
            m_molecule->removeFromParent();
            m_molecule=molecule;
            addChild(m_molecule);
        }
        
    }
    


    
    
    
};
class CbubbleSet:public CspriteSet
{
public:
     int getCoinBubbleCount()const{//获得coin bubble的数量--abc
        int count=0;
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            Cbubble*pbubble=(Cbubble*)actorList[i];
            if(pbubble!=NULL&&pbubble->getMolecule()!=NULL
               &&pbubble->getMolecule()->funcID==MOLECULE_FUNC_coinAdd){
                count++;
            }
        }
        return count;
    }
    Cbubble*getCoinBubbleByIndex(int index){
        assert(index>=0&&index<getCoinBubbleCount());
        return (Cbubble*)actorList[index];
    }

    void calRange(float &xmin,float &xmax,float &ymin,float &ymax,float &zmin,float &zmax){
        int n=(int)actorList.size();
        if(n==0){
            xmin=0;
            xmax=0;
            ymin=0;
            ymax=0;
            zmin=0;
            zmax=0;
            return;
        }
        xmin=c3d_INF;
		xmax=-c3d_INF;
		ymin=c3d_INF;
		ymax=-c3d_INF;
		zmin=c3d_INF;
		zmax=-c3d_INF;
        
        for(int i=0;i<n;i++){
            Cbubble*pbubble=(Cbubble*)actorList[i];
            Cc3dVector4 v=pbubble->getPos();
            if(v.x()<xmin)xmin=v.x();
            if(v.x()>xmax)xmax=v.x();
            if(v.y()<ymin)ymin=v.y();
            if(v.y()>ymax)ymax=v.y();
            if(v.z()<zmin)zmin=v.z();
            if(v.z()>zmax)zmax=v.z();
        }
        //加个margin
        const float margin=10;
        xmin-=margin;
        xmax+=margin;
        ymin-=margin;
        ymax+=margin;
        zmin-=margin;
        zmax+=margin;
        
        
    }

    void loadConfig(const string&fileNameWithExt){
        //------------------获得绝对路径--abc
        string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(fileNameWithExt);
        //打开文件--abc
        FILE  *fp=NULL;
        fp=fopen(pathName.c_str(),"r");
        if(fp==NULL)
        {
            cout<<"Open "<<pathName<<" failed!"<<endl;
            assert(false);
        }
        //------------------提取文件信息，并建立列模型列表--abc
        {
            char buffer[1024]={0};
            int ID_foe=-1;
            //提取项目数--abc
            int n;
            fscanf(fp, "%d",&n);
            //提取项目--abc
            for(int i=0;i<n;i++){
                Cbubble*pbubble=new Cbubble();
                pbubble->autorelease();
                Cmolecule*pmolecule=new Cmolecule();
                pmolecule->autorelease();
                
                
                //----bubble ID
                fscanf(fp,"%s", buffer);
                int ID;
                fscanf(fp,"%d", &ID);
                pbubble->ID=ID;
                //检查ID是否恰好比ID_foe大1
                if(ID-ID_foe!=1){
                    cout<<"error:ID不连续!"<<endl;
                    assert(false);
                }else{
                    ID_foe=ID;//更新ID_foe
                }

                //----bubble name
                fscanf(fp, "%s",buffer);
                pbubble->name=buffer;
                //----sound_eaten
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%s",buffer);
                pbubble->fileName_sound_eaten=buffer;
                //----pos
                float pos[4]={0,0,0,1};
                fscanf(fp, "%f",&pos[0]);
                fscanf(fp, "%f",&pos[1]);
                fscanf(fp, "%f",&pos[2]);
                pbubble->setPos(pos);
                //----r
                float r;
                fscanf(fp, "%f",&r);
                pbubble->init(r, r);
                pbubble->setRc(r);
                //----moleculeOffset
                float moleculeOffset[3];
                fscanf(fp, "%f",&moleculeOffset[0]);
                fscanf(fp, "%f",&moleculeOffset[1]);
                fscanf(fp, "%f",&moleculeOffset[2]);
                Cc3dVector4 moleculePos;
                Cc3dVector4 bubblePos;
                bubblePos=pbubble->getPos();
                moleculePos=Cc3dVector3(moleculeOffset).toV4(0)+bubblePos;
                pmolecule->setPos(moleculePos);
                //----molecule name
                char buffer2[1024];
                fscanf(fp, "%s",buffer2);
                pmolecule->name=buffer2;
                //----molecule funcName
                char buffer3[1024];
                fscanf(fp, "%s",buffer3);
                string funcName=buffer3;
                pmolecule->setFuncIDWithFuncName(funcName);
                //如果funcName为transport，则还要继续提取起点和终点名--abc
                if(funcName=="transport"){
                    char buffer_start[1024];
                    fscanf(fp, "%s",buffer_start);
                    char buffer_dest[1024];
                    fscanf(fp, "%s",buffer_dest);
                    pmolecule->startName=buffer_start;
                    pmolecule->destinationName=buffer_dest;
                }
                //add molecule to bubble
                //add molecule to bubble should be late than bubble init, in order to draw bubble first than draw molecule
                pbubble->setMolecule(pmolecule);
                //将pbubble加入到pModelBaseList
                addActor(pbubble);
            }
            fscanf(fp, "%s",buffer);
            if(buffer[0]!='#'){
                cout<<"error:缺少#或实际条目数比n值大!"<<endl;
                assert(false);
            }
        }
        //关闭文件--abc
        fclose(fp);
    }
};
#endif /* defined(__HelloOpenGL__bubble__) */
