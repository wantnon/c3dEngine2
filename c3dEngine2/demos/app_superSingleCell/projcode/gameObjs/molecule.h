//
//  molecule.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 13-1-5.
//
//

#ifndef __HelloOpenGL__molecule__
#define __HelloOpenGL__molecule__

#include <iostream>
#include "sprite.h"
#include "timeCounter.h"
#include "c3d.h"
//分子作用--abc
#define MOLECULE_FUNC_noFunc 0 //无作用--abc
#define MOLECULE_FUNC_lifeAdd 1 //加生命--abc
#define MOLECULE_FUNC_noMatch 2 //无敌--abc
#define MOLECULE_FUNC_noShotBall 3 //失去射击能力--abc
#define MOLECULE_FUNC_keyAdd 4 //增加钥匙--abc
#define MOLECULE_FUNC_blast_and_lifeDec 5 //爆炸并掉血--abc
#define MOLECULE_FUNC_transport 6 //传送--abc
#define MOLECULE_FUNC_coinAdd 7 //加金币--abc
#define MOLECULE_FUNC_checkPointUpdate 8 //更新检查点--abc
//分子效果--abc
#define MOLECULEEFFECTTYPE_specifiedColorTransp 0
#define MOLECULEEFFECTTYPE_useColorValueAsAlpha 1
#define MOLECULEEFFECTTYPE_normal 2
//重启传送门时间--abc
#define RESTARTTRANSCOUNTVALUE 120
class Cmolecule:public Csprite
{
protected:
    vector<Cc3dTexture*> pFrameTexList;
public:
    string name;
    
