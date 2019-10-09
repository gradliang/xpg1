//---------------------------------------------------------------------------

#ifndef fmMainH
#define fmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>


//---------------------------------------------------------------------------
class TwMain : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *New1;
    TMenuItem *LoadXPW;
    TMenuItem *SaveXPW1;
    TMenuItem *N1;
    TMenuItem *ImportImages1;
    TMenuItem *ExportImages1;
    TMenuItem *N2;
    TMenuItem *Exit1;
    TImageList *ImageList1;
    TToolBar *ToolBar1;
    TToolButton *bnLoad;
    TToolButton *bnSave;
    TMenuItem *N4;
    TTimer *TimerInit;
    TMenuItem *Edit1;
    TMenuItem *NewPage1;
    TMenuItem *SaveXPG1;
    TMenuItem *N5;
    TMenuItem *Reopen1;
    TMenuItem *recentFile4;
    TMenuItem *recentFile3;
    TMenuItem *recentFile5;
    TMenuItem *recentFile2;
    TMenuItem *recentFile1;
    TMenuItem *recentFile0;
    TToolButton *bnReopen1;
    TMenuItem *Window1;
    TMenuItem *Stage1;
    TMenuItem *Media1;
    TMenuItem *Page1;
    TMenuItem *Command1;
    TToolButton *ToolButton1;
    TMenuItem *N6;
    TMenuItem *LoadMovieCmdSet;
    TMenuItem *SpriteList1;
    TMenuItem *ThumbArray1;
    TMenuItem *ScreenSize1;
    TMenuItem *Help1;
    TMenuItem *About1;
    TMenuItem *ClearPage1;
    TMenuItem *N3;
    TMenuItem *SaveXPI1;
    TPanel *Panel1;
    TLabel *Label1;
    TTrackBar *TrackBar1;
    TMenuItem *SetFile1;
    TMenuItem *Image1;
    TMenuItem *Import1;
    TMenuItem *Export1;
    TMenuItem *DeleteSelected1;
    TMenuItem *DeleteUnused1;
    TMenuItem *LoadXPI1;
    TMenuItem *DeletAllPages1;
    TMenuItem *ExportAllImages1;
    TMenuItem *N7;
    TMenuItem *ImportAllImages1;
    void __fastcall New1Click(TObject *Sender);
    void __fastcall LoadXPWClick(TObject *Sender);
    void __fastcall SaveXPW1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);

    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall ExportImages1Click(TObject *Sender);
    void __fastcall ImportImages1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

    void __fastcall TimerInitTimer(TObject *Sender);
    void __fastcall NewPage1Click(TObject *Sender);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall SaveXPG1Click(TObject *Sender);

    void __fastcall recentFile0Click(TObject *Sender);
    void __fastcall recentFile1Click(TObject *Sender);
    void __fastcall recentFile2Click(TObject *Sender);
    void __fastcall recentFile3Click(TObject *Sender);
    void __fastcall recentFile4Click(TObject *Sender);
    void __fastcall recentFile5Click(TObject *Sender);
    void __fastcall Stage1Click(TObject *Sender);
    void __fastcall Media1Click(TObject *Sender);
    void __fastcall Page1Click(TObject *Sender);
    void __fastcall Command1Click(TObject *Sender);

    void __fastcall ToolButton1Click(TObject *Sender);
    void __fastcall LoadMovieCmdSetClick(TObject *Sender);
    void __fastcall SpriteList1Click(TObject *Sender);
    void __fastcall ScreenSize1Click(TObject *Sender);
    void __fastcall About1Click(TObject *Sender);
    void __fastcall ClearPage1Click(TObject *Sender);
    void __fastcall TrackBar1Change(TObject *Sender);
    void __fastcall SetFile1Click(TObject *Sender);
    void __fastcall Import1Click(TObject *Sender);
    void __fastcall Export1Click(TObject *Sender);
    void __fastcall DeleteSelected1Click(TObject *Sender);
    void __fastcall DeleteUnused1Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall DeletAllPages1Click(TObject *Sender);
    void __fastcall ExportAllImages1Click(TObject *Sender);
    void __fastcall ImportAllImages1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    AnsiString stCaptionHead;
    AnsiString stCurXpgFile;
    TStringList *ButtonList;
    //TStringList *ActionList;
    TStringList *PageNameList;

    AnsiString stMovieFilName;
    AnsiString stCurPath;

    bool boNewFile;
    bool boModified;
    bool boControlKey;
    bool boExit;

    __fastcall TwMain(TComponent* Owner);
    void __fastcall ClearAndReset(TObject *Sender);
    void __fastcall SetMovieRegion(TObject *Sender, int width, int height);
    void __fastcall ResizeMovieRegion(TObject *Sender, int width, int height);
    void __fastcall LoadXPG(TObject *Sender, AnsiString FileName);
    bool __fastcall ExportCcode(TObject *Sender, AnsiString FileName);
    void __fastcall LoadINI(AnsiString FileName);
    void __fastcall SaveINI();
    void __fastcall AddRecentFile(AnsiString FileName);
    void __fastcall UpdatePageNameList();
    void __fastcall LoadMovieCommandSet(AnsiString FileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TwMain *wMain;
extern int g_iJpegQuality;
//---------------------------------------------------------------------------
#endif
