//---------------------------------------------------------------------------

#ifndef fmPageH
#define fmPageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TwPage : public TForm
{
__published:	// IDE-managed Components
    TListView *ListView1;
    TImageList *IconImageList;
    TImage *IconImage;
    TPopupMenu *PopupMenu1;
    TMenuItem *New1;
    TMenuItem *Delete1;
    TMenuItem *Insert1;
    TMenuItem *Duplicate1;
    TMenuItem *MoveTo1;
    TTimer *TimerRefresh;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall Delete1Click(TObject *Sender);
    void __fastcall New1Click(TObject *Sender);
    void __fastcall Insert1Click(TObject *Sender);
    void __fastcall Duplicate1Click(TObject *Sender);
    void __fastcall wRoleCopyName1Click(TObject *Sender);
    void __fastcall MoveTo1Click(TObject *Sender);
    void __fastcall ListView1DblClick(TObject *Sender);
    void __fastcall ListView1SelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
private:	// User declarations
public:		// User declarations
    int iEditPage;

    __fastcall TwPage(TComponent* Owner);
    void __fastcall ClearAndReset(TObject *Sender);
    void __fastcall DeletePage(TObject *Sender, int iPage);
    void __fastcall DeleteAllPages(TObject *Sender);    
    //void __fastcall AddPage(TObject *Sender, int iPage);
    //void __fastcall ShowPage(TObject *Sender, int iPage);
    AnsiString __fastcall GetPageName(int iPage);
    void __fastcall SelectPage(TObject *Sender, int iPage);
    void __fastcall UpdatePage(TObject *Sender, int iPage);
	void __fastcall UpdateAllPages(TObject *Sender);
    void __fastcall AddSprite(TObject *Sender, int iRole);
};
//---------------------------------------------------------------------------
extern PACKAGE TwPage *wPage;
//---------------------------------------------------------------------------
#endif
