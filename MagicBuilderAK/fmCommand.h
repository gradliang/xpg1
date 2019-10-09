//---------------------------------------------------------------------------

#ifndef fmCommandH
#define fmCommandH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TwCommand : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel2;
    TPanel *Panel1;
    TLabel *Label4;
    TLabel *Label3;
    TLabel *Label2;
    TEdit *edPageName;
    TButton *bnChangeName;
    TStringGrid *StringGrid1;
    TComboBox *cbActions;
    TComboBox *cbPages;
    TPanel *Panel3;
    TComboBox *cbButtons;
    TToolBar *ToolBar1;
    TToolButton *bnSelectAll;
    TToolButton *bnCopy;
    TToolButton *bnPaste;
    TToolButton *bnClear;
    TLabel *lbPageHash;
    TLabel *Label1;
    TComboBox *cbEvents;
    TPopupMenu *PopupMenu1;
    TMenuItem *Copy1;
    TMenuItem *Paste1;
    TStringGrid *StringGrid2;
    TMenuItem *clear1;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall cbActionsSelect(TObject *Sender);
	void __fastcall SetEventList(TStringList *bList);
    void __fastcall cbPagesSelect(TObject *Sender);
    void __fastcall StringGrid1DblClick(TObject *Sender);
    void __fastcall StringGrid1SetEditText(TObject *Sender, int ACol,
          int ARow, const AnsiString Value);
    void __fastcall StringGrid1SelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall bnChangeNameClick(TObject *Sender);
    void __fastcall ClearAndReset(TObject *Sender);
    void __fastcall edPageNameKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall cbButtonsSelect(TObject *Sender);
    void __fastcall bnSelectAllClick(TObject *Sender);
    void __fastcall cbEventsSelect(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall Copy1Click(TObject *Sender);
    void __fastcall Paste1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations

    __fastcall TwCommand(TComponent* Owner);
    void __fastcall UpdateStringGrid(TObject *Sender);
    void __fastcall SetButtonList(TStringList *bList);
    void __fastcall SetActionList(TStringList *aList);
    void __fastcall SetPageList(TStringList *pList);
    void __fastcall ClearStringGridSelection();
    void __fastcall SelectPage(TObject *Sender, int iPage);
};
//---------------------------------------------------------------------------
extern PACKAGE TwCommand *wCommand;
//---------------------------------------------------------------------------
#endif
