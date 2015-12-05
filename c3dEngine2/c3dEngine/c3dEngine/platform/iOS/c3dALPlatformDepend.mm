//
//  c3dALPlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-6.
//
//

#include "c3dALPlatformDepend.h"
void initBuffer_plat(const string&fileNameFull,void*&data,ALuint&buffer)
{
    ALenum  error = AL_NO_ERROR;
    ALenum  format=0;
    ALsizei size=0;
    ALsizei freq=0;
    
    // get some audio data from a wave file
    NSString *fileNameFull_NS=[[[NSString alloc] initWithUTF8String:fileNameFull.c_str()] autorelease];
    CFURLRef fileURL = (CFURLRef)[[NSURL fileURLWithPath:fileNameFull_NS] retain];
    if (fileURL)
    {
        data = MyGetOpenALAudioData(fileURL, &size, &format, &freq);
        NSLog(@"audioInfo:%d,%d,%d",size,format,freq);
        CFRelease(fileURL);
        
        if((error = alGetError()) != AL_NO_ERROR) {
            printf("warning: error loading sound: %x\n", error);
        }
        
        // use the static buffer data API
        alBufferDataStaticProc(buffer, format, data, size, freq);
        
        if((error = alGetError()) != AL_NO_ERROR) {
            printf("warning: error attaching audio to buffer: %x\n", error);
        }
    }
    else
    {
        printf("Could not find file!\n");
        data=NULL;
        assert(false);
    }
}
void initOpenAL_plat(){
    
    ALCcontext		*newContext = NULL;
    ALCdevice		*newDevice = NULL;
    
    // Create a new OpenAL Device
    // Pass NULL to specify the system’s default output device
    newDevice = alcOpenDevice(NULL);
    if (newDevice != NULL)
    {
        // Create a new OpenAL Context
        // The new context will render to the OpenAL Device just created
        newContext = alcCreateContext(newDevice, 0);
        if (newContext != NULL)
        {
            // Make the new context the Current OpenAL Context
            alcMakeContextCurrent(newContext);
            
        }else{
            cout<<"error:newContext==NULL!"<<endl;
            assert(false);
        }
    }else{
        cout<<"error:newDevice==NULL!"<<endl;
        //http://stackoverflow.com/questions/27443311/ios-simulator-does-not-play-sound
        cout<<"please test in real device!"<<endl;
       // assert(false);
    }
    // clear any errors
    alGetError();

}
void teardownOpenAL_plat(){
    TeardownOpenAL();
}
