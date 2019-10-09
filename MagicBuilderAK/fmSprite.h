//---------------------------------------------------------------------------

#ifndef fmSpriteH
#define fmSpriteH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TwSprite : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *test;
    TToolBar *ToolBar1;
    TEdit *edValue;
    TPopupMenu *PopupMenu1;
    TMenuItem *Copy1;
    TMenuItem *Paste1;
    TControlBar *ControlBar1;
    TImageList *ImageList1;
    TToolButton *ToolButton2;
    TPanel *Panel1;
    TPopupMenu *PopupMenu2;
    TMenuItem *Copy2;
    TMenuItem *Paste2;
    TStringGrid *StringGrid1;
    TMenuItem *N1;
    TMenuItem *BringForward1;
    TMenuItem *SendBackward1;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall testSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall bnChangeValueClick(TObject *Sender);
    void __fastcall Copy1Click(TObject *Sender);
    void __fastcall Paste1Click(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall testMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall testMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall testDblClick(TObject *Sender);
    void __fastcall edValueKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall ToolButton2Click(TObject *Sender);
    void __fastcall Copy2Click(TObject *Sender);
    void __fastcall Paste2Click(TObject *Sender);
    void __fastcall SendBackward1Click(TObject *Sender);
    void __fastcall BringForward1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TwSprite(TComponent* Owner);

    void __fastcall SelectPage(TObject *Sender, int iPage);
    void __fastcall SelectSprite(TObject *Sender, int iSprite);
    void __fastcall ChangeSpriteProperty(TObject *Sender, int iCol, int iRow, String NewValueString);

    bool boShiftDown;
};
//---------------------------------------------------------------------------
extern PACKAGE TwSprite *wSprite;
//---------------------------------------------------------------------------
#endif
