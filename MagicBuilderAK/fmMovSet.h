//---------------------------------------------------------------------------

#ifndef fmMovSetH
#define fmMovSetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TwMovSet : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo1;
    TToolBar *ToolBar1;
    TToolButton *ToolButton1;
    TImageList *ImageList1;
    TToolButton *bnUpdate;
    TOpenDialog *OpenDialog1;
    TSaveDialog *SaveDialog1;
    TToolButton *bnSaveAs;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall LoadMovieCmdSetClick(TObject *Sender);
    void __fastcall UpdateSettingClick(TObject *Sender);
    void __fastcall bnSaveAsClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TwMovSet(TComponent* Owner);
    void __fastcall LoadMovieCommandSet(AnsiString FileName);
    void __fastcall SaveMovieCommandSet(AnsiString FileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TwMovSet *wMovSet;
//---------------------------------------------------------------------------
#endif
