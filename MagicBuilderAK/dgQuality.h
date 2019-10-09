//----------------------------------------------------------------------------
#ifndef dgQualityH
#define dgQualityH
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
#include <ComCtrls.hpp>
//----------------------------------------------------------------------------
class TDlgQuality : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
    TTrackBar *TrackBar1;
    TLabel *Label1;
    void __fastcall OKBtnClick(TObject *Sender);
    void __fastcall TrackBar1Change(TObject *Sender);
private:
public:
	virtual __fastcall TDlgQuality(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TDlgQuality *DlgQuality;
//----------------------------------------------------------------------------
#endif    
