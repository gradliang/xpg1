//---------------------------------------------------------------------------

#ifndef fmStageH
#define fmStageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ValEdit.hpp>
#include <ActnList.hpp>
class Sprites;

//---------------------------------------------------------------------------
class TwStage : public TForm
{
__published:	// IDE-managed Components
    TPopupMenu *PopupMenu1;
    TMenuItem *BringToFront1;
    TMenuItem *SendToBack1;
    TMenuItem *Delete1;
    TScrollBox *WorkArea;
    TImageList *ImageList1;
    TStatusBar *StatusBar1;
    TPanel *StagePanel;
    TShape *SelFrame;
    TImage *StageImage;
    TSplitter *Splitter1;
    TValueListEditor *ValueListEditor1;
    TLabel *Label2;
    TMenuItem *BringForward1;
    TMenuItem *SendBackward1;
    TMenuItem *ReplaceImage1;
    TMenuItem *SelectImage1;
    TComboBox *cbSprite;
    TImage *BufferImage;
    TToolButton *bnBringToFront;
    TToolButton *bnBringForward;
    TToolButton *bnSendBackward;
    TToolButton *bnSendToBack;
    TLabel *Label1;
    TPanel *ToolPanel;
    TToolBar *ToolBar3;
    TPanel *Panel3;
    TLabel *Label4;
    TComboBox *cmbScale;
    TToolBar *ToolBar1;
    TCoolBar *CoolBar1;
    TTimer *Timer1;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClick(TObject *Sender);
    void __fastcall BringToFront1Click(TObject *Sender);
    void __fastcall SendToBack1Click(TObject *Sender);
    void __fastcall StageImageMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall StageImageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall StageImageMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall Delete1Click(TObject *Sender);
    void __fastcall edPageNameChange(TObject *Sender);
    void __fastcall StringGrid1KeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall cmbScaleChange(TObject *Sender);
    void __fastcall WorkAreaClick(TObject *Sender);
    
    void __fastcall FormResize(TObject *Sender);
	
    void __fastcall ValueListEditor1EditButtonClick(TObject *Sender);
    void __fastcall ValueListEditor1SetEditText(TObject *Sender, int ACol,
          int ARow, const AnsiString Value);
    void __fastcall BringForward1Click(TObject *Sender);
    void __fastcall SendBackward1Click(TObject *Sender);
    void __fastcall ReplaceImage1Click(TObject *Sender);
    void __fastcall ValueListEditor1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall cbSpriteChange(TObject *Sender);
    void __fastcall SelectImage1Click(TObject *Sender);
    void __fastcall bnSendToBackClick(TObject *Sender);
    void __fastcall bnSendBackwardClick(TObject *Sender);
    void __fastcall bnBringForwardClick(TObject *Sender);
    void __fastcall bnBringToFrontClick(TObject *Sender);
    void __fastcall ValueListEditor1KeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall StageImageDblClick(TObject *Sender);
    void __fastcall ToolPanelResize(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall WorkAreaDblClick(TObject *Sender);
    void __fastcall StageImageDragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
    void __fastcall StageImageDragDrop(TObject *Sender, TObject *Source,
          int X, int Y);
private:	// User declarations
public:		// User declarations
    __fastcall TwStage(TComponent* Owner);
    void __fastcall ClearBufferCanvas(TObject *Sender);
    void __fastcall ClearStageCanvas(TObject *Sender);
    void __fastcall ClearPage(TObject *Sender);
    void __fastcall ClearAndReset(TObject *Sender);
    void __fastcall SetMovieRegion(TObject *Sender, int iWidth, int iHeight);
    void __fastcall UpdateStage(TObject *Sender);
    void __fastcall ShowPage(TObject *Sender, int iPage);
    void __fastcall SetRepaintPage(TObject *Sender, int iPage);
    void __fastcall UpdateSpriteFrames(TObject *Sender, int iPage);
    void __fastcall SelectPage(TObject *Sender, int iPage);
    void __fastcall SelectSprite(TObject *Sender, int iSprite);
    void __fastcall UnselectSprite(TObject *Sender);
    void __fastcall UnSelectAllSprite(TObject *Sender, int iPage);
    void __fastcall MoveSelectedSprite(TObject *Sender, int dx, int dy);
    void __fastcall SetSelectedSpritePos(TObject *Sender, int x, int y);
    void __fastcall UpdateEditSprite(TObject *Sender);
	void __fastcall UpdateSpriteValueEditor(TObject *Sender);
    void __fastcall UpdateSpriteToolbar(TObject *Sender);
    void __fastcall UpdateComboSpriteList(TObject *Sender);
	void __fastcall UpdateStatusBar(TObject *Sender);
	void __fastcall UpdateStageInfo(TObject *Sender);

    bool boSpriteDragging;
    int stageWidth;
    int stageHeight;
    int stageColorDepth;
    int stageColorFormat;
    int StageLeft;
    int StageTop;

    bool boStageChanged;
    Sprites *pEditSprite;
	TList *SelectedSpriteList;
    int iEditPage;
    int iDragX;
    int iDragY;
    bool boScaleFit;
    int iStageScale;
    Double dStageScale;
    bool boShiftDown;
    bool boNeedRepaint;

	
};
//---------------------------------------------------------------------------
extern PACKAGE TwStage *wStage;
//---------------------------------------------------------------------------
#endif
