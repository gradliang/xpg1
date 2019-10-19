//---------------------------------------------------------------------------
// MagicPixel UI Builder System
//
// main program
//
// v2.1.2 2006.2.14 - fix page number 0 become 1 error
// v2.1.3 2006.2.18 - fix delete page error
// v2.1.3 2006.3.30 - fix drag error and role type can't select
//---------------------------------------------------------------------------

#include <vcl.h>
#include <IniFiles.hpp>
#include <process.h>
#pragma hdrstop

#include <stdlib.h>
#include <time.h>
#include "fmMain.h"
#include "fmRole.h"
#include "fmPage.h"
#include "fmStage.h"
#include "fmSprite.h"
#include "fmCommand.h"
#include "fmMovSet.h"

#include "dgQuality.h"
#include "dgMovieSize.h"
#include "dgAbout.h"
#include "dgResize.h"
#include "xpg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TwMain *wMain;
int g_iJpegQuality;
AnsiString gExtPlayerFileName;
AnsiString gTempMovieFileName;
AnsiString gLastDir;
AnsiString gLastFile;
extern AnsiString gCommandSetFileName;
int saveMovieWidth=0;
int saveMovieHeight=0;
//---------------------------------------------------------------------------
__fastcall TwMain::TwMain(TComponent* Owner)
    : TForm(Owner)
{
    stCaptionHead = Caption;
    Caption = stCaptionHead + "New File";
    g_iJpegQuality = 80;
    boNewFile = true;
    boModified = false;
    boExit = false;
    srand(time(NULL));
}
//---------------------------------------------------------------------------
void __fastcall TwMain::FormCreate(TObject *Sender)
{
    //ButtonList = new TStringList;
    //ActionList = new TStringList;
    PageNameList = new TStringList;
    //SpriteTypeList = new TStringList;
    TrackBar1->Position = g_iJpegQuality;
    xpg_open();
}
//---------------------------------------------------------------------------