    int i_curFrameTex;//当前用的是几号帧--abc
    int i_foeFrameTex;//上一刻用的是几号帧，只在回滚模式中记录和使用--abc
    float wDivh;//横纵比--abc
    int funcID;//功效--abc
    string startName;//传送起点名称--abc
    string destinationName;//传送终点名称--abc
    CtimeCounter revTimeCounter_restartTrans;//还差多少时间恢复传送--abc
    int effectType;//效果类型--abc
    CtimeCounter revTimeCounter_betweenFrame;//两相邻帧之间的倒计时,如果两帧之间不隔着空cycle，则其倒计时值为1
    int timeCountBetweenFrame;//两相邻帧之间的时间间隔，如果两帧率之间不隔着空cycle，则值为1
    Cmolecule(){
        timeCountBetweenFrame=1;
        effectType=MOLECULEEFFECTTYPE_specifiedColorTransp;
        wDivh=1;
        funcID=0;
        i_curFrameTex=-1;
        i_foeFrameTex=-1;
    }
    virtual~Cmolecule(){
        
    }
    vector<Cc3dTexture*> getFrameTexList()const {return pFrameTexList;}
    void addFrameTex(Cc3dTexture*texture){
        pFrameTexList.push_back(texture);
        texture->retain();
    
    }
    void setFuncIDWithFuncName(const string&funcName){
        if(funcName=="noFunc"){
            funcID=MOLECULE_FUNC_noFunc;
        }else if(funcName=="lifeAdd"){
            funcID=MOLECULE_FUNC_lifeAdd;
        }else if(funcName=="noMatch"){
            funcID=MOLECULE_FUNC_noMatch;
        }else if(funcName=="noShotBall"){
            funcID=MOLECULE_FUNC_noShotBall;
        }else if(funcName=="keyAdd"){
            funcID=MOLECULE_FUNC_keyAdd;
        }else if(funcName=="blast_and_lifeDec"){
            funcID=MOLECULE_FUNC_blast_and_lifeDec;
        }else if(funcName=="transport"){
            funcID=MOLECULE_FUNC_transport;
        }else if(funcName=="coinAdd"){
            funcID=MOLECULE_FUNC_coinAdd;
        }else if(funcName=="checkPointUpdate"){
            funcID=MOLECULE_FUNC_checkPointUpdate;
        }else{
            cout<<"error:不识别的funcName:"<<funcName<<endl;
            assert(false);
        }
    
    }
    void useTex(int i){//指定使用frameTexList中哪个帧来作为当前texture
        i_curFrameTex=i;
        int n=(int)pFrameTexList.size();
        this->getMesh()->getSubMeshByIndex(0)->setTexture(pFrameTexList[i%n]);//如果i越界则取余--abc
    }
    void useLastTex(){
        int nTex=(int)pFrameTexList.size();
        useTex(nTex-1);
    }
    bool useNextFrameTex(){//使用下一帧，如果没有下一帧，则绕回，返回是否已播完此轮--abc
        if(revTimeCounter_betweenFrame.getCount()!=0){//如果倒计时还没结束--abc
            //继续倒计时，然后返回--abc
            revTimeCounter_betweenFrame.doCount_dec();
            return false;
        }//如果倒计时已结束--abc
        revTimeCounter_betweenFrame.setValue(timeCountBetweenFrame);//重设倒计时值--abc
        bool lastFrameOfCurRollFinished=false;
        int n=(int)pFrameTexList.size();
        i_curFrameTex++;
        if(i_curFrameTex>=n){
            lastFrameOfCurRollFinished=true;
            i_curFrameTex-=n;
        }
        this->getMesh()->getSubMeshByIndex(0)->setTexture(pFrameTexList[i_curFrameTex]);
        return lastFrameOfCurRollFinished;
    }
    bool useNextFrameTex_notRewind(){//使用下一帧，如果没有下一帧，则停止在当前帧，返回是否已播完此轮--abc
        if(revTimeCounter_betweenFrame.getCount()!=0){//如果倒计时还没结束--abc
            //继续倒计时，然后返回--abc
            revTimeCounter_betweenFrame.doCount_dec();
            return false;
        }//如果倒计时已结束--abc
        revTimeCounter_betweenFrame.setValue(timeCountBetweenFrame);//重设倒计时值--abc
        bool lastFrameOfCurRollFinished=false;
        int n=(int)pFrameTexList.size();
        i_curFrameTex++;
        if(i_curFrameTex>=n){
            lastFrameOfCurRollFinished=true;
            i_curFrameTex--;
        }
        this->getMesh()->getSubMeshByIndex(0)->setTexture(pFrameTexList[i_curFrameTex]);
        return lastFrameOfCurRollFinished;
    }
    void useNextFrameTex_rollback(){//使用下一帧，如果没有下一帧，回滚--abc
        if(revTimeCounter_betweenFrame.getCount()!=0){//如果倒计时还没结束--abc
            //继续倒计时，然后返回--abc
            revTimeCounter_betweenFrame.doCount_dec();
            return;
        }//如果倒计时已结束--abc
        revTimeCounter_betweenFrame.setValue(timeCountBetweenFrame);//重设倒计时值--abc
        if(i_curFrameTex==i_foeFrameTex) {
            i_curFrameTex++;
            
        }else if(i_curFrameTex>i_foeFrameTex){
            i_foeFrameTex=i_curFrameTex;
            i_curFrameTex++;
            if(i_curFrameTex>=(int)pFrameTexList.size()){
                i_curFrameTex-=2;
            }
        }else if(i_curFrameTex<i_foeFrameTex){
            i_foeFrameTex=i_curFrameTex;
            i_curFrameTex--;
            if(i_curFrameTex<0){
                i_curFrameTex+=2;
            }
        
        }
        this->getMesh()->getSubMeshByIndex(0)->setTexture(pFrameTexList[i_curFrameTex]);
    }
    void loadFrameTexList(const string&clipName){//clipName最后一个字符必须是"/"
        if(clipName[(int)clipName.size()-1]!='/'){
            cout<<"error:clipName最后一个字符必须是'/' !";
            assert(false);
        }
        //获得config文件的路径--abc
        string configFileNameWithExt=clipName+"config.txt";
        //------------------获得config文件绝对路径--abc
        string configPathName=Cc3dFileUtils::sharedFileUtils()->getFullPath(configFileNameWithExt);
        //打开文件--abc
        FILE  *fp=NULL;
        fp=fopen(configPathName.c_str(),"r");
        if(fp==NULL)
        {
            cout<<"Open "<<configPathName<<" failed!"<<endl;
            assert(false);
        }
        //------------------提取文件信息--abc
        char buffer[1024]={0};
        //提取帧数--abc
        int nFrame;//帧数--abc
        fscanf(fp, "%d",&nFrame);
        //提取透明色--abc
        float t_transpColor[4]={1,1,1,1};
        fscanf(fp, "%f",&t_transpColor[0]);
        fscanf(fp, "%f",&t_transpColor[1]);
        fscanf(fp, "%f",&t_transpColor[2]);
        transpColor.init(t_transpColor);
        //提取横纵比--abc
        fscanf(fp,"%f",&wDivh);
        //提取effect
        string effectStr;
        char _buffer[1024];
        fscanf(fp, "%s",buffer);
        fscanf(fp, "%s",_buffer);
        effectStr=_buffer;
        if(effectStr=="specifiedColorTransp"){
            effectType=MOLECULEEFFECTTYPE_specifiedColorTransp;
        }else if(effectStr=="useColorValueAsAlpha"){
            effectType=MOLECULEEFFECTTYPE_useColorValueAsAlpha;
        }else if(effectStr=="normal"){
            effectType=MOLECULEEFFECTTYPE_normal;
        }else{
            cout<<"error:未知的effect类型:"<<effectStr<<endl;
            assert(false);
        }
        //提取timeCountBetweenFrame
        fscanf(fp, "%s",buffer);
        fscanf(fp, "%d",&timeCountBetweenFrame);
        //关闭文件--abc
        fclose(fp);
        //------------------获得clipName的最后一级名--abc
        string clipNameLastSeg;//clipName的最后一级名--abc
        string clipNameLastSeg_reverse;//clipNameLastSeg的逆序--abc
        int len_clipName=(int)clipName.size();
        for(int i=len_clipName-2;i>=0;i--){
            if(clipName[i]=='/'){
                break;
            }else{
                clipNameLastSeg_reverse+=clipName[i];
            }
        }//得到clipNameLastSeg_reverse
        int len_clipNameLastSeg_reverse=(int)clipNameLastSeg_reverse.size();
        for(int i=len_clipNameLastSeg_reverse-1;i>=0;i--){
            clipNameLastSeg.push_back(clipNameLastSeg_reverse[i]);
        }//得到clipNameLastSeg
  //      cout<<"hihi:"<<clipNameLastSeg<<endl;
        //------------------制作各图片文件名--abc
        vector<string> imageNameList;
        for(int i=0;i<nFrame;i++){
            string numStr=numberToStr(i+1);//注意，编号是从1开始的--abc
            if((int)numStr.size()==1){//如果是一位数"k"，则改成"0k"
                numStr="0"+numStr;
            }
            string imageName=clipName+clipNameLastSeg+"_"+numStr+".png";
            imageNameList.push_back(imageName);
        }//得到imageNameList
        //------------------根据imageNameList加载纹理--abc
        for(int i=0;i<nFrame;i++){
            string&imageName=imageNameList[i];
            Cc3dTexture*ptex=Cc3dTextureCache::sharedTextureCache()->addImage(imageName);
            this->addFrameTex(ptex);
        }
        
    }
    
    
};
#endif /* defined(__HelloOpenGL__molecule__) */
