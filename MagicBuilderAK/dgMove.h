//----------------------------------------------------------------------------
#ifndef dgMoveH
#define dgMoveH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TOKBottomDlg1 : public TForm
{
__published:
    TBevel *Bevel1;
    TButton *OKBtn;
    TButton *CancelBtn;
    TComboBox *ComboBox1;
    TStaticText *StaticText1;
    void __fastcall OKBtnClick(TObject *Sender);
    void __fastcall CancelBtnClick(TObject *Sender);
    void __fastcall ComboBox1Change(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:
public:
	virtual __fastcall TOKBottomDlg1(TComponent* AOwner);
    int i_ItemIndex;
};
//----------------------------------------------------------------------------
extern PACKAGE TOKBottomDlg1 *OKBottomDlg1;
//----------------------------------------------------------------------------
#endif    
