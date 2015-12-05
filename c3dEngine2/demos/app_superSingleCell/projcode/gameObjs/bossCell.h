//
//  bossCell.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-31.
//
//

#ifndef __HelloOpenGL__bossCell__
#define __HelloOpenGL__bossCell__

#include <iostream>
#include "cell.h"
#include "ship.h"
#include "c3d.h"
class CbossCell:public Ccell
{
public:

    int templateID;//模板ID
    float MHD_fromMyCell;//与主角的曼哈顿距离--abc
    bool midFar;//中等远--abc
    bool tooFar;//太远--abc
    Cc3dRange flyRange_relativeToInitPos;//相对于初始位置的飞行范围--abc
    Cc3dRange flyRange;//飞行范围--abc
    CtimeCounter revTimeCounter_noHert;//重生后从无伤害到有伤害倒计时--abc
    CtimeCounter revTimeCounter_headingToTargetAgain;//再次向目标前进倒计时--abc
    long afterHowLongHeadingToTargetAgain;//多长时间后再次向目标前进--abc
    bool initiativeAttack;//是否主动攻击--abc
    float vup_autoJump;//自动起跳的竖直初速度(对飞行boss不起作用)
    float v_autoForward;//自动前进的速度(对飞行boss不起作用)
    float modelScale;//模型缩放比例--abc
    Cc3dVector4 modelOffsetVec;//模型偏移向量--abc

    CbossCell(){
        
        v_autoForward=1.2;
        vup_autoJump=3.0;
        initiativeAttack=false;
        afterHowLongHeadingToTargetAgain=70;
        templateID=-1;
        tooFar=false;
        midFar=false;
        MHD_fromMyCell=c3d_INF;
    }

