//---------------------------------------------------------------------------

#ifndef xpgMovieH
#define xpgMovieH

#include <stdio.h>

#define EDITOR
enum ImageFileType { IF_JPEG, IF_TIFF, IF_PNG, IF_CEL, IF_TGA, IF_RGB, IF_BMP,
                     IF_GIF, IF_PCX, IF_NONE };

typedef int Fixed;

class RoleImages;
class Sprites;
class Pages;

class ObjNames {
public:
	int m_iIndex;
	int m_iKey;
	AnsiString m_Name;
public:
    ObjNames(int i, int k, AnsiString name)
	{
		m_iIndex = i;
		m_iKey = k;
		m_Name = name;
    }

	~ObjNames()
	{
		m_Name = NULL;
	}
};

class ObjNameLists {
public:
	int Count;
	TList *m_ObjList;
	TStringList *m_NameList;
public:
	ObjNameLists()
	{
		m_ObjList = new TList;
		m_NameList = new TStringList;
		Count = 0;
	}

	~ObjNameLists()
	{
		Clear();
		delete m_ObjList;
		delete m_NameList;
	}

	int GetCount()
	{
	
		return Count;
	}
	
    void Add(int i, int k, AnsiString name)
	{
		ObjNames *pObj = new ObjNames(i, k, name);
		m_ObjList->Add(pObj);
		//m_NameList->Add(String(k) + " " + name);
        m_NameList->Add(name);
		Count++;
    }

	void Clear() {
		for (int i = 0; i < m_ObjList->Count; i++)
	    {
	        ObjNames *pObj = (ObjNames *)m_ObjList->Items[i];
			delete pObj;
	    }

		m_ObjList->Clear();
		m_NameList->Clear();
		Count = 0;
	}
	
	int GetKey(int index) {
		if (index < m_ObjList->Count)
	    {
            ObjNames *pObj = (ObjNames *)m_ObjList->Items[index];
			return pObj->m_iKey;
	    }
		return -1;
	}

	int GetKey(AnsiString name) {
		for (int i = 0; i < m_ObjList->Count; i++)
	    {
	        ObjNames *pObj = (ObjNames *)m_ObjList->Items[i];
			if (pObj->m_Name == name) {
				return pObj->m_iKey;
			}
	    }
		return -1;
	}

	int GetIndex(int key) {
		for (int i = 0; i < m_ObjList->Count; i++)
	    {
	        ObjNames *pObj = (ObjNames *)m_ObjList->Items[i];
			if (pObj->m_iKey == key) {
				return i;
			}
	    }
		return -1;
	}

	int GetIndex(AnsiString name) {
		for (int i = 0; i < m_ObjList->Count; i++)
	    {
	        ObjNames *pObj = (ObjNames *)m_ObjList->Items[i];
			if (pObj->m_Name == name) {
				return i;
			}
	    }
		return -1;
	}

	AnsiString GetNameByKey(int key) {
		for (int i = 0; i < m_ObjList->Count; i++)
	    {
	        ObjNames *pObj = (ObjNames *)m_ObjList->Items[i];
			if (pObj->m_iKey == key) {
				return pObj->m_Name;
			}
	    }
        return "";
	}

	AnsiString GetName(int index) {		
		if (index >= 0 && index < m_ObjList->Count)
	    {
	        ObjNames *pObj = (ObjNames *)m_ObjList->Items[index];
			return pObj->m_Name;
	    }
		return "";
	}
	
};



#if 0
class CommandLists {
	TList *m_ObjList;

public:
	CommandLists()
	{
		m_ObjList = new TList;
	}

	~CommandLists()
	{
		Clear();
		delete m_ObjList;
	}
    void Add(int b, int p, int a)
	{
		Commands *pObj = new Commands(b, p, a);
		keyMap[k] = i;
		indexMap[i] = k;
		m_ObjList->Add(pObj);
		m_NameList->Add(String(k) + " " + name);
    }

	void Clear() {
		for (int i = 0; i < m_ObjList->Count; i++)
	    {
	        Commands *pObj = (Commands *)m_ObjList->Items[i];
			delete pObj;
	    }

		m_ObjList->Clear();
	}
		
};

#endif

