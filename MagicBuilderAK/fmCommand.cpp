//---------------------------------------------------------------------------

#include <vcl.h>
#include <vcl\Clipbrd.hpp>
#pragma hdrstop

#include "fmCommand.h"
#include "fmMain.h"
#include "fmPage.h"
#include "xpg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TwCommand *wCommand;

//---------------------------------------------------------------------------
void __fastcall TwCommand::ClearStringGridSelection()
{
    TGridRect myRect;
    myRect.Left = 0;
    myRect.Top = 0;
    myRect.Right = 0;
    myRect.Bottom = 0;
    StringGrid1->Selection = myRect;
}
//---------------------------------------------------------------------------
__fastcall TwCommand::TwCommand(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TwCommand::FormCreate(TObject *Sender)
{
    //Width = Bevel1->Width + Width - ClientWidth;
    //Height = Bevel1->Height + Height - ClientHeight;
    StringGrid1->Cells[0][0] = "Event";
    StringGrid1->Cells[1][0] = "Goto";
    StringGrid1->Cells[2][0] = "Action";

    Height = 320;
    StringGrid1->Width =
        StringGrid1->ColWidths[0] + StringGrid1->ColWidths[1] +
        StringGrid1->ColWidths[2] + 20;
    //Panel3->Width = StringGrid1->Width;
    //Panel2->Width = Panel3->Width;
    Width = StringGrid1->Width + 28;
    Panel2->Align = alClient;

    //for (int i = 1; i < StringGrid1->RowCount; i++)
    //    StringGrid1->Cells[0][i] = IntToStr(i);
    //AutoSize = true;
}
//---------------------------------------------------------------------------
void __fastcall TwCommand::UpdateStringGrid(TObject *Sender)
{
#if 0
    for (int i = 1; i < StringGrid1->RowCount; i++) {
        if (StringGrid1->Cells[1][i] != "" || StringGrid1->Cells[2][i] != "")
            StringGrid1->RowHeights[i] = 22;
        else
            StringGrid1->RowHeights[i] = 4;
    }
#endif    
}
//---------------------------------------------------------------------------
void __fastcall TwCommand::SetButtonList(TStringList *bList)
{
    int n = bList->Count;
	cbButtons->Clear();
	for (int i = 0; i < n; i++) {
        cbButtons->AddItem(bList->Strings[i], NULL);
    }
	/*
    StringGrid1->RowCount = n;

    for (int i = 1; i < n; i++)
        StringGrid1->Cells[0][i] = bList->Strings[i-1];
        */
}
//---------------------------------------------------------------------------
void __fastcall TwCommand::SetEventList(TStringList *bList)
{
    int n = bList->Count;
	cbEvents->Clear();
	for (int i = 0; i < n; i++) {
        cbEvents->AddItem(bList->Strings[i], NULL);
    }
}
//---------------------------------------------------------------------------
void __fastcall TwCommand::SetActionList(TStringList *aList)
{
    int n = aList->Count;
    cbActions->Clear();

    for (int i = 0; i < n; i++) {
        cbActions->AddItem(aList->Strings[i], NULL);
    }
    //cbActions->Text = "Select Action";
}
//---------------------------------------------------------------------------
void __fastcall TwCommand::SetPageList(TStringList *pList)
{
    cbPages->Clear();
    int n = pList->Count;

    for (int i = 0; i < n; i++)
    {
        //AnsiString temp = IntToStr(i) + " " + PageList->Strings[i];
        //cbPages->AddItem(temp, NULL);
        cbPages->AddItem(pList->Strings[i],NULL);
    }
    cbPages->Text = "Select Pages";
}
//---------------------------------------------------------------------------

void __fastcall TwCommand::cbActionsSelect(TObject *Sender)
{
    int iCol = StringGrid1->Col;
    int iRow = StringGrid1->Row;

    StringGrid1->Cells[iCol][iRow] = cbActions->Text;
    cbActions->Visible = false;

    if (xpgMovie->m_pCurPage != NULL) {
        //xpgMovie->m_pCurPage->m_ButtonAction[iRow-1] = cbActions->Text;
        xpgMovie->m_pCurPage->SetCommandAction(iRow-1, cbActions->Text);
    }
}
//---------------------------------------------------------------------------

void __fastcall TwCommand::cbPagesSelect(TObject *Sender)
{
    int iCol = StringGrid1->Col;
    int iRow = StringGrid1->Row;

    StringGrid1->Cells[iCol][iRow] = cbPages->Text;

    cbPages->Visible = false;

    if (xpgMovie->m_pCurPage != NULL) {
        //xpgMovie->m_pCurPage->m_ButtonPage[iRow-1] = cbPages->Text;
        xpgMovie->m_pCurPage->SetCommandPage(iRow-1, cbPages->Text);
    }
}
//---------------------------------------------------------------------------

void __fastcall TwCommand::SelectPage(TObject *Sender, int iPage)
{
    Pages *pPage = xpgMovie->m_GetPage(iPage);

    edPageName->Text = pPage->m_Name;
    lbPageHash->Caption = IntToHex((int)(pPage->m_lHashKey), 8);
    Label3->Caption = IntToStr(iPage);

    for (int i = 0; i < XPG_COMMAND_COUNT; i++) {
		StringGrid1->Cells[0][i+1] = pPage->GetCommandButton(i);
        StringGrid1->Cells[1][i+1] = pPage->GetCommandPage(i);
        StringGrid1->Cells[2][i+1] = pPage->GetCommandAction(i);
    }
    UpdateStringGrid(Sender);
    cbActions->Visible = false;
    cbPages->Visible = false;     
}

//---------------------------------------------------------------------------

void __fastcall TwCommand::StringGrid1DblClick(TObject *Sender)
{
    cbButtons->Visible = false;
    cbPages->Visible = false;
    cbActions->Visible = false;
    cbEvents->Visible = false;
#if 1
    int iCol = StringGrid1->Col;
    int iRow = StringGrid1->Row;
#else
    int iCol = ACol;
    int iRow = ARow;
#endif

    int x = StringGrid1->Left + 2;
    int y = StringGrid1->Top + 1;
    TRect rct = StringGrid1->CellRect(iCol, iRow);

    if (iCol == 0) {
        if (StringGrid1->Cells[iCol][iRow] == "")
            cbButtons->Text = "Select Button";
        else
            cbButtons->Text = StringGrid1->Cells[iCol][iRow];
        cbButtons->Top = rct.Top + y;
        cbButtons->Left = rct.Left + x;
        cbButtons->Width = rct.Right - rct.Left + 2;
        cbButtons->Height = rct.Bottom - rct.Top - 2;
        cbButtons->Visible = true;
    } else if (iCol == 1) {
        //wMain->UpdatePageNameList();
        //SetPageList(wMain->PageNameList);

        if (StringGrid1->Cells[iCol][iRow] == "")
            cbPages->Text = "Select Page";
        else
            cbPages->Text = StringGrid1->Cells[iCol][iRow];
        cbPages->Top = rct.Top + y;
        cbPages->Left = rct.Left + x;
        cbPages->Width = rct.Right - rct.Left + 2;
        cbPages->Height = rct.Bottom - rct.Top - 2;
        cbPages->Visible = true;
    } else if (iCol == 2) {
        if (StringGrid1->Cells[iCol][iRow] == "")
            cbActions->Text = "Select Action";
        else
            cbActions->Text = StringGrid1->Cells[iCol][iRow];
        cbActions->Top = rct.Top + y;
        cbActions->Left = rct.Left + x;
        cbActions->Width = rct.Right - rct.Left + 1;
        cbActions->Height = rct.Bottom - rct.Top - 2;
        cbActions->Visible = true;
    }
    wMain->boModified = true;        
}
//---------------------------------------------------------------------------

void __fastcall TwCommand::StringGrid1SetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
    if (Sender == StringGrid1)
    {
        int iCol = StringGrid1->Col;
        int iRow = StringGrid1->Row;
        AnsiString str = StringGrid1->Cells[iCol][iRow];
        //StringGrid1->Cells[iCol][iRow] = cbPages->Text;
        //cbPages->Visible = false;

        if (xpgMovie->m_pCurPage != NULL) {
			xpgMovie->m_pCurPage->SetCommand(iRow-1, iCol, str);
        }

        UpdateStringGrid(Sender);
    }
}
//---------------------------------------------------------------------------

