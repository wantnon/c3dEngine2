//
//  c3dDocuments.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#include "c3dDocuments.h"
static Cc3dDocuments*s_documents=NULL;
Cc3dDocuments*Cc3dDocuments::sharedDocuments(){
    if(s_documents==NULL){
        s_documents=new Cc3dDocuments();
    }
    return s_documents;
}
string Cc3dDocuments::getDocumentsPath(){
    return getDocumentsPath_plat();
}