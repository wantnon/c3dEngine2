#include "c3dImage.h"
#include "libpng/png.h"
#include "core/c3dFileUtils.h"
// premultiply alpha, or the effect will wrong when want to use other pixel format in CCTexture2D,
// such as RGB888, RGB5A1
#define CC_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))


typedef struct 
{
    unsigned char* data;
    int size;
    int offset;
}tImageSource;
static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
    tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);

    if((int)(isource->offset + length) <= isource->size)
    {
        memcpy(data, isource->data+isource->offset, length);
        isource->offset += length;
    }
    else
    {
        png_error(png_ptr, "pngReaderCallback failed");
    }
}

//-----------------------------------------------------------------------------------------------------
Cc3dImage::Cc3dImage(void)
: m_nWidth(0)
, m_nHeight(0)
, m_nBitsPerComponent(0)
, m_pData(0)
, m_bHasAlpha(false)
, m_bPreMulti(false)
{
}


Cc3dImage::~Cc3dImage(void)
{
	if(m_pData) { 
		delete[] (m_pData); 
		m_pData = NULL; 
	}
}


bool Cc3dImage::initWithImageFile(const char * strPath)
{
    bool bRet = false;

    unsigned long nSize = 0;
    std::string fullPath = Cc3dFileUtils::sharedFileUtils()->getFullPath(strPath);
    unsigned char* pBuffer = Cc3dFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "rb", &nSize);
    if (pBuffer != NULL && nSize > 0)
    {
        bRet = initWithImageData(pBuffer, nSize);
    }
	if(pBuffer) {
		delete[] (pBuffer); 
		pBuffer = NULL; 
	}
    return bRet;
}


bool Cc3dImage::initWithImageData(void * pData, 
                                int nDataLen)
{
    bool bRet = false;
    do 
    {
		if(! pData || nDataLen <= 0)break;

		
            bRet = _initWithPngData(pData, nDataLen);
            break;

    } while (0);
    return bRet;
}

bool Cc3dImage::_initWithPngData(void * pData, int nDatalen)
{
// length of bytes to check if it is a valid png file
#define PNGSIGSIZE  8
    bool bRet = false;
    png_byte        header[PNGSIGSIZE]   = {0}; 
    png_structp     png_ptr     =   0;
    png_infop       info_ptr    = 0;

    do 
    {
        // png header len is 8 bytes
        if(nDatalen < PNGSIGSIZE) break;

        // check the data is png or not
        memcpy(header, pData, PNGSIGSIZE);
        if(png_sig_cmp(header, 0, PNGSIGSIZE)) break;

        // init png_struct
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        if(! png_ptr) break;

        // init png_info
        info_ptr = png_create_info_struct(png_ptr);
        if(!info_ptr) break;


        // set the read call back function
        tImageSource imageSource;
        imageSource.data    = (unsigned char*)pData;
        imageSource.size    = nDatalen;
        imageSource.offset  = 0;
        png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

        // read png header info
        
        // read png file info
        png_read_info(png_ptr, info_ptr);
        
        m_nWidth = png_get_image_width(png_ptr, info_ptr);
        m_nHeight = png_get_image_height(png_ptr, info_ptr);
        m_nBitsPerComponent = png_get_bit_depth(png_ptr, info_ptr);
        png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

        //CCLOG("color type %u", color_type);
        
        // force palette images to be expanded to 24-bit RGB
        // it may include alpha channel
        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(png_ptr);
        }
        // low-bit-depth grayscale images are to be expanded to 8 bits
        if (color_type == PNG_COLOR_TYPE_GRAY && m_nBitsPerComponent < 8)
        {
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        // expand any tRNS chunk data into a full alpha channel
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(png_ptr);
        }  
        // reduce images with 16-bit samples to 8 bits
        if (m_nBitsPerComponent == 16)
        {
            png_set_strip_16(png_ptr);            
        } 
        // expand grayscale images to RGB
        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        {
            png_set_gray_to_rgb(png_ptr);
        }

        // read png data
        // m_nBitsPerComponent will always be 8
        m_nBitsPerComponent = 8;
        png_uint_32 rowbytes;
        png_bytep* row_pointers = (png_bytep*)malloc( sizeof(png_bytep) * m_nHeight );
        
        png_read_update_info(png_ptr, info_ptr);
        
        rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        
        m_pData = new unsigned char[rowbytes * m_nHeight];
        if(!m_pData) break;
        
        for (unsigned short i = 0; i < m_nHeight; ++i)
        {
            row_pointers[i] = m_pData + i*rowbytes;
        }
        png_read_image(png_ptr, row_pointers);
        
        png_read_end(png_ptr, NULL);
        
        png_uint_32 channel = rowbytes/m_nWidth;
        if (channel == 4)
        {
            m_bHasAlpha = true;
            unsigned int *tmp = (unsigned int *)m_pData;
            for(unsigned short i = 0; i < m_nHeight; i++)
            {
                for(unsigned int j = 0; j < rowbytes; j += 4)
                {
                    *tmp++ = CC_RGB_PREMULTIPLY_ALPHA( row_pointers[i][j], row_pointers[i][j + 1], 
                                                      row_pointers[i][j + 2], row_pointers[i][j + 3] );
                }
            }
            
            m_bPreMulti = true;
        }

		do { if(row_pointers) { free(row_pointers); (row_pointers) = 0; } } while(0);

        bRet = true;
    } while (0);

    if (png_ptr)
    {
        png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
    }
    return bRet;
}
