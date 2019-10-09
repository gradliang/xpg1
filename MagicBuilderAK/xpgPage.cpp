//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "xpgPage.h"
#include "xpg.h"
#include "xpgDIB.h"
#include "fmStage.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
Pages::Pages()
{
    m_SpriteList = new TList;
	m_iSpriteCount = 0;
    for(int i = 0; i< XPG_COMMAND_COUNT; i++)
    {
        SetCommandButton(i, "");
        SetCommandPage(i, "");
        SetCommandAction(i, "");
    }
}
//---------------------------------------------------------------------------
Pages::~Pages()
{
    if (m_SpriteList != NULL) {
    m_Clear();
    delete m_SpriteList;
}
        m_iSpriteCount = 0;
    m_pCurSprite = NULL;
    m_Name = NULL;
}
//---------------------------------------------------------------------------
void Pages::m_Clear()
{
    if (m_SpriteList != NULL) {
        // clear Sprite List
        if (m_SpriteList->Count > 0) {
          m_iSpriteCount = m_SpriteList->Count;
          for (int i = 0; i < m_iSpriteCount; i++)
          {
              Sprites *pSprite = (Sprites *)m_SpriteList->Items[i];
              if (pSprite != NULL) {
                  pSprite->m_Clear();
                  delete pSprite;
              }
          }
          m_SpriteList->Clear();
        }

    }
    m_iSpriteCount = 0;
    m_pCurSprite = NULL;
    m_Name = NULL;
}
//---------------------------------------------------------------------------
Sprites *Pages::m_AddNewSprite(RoleImages *pRole, int x, int y)
{
    Sprites *pSprite = new Sprites;
    pSprite->m_SetRole(pRole);
    pSprite->m_MoveTo(x, y);

    pSprite->m_iType = 0;
	pSprite->m_iTypeIndex = 0;
    pSprite->m_lHashKey = 0;
    int iSprite = m_SpriteList->Add(pSprite);
    pSprite->m_iIndex = iSprite;
    m_iSpriteCount = m_SpriteList->Count;

    return pSprite;
}
//---------------------------------------------------------------------------
Sprites *Pages::m_AddCopySprite(Sprites *pSrcSprite)
{
    //copy sprite info
    Sprites *pDstSprite = new Sprites;
    pDstSprite->m_SetRole(pSrcSprite->m_pRole);
    pDstSprite->m_MoveTo(pSrcSprite->m_iPx, pSrcSprite->m_iPy);

    pDstSprite->m_Name = pSrcSprite->m_Name;
    pDstSprite->m_iType = pSrcSprite->m_iType;
	pDstSprite->m_iTypeIndex = pSrcSprite->m_iTypeIndex;
    pDstSprite->m_lHashKey = 0;
    int iSprite = m_SpriteList->Add(pDstSprite);
    pDstSprite->m_iIndex = iSprite;
    m_iSpriteCount = m_SpriteList->Count;

    return pDstSprite;
}

//---------------------------------------------------------------------------
Sprites *Pages::m_GetSprite(int iSprite)
{
    if (iSprite >= m_SpriteList->Count)
        return NULL;
    else
        return (Sprites *)(m_SpriteList->Items[iSprite]);
}

