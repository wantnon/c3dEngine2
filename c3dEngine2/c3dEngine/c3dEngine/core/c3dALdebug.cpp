//
//  c3dALdebug.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-7.
//
//

#include "c3dALdebug.h"
string errorTranslate(ALenum error){
    string errorStr;
    //ref to OpenAL 1.1 Specification (page 16)
    switch(error){
        case AL_NO_ERROR:
            errorStr="AL_NO_ERROR";
        case AL_INVALID_NAME:
            errorStr="AL_INVALID_NAME";
            break;
        case AL_INVALID_ENUM:
            errorStr="AL_INVALID_ENUM";
            break;
        case AL_INVALID_VALUE:
            errorStr="AL_INVALID_VALUE";
            break;
        case AL_INVALID_OPERATION:
            errorStr="AL_INVALID_OPERATION";
            break;
        case AL_OUT_OF_MEMORY:
            errorStr="AL_OUT_OF_MEMORY";
            break;
        default:
            errorStr="???";//unknow error
            //if the error is 0xFFFFFFFF, may means too many source play at the same time. (i guess)
            break;
            
    }
    return errorStr;
}