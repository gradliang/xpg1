//---------------------------------------------------------------------------

#include <vcl.h>
#include <IniFiles.hpp>
#include <process.h>
#pragma hdrstop

#include "fmMain.h"
#include "fmStage.h"
#include "fmCommand.h"
#include "fmMovSet.h"

#include "xpg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TwMovSet *wMovSet;
AnsiString gCommandSetFileName = "";

//---------------------------------------------------------------------------
__fastcall TwMovSet::TwMovSet(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TwMovSet::FormCreate(TObject *Sender)
{
    Width = 360;
    Height = 360;
    Memo1->Align = alClient;
}
//---------------------------------------------------------------------------
void __fastcall TwMovSet::LoadMovieCmdSetClick(TObject *Sender)
{
    OpenDialog1->DefaultExt = "set";
    OpenDialog1->Filter = "Movie Command set file (*.set)|*.set";

    if ( OpenDialog1->Execute() )
    {
        LoadMovieCommandSet(OpenDialog1->FileName);
    }
}
//---------------------------------------------------------------------------

void __fastcall TwMovSet::UpdateSettingClick(TObject *Sender)
{
    wMovSet->Memo1->Lines->SaveToFile(gCommandSetFileName);
    LoadMovieCommandSet(gCommandSetFileName);
}
//---------------------------------------------------------------------------

void __fastcall TwMovSet::SaveMovieCommandSet(AnsiString FileName)
{
    gCommandSetFileName = FileName;
    wMovSet->Memo1->Lines->SaveToFile(FileName);
}

//---------------------------------------------------------------------------

void __fastcall TwMovSet::LoadMovieCommandSet(AnsiString FileName)
{

    wMovSet->Caption = "Set-" + FileName;
    
    TIniFile *ini;
    ini = new TIniFile( FileName );
	
    if (ini == NULL || !ini->SectionExists("BUTTON")
		|| !ini->SectionExists("ACTION")
		|| !ini->SectionExists("SPRITETYPE")) 
	{
        LoadMovieCmdSetClick(NULL);
        return;
    }

	gCommandSetFileName = FileName;

    ini->DeleteKey("BUTTON", "count");
    ini->DeleteKey("ACTION", "count");
    ini->DeleteKey("SPRITETYPE", "count");
                                    
    TStringList *TempList = new TStringList;
    ini->ReadSection("BUTTON", TempList);
    ini->ReadSection("ACTION", TempList);
    ini->ReadSection("SPRITETYPE", TempList);

    // read Button List
	xpgMovie->m_ButtonList.Clear();

	AnsiString CurSection = "BUTTON";
    ini->ReadSection(CurSection,TempList);
    int n = TempList->Count;
	xpgMovie->m_ButtonList.Add(0, 0, "NULL");
    for (int i = 0; i < n; i++)
    {
        AnsiString strKey = TempList->Strings[i];
		AnsiString strName = ini->ReadString( CurSection, strKey, strKey );
        int pos = strKey.Pos("button");
        int iKey = StrToInt(strKey.SubString(pos + 6, 3));

		xpgMovie->m_ButtonList.Add(i, iKey, strName);
    }
#if 0	
    // read Event List
    xpgMovie->m_EventList.Clear();
    CurSection = "EVENT";
    ini->ReadSection(CurSection,TempList);
    n = TempList->Count;
	if (n == 0) {
		ini->WriteString( "EVENT", "event0", "KeyDown" );
		ini->WriteString( "EVENT", "event1", "KeyUp" );
		ini->WriteString( "EVENT", "event2", "KeyRepeat" );
		ini->WriteString( "EVENT", "event3", "KeyHold1" );
		ini->WriteString( "EVENT", "event4", "KeyHold2" );
		ini->WriteString( "EVENT", "event5", "KeyHold3" );
		ini->WriteString( "EVENT", "event6", "EnterPage" );
		ini->WriteString( "EVENT", "event7", "ExitPage" );	
	}
    ini->ReadSection(CurSection,TempList);
    n = TempList->Count;
	
    for (int i = 0; i < n; i++)
    {
        AnsiString strKey = TempList->Strings[i];
		AnsiString strName = ini->ReadString( CurSection, strKey, strKey );
 
        int pos = strKey.Pos("event");
        int iKey = StrToInt(strKey.SubString(pos + 5, 3));

		xpgMovie->m_EventList.Add(i, iKey, strName);
    }
#endif
    // read Action List
    xpgMovie->m_ActionList.Clear();
    CurSection = "ACTION";
    ini->ReadSection(CurSection,TempList);
    n = TempList->Count;
    for (int i = 0; i < n; i++)
    {
        AnsiString strKey = TempList->Strings[i];
		AnsiString strName = ini->ReadString( CurSection, strKey, strKey );
 
        int pos = strKey.Pos("act");
        int iKey = StrToInt(strKey.SubString(pos + 3, 3));

		xpgMovie->m_ActionList.Add(i, iKey, strName);
    }

    // read SpriteType List
    CurSection = "SPRITETYPE";
	xpgMovie->m_SpriteTypeList.Clear();
	xpgMovie->m_SpriteTypeList.Add(0, 0, "No Type");
		
    ini->ReadSection(CurSection,TempList);
    n = TempList->Count;
    for( int i = 0; i<n; i++)
    {
        AnsiString strKey = TempList->Strings[i];
		AnsiString strName = ini->ReadString( CurSection, strKey, strKey );
        int pos = strKey.Pos("type");
        int iKey = StrToInt(strKey.SubString(pos + 4, 3));

		xpgMovie->m_SpriteTypeList.Add(i, iKey, strName);
    }

    wCommand->SetButtonList(xpgMovie->m_ButtonList.m_NameList);
	wCommand->SetEventList(xpgMovie->m_EventList.m_NameList);
    wCommand->SetActionList(xpgMovie->m_ActionList.m_NameList);

    wStage->ValueListEditor1->ItemProps[1]->PickList = 
			xpgMovie->m_SpriteTypeList.m_NameList;

	delete TempList;
    delete ini;

    try
    {            // test for C++ exceptions
          wMovSet->Memo1->Lines->LoadFromFile(FileName);
    }
    catch ( ... )
    {
       return;
    }        
}

//---------------------------------------------------------------------------

void __fastcall TwMovSet::bnSaveAsClick(TObject *Sender)
{
    SaveDialog1->DefaultExt = "set";
    SaveDialog1->Filter = "Movie Command set file (*.set)|*.set";
    SaveDialog1->InitialDir = OpenDialog1->InitialDir;
    
    if ( SaveDialog1->Execute() )
    {
        SaveMovieCommandSet(SaveDialog1->FileName);
    }
}
//---------------------------------------------------------------------------

