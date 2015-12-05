#pragma once
#include "core/c3dObject.h"
class Cc3dImage : public Cc3dObject
{
public:
	Cc3dImage(void);
	virtual ~Cc3dImage(void);
    
    /**
    @brief  Load the image from the specified path. 
    @param strPath   the absolute file path.
    @return  true if loaded correctly.
    */
    bool initWithImageFile(const char * strPath);

    /**
    @brief  Load image from stream buffer.

    @warning kFmtRawData only supports RGBA8888.
    @param pBuffer  stream buffer which holds the image data.
    @param nLength  data length expressed in (number of) bytes.
    @param nWidth, nHeight, nBitsPerComponent are used for kFmtRawData.
    @return true if loaded correctly.
    */
    bool initWithImageData(void * pData, 
                           int nDataLen);


    unsigned char *   getData()               { return m_pData; }
    int               getDataLen()            { return m_nWidth * m_nHeight; }

	bool hasAlpha()                     { return m_bHasAlpha;   }
    bool isPremultipliedAlpha()         { return m_bPreMulti;   }

	unsigned short getWidth()const {return m_nWidth;}
	unsigned short getHeight()const {return m_nHeight;}

	int getBitsPerComponent()const {return m_nBitsPerComponent;}


protected:
    bool _initWithPngData(void *pData, int nDatalen);
//    // @warning kFmtRawData only support RGBA8888
//    bool _initWithRawData(void *pData, int nDatalen, int nWidth, int nHeight, int nBitsPerComponent, bool bPreMulti);

protected:
	unsigned short m_nWidth;
	unsigned short m_nHeight;
    unsigned char *m_pData;
	int m_nBitsPerComponent;
    bool m_bHasAlpha;
    bool m_bPreMulti;

};

/*
    typedef enum
    {
        kFmtJpg = 0,
        kFmtPng,
        kFmtTiff,
        kFmtWebp,
        kFmtRawData,
        kFmtUnKnown
    }EImageFormat;

    typedef enum
    {
        kAlignCenter        = 0x33, ///< Horizontal center and vertical center.
        kAlignTop           = 0x13, ///< Horizontal center and vertical top.
        kAlignTopRight      = 0x12, ///< Horizontal right and vertical top.
        kAlignRight         = 0x32, ///< Horizontal right and vertical center.
        kAlignBottomRight   = 0x22, ///< Horizontal right and vertical bottom.
        kAlignBottom        = 0x23, ///< Horizontal center and vertical bottom.
        kAlignBottomLeft    = 0x21, ///< Horizontal left and vertical bottom.
        kAlignLeft          = 0x31, ///< Horizontal left and vertical center.
        kAlignTopLeft       = 0x11, ///< Horizontal left and vertical top.
    }ETextAlign;
	*/