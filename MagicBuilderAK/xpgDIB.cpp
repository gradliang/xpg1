//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "xpgDIB.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

TColor CanvasPal[256];

/****************************************************************************/
/*
// Creates a BITMAPINFOHEADER for the given width, height, bit count, and
// compression.  Compression must = BI_RGB, BI_BITFIELDS, BI_RLE4, or BI_RLE8.
*/
PBITMAPINFO xpgCreateBMI( DWORD dwWidth,  /* width */
                              DWORD dwHeight, /* height */
                              WORD wBitCount, /* bit count */
                              int compression )  /* compression type */
{
     PBITMAPINFO pbmi;         /* bitmap header */
     DWORD dwBytesPerLine;        /* Number of bytes per scanline */
     int iBmiSize;

     /* Make sure bits per pixel is valid */
     if (wBitCount <= 1)
        wBitCount = 1;
     else if (wBitCount <= 4)
        wBitCount = 4;
     else if (wBitCount <= 8)
        wBitCount = 8;
     else if (wBitCount <= 16)
        wBitCount = 16;
     else if (wBitCount <= 24)
        wBitCount = 24;
     else if (wBitCount <= 32)
        wBitCount = 32;
     else
        wBitCount = 8;  /* set default value to 8 if parameter is bogus */


	if (wBitCount <= 8)
        iBmiSize = sizeof(BITMAPINFOHEADER) +
		sizeof(RGBQUAD) * (1<< wBitCount);
    else
	// There is no RGBQUAD array for the 24-bit-per-pixel format.
       iBmiSize = sizeof(BITMAPINFOHEADER);

    pbmi = (PBITMAPINFO) LocalAlloc(LPTR, iBmiSize);

   dwBytesPerLine =   (((wBitCount * dwWidth) + 31) / 32 * 4);

   /* initialize BITMAPINFO */
   pbmi->bmiHeader.biSize = iBmiSize;
   pbmi->bmiHeader.biWidth = dwWidth;
   pbmi->bmiHeader.biHeight = dwHeight;
   pbmi->bmiHeader.biPlanes = 1;              /* must be 1 */
   pbmi->bmiHeader.biBitCount = wBitCount;
   pbmi->bmiHeader.biCompression =  compression;
   pbmi->bmiHeader.biSizeImage = dwBytesPerLine*dwHeight;
   pbmi->bmiHeader.biXPelsPerMeter = 1200;
   pbmi->bmiHeader.biYPelsPerMeter = 1200;
   pbmi->bmiHeader.biClrUsed = wBitCount <= 8 ? 1U << wBitCount : 0;
   pbmi->bmiHeader.biClrImportant = pbmi->bmiHeader.biClrUsed;

   return pbmi;
}
//---------------------------------------------------------------------------

