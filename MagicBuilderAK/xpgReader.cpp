//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "BMGUtils.h"
#include "BMGDLL.h"
#include "xpg.h"
#include "xpgWriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <malloc.h>
#include "jpeglib.h"   /* size_t must be defined before this file is included */
#include "fmRole.h"
#include "fmMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
 
//-----------------------------------------------------------------------
HBITMAP XpgMovies::m_ReadJPEG( FILE *fp, long lFileSize )
{
#if 1
    AnsiString tempFileName = "tmp.jpg";
    if (FileExists(tempFileName)) DeleteFile(tempFileName);

    unsigned char *pszBuffer;

    long lDataLen = lFileSize;
    if (lDataLen % 4 != 0)
        lDataLen += 4 - (lDataLen % 4);
    pszBuffer = new unsigned char[lDataLen + 1];

    fread( pszBuffer, 1, lDataLen, fp );

    // write data into temp file
    FILE *fpTemp;
    if ((fpTemp = fopen(tempFileName.c_str(), "wb")) == NULL)
        return NULL;
    fseek( fpTemp, 0, SEEK_SET );

    unsigned char b[4];
    for (int i = 0; i < lDataLen; i += 4) {
        b[0] = pszBuffer[i+3];
        b[1] = pszBuffer[i+2];
        b[2] = pszBuffer[i+1];
        b[3] = pszBuffer[i+0];
        pszBuffer[i+3] = b[3];
        pszBuffer[i+2] = b[2];
        pszBuffer[i+1] = b[1];
        pszBuffer[i+0] = b[0];
    }

    fwrite( pszBuffer, 1, lFileSize, fpTemp );
    fclose(fpTemp);
    delete pszBuffer;

    return CreateBitmapFromFile( tempFileName.c_str(), NULL, 0 );
#else
    return CreateBitmapFromJpegStream( fp, NULL, 0 );
#endif
}
//----------------------------------------------------------------------------
/*  format	            1	0	jpg, png, gif¡K. Or other flag
    Pixel depth	        1	1
    Pixel type	        4	2	444, 555, 888¡K
    width	            2	6
    height	            2	8
    Image Data Length	4	10
    Image Data Position	4	14
    len = 18
*/
bool XpgMovies::m_ReadRoleHeader(FILE *fp, RoleImages *curRole)
{
    if (fp == NULL) return false;

    curRole->m_iFormat = fgetc(fp);
    curRole->m_cBpp = fgetc(fp);
    curRole->m_iType = fgetc(fp);
    curRole->m_iQuality = fgetc(fp);

    //int iPixelType = 0x888;
    int iType1 = 0;
    fread(&(iType1), 4, 1, fp);
    curRole->m_boOSD = iType1 & 1;
    curRole->m_boTrans = iType1 & 0x10000000;

    fread(&(curRole->m_iWidth), 4, 1, fp);
    fread(&(curRole->m_iHeight), 4, 1, fp);
    fread(&(curRole->m_lFilePos), 4, 1, fp);
    fread(&(curRole->m_lDataLen), 4, 1, fp);

    fread(&(curRole->m_lHashKey), 4, 1, fp);

    if (m_boXPW) {
        char buf[33];
        memset(buf, 0, 33);
        
        fread((buf), 1, 16, fp);
        curRole->m_Name = (buf);

        fread((buf), 1, 32, fp);
        curRole->m_FileName = (buf);

        if (curRole->m_Name == "" && curRole->m_FileName != "") {
            curRole->m_Name = curRole->m_FileName;
        }
    }

    return true;
}

