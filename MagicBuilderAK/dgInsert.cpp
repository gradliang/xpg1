//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "dgInsert.h"
#include "xpg.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TOKBottomDlg *OKBottomDlg;
//---------------------------------------------------------------------
__fastcall TOKBottomDlg::TOKBottomDlg(TComponent* AOwner)
	: TForm(AOwner)
{

}
//---------------------------------------------------------------------
void __fastcall TOKBottomDlg::OKBtnClick(TObject *Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TOKBottomDlg::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ComboBox1->Items->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TOKBottomDlg::CancelBtnClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TOKBottomDlg::ComboBox1Change(TObject *Sender)
{
    i_ItemIndex = ComboBox1->ItemIndex;
    OKBtn->Enabled = true;
}
//---------------------------------------------------------------------------

