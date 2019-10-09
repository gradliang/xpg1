//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "BMGUtils.h"
#include "BMGDLL.h"
#include "xpgMovie.h"
#include "xpg.h"
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "jpeglib.h"   /* size_t must be defined before this file is included */

//---------------------------------------------------------------------------
#pragma package(smart_init)
//----------------------------------------------------------------------------
XpgMovies::XpgMovies()
{
    m_InitHeader(true);
    m_RoleList = new TList;
    m_PageList = new TList;

	//m_CommandList = new TList;
	
    m_pCurPage = NULL;
    m_pCurRole = NULL;
    m_boXPW = false;
    m_iScreenWidth = 720;
    m_iScreenHeight = 480;
}
//----------------------------------------------------------------------------
XpgMovies::~XpgMovies()
{
    m_Clear();

    delete m_RoleList;
    delete m_PageList;

    //delete m_CommandList;
}
//-----------------------------------------------------------------------------
void XpgMovies::m_Clear()
{
    // clear PageList
    if (m_PageList != NULL && m_PageList->Count > 0) {
        m_iPageCount = m_PageList->Count;

        for (int i = 0; i < m_iPageCount; i++)
        {
            Pages *pPage = (Pages *)m_PageList->Items[i];
            if (i == m_iPageCount - 1) {
                //break;
                m_iPageCount = 0;
            }
            if (pPage != NULL) {
                //pPage->m_Clear();  // This will clear spriteList of pPage
                delete pPage;
            }
        }
        m_PageList->Clear();
    }

    m_iPageCount = 0;

    // clear RoleList
    m_iRoleCount = m_RoleList->Count;
    for (int iRole = 0; iRole < m_iRoleCount; iRole++)
    {
        RoleImages *pRole = (RoleImages *)m_RoleList->Items[iRole];
        pRole->m_Clear();
        delete pRole;
        //m_RoleList->Items[iRole] = NULL;
    }
    m_RoleList->Clear();
    m_iRoleCount = 0;

    m_pCurPage = NULL;
    m_pCurSprite = NULL;
    m_pCurRole = NULL;
	
	m_SpriteTypeList.Clear();
	m_ButtonList.Clear();
	m_ActionList.Clear();
	
	//m_CommandList->Clear();
	
}

