//---------------------------------------------------------------------------

#ifndef xpgRoleH
#define xpgRoleH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "BMGDLL.h"

class XpgMovies;
//---------------------------------------------------------------------------
class RoleImages
{
public:
    XpgMovies *m_pMovie;
    
    long  m_lHashKey;
    long  m_lFilePos;
    long  m_lDataLen;

    long  m_iIndex;
    char  m_cBpp;

    //LSB: 1- RLE format; 0- not RLE format
    //MSB: 1- Transparent; 0- Opague
	long  m_iType;

    long  m_iFormat;
    long  m_iQuality;
  	long  m_iWidth;
    long  m_iHeight;
	long  m_iOx, m_iOy;
    unsigned long  m_iTransColor;

    bool  m_boOSD;
    bool  m_boSourceOutput;
    bool  m_boTrans;
	bool  m_boLoaded;
	bool  m_boExist;
	bool  m_boUsedAtThisPage;

    Graphics::TBitmap *m_pBitmap;
	struct BMGImageStruct img;

    BYTE *m_pImgSource;
    long m_iImgSourceSize;
    AnsiString m_Name;
    AnsiString m_FileName;
     
    RoleImages();
    ~RoleImages();

public:
    void m_Clear();
	/*void freeRoleImage();
	int loadRoleImage(char beginColor, FILE *fp = NULL);
    int loadRoleImage256(char beginColor, FILE *fp = NULL);
	int loadPcxRoleImage(char *fileName);
	void DibToImgBits(unsigned char *lpDib, int iWd, int iHt,
                      int iChannels, int iFormat);
	bool readPngFormatImg(FILE *fp = NULL);
	void unloadRoleImage();
	void clearRoleImage();
	void drawString(int x, int y, int color, char str[]);
    void drawImage();
    void newImage(int w, int h, int fmt);
    void loadMap(FILE *fp);
	void zoom_image( int d_wd, int d_ht );  */
#ifdef EDITOR_MODE
	
#endif	
};

#endif