    void loadConfigAndMeshes(const string&configPathShort,const string&modelClipPathShort){
        //------------------获得绝对路径--abc
        string pathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(configPathShort);
        //打开文件--abc
        FILE  *fp=NULL;
        fp=fopen(pathName.c_str(),"r");
        if(fp==NULL)
        {
            cout<<"Open "<<pathName<<" failed!"<<endl;
            assert(false);
        }
        //------------------提取文件信息，并建立模型列表--abc
        {
            Cc3dMesh *mesh=new Cc3dMesh();//临时模型--abc
            mesh->autorelease();
            char buffer[1024]={0};
            //提取modelScale
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&this->modelScale);
            //提取modelOffsetVec
            float offset[4]={0,0,0,0};
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&offset[0]);
            fscanf(fp, "%f",&offset[1]);
            fscanf(fp, "%f",&offset[2]);
            this->modelOffsetVec.init(offset);
            //提取nSubMesh
            int nSubMesh;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%d",&nSubMesh);
            //加载模型--abc
            loadInfo_mesh(mesh, modelClipPathShort,this->modelScale,this->modelOffsetVec);
            if((int)mesh->getSubMeshCount()!=nSubMesh){
                cout<<"error:pmeshList.size()!=nSubMesh!"<<endl;
                cout<<"pmeshList.size():"<<(int)mesh->getSubMeshCount()<<endl;
                cout<<"nSubMesh:"<<nSubMesh<<endl;
                assert(false);
            }
            //提取各submesh所属cellPart，并将submesh填充到相应mesh
            for(int i=0;i<nSubMesh;i++){
                //提取subMeshID
                int subMeshID;
                fscanf(fp, "%d",&subMeshID);
                //提取cellPartName
                string cellPartName;
                fscanf(fp, "%s",buffer);
                cellPartName=buffer;
                //根据cellPartName求cellPartID
                int cellPartID=getCellPartID(cellPartName);
                //生成submesh
                Cc3dSubMesh*subMesh=mesh->getSubMeshByIndex(subMeshID);
                //将submesh从mesh中取下，但不删除--abc
                subMesh->retain();
                subMesh->removeFromParent();
                //将subMesh添加到this
                getModel()->getMeshByIndex(cellPartID)->addSubMesh(subMesh);
            }
            //为各模型genVBOBuffers
            {
                int nMesh=(int)getModel()->getMeshCount();
                for(int i=0;i<nMesh;i++){
                    Cc3dMesh*mesh=getModel()->getMeshByIndex(i);
                    if(mesh==NULL)continue;
                    mesh->genVBOBuffers();
                }
            }
            //提取各cellPart的材质参数，并填充到mesh
            for(int i=0;i<CELLPARTCOUNT;i++){
                Cc3dMesh*mesh=getModel()->getMeshByIndex(i);
                fscanf(fp, "%s",buffer);
                //提取diffuse
                float diffuse[4];
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%f",&diffuse[0]);
                fscanf(fp, "%f",&diffuse[1]);
                fscanf(fp, "%f",&diffuse[2]);
                fscanf(fp, "%f",&diffuse[3]);
                //提取ambient
                float ambient[4];
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%f",&ambient[0]);
                fscanf(fp, "%f",&ambient[1]);
                fscanf(fp, "%f",&ambient[2]);
                fscanf(fp, "%f",&ambient[3]);
                //提取specular
                float specular[4];
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%f",&specular[0]);
                fscanf(fp, "%f",&specular[1]);
                fscanf(fp, "%f",&specular[2]);
                fscanf(fp, "%f",&specular[3]);
                //提取shininess
                float shininess;
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%f",&shininess);
                //提取isHighlightUntransp
                float isHighlightUntransp;
                fscanf(fp, "%s",buffer);
                fscanf(fp, "%f",&isHighlightUntransp);
                //填充到mesh
                if(diffuse[0]!=-1){
                    mesh->setDiffuseAlpha(diffuse[3]);
                    mesh->setDiffuseRGB(diffuse[0], diffuse[1], diffuse[2]);
                }
                if(ambient[0]!=-1)mesh->setAmbient(ambient[0], ambient[1], ambient[2]);
                if(specular[0]!=-1)mesh->setSpecular(specular[0], specular[1], specular[2]);
                if(shininess!=-1)mesh->setShininess(shininess);
                if(isHighlightUntransp!=-1)mesh->setIsHighlightUntransp(isHighlightUntransp);
            }
            //提取其余参数--abc
            float Rc_small_normal;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&Rc_small_normal);
            float Rc_small_flat;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&Rc_small_flat);
            float Rc;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&Rc);
            float H_causeFlat;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&H_causeFlat);
            float H_sinkInWater;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&H_sinkInWater);
            if(H_sinkInWater==0){
                cout<<"error:为了使判断是否接触水面更准确鲁棒，要求H_sinkInWater>0，可以取得很小，但不能为0!"<<endl;
                assert(false);
            }
            float v_legsRot;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&v_legsRot);
            float v_forward_roll;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&v_forward_roll);
            float v_back_roll;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&v_back_roll);
            float isFly;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&isFly);
            Cc3dRange flyRange_relativeToInitPos;
            float xmin,xmax,ymin,ymax,zmin,zmax;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&xmin);
            fscanf(fp, "%f",&xmax);
            fscanf(fp, "%f",&ymin);
            fscanf(fp, "%f",&ymax);
            fscanf(fp, "%f",&zmin);
            fscanf(fp, "%f",&zmax);
            flyRange_relativeToInitPos.init(xmin, xmax, ymin, ymax, zmin, zmax);
            int initiativeAttack;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%d",&initiativeAttack);
            float vup_autoJump;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&vup_autoJump);
            float v_autoForward;
            fscanf(fp, "%s",buffer);
            fscanf(fp, "%f",&v_autoForward);
            //填充到this
            this->setRc_small_value_normal(Rc_small_normal);
            this->setRc_small_value_flat(Rc_small_flat);
            this->set_Rc_small(this->Rc_small_value_normal);
            this->setRc(Rc);
            this->set_H_causeFlat(H_causeFlat);
            this->set_H_sinkInWater(H_sinkInWater);
            this->set_v_legsRot(v_legsRot);
            this->set_v_forward_roll_value(v_forward_roll);
            this->set_v_back_roll_value(v_back_roll);
            this->isFly=isFly;
            this->flyRange_relativeToInitPos=flyRange_relativeToInitPos;
            this->initiativeAttack=initiativeAttack;
            this->vup_autoJump=vup_autoJump;
            this->v_autoForward=v_autoForward;
        }
        //关闭文件--abc
        fclose(fp);
    }
    int getCellPartID(const string&cellPartName){
        if(cellPartName=="ene"){
            return CELLPART_ene;
        }else if(cellPartName=="shell"){
            return CELLPART_shell;
        }else if(cellPartName=="core"){
            return CELLPART_core;
        }else if(cellPartName=="legs"){
            return CELLPART_legs;
        }else if(cellPartName=="body"){
            return CELLPART_body;
        }else if(cellPartName=="tooth"){
            return CELLPART_tooth;
        }else if(cellPartName=="eye"){
            return CELLPART_eye;
        }else if(cellPartName=="wingL"){
            return CELLPART_wingL;
        }else if(cellPartName=="wingR"){
            return CELLPART_wingR;
        }else{
            cout<<"error:未知的cellPartName!"<<endl;
            assert(false);
        }
    
    }
    virtual~CbossCell(){
     
 
        
    }
    void autoMov(const Cc3dVector4&targetPos)
    //targetPos为this的攻击目标z位置--abc
    {
        if(initiativeAttack&&revTimeCounter_headingToTargetAgain.getCount()==0){//主动攻击为true且倒计时结束才重新朝向目标--abc
            //求与由this指向主角的单位向量--abc
            Cc3dVector4 posThis=getTransform()->getPos();
            Cc3dVector4 thisToTarget_e=targetPos-posThis;
            thisToTarget_e.sety(0);//舍弃Y分量--abc
            thisToTarget_e=normalize(thisToTarget_e);//得到thisToTarget_e
            //将this的dir软调整为thisToTarget_e
            this->setDir_soft(thisToTarget_e,0.15);
        }else{//倒计时没结束，随机朝向--abc
            if(rand()%30==0)this->rotLeft();
            if(rand()%50==0)this->rotRight();
            
        }
        //前进--abc
        this->m_transform->moveZ(-v_autoForward);
        //随机起跳--abc
        if(rand()%200==0){
            if(this->isJump.get()==false)this->doJump(vup_autoJump);
        }
        if(v_forward_roll_value!=0){
            this->getModel()->getMeshByIndex(CELLPART_body)->getTransform()->rotate(1, 0, 0, cos_v_forward_roll_value, -sin_v_forward_roll_value);
        }
        
    }
    void useFunc(int funcID){
        //加减血不在这里面做--abc
        switch (funcID) {
            case MOLECULE_FUNC_noFunc:
                break;
            case MOLECULE_FUNC_lifeAdd:
                break;
            case MOLECULE_FUNC_noMatch:
                onFire=true;
                break;
            case MOLECULE_FUNC_noShotBall:
                break;
            case MOLECULE_FUNC_keyAdd:
                key_got++;
                break;
            case MOLECULE_FUNC_blast_and_lifeDec:
                break;
            case MOLECULE_FUNC_transport:
                break;
            case MOLECULE_FUNC_coinAdd:
                break;
            case MOLECULE_FUNC_checkPointUpdate:
                break;
            default:
                cout<<"error:未定义的funcID!"<<endl;
                assert(false);
                break;
        }
    }
    bool collisionWithArchWall_boss(CarchScene&archScene,bool multiPoint,const vector<int>&skipTagList,const int intended_nSP)
    //必须放在着陆之后，否则容易发生与地面之下的墙面进行误碰撞--abc
    {
        //位置--abc
        Cc3dVector4 pos=this->m_transform->getPos();
        //主角的向上方向--abc
        Cc3dMatrix4 t_m=m_transform->getRTmat();
        Cc3dVector4 up(t_m.getAt(4),t_m.getAt(5),t_m.getAt(6),0);
        //用于接收单位回退向量--abc
        Cc3dVector4 e_back;
        //碰撞--abc
        float d;
        if(multiPoint){
            d=archScene.collisionTestWithWall_multiPoint(pos,Rc,Rc_small_value_normal,up,e_back,skipTagList,intended_nSP);
        }else{
            d=archScene.collisionTestWithWall_singlePoint(pos,Rc,Rc_small_value_normal,up,e_back,skipTagList,intended_nSP);
        }
        if(d>0){
            //恢复平移分量，不恢复旋转分量，因为允许旋转--abc
            Cc3dMatrix4 RTmat=m_transform->getRTmat();
            RTmat.setAt(12, this->RTmat_store.getAt(12));
            RTmat.setAt(13, this->RTmat_store.getAt(13));
            RTmat.setAt(14, this->RTmat_store.getAt(14));
            m_transform->setRTmat(RTmat);
            Cc3dVector4 back;
            float d_back=d*0.35;//0.25;//d*0.1;//0.25;//0.15;//除了恢复原位外另加一个回退--abc
            back=e_back*d_back;
            back.sety(0);
            m_transform->moveRelativeToFather(back.x(),back.y(),back.z());
            //转弯--abc
            //       transf.rot(0, 1, 0,-1, 0);
            m_transform->rotate(0, 1, 0, 0.766, 0.642);
            //设定再次向目标前进的倒计时--abc
            revTimeCounter_headingToTargetAgain.setValue(afterHowLongHeadingToTargetAgain);
            return true;
        }else{
            return false;
        }
    }
};
class CbossCellSet:public CactorSet
{
public:
    int getLiveBossCount()const{
        int count=0;
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            CbossCell*pbossCell=(CbossCell*)actorList[i];
            if(pbossCell->getIsDied()==false)count++;
        }
        return count;
    }
    vector<Ccell*> getFlyLiveAndMovingBossList()const {
        vector<Ccell*> pFlyLiveBossList;
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            CbossCell*pbossCell=(CbossCell*)actorList[i];
            if(pbossCell->getIsDied()||pbossCell->get_isFly()==false||pbossCell->midFar)continue;
            pFlyLiveBossList.push_back(pbossCell);
        }
        return pFlyLiveBossList;
    }
    vector<Ccell*> getLiveAndMovingBossList()const {
        vector<Ccell*> pLiveBossList;
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            CbossCell*pbossCell=(CbossCell*)actorList[i];
            if(pbossCell->getIsDied()||pbossCell->midFar)continue;
            pLiveBossList.push_back(pbossCell);
        }
        return pLiveBossList;
    }

    Cc3dRange getBossShipFlyRange(){//获得所有bossShip的飞行范围的并--abc
        Cc3dRange range;//和range
        bool rangeIsEmpty=true;
        int n=(int)actorList.size();
        for(int i=0;i<n;i++){
            CbossCell*pbossCell=(CbossCell*)actorList[i];
            if(pbossCell->get_isFly()){
                if(rangeIsEmpty){
                    range=pbossCell->flyRange;
                    rangeIsEmpty=false;
                }else{
                    range.add(pbossCell->flyRange);
                }
            }
        }
        return range;
    
    }
    void loadConfig(const string&fileNameWithExt);

};
#endif /* defined(__HelloOpenGL__bossCell__) */
