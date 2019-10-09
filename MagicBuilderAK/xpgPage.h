//---------------------------------------------------------------------------

#ifndef xpgPageH
#define xpgPageH

#include <vcl.h>
//------------------------------------------------------------------------------
#define XPG_COMMAND_COUNT 256
class RoleImages;
class Sprites;
class Pages;
class XpgMovies;

class Commands {	
public:	
	bool m_boEmpty;
	int m_iIndex;  		
	int m_iButtonKey;
	int m_iEventKey;
	int m_iPageIndex;
	int m_iActionKey;
	int m_iParamKey;
	AnsiString m_Button;
	AnsiString m_Event;
	AnsiString m_Page;
	AnsiString m_Action;
	AnsiString m_Param;
public:
    Commands() { 
		m_boEmpty = true;
    	m_Button = "";
		m_Event = "";
		m_Page = "";
		m_Action = "";	
		m_iButtonKey = 0;
		m_iEventKey = 0;
		m_iPageIndex = 0;
		m_iActionKey = 0;
	}
    ~Commands()
    {
    	m_Button = NULL;
		m_Event = NULL;
		m_Page = NULL;
		m_Action = NULL;
    }

	bool CheckEmpty();
	void SetButton(int iKey);
	void SetEvent(int iKey);
	void SetPage(int iKey);
	void SetAction(int iKey);
	void SetParam(int iKey);
	void SetButton(AnsiString str);
	void SetEvent(AnsiString str);
	void SetPage(AnsiString str);
	void SetAction(AnsiString str);
	void SetParam(AnsiString str);
	
	AnsiString GetButtonName();
	AnsiString GetEventName();
	AnsiString GetActionName();
	AnsiString GetPageName();
	AnsiString GetParamName();
};


class Pages {
public:
    XpgMovies *m_pMovie;

    long  m_lHashKey;
    long  m_lFilePos;
    long  m_lSpriteFilePos;
    long  m_iIndex;       // page index
    long  m_lScriptFilePos;

    long  m_iSpriteCount;  // channel object number
	long  m_iCommandCount;
	
    long  m_iTimeLength;
    bool  m_boGoNext;
    bool  m_boLoaded;

    AnsiString m_Name;
	Commands m_Command[XPG_COMMAND_COUNT];
	
    //AnsiString m_ButtonAction[XPG_COMMAND_COUNT];
    //AnsiString m_ButtonPage[XPG_COMMAND_COUNT];

    Sprites *m_pCurSprite;
    TList   *m_SpriteList;
	//TList   *m_CommandList;

public:
    Pages();
    ~Pages();
    void m_Clear();
    Sprites *m_AddNewSprite(RoleImages *pRole, int x, int y);
    Sprites *m_AddCopySprite(Sprites *pSrcSprite);
    Sprites *m_GetSprite(int iSprite);
    void m_DrawSprites(Byte *lpCanvasDib);
    void m_DrawSprites();
    void m_ResetSpriteIndex();
	int m_GetSpriteCount();

	int GetCommandCount();	
	bool CheckCommandEmpty(int i);
	
	void SetCommandButton(int i, int iKey);
	void SetCommandEvent(int i, int iKey);
	void SetCommandPage(int i, int iKey);
	void SetCommandAction(int i, int iKey);
	void SetCommandParam(int i, int iKey);
	
	void SetCommandButton(int i, AnsiString str);
	void SetCommandEvent(int i, AnsiString str);
	void SetCommandPage(int i, AnsiString str);
	void SetCommandAction(int i, AnsiString str);
	void SetCommandParam(int i, AnsiString str);
	
	void SetCommand(int iCommand, int iField, AnsiString str);
	void SetCommand(int iCommand, int iField, int iKey);
	
	AnsiString GetCommandButton(int i);
	AnsiString GetCommandEvent(int i);
	AnsiString GetCommandPage(int i);
	AnsiString GetCommandAction(int i);
	AnsiString GetCommandParam(int i);
	
	void CopyPage(Pages *pSrcPage);

    //void m_LoadPage();
};

//---------------------------------------------------------------------------
#endif
