//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "dgQuality.h"


//---------------------------------------------------------------------
#pragma resource "*.dfm"
TDlgQuality *DlgQuality;
extern int g_iJpegQuality;
//---------------------------------------------------------------------
__fastcall TDlgQuality::TDlgQuality(TComponent* AOwner)
	: TForm(AOwner)
{
    TrackBar1->Position = g_iJpegQuality;
}
//---------------------------------------------------------------------
void __fastcall TDlgQuality::OKBtnClick(TObject *Sender)
{
    g_iJpegQuality = TrackBar1->Position;
}
//---------------------------------------------------------------------------

void __fastcall TDlgQuality::TrackBar1Change(TObject *Sender)
{
    Label1->Caption = AnsiString(TrackBar1->Position) + "%";    
}
//---------------------------------------------------------------------------

