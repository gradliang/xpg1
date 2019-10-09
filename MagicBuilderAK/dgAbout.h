//----------------------------------------------------------------------------
#ifndef dgAboutH
#define dgAboutH
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
class TdlgAbout : public TForm
{
__published:        
	TButton *OKBtn;
    TLabel *Label1;
    TLabel *Label2;
    TPanel *Panel1;
    TImage *Image1;
    TPanel *Panel2;
private:
public:
	virtual __fastcall TdlgAbout(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TdlgAbout *dlgAbout;
//----------------------------------------------------------------------------
#endif    
