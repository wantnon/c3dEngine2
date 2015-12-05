//
//  c3dALPlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-6.
//
//

#include "platform/win32/c3dALPlatformDepend.h"
#include "core/c3dAssert.h"
#include "core/c3dALdebug.h"

void initBuffer_plat(const string&fileNameFull,ALuint&buffer){
	// Variables to load into.

	ALenum format;
	ALsizei size;
	ALsizei freq;
	ALboolean loop;

	// Load wav data into buffers.

	alGenBuffers(1, &buffer);

	C3DCHECK_AL_ERROR_DEBUG();
	void* data=NULL;
	alutLoadWAVFile((ALbyte *)fileNameFull.c_str(), &format, &data, &size, &freq, &loop);
	alBufferData(buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	C3DCHECK_AL_ERROR_DEBUG();
}
void initBuffer_plat(const string&fileNameFull,void*&data,ALuint&buffer)
{
	//on windows, there is no need for data
	initBuffer_plat(fileNameFull,buffer);
	

}

void initOpenAL_plat(){
	//initialize OpenAL and clear the error bit.
	alutInit(NULL, 0);
	alGetError();//clear error bit
 
}
 void teardownOpenAL_plat(){
	alutExit();
 }