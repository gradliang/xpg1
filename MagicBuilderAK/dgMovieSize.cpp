//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "dgMovieSize.h"
#include "xpg.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TdlgMovieSize *dlgMovieSize;

int iDefaultSize[][2] = {
    {176, 220},
    {220, 176},
    {240, 320},
    {320, 240},
    {352, 288},
    {640, 480},
    {720, 480},
    {800, 600},
    {1024, 576},
    {1024, 768},
    {1280, 720},
    {1280, 768},
    {1280, 1024},
    {1600, 1200},
    {1920, 1080}
};

int iCustomWidth = 320;
int iCustomHeight = 240;
int iDefaultWidth = 720;
int iDefaultHeight = 480;
//---------------------------------------------------------------------
__fastcall TdlgMovieSize::TdlgMovieSize(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TdlgMovieSize::FormCreate(TObject *Sender)
{
    iCustomWidth = xpgMovie->m_iScreenWidth;
    iCustomHeight = xpgMovie->m_iScreenHeight;
    EditWidth->Text = IntToStr(xpgMovie->m_iScreenWidth);
    EditHeight->Text = IntToStr(xpgMovie->m_iScreenHeight);
}
//---------------------------------------------------------------------------

void __fastcall TdlgMovieSize::EditWidthChange(TObject *Sender)
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

void __fastcall TdlgMovieSize::EditHeightChange(TObject *Sender)
{
    int iValue = StrToIntDef(EditHeight->Text, -1);
    if (iValue <= 0)
    {
        EditHeight->Text = IntToStr(iCustomHeight);
    }
    else
        iCustomHeight = iValue;
}
//---------------------------------------------------------------------------

void __fastcall TdlgMovieSize::OKBtnClick(TObject *Sender)
{
    if (chkCustomSize->Checked) {
        xpgMovie->m_iScreenWidth = iCustomWidth;
        xpgMovie->m_iScreenHeight = iCustomHeight;
    } else {
        xpgMovie->m_iScreenWidth = iDefaultWidth;
        xpgMovie->m_iScreenHeight = iDefaultHeight;
    }
    ModalResult = mrOk;
}

//---------------------------------------------------------------------------

void __fastcall TdlgMovieSize::CancelBtnClick(TObject *Sender)
{
    ModalResult = mrCancel;    
}
//---------------------------------------------------------------------------

void __fastcall TdlgMovieSize::cbMovieSizeChange(TObject *Sender)
{
    if (cbMovieSize->ItemIndex < 0)
        cbMovieSize->ItemIndex = 0;

    int i = cbMovieSize->ItemIndex;
    iDefaultWidth = iDefaultSize[i][0];
    iDefaultHeight = iDefaultSize[i][1];
}
//---------------------------------------------------------------------------




