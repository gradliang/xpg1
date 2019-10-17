//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <vector>
#include <string>
#include <stdlib.h>
#include <io.h>
#include <time.h>
#include "xpgHash.h"
#include "BMGUtils.h"
#include "BMGDLL.h"
#include "xpg.h"
#include "xpgDIB.h"
#include "xpgWriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "jpeglib.h"   /* size_t must be defined before this file is included */
#include "fmRole.h"
#include "fmMain.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

#include <stdio.h>
struct PCXInfo
    {
      unsigned char Identifier,Version,Encoding, BitsPerPixel;

      short int Left,Top,Right,Bottom,HorizontalRes,VerticalRes;

      unsigned char Reserved,Planes;

      short int BytesPerLine,PaletteInfo;

      unsigned char ColormapSignature;
    };

void WritePCXHeader(FILE *fp, Graphics::TBitmap* pBitmap, PCXInfo& PCXHeader)
{
    //Initializing Header
    PCXHeader.Identifier= 0x0A;
    PCXHeader.Version=5;
    PCXHeader.Encoding=1;
    PCXHeader.BitsPerPixel=8;
    PCXHeader.Left=0;
    PCXHeader.Top=0;
    PCXHeader.Right= pBitmap->Width-1;
    PCXHeader.Bottom= pBitmap->Height-1;
    PCXHeader.HorizontalRes = pBitmap->Width;
    PCXHeader.VerticalRes = pBitmap->Height;
    PCXHeader.Reserved=1;
    PCXHeader.Planes= 1;
    PCXHeader.BytesPerLine= (pBitmap->Width*PCXHeader.BitsPerPixel+7)/8;
    PCXHeader.PaletteInfo=1;

    //Write Header to File
    fwrite(&PCXHeader.Identifier, 1, 1, fp);
    fwrite(&PCXHeader.Version,1, 1, fp);
    fwrite(&PCXHeader.Encoding,1, 1, fp);
    fwrite(&PCXHeader.BitsPerPixel,1, 1, fp);
    fwrite(&PCXHeader.Left,2, 1, fp);
    fwrite(&PCXHeader.Top,2, 1, fp);
    fwrite(&PCXHeader.Right,2, 1, fp);
    fwrite(&PCXHeader.Bottom,2, 1, fp);
    fwrite(&PCXHeader.HorizontalRes,2, 1, fp);
    fwrite(&PCXHeader.VerticalRes,2, 1, fp);

    //Write Transparent Color to the palette section, [palette index, R, G ,B]-4 bytes
    PALETTEENTRY PaletteEntry[256];
    GetPaletteEntries(pBitmap->Palette,0,256,PaletteEntry);
    int i;
    for(i=0;i<256;i++)
    {
        if((PaletteEntry[i].peRed==0x00)&&(PaletteEntry[i].peGreen==0x00)&&(PaletteEntry[i].peBlue==0xff))
        {
            break;
        }
    }
    Byte index = (byte)i;
    fwrite(&index,1, 1, fp);
    fwrite(&PaletteEntry[i].peRed,1, 1, fp);
    fwrite(&PaletteEntry[i].peGreen,1, 1, fp);
    fwrite(&PaletteEntry[i].peBlue,1, 1, fp);

    //Pad the rest of the palette section with 0
    Byte DummyPallet[44] = "";
    fwrite(DummyPallet,sizeof(DummyPallet), 1, fp);

    //Write the rest of the header
    fwrite(&PCXHeader.Reserved,1, 1, fp);
    fwrite(&PCXHeader.Planes,1, 1, fp);
    fwrite(&PCXHeader.BytesPerLine,2, 1, fp);
    fwrite(&PCXHeader.PaletteInfo,2, 1, fp);

    //Fill the rest of the header with 0
    Byte Filler[58] ="";
    fwrite(Filler, sizeof(Filler), 1, fp);

};

//---------------------------------------------------------------------------
void WritePCXPalette(FILE *fp, Graphics::TBitmap* pBitmap, PCXInfo&PCXHeader)
{

    PCXHeader.ColormapSignature=0x0c;
    fwrite(&PCXHeader.ColormapSignature,1, 1, fp);

    tagPALETTEENTRY PaletteEntry[256];
    //PaletteEntry = (tagPALETTEENTRY)malloc(sizeof(tagPALETTEENTRY) * 256);
    GetPaletteEntries(pBitmap->Palette,0,256,PaletteEntry);

    BYTE PCXColorMap[256][3];
    for(int i=0; i<256; i++)
    {
        PCXColorMap[i][0]= PaletteEntry[i].peRed;
        PCXColorMap[i][1]= PaletteEntry[i].peGreen;
        PCXColorMap[i][2]= PaletteEntry[i].peBlue;
        //PaletteEntry++;
    }
    fwrite(&PCXColorMap,1, 256*3, fp);
};