void __fastcall TwMain::TimerInitTimer(TObject *Sender)
{
    TimerInit->Enabled = false;
    New1Click(TimerInit);
    //SetMovieRegion(Sender, 176, 220);
    LoadINI(ChangeFileExt( Application->ExeName, ".ini" ));
    if (gCommandSetFileName == "")
        gCommandSetFileName = ChangeFileExt( Application->ExeName, ".set" );
    LoadMovieCommandSet(gCommandSetFileName);
}
//---------------------------------------------------------------------------
void __fastcall TwMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    boExit = true;
    if (boModified) {
        int iRes = MessageDlg("Movie has been modified, Save Movie before close?",
               mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0);
        if (iRes == mrYes)
            SaveXPW1Click(Sender);
    }

    SaveINI(); //save status before clear all
    ClearAndReset(Sender);

    //delete xpgMovie;
    xpg_close();
    //delete ButtonList;
    //delete ActionList;
    delete PageNameList;
    //delete SpriteTypeList;
}
//---------------------------------------------------------------------------
void __fastcall TwMain::Exit1Click(TObject *Sender)
{
    boExit = true;
    if (boModified) {
        int iRes = MessageDlg("Movie has been modified, Save Movie before close?",
               mtWarning, mbYesNoCancel	, 0);
        if (iRes == mrYes) {
            SaveXPW1Click(Sender);
        } else if (iRes == mrCancel)
            return;

        ClearAndReset(Sender);
        boModified = false;
    }
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TwMain::ClearAndReset(TObject *Sender)
{
    xpgMovie->m_Clear();
    wRole->ClearAndReset(Sender);
    wPage->ClearAndReset(Sender);
    wStage->ClearAndReset(Sender);
    wCommand->ClearAndReset(Sender);
    Caption = stCaptionHead + "New File";
    boNewFile = true;
    boModified = false;
}
//---------------------------------------------------------------------------

void __fastcall TwMain::New1Click(TObject *Sender)
{
    ClearAndReset(Sender);
    NewPage1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TwMain::LoadXPG(TObject *Sender, AnsiString FileName)
{ 
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;    // Show hourglass cursor
    //Screen->Cursor = Save_Cursor;

    gLastFile = FileName;
    FileName.Delete(FileName.Pos("&"), 1);

    ClearAndReset(Sender);
    boModified = false;

    stCurXpgFile = FileName;

    if (!xpgMovie->m_LoadFile(FileName.c_str())) {
        Screen->Cursor = Save_Cursor;
        Application->MessageBox("File read fail!", "Error", MB_OK);
        return;
    }
    Caption = stCaptionHead + stCurXpgFile;// + "-V" + String(xpgMovie->m_iVersion / 1000.);      //set Window caption

        saveMovieWidth = xpgMovie->m_iScreenWidth;
        saveMovieHeight = xpgMovie->m_iScreenHeight;
	SetMovieRegion(NULL, xpgMovie->m_iScreenWidth, xpgMovie->m_iScreenHeight);
	
    xpgMovie->m_iRoleCount = xpgMovie->m_RoleList->Count;
    for (int iRole = 0; iRole < xpgMovie->m_iRoleCount; iRole++)
    {
        wRole->AddRole(Sender, iRole);
    }

    xpgMovie->m_iPageCount = xpgMovie->m_PageList->Count;
    wPage->UpdateAllPages(Sender);

    wRole->ListView1->ItemIndex = 0;   
    wRole->ListView1->Update();
    //wPage->ListView1->Update();

    if (xpgMovie->m_iPageCount > 0) {
        wPage->ListView1->ItemIndex = 0;
        wStage->SelectPage( Sender, 0 );
    }

    boNewFile = false;
    
    stCurXpgFile = ChangeFileExt(stCurXpgFile,".xpw");
    UpdatePageNameList();
    Screen->Cursor = Save_Cursor;
}
//---------------------------------------------------------------------------
void __fastcall TwMain::LoadXPWClick(TObject *Sender)
{
    if (boModified) {
        int iRes = MessageDlg("Movie has been modified, Save Movie before load?",
               mtWarning, mbYesNoCancel	, 0);
        if (iRes == mrYes) {
            SaveXPW1Click(Sender);
        } else if (iRes == mrCancel)
            return;
    }

    TOpenDialog *o = new TOpenDialog( this );
    o->DefaultExt = "xpw";
    o->Filter = "Multi JPEG Work File (*.xpw)|*.xpw";

    if ( o->Execute() )
    {
        AddRecentFile(o->FileName);
        LoadXPG(Sender, o->FileName);
        //LoadMovieCommandSet(gCommandSetFileName);
    }
    delete o;
    boModified = false;
}
//---------------------------------------------------------------------------
bool __fastcall TwMain::ExportCcode(TObject *Sender, AnsiString FileName)
{
    //FILE *fpDst = NULL;
    FILE *fpSrc = NULL;
    AnsiString TextFileName = ChangeFileExt(FileName,".txt");

    if ((fpSrc = fopen(FileName.c_str(), "rb")) == NULL) return false;

    
	{
        fseek(fpSrc, 0, SEEK_END);
        long lFileSize = ftell(fpSrc);
        long lDataLen = lFileSize;
        if (lDataLen % 4 != 0) {
            lDataLen += 4 - (lDataLen % 4);
        }

        fseek( fpSrc, 0, SEEK_SET );
        unsigned char *pszBuffer;
        pszBuffer = new unsigned char[lDataLen + 1];

        unsigned char *pszBuffer1;
        pszBuffer1 = new unsigned char[lDataLen + 1];
        memset(pszBuffer, 0, lDataLen);
        memset(pszBuffer1, 0, lDataLen);
        fread( pszBuffer + 4, 1, lFileSize, fpSrc ); 

        fclose(fpSrc);

        if ((fpSrc = fopen(FileName.c_str(), "wb")) != NULL) {

            fseek( fpSrc, 0, SEEK_SET );
            for (int i = 0; i < lDataLen; i+=4) {
                pszBuffer1[i+0] = pszBuffer[i+3];
                pszBuffer1[i+1] = pszBuffer[i+2];
                pszBuffer1[i+2] = pszBuffer[i+1];
                pszBuffer1[i+3] = pszBuffer[i+0];
            }

            fwrite( pszBuffer1 + 4, 1, lDataLen, fpSrc );
            fclose(fpSrc);
        }
        if (lDataLen <= 0) return -1;

        unsigned long *pulBuffer = (unsigned long *)pszBuffer;
        //long lDwDataLen = lDataLen / 4;
        *pulBuffer = lDataLen;
#if 0
		if ((fpDst = fopen(TextFileName.c_str(), "w")) == NULL) return false; 
        fprintf(fpDst, "DWORD xpgTempMovieBuffer[] = {");
        fprintf(fpDst, "%d,\n", lDataLen);

        for (int i = 1, j = 0; i < lDwDataLen; i++, j++) {
            if (j ==  5) {
                fputc('\n', fpDst);
                j = 0;
            }
            if (i == lDwDataLen - 1)
                fprintf(fpDst, "0x%08X", pulBuffer[i]);
            else
                fprintf(fpDst, "0x%08X,", pulBuffer[i]);
        }

        
        fprintf(fpDst, "\n};\n");
        fclose(fpDst);
#endif		
		delete pszBuffer;
    }
    fclose(fpSrc);
    return true;
}
//---------------------------------------------------------------------------

void __fastcall TwMain::SaveXPG1Click(TObject *Sender)
{
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;    // Show hourglass cursor
    TSaveDialog *o = new TSaveDialog( this );
    o->FileName = ChangeFileExt(stCurXpgFile,".xpg");
    o->DefaultExt = "xpg";
    o->Filter = "Multi JPEG Data(.xpg)|*.xpg|";

    if ( o->Execute() )
    {
        if (1)//(DlgQuality->ShowModal() == mrOk)
        {
            TCursor Save_Cursor = Screen->Cursor;
            Screen->Cursor = crHourGlass;    // Show hourglass cursor
            //Screen->Cursor = Save_Cursor;
            AnsiString fileName = ChangeFileExt(o->FileName,".xpg");
            xpgMovie->m_boXPW = false;
            xpgMovie->m_SaveFile(fileName.c_str(), g_iJpegQuality);

            //ExportCcode(Sender, fileName);
            Screen->Cursor = Save_Cursor;
        }
    }

    delete o;
    Screen->Cursor = Save_Cursor;
}
//---------------------------------------------------------------------------

void __fastcall TwMain::SaveXPW1Click(TObject *Sender)
{
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;    // Show hourglass cursor
    //if(boModified==true)
    //{
        TSaveDialog *o = new TSaveDialog( this );
        o->FileName = ChangeFileExt(stCurXpgFile,".xpw");
        o->DefaultExt = "xpw";
        o->Filter = "Multi JPEG Work File (*.xpw)|*.xpw|";

        if ( o->Execute() )
        {
            AnsiString fileName = ChangeFileExt(o->FileName,".xpw");
            AddRecentFile(fileName);
            xpgMovie->m_boXPW = true;
            xpgMovie->m_SaveFile(fileName.c_str(), 100);

            stCurXpgFile = fileName;
            boModified = false;
        }

        delete o;
    //}
    //else
        //ShowMessage("The file has not been modified.");
    Screen->Cursor = Save_Cursor;
}

//---------------------------------------------------------------------------
void __fastcall TwMain::SetMovieRegion(TObject *Sender, int width, int height)
{
    wRole->SetRoleRegion(Sender, width, height);
    wStage->SetMovieRegion(Sender, width, height);

    //if (Sender != NULL)
    {
        xpgMovie->m_iScreenWidth = width;
        xpgMovie->m_iScreenHeight = height;
    }
    AnsiString PageSize = "Page Size: " + String(width) + "x" + String(height);
    wStage->StatusBar1->Panels->Items[0]->Text = PageSize;

}
//---------------------------------------------------------------------------
void __fastcall TwMain::ResizeMovieRegion(TObject *Sender, int width, int height)
{
    //float xRatio = (float)width / xpgMovie->m_iScreenWidth;
    //float yRatio = (float)height / xpgMovie->m_iScreenHeight;
    float xRatio = (float)width / saveMovieWidth;
    float yRatio = (float)height / saveMovieHeight;

    wRole->ResizeRoleRegion(Sender, width, height, xRatio, yRatio);
    //wStage->ResizeMovieRegion(Sender, width, height, xRatio, yRatio);
    //xpgMovie->ResizeMovieRegio(width, height, xRatio, yRatio);
    //if (Sender != NULL)
    {
        xpgMovie->m_iScreenWidth = width;
        xpgMovie->m_iScreenHeight = height;
    }
    AnsiString PageSize = "Page Size: " + String(width) + "x" + String(height);
    wStage->StatusBar1->Panels->Items[0]->Text = PageSize;

}
//---------------------------------------------------------------------------

void __fastcall TwMain::ScreenSize1Click(TObject *Sender)
{
    if (dlgMovieSize->ShowModal() == mrOk)
    {
        SetMovieRegion( Sender, xpgMovie->m_iScreenWidth, xpgMovie->m_iScreenHeight );
        ResizeMovieRegion( Sender, xpgMovie->m_iScreenWidth, xpgMovie->m_iScreenHeight);
    }
}

//---------------------------------------------------------------------------

void __fastcall TwMain::ExportImages1Click(TObject *Sender)
{
    wRole->bnExportImageClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TwMain::ImportImages1Click(TObject *Sender)
{
    wRole->bnImportImageClick(Sender);    
}

//---------------------------------------------------------------------------

void __fastcall TwMain::NewPage1Click(TObject *Sender)
{
    int iPage = xpgMovie->m_AddPage();
    wStage->ShowPage(NULL, iPage);
    wPage->UpdatePage(Sender, iPage);
    wPage->SelectPage(Sender, iPage);
    UpdatePageNameList();
}
//---------------------------------------------------------------------------

void __fastcall TwMain::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    boControlKey = Shift.Contains(ssCtrl);
}

//---------------------------------------------------------------------------

void __fastcall TwMain::LoadINI(AnsiString FileName)
{
    TIniFile *ini;
    ini = new TIniFile( FileName );

    gLastDir = ini->ReadString( "DIR", "initdir", "" );
    gExtPlayerFileName = ini->ReadString( "EXTERN", "external player", "" );
    gTempMovieFileName = ini->ReadString( "EXTERN", "temp file", "c:/Temp/xg.tmp" );

    for (int i = 0; i < 6; i++) {
        AnsiString str = ini->ReadString( "FILES", "recent" + AnsiString(i), "");
        if (str != "") {
            Reopen1->Items[i]->Caption = str;
            Reopen1->Items[i]->Visible = true;
            Reopen1->Items[i]->Enabled = true;
        } else {
            Reopen1->Items[i]->Visible = false;
        }
    }

    int w = ini->ReadInteger( "MOVIES", "width", 176 );
    int h = ini->ReadInteger( "MOVIES", "height", 220 );
    gLastDir = ini->ReadString( "MOVIES", "LastDir", "" );
    gLastFile = ini->ReadString( "MOVIES", "LastFile", "" );

    gCommandSetFileName = ini->ReadString( "MOVIES", "Command set file", ChangeFileExt( Application->ExeName, ".set" ));

   /* wStage->Visible = ini->ReadBool( "WINDOWS", "wStage", true);
    wPage->Visible = ini->ReadBool( "WINDOWS", "wPage", true);
    wRole->Visible = ini->ReadBool( "WINDOWS", "wRole", true); */
#if 1
    wStage->WindowState = (TWindowState)ini->ReadInteger( "WINDOWS", "wStage", wsNormal);
    wPage->WindowState = (TWindowState)ini->ReadInteger( "WINDOWS", "wPage", wsNormal);
    wRole->WindowState = (TWindowState)ini->ReadInteger( "WINDOWS", "wRole", wsNormal);
    wSprite->WindowState = (TWindowState)ini->ReadInteger( "WINDOWS", "wSprite", wsNormal);
    wCommand->WindowState = (TWindowState)ini->ReadInteger( "WINDOWS", "wCommand", wsNormal);
	wMovSet->WindowState = (TWindowState)ini->ReadInteger( "WINDOWS", "wMovSet", wsNormal);
#endif
    SetMovieRegion(wMain, w, h);

   // LoadXPG(NULL, gLastFile);
    delete ini;
}

//---------------------------------------------------------------------------

void __fastcall TwMain::SaveINI()
{
    TIniFile *ini;
    ini = new TIniFile(ChangeFileExt( Application->ExeName, ".ini" ) );

    ini->WriteString( "DIR", "initdir", gLastDir );
    ini->WriteString( "EXTERN", "external player", gExtPlayerFileName );
    ini->WriteString( "EXTERN", "temp file", gTempMovieFileName );

    for (int i = 0; i < 6; i++) {
        ini->WriteString( "FILES", "recent" + AnsiString(i), Reopen1->Items[i]->Caption );
    }

    ini->WriteInteger( "MOVIES", "width", xpgMovie->m_iScreenWidth );
    ini->WriteInteger( "MOVIES", "height", xpgMovie->m_iScreenHeight );

    ini->WriteString( "MOVIES", "LastDir", gLastDir );
    ini->WriteString( "MOVIES", "LastFile", gLastFile );

    ini->WriteString( "MOVIES", "Command set file", gCommandSetFileName);

    ini->WriteBool( "WINDOWS", "wStage", wStage->Visible);
    ini->WriteBool( "WINDOWS", "wPage", wPage->Visible);
    ini->WriteBool( "WINDOWS", "wRole", wRole->Visible);
    ini->WriteInteger( "WINDOWS", "wStage", wStage->WindowState);
    ini->WriteInteger( "WINDOWS", "wPage", wPage->WindowState);
    ini->WriteInteger( "WINDOWS", "wRole", wRole->WindowState);
    ini->WriteInteger( "WINDOWS", "wSprite", wSprite->WindowState);
    ini->WriteInteger( "WINDOWS", "wCommand", wCommand->WindowState);
	ini->WriteInteger( "WINDOWS", "wMovSet", wMovSet->WindowState);

    ini->WriteInteger( "Window Position", "wStage Left", wStage->Left );
    ini->WriteInteger( "Window Position", "wStage Top", wStage->Top );

    ini->WriteInteger( "Window Position", "wPage Left", wPage->Left );
    ini->WriteInteger( "Window Position", "wPage Top", wPage->Top );

    ini->WriteInteger( "Window Position", "wRole Left", wRole->Left );
    ini->WriteInteger( "Window Position", "wRole Top", wRole->Top );

    ini->WriteInteger( "Window Position", "wSprite Left", wSprite->Left );
    ini->WriteInteger( "Window Position", "wSprite Top", wSprite->Top );
    delete ini;
}

//---------------------------------------------------------------------------

void __fastcall TwMain::AddRecentFile(AnsiString FileName)
{
    gLastFile = FileName;
    FileName.Delete(FileName.Pos("&"), 1);
    int n = Reopen1->Count;

    TMenuItem *curItem = Reopen1->Items[n - 1];
    for (int i = 0; i < n; i++) {
        AnsiString str = Reopen1->Items[i]->Caption;
        str.Delete(str.Pos("&"), 1);
        if (str == FileName) {
             curItem = Reopen1->Items[i];
             break;
        }
    }

    Reopen1->Remove(curItem);
    curItem->Caption = FileName;
    curItem->Visible = true;

    Reopen1->Insert(0, curItem);
    curItem->AutoHotkeys = maManual;
}
//---------------------------------------------------------------------------

void __fastcall TwMain::recentFile0Click(TObject *Sender)
{
    AnsiString fileName = recentFile0->Caption;
    recentFile0->MenuIndex = 0;
    LoadXPG(Sender, fileName);
}
//---------------------------------------------------------------------------

void __fastcall TwMain::recentFile1Click(TObject *Sender)
{
    AnsiString fileName = recentFile1->Caption;
    recentFile1->MenuIndex = 0;
    LoadXPG(Sender, fileName);
}
//---------------------------------------------------------------------------

void __fastcall TwMain::recentFile2Click(TObject *Sender)
{
    AnsiString fileName = recentFile2->Caption;
    recentFile2->MenuIndex = 0;
    LoadXPG(Sender, fileName);
}
//---------------------------------------------------------------------------

void __fastcall TwMain::recentFile3Click(TObject *Sender)
{
    AnsiString fileName = recentFile3->Caption;
    recentFile3->MenuIndex = 0;
    LoadXPG(Sender, fileName);
}
//---------------------------------------------------------------------------

void __fastcall TwMain::recentFile4Click(TObject *Sender)
{
    AnsiString fileName = recentFile4->Caption;
    recentFile4->MenuIndex = 0;
    LoadXPG(Sender, fileName);
}
//---------------------------------------------------------------------------

void __fastcall TwMain::recentFile5Click(TObject *Sender)
{
    AnsiString fileName = recentFile5->Caption;
    recentFile5->MenuIndex = 0;
    LoadXPG(Sender, fileName);
}
//---------------------------------------------------------------------------
void __fastcall TwMain::UpdatePageNameList()
{
    PageNameList->Clear();
    for (int iPage = 0; iPage < xpgMovie->m_iPageCount; iPage++)
    {
        Pages *pPage = xpgMovie->m_GetPage(iPage);
        PageNameList->Add( pPage->m_Name );
    }
    wCommand->SetPageList(PageNameList);
}
//---------------------------------------------------------------------------

void __fastcall TwMain::Stage1Click(TObject *Sender)
{
    TForm *pForm = wStage;
    pForm->BringToFront();
    pForm->Show();
    pForm->WindowState = wsNormal;
}
//---------------------------------------------------------------------------

void __fastcall TwMain::Media1Click(TObject *Sender)
{
    TForm *pForm = wRole;
    pForm->BringToFront();
    pForm->Show();
    pForm->WindowState = wsNormal;
}
//---------------------------------------------------------------------------

void __fastcall TwMain::Page1Click(TObject *Sender)
{
    TForm *pForm = wPage;
    pForm->BringToFront();
    pForm->Show();
    pForm->WindowState = wsNormal;
}
//---------------------------------------------------------------------------

void __fastcall TwMain::Command1Click(TObject *Sender)
{
    TForm *pForm = wCommand;
    pForm->BringToFront();
    pForm->Show();
    pForm->WindowState = wsNormal;
}
//---------------------------------------------------------------------------

void __fastcall TwMain::ToolButton1Click(TObject *Sender)
{
    if (DlgResize->ShowModal() == mrOk)  {
        int width = xpgMovie->m_iNewScreenWidth;
        int height = xpgMovie->m_iNewScreenHeight;
        if (width == 0 || height == 0 ||
        xpgMovie->m_iScreenWidth == 0 || xpgMovie->m_iScreenHeight == 0)
            return;
        
        float xRatio = (float)width / xpgMovie->m_iScreenWidth;
        float yRatio = (float)height / xpgMovie->m_iScreenHeight;

        wRole->ResizeRoleRegion(Sender, width, height, xRatio, yRatio);

        xpgMovie->m_Resize(width, height, xRatio, yRatio);
        wStage->SetMovieRegion(Sender, width, height);

        AnsiString PageSize = "Page Size: " + String(width) + "x" + String(height);
        wStage->StatusBar1->Panels->Items[0]->Text = PageSize;
    }
}
//---------------------------------------------------------------------------

void __fastcall TwMain::LoadMovieCommandSet(AnsiString FileName)
{
    wMovSet->LoadMovieCommandSet(FileName);
}
//-----------------------------------------------------------------------------
void __fastcall TwMain::LoadMovieCmdSetClick(TObject *Sender)
{
    wMovSet->LoadMovieCmdSetClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TwMain::SpriteList1Click(TObject *Sender)
{
    wSprite->BringToFront();
    wSprite->Show();
    wSprite->WindowState = wsNormal;
}

//---------------------------------------------------------------------------

void __fastcall TwMain::About1Click(TObject *Sender)
{
    dlgAbout->ShowModal();    
}
//---------------------------------------------------------------------------

void __fastcall TwMain::ClearPage1Click(TObject *Sender)
{
    wStage->ClearPage(Sender);
}

//---------------------------------------------------------------------------

void __fastcall TwMain::TrackBar1Change(TObject *Sender)
{
    Label1->Caption = AnsiString(TrackBar1->Position) + "%";
}
//---------------------------------------------------------------------------

void __fastcall TwMain::SetFile1Click(TObject *Sender)
{
    wMovSet->BringToFront();
    wMovSet->Show();
    wMovSet->WindowState = wsNormal;
}
//---------------------------------------------------------------------------

void __fastcall TwMain::Import1Click(TObject *Sender)
{
    wRole->bnImportImageClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TwMain::Export1Click(TObject *Sender)
{
    wRole->bnExportImageClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TwMain::DeleteSelected1Click(TObject *Sender)
{
    wRole->bnDeleteClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TwMain::DeleteUnused1Click(TObject *Sender)
{
    wRole->bnDeleteUnusedClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TwMain::FormDestroy(TObject *Sender)
{
    boExit = true;    
}

//---------------------------------------------------------------------------


void __fastcall TwMain::DeletAllPages1Click(TObject *Sender)
{
    wPage->DeleteAllPages(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TwMain::ExportAllImages1Click(TObject *Sender)
{
    AnsiString sPath = ExtractFilePath(stCurXpgFile);
    AnsiString NewPathName = ChangeFileExt(stCurXpgFile, ".Images");

    if (!DirectoryExists(NewPathName))
    {
      if (!CreateDir(NewPathName))
        throw Exception("Cannot create " + NewPathName +  " directory.");
    }

    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;    // Show hourglass cursor

    AnsiString FileName;
    for (int i = 0; i < xpgMovie->m_iRoleCount; i++) {
        FileName.sprintf("\\%04d_", i);
        if (false == wRole->ExportImage(Sender, i, NewPathName + FileName))
			return;
    }
    Screen->Cursor = Save_Cursor;
    ShowMessage("Export Images complete to " + NewPathName + ".");
}
//---------------------------------------------------------------------------

void __fastcall TwMain::ImportAllImages1Click(TObject *Sender)
{
    TOpenDialog *o = new TOpenDialog( this );
    o->FileName = "*.jpg";
    o->DefaultExt = "jpg";
    o->Filter = "Sequentialize File (*.jpg)|*.jpg | Sequentialize File (*.*)|*.*";
    o->Options << ofAllowMultiSelect << ofEnableSizing;

    if ( o->Execute() )
    {
        TCursor Save_Cursor = Screen->Cursor;
        Screen->Cursor = crHourGlass;    // Show hourglass cursor
        
        for (int i = 0; i < o->Files->Count; i++)
        {
            AnsiString FileName = o->Files->Strings[i];
            int iRole = 0;

            FileName = ExtractFileName(FileName);
            sscanf(FileName.c_str(), "%04d", &iRole);

            bool boAppend = (iRole >= xpgMovie->m_iRoleCount);
            wRole->ImportImage(Sender, iRole, o->Files->Strings[i], boAppend);

            RoleImages *pCurRole;
            pCurRole = (RoleImages *)(xpgMovie->m_RoleList->Items[iRole]);

            pCurRole->m_FileName = FileName.SubString(5, FileName.Length() - 1);
        }
        
        wMain->boModified = true;
        Screen->Cursor = Save_Cursor;
    }
    delete o;
}
//---------------------------------------------------------------------------