Byte *xpgCreateDib(int Width, int Height, int iBitsPixel)
{
    //Byte *lpBits;
    PBITMAPINFO   lpBmi;

    PBITMAPINFO lpTempBmi = xpgCreateBMI( Width, Height, iBitsPixel, BI_RGB );

    Byte *lpDib = (Byte *)malloc( lpTempBmi->bmiHeader.biSizeImage + sizeof(BITMAPINFO) );
    if ( lpDib == NULL )
        return NULL;

    lpBmi = (PBITMAPINFO)lpDib;
    //lpBits = (Byte *)(lpDib + sizeof(BITMAPINFO));
    memcpy(lpBmi, lpTempBmi, sizeof(BITMAPINFO));
    return lpDib;
}
//---------------------------------------------------------------------------
Byte *xpgGetBitmapDib(HBITMAP hBitmap)
{
    if (hBitmap == NULL) {
        //Application->MessageBox("Bitmap Handle == NULL", "Bitmap Handle Error", IDOK);

        return NULL;
    }
    Byte *lpDib, *lpBits;
    BITMAPINFO   *lpBmi;
    unsigned int        DIBScanWidth;
    DIBSECTION          DS;
    HWND                hWnd = GetForegroundWindow();
    HDC                 hDC = NULL;
    //HDC                 hMemDC = NULL;
    BITMAP bm;
    PBITMAPINFO lpTempbmi;

    int numBytes = GetObject( hBitmap, sizeof(DIBSECTION), &DS );
    if ( numBytes == 0 )
        return NULL;

    if ( GetObject( hBitmap, sizeof(BITMAP), &bm ) == 0 )
        return NULL;

    int iBitsPixel = bm.bmBitsPixel;
    //int iBitsPixel = 32;
    lpTempbmi = xpgCreateBMI( bm.bmWidth, bm.bmHeight, iBitsPixel, BI_RGB );

    lpDib = (Byte *)malloc( lpTempbmi->bmiHeader.biSizeImage + sizeof(BITMAPINFO)  );
    if ( lpDib == NULL )
        return NULL;

    lpBmi = (BITMAPINFO *)lpDib;
    lpBits = (Byte *)(lpDib + sizeof(BITMAPINFO));
    memcpy(lpBmi, lpTempbmi, sizeof(BITMAPINFO));

    hDC = GetDC( hWnd );

    UINT uColorUsage = DIB_PAL_COLORS;//(bmi.bmiHeader.biClrUsed) ? DIB_PAL_COLORS : DIB_RGB_COLORS;
    if ( GetDIBits(hDC, hBitmap, 0, bm.bmHeight, (void *)lpBits, lpTempbmi,
                   uColorUsage ) == 0 ) {
        free(lpDib);
        lpDib = NULL;
    }

    ReleaseDC( hWnd, hDC );
    hDC = NULL;

    return lpDib;
}
//---------------------------------------------------------------------------
void xpgDrawDib(Byte *lpCanvasDib, int left, int top, Byte *lpDib)
{
    if( lpCanvasDib == NULL || lpDib == NULL ) return;

    BITMAPINFO *lpCanvasBmi = (BITMAPINFO *)lpCanvasDib;
    Byte *lpCanvasBits = (Byte *)(lpCanvasDib + sizeof(BITMAPINFO));

    BITMAPINFO *lpBmi = (BITMAPINFO *)lpDib;
    Byte *lpBits = (Byte *)(lpDib + sizeof(BITMAPINFO));

    int iCanvasWidth  = lpCanvasBmi->bmiHeader.biWidth;
    int iCanvasHeight = lpCanvasBmi->bmiHeader.biHeight;
    int iDibWidth  = lpBmi->bmiHeader.biWidth;
    int iDibHeight = lpBmi->bmiHeader.biHeight;

    int xLimit = iDibWidth;
    int yLimit = iDibHeight;

    if (left + xLimit >= iCanvasWidth) xLimit = iCanvasWidth - left - 1;
    if (top + yLimit >= iCanvasHeight) yLimit = iCanvasHeight - top - 1;

    int iBytesPerLine = (((lpBmi->bmiHeader.biBitCount * iDibWidth) + 31) / 32 * 4);
    int iCanvasBytesPerLine = (((lpCanvasBmi->bmiHeader.biBitCount * iCanvasWidth) + 31) / 32 * 4);


    for (int y = 0; y < yLimit; y++, top++) {
        DWORD *pCanvasAddr = (DWORD *)(lpCanvasBits + (iCanvasHeight - top) * iCanvasBytesPerLine + left * 4);
        DWORD *pAddr = (DWORD *)(lpBits + (iDibHeight - y) * iBytesPerLine);
        for (int x = 0; x < xLimit; x++) {
            DWORD c = (*pAddr++);
            if ((c & 0xffffff) != 0 && (c & 0xff000000) == 0)
             *pCanvasAddr = c;

            pCanvasAddr++;
        }
    }
}

//---------------------------------------------------------------------------
void xpgDrawBitmap(Byte *lpCanvasDib, int x, int y, HBITMAP hBitmap)
{
    Byte *lpDib = xpgGetBitmapDib( hBitmap);
    if (lpDib) {
        xpgDrawDib(lpCanvasDib, x, y, lpDib);
        free(lpDib);
    }
}

//---------------------------------------------------------------------------

Byte *xpgInitCanvas(TImage *pImage)
{
    Byte *lpDib = xpgCreateDib(pImage->Width, pImage->Height, DIB_BITCOUNT);
    return lpDib;
}
//---------------------------------------------------------------------------
void xpgShowCanvas(TCanvas *pCanvas, Byte *lpDib)
{
    HDC hdc = pCanvas->Handle; //GetDC( hWnd );
    if( hdc == NULL || lpDib == NULL ) return;

    pCanvas->Lock();

    BITMAPINFO *lpBmi = (BITMAPINFO *)lpDib;
    Byte *lpBits = (Byte *)(lpDib + sizeof(BITMAPINFO));

    int iDibWidth  = lpBmi->bmiHeader.biWidth;
    int iDibHeight = lpBmi->bmiHeader.biHeight;

    UINT uColorUsage = (lpBmi->bmiHeader.biBitCount <= 8) ? DIB_PAL_COLORS : DIB_RGB_COLORS;
    StretchDIBits ( hdc, 0, 0, iDibWidth, iDibHeight,  //dest
                         0, 0, iDibWidth, iDibHeight,  //source
                         lpBits,
                         lpBmi,
                         uColorUsage, SRCCOPY );
    pCanvas->Unlock();
}
//---------------------------------------------------------------------------

