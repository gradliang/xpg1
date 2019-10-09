//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "xpg.h"

#include "dgTransColor.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TDlgTransColor *DlgTransColor;


//---------------------------------------------------------------------
__fastcall TDlgTransColor::TDlgTransColor(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TDlgTransColor::OKBtnClick(TObject *Sender)
{
    try{
        xpgMovie->m_iTransColor = StrToInt(MaskEdit1->Text);
        ModalResult = mrOk;
    }
    catch(...)
    {
        ShowMessage("Please Enter a Valid 6-digit Hexdecimal Value.");
    }
}
//---------------------------------------------------------------------------

void __fastcall TDlgTransColor::CancelBtnClick(TObject *Sender)
{
    ModalResult = mrCancel;    
}
//---------------------------------------------------------------------------


