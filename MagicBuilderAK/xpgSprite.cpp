//---------------------------------------------------------------------------


#pragma hdrstop

#include "xpgSprite.h"
#include "xpgRole.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

Sprites::Sprites()
{
    m_pRole = NULL;
    m_pMovie = NULL;
    m_pPage = NULL;
    //////////////////////////
    m_touchEnable = 0;
    m_touchFlag = 0;
}
//---------------------------------------------------------------------------

Sprites::~Sprites()
{
    m_Clear();
}
//---------------------------------------------------------------------------
void Sprites::m_Clear()
{
    m_pRole = NULL;
    m_pMovie = NULL;
    m_pPage = NULL;
    m_Name = NULL;
}
//---------------------------------------------------------------------------
void Sprites::m_SetRole(RoleImages *pRole)
{
    m_pRole = pRole;
    m_iRole = pRole->m_iIndex;
    m_iWidth = pRole->m_iWidth;
    m_iHeight = pRole->m_iHeight;
}
//---------------------------------------------------------------------------
void Sprites::m_MoveTo(int dstX, int dstY)
{
    m_iPx = dstX;
    m_iPy = dstY;
    m_iLeft = m_iPx;
    m_iTop = m_iPy;
    m_iRight = m_iLeft + m_iWidth - 1;
    m_iBottom = m_iTop + m_iHeight - 1;
}
//---------------------------------------------------------------------------
void Sprites::m_Move(int x, int y)
{
    m_iPx += x;
    m_iPy += y;
    m_iLeft = m_iPx;
    m_iTop = m_iPy;
    m_iRight = m_iLeft + m_iWidth - 1;
    m_iBottom = m_iTop + m_iHeight - 1;
}
//---------------------------------------------------------------------------
bool Sprites::m_Inside(int x, int y)
{
    return (x > m_iLeft && x < m_iRight && y > m_iTop && y < m_iBottom);
}
//---------------------------------------------------------------------------
void Sprites::m_Resize(float xRatio, float yRatio)
{
    m_iPx = (int)(m_iPx * xRatio);
    m_iPy = (int)(m_iPy * yRatio);
    m_iWidth = m_pRole->m_iWidth;
    m_iHeight = m_pRole->m_iHeight;
}
//---------------------------------------------------------------------------


