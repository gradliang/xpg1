#ifndef _JPEG_RW_H_
#define _JPEG_RW_H_
/*
//  header file for the BMGLib JPEG functions
//
//  Copyright 2000, 2001 M. Scott Heiman
//  All Rights Reserved
//  libJPEG is Copyright (C) 1991-1998, Thomas G. Lane and is part of the
//      Independent JPEG Group's software.
*/
#include "BMGImage.h"
#include <stdio.h>


#if defined(__cplusplus)
extern "C" {
#endif

extern
BMGError BMG_EXPORT ReadJPEG( const char *filename,
                         struct BMGImageStruct *img );

extern
BMGError BMG_EXPORT ReadJPEGFile( FILE *infile,
                         struct BMGImageStruct *img );

extern
BMGError BMG_EXPORT WriteJPEG( const char *filename,
                          struct BMGImageStruct img,
                          int quality );

extern
BMGError BMG_EXPORT WriteJPEGFile( FILE *outfile,
                          struct BMGImageStruct img,
                          int quality );

#if defined(__cplusplus)
 }
#endif

#endif