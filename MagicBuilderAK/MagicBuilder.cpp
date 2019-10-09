//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("fmMain.cpp", wMain);
USEFORM("fmPage.cpp", wPage);
USEFORM("fmStage.cpp", wStage);
USEFORM("fmSprite.cpp", wSprite);
USEFORM("fmRole.cpp", wRole);
USEFORM("fmCommand.cpp", wCommand);
USEFORM("dgQuality.cpp", DlgQuality);
USEFORM("dgMovieSize.cpp", dlgMovieSize);
USEFORM("dgInsert.cpp", OKBottomDlg);
USEFORM("dgMove.cpp", OKBottomDlg1);
USEFORM("dgTransColor.cpp", DlgTransColor);
USEFORM("dgResize.cpp", DlgResize);
USEFORM("dgThumbArray.cpp", OKBottomDlg2);
USEFORM("dgAbout.cpp", dlgAbout);
USEFORM("fmMovSet.cpp", wMovSet);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TwMain), &wMain);
         Application->CreateForm(__classid(TwRole), &wRole);
         Application->CreateForm(__classid(TDlgQuality), &DlgQuality);
         Application->CreateForm(__classid(TdlgMovieSize), &dlgMovieSize);
         Application->CreateForm(__classid(TwStage), &wStage);
         Application->CreateForm(__classid(TwPage), &wPage);
         Application->CreateForm(__classid(TwCommand), &wCommand);
         Application->CreateForm(__classid(TOKBottomDlg), &OKBottomDlg);
         Application->CreateForm(__classid(TOKBottomDlg1), &OKBottomDlg1);
         Application->CreateForm(__classid(TDlgTransColor), &DlgTransColor);
         Application->CreateForm(__classid(TDlgResize), &DlgResize);
         Application->CreateForm(__classid(TwSprite), &wSprite);
         Application->CreateForm(__classid(TOKBottomDlg2), &OKBottomDlg2);
         Application->CreateForm(__classid(TdlgAbout), &dlgAbout);
         Application->CreateForm(__classid(TwMovSet), &wMovSet);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
