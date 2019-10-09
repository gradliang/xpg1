//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "dgResize.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TDlgResize *DlgResize;
#include "xpg.h"
extern int iDefaultSize[][2];

//---------------------------------------------------------------------
__fastcall TDlgResize::TDlgResize(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TDlgResize::EditWidthChange(TObject *Sender)
{
    int iValue = StrToInt(EditWidth->Text);
    if (iValue <= 0)
    {
        EditWidth->Text = IntToStr(iCustomWidth);
    }
    else
        iCustomWidth = iValue;
}
//---------------------------------------------------------------------------

void __fastcall TDlgResize::EditHeightChange(TObject *Sender)
{
    int iValue = StrToInt(EditHeight->Text);
    if (iValue <= 0)
    {
        EditHeight->Text = IntToStr(iCustomHeight);
    }
    else
        iCustomHeight = iValue;
}
//---------------------------------------------------------------------------

void __fastcall TDlgResize::OKBtnClick(TObject *Sender)
{
    if (chkCustomSize->Checked) {
        xpgMovie->m_iNewScreenWidth = iCustomWidth;
        xpgMovie->m_iNewScreenHeight = iCustomHeight;
    } else {
        xpgMovie->m_iNewScreenWidth = iDefaultWidth;
        xpgMovie->m_iNewScreenHeight = iDefaultHeight;
    }
    ModalResult = mrOk;    
}
//---------------------------------------------------------------------------

void __fastcall TDlgResize::CancelBtnClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TDlgResize::ComboBox1Change(TObject *Sender)
{
    if (ComboBox1->ItemIndex < 0)
        ComboBox1->ItemIndex = 0;

    int i = ComboBox1->ItemIndex;
    iDefaultWidth = iDefaultSize[i][0];
    iDefaultHeight = iDefaultSize[i][1];
}
//---------------------------------------------------------------------------

void __fastcall TDlgResize::FormCreate(TObject *Sender)
{
    int i = 6;
    xpgMovie->m_iNewScreenWidth = iCustomWidth = iDefaultWidth = iDefaultSize[i][0];
    xpgMovie->m_iNewScreenHeight = iCustomHeight = iDefaultHeight = iDefaultSize[i][1];
    EditWidth->Text = IntToStr(iCustomWidth);
    EditHeight->Text = IntToStr(iCustomHeight);
}
//---------------------------------------------------------------------------

