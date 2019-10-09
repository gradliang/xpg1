//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "dgMove.h"
#include "xpg.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TOKBottomDlg1 *OKBottomDlg1;
//---------------------------------------------------------------------
__fastcall TOKBottomDlg1::TOKBottomDlg1(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TOKBottomDlg1::OKBtnClick(TObject *Sender)
{
    ModalResult = mrOk;    
}
//---------------------------------------------------------------------------

void __fastcall TOKBottomDlg1::CancelBtnClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TOKBottomDlg1::ComboBox1Change(TObject *Sender)
{
    i_ItemIndex = ComboBox1->ItemIndex;
    OKBtn->Enabled = true;    
}
//---------------------------------------------------------------------------


void __fastcall TOKBottomDlg1::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ComboBox1->Items->Clear();    
}
//---------------------------------------------------------------------------

