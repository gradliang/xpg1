//---------------------------------------------------------------------------

#include <vcl.h>
#include <vcl\Clipbrd.hpp>
#pragma hdrstop

#include "fmPage.h"
#include "fmMain.h"
#include "fmStage.h"
#include "xpg.h"
#include "dgInsert.h"
#include "dgMove.h"
#include "fmCommand.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TwPage *wPage;
//---------------------------------------------------------------------------
__fastcall TwPage::TwPage(TComponent* Owner)
    : TForm(Owner)
{
    
}
//---------------------------------------------------------------------------
void __fastcall TwPage::FormCreate(TObject *Sender)
{
    Width = wMain->ClientWidth - 4;
    Height = 160;

    Left = 0;
    Top = wMain->ClientHeight - Height - 68;

    ListView1->Align = alClient;
    iEditPage = -1;
    IconImage->Width = 80;
    IconImage->Height = 80;
    IconImage->Canvas->Brush->Color = clWhite;
    IconImage->Canvas->Brush->Style = bsSolid;    
}
//---------------------------------------------------------------------------
void __fastcall TwPage::ClearAndReset(TObject *Sender)
{
    IconImageList->Clear();
    ListView1->Clear();
    Caption = "Page";
}
//---------------------------------------------------------------------------
void __fastcall TwPage::SelectPage(TObject *Sender, int iPage)
{
    if (iPage >= ListView1->Items->Count) return;
    ListView1->Selected = ListView1->Items->Item[iPage];
}

//---------------------------------------------------------------------------
AnsiString __fastcall TwPage::GetPageName(int iPage)
{
    Pages *pPage = xpgMovie->m_GetPage(iPage);
    return ( pPage->m_Name );
}

//---------------------------------------------------------------------------

void __fastcall TwPage::UpdatePage(TObject *Sender, int iPage)
{
    IconImage->Canvas->StretchDraw(
        IconImage->ClientRect,
        wStage->StageImage->Picture->Bitmap );
    //IconImage->Refresh();

    if (iPage < IconImageList->Count)
    {
        IconImageList->Replace(iPage, IconImage->Picture->Bitmap, NULL);
		//if (iPage < ListView1->Items->Count)
        {
		    TListItem *pItem = ListView1->Items->Item[iPage];
	        Pages *pPage = xpgMovie->m_GetPage(iPage);
	        AnsiString name = IntToStr(iPage) + " " + pPage->m_Name;
			if (name != pItem->Caption) pItem->Caption = name;
			if (pItem->ImageIndex != iPage) pItem->ImageIndex = iPage;
		}	
#if 0
        //Update Item Caption
        for(int i = 0; i<IconImageList->Count; i++)
        {
            TListItem *pItem = ListView1->Items->Item[i];
            Pages *pPage = xpgMovie->m_GetPage(i);
            pItem->Caption = IntToStr(i) + " " + pPage->m_Name;
            pItem->ImageIndex = i;     //reset ImageIndex
        }
#endif		
    }
    else {
        IconImageList->Add(IconImage->Picture->Bitmap, NULL);

        TListItem *pItem = ListView1->Items->Add();
        if (pItem) {
            Pages *pPage = xpgMovie->m_GetPage(iPage);
            if (pPage != NULL)
                pItem->Caption = IntToStr(iPage) + " " + pPage->m_Name;
            pItem->ImageIndex = iPage;
        }
    }
}


//---------------------------------------------------------------------------

void __fastcall TwPage::UpdateAllPages(TObject *Sender)
{
    for (int i = 0; i < xpgMovie->m_PageList->Count; i++) 
	{
        wStage->ShowPage(Sender, i);
        UpdatePage(Sender, i);
		TListItem *pItem = ListView1->Items->Item[i];
        Pages *pPage = xpgMovie->m_GetPage(i);
        pItem->Caption = IntToStr(i) + " " + pPage->m_Name;
        pItem->ImageIndex = i;     //reset ImageIndex
    }
}
//---------------------------------------------------------------------------

void __fastcall TwPage::ListView1SelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
    if (Sender == NULL)
        return;
        
    if (Selected) {
        iEditPage = ListView1->Selected->Index;
        wStage->SelectPage( Sender, iEditPage );
    }
}
//---------------------------------------------------------------------------