//------------------------------------------------------------------------------
class XpgMovies {
public:
	long  m_iVersion;
    long  m_iScreenWidth;      //Screen Width, 40,2
    long  m_iScreenHeight;     //Screen Height, 42,2
    long  m_iHeaderInfo;
    long  m_iBkgColor;         //Background Color , 44,6
    long  m_iTransColor;       //Transparent Color, 50,6
    long  m_iMaxSprites;       //Maximun Sprite Layer, 56,2
    long  m_iSpriteDataLen;    //Sprite Data Length, 58,2
    char  m_cReserve[16];      //Reserve, 60,16
    long  m_iRoleCount;        //Image Number, 76,2
    long  m_iImageHeaderLen;   //Image Header Length, 78,2
    long  m_lImageHeaderPos;   //Image Header  Position, 80,4
    long  m_iPageCount;        //Page Number, 84,2
    long  m_iPageHeaderLen;    //Page Header Length, 86,2
    long  m_lPageHeaderPos;    //Page Header Position, 88,4
    long  m_iScriptCount;      //Script Number, 92,2
    long  m_iScriptHeaderLen;  //Script Header Length, 94,2
    long  m_lScriptHeaderPos;  //Script Header Position, 96,4
    int   m_iQuality;
    bool  m_boXPG;
    bool  m_boXPW;
    bool  m_boVersion4;
    long  m_iNewScreenWidth;      //Screen Width, 40,2
    long  m_iNewScreenHeight;     //Screen Height, 42,2
    long  m_iSettingCount;

    TList *m_PageList;
    TList *m_RoleList;

	ObjNameLists m_SpriteTypeList;
    ObjNameLists m_EventList;
	ObjNameLists m_ButtonList;
	ObjNameLists m_ActionList;
    ObjNameLists m_SettingList;
	ObjNameLists m_ColorList;
	ObjNameLists m_InkList;

	//TList *m_CommandList;

    Pages *m_pCurPage;
    Sprites *m_pCurSprite;
    RoleImages *m_pCurRole;

    TCanvas *m_pCanvas;
    Graphics::TBitmap *m_pBitmap;

private:
    void m_InitHeader( bool boReset );
    bool m_WriteHeader( FILE *fp );
    bool m_WriteRoleHeader( FILE *fp, RoleImages *curRole );
    long m_WriteJPEG( FILE *fp, Graphics::TBitmap *Bitmap, int quality );
    long m_WriteRLE( FILE *fp, Graphics::TBitmap *Bitmap );
	long m_WriteOSD( FILE *fp, Graphics::TBitmap *Bitmap );
    long m_Write1BitsBitmap( FILE *fp, Graphics::TBitmap *Bitmap );
	long m_Write4BitsBitmap( FILE *fp, Graphics::TBitmap *Bitamp );
	long m_WriteIndexColorImage( FILE *fp, Graphics::TBitmap *Bitamp, int iColorCount );
    long m_WriteRgbIndexColorImage( FILE *fp, Graphics::TBitmap *Bitamp, int iColorCount );
    bool m_WriteSpriteData( FILE *fp, Sprites *pSprite );
    bool m_WritePageHeader( FILE *fp, Pages *pPage );

    bool m_ReadHeader( FILE *fp );
    bool m_ReadRoleHeader( FILE *fp, RoleImages *curRole );
	long XpgMovies::m_WriteRoleData(FILE *fp, RoleImages *pCurRole);
    HBITMAP m_ReadJPEG( FILE *fp, long lDataLen  );
    HBITMAP m_ReadRLE( FILE *fp, long lDataLen  );
    bool m_ReadSpriteData( FILE *fp, Sprites *pSprite );
    bool m_ReadPageHeader( FILE *fp, Pages *pPage );

public:
    XpgMovies();
    ~XpgMovies();
    bool m_SaveFile( const char *filename, int quality );
    bool m_WriteNewFormatXPGFile( const char *filename);             // grad add
    bool m_LoadFile( const char *filename );
    void m_Clear();
	void m_DeleteAllPages();
    int m_AddRole( Graphics::TBitmap *Bitmap, int iWd, int iHt );
    void m_ReplaceRole( int iRole, Graphics::TBitmap *Bitmap, int iWd, int iHt );

    bool m_DeleteRole(int iRole);
    bool m_CheckRoleUsed(int iRole);

    int m_AddPage();
    int m_DuplicatePage(int iPage);

    int m_InsertDuplicate(int iSrcPage, int iDstPage);
    int m_MovePage(int iSrcPage, int iDstPage);
    
    bool m_InsertPage(int iPage);
    bool m_DeletePage(int iPage);
	bool m_ReplacePage(int iPage, Pages*pPage);
    void m_Draw();
    void m_Resize(int width, int height, float xRatio, float yRatio);

    RoleImages *m_GetRole(int i);
    Pages *m_GetPage(int i);
    Pages *m_GetCurPage();

	int m_GetPageKey(AnsiString Name);
	int m_GetButtonKey(AnsiString Name);
	int m_GetActionKey(AnsiString Name);
	int m_GetEventKey(AnsiString Name);
	int m_GetParamKey(AnsiString Name);
	
	AnsiString m_GetPageName(int i);
	AnsiString m_GetEventName(int i);
	AnsiString m_GetButtonName(int i);
	AnsiString m_GetActionName(int i);
	AnsiString m_GetParamName(int i);
	
	AnsiString m_GetButtonNameByKey(int i);
	AnsiString m_GetActionNameByKey(int i);
	
};


//---------------------------------------------------------------------------
#endif
