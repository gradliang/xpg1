//---------------------------------------------------------------------------

#ifndef xpgSpriteH
#define xpgSpriteH

#include <vcl.h>
//---------------------------------------------------------------------------

class XpgMovies;
class Pages;
class RoleImages;
//---------------------------------------------------------------------------
class Sprites
{
public:
    XpgMovies *m_pMovie;
    Pages  *m_pPage;

    long  m_lHashKey;
    long  m_lFilePos;
    long  m_iIndex;
	long  m_iRole;
    RoleImages *m_pRole;

    bool m_boExist;
	bool m_boChanged;
    bool m_boVisible;
    bool m_boSelected;
    char m_cFlag;
    long m_iType;
    long m_iTypeIndex;
    
    // position and region
    long  m_iPx, m_iPy;
    long  m_iLeft, m_iRight, m_iTop, m_iBottom;
    long  m_iWidth, m_iHeight;

    long  m_iBlend;
    long  m_iInk;
    long  m_iInkValue;

    AnsiString m_Name;

    unsigned m_touchEnable;
    unsigned m_touchFlag;
public:
    Sprites();
    ~Sprites();

    void m_SetRole(RoleImages *pRole);
    void m_MoveTo(int dstX, int dstY);
    void m_Move(int x, int y);
    bool m_Inside(int x, int y);
    void m_Resize(float xRatio, float yRatio);
    void m_Clear();

   /*	void init();
	void reset();
    virtual void setPath(int path_no, int delaytime, int pic_x, int pic_y);
	void setAllData(unsigned int spNo, unsigned char buf[]);
	void move(Fixed offx, Fixed offy);
	void moveTo(Fixed dstx, Fixed dsty);
	void movex(Fixed dstx);
	void movexTo(Fixed dstx);
	void moveyTo(Fixed dsty);
	virtual void setRole(int n);
	void move();
	void draw(bool boAddToCanvas = false);
	void checkChanged();
	void drawInClip();
	int inside(int x, int y);
	int touch(Sprites *sp);
	int touch(int lt, int tp, int rt, int bm);
	void copySprite(Sprites *src);
	void drawText(int x, int y, int color, char str[], int len, char flag);
	void clearRoleImage();
	void hide();
	void show();
	void addToCanvasClip();	
	void saveRect();
	virtual void runPath();
	void addSaveRectToCanvasClip();
	void caculateBound();
	void run_music(signed char *data);
	XGPIXEL getPixel(int x, int y);     */
};


#endif
