//----------------------------------------------------------------------------
#ifndef dgMovieSizeH
#define dgMovieSizeH
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
class TdlgMovieSize : public TForm
{
__published:
    TBevel *Bevel1;
    TBevel *Bevel2;
    TButton *OKBtn;
    TButton *CancelBtn;
    TLabeledEdit *EditWidth;
    TLabeledEdit *EditHeight;
    TComboBox *cbMovieSize;
    TRadioButton *chkCustomSize;
    TRadioButton *chkDefaultSize;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall EditWidthChange(TObject *Sender);
    void __fastcall OKBtnClick(TObject *Sender);
    void __fastcall EditHeightChange(TObject *Sender);
    void __fastcall CancelBtnClick(TObject *Sender);
    void __fastcall cbMovieSizeChange(TObject *Sender);
private:
public:
	virtual __fastcall TdlgMovieSize(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TdlgMovieSize *dlgMovieSize;
//----------------------------------------------------------------------------
#endif    
