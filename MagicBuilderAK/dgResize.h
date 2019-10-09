//----------------------------------------------------------------------------
#ifndef dgResizeH
#define dgResizeH
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
class TDlgResize : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
    TLabeledEdit *EditWidth;
    TLabeledEdit *EditHeight;
    TComboBox *ComboBox1;
    TBevel *Bevel2;
    TRadioButton *chkDefaultSize;
    TRadioButton *chkCustomSize;
    void __fastcall EditWidthChange(TObject *Sender);
    void __fastcall EditHeightChange(TObject *Sender);
    void __fastcall OKBtnClick(TObject *Sender);
    void __fastcall CancelBtnClick(TObject *Sender);
    void __fastcall ComboBox1Change(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:
public:
	virtual __fastcall TDlgResize(TComponent* AOwner);

    int iResizeWidth;
    int iResizeHeight;

    int iCustomWidth;
    int iCustomHeight;
    int iDefaultWidth;
    int iDefaultHeight;
};
//----------------------------------------------------------------------------
extern PACKAGE TDlgResize *DlgResize;
//----------------------------------------------------------------------------
#endif    
