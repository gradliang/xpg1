//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "xpgRole.h"
#include "xpgMovie.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


RoleImages::RoleImages()
{
    m_pBitmap = NULL;
    m_iIndex = 0;
    m_iType = 0;
    m_iTransColor = 0;
    m_boTrans = false;
    m_boOSD = false;
    m_boSourceOutput = false;
    m_pImgSource = NULL;
	m_cBpp = 24;
	m_iFormat = IF_JPEG;
	m_iType = 0;
	m_iQuality = 0;
    m_iImgSourceSize = 0;
}
//---------------------------------------------------------------------------

RoleImages::~RoleImages()
{
    m_Clear();
}

//---------------------------------------------------------------------------
void RoleImages::m_Clear()
{
    if (m_pBitmap) delete m_pBitmap;
	if (m_pImgSource) delete m_pImgSource;
    m_pBitmap = NULL;
    m_iIndex = 0;
    m_iType = 0;
    m_iTransColor = 0;
    m_boTrans = false;
    m_boOSD = false;
	m_boSourceOutput = false;
    m_pImgSource = NULL;
	m_cBpp = 24;
	m_iFormat = IF_JPEG;
	m_iType = 0;
	m_iQuality = 0;
    m_iImgSourceSize = 0;
}
//---------------------------------------------------------------------------