// for only support 7bits, this function need change to 128 colors
void SavePCX(FILE *fp, Graphics::TBitmap *pBitmap)
{
    PCXInfo PCXHeader;

    //Downscale image to 8 bit
    pBitmap->PixelFormat = pf8bit;

    Byte *ptr;  //pointer to one scanline
    Byte previous, packet;
    Byte count;  //count of pixels

    //Write PCX header
    WritePCXHeader(fp, pBitmap, PCXHeader);

    //RLE Encoding and Write Data
    for (int y = 0; y < pBitmap->Height; y++)
    {
      ptr =(Byte*) pBitmap->ScanLine[y];

      count = 1;
      previous = ptr[0];

      for (int x = 1; x <= pBitmap->Width; x++)
      {
        if(x<pBitmap->Width)
            packet = ptr[x];
        else{}

        if( (x<pBitmap->Width)&&(packet==previous)&&(count <63))
            count++;
        else
        {
            //previous & C0h = C0h mean > 192
            if( (count>1)||((previous&0xc0)==0xc0))
            {
            //ShowMessage(count);
            count|=0xc0;
            //ShowMessage(count);
            fwrite(&count,1, 1, fp);   //write count
            }
            //ShowMessage(previous);
            fwrite(&previous,1, 1, fp);    //write color
            count = 1;
            if(x<pBitmap->Width)
                previous = ptr[x];
        }
       }
    }

    //Append Pallete Infomation
    WritePCXPalette(fp, pBitmap, PCXHeader);
}
//----------------------------------------------------------------------------
/*
XPG File Header
	name	length	Position	Description
	xpg	                4	0
	version	            4	4	1010 = v1.01
	Company name	    16	8	magicpixel
	Editor name	        16	24	"Magicbuilder, xpgConverter"
	Screen Width	    4	40
	Screen Height	    4	44
	Background Color 	6	48	RGB
	Transparent Color	6	54
	Maximun SpriteLayer	4	60
	Sprite Data Length	4	64
	Reserve	            8	68
	Image Number	    4	76
	Image Header Len	4	80
	Image Header Pos	4	84
	Page Number	        4	88
	Page Header Len	    4	92
	Page Header Pos	    4	96
	Script Number	    4	100
    Script Header Len	4	104
    Script Header Pos	4	108
*/
bool XpgMovies::m_WriteHeader(FILE *fp)
{
    if (fp == NULL) return false;

    if (m_boXPW)
    {
        //fwrite( "xpw0", 1, 4, fp );           // magic builder 2.x
        //fwrite( "XPW4", 1, 4, fp );           // magic builder 4.x
        fwrite( "XPWE", 1, 4, fp );             // magic builder 4.x (Anyka Platform Ext)
    }
    else
        fwrite( "xpg0", 1, 4, fp );

    m_iVersion = MAGICBUILDER_VERSION;  
    fwrite( &m_iVersion, 1, 4, fp );
    fwrite( "magicpixel      ", 1, 16, fp );
    fwrite( "magicbuilder    ", 1, 16, fp );

    fseek( fp, 40, SEEK_SET );
    fwrite( &m_iScreenWidth, 4, 1, fp );
    fwrite( &m_iScreenHeight, 4, 1, fp );
    fwrite( &m_iHeaderInfo, 4, 1, fp );

    fseek( fp, 64, SEEK_SET );
    fwrite( &m_iSpriteDataLen, 4, 1, fp );
    fwrite( &m_iQuality, 1, 1, fp );

    fseek( fp, 76, SEEK_SET );
    m_iRoleCount = m_RoleList->Count;

    fwrite( &m_iRoleCount, 4, 1, fp );  //Image Number

    fwrite( &m_iImageHeaderLen, 4, 1, fp );
    fwrite( &m_lImageHeaderPos, 4, 1, fp );

    fwrite( &m_iPageCount, 4, 1, fp );
    fwrite( &m_iPageHeaderLen, 4, 1, fp );
    fwrite( &m_lPageHeaderPos, 4, 1, fp );

    fwrite( &m_iScriptCount, 4, 1, fp );
    fwrite( &m_iScriptHeaderLen, 4, 1, fp );
    fwrite( &m_lScriptHeaderPos, 4, 1, fp );


    if (m_boXPW) {
        // write command set file name to xpw file
        fseek( fp, 128, SEEK_SET );
        char buf[128];
        memset(buf, 0, 128);
        extern AnsiString gCommandSetFileName;
        strcpy( buf, gCommandSetFileName.c_str() );
        fwrite(buf, 1, 128, fp);
    } else {
        fseek( fp, 128, SEEK_SET );

    }
    return true;
}
long GetJPEGData(Graphics::TBitmap *Bitmap, int quality )
{
    AnsiString tempFileName = "tmp.jpg";
    if (FileExists(tempFileName)) DeleteFile(tempFileName);
    //WriteJpegFile to temp file first
    if ( SaveBitmapToFile( Bitmap->Handle, tempFileName.c_str(),
                     (void *)&quality ) != BMG_OK )
    {
        const char *msg;
        GetLastBMGErrorMessage( &msg );
        Application->MessageBox( msg, "BMG Error", MB_OK | MB_ICONERROR );
        //return -1;
    }

    // read data from temp file
    // AnsiString tempFileName = "tmp.jpg";
    FILE *fpTemp;
    if ((fpTemp = fopen(tempFileName.c_str(), "rb")) == NULL)
        return NULL;
    fseek( fpTemp, 0, SEEK_END );
    long lFileSize = ftell( fpTemp );
    /*
    long lDataLen = lFileSize;
    if (lDataLen % 4 != 0)
        lDataLen += 4 - (lDataLen % 4);

    fseek( fpTemp, 0, SEEK_SET );
    unsigned char *pszBuffer;
    pszBuffer = new unsigned char[lDataLen + 1];

    fread( pszBuffer, 1, lFileSize, fpTemp );
    */
    fclose(fpTemp);
    return lFileSize;
}
long GetRoleData(RoleImages *pCurRole)
{ // adopted from m_WriteRoleData(FILE *fp, RoleImages *pCurRole)
    // lDataLen = m_WriteJPEG(fp, pCurRole->m_pBitmap, 100);
    long lDataLen = 0;
    lDataLen = GetJPEGData(pCurRole->m_pBitmap, 100);
    return lDataLen;
}
//----------------------------------------------------------------------------
bool XpgMovies::m_SaveFile( const char *filename, int quality )
{
    if (m_boXPW == false)
        return m_WriteNewFormatXPGFile(filename);
    //////////////////////////////////////////////////////    
    FILE *fp = NULL;		/* target file */
    bool boNoErr = true;

    if ((fp = fopen(filename, "wb")) == NULL)
        return false;

	m_iQuality = quality;
    m_InitHeader(false);

    // reserve 512 bytes for file header
    if (m_boXPW)
        fseek( fp, 1024, SEEK_SET );
    else
        fseek( fp, 512, SEEK_SET );
    if (boNoErr)
    {
        // Write Role Image Data
        m_lImageHeaderPos = ftell(fp);
        fseek( fp, m_lImageHeaderPos + m_iRoleCount * m_iImageHeaderLen, SEEK_SET );
        for (int iRole = 0; iRole < m_iRoleCount; iRole++)
        {

            m_pCurRole = (RoleImages *)m_RoleList->Items[iRole];
            m_pCurRole->m_iIndex = iRole;
            m_pCurRole->m_lFilePos = ftell(fp);

            // if SaveXPW then Set off in XPW4, if SaveXPG then save
            if( m_boXPW ) ; // skip  when SaveXPW
            else  m_pCurRole->m_lDataLen = m_WriteRoleData(fp, m_pCurRole); // SaveXPG

            if (m_boXPW)
            {
                // extra write source
                if (m_pCurRole->m_iImgSourceSize <= 0)
                { // Convert RoleData to ImgSource, adopted from
                        long lRoleData=0;
                        lRoleData = GetRoleData(m_pCurRole);  // m_WriteRoleData(FILE *fp, RoleImages *pCurRole)
                        m_pCurRole->m_iImgSourceSize = lRoleData;
                        fwrite(&(m_pCurRole->m_iImgSourceSize), 4, 1, fp);

                        AnsiString tempFileName = "tmp.jpg"; // read data from temp file
                        FILE *fpTemp;
                        if ((fpTemp = fopen(tempFileName.c_str(), "rb")) == NULL)
                                return NULL;
                        fseek( fpTemp, 0, SEEK_END );
                        long lFileSize = ftell( fpTemp );
                        long lDataLen = lFileSize;
                        if (lDataLen % 4 != 0)
                                lDataLen += 4 - (lDataLen % 4);
                        fseek( fpTemp, 0, SEEK_SET );
                        unsigned char *pszBuffer;
                        pszBuffer = new unsigned char[lDataLen + 1];

                        fread( pszBuffer, 1, lFileSize, fpTemp );
                        fclose(fpTemp);
                        fwrite(pszBuffer, 1, m_pCurRole->m_iImgSourceSize, fp);
                        delete pszBuffer;

                }
                else
                {
                        fwrite(&(m_pCurRole->m_iImgSourceSize), 4, 1, fp);
                        if (m_pCurRole->m_iImgSourceSize > 0)
                                fwrite(m_pCurRole->m_pImgSource, 1, m_pCurRole->m_iImgSourceSize, fp);
                }
            }

            if (m_pCurRole->m_lDataLen < 0) {
                boNoErr = false;
                break;
            }
        }

        m_lPageHeaderPos = ftell(fp);
        // Write Role Header
        for (int iRole = 0; iRole < m_iRoleCount; iRole++)
        {
            fseek( fp, m_lImageHeaderPos + iRole * m_iImageHeaderLen, SEEK_SET );
            m_pCurRole = (RoleImages *)m_RoleList->Items[iRole];
            m_WriteRoleHeader(fp, m_pCurRole);
        }

        // Write Page's Sprite & Script Data
        m_iPageCount = m_PageList->Count;
        fseek( fp, m_lPageHeaderPos + m_iPageCount * m_iPageHeaderLen, SEEK_SET );
        for (int iPage = 0; iPage < m_iPageCount; iPage++)
        {
            // Write Sprite Data
            Pages *pPage = (Pages *)m_PageList->Items[iPage];
            pPage->m_iIndex = iPage;
            pPage->m_lSpriteFilePos = ftell(fp);

            pPage->m_iSpriteCount = pPage->m_SpriteList->Count;
            for (int iSprite = 0; iSprite < pPage->m_iSpriteCount; iSprite++)
            {
                Sprites *pSprite = (Sprites *)(pPage->m_SpriteList->Items[iSprite]);
                pSprite->m_iIndex = iSprite;
                m_WriteSpriteData(fp, pSprite);
            }

            // Write Script Data
            int iCommandCount = pPage->GetCommandCount();

            pPage->m_lScriptFilePos = 0;
            if (iCommandCount > 0)
            {
                pPage->m_lScriptFilePos = ftell(fp);

                fwrite(&iCommandCount, 4, 1, fp);
                for (int iScript = 0; iScript < XPG_COMMAND_COUNT; iScript++)
                {
                	if (pPage->m_Command[iScript].m_boEmpty)
                       continue;                    

                    AnsiString strButton = 	pPage->GetCommandButton(iScript); 
					AnsiString strPage = 	pPage->GetCommandPage(iScript);
					AnsiString strAction = 	pPage->GetCommandAction(iScript);
				
					WORD wButtonKey = m_GetButtonKey(strButton) - 1;
					WORD wPageKey   = m_GetPageKey(strPage);
					WORD wActionKey = m_GetActionKey(strAction) + 1;  
					WORD wTemp = 0;
					
					fwrite(&(wButtonKey), 2, 1, fp);
					fwrite(&(wTemp), 2, 1, fp);
					
					fwrite(&(wPageKey), 2, 1, fp);
					fwrite(&(wTemp), 2, 1, fp);
					
					fwrite(&(wActionKey), 2, 1, fp);
					fwrite(&(wTemp), 2, 1, fp);

                    if (m_boXPW) {
                        char buf[33];
                        memset(buf, 0, 33);
                        strcpy( buf, strPage.c_str() );
                        fwrite((buf), 1, 16, fp);
                        strcpy( buf, strAction.c_str() );
                        fwrite((buf), 1, 32, fp);
                    }   
                }
            }
        }

        fputc(0, fp);
        fputc(0, fp);
        fputc(0, fp);
        fputc(0, fp);

        // Write Page Header
        for (int iPage = 0; iPage < m_iPageCount; iPage++)
        {
            fseek( fp, m_lPageHeaderPos + iPage * m_iPageHeaderLen, SEEK_SET );
            m_WritePageHeader(fp, (Pages *)(m_PageList->Items[iPage]));
        }
    }

    if (boNoErr) {
        fseek( fp, 0, SEEK_SET );
        boNoErr = m_WriteHeader(fp);
    }

    if (boNoErr) {
        fseek( fp, 0, SEEK_END );
        long lFileSize = ftell(fp);
        //char c = fgetc(fp);
        if (lFileSize % 4 != 0) {
            int n = 4 - (lFileSize % 4);
            for (int i = 0; i < n; i++)
                fputc(0, fp);

        }
    }

    fclose(fp);
    return boNoErr;
}