void XpgMovies::m_DeleteAllPages()
{
   // clear PageList
    if (m_PageList != NULL && m_PageList->Count > 0) {
        m_iPageCount = m_PageList->Count;

        for (int i = 0; i < m_iPageCount; i++)
        {
            Pages *pPage = (Pages *)m_PageList->Items[i];
            if (i == m_iPageCount - 1) {
                //break;
                //m_iPageCount = 0;
            }
            if (pPage != NULL) {
                //pPage->m_Clear();  // This will clear spriteList of pPage
                delete pPage;
            }
        }
        m_PageList->Clear();
    }
    m_iPageCount = 0;
    m_pCurPage = NULL;
    m_pCurSprite = NULL;
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
void XpgMovies::m_InitHeader(bool boReset)
{
    if (boReset) {
        m_iScreenWidth = 240;
        m_iScreenHeight = 320;
        m_iBkgColor = 0xfffff;
        //TColor tempColor = clDontMask;
        m_iTransColor = 0x00FFFFFF; //Purple: R:FF, G:00, B:FF
        m_iMaxSprites = 128;

        m_iSpriteDataLen = 12;
        m_iRoleCount = 0;
    }
    else {
        m_iRoleCount = m_RoleList->Count;
        m_iPageCount = m_PageList->Count;
        m_iMaxSprites = 0;
        for (int i = 0; i < m_iPageCount; i++) {
            Pages *pPage = m_GetPage(i);
            if (pPage->m_iSpriteCount > m_iMaxSprites)
                m_iMaxSprites = pPage->m_iSpriteCount;
        }
    }

    // init chunk header length & position
    if (m_boXPW) {
        m_iImageHeaderLen  = 128;
        m_iPageHeaderLen   = 128;
        m_iScriptHeaderLen = 64;
        m_iSpriteDataLen   = 64;
        m_iHeaderInfo = (128 << 8) | 1024;
    } else {
        m_iImageHeaderLen  = 32;
        m_iPageHeaderLen   = 24;
        m_iScriptHeaderLen = 20;
        m_iSpriteDataLen   = 16;
        m_iHeaderInfo = (128 << 8) | 512;
    }
    m_lImageHeaderPos = 0;
    m_lPageHeaderPos = 0;
    m_lScriptHeaderPos = 0;
    m_iPageCount = 0;
    m_iScriptCount = 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void XpgMovies::m_ReplaceRole( int iRole, Graphics::TBitmap *Bitmap, int iWd, int iHt )
{
    RoleImages *pRole = (RoleImages *)(m_RoleList->Items[iRole]);
    //if (m_pBitmap) delete m_pBitmap;
    //pRole->m_pBitmap = Bitmap;
    pRole->m_pMovie = this;
    pRole->m_iWidth = iWd;
    pRole->m_iHeight = iHt;
}
//-----------------------------------------------------------------------------
int XpgMovies::m_AddRole( Graphics::TBitmap *Bitmap, int iWd, int iHt )
{
    m_pCurRole = new RoleImages;
    m_pCurRole->m_pBitmap = Bitmap;
    m_pCurRole->m_pMovie = this;
    m_pCurRole->m_iWidth = iWd;
    m_pCurRole->m_iHeight = iHt;

    int iRole = m_RoleList->Add(m_pCurRole);
    m_pCurRole->m_iIndex = iRole;
    m_iRoleCount = m_RoleList->Count;
    return iRole;
}
//-----------------------------------------------------------------------------
bool XpgMovies::m_DeleteRole(int iRole)
{
    if (iRole < m_RoleList->Count)
    {
        RoleImages *pRole = (RoleImages *)m_RoleList->Items[iRole];
        delete pRole;
        m_RoleList->Delete(iRole);
        m_iRoleCount = m_RoleList->Count;

        //Reset RoleImage index
        for( int i = iRole; i< m_RoleList->Count; i++)
        {
            RoleImages *pRole = xpgMovie->m_GetRole(i);
            pRole->m_iIndex = i;
        }

        for (int iPage = 0; iPage < m_PageList->Count; iPage++)
        {
            Pages *pPage = (Pages *)m_GetPage(iPage);
            for (int i = 0; i< pPage->m_SpriteList->Count; i++)
            {
                Sprites *pSprite = (Sprites *)pPage->m_SpriteList->Items[i];
                pSprite->m_iRole = pSprite->m_pRole->m_iIndex;
            }
        }
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool XpgMovies::m_CheckRoleUsed(int iRole)
{
    RoleImages *pRole = xpgMovie->m_GetRole(iRole);
    for (int iPage = 0; iPage < m_PageList->Count; iPage++)
    {
        Pages *pPage = (Pages *)m_GetPage(iPage);
        for (int i = 0; i< pPage->m_SpriteList->Count; i++)
        {
            Sprites *pSprite = (Sprites *)pPage->m_SpriteList->Items[i];
            if (pSprite->m_pRole == pRole)
                return true;
        }
    }
    return false;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int XpgMovies::m_AddPage()
{
    m_pCurPage = new Pages;
    m_pCurPage->m_pMovie = this;
    m_pCurPage->m_Name = "";
    m_pCurPage->m_lHashKey = 0;
    m_pCurPage->m_iSpriteCount = 0;

    int iPage = m_PageList->Add(m_pCurPage);
    m_pCurPage->m_iIndex = iPage;
    m_iPageCount = m_PageList->Count;

    return iPage;
}
//-----------------------------------------------------------------------------
int XpgMovies::m_DuplicatePage(int iPage)
{
    Pages *pSrcPage = m_GetPage(iPage);
    Pages *pDstPage = new Pages;
    pDstPage->m_pMovie = this;
	pDstPage->CopyPage(pSrcPage);

    iPage = m_PageList->Add(pDstPage);
    pDstPage->m_iIndex = iPage;

    m_iPageCount = m_PageList->Count;
    return iPage;
}
//-----------------------------------------------------------------------------
bool XpgMovies::m_InsertPage(int iPage)
{
    m_pCurPage = new Pages;
    m_pCurPage->m_pMovie = this;
    m_pCurPage->m_Name = "";
    m_pCurPage->m_lHashKey = 0;

    m_PageList->Insert(iPage, m_pCurPage);
    m_pCurPage->m_iIndex = iPage;

    //Reset m_iIndex after insertion
    for(int i = iPage+1; i<m_PageList->Count ; i++)
    {
        Pages *pPage = m_GetPage(i);
        pPage->m_iIndex = i;
    }

    m_iPageCount = m_PageList->Count;
    return true;
}
//-----------------------------------------------------------------------------
int XpgMovies::m_MovePage(int iSrcPage, int iDstPage)
{
    m_PageList->Move(iSrcPage, iDstPage);
    m_pCurPage->m_iIndex = iDstPage;

    for(int i = 0; i<m_PageList->Count; i++)
    {
        Pages *pPage = m_GetPage(i);
        pPage->m_iIndex = i;
    }

    return iDstPage;

}
//-----------------------------------------------------------------------------
int XpgMovies::m_InsertDuplicate(int iSrcPage, int iDstPage)
{
    Pages *pSrcPage = m_GetPage(iSrcPage);
    m_pCurPage = new Pages;
    m_pCurPage->m_pMovie = this;
	m_pCurPage->CopyPage(pSrcPage);

    m_PageList->Insert(iDstPage, m_pCurPage);
    m_pCurPage->m_iIndex = iDstPage;

    //Reset m_iIndex after insertion
    for(int i = iDstPage+1; i<m_PageList->Count ; i++)
    {
        Pages *pPage = m_GetPage(i);
        pPage->m_iIndex = i;
    }

    m_iPageCount = m_PageList->Count;

    return iDstPage;
}

//-----------------------------------------------------------------------------
bool XpgMovies::m_DeletePage(int iPage)
{
    if (iPage < m_PageList->Count) {
        Pages *pPage = (Pages *)m_PageList->Items[iPage];
        delete pPage;
        m_PageList->Delete(iPage);
        m_iPageCount = m_PageList->Count;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
bool XpgMovies::m_ReplacePage(int iPage, Pages*pPage)
{
    if (iPage < m_PageList->Count) {
        Pages *pOldPage = (Pages *)m_PageList->Items[iPage];        
        m_PageList->Items[iPage] = pPage;
		if (pOldPage != NULL)
			delete pOldPage;
        return true;
    } else {
	    iPage = m_PageList->Add(pPage);
	    pPage->m_iIndex = iPage;
	    m_iPageCount = m_PageList->Count;
    }
    return false;
}
//-----------------------------------------------------------------------------
void XpgMovies::m_Draw()
{

}
//-----------------------------------------------------------------------------
RoleImages *XpgMovies::m_GetRole(int i)
{
    if (i >= m_RoleList->Count)
        return NULL;
    else
        return (RoleImages *)m_RoleList->Items[i];
}
//-----------------------------------------------------------------------------
Pages *XpgMovies::m_GetPage(int i)
{
    if (i >= m_PageList->Count)
        return NULL;
    else
        return (Pages *)m_PageList->Items[i];
}
//-----------------------------------------------------------------------------
Pages *XpgMovies::m_GetCurPage()
{
    if (!m_pCurPage) {
        if (m_PageList->Count == 0) {
            int iPage = m_AddPage();
            m_pCurPage = m_GetPage(iPage);
        } else {
            m_pCurPage = m_GetPage(0);
        }
    }

    return m_pCurPage;
}

//---------------------------------------------------------------------------

int XpgMovies::m_GetPageKey(AnsiString Name)
{
	for (int i = 0; i < m_PageList->Count; i++) {
		Pages *pPage = (Pages *)m_PageList->Items[i];
		
		if (pPage->m_Name == Name) {
			if (i > 0)
				return (i + 1);
			else 
				return 0;
		}
	}
	return 0;		
}

int XpgMovies::m_GetEventKey(AnsiString Name)
{
	if (m_EventList.Count == 0) return 0;
	return m_EventList.GetKey(Name);
}


int XpgMovies::m_GetButtonKey(AnsiString Name)
{
	if (m_ButtonList.Count == 0) return 0;
	return m_ButtonList.GetKey(Name);
}


int XpgMovies::m_GetActionKey(AnsiString Name)
{
	if (m_ActionList.Count == 0) return 0;
	return m_ActionList.GetKey(Name);
}

int XpgMovies::m_GetParamKey(AnsiString Name)
{
	int i;
	if (Name == "") return 0;

    if (TryStrToInt(Name, i))
	    return i;
    else
        return 0;
}

//-----------------------------------------------------------------------------

AnsiString XpgMovies::m_GetPageName(int i)
{
	if (i > m_PageList->Count) return "";
	
	Pages *pPage = (Pages *)m_PageList->Items[i];
	return pPage->m_Name;
}
AnsiString XpgMovies::m_GetButtonName(int i)	
{
	return m_ButtonList.GetName(i);
}
AnsiString XpgMovies::m_GetEventName(int i)	
{
	return m_EventList.GetName(i);
}
AnsiString XpgMovies::m_GetActionName(int i)
{
	return m_ActionList.GetName(i);
}
//-----------------------------------------------------------------------------


AnsiString XpgMovies::m_GetButtonNameByKey(int i)	
{
	return m_ButtonList.GetNameByKey(i);
}
AnsiString XpgMovies::m_GetActionNameByKey(int i)
{
	return m_ActionList.GetNameByKey(i);
}
//-----------------------------------------------------------------------------

void XpgMovies::m_Resize(int width, int height, float xRatio, float yRatio)
{
    m_iScreenWidth = width;
    m_iScreenHeight = height;

    for (int iPage = 0; iPage < m_iPageCount; iPage++)
    {
        Pages *pPage = m_GetPage(iPage);
        if (pPage == NULL) return;
        for (int iSprite = 0; iSprite < pPage->m_SpriteList->Count; iSprite++)
        {
            Sprites *pSprite = pPage->m_GetSprite(iSprite);
            pSprite->m_Resize(xRatio, yRatio);
        }
    }
}
