//
//  c3dTextureCache.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-4.
//
//
#ifndef HelloOpenGL_c3dTextureCache_h
#define HelloOpenGL_c3dTextureCache_h
#include <vector>
using namespace std;
#include "c3dTexture.h"
#include "c3dGLdebug.h"
class Cc3dTextureCache
{
protected:
    vector<Cc3dTexture*> m_textureList;
    bool m_isRemoveUnusedTexturesOnNextFrame;
public:
    static Cc3dTextureCache*sharedTextureCache();
    Cc3dTextureCache(){
        m_isRemoveUnusedTexturesOnNextFrame=false;
    }
    virtual ~Cc3dTextureCache(){
        for(int i=0;i<(int)m_textureList.size();i++){
            m_textureList[i]->release();
        }
    }
    
    
    void removeUnusedTextures(){
        int n=(int)m_textureList.size();
        for(int i=0;i<n;i++){
            Cc3dTexture*texture=m_textureList[i];
            if(texture->getRetainCount()==1){
                m_textureList.erase(m_textureList.begin()+i);
                i--;
                n--;
                texture->release();
                
            }
            
        }
    }
    void removeUnusedTexturesOnNextFrame(){
        m_isRemoveUnusedTexturesOnNextFrame=true;
    }
    void performDelayRemoveUnusedTextures(){
        if(m_isRemoveUnusedTexturesOnNextFrame){
            removeUnusedTextures();
            m_isRemoveUnusedTexturesOnNextFrame=false;
        }
    
    }
    
    Cc3dTexture* getTextureByIndex(int index){
        assert(index>=0&&index<(int)m_textureList.size());
        return m_textureList[index];
    }
    Cc3dTexture* getTextureByKey(const string&key){//for texture, filePath is the key
        vector<Cc3dTexture*> rsList;
        int n=(int)m_textureList.size();
        for(int i=0;i<n;i++){
            Cc3dTexture*texture=m_textureList[i];
            if(texture->getFilePath()==key){
                rsList.push_back(texture);
            }
        }
        assert((int)rsList.size()<=1);
        if((int)rsList.size()==0){
            return NULL;
        }else{
            return rsList[0];
        }
    }
    Cc3dTexture* addImage( const string&filePath,int wrapS=GL_REPEAT,int wrapT=GL_REPEAT,GLint minFilter=GL_LINEAR_MIPMAP_NEAREST,GLint magFilter=GL_LINEAR,bool genMipmap=true)
	//magFilter can not use GL_XXX_MIPMAP_XXXX, see: http://www.cnblogs.com/wantnon/p/5024363.html
    //if exist , do not create again
    {
		//if found, return texture directly
        Cc3dTexture* texture=getTextureByKey(filePath);
        if(texture!=NULL){//found
            
            return texture;
        }
        //create texture
        texture=new Cc3dTexture();
        texture->autorelease();
        texture->init(filePath,wrapS,wrapT,minFilter,magFilter,genMipmap);
        m_textureList.push_back(texture);
        texture->retain();
    //    C3DLOG("texture filePathShort:%s",filePathShort.c_str());
        return texture;
    }
    void print(){
        cout<<"--------------------"<<endl;
#if DEBUG == 1
        cout<<"texture:"<<endl;
        int n=(int)m_textureList.size();
        for(int i=0;i<n;i++){
            Cc3dTexture*texture=m_textureList[i];
            if(texture->getFilePath().empty()){
                cout<<"EMPTY_PATH"<<" ";
            }else{
                cout<<texture->getFilePath()<<" ";
            }
        }cout<<endl;
#endif
        cout<<"total texture:"<<(int)m_textureList.size()<<endl;
        cout<<"--------------------"<<endl;
    }
     
    
};

#endif