//----------------------------------------------------------------------------
/*  format	            1	0	jpg, png, gif. Or other flag
    Pixel depth	        1	1
    Pixel type	        4	2	444, 555, 888
    width	            2	6
    height	            2	8
    Image Data Length	4	10
    Image Data Position	4	14
    len = 24
*/
bool XpgMovies::m_WriteRoleHeader(FILE *fp, RoleImages *curRole)
{
    if (fp == NULL) return false;
      switch (curRole->m_iType) {
          case 0 : // 24bits JPEG
              curRole->m_cBpp = 24;
              break;
          case 1 : // 1 bit OSD
              curRole->m_cBpp = 1;
              break;
          case 2 : // 4 bits OSD
              curRole->m_cBpp = 4;
              break;
          case 3 : // 8 bits OSD
              curRole->m_cBpp = 7;
              break;
          case 4 : // 1bit BITMAP
              curRole->m_cBpp = 1;
              break;
          case 5 : // 24bits JPEG Transperant
              curRole->m_cBpp = 24;
              break;
          case 6 : // 8 bits OSD Transperant
              curRole->m_cBpp = 7;
              break;
          case 7 : // fileName only
              curRole->m_cBpp = 0;
              break;
          case 8 : // Image source
              curRole->m_cBpp = 0;
              break;
      }


    fputc(curRole->m_iFormat & 0xff, fp); //IF_JPEG
    fputc(curRole->m_cBpp & 0xff, fp);
    fputc(curRole->m_iType & 0xff, fp);
    fputc(curRole->m_iQuality & 0xff, fp);

    //int iPixelType = 0x888;
    //fwrite(&iPixelType, 4, 1, fp);

    /* TODO : Please double checkRLE: RLE status flag*/
    int iType1 = curRole->m_iType;
    if (curRole->m_boOSD) iType1 |= 1;
    if (curRole->m_boTrans) iType1 |= 0x10000000;
	iType1 |= (curRole->m_cBpp & 0xff) << 8;
    fwrite(&(iType1), 4,1,fp);

    fwrite(&(curRole->m_iWidth), 4, 1, fp);
    fwrite(&(curRole->m_iHeight), 4, 1, fp);

    fwrite(&(curRole->m_lFilePos), 4, 1, fp);
    fwrite(&(curRole->m_lDataLen), 4, 1, fp);

    int n = curRole->m_Name.Length();
    curRole->m_lHashKey = xpgHash(curRole->m_Name.c_str(), n);
    fwrite(&(curRole->m_lHashKey), 4, 1, fp);
    if (m_boXPW) {
        char buf[64];
        memset(buf, 0, 64);
        strcpy( buf, curRole->m_Name.c_str() );
        fwrite(buf, 1, 16, fp);

        strcpy( buf, curRole->m_FileName.c_str() );
        fwrite(buf, 1, 32, fp);
    }
    return true;
}

//-----------------------------------------------------------------------
long XpgMovies::m_WriteRoleData(FILE *fp, RoleImages *pCurRole)
{
    if (fp == NULL) return false;

	long lDataLen = 0;

	if (pCurRole->m_pBitmap == NULL) return 0;
    if ((!m_boXPW && !m_CheckRoleUsed(pCurRole->m_iIndex))) {
        return 0;
    }

    /* TODO : Please doulbe-check: Check whether to use RLE or JPEG encoding */
    /*
    24bits JPEG
    1 bit BITMAP
    4 bits BITMAP
    7 bits BITMAP
    8 bits BITMAP
    24bits JPEG Transperant
    8 bits RGB BITMAP
    FileName only
    */
	if (m_boXPW) {
	  	lDataLen = m_WriteJPEG(fp, pCurRole->m_pBitmap, 100);

	} else {
        int iQuality = 100;
		switch (pCurRole->m_iType) {
			case 0 : // 24bits JPEG

				switch (pCurRole->m_iQuality) {
					case 0 :
						iQuality = m_iQuality;
						break;
					case 1 :
						iQuality = 60;
						break;
					case 2 :
						iQuality = 70;
						break;
					case 3 :
						iQuality = 80;
						break;
					case 4 :
						iQuality = 85;
						break;
					case 5 :
						iQuality = 90;
						break;
					case 6 :
						iQuality = 99;
						break;
					case 7 :
						iQuality = 100;
						break;
				}

				lDataLen = m_WriteJPEG(fp, pCurRole->m_pBitmap, iQuality);
				break;
			case 1 : // 1 bit Bitmap
				pCurRole->m_cBpp = 1;
				lDataLen = m_Write1BitsBitmap(fp, pCurRole->m_pBitmap);
				break;
			case 2 : // 4 bits Bitmap
				pCurRole->m_cBpp = 4;
				lDataLen = m_Write4BitsBitmap(fp, pCurRole->m_pBitmap);		
				break;
			case 3 : // 7 bits Bitmap
				pCurRole->m_cBpp = 7;
				lDataLen = m_WriteIndexColorImage(fp, pCurRole->m_pBitmap, 120);
				//lDataLen = m_WriteOSD(fp, pCurRole->m_pBitmap);
                break;
			case 4 : // 8 bits BITMAP
				pCurRole->m_cBpp = 8;
				lDataLen = m_WriteIndexColorImage(fp, pCurRole->m_pBitmap, 256);
                break;
			case 5 : // 24bits JPEG Transperant
				pCurRole->m_cBpp = 24;
				lDataLen = m_WriteJPEG(fp, pCurRole->m_pBitmap, 100);
                break;
			case 6 : // 8 bits rgb BITMAP
				pCurRole->m_cBpp = 8;
				lDataLen = m_WriteRgbIndexColorImage(fp, pCurRole->m_pBitmap, 256);
				break;
            case 7 : // fileName only
				pCurRole->m_cBpp = 0;
                lDataLen = fputs(pCurRole->m_Name.c_str(), fp);
				break;
            case 8 : // Image source
				pCurRole->m_cBpp = 0;
                lDataLen = pCurRole->m_iImgSourceSize;
                fwrite(pCurRole->m_pImgSource, 1, lDataLen, fp);
				break;
		}

	}
    return lDataLen;
}
//-----------------------------------------------------------------------
long XpgMovies::m_WriteJPEG( FILE *fp, Graphics::TBitmap *Bitmap, int quality )
{
    if (Bitmap->Handle == NULL) return -1;

    AnsiString tempFileName = "tmp.jpg";

    if (FileExists(tempFileName)) DeleteFile(tempFileName);

    //WriteJpegFile to temp file first
    if ( SaveBitmapToFile( Bitmap->Handle, tempFileName.c_str(),
                     (void *)&quality ) != BMG_OK )
    {
        const char *msg;
        GetLastBMGErrorMessage( &msg );
        Application->MessageBox( msg, "BMG Error", MB_OK | MB_ICONERROR );
        return -1;
    }

    // read data from temp file
    FILE *fpTemp;
    if ((fpTemp = fopen(tempFileName.c_str(), "rb")) == NULL)
        return NULL;
    fseek( fpTemp, 0, SEEK_END );
    long lFileSize = ftell( fpTemp );
    long lDataLen = lFileSize;
    if (lDataLen % 4 != 0)
        lDataLen += 4 - (lDataLen % 4);

    fseek( fpTemp, 0, SEEK_SET );
    unsigned char *pszBuffer;
    pszBuffer = new unsigned char[lDataLen + 1];

    fread( pszBuffer, 1, lFileSize, fpTemp );
    fclose(fpTemp);

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

    if (lFileSize <= 0) return -1;
    fwrite(pszBuffer, 1, lDataLen, fp);

    delete pszBuffer;

    return lFileSize;
}

