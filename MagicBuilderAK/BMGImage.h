#ifndef _BMG_IMAGE_H_
#define _BMG_IMAGE_H_
/*
//  header file for the BMGImage functions
//
//  Copyright 2000, 2001 M. Scott Heiman
*/

#include <windows.h>
#include <stdio.h>

#ifdef BUILD_BMG_DLL
#	define BMG_EXPORT __declspec( dllexport )
#else
#	define BMG_EXPORT
#endif

enum BMG_Error 
{ 
	BMG_OK = 0,
    errLib = 1,
    errInvalidPixelFormat = 2,
    errMemoryAllocation = 3,
	errInvalidSize = 4,
    errInvalidBitmapHandle = 5,
    errWindowsAPI = 6,
	errFileOpen = 7,
    errUnsupportedFileFormat = 8,
    errInvalidBMGImage = 9,
	errInvalidFileExtension = 10,
    errFileRead = 11,
    errFileWrite = 12,
	errInvalidGeoTIFFPointer = 13,
    errUndefinedBGImage = 14,
    errBGImageTooSmall = 15,
	errCorruptFile = 16
};

typedef enum BMG_Error BMGError;

#pragma pack(push,1)
struct BMGImageStruct
{
    unsigned int width;
    unsigned int height;
    unsigned char bits_per_pixel;
    unsigned char *bits;
    unsigned short palette_size;
    unsigned char bytes_per_palette_entry;
    unsigned char *palette;
    unsigned int scan_width;
    int opt_for_bmp; /*= 1 if memory has been sized for HBITMAP, 0 otherwise*/
    short transparency_index;
};
#pragma pack(pop)

#if defined(__cplusplus)
extern "C" {
#endif

/* initializes a BMGImage to default values */
extern
void BMG_EXPORT InitBMGImage( struct BMGImageStruct *img );

/* frees memory allocated to a BMGImage */
extern
void BMG_EXPORT FreeBMGImage( struct BMGImageStruct *img );

/* allocates memory (bits & palette) for a BMGImage.
   returns 1 if successfull, 0 otherwise.
   width, height, bits_per_pixel, palette_size, & opt_for_bmp must have valid
   values before this function is called.
   Assumes that all images with bits_per_pixel <= 8 requires a palette.
   will set bits_per_palette_entry, scan_width, bits, & palette */
extern
BMGError BMG_EXPORT AllocateBMGImage( struct BMGImageStruct *img );

/* compresses 8 BPP paletted images to 1 BPP or 4 BPP paletted images if
   possible */
extern
BMGError BMG_EXPORT CompressBMGImage( struct BMGImageStruct *img );


/* a utility function for freeing memory created in BMGLib */
extern
void BMG_EXPORT FreeBMGMemory( unsigned char *mem );

/* converts a color image to a gray scale image */
extern
BMGError BMG_EXPORT ConvertToGrayScale( struct BMGImageStruct *img );

/* converts a color image to a pseudo-gray scale image */
extern
BMGError BMG_EXPORT ConvertToPseudoGrayScale( struct BMGImageStruct *img );

/* stores the contents of a bitmap into a BMGImageStruct */
extern
BMGError BMG_EXPORT GetDataFromBitmap( HBITMAP hBitmap,
					              struct BMGImageStruct *img,
                                  int remove_alpha );

/* creates an HBITMAP from a BMGImageStruct */
extern
HBITMAP BMG_EXPORT CreateBitmapFromData( struct BMGImageStruct img,
                                         int alpha_blend );

/* sets the background color for alpha blending
  color points to an array of 4 unsigned chars
  color[0] = blue, color[1] = green, color[2] = red, color[3] = unused */
extern
void BMG_EXPORT SetBMGBackgroundColor( unsigned char *color );

/* defines the background bitmap that is used for alpha blending & transparent
   pixels */
extern
BMGError BMG_EXPORT SetBMGBackgroundBitmap( HBITMAP hBitmap );

/* defines the background image that is used for alpha blending & transparent
   pixels */
extern
BMGError BMG_EXPORT SetBMGBackgroundImage( struct BMGImageStruct img );

/* Converts paletted images and 16-BPP images to 24-BPP images */
extern
BMGError BMG_EXPORT ConvertPaletteToRGB( struct BMGImageStruct img_in,
                                         struct BMGImageStruct *img_out );

/* copies the contents of the input image into the output image */
extern
BMGError BMG_EXPORT CopyBMGImage( struct BMGImageStruct img_in,
                                  struct BMGImageStruct *img_out );

/* returns the last error state */
extern
BMGError BMG_EXPORT GetLastBMGError();

/* gets the error message */
extern
void BMG_EXPORT GetLastBMGErrorMessage( const char **msg );

#if defined(__cplusplus)
 }
#endif

#endif