//----------------------------------------------------------------------------
/*
XPG File Header
	name	length	Position	Description
	xpg	                4	0
	version	            4	4	1010 = v1.01
	Company name	    16	8	magicpixel
	Editor name	        16	24	"Magicbuilder, xpgConverter"
	Screen Width	    2	40
	Screen Height	    2	42
	Background Color 	6	44	RGB
	Transparent Color	6	50
	Maximun SpriteLayer	2	56
	Sprite Data Length	2	58
	Reserve	            16	60
	Image Number	    2	76
	Image Header Len	2	78
	Image Header Pos	4	80
	Page Number	        2	84
	Page Header Len	    2	86
	Page Header Pos	    4	88
	Script Number	    2	92
    Script Header Len	2	94
    Script Header Pos	4	96
*/
//----------------------------------------------------------------------------
bool XpgMovies::m_ReadHeader(FILE *fp)
{
    if (fp == NULL) return false;

    char ch[17];
    m_boXPG = false;
    m_boXPW = false;
    m_boVersion4 = false;
    fread( &ch, 1, 4, fp );
    if ( ch[0] == 'x' && ch[1] == 'p' )
    {
        if ( ch[2] == 'w' )
            m_boXPW = true;
        else  if ( ch[2] == 'g' )
            m_boXPG = true;
        m_isAnykaExt = false;
    }
    else
    {
        if(ch[0] == 'X' && ch[1] == 'P' && ch[2] == 'W' && ch[3] == '4' )
	    {
            m_boXPW = true;
            m_boVersion4 = true;
            m_isAnykaExt = false;
        }
        else if(ch[0] == 'X' && ch[1] == 'P' && ch[2] == 'W' && ch[3] == 'E' )
        {
            m_boXPW = true;
            m_boVersion4 = true;
            m_isAnykaExt = true;
        }
        else
            return false;
    }

    //temp skip header info

   /* fwrite( "xpg0", 1, 4, fp );
    fwrite( "1000", 1, 4, fp ); // version = 1.0.0
    fwrite( "magicpixel      ", 1, 16, fp );
    fwrite( "xpgconverter    ", 1, 16, fp );
    */
	fread( &m_iVersion, 4, 1, fp );
    if (m_iVersion == 0x30303031) m_iVersion = 1000;
    fseek( fp, 40, SEEK_SET );
    fread( &m_iScreenWidth, 4, 1, fp );
    fread( &m_iScreenHeight, 4, 1, fp );

    fseek( fp, 64, SEEK_SET );
    fread( &m_iSpriteDataLen, 4, 1, fp );
    fread( &m_iQuality, 1, 1, fp );
    if (m_iQuality == 0) m_iQuality = 80;

    fseek( fp, 76, SEEK_SET );

    fread( &m_iRoleCount, 4, 1, fp );  //Image Number
    fread( &m_iImageHeaderLen, 4, 1, fp );
    fread( &m_lImageHeaderPos, 4, 1, fp );

    fread( &m_iPageCount, 4, 1, fp );
    fread( &m_iPageHeaderLen, 4, 1, fp );
    fread( &m_lPageHeaderPos, 4, 1, fp );

    fread( &m_iScriptCount, 4, 1, fp );
    fread( &m_iScriptHeaderLen, 4, 1, fp );
    fread( &m_lScriptHeaderPos, 4, 1, fp );

    if (m_boXPW) {
        // write command set file name to xpw file
        fseek( fp, 128, SEEK_SET );
        char buf[128];
        memset(buf, 0, 128);
        extern AnsiString gCommandSetFileName;
        fread(buf, 1, 128, fp);
		gCommandSetFileName = buf;
        wMain->LoadMovieCommandSet(gCommandSetFileName);
    }
    return true;
}
//----------------------------------------------------------------------------
int CheckHeaderFormat(unsigned long header)
{ // return 1 - .jpg, 2 - .png, 3 - .bmp, else 0,-1 invalid
        int rc=0;
        if(    header==0xFFD8FFDB || header==0xDBFFD8FF
            || header==0xFFD8FFE0 || header==0xE0FFD8FF
            || header==0xFFD8FFE1 || header==0xE1FFD8FF
            || header==0xFFD8FFE2 || header==0xE2FFD8FF
            || header==0xFFD8FFE3 || header==0xE3FFD8FF
            || header==0xFFD8FFE8 || header==0xE8FFD8FF
            )
            rc = 1;
        else if( header==0x474E5089 || header==0x89504E47)
                rc = 2;
        else if( header==0x424DD641 || header==0x41D64D42)
                  rc = 3;
        else rc = 0; //-1;
        return rc;
}