//-----------------------------------------------------------------------
bool XpgMovies::m_WriteSpriteData(FILE *fp, Sprites *pSprite)
{
/*
Layer	    2	0
X	        2	2
Y	        2	4
Image Index	2	6
ink	        1	8
ink value	3	9
  len = 20
*/
    if (fp == NULL) return false;

    fwrite(&(pSprite->m_iIndex), 4, 1, fp);

    fwrite(&(pSprite->m_iPx), 4, 1, fp);
    fwrite(&(pSprite->m_iPy), 4, 1, fp);
    fwrite(&(pSprite->m_iRole), 4, 1, fp);

	int iType = (pSprite->m_iType << 16) | pSprite->m_iTypeIndex;
    fwrite(&(iType), 4, 1, fp);

    int n = pSprite->m_Name.Length();
    pSprite->m_lHashKey = xpgHash(pSprite->m_Name.c_str(), n);
    fwrite(&(pSprite->m_lHashKey), 4, 1, fp);

    // anyka platform ext
    int tempvar = 0;
    fwrite(&(pSprite->m_touchEnable), 4, 1, fp);
    fwrite(&(pSprite->m_touchFlag), 4, 1, fp);
    fwrite(&tempvar, 4, 1, fp);                         // 保留
    fwrite(&tempvar, 4, 1, fp);
    fwrite(&tempvar, 4, 1, fp);
    fwrite(&tempvar, 4, 1, fp);
    fwrite(&tempvar, 4, 1, fp);
    fwrite(&tempvar, 4, 1, fp);
    
    if (m_boXPW) {
        char buf[17];
        strcpy( buf, pSprite->m_Name.c_str() );
        fwrite((buf), 1, 16, fp);
    }
    return true;
}
//-----------------------------------------------------------------------
bool XpgMovies::m_WritePageHeader(FILE *fp, Pages *pPage)
{
/*
Page Index	                2	0
Page Script Index	        2	2
Sprite Number	            2	4
Sprite Data Start Position	4	6
len = 16
*/
    if (fp == NULL) return false;

    fwrite(&(pPage->m_iIndex), 4, 1, fp);
    fwrite(&(pPage->m_lScriptFilePos), 4, 1, fp);
    fwrite(&(pPage->m_iSpriteCount), 4, 1, fp);
    fwrite(&(pPage->m_lSpriteFilePos), 4, 1, fp);

    int n = pPage->m_Name.Length();
    pPage->m_lHashKey = xpgHash(pPage->m_Name.c_str(), n);
    fwrite(&(pPage->m_lHashKey), 4, 1, fp);


    // xpw version 1000 only
    // xpw version 1001 add PageEnterScript PageExitScript
    if (m_boXPW) {
        char buf[17];
        memset(buf, 0, 17);
        strcpy( buf, pPage->m_Name.c_str() );
        fwrite((buf), 1, 16, fp);

        char buf1[20];
        memset(buf1, 0, 20);
        fwrite((buf1), 1, 20, fp);
    }
    return true;
}
///////////////////////////////////////////////////////////////////////////////
struct TXpgFileHead {
    char name[100];    // name  name of file
    char mode[8];      // mode  file mode
    char uid[8];       // uid  owner user ID
    char gid[8];       // gid  owner group ID
    char size[12];     // size  length of file in bytes
    char mtime[12];    // mtime  modify time of file
    char chksum[8];    // chksum  checksum for header
    char typeflag[1];  // typeflag  type of file
    char linkname[100];// linkname  name of linked file
    char magic[6];     // magic  USTAR indicator
    char version[2];   // version  USTAR version
    char uname[32];    // uname  owner user name
    char gname[32];    // gname  owner group name
    char devmajor[8];  // devmajor  device major number
    char devminor[8];  // devminor  device minor number
    char prefix[155];  // prefix  prefix for file name
};
struct TPageSpriteInfo {
    unsigned int uSprite0_offset;
    unsigned int uSprite_num;
    unsigned int uScript_offset;
    unsigned int uCmd_num;
};
///////////////////////////////////////////////////////////////////////////////
int writeTarHead(FILE* fp, const char* filename, unsigned int size)
{
    char tmpbuf[64];
    unsigned char sector[512];
    unsigned sum, i;
    TXpgFileHead  head;
    memset(sector, 0, 512);
    memset(&head, 0, sizeof(head));
    
    strcpy(head.name, filename);
    strcpy(head.mode, "0100777");
    strcpy(head.uid, "0000000");
    strcpy(head.gid, "0000000");
    memset(tmpbuf, 0, sizeof(tmpbuf));
    sprintf(tmpbuf, "%011o", (int)size);
    strncpy(head.size, tmpbuf, 11);
    memset(tmpbuf, 0, sizeof(tmpbuf));
    sprintf(tmpbuf, "%011o", (int)time(NULL));
    strncpy(head.mtime, tmpbuf, 11);
    head.typeflag[0] = '0';
    strcpy(head.magic, "ustar");
    head.version[0] = '0';
    head.version[1] = '0';
    memset(head.chksum, ' ', sizeof(head.chksum));
    memcpy(sector, &head, sizeof(head));
    
    sum = 0;
    for (i = 0; i < 512; i++)
        sum += sector[i];
    memset(tmpbuf, 0, sizeof(tmpbuf));
    sprintf(tmpbuf, "%06o", (int)sum);
    strncpy(head.chksum, tmpbuf, 7);

    memcpy(sector, &head, sizeof(head));
    fwrite(sector, 1, 512, fp);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
int writeTarData(FILE* fp, const unsigned char* buf, unsigned int size)
{
    int rsize = (int)size;
    int wsize = 0;
    unsigned char sector[512];

    while (rsize > 0)
    {
        if (rsize >= 512)
        {
            fwrite(&buf[wsize], 1, 512, fp);
            wsize += 512;
            rsize -= 512;
        }
        else
        {
            memset(sector, 0, sizeof(sector));
            memcpy(sector, &buf[wsize], rsize);
            fwrite(sector, 1, 512, fp);
            wsize += rsize;
            rsize -= rsize;
        }
    }
    
    return wsize;
}
///////////////////////////////////////////////////////////////////////////////
const char * getFileExtByContent(const unsigned char* buf)
{
    if (buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G')
        return "png";
    else if (buf[0] == 0xff && buf[1] == 0xd8)
        return "jpg";
    else if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
        return "gif";
    else if (buf[0] == 'B' && buf[1] == 'M')
        return "bmp";
    else
        return "bin";
}
///////////////////////////////////////////////////////////////////////////////
bool XpgMovies::m_WriteNewFormatXPGFile(const char *filename)
{
    static char tmpbuf[128];
    FILE * xpg = NULL;
    bool boNoErr = true;
    std::vector<TPageSpriteInfo>  allpage;

    std::string tempdir;
    std::string tempfile;
    const char * p;
    unsigned int uRoleTabOffset, uRoleNum;

    tempdir = getenv("TEMP");
    p = tempdir.c_str();
    if (p[strlen(p) - 1] != '\\')
        tempdir += "\\";

    sprintf(tmpbuf, "zzzzzzz_%05d%05d.xpg", rand(), rand());
    tempfile = tempdir + tmpbuf;
    
    xpg = fopen(tempfile.c_str(), "wb");
    if (xpg == NULL)
        return false;

    memset(tmpbuf, 0, 128);
    fwrite(tmpbuf, 1, 128, xpg);

    // 写入ROLE表
    uRoleTabOffset = ftell(xpg);
    uRoleNum = m_iRoleCount;
    for (int iRole = 0; iRole < m_iRoleCount; iRole++)
    {
        RoleImages * curRole = (RoleImages *)m_RoleList->Items[iRole];
        fwrite(&(curRole->m_iWidth), 4, 1, xpg);
        fwrite(&(curRole->m_iHeight), 4, 1, xpg);
        if (curRole->m_iImgSourceSize <= 0)
        {
            fwrite(&(curRole->m_iImgSourceSize), 4, 1, xpg);
        }
        else
        {
            fwrite(&(curRole->m_iImgSourceSize), 4, 1, xpg);
        }
        switch (curRole->m_iType) {
          case 0 : // 24bits JPEG
              curRole->m_cBpp = 24;
              break;
          case 1 : // 1 bit OSD
              curRole->m_cBpp = 1;
              break;
          case 2 : // 4 bits OSD
              curRole->m_cBpp = 4;
              break;
          case 3 : // 8 bits OSD
              curRole->m_cBpp = 7;
              break;
          case 4 : // 1bit BITMAP
              curRole->m_cBpp = 1;
              break;
          case 5 : // 24bits JPEG Transperant
              curRole->m_cBpp = 24;
              break;
          case 6 : // 8 bits OSD Transperant
              curRole->m_cBpp = 7;
              break;
          case 7 : // fileName only
              curRole->m_cBpp = 0;
              break;
          case 8 : // Image source
              curRole->m_cBpp = 0;
              break;
        }
        fputc(curRole->m_iFormat & 0xff, xpg); //JPG, PNG
        fputc(curRole->m_cBpp & 0xff, xpg);
        fputc(curRole->m_iType & 0xff, xpg);
        fputc(0, xpg);
        
        //int n = curRole->m_Name.Length();
        //curRole->m_lHashKey = xpgHash(curRole->m_Name.c_str(), n);
        //fwrite(&(curRole->m_lHashKey), 4, 1, xpg);
    }

    // 写入PAGE表和脚本
    allpage.clear();
    
    m_iPageCount = m_PageList->Count;
    for (int iPage = 0; iPage < m_iPageCount; iPage++)
    {
        TPageSpriteInfo  myPageSpritesInfo;
        
        // Write Sprite Data
        Pages *pPage = (Pages *)m_PageList->Items[iPage];
        pPage->m_iIndex = iPage;
        pPage->m_lSpriteFilePos = ftell(xpg);
        pPage->m_iSpriteCount = pPage->m_SpriteList->Count;

        myPageSpritesInfo.uSprite0_offset = ftell(xpg);
        myPageSpritesInfo.uSprite_num = pPage->m_iSpriteCount;

        for (int iSprite = 0; iSprite < pPage->m_iSpriteCount; iSprite++)
        {
            Sprites *pSprite = (Sprites *)(pPage->m_SpriteList->Items[iSprite]);
            pSprite->m_iIndex = iSprite;
            
            fwrite(&(pSprite->m_iIndex), 4, 1, xpg);            // Layer
            fwrite(&(pSprite->m_iPx), 4, 1, xpg);               // X
            fwrite(&(pSprite->m_iPy), 4, 1, xpg);               // Y
            fwrite(&(pSprite->m_iRole), 4, 1, xpg);             // Image Index
            int iType = (pSprite->m_iType << 16) | pSprite->m_iTypeIndex;
            fwrite(&(iType), 4, 1, xpg);

            fwrite(&(pSprite->m_touchEnable), 4, 1, xpg);       // touch Enable
            fwrite(&(pSprite->m_touchFlag), 4, 1, xpg);         // touch Flag
        }
        
        // Write Script Data
        myPageSpritesInfo.uScript_offset = ftell(xpg);
        int iCommandCount = pPage->GetCommandCount();
        pPage->m_lScriptFilePos = 0;
        pPage->m_lScriptFilePos = ftell(xpg);
        if (iCommandCount > 0)
        {
            fwrite(&iCommandCount, 4, 1, xpg);
            for (int iScript = 0; iScript < XPG_COMMAND_COUNT; iScript++)
            {
                if (pPage->m_Command[iScript].m_boEmpty)
                    continue;                    

                AnsiString strButton = 	pPage->GetCommandButton(iScript);
                AnsiString strPage = 	pPage->GetCommandPage(iScript);
                AnsiString strAction = 	pPage->GetCommandAction(iScript);
				
                WORD wButtonKey = m_GetButtonKey(strButton) - 1;
                WORD wPageKey   = m_GetPageKey(strPage);
                WORD wActionKey = m_GetActionKey(strAction) + 1;
                WORD wTemp = 0;
					
                fwrite(&(wButtonKey), 2, 1, xpg);
                fwrite(&(wPageKey), 2, 1, xpg);
                fwrite(&(wActionKey), 2, 1, xpg);
                fwrite(&(wTemp), 2, 1, xpg);
            }
        }
        else
        {
            iCommandCount = 0;
            fwrite(&iCommandCount, 4, 1, xpg);
        }
        myPageSpritesInfo.uCmd_num = iCommandCount;
        
        allpage.push_back(myPageSpritesInfo);
    }

    // Write Page Header
    unsigned int uPageHeadOffset, uPageNum;
    uPageHeadOffset = ftell(xpg);
    uPageNum = m_iPageCount;
    
    for (int iPage = 0; iPage < m_iPageCount; iPage++)
    {
        unsigned int temp;
        Pages *pPage = (Pages *)m_PageList->Items[iPage];
        
        fwrite(&(pPage->m_iIndex), 4, 1, xpg);
        
        temp = allpage[iPage].uSprite0_offset;
        fwrite(&temp, 4, 1, xpg);
        temp = allpage[iPage].uSprite_num;
        fwrite(&temp, 4, 1, xpg);
        temp = allpage[iPage].uScript_offset;
        fwrite(&temp, 4, 1, xpg);
        temp = allpage[iPage].uCmd_num;
        fwrite(&temp, 4, 1, xpg);

        int n = pPage->m_Name.Length();
        pPage->m_lHashKey = xpgHash(pPage->m_Name.c_str(), n);
        fwrite(&(pPage->m_lHashKey), 4, 1, xpg);
    }
    fflush(xpg);
    
    fseek(xpg, 0, SEEK_SET);
    fwrite("XPG6", 4, 1, xpg);
    unsigned dwTemp = 1;
    fwrite(&dwTemp, 4, 1, xpg);

    fwrite(&uRoleTabOffset, 4, 1, xpg);
    fwrite(&uRoleNum, 4, 1, xpg);
    fwrite(&uPageHeadOffset, 4, 1, xpg);
    fwrite(&uPageNum, 4, 1, xpg);
    fflush(xpg);
    fclose(xpg);

    ///////////////////////////////////////////////////////

    unsigned char * pXpgIndexData;
    unsigned int    dwIndexDataSize;
    xpg = fopen(tempfile.c_str(), "rb");
    if (xpg == NULL)
        return false;
    dwIndexDataSize = filelength(fileno(xpg));
    pXpgIndexData = (unsigned char *) malloc(dwIndexDataSize);
    fread(pXpgIndexData, 1, dwIndexDataSize, xpg);
    fclose(xpg);
    unlink(tempfile.c_str());
    
    ///////////////////////////////////////////////////////

    xpg = fopen(filename, "wb");
    if (xpg == NULL)
        return false;
    fseek(xpg, 0, SEEK_SET);    
    writeTarHead(xpg, "index.xpg", dwIndexDataSize);
    writeTarData(xpg, pXpgIndexData, dwIndexDataSize);
    free(pXpgIndexData);

    for (int iRole = 0; iRole < m_iRoleCount; iRole++)
    {
        char picName[64];
        m_pCurRole = (RoleImages *)m_RoleList->Items[iRole];
        m_pCurRole->m_iIndex = iRole;

        if (m_pCurRole->m_iImgSourceSize <= 0)
        {
            // Convert RoleData to ImgSource, adopted from
            long lRoleData=0;
            lRoleData = GetRoleData(m_pCurRole);                // m_WriteRoleData(FILE *fp, RoleImages *pCurRole)
            m_pCurRole->m_iImgSourceSize = lRoleData;

            AnsiString tempFileName = "tmp.jpg"; // read data from temp file
            FILE *fpTemp;
            if ((fpTemp = fopen(tempFileName.c_str(), "rb")) == NULL)
                return NULL;
            fseek( fpTemp, 0, SEEK_END );
            long lFileSize = ftell( fpTemp );
            long lDataLen = lFileSize;
            if (lDataLen % 4 != 0)
                lDataLen += 4 - (lDataLen % 4);
            fseek( fpTemp, 0, SEEK_SET );
            
            unsigned char *pszBuffer;
            pszBuffer = new unsigned char[lDataLen + 8];
            memset(pszBuffer, 0, lDataLen + 8);
            fread( pszBuffer, 1, lFileSize, fpTemp );
            fclose(fpTemp);

            sprintf(picName, "%04d.jpg", iRole, getFileExtByContent(pszBuffer));
            writeTarHead(xpg, picName, m_pCurRole->m_iImgSourceSize);
            if (m_pCurRole->m_iImgSourceSize > 0)
                writeTarData(xpg, pszBuffer, m_pCurRole->m_iImgSourceSize);

            delete[] pszBuffer;

        }
        else
        {
            sprintf(picName, "%04d.%s", iRole, getFileExtByContent(m_pCurRole->m_pImgSource));
            
            writeTarHead(xpg, picName, m_pCurRole->m_iImgSourceSize);
            if (m_pCurRole->m_iImgSourceSize > 0)
                writeTarData(xpg, m_pCurRole->m_pImgSource, m_pCurRole->m_iImgSourceSize);
        }
    }

    fclose(xpg);
    
    return boNoErr;
}


#if 1
//**********************************************************************************
// RGB2YUV
// This routine converts RGB color to YUV color.
//
// INPUT:
//	BYTE R - Red
//	BYTE G - Green
//	BYTE B - Blue
// OUTPUT:
//	NONE
//**********************************************************************************
DWORD RGB2YUV(DWORD color)
{
	DWORD R, G, B;
	DWORD Y, Cb, Cr;

	R = (color >> 0) & 0xff;
	G = (color >> 8) & 0xff;
	B = (color >> 16) & 0xff;	

	Cb = ((-151 * R) + (-297 * G) + (449 * B) + 131072) >> 10;
	Cr = ((449 * R) + (-376 * G) + (-73 * B) + 131072) >> 10;
    Y = ((263 * R) + (514 * G) + (100 * B)) >> 10;

	return ((Y & 0xff) << 0) | ((Y & 0xff) << 8) | ((Cb & 0xff) << 16) | ((Cr & 0xff) << 24);
}

DWORD RGBPixel2YUV(DWORD color1, DWORD color2)
{
	DWORD R1, G1, B1, R2, G2, B2;
	DWORD Y, Cb, Cr;

	R1 = (color1 >> 0) & 0xff;
	G1 = (color1 >> 8) & 0xff;
	B1 = (color1 >> 16) & 0xff;	
	R2 = (color2 >> 0) & 0xff;
	G2 = (color2 >> 8) & 0xff;
	B2 = (color2 >> 16) & 0xff;

	R1 = (R1 + R2) >> 1;
	G1 = (G1 + G2) >> 1;
	B1 = (B1 + B2) >> 1;
	Cb = ((-151 * R1) + (-297 * G1) + (449 * B1) + 131072) >> 10;
	Cr = ((449 * R1) + (-376 * G1) + (-73 * B1) + 131072) >> 10;
    Y = ((263 * R1) + (514 * G1) + (100 * B1)) >> 10;

	return (Y << 16) | (Cb << 8) | Cr;
}

DWORD YUV_Buffer[2048][2048];
//----------------------------------------------------------------------------
long XpgMovies::m_WriteRLE( FILE *fp, Graphics::TBitmap *Bitmap )
{
    int wd = Bitmap->Width;
    int ht = Bitmap->Height;
    long pic_length = 0;
    int x, y, i;
    TCanvas *pCanvas = Bitmap->Canvas;

    fwrite(&wd, 2, 1, fp);
    fwrite(&ht, 2, 1, fp);
    //unsigned long startPos = ftell(fp);

    DWORD c0, c1;
    for (y = 0; y < ht; y++) {
	    for(x = 0, i = 0; ; x+=2, i++ ) {
            c0 = pCanvas->Pixels[x][y];
            c1 = (x + 1 < wd) ? pCanvas->Pixels[x+1][y] : 0;
            YUV_Buffer[i][y] = RGBPixel2YUV(c0, c1);
            if (x >= wd) break;
        }
    }
    wd >>= 1;
    for (y = 0; y < ht; y++) {
	    unsigned long color = YUV_Buffer[0][y];
	    unsigned char count = 0;
	    for(x = 0; x < wd+1; x++ ) {
            unsigned long cc = YUV_Buffer[x][y];
		    if( cc == color && count < 255 && x < wd )
			    count++;
		    else  {
                /* TODO : Please double-check: changed cc to color*/
                unsigned long c = (color & 0xffffff) | (count << 24);
			    fwrite(&c, 4, 1, fp);
                pic_length += 4;
			    count = 1;
			    color = cc;
            }
        }
    }

  /*  unsigned long endPos = ftell(fp);
    fseek( fp, SEED_SET, startPos );
    fwrite( &pic_length, 4, 1, fp );
    fseek( fp, SEED_SET, endPos );    */
    return pic_length;
}
//----------------------------------------------------------------------------
long XpgMovies::m_WriteOSD( FILE *fp, Graphics::TBitmap *Bitmap )
{
	if (Bitmap->Handle == NULL) return -1;

    long fPos0 = ftell(fp);
    SavePCX(fp, Bitmap);
    long pic_length = ftell(fp) - fPos0;
    return (pic_length);
}


void SwapBufferEndian(DWORD *pdwBuffer, int iBytes)
{
	DWORD dwData;

	for (int i = 0; i < iBytes; i += 4) {
		dwData = *pdwBuffer;
    	*pdwBuffer = ((dwData << 24) & 0xff000000) + ((dwData << 8) & 0x00ff0000)
            + ((dwData >> 8) & 0x0000ff00) + ((dwData >> 24) & 0xff);
		pdwBuffer++;
	}
}
//------------------------------------------------------------------------------
int findNearRgb(TColor palette[], TColor c, int iColorCount)
{
    int minVal[3] = {256, 256, 256};
    int minIndex = 0;

    for (int i = 1; i < iColorCount; i++) {
        TColor rgb = palette[i];
        int dr = abs((rgb & 0xff) - (c & 0xff));
        int dg = abs(((rgb >> 8) & 0xff) - ((c >> 8) & 0xff));
        int db = abs(((rgb >> 16) & 0xff) - ((c >> 16) & 0xff));
        if (dr < minVal[0] && dg < minVal[1] && db < minVal[2]) {
            minIndex = i;
            minVal[0] = dr;
            minVal[1] = dg;
            minVal[2] = db;
        }
    }
    return minIndex;
}
//------------------------------------------------------------------------------
int findExactSameRgb(TColor palette[], TColor c, int iColorCount, int offset)
{
    //if (c == 0) return 255;
    for (int i = 0; i < iColorCount; i++) {
        if (palette[i] == c) return i;
    }
    if (offset == 0) return -1;

    for (int i = 0; i < iColorCount; i++)
    {
        TColor rgb = palette[i];
        int dr = abs((rgb & 0xff) - (c & 0xff));
        int dg = abs(((rgb >> 8) & 0xff) - ((c >> 8) & 0xff));
        int db = abs(((rgb >> 16) & 0xff) - ((c >> 16) & 0xff));
        if (dr < offset && dg < offset && db < offset)
            return i;
    }

    return -1;
}
//------------------------------------------------------------------------------
int findSameRgb(TColor palette[], TColor c, int iColorCount)
{
    //if (c == 0) return 255;
    for (int i = 0; i < iColorCount; i++) {
        if (palette[i] == c) return i;
    }

    int i = findNearRgb( palette, c, iColorCount );

    TColor rgb = palette[i];
    int dr = abs((rgb & 0xff) - (c & 0xff));
    int dg = abs(((rgb >> 8) & 0xff) - ((c >> 8) & 0xff));
    int db = abs(((rgb >> 16) & 0xff) - ((c >> 16) & 0xff));
    if (dr < 16 && dg < 16 && db < 16)
        return i;

    return -1;
}
//------------------------------------------------------------------------------
int findEmptyRgb(TColor palette[], TColor c, int iColorCount)
{
    for (int i = 1; i < iColorCount; i++) {
        if (palette[i] == 0) {
            palette[i] = c;
            return i;
        }
    }
    return -1;
}

//------------------------------------------------------------------------------
long XpgMovies::m_WriteIndexColorImage(FILE *fp, Graphics::TBitmap *Bitmap, int iColorCount)
{
    int wd = Bitmap->Width;
    int ht = Bitmap->Height;

    TCanvas *pCanvas = Bitmap->Canvas;
	TColor tcRgbPalette[256];
    memset(tcRgbPalette, 0, iColorCount);

	tcRgbPalette[0] = (TColor)0;
	tcRgbPalette[1] = (TColor)0xffffff;


	int iPixelSize = wd * ht;
	if (iPixelSize & 3) iPixelSize += 4 - (iPixelSize & 3);	
	
	Byte *pbPixels = (Byte *)malloc(wd * ht);

    TColor *tempPal;
    int *tempColorCount;
    int iMaxPalCount;

    iMaxPalCount = (wd * ht) / 2;
    if (iMaxPalCount < iColorCount) iMaxPalCount = iColorCount;

    tempPal = (TColor *)malloc(iMaxPalCount * sizeof(TColor));
    tempColorCount = (int *)malloc(iMaxPalCount * sizeof(int));
    memset(tempColorCount, 0, iMaxPalCount);
    tempPal[0] = (TColor)0;
	tempPal[1] = (TColor)0xffffff;
	// convert to index color
    int iPalCount = 2;
    for (int i = 0, y = 0; y < ht; y++) {
        for (int x = 0; x < wd; x++, i++) {
            TColor c = pCanvas->Pixels[x][y];
            int idx = findExactSameRgb(tempPal, c, iPalCount, 8);
            if (idx == -1) {
                idx = findEmptyRgb(tempPal, c, iMaxPalCount); // maybe should find near color first
                if (idx >= iPalCount) iPalCount = idx + 1;
            }
            tempColorCount[idx]++;
            //pCanvas->Pixels[x][y] = tempPal[idx];
        }
    }

    if (iPalCount <= iColorCount) {
        for (int i = 2; i < iPalCount; i++) {
            tcRgbPalette[i] = tempPal[i];
        }
    } else {

        for (int i = 2; i < iPalCount; i++)
        {
            int iMaxVal = tempColorCount[i];
            int iMaxIndex = -1;
            for (int j = i + 1; j < iPalCount; j++) {
                if (tempColorCount[j] > iMaxVal) {
                    iMaxIndex = j;
                    iMaxVal = tempColorCount[j];
                }
            }
            if (iMaxIndex != -1) {
                tempColorCount[iMaxIndex] = tempColorCount[i];
                tempColorCount[i] = iMaxVal;

                iMaxVal = tempPal[iMaxIndex];
                tempPal[iMaxIndex] = tempPal[i];
                tempPal[i] = (TColor)iMaxVal;
            }
        }

        for (int i = 2; i < 32; i++) {
           tcRgbPalette[i] = tempPal[i];
        }
    }
    free(tempPal);
    free(tempColorCount);

	// convert to index color
    for (int i = 0, y = 0; y < ht; y++) {
        for (int x = 0; x < wd; x++, i++) {
            TColor c = pCanvas->Pixels[x][y];
            int idx = findSameRgb(tcRgbPalette, c, iColorCount);
            if (idx == -1) {
                idx = findEmptyRgb(tcRgbPalette, c, iColorCount); // maybe should find near color first
                if (idx == -1)
                    idx = findNearRgb(tcRgbPalette, c, iColorCount);
            }
            pbPixels[i] = idx;
            //pCanvas->Pixels[x][y] = tcRgbPalette[idx];
        }
    }

    int iBufferSize = iColorCount * 4 + iPixelSize;
	Byte *pbDestBuffer = (Byte *)malloc(iBufferSize);
    DWORD *pdwPalette = (DWORD *)pbDestBuffer;

    memset(pbDestBuffer, 0, iBufferSize);
    //convert rgb palette to yuv
	for (int i = 0; i < iColorCount; i++) {
		pdwPalette[i] = RGB2YUV(tcRgbPalette[i]);
	}

	// convert bitmap array to pcx run lenght pixels
    iBufferSize = iColorCount * 4; //skip palette
    char cc;
    for (int y = 0; y < ht; y++) {
        Byte *addr = pbPixels + (wd * y);
	    Byte color = addr[0];
	    Byte count = 0;
        cc = color;
        if (y == ht - 1) {
            cc = 0;
        }
	    for( int x = 0; x <= wd; x++ ) {
            if (iBufferSize + 2 >= (iColorCount * 4 + iPixelSize))
                break;
            if (x < wd)
                cc = addr[x];
            else {
                cc = 0;
            }

		    if( cc == color && count < 62 && x < wd )
			  	count++;
		    else  {
                if (color >= iColorCount)
                    color = 0;
				if( count == 1 && (color & 0xc0) != 0xc0 ) {
					pbDestBuffer[iBufferSize] = color;
				  	iBufferSize++;
				} else {
                    if (count == 0) {
                        count = 1;
                        color = cc;
                    }
					pbDestBuffer[iBufferSize] = count | 0xc0;     /* store number */
					iBufferSize++;
					pbDestBuffer[iBufferSize] = color;	          /* store color */
					iBufferSize++;
				}
				count = 1;
				color = cc;
            }
        }
    }

	if (iBufferSize & 3) iBufferSize += 4 - (iBufferSize & 3);

	SwapBufferEndian( (DWORD *)pbDestBuffer, iBufferSize );

    DWORD dwTemp = (iBufferSize & 0xffffff) | (iColorCount << 24);
    fwrite( &dwTemp, 4, 1, fp );
	fwrite( pbDestBuffer, 1, iBufferSize, fp );
	free(pbDestBuffer);
	free(pbPixels);
	
	return iBufferSize;
}

//------------------------------------------------------------------------------
long XpgMovies::m_WriteRgbIndexColorImage(FILE *fp, Graphics::TBitmap *Bitmap, int iColorCount)
{
    int wd = Bitmap->Width;
    int ht = Bitmap->Height;

    TCanvas *pCanvas = Bitmap->Canvas;
	TColor tcRgbPalette[256];
    memset(tcRgbPalette, 0, iColorCount);

	tcRgbPalette[0] = (TColor)0;
	tcRgbPalette[1] = (TColor)0xffffff;


	int iPixelSize = wd * ht;
	if (iPixelSize & 3) iPixelSize += 4 - (iPixelSize & 3);	
	
	Byte *pbPixels = (Byte *)malloc(wd * ht);

    TColor *tempPal;
    int *tempColorCount;
    int iMaxPalCount;

    iMaxPalCount = (wd * ht) / 2;
    if (iMaxPalCount < iColorCount) iMaxPalCount = iColorCount;

    tempPal = (TColor *)malloc(iMaxPalCount * sizeof(TColor));
    tempColorCount = (int *)malloc(iMaxPalCount * sizeof(int));
    memset(tempColorCount, 0, iMaxPalCount);
    tempPal[0] = (TColor)0;
	tempPal[1] = (TColor)0xffffff;
	// convert to index color
    int iPalCount = 2;
    for (int i = 0, y = 0; y < ht; y++) {
        for (int x = 0; x < wd; x++, i++) {
            TColor c = pCanvas->Pixels[x][y];
            int idx = findExactSameRgb(tempPal, c, iPalCount, 8);
            if (idx == -1) {
                idx = findEmptyRgb(tempPal, c, iMaxPalCount); // maybe should find near color first
                if (idx >= iPalCount) iPalCount = idx + 1;
            }
            tempColorCount[idx]++;
            //pCanvas->Pixels[x][y] = tempPal[idx];
        }
    }

    if (iPalCount <= iColorCount) {
        for (int i = 2; i < iPalCount; i++) {
            tcRgbPalette[i] = tempPal[i];
        }
    } else {

        for (int i = 2; i < iPalCount; i++)
        {
            int iMaxVal = tempColorCount[i];
            int iMaxIndex = -1;
            for (int j = i + 1; j < iPalCount; j++) {
                if (tempColorCount[j] > iMaxVal) {
                    iMaxIndex = j;
                    iMaxVal = tempColorCount[j];
                }
            }
            if (iMaxIndex != -1) {
                tempColorCount[iMaxIndex] = tempColorCount[i];
                tempColorCount[i] = iMaxVal;

                iMaxVal = tempPal[iMaxIndex];
                tempPal[iMaxIndex] = tempPal[i];
                tempPal[i] = (TColor)iMaxVal;
            }
        }

        for (int i = 2; i < 32; i++) {
           tcRgbPalette[i] = tempPal[i];
        }
    }
    free(tempPal);
    free(tempColorCount);

	// convert to index color
    for (int i = 0, y = 0; y < ht; y++) {
        for (int x = 0; x < wd; x++, i++) {
            TColor c = pCanvas->Pixels[x][y];
            int idx = findSameRgb(tcRgbPalette, c, iColorCount);
            if (idx == -1) {
                idx = findEmptyRgb(tcRgbPalette, c, iColorCount); // maybe should find near color first
                if (idx == -1)
                    idx = findNearRgb(tcRgbPalette, c, iColorCount);
            }
            pbPixels[i] = idx;
            //pCanvas->Pixels[x][y] = tcRgbPalette[idx];
        }
    }

    int iBufferSize = iColorCount * 4 + iPixelSize;
	Byte *pbDestBuffer = (Byte *)malloc(iBufferSize);
    DWORD *pdwPalette = (DWORD *)pbDestBuffer;

    memset(pbDestBuffer, 0, iBufferSize);
    //copy rgb palette
	for (int i = 0; i < iColorCount; i++) {
		pdwPalette[i] = (tcRgbPalette[i]);
	}

	// convert bitmap array to pcx run lenght pixels
    iBufferSize = iColorCount * 4; //skip palette
    char cc;
    for (int y = 0; y < ht; y++) {
        Byte *addr = pbPixels + (wd * y);
	    Byte color = addr[0];
	    Byte count = 0;
        cc = color;
        if (y == ht - 1) {
            cc = 0;
        }
	    for( int x = 0; x <= wd; x++ ) {
            if (iBufferSize + 2 >= (iColorCount * 4 + iPixelSize))
                break;
            if (x < wd)
                cc = addr[x];
            else {
                cc = 0;
            }

		    if( cc == color && count < 62 && x < wd )
			  	count++;
		    else  {
                if (color >= iColorCount)
                    color = 0;
				if( count == 1 && (color & 0xc0) != 0xc0 ) {
					pbDestBuffer[iBufferSize] = color;
				  	iBufferSize++;
				} else {
                    if (count == 0) {
                        count = 1;
                        color = cc;
                    }
					pbDestBuffer[iBufferSize] = count | 0xc0;     /* store number */
					iBufferSize++;
					pbDestBuffer[iBufferSize] = color;	          /* store color */
					iBufferSize++;
				}
				count = 1;
				color = cc;
            }
        }
    }

	if (iBufferSize & 3) iBufferSize += 4 - (iBufferSize & 3);

	SwapBufferEndian( (DWORD *)pbDestBuffer, iBufferSize );

    DWORD dwTemp = (iBufferSize & 0xffffff) | (iColorCount << 24);
    fwrite( &dwTemp, 4, 1, fp );
	fwrite( pbDestBuffer, 1, iBufferSize, fp );
	free(pbDestBuffer);
	free(pbPixels);
	
	return iBufferSize;
}
//----------------------------------------------------------------------------
long XpgMovies::m_Write4BitsBitmap(FILE *fp, Graphics::TBitmap *Bitmap)
{    
    return m_WriteIndexColorImage(fp, Bitmap, 16);
}

//----------------------------------------------------------------------------
long XpgMovies::m_Write1BitsBitmap( FILE *fp, Graphics::TBitmap *Bitmap )
{
    int wd = Bitmap->Width;
    int ht = Bitmap->Height;
    long pic_length = 0;
    int x, y, i;
    TCanvas *pCanvas = Bitmap->Canvas;
	Byte *pbDestBuffer, *pbDestPixel;
	WORD *pwDestBuffer;
	int iLineBytesWidth = (wd & 0x7) ? ((wd >> 3) + 1) : (wd >> 3);
	int iBufferSize = (iLineBytesWidth + 4) * ht;
	Byte *pbLinePixels = (Byte *)malloc(wd);

	if (iBufferSize & 3) iBufferSize += 4 - (iBufferSize & 3);
	pbDestBuffer = (Byte *)malloc(iBufferSize);
	pbDestPixel = pbDestBuffer;
	
    for (y = 0; y < ht; y++) 
	{
		int xStart = wd;
		//int xEnd = 0;
		int iPixelCount = 0;
		for(x = 0; x < wd; x++) {
			DWORD dwColor = pCanvas->Pixels[x][y];
			Byte r = dwColor & 0xff;
			Byte g = (dwColor & 0xff00) >> 8;
			Byte b = (dwColor & 0xff0000) >> 16;
			int iBright = ((263 * r) + (514 * g) + (100 * b)) >> 10;

			if (iBright < 0x7f) {
				pbLinePixels[x] = 1;
				iPixelCount++;
				if (xStart > x) xStart = x;
				//xEnd = x;
			}
			else
				pbLinePixels[x] = 0;
				
		}
		
	    for(x = 0; x < wd; x+=8) 
		{
			*pbDestPixel = 0;
			for (i = 0; i < 8; i++) 
			{
				if (pbLinePixels[x + i] == 1) 
					*pbDestPixel |= 1 << (7 - i);
			}
			
			pbDestPixel++;
			pic_length++;
        }
    }

	SwapBufferEndian( (DWORD *)pbDestBuffer, iBufferSize );

	fwrite( pbDestBuffer, 1, iBufferSize, fp );
	free(pbDestBuffer);

  /*  unsigned long endPos = ftell(fp);
    fseek( fp, SEED_SET, startPos );
    fwrite( &pic_length, 4, 1, fp );
    fseek( fp, SEED_SET, endPos );    */
    return pic_length;
}
#else

//----------------------------------------------------------------------------
long XpgMovies::m_WriteRLE( FILE *fp, Graphics::TBitmap *Bitmap )
{
    int wd = Bitmap->Width;
    int ht = Bitmap->Height;
    long pic_length = 0;
    TCanvas *pCanvas = Bitmap->Canvas;

    fwrite(&wd, 2, 1, fp);
    fwrite(&ht, 2, 1, fp);
    unsigned long startPos = ftell(fp);

    for (int y = 0; y < ht; y++) {
	    unsigned long color = pCanvas->Pixels[0][y];
	    unsigned char count = 0;
	    for( int x = 0; x < wd+1; x++ ) {
            unsigned long cc = pCanvas->Pixels[x][y];
		    if( cc == color && count < 255 && x < wd )
			    count++;
		    else  {
                /* TODO : Please double-check: changed cc to color*/
                unsigned long c = (color & 0xffffff) | (count << 24);
			    fwrite(&c, 4, 1, fp);
                pic_length += 4;
			    count = 1;
			    color = cc;
            }
        }
    }

  /*  unsigned long endPos = ftell(fp);
    fseek( fp, SEED_SET, startPos );
    fwrite( &pic_length, 4, 1, fp );
    fseek( fp, SEED_SET, endPos );    */
    return pic_length;
}
#endif
//-----------------------------------------------------------------------


