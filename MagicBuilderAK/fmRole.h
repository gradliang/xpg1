//---------------------------------------------------------------------------

#ifndef fmRoleH
#define fmRoleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ExtDlgs.hpp>
#include <ComCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <DB.hpp>
#include <DBClient.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Provider.hpp>
#include <Menus.hpp>
#include <DdeMan.hpp>

#include "xpg.h"
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TwRole : public TForm
{
__published:	// IDE-managed Components
    TStatusBar *StatusBar1;
    TImageList *ImageList1;
    TSaveDialog *SaveDialog1;
    TOpenDialog *OpenDialog1;
    TSplitter *Splitter1;
    TImageList *IconImageList;
    TToolBar *ToolBar1;
    TToolButton *bnImportImage;
    TToolButton *bnExportImage;
    TScrollBox *ScrollBox2;
    TListView *ListView1;
    TImage *IconImage;
    TImage *RoleImage;
    TShape *Shape1;
    TShape *Shape2;
    TToolButton *bnPutOnStage;
    TPopupMenu *PopupMenu1;
    TMenuItem *Delete1;
    TMenuItem *PutonStage1;
    TMenuItem *PutonNewpage1;
    TMenuItem *ReplaceImage1;
    TEdit *edName;
    TToolButton *bnDelete;
    TToolButton *bnDeleteUnused;
    TToolButton *ToolButton1;
    TColorDialog *ColorDialog1;
    TComboBox *cbRoleType;
    TComboBox *cbRoleQuality;
    TCheckBox *CheckBox1;
    TCheckBox *CheckBox2;
    TShape *ShapeTransColor;
    TToolBar *ToolBar2;
    TToolBar *ToolBar3;
    TCoolBar *CoolBar1;
    TCoolBar *CoolBar2;
    TMenuItem *Copy1;
    TMenuItem *Paste1;
    TPopupMenu *PopupMenu2;
    TMenuItem *Copy2;
    TMenuItem *Paste2;
    TMenuItem *Import1;
    TMenuItem *Export1;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *DelUnused1;
    TMenuItem *ImportImages1;
    TMenuItem *ExportImage1;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *N6;
    void __fastcall ListView1Resize(TObject *Sender);
    void __fastcall bnImportImageClick(TObject *Sender);
    void __fastcall bnExportImageClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall bnPutOnStageClick(TObject *Sender);
    void __fastcall PutonStage1Click(TObject *Sender);
    void __fastcall Delete1Click(TObject *Sender);
    void __fastcall PutonNewpage1Click(TObject *Sender);
    void __fastcall edNameKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall ReplaceImage1Click(TObject *Sender);
    void __fastcall ListView1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall CheckBox2Click(TObject *Sender);
    void __fastcall ToolButton3Click(TObject *Sender);
    void __fastcall Shape2MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall bnDeleteClick(TObject *Sender);
    void __fastcall bnDeleteUnusedClick(TObject *Sender);
    void __fastcall cbRoleTypeChange(TObject *Sender);
    void __fastcall cbRoleQualityChange(TObject *Sender);
    void __fastcall ListView1SelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
    void __fastcall Copy1Click(TObject *Sender);
    void __fastcall Paste1Click(TObject *Sender);


private:	// User declarations
public:		// User declarations
    ImageFileType FileType;

    __fastcall TwRole(TComponent* Owner);
    void * __fastcall LoadImageSource( RoleImages *pCurRole, AnsiString FileName );
    bool __fastcall ExportImage(TObject *Sender, int iRole, AnsiString sPath);
    void __fastcall ImportImage(TObject *Sender, int iRole,
                                AnsiString FileName, bool boAppend);
    void __fastcall ClearAndReset(TObject *Sender);
    void __fastcall AddRole(TObject *Sender, int iRole);
    bool __fastcall DrawRole(TObject *Sender, int iRole);
    bool __fastcall ShowRole(TObject *Sender, int iRole);
    void __fastcall SelectRole(TObject *Sender, int iRole);
    void __fastcall ResizeRole(TObject *Sender, int iRole, float xRatio, float yRatio);
    void __fastcall SetRoleRegion(TObject *Sender, int width, int height);
    void __fastcall ResizeRoleRegion(TObject *Sender, int width, int height, float xRatio, float yRatio);
    bool __fastcall ExportJpegCcode(TObject *Sender, AnsiString FileName);
    int __fastcall GetBitsFromBitmap( HBITMAP hBitmap, Byte *lpBits, int remove_alpha );
};
//---------------------------------------------------------------------------
extern PACKAGE TwRole *wRole;

//---------------------------------------------------------------------------
#endif