bool XpgMovies::m_LoadFile( const char *filename )
{
    FILE * fp = NULL;
    bool boNoErr = true;
    HBITMAP hBitmap;

    if ((fp = fopen(filename, "rb")) == NULL)
        return false;

    if (m_RoleList == NULL) return false;

    boNoErr = m_ReadHeader( fp );
    if (m_iRoleCount <= 0) return false;

    if (boNoErr)
    {
        // Read Role Data
        for (int iRole = 0; iRole < m_iRoleCount; iRole++)
        {
                //int Image794=0;
                //if(iRole == 794)
                //       Image794=1;
             // Read Role Header
            fseek( fp, m_lImageHeaderPos + iRole * m_iImageHeaderLen, SEEK_SET );
            m_pCurRole = new RoleImages;

            m_ReadRoleHeader(fp, m_pCurRole);
            m_RoleList->Add(m_pCurRole);
            m_pCurRole->m_iIndex = iRole;
            // check m_lDataLen first
            if (m_pCurRole->m_lDataLen <= 0)
            {
                fseek( fp, m_pCurRole->m_lFilePos, SEEK_SET );
                fread(&(m_pCurRole->m_iImgSourceSize), 4, 1, fp);
                if (m_pCurRole->m_iImgSourceSize > 0)
                    m_pCurRole->m_lDataLen = m_pCurRole->m_iImgSourceSize;
            }


                // Read Role Image Data
                fseek( fp, m_pCurRole->m_lFilePos, SEEK_SET );

                if(!m_boVersion4) // Version4 don't read JPEG data or it will getting blurry
                {
                        hBitmap = m_ReadJPEG(fp, m_pCurRole->m_lDataLen);  //HBITMAP hBitmap = m_ReadRLE(fp, m_pCurRole->m_lDataLen);
                        if (hBitmap) {
                            m_pCurRole->m_pBitmap = new Graphics::TBitmap;
                            m_pCurRole->m_pBitmap->Handle = hBitmap;
                        }
                }

                if (m_boXPW) //if (m_iVersion >= VERSION_WITH_SOURCE && m_iVersion != VERSION3_WITHOUT_SOURCE && m_boXPW)
                {

                    fread(&(m_pCurRole->m_iImgSourceSize), 4, 1, fp);
                    // add more check for v4.1.10's bug case
                    long curFilePos = ftell(fp);
                    unsigned long header;
                    fread(&header, 4, 1, fp);
                    if(CheckHeaderFormat(header) <= 0) // v4.1.10 bug - correct bitmap but invalid image source
                    {
                        fseek( fp, m_pCurRole->m_lFilePos, SEEK_SET );
                         m_pCurRole->m_lDataLen += 4 - (m_pCurRole->m_lDataLen % 4);
                         m_pCurRole->m_iImgSourceSize = m_pCurRole->m_lDataLen;
                    }
                    else
                        fseek( fp, curFilePos, SEEK_SET );
                    // end of add more check for v4.1.10 bug case
                    // extra read source
                    if (m_pCurRole->m_iImgSourceSize <= 0) // on 20091222 F:\XPW\Fengers\original\Philips800X480DPF.xpw , but JPPEG data exist!
                    {
                        m_pCurRole->m_iImgSourceSize =  m_pCurRole->m_lDataLen;
                        fseek( fp, m_pCurRole->m_lFilePos, SEEK_SET );
                    }
                    if (m_pCurRole->m_iImgSourceSize > m_pCurRole->m_lDataLen) // v4.1.6 add for ui_T2_7-100105_1_CopyPopupBG_MB4.1..4.xpw , image source 70.110 partial image lost!
                    {
                        m_pCurRole->m_lDataLen = m_pCurRole->m_iImgSourceSize;
                    }
                        m_pCurRole->m_pImgSource = (unsigned char *)malloc(m_pCurRole->m_iImgSourceSize);
                        if (m_pCurRole->m_pImgSource == NULL)
                        {
                           Application->MessageBox("!!!alocate m_pImgSource Fail in xpgReader.cpp m_LoadFile()", "Error", MB_OK);
                           return  false;
                        }
                        fread(m_pCurRole->m_pImgSource, 1, m_pCurRole->m_iImgSourceSize, fp);
                        // simulate JPEG data information
                        fseek( fp, m_pCurRole->m_lFilePos+4, SEEK_SET );
                        hBitmap = m_ReadJPEG(fp, m_pCurRole->m_lDataLen);
                        if (hBitmap) {
                            m_pCurRole->m_pBitmap = new Graphics::TBitmap;
                            m_pCurRole->m_pBitmap->Handle = hBitmap;
                        }
                  } // if (m_boXPW)

                // Jonny added, To set hBitmap by original ImgSource
                AnsiString extStr = ExtractFileExt(m_pCurRole->m_FileName);
                extStr = extStr.LowerCase();
                AnsiString tempFileName;
                // **************************
                // check file header leading 4 bytes to decide format
                unsigned long header;

                fseek( fp, m_pCurRole->m_lFilePos, SEEK_SET );
                fread(&header, 4, 1, fp);
                if(  header==0xFFD8FFDB || header==0xDBFFD8FF
                  || header==0xFFD8FFE0 || header==0xE0FFD8FF
                  || header==0xFFD8FFE1 || header==0xE1FFD8FF
                  || header==0xFFD8FFE2 || header==0xE2FFD8FF
                  || header==0xFFD8FFE3 || header==0xE3FFD8FF
                  || header==0xFFD8FFE8 || header==0xE8FFD8FF
                  )
                    tempFileName = "tmp.jpg";
                else if( header==0x474E5089 || header==0x89504E47 )
                    tempFileName = "tmp.png";
                else if( header==0x424DD641 || header==0x41D64D42 )
                    tempFileName = "tmp.bmp";
                else // maybe it's ImgSourceSize in different version , try next 4 bytes
                {
                     fread(&header, 4, 1, fp);
                     if(  header==0xFFD8FFDB || header==0xDBFFD8FF
                        || header==0xFFD8FFE0 || header==0xE0FFD8FF
                        || header==0xFFD8FFE1 || header==0xE1FFD8FF
                        || header==0xFFD8FFE2 || header==0xE2FFD8FF
                        || header==0xFFD8FFE3 || header==0xE3FFD8FF
                        || header==0xFFD8FFE8 || header==0xE8FFD8FF
                        )
                         tempFileName = "tmp.jpg";
                     else if( header==0x474E5089 || header==0x89504E47 )
                         tempFileName = "tmp.png";
                     else if( header==0x424DD641 || header==0x41D64D42 )
                         tempFileName = "tmp.bmp";
                     else
                     {
                          Application->MessageBox("Error !!! Unknown file type in header checking!", "Error", MB_OK);
                           return  false;
                     }
                }
                // ***************************
                if (FileExists(tempFileName)) DeleteFile(tempFileName);
                FILE *fpTemp = NULL;
                if ((fpTemp = fopen(tempFileName.c_str(), "wb")) != NULL)
                {
                    fwrite(m_pCurRole->m_pImgSource, 1, m_pCurRole->m_iImgSourceSize, fpTemp);
                    fclose(fpTemp);
                }
                struct BMGImageStruct img;
	        InitBMGImage( &img );
                BMGError out;
                out = GetDataFromFile(tempFileName.c_str(), &img, NULL );
		if ( out == BMG_OK )
	                hBitmap = CreateBitmapFromData( img, 0 );
                FreeBMGImage(&img);
                if (hBitmap) {
                    m_pCurRole->m_pBitmap = new Graphics::TBitmap;
                    m_pCurRole->m_pBitmap->Handle = hBitmap;
                } // else ? m_pCurRole->m_pBitmap->Handle = NULL;
                if (FileExists(tempFileName)) DeleteFile(tempFileName);

        }  // for


        if (m_PageList->Count != 0) {
            m_PageList->Clear();
        }
        //Read Pages & Sprites
        for (int iPage = 0; iPage < m_iPageCount; iPage++)
        {
            // Read Page Header
            Pages *pPage = new Pages;
            pPage->m_pMovie = this;

            fseek( fp, m_lPageHeaderPos + iPage * m_iPageHeaderLen, SEEK_SET );
            m_ReadPageHeader(fp, pPage);

            // Read Sprite Data
            if (pPage->m_lSpriteFilePos > 0) {
                fseek( fp, pPage->m_lSpriteFilePos, SEEK_SET );
                for (int iSp = 0; iSp < pPage->m_iSpriteCount; iSp++)
                {
                    Sprites *pSprite = new Sprites;
                    m_ReadSpriteData(fp, pSprite);

                    pPage->m_SpriteList->Add(pSprite);
                }
            }

            // Read Script Data
            if (pPage->m_lScriptFilePos > 0) {
                fseek( fp, pPage->m_lScriptFilePos, SEEK_SET );
                int nCommands = 0;
                fread(&nCommands, 4, 1, fp);
                for (int iCmd = 0; iCmd < nCommands; iCmd++)
                {
                    short iScript;
                    short iButton;
                    short iEvent;
					short iGotoPage = 0;
					short iAction = 0;
					short iTemp = 0;
					short iParam = 0;

					//fread(&iScript, 4, 1, fp);
                    fread(&iButton, 2, 1, fp);
					fread(&iEvent, 2, 1, fp);

					pPage->SetCommandButton(iCmd, m_ButtonList.GetNameByKey(iButton + 1));
					pPage->SetCommandEvent(iCmd, m_EventList.GetNameByKey(iEvent));

					if (!m_boXPW || m_iVersion >= 1100) {
						fread(&iGotoPage, 2, 1, fp);
                        fread(&iTemp, 2, 1, fp);
						fread(&iAction, 2, 1, fp);
                        fread(&iParam, 2, 1, fp);
					}
					
                    if (m_boXPW){
                        char buf[33];
                        memset(buf, 0, 33);

                        fread((buf), 1, 16, fp);
                        pPage->SetCommandPage(iCmd, AnsiString(buf));
                        //pPage->m_ButtonPage[iScript] = AnsiString(buf);

                        fread((buf), 1, 32, fp);
                        pPage->SetCommandAction(iCmd, AnsiString(buf));
                        //pPage->m_ButtonAction[iScript] = AnsiString(buf);    
                    }
                }
            }
            m_PageList->Add(pPage);
        }
    }

    fclose(fp);
    return boNoErr;
}
//-----------------------------------------------------------------------
bool XpgMovies::m_ReadSpriteData(FILE *fp, Sprites *pSprite)
{
/*
Layer	    2	0
X	        2	2
Y	        2	4
Image Index	2	6
ink	        1	8
ink value	3	9

*/
    if (fp == NULL) return false;

    int i;
    fread(&(i), 4, 1, fp);
    pSprite->m_iIndex = i;
    int x, y;
    int iRole;
    fread(&(x), 4, 1, fp);
    fread(&(y), 4, 1, fp);
    fread(&(iRole), 4, 1, fp);

    //x = y = 0;
    
    if (iRole >= m_RoleList->Count) iRole = 0;

    pSprite->m_SetRole((RoleImages *)m_RoleList->Items[iRole]);
    pSprite->m_MoveTo(x, y);

	int iType = 0;
    fread(&(iType), 4, 1, fp);
    //fread(&(pSprite->m_iInkValue), 3, 1, fp);
	pSprite->m_iType = (iType >> 16) & 0xffff;
	pSprite->m_iTypeIndex = iType & 0xffff;
    fread(&(pSprite->m_lHashKey), 4, 1, fp);

    if (m_isAnykaExt)
    {
        unsigned temp;
        fread(&temp, 4, 1, fp);
        pSprite->m_touchEnable = temp;
        fread(&temp, 4, 1, fp);
        pSprite->m_touchFlag = temp;
        fread(&temp, 4, 1, fp);
        pSprite->m_flag = temp;
        fread(&temp, 4, 1, fp);                 // ±£Áô
        fread(&temp, 4, 1, fp);
        fread(&temp, 4, 1, fp);
        fread(&temp, 4, 1, fp);
        fread(&temp, 4, 1, fp);
    }
    else
    {
        pSprite->m_touchEnable = 0;
        pSprite->m_touchFlag = 0;
        pSprite->m_flag = 0;
    }

    if (m_boXPW)
    {
        char buf[17];
        //strcpy( buf, pSprite->m_Name.c_str() );
        fread((buf), 1, 16, fp);
        pSprite->m_Name = String(buf);
    }
    return true;
}
//-----------------------------------------------------------------------
bool XpgMovies::m_ReadPageHeader(FILE *fp, Pages *pPage)
{
/*
Page Index	                2	0
Page Script Index	        2	2
Sprite Number	            2	4
Sprite Data Start Position	4	6
*/
    if (fp == NULL) return false;

    fread(&(pPage->m_iIndex), 4, 1, fp);
    fread(&(pPage->m_lScriptFilePos), 4, 1, fp);
    fread(&(pPage->m_iSpriteCount), 4, 1, fp);
    fread(&(pPage->m_lSpriteFilePos), 4, 1, fp);
    fread(&(pPage->m_lHashKey), 4, 1, fp);

    //if (pPage->m_iSpriteCount > 1) pPage->m_iSpriteCount = 1;
    if (m_iPageHeaderLen >= 40) {
        char buf[17];
        memset(buf, 0, 17);
        fread(&buf, 1, 16, fp);
        pPage->m_Name = buf;
    }

    return true;
}
//-----------------------------------------------------------------------
HBITMAP XpgMovies::m_ReadRLE( FILE *fp, long lFileSize )
{
    /* TODO : Add code:  RLE decoding */
    int wd = 0;
    int ht = 0;
    fread(&wd, 2, 1, fp);
    fread(&ht, 2, 1, fp);

    int nSize = (wd % 2) ? (wd + 1) * ht : wd * ht;

    unsigned long *pBits = (unsigned long *)malloc(nSize * 4);

    HDC hdc = wRole->Canvas->Handle;
    HBITMAP hbmp = CreateCompatibleBitmap( hdc, wd, ht );
    //unsigned long *pBits =  Bitmap->bmBits;
    //BITMAPINFO *lpbi = (BITMAPINFO *)malloc(sizeof(BITMAPINFO));
    //GetDIBits( hdc, hbmp, 0, ht, pBits, lpbi, DIB_RGB_COLORS );


   // pBits = (unsigned long *)malloc(wd * ht * 4);
    //for (int i = 0; i < nSize; i++) *pBits++ = i;

   /* unsigned long pixel = 0;
    for (int y = 0; y < ht; y++) {
        for (int x = 0; x < wd; ) {
            fread(&pixel, 4, 1, fp);

            int count = (pixel >> 24) & 0xff;
            int color = (pixel & 0xffffff) | 0x7f000000;
            for (int i = 0; i < count && x < wd; i++, x++) {
                *pBits++ = color;
            }
        }
    }
    */
    for (int i = 0; i < nSize; i++)
        *pBits++ = 0xffffffff | i;
    return hbmp;
}
//-----------------------------------------------------------------------




