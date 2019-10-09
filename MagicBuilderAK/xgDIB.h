//---------------------------------------------------------------------------
#ifndef xgDIBH
#define xgDIBH

#include <vcl.h>
typedef unsigned short XGPIXEL;
typedef unsigned long XPGPIXEL;

#define XPG_IMAGE_HEADER_SIZE 16
#define IMG_START_ADDR XPG_IMAGE_HEADER_SIZE
void  ImageDrawBitmap( TImage *pImg, void *lpDib );
void ViewBits( TImage *pImg, void *pBits, int iWd, int iHt, int iFormat );
void  ViewDiBits( TImage *pImg, void *pBits, int iWd, int iHt, int iFormat );
void g_ShowDib( HDC hdc, int x, int y, BYTE *lpDib );
//---------------------------------------------------------------------------
#endif
