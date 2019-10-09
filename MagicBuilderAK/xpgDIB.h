//---------------------------------------------------------------------------
#ifndef xpgDIBH
#define xpgDIBH

#include <vcl.h>

#define DIB_BITCOUNT 32
Byte *xpgInitCanvas(TImage *pImage);
void xpgShowCanvas(TCanvas *pCanvas, Byte *lpDib);
void xpgDrawBitmap(Byte *lpCanvasDib, int x, int y, HBITMAP hBitmap);

//---------------------------------------------------------------------------
#endif
