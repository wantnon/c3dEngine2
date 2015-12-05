//
//  c3dProgramPlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-9.
//
//

#include "c3dProgramPlatformDepend.h"
#include "c3dGL.h"
GLuint createShader_plat(const char*shaderName,const char*ext)
{
    //转化成NSString
    NSString *shaderName_NS=[[[NSString alloc] initWithUTF8String:shaderName] autorelease];
    NSString *ext_NS=[[[NSString alloc] initWithUTF8String:ext] autorelease];
    //包括加载和编译
    // 0
    GLenum shaderType;
    if([ext_NS isEqualToString:@"vert"]==YES){
        shaderType=GL_VERTEX_SHADER;
    }else if([ext_NS isEqualToString:@"frag"]==YES){
        shaderType=GL_FRAGMENT_SHADER;
    }else{
        cout<<"无效的后缀名! "<<ext<<endl;
        exit(0);
    }
    // 1
    NSString* shaderPath = [[NSBundle mainBundle] pathForResource:shaderName_NS ofType:ext_NS];
    //   cout<<[shaderPath UTF8String]<<endl;
    NSError* error;
    NSString* shaderString = [NSString stringWithContentsOfFile:shaderPath encoding:NSUTF8StringEncoding error:&error];
    if (!shaderString) {
        NSLog(@"Error loading shader: %@", error.localizedDescription);
        cout<<shaderName<<" "<<ext<<endl;
        assert(false);
    }
    // 2
    GLuint shaderHandle = glCreateShader(shaderType);
    // 3
    const char * shaderStringUTF8 = [shaderString UTF8String];
    int shaderStringLength = [shaderString length];
    glShaderSource(shaderHandle, 1, &shaderStringUTF8, &shaderStringLength );
    // 4
    glCompileShader(shaderHandle);
    // 5
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"%@", messageString);
        exit(1);
    }
    return shaderHandle;
}
