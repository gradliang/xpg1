//----------------------------------------------------------------------------
#ifndef dgThumbArrayH
#define dgThumbArrayH
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
class TOKBottomDlg2 : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
    TLabeledEdit *EditRow;
    TLabeledEdit *EditHeight;
private:
public:
	virtual __fastcall TOKBottomDlg2(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TOKBottomDlg2 *OKBottomDlg2;
//----------------------------------------------------------------------------
#endif    
