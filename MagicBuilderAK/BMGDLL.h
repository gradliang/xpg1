#ifndef _BMG_LIB_
#define _BMG_LIB_
/*
//  header file for the BMGLib DLL
//  This DLL encapsulates the libTIFF library, libJPEG library,
//  libPNG library, and the GeoTIFF library.
//
//  Copyright 2000, 2001 M. Scott Heiman
//  All Rights Reserved
//  libTIFF is Copyright Sam Leffler and SGI
//  libJPEG is Copyright (C) 1991-1998, Thomas G. Lane and is part of the
//      Independent JPEG Group's software.
//  libPNG is Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.
//    (libpng versions 0.5, May 1995, through 0.89c, May 1996)
//    Copyright (c) 1996, 1997 Andreas Dilger
//    (libpng versions 0.90, December 1996, through 0.96, May 1997)
//    Copyright (c) 1998, 1999 Glenn Randers-Pehrson
//    (libpng versions 0.97, January 1998, through 1.0.5, October 15, 1999)
//  zLib Copyright (C) 1995-1998 Jean-loup Gailly.
//  GeoTIFF is Copyright (c) 1999, Frank Warmerdam
//  libPROJ (used by GeoTIFF) is Copytight (c) 2000, Frank Warmerdam
//  libUnGif is Copyright (c) 1997,  Eric S. Raymond
*/

#ifdef BUILD_BMG_DLL
#   define BMG_EXPORT __declspec( dllexport )
#else
#   define BMG_EXPORT
#endif

#include "BMGImage.h"
#include "tiffrw.h"
#include "pngrw.h"
#include "jpegrw.h"
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* returns a BITMAPINFO structure with the given height, width,
  bit count, and compression scheme.  This structure DOES NOT contain any
  palette information (bmiColors = NULL) */
extern
BITMAPINFO BMG_EXPORT CreateBMI( DWORD dwWidth,      /* width in pixels */
                                 DWORD dwHeight,     /* height in pixels */
                                 WORD wBitCount,     /* 1, 4, 8, 16, 24, & 32 */
                                 int compression );  /* biCompression value */

/* returns an array of RGBA or BGRA values for all supported graphics file 
   formats.  The RGBA pixel format is supported by all versions of OpenGL.
   The BGRA format is an extension supported by may OpenGL vendors. */
extern
BMGError BMG_EXPORT GetUnpackedArray( const char *filename,
                                 unsigned int *width,
                                 unsigned int *height,
                                 unsigned char **bits,
                                 int bgra );

/* Saves an array of RGB, RGBA, BGR, and BGRA values to a file.  The RGB and RGBA 
   pixel formats are supported by OpenGL.  The BGR and BGRA extensions are
   supported by many OpenGL vendors */
extern
BMGError BMG_EXPORT SaveUnpackedArray( const char *filename,
                                  unsigned char bytes_per_pixel,
                                  unsigned int width,
                                  unsigned int height,
                                  unsigned char *bits,
								  int bgra );

/* saves the contents of an HBITMAP to a file.  The extension of the file name
// determines the file type.  returns 1 if successfull, 0 otherwise */
extern
BMGError BMG_EXPORT SaveBitmapToFile( HBITMAP hBitmap,      /* bitmap to be saved */
                                 const char *filename, /* name of output file */
                                 void *parameters );

/* Creates an HBITMAP to an image file.  The extension of the file name
// determines the file type.  returns an HBITMAP if successfull, NULL
// otherwise */
extern
HBITMAP BMG_EXPORT CreateBitmapFromFile( const char *filename,
                                         void *parameters,
                                         int blend );

/* extracts a BMGImageStruct from any one of the supported image files */
extern 
BMGError BMG_EXPORT GetDataFromFile( const char *filename,
									 struct BMGImageStruct *img,
									 void *parameters );

/* saves the contents of an HBITMAP to a file.  The extension of the file name
// determines the file type.  returns 1 if successfull, 0 otherwise */
extern
BMGError BMG_EXPORT SaveBitmapToJpegStream( HBITMAP hBitmap,      /* bitmap to be saved */
                                 FILE *outfile, /* name of output file */
                                 void *parameters );

/* Creates an HBITMAP to an image file.  The extension of the file name
// determines the file type.  returns an HBITMAP if successfull, NULL
// otherwise */
extern
HBITMAP BMG_EXPORT CreateBitmapFromJpegStream( FILE *infile,
                                         void *parameters,
                                         int blend );

/* the following functions will read/write image files using raw data */
extern
BMGError BMG_EXPORT ReadRGB( const char *filename,
                        struct BMGImageStruct *img );

extern
BMGError BMG_EXPORT WriteRGB( const char *filename,
                         struct BMGImageStruct img );

extern
BMGError BMG_EXPORT ReadTGA( const char *filename,
                        struct BMGImageStruct *img );

extern
BMGError BMG_EXPORT WriteTGA( const char *filename,
                        struct BMGImageStruct img );

extern
BMGError BMG_EXPORT ReadBMP( const char *filename,
                        struct BMGImageStruct *img );

extern
BMGError BMG_EXPORT WriteBMP( const char *filename,
                         struct BMGImageStruct img );

extern
BMGError BMG_EXPORT ReadCEL( const char *filename,
                        struct BMGImageStruct *img );

extern
BMGError BMG_EXPORT ReadGIF( const char *filename,
                        struct BMGImageStruct *img );

extern
BMGError BMG_EXPORT ReadPSD( const char *filename, 
						struct BMGImageStruct *img );

extern
BMGError BMG_EXPORT ReadIFF( const char *filename, 
						struct BMGImageStruct *img );

extern
BMGError BMG_EXPORT ReadPCX( const char *filename,
						struct BMGImageStruct *img );
extern
BMGError BMG_EXPORT ReadJPEG( const char *filename,
                        struct BMGImageStruct *img );
extern
BMGError BMG_EXPORT ReadJPEGFile( FILE *infile,
                        struct BMGImageStruct *img );


#if defined(__cplusplus)
 }
#endif

#endif
