//----------------------------------------------------------------------------
#ifndef dgTransColorH
#define dgTransColorH
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
#include <Mask.hpp>
//----------------------------------------------------------------------------
class TDlgTransColor : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
    TLabel *Label1;
    TMaskEdit *MaskEdit1;
    void __fastcall OKBtnClick(TObject *Sender);
    void __fastcall CancelBtnClick(TObject *Sender);
private:
public:
	virtual __fastcall TDlgTransColor(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TDlgTransColor *DlgTransColor;
//----------------------------------------------------------------------------
#endif    