//---------------------------------------------------------------------------
void Pages::m_DrawSprites(Byte *lpCanvasDib)
{
    if (m_pMovie == NULL) return;
    //TCanvas *pCanvas = m_pMovie->m_pCanvas;

    for (int i = 0; i < m_SpriteList->Count; i++)
    {
        Sprites *pSprite = (Sprites *)m_SpriteList->Items[i];
        xpgDrawBitmap(lpCanvasDib,
            pSprite->m_iPx,
            pSprite->m_iPy,
            pSprite->m_pRole->m_pBitmap->Handle);
    }
}
//---------------------------------------------------------------------------
void Pages::m_DrawSprites()
{
    if (m_pMovie == NULL) return;
    TCanvas *pCanvas = m_pMovie->m_pCanvas;

    for (int i = 0; i < m_SpriteList->Count; i++)
    {
        Sprites *pSprite = (Sprites *)m_SpriteList->Items[i];
        if (pSprite->m_pRole == NULL) continue;
        //Check if the sprite has its transparent property set
        if (pSprite->m_pRole->m_boTrans)
        {
            //wStage->Image1->Canvas->Draw(0,0,pSprite->m_pRole->m_pBitmap);
            //Graphics::TBitmap *pBitmap = wStage->Image1->Picture->Bitmap;
            Graphics::TBitmap *pBitmap = pSprite->m_pRole->m_pBitmap;
            Graphics::TBitmap *pBmpColor = new Graphics::TBitmap;
            Graphics::TBitmap *pBmpMono = new Graphics::TBitmap;

            int iRoleWidth = pBitmap->Width;
            int iRoleHeight = pBitmap->Height;
            TCanvas *pCanvasRole = pBitmap->Canvas;
            pCanvasRole->Lock();

            int *pBits = (int *)malloc(iRoleWidth * iRoleHeight * 4);
            int *pBitsMono = (int *)malloc(iRoleWidth * iRoleHeight *4);
            int *pBitsTemp;
            BITMAPINFO *pbi = new BITMAPINFO;
            BITMAPINFOHEADER *pDibHeader = (BITMAPINFOHEADER *)pbi;

            // **** Set the value of the BITMAPINFOHEADER. ****
            pDibHeader->biSize = sizeof(BITMAPINFOHEADER);
            pDibHeader->biWidth = iRoleWidth;
            pDibHeader->biHeight = iRoleHeight;
            pDibHeader->biPlanes = 1;
            pDibHeader->biBitCount = 32;
            pDibHeader->biCompression = 0;
            pDibHeader->biSizeImage = 0;
            pDibHeader->biXPelsPerMeter = 1200;
            pDibHeader->biYPelsPerMeter = 1200;
            pDibHeader->biClrUsed = 0;
            pDibHeader->biClrImportant = 0;

            //color

            //Extract pixels
            bool boRes = GetDIB(pBitmap->Handle,Null,pbi,pBits);


            pBitsTemp = pBits;  //store pBits original position
            for( int i = 0; i < (iRoleWidth * iRoleHeight); i++)
            {
                if( (*pBits & 0x00FFFFFF) == (xpgMovie->m_iTransColor & 0x00FFFFFF) )
                    *pBits = 0x00000000;
                else
                {}
                pBits++;
            }
            pBits = pBitsTemp;  //restore pBits to original position

            //Copy pixels to pBmpColor
            if (boRes == true)
                StretchDIBits ( pBmpColor->Canvas,
                            //0, 0, StageImage->Width, StageImage->Height,
                            0, 0, iRoleWidth, iRoleHeight,
                            0, 0, iRoleWidth, iRoleHeight,
                            pBits,
                            pbi,
                            DIB_RGB_COLORS, SRCCOPY );
            else
                ShowMessage("doh");
            //mono

            //Extract pixels
            boRes = GetDIBits(
                pBitmap->Canvas,            // handle of device context
                pBitmap->Handle,	// handle of bitmap
                0,	                                    // first scan line to set in destination bitmap
                iRoleHeight,	                            // number of scan lines to copy
                pBitsMono,	                                // address of array for bitmap bits
                pbi,	                                // address of structure with bitmap data
                DIB_RGB_COLORS	 	                    // RGB or palette index
            );

            pBitsTemp = pBitsMono;  //store pBitsmono original position
            for( int i=0; i< (iRoleWidth * iRoleHeight); i++)
            {
                if( (*pBitsMono & 0x00FFFFFF) == (xpgMovie->m_iTransColor & 0x00FFFFFF) )
                    *pBitsMono = 0x00FFFFFF;
                else
                    *pBitsMono = 0x00000000;
                pBitsMono++;
            }
            pBitsMono = pBitsTemp;  //Restore pBitsMono to original position
            if (boRes == true)
                StretchDIBits ( pBmpMono->Canvas,
                            //0, 0, StageImage->Width, StageImage->Height,
                            0, 0, iRoleWidth, iRoleHeight,
                            0, 0, iRoleWidth, iRoleHeight,
                            pBitsMono,
                            pbi,
                            DIB_RGB_COLORS, SRCCOPY );


            free(pBits);
            free(pBitsMono);
            delete pbi;

            pCanvasRole->Unlock();

            pCanvas->CopyMode = cmSrcAnd;
            pCanvas->Draw(pSprite->m_iPx, pSprite->m_iPy,pBmpMono);
            //pCanvas->CopyMode = cmSrcPaint;
            //pCanvas->Draw(pSprite->m_iPx, pSprite->m_iPy,pBmpColor);
        }
        else
        {
            pCanvas->CopyMode = cmSrcCopy;
            pCanvas->Draw(pSprite->m_iPx, pSprite->m_iPy,
                      pSprite->m_pRole->m_pBitmap);
        }
    }
    //pCanvas->Refresh();
}
//---------------------------------------------------------------------------

