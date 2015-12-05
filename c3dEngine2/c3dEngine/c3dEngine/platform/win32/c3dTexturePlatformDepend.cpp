//
//  c3dTexturePlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-9.
//
//

#include "platform/win32/c3dTexturePlatformDepend.h"
#include "core/c3dAssert.h"
#if (C3D_TARGET_PLATFORM == C3D_PLATFORM_WIN32) 
	#include "c3dImage.h"
#else
    
#endif

unsigned int bitsPerPixelForFormat(CCTexture2DPixelFormat format)
{
	unsigned int ret=0;

	switch (format) {
		case kCCTexture2DPixelFormat_RGBA8888:
			ret = 32;
			break;
		case kCCTexture2DPixelFormat_RGB888:
			// It is 32 and not 24, since its internal representation uses 32 bits.
			ret = 32;
			break;
		case kCCTexture2DPixelFormat_RGB565:
			ret = 16;
			break;
		case kCCTexture2DPixelFormat_RGBA4444:
			ret = 16;
			break;
		case kCCTexture2DPixelFormat_RGB5A1:
			ret = 16;
			break;
		case kCCTexture2DPixelFormat_AI88:
			ret = 16;
			break;
		case kCCTexture2DPixelFormat_A8:
			ret = 8;
			break;
		case kCCTexture2DPixelFormat_I8:
			ret = 8;
			break;
		case kCCTexture2DPixelFormat_PVRTC4:
			ret = 4;
			break;
		case kCCTexture2DPixelFormat_PVRTC2:
			ret = 2;
			break;
		default:
			ret = -1;
			C3DASSERT(false , "unrecognized pixel format");
			C3DLOG("bitsPerPixelForFormat: %ld, cannot give useful result", (long)format);
			break;
	}
	return ret;
}
GLuint createGLTexture_plat(const string&filePath,int wrapS,int wrapT,GLint minFilter,GLint magFilter,float&textureWidth,float&textureHeight)
{
	//cout<<"filePath:"<<filePath<<endl;
	Cc3dImage* pImage = NULL;
	pImage = new Cc3dImage();
    C3DASSERT(pImage);
    bool bRet = pImage->initWithImageFile(filePath.c_str());
    C3DASSERT(bRet);

    unsigned int imageWidth = pImage->getWidth();
    unsigned int imageHeight = pImage->getHeight();
	textureWidth=imageWidth;
    textureHeight=imageHeight;
    //cout<<"imageSize:"<<imageWidth<<" "<<imageHeight<<endl;
	unsigned char*            tempData = pImage->getData();
    unsigned int*             inPixel32  = NULL;
    unsigned char*            inPixel8 = NULL;
    unsigned short*           outPixel16 = NULL;
    bool                      hasAlpha = pImage->hasAlpha();
  //  CCSize                    imageSize = CCSizeMake((float)(image->getWidth()), (float)(image->getHeight()));
    CCTexture2DPixelFormat    pixelFormat;
    size_t                    bpp = pImage->getBitsPerComponent();
	 // compute pixel format
    if (hasAlpha)
    {
    	pixelFormat = kCCTexture2DPixelFormat_RGBA8888;
    }
    else
    {
        if (bpp >= 8)
        {
            pixelFormat = kCCTexture2DPixelFormat_RGB888;
        }
        else 
        {
            pixelFormat = kCCTexture2DPixelFormat_RGB565;
        }
        
    }
    
    // Repack the pixel data into the right format
    unsigned int length = imageWidth * imageHeight;

    if (pixelFormat == kCCTexture2DPixelFormat_RGB565)
    {
        if (hasAlpha)
        {
            // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
            
            tempData = new unsigned char[imageWidth * imageHeight * 2];
            outPixel16 = (unsigned short*)tempData;
            inPixel32 = (unsigned int*)pImage->getData();
            
            for(unsigned int i = 0; i < length; ++i, ++inPixel32)
            {
                *outPixel16++ = 
                ((((*inPixel32 >>  0) & 0xFF) >> 3) << 11) |  // R
                ((((*inPixel32 >>  8) & 0xFF) >> 2) << 5)  |  // G
                ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);    // B
            }
        }
        else 
        {
            // Convert "RRRRRRRRRGGGGGGGGBBBBBBBB" to "RRRRRGGGGGGBBBBB"
            
            tempData = new unsigned char[imageWidth * imageHeight * 2];
            outPixel16 = (unsigned short*)tempData;
            inPixel8 = (unsigned char*)pImage->getData();
            
            for(unsigned int i = 0; i < length; ++i)
            {
                *outPixel16++ = 
                (((*inPixel8++ & 0xFF) >> 3) << 11) |  // R
                (((*inPixel8++ & 0xFF) >> 2) << 5)  |  // G
                (((*inPixel8++ & 0xFF) >> 3) << 0);    // B
            }
        }    
    }
    else if (pixelFormat == kCCTexture2DPixelFormat_RGBA4444)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
        
        inPixel32 = (unsigned int*)pImage->getData();  
        tempData = new unsigned char[imageWidth * imageHeight * 2];
        outPixel16 = (unsigned short*)tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 4) <<  8) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 4) << 0);  // A
        }
    }
    else if (pixelFormat == kCCTexture2DPixelFormat_RGB5A1)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
        inPixel32 = (unsigned int*)pImage->getData();   
        tempData = new unsigned char[imageWidth * imageHeight * 2];
        outPixel16 = (unsigned short*)tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 3) <<  6) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 3) << 1) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 7) << 0);  // A
        }
    }
    else if (pixelFormat == kCCTexture2DPixelFormat_A8)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "AAAAAAAA"
        inPixel32 = (unsigned int*)pImage->getData();
        tempData = new unsigned char[imageWidth * imageHeight];
        unsigned char *outPixel8 = tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel8++ = (*inPixel32 >> 24) & 0xFF;  // A
        }
    }
    
    if (hasAlpha && pixelFormat == kCCTexture2DPixelFormat_RGB888)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRRRRGGGGGGGGBBBBBBBB"
        inPixel32 = (unsigned int*)pImage->getData();
        tempData = new unsigned char[imageWidth * imageHeight * 3];
        unsigned char *outPixel8 = tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel8++ = (*inPixel32 >> 0) & 0xFF; // R
            *outPixel8++ = (*inPixel32 >> 8) & 0xFF; // G
            *outPixel8++ = (*inPixel32 >> 16) & 0xFF; // B
        }
    }


	
    unsigned int bitsPerPixel;
    //Hack: bitsPerPixelForFormat returns wrong number for RGB_888 textures. See function.
    if(pixelFormat == kCCTexture2DPixelFormat_RGB888)
    {
        bitsPerPixel = 24;
    }
    else
    {
        bitsPerPixel = bitsPerPixelForFormat(pixelFormat);
    }

    unsigned int bytesPerRow = imageWidth * bitsPerPixel / 8;

    if(bytesPerRow % 8 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    }
    else if(bytesPerRow % 4 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
    else if(bytesPerRow % 2 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }
    
	
	 GLuint texture;
    glGenTextures(1, &texture);
    assert(texture!=0);
    glBindTexture(GL_TEXTURE_2D, texture);
    //no need to use mipmap, that is slow
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );
	if(pixelFormat == kCCTexture2DPixelFormat_RGB888){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)imageWidth, (GLsizei)imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tempData);
	}else if(pixelFormat == kCCTexture2DPixelFormat_RGBA8888){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)imageWidth, (GLsizei)imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempData);
	}else{
		C3DASSERT(false);
	}
	
	C3DCHECK_GL_ERROR_DEBUG();
	
	//tempData can be released
	if (tempData != pImage->getData())
    {
        delete [] tempData;
    }
	//image can be released
	pImage->release(); 


	

    return texture;

 
}



