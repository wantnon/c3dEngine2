//
//  c3dALBuffer.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-23.
//
//

#include "c3dALBuffer.h"

void Cc3dALBuffer::initBuffer(const string&filePath){
    m_filePath=filePath;
    string fileNameFull=Cc3dFileUtils::sharedFileUtils()->getFullPath(m_filePath);
    assert(m_buffer==0);
    alGenBuffers(1, &m_buffer);
    C3DCHECK_AL_ERROR_DEBUG();
    initBuffer(fileNameFull,m_data,m_buffer);
}
void Cc3dALBuffer::initBuffer(const string&fileNameFull,void*&data,ALuint&buffer)
{
    initBuffer_plat(fileNameFull,data,buffer);
}