void __fastcall TwPage::DeletePage(TObject *Sender, int iPage)
{
    if (iPage >= 0) {

        Pages *pPage = xpgMovie->m_GetPage(iPage);
        if (pPage == NULL) return;

        AnsiString OldName = pPage->m_Name;

        if (xpgMovie->m_DeletePage(iPage)) {
            UpdateAllPages(Sender);

            ListView1->Items->Delete(ListView1->Items->Count - 1);
            IconImageList->Delete(IconImageList->Count - 1);

            if (iPage < xpgMovie->m_PageList->Count)
                SelectPage(Sender, iPage);
            else if (iPage - 1 >= 0)
                SelectPage(Sender, iPage - 1);            
#if 0

            IconImageList->Delete(iPage);

            for (int i = 0; i < xpgMovie->m_PageList->Count; i++)
			{
		        wStage->ShowPage(Sender, i);
                UpdatePage(Sender, i);

                Pages* pPage = xpgMovie->m_GetPage(i);
                    for(int j=0 ; j < XPG_COMMAND_COUNT; j++)
                    {
                        if(pPage->GetCommandButton(j) == OldName)
                        {
                            pPage->SetCommandButton(j, "");
                            if(i == j)
                                wCommand->StringGrid1->Cells[1][j+1] =  "";
                        }
                    }
            }
            ListView1->UpdateItems(0, ListView1->Items->Count);
#endif
        }
        wMain->UpdatePageNameList();
        wMain->boModified = true;

        //After deleting the last page, automatically start a new one
        if(xpgMovie->m_PageList->Count ==0)
        {
            wMain->NewPage1Click(Sender);
        }

    }
    else {
        Application->MessageBox("Can't delete page.", "Warning!", MB_OK);
    }
}
//---------------------------------------------------------------------------

void __fastcall TwPage::Delete1Click(TObject *Sender)
{
    if (ListView1->Selected == NULL) {
        Application->MessageBox("Please select page to Delete.", "Warning!", MB_OK);
        return;
    }

    int iPage = ListView1->Selected->Index;
    DeletePage(Sender, iPage);
}

//---------------------------------------------------------------------------

void __fastcall TwPage::DeleteAllPages(TObject *Sender)
{
    int n = xpgMovie->m_PageList->Count;
    for (int i = n - 1; i >= 0; i--)
	{
        DeletePage(Sender, i);
    }
}
//---------------------------------------------------------------------------

void __fastcall TwPage::New1Click(TObject *Sender)
{
    int iPage = xpgMovie->m_AddPage();
    wStage->ShowPage(NULL, iPage);
    UpdatePage(Sender, iPage);
    SelectPage(Sender, iPage);

    ListView1->Scroll(ListView1->Items->Item[iPage]->Left, 0);
    wMain->UpdatePageNameList();
    wMain->boModified = true;
}
//---------------------------------------------------------------------------

void __fastcall TwPage::Insert1Click(TObject *Sender)
{
    if (ListView1->Selected == NULL) {
        Application->MessageBox("Please select page to Insert.", "Warning!", MB_OK);
        return;
    }

    int iPage = iEditPage;
    if (iPage == -1) iPage = 0;
    xpgMovie->m_InsertPage(iPage);
    UpdateAllPages(Sender);

    wMain->UpdatePageNameList();
    SelectPage(Sender, iPage);

    wMain->boModified = true;
}
//---------------------------------------------------------------------------

void __fastcall TwPage::Duplicate1Click(TObject *Sender)
{
    if (ListView1->Selected == NULL) {
        Application->MessageBox("Please select page to Insert Copy.", "Warning!", MB_OK);
        return;
    }

    int iSrcPage = ListView1->Selected->Index;
    int iDstPage = iSrcPage + 1; //OKBottomDlg->i_ItemIndex;
    int iPage = xpgMovie->m_InsertDuplicate(iSrcPage, iDstPage);

    UpdateAllPages(Sender);
    SelectPage(Sender, iPage);
    wMain->UpdatePageNameList();
    wMain->boModified = true;
}
//---------------------------------------------------------------------------

void __fastcall TwPage::wRoleCopyName1Click(TObject *Sender)
{
    Clipboard()->SetTextBuf(ListView1->Selected->Caption.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TwPage::MoveTo1Click(TObject *Sender)
{
     if (ListView1->Selected == NULL) {
        Application->MessageBox("Please select page to Move.", "Warning!", MB_OK);
        return;
    }

    for(int i=0; i < xpgMovie->m_iPageCount ; i++)
    {
        AnsiString temp = IntToStr(i);
        OKBottomDlg1->ComboBox1->AddItem(temp, NULL);
    }

    if(OKBottomDlg1->ShowModal() == mrOk)
    {
        int iSrcPage = ListView1->Selected->Index;
        int iDstPage = OKBottomDlg1->i_ItemIndex;
        int iPage = xpgMovie->m_MovePage(iSrcPage, iDstPage);

        UpdateAllPages(Sender);
        SelectPage(Sender, iPage);
        wMain->UpdatePageNameList();
        wMain->boModified = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TwPage::ListView1DblClick(TObject *Sender)
{
    wCommand->BringToFront();
    wCommand->WindowState = wsNormal;
}
//---------------------------------------------------------------------------