void Pages::m_ResetSpriteIndex()
{
    for (int i = 0; i < m_SpriteList->Count; i++) {
        Sprites *pSprite = (Sprites *)m_SpriteList->Items[i];
        pSprite->m_iIndex = i;
    }
}
//---------------------------------------------------------------------------
int Pages::GetCommandCount()
{
    int m_iCommandCount = 0;
    for (int i = 0; i < XPG_COMMAND_COUNT; i++) {
		if (CheckCommandEmpty(i) == false) {
            m_iCommandCount++;
        }
    }	
	return m_iCommandCount;	
}

bool Pages::CheckCommandEmpty(int i)
{
	return m_Command[i].CheckEmpty();	
}
//---------------------------------------------------------------------------
void Pages::SetCommandButton(int i, int iKey) 
{
	m_Command[i].SetButton(iKey);
}

void Pages::SetCommandEvent(int i, int iKey) 
{
	m_Command[i].SetEvent(iKey);
}	

void Pages::SetCommandPage(int i, int iKey) 
{
	m_Command[i].SetPage(iKey);
}	

void Pages::SetCommandAction(int i, int iKey) 
{
	m_Command[i].SetAction(iKey);
}		

void Pages::SetCommandParam(int i, int iKey) 
{
	m_Command[i].SetParam(iKey);
}
//---------------------------------------------------------------------------
void Pages::SetCommandButton(int i, AnsiString str) 
{
	m_Command[i].SetButton(str);

}

void Pages::SetCommandEvent(int i, AnsiString str) 
{
	m_Command[i].SetEvent(str);
}	

void Pages::SetCommandPage(int i, AnsiString str) 
{
	m_Command[i].SetPage(str);
}	

void Pages::SetCommandAction(int i, AnsiString str) 
{
	m_Command[i].SetAction(str);
}		

void Pages::SetCommandParam(int i, AnsiString str) 
{
	m_Command[i].SetParam(str);
}	
//---------------------------------------------------------------------------
void Pages::SetCommand(int iCommand, int iField, int iKey) 
{
	if (iField == 0)
		SetCommandButton(iCommand, iKey);
	else if (iField == 1)
		SetCommandPage(iCommand, iKey);
	else
		SetCommandAction(iCommand, iKey);
}	

void Pages::SetCommand(int iCommand, int iField, AnsiString str) 
{
	if (iField == 0)
		SetCommandButton(iCommand, str);
	else if (iField == 1)
		SetCommandPage(iCommand, str);
	else 
		SetCommandAction(iCommand, str);
}	
//---------------------------------------------------------------------------
AnsiString Pages::GetCommandButton(int i) 
{
	return m_Command[i].GetButtonName();
}

