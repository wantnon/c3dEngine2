//
//  c3dTexturePlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-9.
//
//

#include "c3dTexturePlatformDepend.h"
GLuint createGLTexture_plat(const string&filePath,int wrapS,int wrapT,GLint minFilter,GLint magFilter,float&textureWidth,float&textureHeight)

{
    // 1
    NSString *fileName_NS=[[[NSString alloc] initWithUTF8String:filePath.c_str()] autorelease];
    CGImageRef spriteImage = [UIImage imageNamed:fileName_NS].CGImage;
    if (!spriteImage) {
        NSLog(@"Failed to load image %@", fileName_NS);
        exit(1);
    }
    
    // 2
    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);
    
    textureWidth=width;
    textureHeight=height;
    
    
    
    GLubyte * spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    // 3
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    
    CGContextRelease(spriteContext);
    
    // 4
    GLuint texture;
    glGenTextures(1, &texture);
    assert(texture!=0);
    glBindTexture(GL_TEXTURE_2D, texture);
   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    glGenerateMipmap(GL_TEXTURE_2D);
    //data can be released
    free(spriteData);
    //
    C3DCHECK_GL_ERROR_DEBUG();
    return texture;
    
}
unsigned char* getImageData_plat(const string&filePath,CCTexture2DPixelFormat&_pixelFormat,float&_imageWidth,float&_imageHeight){
    // 1
    NSString *fileName_NS=[[[NSString alloc] initWithUTF8String:filePath.c_str()] autorelease];
    CGImageRef spriteImage = [UIImage imageNamed:fileName_NS].CGImage;
    if (!spriteImage) {
        NSLog(@"Failed to load image %@", fileName_NS);
        exit(1);
    }
    
    // 2
    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);
    
    
    
    GLubyte * spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    // 3
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    
    CGContextRelease(spriteContext);
    
    //
    _pixelFormat=kCCTexture2DPixelFormat_RGBA8888;
    _imageWidth=width;
    _imageHeight=height;
    assert(sizeof(GLubyte)==sizeof(unsigned char));
    return spriteData;

}