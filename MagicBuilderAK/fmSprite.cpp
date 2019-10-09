//---------------------------------------------------------------------------

#include <vcl.h>
#include <vcl\Clipbrd.hpp>
#pragma hdrstop

#include "fmSprite.h"
#include "fmStage.h"
#include "fmMain.h"
#include "xpg.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TwSprite *wSprite;
//---------------------------------------------------------------------------
__fastcall TwSprite::TwSprite(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TwSprite::FormCreate(TObject *Sender)
{
    int i = 0;
    test->Cells[i++][0] = "Index";
    test->Cells[i++][0] = "Name";
    //test->Cells[i++][0] = "HashKey";
    test->Cells[i++][0] = "Image";
    test->Cells[i++][0] = "Type";
    test->Cells[i++][0] = "tIndex";
    test->Cells[i++][0] = "Left";
    test->Cells[i++][0] = "Top";
    test->Cells[i++][0] = "Width";
    test->Cells[i++][0] = "Height";
    test->Cells[i++][0] = "Flag";

    Width = test->Width + 40;

    for (i = 1; i < 256; i++)
        test->Cells[0][i] = i - 1;

    test->Align = alClient;
}

//---------------------------------------------------------------------------
void __fastcall TwSprite::SelectPage(TObject *Sender, int iPage)
{
    Pages *pPage = xpgMovie->m_GetPage(iPage);
	
    if (pPage == NULL) return;
    for (int iSprite = 0; iSprite < pPage->m_SpriteList->Count; iSprite++) {
        Sprites *pSprite = pPage->m_GetSprite(iSprite);
        int iRow = iSprite + 1;
        test->Cells[1][iRow] = pSprite->m_Name;
        if (pSprite->m_pRole == NULL)
            test->Cells[2][iRow] = String(pSprite->m_iRole);
        else
        	test->Cells[2][iRow] = String(pSprite->m_pRole->m_iIndex);
        test->Cells[3][iRow] = String(pSprite->m_iType);
        test->Cells[4][iRow] = String(pSprite->m_iTypeIndex);
        test->Cells[5][iRow] = String(pSprite->m_iPx);
        test->Cells[6][iRow] = String(pSprite->m_iPy);
        test->Cells[7][iRow] = String(pSprite->m_iWidth);
        test->Cells[8][iRow] = String(pSprite->m_iHeight);
    }

    for (int i = pPage->m_SpriteList->Count + 1; i < test->RowCount; i++) {
        for (int iCol = 1; iCol < test->ColCount; iCol++) {
            test->Cells[iCol][i] = "";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TwSprite::SelectSprite(TObject *Sender, int iSprite)
{
    TGridRect rct;

    if (iSprite == -1)
    {
      rct.Left = 0;
      rct.Right = 0;
      rct.Top = 0;
      rct.Bottom = 0;
    }
    else
    {
      rct.Left = 0;
      rct.Right = 10;
      rct.Top = iSprite + 1;
      rct.Bottom = iSprite + 1;
    }
    test->Selection = rct;
}
//---------------------------------------------------------------------------
void __fastcall TwSprite::testSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    edValue->Text = test->Cells[ACol][ARow];
    edValue->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::bnChangeValueClick(TObject *Sender)
{
    TGridRect SelRect;
    SelRect = test->Selection;

    for (int i = SelRect.Top; i <= SelRect.Bottom; i++) {
        for (int iCol = SelRect.Left; iCol <= SelRect.Right; iCol++) {
            ChangeSpriteProperty(Sender, iCol, i, edValue->Text);
            //test->Cells[iCol][i] = edValue->Text;
        }
    }
}

//---------------------------------------------------------------------------

void __fastcall TwSprite::ChangeSpriteProperty(TObject *Sender, int iCol, int iRow, String NewValueString)
{
    if (xpgMovie == NULL) return;
    int iNewValue = StrToIntDef(NewValueString, 0xffff);

    int iSprite = iRow - 1;
    Pages *pPage = xpgMovie->m_pCurPage;
    if (pPage == NULL) return;

    if (pPage->m_SpriteList->Count <= 0) return;
    Sprites *pSprite = (Sprites *)pPage->m_SpriteList->Items[iSprite];
    if (pSprite == NULL) return;
    
    if (iCol > 6) {
        if (iNewValue == 0xffff) {
            Application->MessageBox("Invalid Value!", "Error", MB_OK);
            return;
        }
    }
    switch (iCol) {
        case 1 :
            pSprite->m_Name = NewValueString;
            break;
        case 2 :
            if (iNewValue >= xpgMovie->m_iRoleCount) {
                Application->MessageBox("Invalid role number!", "Error", MB_OK);
                return;
            }
            pSprite->m_SetRole(xpgMovie->m_GetRole(iNewValue));
            break;
        case 3 :
            if (iNewValue == 0xffff) {
                Application->MessageBox("Invalid Type Value!", "Error", MB_OK);
                return;
            }
            pSprite->m_iType = iNewValue;
            break;
        case 4 :
            if (iNewValue == 0xffff) {
                Application->MessageBox("Invalid TypeIndex Value!", "Error", MB_OK);
                return;
            }
            pSprite->m_iTypeIndex = iNewValue;
            break;
        case 5 :
            if (iNewValue >= xpgMovie->m_iScreenWidth) {
                Application->MessageBox("Invalid value!", "Error", MB_OK);
                return;
            }
            pSprite->m_MoveTo(iNewValue, pSprite->m_iPy);
            break;
        case 6 :
            if (iNewValue >= xpgMovie->m_iScreenHeight) {
                Application->MessageBox("Invalid value!", "Error", MB_OK);
                return;
            }
            pSprite->m_MoveTo(pSprite->m_iPx, iNewValue);
            break;
        default :
            Application->MessageBox("This value can't modified!", "Error", MB_OK);
            return;
    }
    test->Cells[iCol][iRow] = NewValueString;
//    wStage->SelectSprite(Sender, iSprite);
    wStage->UpdateEditSprite(Sender);
    wMain->boModified = true;
}

//---------------------------------------------------------------------------

void __fastcall TwSprite::Copy1Click(TObject *Sender)
{
    TGridRect SelRect;
    SelRect = test->Selection;
    /*
    for (int i = SelRect.Top; i <= SelRect.Bottom; i++) {
        for (int iCol = SelRect.Left; iCol <= SelRect.Right; iCol++) {
            //ChangeSpriteProperty(Sender, iCol, i, edValue->Text);
            //test->Cells[iCol][i] = edValue->Text;
        }
    } */
    TClipboard* pClipboard = Clipboard();
    pClipboard->SetTextBuf( test->Cells[SelRect.Left][SelRect.Top].c_str() );
    Paste1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::Paste1Click(TObject *Sender)
{
    TGridRect SelRect;
    SelRect = test->Selection;

    if (Clipboard()->HasFormat(CF_TEXT)) {
        AnsiString PasteText = Clipboard()->AsText;

        ChangeSpriteProperty(Sender, SelRect.Left, SelRect.Top, Clipboard()->AsText);
        for (int i = SelRect.Top; i <= SelRect.Bottom; i++) {
            for (int iCol = SelRect.Left; iCol <= SelRect.Right; iCol++) {
                ChangeSpriteProperty(Sender, iCol, i, PasteText);
            }
        }
    } else
        Application->MessageBox("The clipboard does not contain text.", NULL, MB_OK);
        
    Paste1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    boShiftDown = Shift.Contains(ssShift);
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (Shift.Contains(ssShift)) boShiftDown = false;    
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::testMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TGridRect SelRect;
    SelRect = test->Selection;
    wStage->UnSelectAllSprite(Sender, wStage->iEditPage);    

    test->Selection = SelRect;
    //wStage->SelectSprite(Sender, wStage->iEditPage);
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::testMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TGridRect SelRect;
    SelRect = test->Selection;
    for (int i = SelRect.Top; i <= SelRect.Bottom; i++) {
        wStage->SelectSprite(Sender, i - 1);
    }

    wStage->UpdateStage(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::testDblClick(TObject *Sender)
{
    TGridRect SelRect;
    SelRect = test->Selection;
    //int iRow = SelRect.Top;
    //int iCol = SelRect.Left;
    test->EditorMode = true;
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::edValueKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    TGridRect SelRect;
    SelRect = test->Selection;
    int iRow = SelRect.Top;
    int iCol = SelRect.Left;
    if(Key == VK_RETURN)
        ChangeSpriteProperty(Sender, iCol, iRow, edValue->Text);
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::ToolButton2Click(TObject *Sender)
{
    TGridRect SelRect;
    SelRect = test->Selection;

    for (int i = SelRect.Top; i <= SelRect.Bottom; i++) {
        for (int iCol = SelRect.Left; iCol <= SelRect.Right; iCol++) {
            ChangeSpriteProperty(Sender, iCol, i, edValue->Text);
            //test->Cells[iCol][i] = edValue->Text;
        }
    }
}
//---------------------------------------------------------------------------


void __fastcall TwSprite::Copy2Click(TObject *Sender)
{
    TGridRect src = test->Selection;

    int iColCount = src.Right - src.Left + 1;
    int iRowCount = src.Bottom - src.Top + 1;
    StringGrid1->ColCount = iColCount;
    StringGrid1->RowCount = iRowCount;

    AnsiString str = ""; //"test\r\n";

    for (int rSrc = src.Top, r1 = 0; rSrc <= src.Bottom; rSrc++, r1++) {
        for (int cSrc = src.Left, c1 = 0; cSrc <= src.Right; cSrc++, c1++) {
            StringGrid1->Cells[c1][r1] = test->Cells[cSrc][rSrc];
            str += test->Cells[cSrc][rSrc] + "\t";
        }
        str += "\n";
    }
    Clipboard()->SetTextBuf(str.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::Paste2Click(TObject *Sender)
{
    TGridRect dst = test->Selection;

    int TextHandle;
    char *pText;

    Clipboard()->Open();
    try
    {
        TextHandle = Clipboard()->GetAsHandle(CF_TEXT);
        pText = (char *)GlobalLock((HGLOBAL)TextHandle);
        GlobalUnlock((HGLOBAL)TextHandle);
    }
    catch (...)
    {
        Clipboard()->Close();
        throw;
    }
    Clipboard()->Close();

    char *pToken;

 #if 1

        int iRowCount = test->RowCount - dst.Top + 1;
        int iColCount = test->ColCount - dst.Left + 1;
        char *pNextLineText;
        int n = strlen(pText);
        char *pTextEnd;
        pTextEnd = pText + n - 1;
        for (int rDst = dst.Top, rSrc = 0; rSrc < iRowCount; rDst++, rSrc++) {
            pNextLineText = strpbrk (pText, "\n");
            *pNextLineText++ = 0;
            pToken = pText;
            pText = strchr(pToken, '\t');
            *pText++ = 0;
            for (int cDst = dst.Left, cSrc = 0; cSrc < iColCount; cDst++, cSrc++) {
                test->Cells[cDst][rDst] = String(pToken);
                ChangeSpriteProperty(Sender, cDst, rDst, String(pToken));
                
                if (pText == NULL || pText >= pNextLineText) break;
                
                pToken = pText;
                pText = strchr(pToken, '\t');

                if (pText != NULL)
                    *pText++ = 0;

                //ChangeSpriteProperty(Sender, cDst, rDst, StringGrid1->Cells[cSrc][rSrc]);
            }
            pText = pNextLineText;
            if (pText >= pTextEnd) return;
        }
#endif
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::SendBackward1Click(TObject *Sender)
{
    wStage->SendBackward1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TwSprite::BringForward1Click(TObject *Sender)
{
    wStage->BringForward1Click(Sender);
}
//---------------------------------------------------------------------------