AnsiString Pages::GetCommandEvent(int i) 
{
	return m_Command[i].GetEventName();
}

AnsiString Pages::GetCommandPage(int i) 
{
	return m_Command[i].GetPageName();
}	

AnsiString Pages::GetCommandAction(int i) 
{
	return m_Command[i].GetActionName();
}

AnsiString Pages::GetCommandParam(int i) 
{
	return m_Command[i].GetParamName();
}
//---------------------------------------------------------------------------
void Pages::CopyPage(Pages *pSrcPage)
{
    for (int i = 0; i < pSrcPage->m_SpriteList->Count; i++) {
        Sprites *pSprite = (Sprites *)pSrcPage->m_SpriteList->Items[i];
        m_AddCopySprite(pSprite);
	}
	
    for(int i = 0; i< XPG_COMMAND_COUNT; i++)
    {
        SetCommandButton(i, pSrcPage->GetCommandButton(i));
        SetCommandPage(i, pSrcPage->GetCommandPage(i));
        SetCommandAction(i, pSrcPage->GetCommandAction(i));
    }	
	
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool Commands::CheckEmpty()
{
	if (m_Button == "" && m_Page == "" && m_Action == "") 
		m_boEmpty = true;			
	else 
		m_boEmpty = false;
		
	return m_boEmpty;	
}
//-----------------------------------------------------------------------------
void Commands::SetButton(int iKey) 
{
	m_iButtonKey = iKey;
}

void Commands::SetEvent(int iKey) 
{
	m_iEventKey = iKey;
}

void Commands::SetPage(int iKey) 
{
	m_iPageIndex = iKey;
}	

void Commands::SetAction(int iKey) 
{
	m_iActionKey = iKey;
}	
//-----------------------------------------------------------------------------
void Commands::SetButton(AnsiString name) 
{
	if (name != "") {
		m_Button = name;
		m_iButtonKey = xpgMovie->m_GetButtonKey(name);
		m_boEmpty = false;
	} else {
		m_iButtonKey = 0;
		m_Button = "";
		CheckEmpty();
	}
}

void Commands::SetEvent(AnsiString name) 
{
	if (name != "") {
		m_Event = name;
		m_iEventKey = xpgMovie->m_GetEventKey(name);
		m_boEmpty = false;
	} else {
		m_iEventKey = 0;
		m_Event = "";
		CheckEmpty();
	}
}

void Commands::SetPage(AnsiString name) 
{
	if (name != "") {
		m_Page = name;
		m_iPageIndex = xpgMovie->m_GetPageKey(name);
		m_boEmpty = false;
	} else {
		m_iPageIndex = 0;
		m_Page = "";
		CheckEmpty();
	}
}	

void Commands::SetAction(AnsiString name) 
{
	if (name != "") {
		m_Action = name;
		m_iActionKey = xpgMovie->m_GetActionKey(name);
		m_boEmpty = false;
	} else {
		m_Action = "";
		m_iActionKey = 0;
		CheckEmpty();
	}
}		


void Commands::SetParam(AnsiString name) 
{
	if (name != "") {
		m_Param = name;
		m_iParamKey = xpgMovie->m_GetActionKey(name);
		m_boEmpty = false;
	} else {
		m_Param = "";
		m_iParamKey = 0;
		CheckEmpty();
	}
}	

//---------------------------------------------------------------------------
AnsiString Commands::GetButtonName()
{
	return m_Button;
}
AnsiString Commands::GetEventName()
{
	return m_Event;
}
AnsiString Commands::GetActionName()
{
	return m_Action;
}
AnsiString Commands::GetPageName()
{
	return m_Page;  
}
AnsiString Commands::GetParamName()
{
	return m_Param;  
}
	
//---------------------------------------------------------------------------


