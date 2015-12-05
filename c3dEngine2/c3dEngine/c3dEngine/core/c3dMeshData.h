//
//  c3dMeshData.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-25.
//
//

#ifndef __HelloOpenGL__c3dMeshData__
#define __HelloOpenGL__c3dMeshData__

#include <iostream>
#include <vector>
using namespace std;
#include "c3dIDtriangle.h"
#include "c3dVertex.h"
#include "c3dObject.h"
class Cc3dSubMeshData:public Cc3dObject{
protected:
    vector<Cc3dVertex> vlist; 
    vector<Cc3dIDTriangle> IDtriList;
public:
    void addVertex(const Cc3dVertex&vertex);
	void addIDtri(const Cc3dIDTriangle&IDtri);
	Cc3dVertex getVertexByIndex(int index)const {
		assert(index>=0&&index<(int)vlist.size());
		return vlist[index];
	}
	Cc3dIDTriangle getIDtriByIndex(int index)const {
		assert(index>=0&&index<(int)IDtriList.size());
		return IDtriList[index];
	}
	void setVertexByIndex(int index,const Cc3dVertex&vertex){
		assert(index>=0&&index<(int)vlist.size());
		vlist[index]=vertex;
	}
	const vector<Cc3dVertex>&getvlist(){return vlist;}
	const vector<Cc3dIDTriangle>&getIDtriList(){return IDtriList;}
	int getVertexCount()const{return (int)vlist.size();}
	int getIDtriCount()const{return (int)IDtriList.size();}
	void clearIDtriList(){IDtriList.clear();}
	void clearvlist(){vlist.clear();}
    void clear(){
		clearvlist();
		clearIDtriList();
    }
	void swapReleaseIDtriList(){
		vector<Cc3dIDTriangle>().swap(IDtriList);
	}
	void swapReleasevlist(){
		vector<Cc3dVertex>().swap(vlist);
	}
	void swapRelease(){
		swapReleaseIDtriList();
		swapReleasevlist();
	}
	void reserveIDtriList(int len){
		IDtriList.reserve(len);
	}
	void reservevlist(int len){
		vlist.reserve(len);
	}
    bool isEmpty(){ return (vlist.empty()&&IDtriList.empty());}
};
#endif /* defined(__HelloOpenGL__c3dMeshData__) */
