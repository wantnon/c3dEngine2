//
//  c3dALSource.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-23.
//
//

#include "c3dALSource.h"
void Cc3dALSource::initSource(Cc3dALBuffer*buffer){
    
    assert(buffer);
    setBuffer(buffer);
    //  //记录buffer和isPermanent
    //  m_buffer=(Cc3dALBuffer*)pBuffer;
    //创建source
    ALenum			error;
    alGenSources(1, &m_source);
    if(alGetError() != AL_NO_ERROR)
    {
        printf("Error generating sources! %x\n", error);
		C3DCHECK_AL_ERROR_DEBUG();
    }
    //初始化source
    alSourcei(m_source, AL_BUFFER, buffer->getBuffer());
    alSourcef(m_source, AL_PITCH, 1.0f);
    alSourcef(m_source, AL_GAIN, 1.0f);
    alSourcei(m_source, AL_LOOPING, AL_FALSE);
    // 设定距离模型参数--abc
    alSourcef(m_source, AL_REFERENCE_DISTANCE, 30);
    alSourcef(m_source, AL_ROLLOFF_FACTOR,0.25);
}