void __fastcall TwCommand::StringGrid1SelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    cbPages->Visible = false;
    cbActions->Visible = false;
    cbButtons->Visible = false;
    cbEvents->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TwCommand::bnChangeNameClick(TObject *Sender)
{
     if (xpgMovie->m_pCurPage != NULL)
     {
        if(wCommand->edPageName->Text == "")
            ShowMessage("Please enter a valid name.");
        else
        {
            Pages *pPage = xpgMovie->m_pCurPage;
            int iPage = pPage->m_iIndex;

            AnsiString newName = wCommand->edPageName->Text;
            AnsiString oldName = pPage->m_Name;

            pPage->m_Name = newName;
            wPage->ListView1->Items->Item[iPage]->Caption =
                IntToStr(iPage) + " " + newName;

            if (wMain->PageNameList->Capacity > 0)
                wMain->PageNameList->Strings[iPage] = newName;
            if (cbPages->Items->Capacity > 0)
                cbPages->Items->Strings[iPage] = newName;

            //replace old names
            if(oldName != "")
            {
                for(int i = 0; i < xpgMovie->m_iPageCount ; i++)
                {
                    Pages* pPage = xpgMovie->m_GetPage(i);
                    for(int j = 0 ; j < XPG_COMMAND_COUNT; j++)
                    {
                        if(pPage->GetCommandPage(j) == oldName)
                        {
                            pPage->SetCommandPage(j, newName);
                            if(i == j)
                                StringGrid1->Cells[1][j+1] = newName;
                        }
                    }
                }
            }
            wMain->boModified = true;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TwCommand::ClearAndReset(TObject *Sender)
{
    cbPages->Clear();
    lbPageHash->Caption = "";
    Label3->Caption = "None";
    edPageName->Text = "";

    for(int iRow = 1; iRow<=2; iRow++)
    {
        for(int iCol = 1; iCol < XPG_COMMAND_COUNT; iCol++)
        {
            StringGrid1->Cells[iRow][iCol] = "";
        }
    }
}

//---------------------------------------------------------------------------

void __fastcall TwCommand::edPageNameKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Key == VK_RETURN)
        bnChangeNameClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TwCommand::cbButtonsSelect(TObject *Sender)
{
    int iCol = StringGrid1->Col;
    int iRow = StringGrid1->Row;

    StringGrid1->Cells[iCol][iRow] = cbButtons->Text;

    cbButtons->Visible = false;

    if (xpgMovie->m_pCurPage != NULL) {
        xpgMovie->m_pCurPage->SetCommandButton(iRow-1, cbButtons->Text);
    }
}
//---------------------------------------------------------------------------


void __fastcall TwCommand::bnSelectAllClick(TObject *Sender)
{
    TGridRect myRect;
    myRect.Left = 0;
    myRect.Top = 1;
    myRect.Right = StringGrid1->ColCount;
    myRect.Bottom = StringGrid1->RowCount;
    StringGrid1->Selection = myRect;
    StringGrid1->EditorMode = false;
}
//---------------------------------------------------------------------------


void __fastcall TwCommand::cbEventsSelect(TObject *Sender)
{
    int iCol = StringGrid1->Col;
    int iRow = StringGrid1->Row;

    StringGrid1->Cells[iCol][iRow] = cbEvents->Text;

    cbEvents->Visible = false;

    if (xpgMovie->m_pCurPage != NULL) {
        xpgMovie->m_pCurPage->SetCommandEvent(iRow-1, cbEvents->Text);
    }
}
//---------------------------------------------------------------------------


void __fastcall TwCommand::FormResize(TObject *Sender)
{
    int w = StringGrid1->ColWidths[0] + StringGrid1->ColWidths[1];
	StringGrid1->ColWidths[2] = StringGrid1->Width - 28 - w;
}
//---------------------------------------------------------------------------





void __fastcall TwCommand::Copy1Click(TObject *Sender)
{
    TGridRect src = StringGrid1->Selection;

    StringGrid2->ColCount = src.Right - src.Left + 1;
    StringGrid2->RowCount = src.Bottom - src.Top + 1;

    for (int rSrc = src.Top, r1 = 0; rSrc <= src.Bottom; rSrc++, r1++) {
        for (int cSrc = src.Left, c1 = 0; cSrc <= src.Right; cSrc++, c1++) {
            StringGrid2->Cells[c1][r1] = StringGrid1->Cells[cSrc][rSrc];
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TwCommand::Paste1Click(TObject *Sender)
{
    TGridRect dst = StringGrid1->Selection;


    for (int rDst = dst.Top, rSrc = 0; rSrc < StringGrid2->RowCount; rDst++, rSrc++) {
        for (int cDst = dst.Left, cSrc = 0; cSrc < StringGrid2->ColCount; cDst++, cSrc++) {
            StringGrid1->Cells[cDst][rDst] = StringGrid2->Cells[cSrc][rSrc];
        }
    }
}
//---------------------------------------------------------------------------