unsigned char*   getImageData_plat(const string&filePath,CCTexture2DPixelFormat&_pixelFormat,float&_imageWidth,float&_imageHeight)
	//the return object is autoreleased object
{
	//cout<<"filePath:"<<filePath<<endl;
	Cc3dImage* pImage = NULL;
	pImage = new Cc3dImage();
    C3DASSERT(pImage);
    bool bRet = pImage->initWithImageFile(filePath.c_str());
    C3DASSERT(bRet);

    unsigned int imageWidth = pImage->getWidth();
    unsigned int imageHeight = pImage->getHeight();

    //cout<<"imageSize:"<<imageWidth<<" "<<imageHeight<<endl;
	unsigned char*            tempData = pImage->getData();
    unsigned int*             inPixel32  = NULL;
    unsigned char*            inPixel8 = NULL;
    unsigned short*           outPixel16 = NULL;
    bool                      hasAlpha = pImage->hasAlpha();
  //  CCSize                    imageSize = CCSizeMake((float)(image->getWidth()), (float)(image->getHeight()));
    CCTexture2DPixelFormat    pixelFormat;
    size_t                    bpp = pImage->getBitsPerComponent();
	 // compute pixel format
    if (hasAlpha)
    {
    	pixelFormat = kCCTexture2DPixelFormat_RGBA8888;
    }
    else
    {
        if (bpp >= 8)
        {
            pixelFormat = kCCTexture2DPixelFormat_RGB888;
        }
        else 
        {
            pixelFormat = kCCTexture2DPixelFormat_RGB565;
        }
        
    }
    
    // Repack the pixel data into the right format
    unsigned int length = imageWidth * imageHeight;

    if (pixelFormat == kCCTexture2DPixelFormat_RGB565)
    {
        if (hasAlpha)
        {
            // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
            
            tempData = new unsigned char[imageWidth * imageHeight * 2];
            outPixel16 = (unsigned short*)tempData;
            inPixel32 = (unsigned int*)pImage->getData();
            
            for(unsigned int i = 0; i < length; ++i, ++inPixel32)
            {
                *outPixel16++ = 
                ((((*inPixel32 >>  0) & 0xFF) >> 3) << 11) |  // R
                ((((*inPixel32 >>  8) & 0xFF) >> 2) << 5)  |  // G
                ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);    // B
            }
        }
        else 
        {
            // Convert "RRRRRRRRRGGGGGGGGBBBBBBBB" to "RRRRRGGGGGGBBBBB"
            
            tempData = new unsigned char[imageWidth * imageHeight * 2];
            outPixel16 = (unsigned short*)tempData;
            inPixel8 = (unsigned char*)pImage->getData();
            
            for(unsigned int i = 0; i < length; ++i)
            {
                *outPixel16++ = 
                (((*inPixel8++ & 0xFF) >> 3) << 11) |  // R
                (((*inPixel8++ & 0xFF) >> 2) << 5)  |  // G
                (((*inPixel8++ & 0xFF) >> 3) << 0);    // B
            }
        }    
    }
    else if (pixelFormat == kCCTexture2DPixelFormat_RGBA4444)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
        
        inPixel32 = (unsigned int*)pImage->getData();  
        tempData = new unsigned char[imageWidth * imageHeight * 2];
        outPixel16 = (unsigned short*)tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 4) <<  8) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 4) << 0);  // A
        }
    }
    else if (pixelFormat == kCCTexture2DPixelFormat_RGB5A1)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
        inPixel32 = (unsigned int*)pImage->getData();   
        tempData = new unsigned char[imageWidth * imageHeight * 2];
        outPixel16 = (unsigned short*)tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 3) <<  6) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 3) << 1) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 7) << 0);  // A
        }
    }
    else if (pixelFormat == kCCTexture2DPixelFormat_A8)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "AAAAAAAA"
        inPixel32 = (unsigned int*)pImage->getData();
        tempData = new unsigned char[imageWidth * imageHeight];
        unsigned char *outPixel8 = tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel8++ = (*inPixel32 >> 24) & 0xFF;  // A
        }
    }
    
    if (hasAlpha && pixelFormat == kCCTexture2DPixelFormat_RGB888)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRRRRGGGGGGGGBBBBBBBB"
        inPixel32 = (unsigned int*)pImage->getData();
        tempData = new unsigned char[imageWidth * imageHeight * 3];
        unsigned char *outPixel8 = tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel8++ = (*inPixel32 >> 0) & 0xFF; // R
            *outPixel8++ = (*inPixel32 >> 8) & 0xFF; // G
            *outPixel8++ = (*inPixel32 >> 16) & 0xFF; // B
        }
    }


	
    unsigned int bitsPerPixel;
    //Hack: bitsPerPixelForFormat returns wrong number for RGB_888 textures. See function.
    if(pixelFormat == kCCTexture2DPixelFormat_RGB888)
    {
        bitsPerPixel = 24;
    }
    else
    {
        bitsPerPixel = bitsPerPixelForFormat(pixelFormat);
    }

    unsigned int bytesPerRow = imageWidth * bitsPerPixel / 8;

    if(bytesPerRow % 8 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    }
    else if(bytesPerRow % 4 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
    else if(bytesPerRow % 2 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }
    
	//assgin
	_pixelFormat=pixelFormat;
	_imageWidth=imageWidth;
	_imageHeight=imageHeight;
	

	//if tempData is not pImage->getData, then release pImage and return tempData
	//else copy pImage->getData to a new buffer and release pImage data and return buffer

	//tempData can be released
	if (tempData != pImage->getData())
    {
        pImage->release(); 
		return tempData;
	}else{
		//it must be RGBA8888
		int pixelPerByte;
		if(pixelFormat == kCCTexture2DPixelFormat_RGBA8888){
			pixelPerByte=4;
		}else if(pixelFormat == kCCTexture2DPixelFormat_RGB888){
			pixelPerByte=3;
		}else{
			C3DASSERT(false);
		}
		tempData = new unsigned char[imageWidth * imageHeight * pixelPerByte];
		memcpy(tempData,pImage->getData(),imageWidth * imageHeight * pixelPerByte);
	//	cout<<"data size:"<<sizeof(pImage->getData())<<endl;
	//	cout<<"temp size:"<<sizeof(tempData)<<endl;
		pImage->release();
		return tempData;
	}

 
}
