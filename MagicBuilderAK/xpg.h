//---------------------------------------------------------------------------

#ifndef xpgH
#define xpgH


class RoleImages;
class Sprites;
class Pages;
class XpgMovies;

#include "xpgRole.h"
#include "xpgSprite.h"
#include "xpgPage.h"
#include "xpgMovie.h"

extern XpgMovies *xpgMovie;

void xpg_open();
void xpg_close();
DWORD RGBPixel2YUV(DWORD color1, DWORD color2);
//---------------------------------------------------------------------------
#endif
