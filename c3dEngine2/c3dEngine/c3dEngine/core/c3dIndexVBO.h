//
//  Cc3dIndexVBO.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-16.
//
//

#ifndef __HelloOpenGL__Cc3dIndexVBO__
#define __HelloOpenGL__Cc3dIndexVBO__

#include <iostream>
#include <vector>
using namespace std;
#include "c3dTransform.h"
#include "c3dCamera.h"
#include "c3dLight.h"
#include "c3dVertex.h"
#include "c3dIDtriangle.h"
#include "c3dIDline.h"
class Cc3dIndexVBO:public Cc3dObject
{
public:
    GLuint vertexbuffer;
    GLuint indexbuffer;
    int m_vertexCount;
    int m_indexCount;
	bool m_isWireMode;
    Cc3dIndexVBO(){
        vertexbuffer=0;
        indexbuffer=0;
        m_vertexCount=0;
        m_indexCount=0;
		m_isWireMode=false;
    }
    virtual~ Cc3dIndexVBO(){
        if(vertexbuffer!=0)glDeleteBuffers(1,&vertexbuffer);
        if(indexbuffer!=0)glDeleteBuffers(1,&indexbuffer);
    }
	void setIsWireMode(bool isWireMode){
		m_isWireMode=isWireMode;
	}
	bool getIsWireMode()const{return m_isWireMode;}
    void genBuffers(){
        assert(vertexbuffer==0);
        assert(indexbuffer==0);
        glGenBuffers(1,&vertexbuffer);
        glGenBuffers(1,&indexbuffer);
    }

   void submitVertex(const vector<Cc3dVertex> &vlist,GLenum usage)
    {
        int vertexCount=(int)vlist.size();
		m_vertexCount=vertexCount;
		if(m_vertexCount==0)return;
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertexCount*(4+2+4+4+2), &vlist[0], usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        
    }
 
    void submitIndex(const vector<Cc3dIDTriangle> &IDtriList,GLenum usage)
    {
		if(m_isWireMode==false){
			int nIDtri=(int)IDtriList.size();
			int indexCount=3*nIDtri;
			m_indexCount=indexCount;
			if(m_indexCount==0)return;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*m_indexCount, &IDtriList[0], usage);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}else{//m_isWireMode
			//convert IDtriList to IDlineList
			vector<Cc3dIDline> IDlineList;
			int nIDtri=(int)IDtriList.size();
			for(int i=0;i<nIDtri;i++){
				const Cc3dIDTriangle&IDtri=IDtriList[i];
				int ID0=IDtri.vID[0];
				int ID1=IDtri.vID[1];
				int ID2=IDtri.vID[2];
				Cc3dIDline IDline0,IDline1,IDline2;
				IDline0.setvID(ID0,ID1);
				IDline1.setvID(ID1,ID2);
				IDline2.setvID(ID2,ID0);
				IDlineList.push_back(IDline0);
				IDlineList.push_back(IDline1);
				IDlineList.push_back(IDline2);
			}//got IDlineList
			int nIDline=(int)IDlineList.size();
			int indexCount=2*nIDline;
			m_indexCount=indexCount;
			if(m_indexCount==0)return;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*m_indexCount, &IDlineList[0], usage);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		}
        
    }
    void bindVertexBuffer(){
  
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    }
    void unbindVertexBuffer(){
   
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

        
    static void setPointers()
	//specify each attribute's format, must put after bindVertexBuffer
    {
        const int posStep=4;
        const int texCoordStep=2;
        const int normStep=4;
		const int colorStep=4;
        const int texCoord2Step=2;
        const int step=posStep+texCoordStep+normStep+colorStep+texCoord2Step;
        glVertexAttribPointer(ATTRIB_LOC_position, posStep, GL_FLOAT, GL_FALSE, sizeof(float)*step,0);
        glVertexAttribPointer(ATTRIB_LOC_texCoord, texCoordStep, GL_FLOAT, GL_FALSE,sizeof(float)*step, (GLvoid*) (sizeof(float)* posStep));
        glVertexAttribPointer(ATTRIB_LOC_normal, normStep, GL_FLOAT, GL_FALSE, sizeof(float)*step, (GLvoid*) (sizeof(float) * (posStep+texCoordStep)));
		glVertexAttribPointer(ATTRIB_LOC_color, colorStep, GL_FLOAT, GL_FALSE, sizeof(float)*step, (GLvoid*) (sizeof(float) * (posStep+texCoordStep+normStep)));
        glVertexAttribPointer(ATTRIB_LOC_texCoord2, texCoord2Step, GL_FLOAT, GL_FALSE, sizeof(float)*step, (GLvoid*) (sizeof(float) * (posStep+texCoordStep+normStep+colorStep)));
        
    }


    void bindIndexBuffer(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    }
    void unbindIndexBuffer(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
	void drawIndexBuffer(){
		if(m_isWireMode){
			drawIndexBuffer(GL_LINES);
		}else{
			drawIndexBuffer(GL_TRIANGLES);
		}
	}
private:
    void drawIndexBuffer(GLenum mode)
    {
		
         //draw index buffer
        if(m_indexCount!=0){
            assert(m_vertexCount!=0);
            //if m_indexCount!=0 but m_vertexCount==0, may means forget to submit before draw
        }
		//cout<<"m_indexCount:"<<m_indexCount<<endl;
        glDrawElements(mode,m_indexCount,GL_UNSIGNED_INT, 0);
        //the last parameter must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
		//even the index type is int, we should use GL_UNSIGNED_INT
        //the first parameter is a symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES,
        //GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
        //GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY
        //and GL_PATCHES are accepted.
        //note there is no GL_QUADS, but have a GL_PATCHES

    }

     
    
};

#endif /* defined(__HelloOpenGL__Cc3dIndexVBO__) */
