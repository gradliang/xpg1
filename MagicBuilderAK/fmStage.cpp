//---------------------------------------------------------------------------

#include <vcl.h>
#include <mmsystem.h>
#pragma hdrstop

#include "fmStage.h"
#include "fmMain.h"
#include "fmPage.h"
#include "fmRole.h"
#include "fmSprite.h"
#include "fmCommand.h"
#include "xpg.h"
#include "xpgDIB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TwStage *wStage;

int stageLeft = 0;
int stageTop = 0;
//flag used to allow modification to Picklist ItemProp[1] only when required
bool boPickList = false;
long TimeMouseDown;
Byte *lpCanvasDib;

//---------------------------------------------------------------------------
__fastcall TwStage::TwStage(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TwStage::FormCreate(TObject *Sender)
{
    boSpriteDragging = false;
    Left = 0;
    Top = 0;
    Width = wMain->ClientWidth/2;
    Height = wMain->ClientHeight * 0.7;

    BufferImage->Width = 2000;
    BufferImage->Height = 1200;
    xpgMovie->m_pCanvas = BufferImage->Canvas;
    xpgMovie->m_pBitmap = BufferImage->Picture->Bitmap;

    StageImage->Left = 0;
    StageImage->Top = 0;
    StageLeft = StageImage->Left;
    StageTop = StageImage->Top;

    //StagePanel->Width = BufferImage->Width + 12;
    //StagePanel->Height = BufferImage->Height + 12;
    boScaleFit = true;
    iStageScale = 100;
    dStageScale = 1;

    //Config Property box
    ValueListEditor1->ItemProps[3]->ReadOnly = true;
    ValueListEditor1->ItemProps[6]->ReadOnly = true;
    ValueListEditor1->ItemProps[7]->ReadOnly = true;
    //ValueListEditor1->ItemProps[8]->ReadOnly = true;  //flag

    ValueListEditor1->ItemProps[0]->EditStyle = esEllipsis;
    ValueListEditor1->ItemProps[1]->EditStyle = esPickList;
    ValueListEditor1->ItemProps[2]->EditStyle = esEllipsis;
    ValueListEditor1->ItemProps[4]->EditStyle = esEllipsis;
    ValueListEditor1->ItemProps[5]->EditStyle = esEllipsis;

    lpCanvasDib = xpgInitCanvas(BufferImage);
}

//---------------------------------------------------------------------------

void __fastcall TwStage::FormClick(TObject *Sender)
{
    UnSelectAllSprite(Sender, iEditPage);
    ShowPage(Sender, iEditPage);
}

//---------------------------------------------------------------------------

void __fastcall TwStage::WorkAreaClick(TObject *Sender)
{
    UnSelectAllSprite(Sender, iEditPage);
    ShowPage(Sender, iEditPage);
}

//---------------------------------------------------------------------------
void __fastcall TwStage::ClearAndReset(TObject *Sender)
{
    ClearStageCanvas(Sender);
    xpgMovie->m_pCanvas = BufferImage->Canvas;
    ShowPage(Sender, 0);
    Caption = "Stage";
    StatusBar1->Panels->Items[1]->Text = "Sprite Count: 0";
}
//---------------------------------------------------------------------------
void __fastcall TwStage::ClearBufferCanvas(TObject *Sender)
{

    TCanvas *pCanvas = BufferImage->Canvas;
	
    pCanvas->Brush->Color = clWhite;
    pCanvas->Brush->Style = bsSolid;
    pCanvas->FillRect(pCanvas->ClipRect);

}
//---------------------------------------------------------------------------
void __fastcall TwStage::ClearStageCanvas(TObject *Sender)
{
    ClearBufferCanvas(Sender);

    StageImage->Canvas->Draw(0, 0, BufferImage->Picture->Bitmap);
}
//---------------------------------------------------------------------------
void __fastcall TwStage::ClearPage(TObject *Sender)
{
    Pages *pPage = xpgMovie->m_GetPage(iEditPage);

    pPage->m_Clear();

    ShowPage(Sender, iEditPage);
    wPage->UpdatePage(Sender, iEditPage);
    pEditSprite = NULL; //(Sprites *)pPage->m_SpriteList->Items[i - 1];

    wMain->boModified = true;
    //----------------------------------------------
    //InitPageInfo(iEditPage);
    UpdateStageInfo(Sender);
}

//---------------------------------------------------------------------------


void __fastcall TwStage::FormResize(TObject *Sender)
{               
    if (boScaleFit) {
        StagePanel->ScaleBy(100, iStageScale);
        WorkArea->Refresh();

        Double dw = (Double)(WorkArea->Width - 8.) / StagePanel->Width;
        Double dh = (Double)(WorkArea->Height - 8.) / StagePanel->Height;

        dStageScale = (dw > dh) ? dh : dw;
        int iNewScale = (int)(dStageScale * 100);
        cmbScale->Text = String(iNewScale) + "%";
        StageImage->Stretch = true;
        StagePanel->ScaleBy(iNewScale, 100);
        iStageScale = iNewScale;
        dStageScale = (Double)iStageScale / 100.;
    }
}
//---------------------------------------------------------------------------

void __fastcall TwStage::cmbScaleChange(TObject *Sender)
{
    WorkArea->HorzScrollBar->Position = 0;
    WorkArea->VertScrollBar->Position = 0;
    AnsiString str = cmbScale->Text;

    boScaleFit = (cmbScale->Text == "Fit Stage");
    int n = str.Length();

    if (str[n] == '%')
        str = str.SubString(0, n - 1);

    int iNewScale = 0;
    bool boInt = TryStrToInt(str, iNewScale);

    if (!boInt) { 
        if (boScaleFit) {
          FormResize(Sender);
          return;
        }
    }

    if (iNewScale > 0) {
        StageImage->Stretch = true;
        StagePanel->ScaleBy(iNewScale, iStageScale);

        iStageScale = iNewScale;
        dStageScale = (Double)iNewScale / 100;
    }     
}

//---------------------------------------------------------------------------
void __fastcall TwStage::SetMovieRegion(TObject *Sender, int iWidth, int iHeight)
{
    StagePanel->ScaleBy(100, iStageScale);

    StageImage->Width = iWidth;
    StageImage->Height = iHeight;

    BufferImage->Picture->Bitmap->Width = iWidth;
    BufferImage->Picture->Bitmap->Height = iHeight;

    BufferImage->Width = iWidth;
    BufferImage->Height = iHeight;

    StagePanel->Width = iWidth + 2;
    StagePanel->Height = iHeight + 2;
    StageImage->Picture->Bitmap->Width = iWidth;            // will cause stretch abnormal
    StageImage->Picture->Bitmap->Height = iHeight;	

    iStageScale = 100;
    dStageScale = 1;

    FormResize(Sender);
}


//---------------------------------------------------------------------------
void __fastcall TwStage::UpdateSpriteFrames(TObject *Sender, int iPage)
{
    if (xpgMovie == NULL) return;
	if (pEditSprite == NULL) return;
	    
    Pages *pPage = xpgMovie->m_GetPage(iPage);
    if (pPage == NULL) return;
    if (pPage->m_SpriteList->Count == 0) return;	

	TCanvas *pCanvas = xpgMovie->m_pCanvas;
    TPoint points[4];
    //TRect SpriteRect;
    pCanvas->Pen->Color = clWhite;
    pCanvas->Pen->Width = 2;
    pCanvas->Pen->Mode = pmXor;
    pCanvas->Pen->Style = psDot;
    
    pCanvas->Brush->Color = clWhite;

	bool boNeedRefresh = false;
    for (int i = 0; i < pPage->m_SpriteList->Count; i++)
    {
        Sprites *pSprite = (Sprites *)pPage->m_SpriteList->Items[i];
        if (pSprite->m_boSelected) {
            points[0] = Point(pSprite->m_iLeft, pSprite->m_iTop);
            points[1] = Point(pSprite->m_iRight, pSprite->m_iTop);
            points[2] = Point(pSprite->m_iRight, pSprite->m_iBottom);
            points[3] = Point(pSprite->m_iLeft, pSprite->m_iBottom);
            points[4] = Point(pSprite->m_iLeft, pSprite->m_iTop);
            pCanvas->Polyline(points, 5);
			boNeedRefresh = true;
        }
    }
	if (boNeedRefresh)
	    pCanvas->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TwStage::UpdateStage(TObject *Sender)
{
    ShowPage(Sender, iEditPage);
}
//---------------------------------------------------------------------------
void __fastcall TwStage::SetRepaintPage(TObject *Sender, int iPage)
{
    boNeedRepaint = true;
}
//---------------------------------------------------------------------------
int iShowPageCount = 0;
void __fastcall TwStage::ShowPage(TObject *Sender, int iPage)
{
    static BOOL boUpdating;
    if (boUpdating) return;
    boUpdating = true;
    boNeedRepaint = false;
	iShowPageCount++;
    Label1->Caption = String(iShowPageCount);
    int iDibWidth = xpgMovie->m_iScreenWidth;
    int iDibHeight = xpgMovie->m_iScreenHeight;
    if (BufferImage->Width != iDibWidth || BufferImage->Height != iDibHeight)
	{
		BufferImage->Width = iDibWidth;
    	BufferImage->Height = iDibHeight;
    }
	
    ClearBufferCanvas(Sender);

    if (xpgMovie->m_iPageCount > 0) {  
        Pages *pPage = xpgMovie->m_GetPage(iPage);
        if (pPage == NULL) return;
        if (pPage->m_SpriteList->Count > 0) {
            pPage->m_DrawSprites();
            //pPage->m_DrawSprites(lpCanvasDib);
        }
    }
    if (iPage == iEditPage)
        UpdateSpriteFrames(Sender, iPage);

//    StageImage->Picture->Bitmap->Width = iDibWidth;            // will cause stretch abnormal
//    StageImage->Picture->Bitmap->Height = iDibHeight;

#if 1
    TCanvas *pCanvas = StageImage->Picture->Bitmap->Canvas;
    pCanvas->Lock();
    pCanvas->Draw(0, 0, BufferImage->Picture->Bitmap);
    pCanvas->Unlock();
    StageImage->Refresh();
#else
    TCanvas *pCanvas = StageImage->Picture->Bitmap->Canvas;
    pCanvas->Lock();
    xpgShowCanvas(pCanvas, lpCanvasDib);
    pCanvas->Unlock();
    StageImage->Refresh();
 #endif
    if (!boSpriteDragging) {
        wStage->Refresh();

        //UpdateSpriteToolbar(Sender);
        wSprite->SelectPage(Sender, iPage);
    }

    boUpdating = false;
}

//---------------------------------------------------------------------------

void __fastcall TwStage::SelectPage(TObject *Sender, int iPage)
{
    if (xpgMovie->m_iPageCount <= 0) return;

    if (iPage >= xpgMovie->m_iPageCount)
        iPage = xpgMovie->m_iPageCount - 1;

    Pages *pPage = xpgMovie->m_GetPage(iPage);
    xpgMovie->m_pCurPage = pPage;
    UnSelectAllSprite(Sender, iPage);
    ShowPage( Sender, iPage );

    WorkArea->Refresh();

    iEditPage = iPage;

    wCommand->SelectPage(Sender, iPage);
    //-------------------------------------
	//InitPageInfo(iPage);    
	UpdateComboSpriteList(Sender);
	UpdateStageInfo(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TwStage::UpdateComboSpriteList(TObject *Sender)
{
    Pages *pPage = xpgMovie->m_GetCurPage();
    // setting cbSprite List
    TStringList *StringList = new TStringList();
    for (int iSprite = 0; iSprite < pPage->m_SpriteList->Count; iSprite++) {
        Sprites *pSprite = pPage->m_GetSprite(iSprite);
        int iRole = (pSprite->m_pRole != NULL) ? pSprite->m_pRole->m_iIndex : pSprite->m_iRole;
        AnsiString str = String(iSprite) + "-" + "Image" + String(iRole);
        StringList->Add(str);
    }
	
    cbSprite->Items->Assign(StringList);
	delete StringList;
	
    if (pEditSprite == NULL)
        cbSprite->ItemIndex = 0;
    else
        cbSprite->ItemIndex = pEditSprite->m_iIndex;
    
}

//---------------------------------------------------------------------------
void __fastcall TwStage::UpdateSpriteToolbar(TObject *Sender)
{

    SendToBack1->Enabled = false;
    SendBackward1->Enabled = false;
    BringToFront1->Enabled = false;
    BringForward1->Enabled = false;
    bnSendToBack->Enabled = false;
    bnSendBackward->Enabled = false;
    bnBringToFront->Enabled = false;
    bnBringForward->Enabled = false;

    if (pEditSprite == NULL) return;

    int iSprite = pEditSprite->m_iIndex;
    int iSpriteCount = xpgMovie->m_GetCurPage()->m_SpriteList->Count;

    if (iSprite < 0 || iSprite >= iSpriteCount)
        return;

    if (iSprite < iSpriteCount - 1) {
        BringToFront1->Enabled = true;
        BringForward1->Enabled = true;
        bnBringToFront->Enabled = true;
        bnBringForward->Enabled = true;
    }
    if (iSprite > 0) {
        SendToBack1->Enabled = true;
        SendBackward1->Enabled = true;
        bnSendToBack->Enabled = true;
        bnSendBackward->Enabled = true;
    }
}

//---------------------------------------------------------------------------

void __fastcall TwStage::UpdateSpriteValueEditor(TObject *Sender)
{
	boPickList = false;

	Sprites *pSprite = pEditSprite;
	
	if (pSprite == NULL) {
        //cbSprite->Text = "";
        cbSprite->ItemIndex = 0;
	
	    for(int i = 1; i < ValueListEditor1->RowCount; i++)
	        ValueListEditor1->Cells[1][i] = "";
		
	} else {		
	    AnsiString strType = xpgMovie->m_SpriteTypeList.GetNameByKey(pSprite->m_iType);
		
	    ValueListEditor1->Cells[1][1] = pSprite->m_Name;
	    ValueListEditor1->Cells[1][2] = strType;
	    ValueListEditor1->Cells[1][3] = String(pSprite->m_iTypeIndex);
	    ValueListEditor1->Cells[1][4] = String(pSprite->m_iIndex);
	    ValueListEditor1->Cells[1][5] = String(pSprite->m_iLeft);
	    ValueListEditor1->Cells[1][6] = String(pSprite->m_iTop);
	    ValueListEditor1->Cells[1][7] = String(pSprite->m_iWidth);
	    ValueListEditor1->Cells[1][8] = String(pSprite->m_iHeight);
	    ValueListEditor1->Cells[1][9] = String(pSprite->m_flag);
        ValueListEditor1->Cells[1][10] = String((int)(pSprite->m_touchEnable));
        ValueListEditor1->Cells[1][11] = String((int)(pSprite->m_touchFlag));

        cbSprite->ItemIndex = pSprite->m_iIndex;
	}
	
	boPickList = true;	
}

//---------------------------------------------------------------------------

void __fastcall TwStage::UpdateStatusBar(TObject *Sender)
{

	if (xpgMovie == NULL || xpgMovie->m_pCurPage == NULL || pEditSprite == NULL) {
	    StatusBar1->Panels->Items[2]->Text = "Select:[NULL]";
	    StatusBar1->Panels->Items[3]->Text = "";
	    StatusBar1->Panels->Items[4]->Text = "";
	    StatusBar1->Panels->Items[5]->Text = "Tag: ";
	} else {
		//Show Sprite Count
	    AnsiString SpriteCount = "SpriteCount:" + String(xpgMovie->m_pCurPage->m_iSpriteCount);
	    StatusBar1->Panels->Items[1]->Text = SpriteCount;      

	    //---------------------------------------
	    AnsiString strInfo = "Select: [";
	    strInfo += String(pEditSprite->m_iIndex) + "]";
	    StatusBar1->Panels->Items[2]->Text = strInfo;
	    StatusBar1->Panels->Items[3]->Text = "X: " + String(pEditSprite->m_iLeft);
	    StatusBar1->Panels->Items[4]->Text = "Y: " + String(pEditSprite->m_iTop);

	    StatusBar1->Panels->Items[5]->Text = "Tag: " + pEditSprite->m_Name;
	}

}
//---------------------------------------------------------------------------

void __fastcall TwStage::UpdateStageInfo(TObject *Sender)
{

	UpdateStatusBar(Sender);
    UpdateSpriteToolbar(Sender);
	UpdateSpriteValueEditor(Sender);

}

//---------------------------------------------------------------------------
void __fastcall TwStage::UpdateEditSprite(TObject *Sender)
{
    static boUpdating;
    if (pEditSprite == NULL) {
        return;
    } else {

        if (boUpdating) return;
        boUpdating = true;
        boStageChanged = true;
        wMain->boModified = true;
        ShowPage( Sender, iEditPage );      
        
        if (!boSpriteDragging) {
            wPage->UpdatePage(Sender, iEditPage);
        }
        //------------------------------
		AnsiString strLeft = AnsiString(pEditSprite->m_iLeft);
		if (ValueListEditor1->Cells[1][5] != strLeft) {
			ValueListEditor1->Cells[1][5] = strLeft;
			StatusBar1->Panels->Items[3]->Text = "X: " + strLeft;
		}
		
		AnsiString strTop = AnsiString(pEditSprite->m_iTop);
		if (ValueListEditor1->Cells[1][6] != strTop) {
			ValueListEditor1->Cells[1][6] = strTop;
			StatusBar1->Panels->Items[4]->Text = "Y: " + strTop;
		}
        boUpdating = false;
    }
}


//---------------------------------------------------------------------------

void __fastcall TwStage::UnselectSprite(TObject *Sender)
{
	if (pEditSprite == NULL) return;
	
	pEditSprite->m_boSelected = true;
    pEditSprite = NULL;

	UpdateStageInfo(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TwStage::UnSelectAllSprite(TObject *Sender, int iPage)
{
    if (xpgMovie == NULL) return;
    Pages *pPage = xpgMovie->m_GetPage(iPage);
    if (pPage == NULL) return;
    if (pPage->m_SpriteList->Count == 0) return;

    for (int i = 0; i < pPage->m_SpriteList->Count; i++)
    {
        Sprites *pSprite = (Sprites *)pPage->m_SpriteList->Items[i];
        pSprite->m_boSelected = false;
    }

	pEditSprite = NULL;
	UpdateStageInfo(Sender);

    wSprite->SelectSprite(NULL, -1);
}

//---------------------------------------------------------------------------
void __fastcall TwStage::SelectSprite(TObject *Sender, int iSprite)
{
    Pages *pPage = xpgMovie->m_pCurPage;

    if (pPage == NULL || iSprite < 0 || iSprite >= pPage->m_iSpriteCount)
    {
        UnselectSprite(Sender);
        return;
    }

    Sprites *pSprite = (Sprites *)pPage->m_SpriteList->Items[iSprite];
	if (pSprite == NULL) {
		UnselectSprite(Sender);
		return;
	}
    pEditSprite = pSprite;
    pSprite->m_boSelected = true;

    UpdateStageInfo(Sender);

}

//---------------------------------------------------------------------------
void __fastcall TwStage::MoveSelectedSprite(TObject *Sender, int dx, int dy)
{
    if (xpgMovie == NULL) return;
    Pages *pPage = xpgMovie->m_GetPage(iEditPage);
    if (pPage == NULL) return;
    if (pPage->m_SpriteList->Count == 0) return;

    for (int i = 0; i < pPage->m_SpriteList->Count; i++)
    {
        Sprites *pSprite = (Sprites *)pPage->m_SpriteList->Items[i];
        if (pSprite->m_boSelected == false) continue;
        pSprite->m_Move(dx, dy);
    }
}
//---------------------------------------------------------------------------
void __fastcall TwStage::SetSelectedSpritePos(TObject *Sender, int x, int y)
{
    if (xpgMovie == NULL) return;
    Pages *pPage = xpgMovie->m_GetPage(iEditPage);
    if (pPage == NULL) return;
    if (pPage->m_SpriteList->Count == 0) return;

    for (int i = 0; i < pPage->m_SpriteList->Count; i++)
    {
        Sprites *pSprite = (Sprites *)pPage->m_SpriteList->Items[i];
        if (pSprite->m_boSelected == false) continue;
        pSprite->m_MoveTo(x, y);
    }
}

//---------------------------------------------------------------------------

void __fastcall TwStage::BringToFront1Click(TObject *Sender)
{
    Pages *pPage = xpgMovie->m_GetPage(iEditPage);

    int i = pEditSprite->m_iIndex;
    int iLast = pPage->m_SpriteList->Count - 1;
    if (i < iLast) {
        pPage->m_SpriteList->Move(i, iLast);
        pPage->m_ResetSpriteIndex();
        ShowPage(Sender, iEditPage);
        wPage->UpdatePage(Sender, iEditPage);
		UpdateComboSpriteList(Sender);

        SelectSprite(NULL, iLast);
        wSprite->SelectSprite(NULL, iLast);
        wMain->boModified = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TwStage::SendToBack1Click(TObject *Sender)
{
    int i = pEditSprite->m_iIndex;
    if (i > 0) {
        Pages *pPage = xpgMovie->m_GetPage(iEditPage);
        pPage->m_SpriteList->Move(i, 0);
        pPage->m_ResetSpriteIndex();

        ShowPage(Sender, iEditPage);
        wPage->UpdatePage(Sender, iEditPage);
		UpdateComboSpriteList(Sender);
        SelectSprite(NULL, 0);
        wSprite->SelectSprite(NULL, 0);
        wMain->boModified = true;
    }
}

//---------------------------------------------------------------------------

void __fastcall TwStage::BringForward1Click(TObject *Sender)
{
    Pages *pPage = xpgMovie->m_GetPage(iEditPage);

    int i = pEditSprite->m_iIndex;
    int iLast = pPage->m_SpriteList->Count - 1;
    if (i < iLast) {
        pPage->m_SpriteList->Move(i, i+1);
        pPage->m_ResetSpriteIndex();
        ShowPage(Sender, iEditPage);
        wPage->UpdatePage(Sender, iEditPage);
		UpdateComboSpriteList(Sender);
        SelectSprite(NULL, i + 1);
        wSprite->SelectSprite(NULL, i + 1);
        wMain->boModified = true;
    }    
}
//---------------------------------------------------------------------------

void __fastcall TwStage::SendBackward1Click(TObject *Sender)
{
    int i = pEditSprite->m_iIndex;
    if (i > 0) {
        Pages *pPage = xpgMovie->m_GetPage(iEditPage);
        pPage->m_SpriteList->Move(i, i - 1);
        pPage->m_ResetSpriteIndex();

        ShowPage(Sender, iEditPage);
        wPage->UpdatePage(Sender, iEditPage);
        UpdateComboSpriteList(Sender);
        SelectSprite(NULL, i - 1);
        wSprite->SelectSprite(NULL, i - 1);
        wMain->boModified = true;
    }  
}

//---------------------------------------------------------------------------

void __fastcall TwStage::bnSendToBackClick(TObject *Sender)
{
    SendToBack1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TwStage::bnSendBackwardClick(TObject *Sender)
{
    SendBackward1Click(Sender);    
}
//---------------------------------------------------------------------------

void __fastcall TwStage::bnBringForwardClick(TObject *Sender)
{
    BringForward1Click(Sender);    
}
//---------------------------------------------------------------------------

void __fastcall TwStage::bnBringToFrontClick(TObject *Sender)
{
    BringToFront1Click(Sender);    
}

//---------------------------------------------------------------------------
void __fastcall TwStage::Delete1Click(TObject *Sender)
{
    if (pEditSprite == NULL) return;

    int i = pEditSprite->m_iIndex;
    if (i >= 0) {
        Pages *pPage = xpgMovie->m_GetPage(iEditPage);
        pEditSprite->m_Clear();
        delete pEditSprite;
        pEditSprite = NULL;

        pPage->m_SpriteList->Delete(i);
        pPage->m_ResetSpriteIndex();
        pPage->m_iSpriteCount--;
        ShowPage(Sender, iEditPage);
        wPage->UpdatePage(Sender, iEditPage);

        wMain->boModified = true;
    }
}

//---------------------------------------------------------------------------

void __fastcall TwStage::StageImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (boSpriteDragging) return;
    StagePanel->SetFocus();
    int xMouse = X;
    int yMouse = Y;
    TimeMouseDown = timeGetTime();

    boShiftDown = Shift.Contains(ssShift);
    if (!boShiftDown)
        UnSelectAllSprite(Sender, iEditPage);
	//else
	//	UnselectSprite(Sender);
    wStage->ActiveControl = NULL;
    wStage->BringToFront();
    wStage->SetFocus();

    UpdateStage(NULL);
    //if (Button == mbLeft)
    {
        X /= dStageScale;
        Y /= dStageScale;
    }
    //int x = StageImage->Left;
    //int y = StageImage->Top;
    //---------------------------------------
    //addded to update PageNameList after "NewMovie" is clicked
    Pages *pPage;
    if(xpgMovie->m_iPageCount ==0)
    {
        pPage = xpgMovie->m_GetCurPage();    //will create a new page if no page exits
        wMain->UpdatePageNameList();
    }
    else
    {
        pPage = xpgMovie->m_GetCurPage();
    }

    //----------------------------------------
    TList *pSpriteList = pPage->m_SpriteList;
    int iCount = pSpriteList->Count;
    for (int i = iCount - 1; i >= 0; i--)
    {
        Sprites *pSprite = (Sprites *)pSpriteList->Items[i];
        if (pSprite->m_pRole == NULL) continue;
        //Reset SelFrame after image replacement
        pSprite->m_iWidth = pSprite->m_pRole->m_iWidth;
        pSprite->m_iHeight = pSprite->m_pRole->m_iHeight;
        pSprite->m_Move(0,0);

        if (pSprite->m_Inside(X, Y))
        {
        	if (pEditSprite == pSprite) 
				UnselectSprite(Sender);
			
            SelectSprite(Sender, i);
            wSprite->SelectSprite(NULL, i);

            X -= pSprite->m_iLeft;
            Y -= pSprite->m_iTop;

            if (Button == mbLeft) {
                boSpriteDragging = true;
                iDragX = X * dStageScale;
                iDragY = Y * dStageScale;
            } else if (Button == mbRight) {
                int x = xMouse + StagePanel->ClientOrigin.x;
                int y = yMouse + StagePanel->ClientOrigin.y;

                PopupMenu1->Popup(x, y);
            }

            return;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TwStage::StageImageMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    boSpriteDragging = false;
    UpdateEditSprite(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TwStage::StageImageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

    long TimeSpan = timeGetTime() - TimeMouseDown;
    //debouncing to avoid unwanted movement on clicking of the sprite
    //700 msec delay
    if(TimeSpan > 200)
    {
        if (pEditSprite != NULL && boSpriteDragging) {
		if (dStageScale == 0)
            dStageScale = 1;	
            int dx = (X - iDragX) / dStageScale;
            int dy = (Y - iDragY) / dStageScale;
            if (dx < 0) dx = 0;
            if (dy < 0) dy = 0;
			if (dx != pEditSprite->m_iLeft || dy != pEditSprite->m_iTop) { 
            	pEditSprite->m_MoveTo(dx, dy);
            	UpdateEditSprite(Sender);
			}
        }
    }   
}

//---------------------------------------------------------------------------

void __fastcall TwStage::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    boShiftDown = Shift.Contains(ssShift);

    if (!wStage->Focused()) return;
    bool boMoved = false;
    if(pEditSprite!=NULL)
    {
        //Allow the position of the selected sprite to be moved
        //only when Shift is held down
        //if(Shift.Contains(ssShift))
        {
            //ValueListEditor1->Enabled = false;
            switch (Key) {
                case VK_LEFT :
                    MoveSelectedSprite(Sender, -1, 0);
                    //pEditSprite->m_Move(-1, 0);
                    boMoved = true;
                    break;
                case VK_RIGHT :
                    MoveSelectedSprite(Sender, 1, 0);
                    boMoved = true;
                    break;
                case VK_UP :
                    MoveSelectedSprite(Sender, 0, -1);
                    boMoved = true;
                    break;
                case VK_DOWN :
                    MoveSelectedSprite(Sender, 0, 1);
                    boMoved = true;
                    break;
            }
        }

        //else
        {
            //Use Page Up & Down to switch among sprites
            Pages * pPage = xpgMovie->m_GetCurPage();
            int iSpriteCount = pPage->m_iSpriteCount;
            int iSpriteIndex = pEditSprite->m_iIndex;
            switch(Key) {
                case VK_NEXT :
                    if(iSpriteIndex == (iSpriteCount -1))
                        SelectSprite(Sender, 0);
                    else
                        SelectSprite(Sender, iSpriteIndex+1);
                    break;
                case VK_PRIOR :
                    if(iSpriteIndex == 0)
                        SelectSprite(Sender, iSpriteCount -1);
                    else
                        SelectSprite(Sender, iSpriteIndex -1);
                    break;
            }
        }
    }
    if (boMoved) {
        UpdateEditSprite(Sender);
        //wPage->UpdatePage(Sender, iEditPage);
    }
    //ValueListEditor1->Enabled = true;
}

//---------------------------------------------------------------------------

void __fastcall TwStage::edPageNameChange(TObject *Sender)
{
    if (xpgMovie->m_pCurPage != NULL) {
        Pages *pPage = xpgMovie->m_pCurPage;
        int iPage = pPage->m_iIndex;

        pPage->m_Name = wCommand->edPageName->Text;
        wPage->ListView1->Items->Item[iPage]->Caption =
            IntToStr(iPage) + " " + pPage->m_Name;
    }
}
//---------------------------------------------------------------------------

void __fastcall TwStage::StringGrid1KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (xpgMovie->m_pCurPage != NULL) {
        Pages *pPage = xpgMovie->m_pCurPage;
        int i = wCommand->StringGrid1->Row;
        pPage->SetCommandAction(i, wCommand->StringGrid1->Cells[1][i]);
    }
}

//---------------------------------------------------------------------------

void __fastcall TwStage::ValueListEditor1EditButtonClick(TObject *Sender)
{
    if(pEditSprite == NULL) return;

    int iRow = ValueListEditor1->Row;
    int iCol = ValueListEditor1->Col;
    if(iCol == 1)
    {
        if(iRow == 1) //Set Name
            pEditSprite->m_Name = ValueListEditor1->Cells[iCol][iRow];
        else if(iRow == 3)  //Set Type Index
        {
            int TypeIndex = 0;
            int boInt = TryStrToInt(ValueListEditor1->Cells[iCol][iRow],TypeIndex);
            if(!boInt)
            {
                ShowMessage("Please enter a valid Integer.");
            } else
                pEditSprite->m_iTypeIndex = TypeIndex;
            UpdateEditSprite(Sender);
        }

        else if(iRow == 5) //Set X
        {
            int iLeft = 0;
            int boInt = TryStrToInt(ValueListEditor1->Cells[iCol][iRow],iLeft);
            if(!boInt)
            {
                ShowMessage("Please enter a valid Ingeter.");
                return;
            }
            pEditSprite->m_MoveTo(iLeft,pEditSprite->m_iTop);
            UpdateEditSprite(Sender);
        }

        else if(iRow == 6) //Set Y
        {
            int iTop = 0;
            int boInt = TryStrToInt(ValueListEditor1->Cells[iCol][iRow],iTop);
            if(!boInt)
            {
                ShowMessage("Please enter a valid Ingeter.");
                return;
            }
            pEditSprite->m_MoveTo(pEditSprite->m_iLeft,iTop);
            UpdateEditSprite(Sender);
        }
        else if(iRow == 9) //Set flag
        {
            int iValue = 0;
            int boInt = TryStrToInt(ValueListEditor1->Cells[iCol][iRow], iValue);
            if(!boInt)
            {
                ShowMessage("Please enter a valid Ingeter.");
                return;
            }
            pEditSprite->m_flag = iValue;
            UpdateEditSprite(Sender);
        }
        else if(iRow == 10) //Set touch enable
        {
            int iValue = 0;
            int boInt = TryStrToInt(ValueListEditor1->Cells[iCol][iRow], iValue);
            if(!boInt)
            {
                ShowMessage("Please enter a valid Ingeter.");
                return;
            }
            pEditSprite->m_touchEnable = iValue;
            UpdateEditSprite(Sender);
        }
        else if(iRow == 11) //Set touch flag
        {
            int iValue = 0;
            int boInt = TryStrToInt(ValueListEditor1->Cells[iCol][iRow], iValue);
            if(!boInt)
            {
                ShowMessage("Please enter a valid Ingeter.");
                return;
            }
            pEditSprite->m_touchFlag = iValue;
            UpdateEditSprite(Sender);
        }
        wMain->boModified = true;
    }

}
//---------------------------------------------------------------------------

void __fastcall TwStage::ValueListEditor1SetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
    if (wMain->boExit) return;
    if (pEditSprite == NULL) return;
    if (Sender == NULL) return;
    if (boPickList == false) return;
    if (ACol == 0) return;

    if (ARow == 2) {
        int i = ValueListEditor1->ItemProps[1]->PickList->IndexOf(Value);
        pEditSprite->m_iType = xpgMovie->m_SpriteTypeList.GetKey(i);
        wMain->boModified = true;
        return;
    }
    int iRow = ARow;
    //int iCol = ACol;
    AnsiString StrValue = Value; //ValueListEditor1->Cells[iCol][iRow];

    if(iRow == 1) {//Set Name
        pEditSprite->m_Name = StrValue;
        wMain->boModified = true;
        return;
    }

    int iValue = 0;
    int boInt = TryStrToInt(StrValue, iValue);
    if (!boInt )return;
    //iValue = StrToIntDef(StrValue, 0);

    if(iRow == 3)  //Set Type Index
    {
        pEditSprite->m_iTypeIndex = iValue;
        UpdateEditSprite(Sender);
    }
    else if(iRow == 5) //Set X
    {
        pEditSprite->m_MoveTo(iValue, pEditSprite->m_iTop);
    }                      
    else if(iRow == 6) //Set Y
    {
        pEditSprite->m_MoveTo(pEditSprite->m_iLeft, iValue);
    }
    else if(iRow == 9) //Set enable
    {
        pEditSprite->m_flag = iValue;
        UpdateEditSprite(Sender);
    }
    else if(iRow == 10) //Set touch enable
    {
        pEditSprite->m_touchEnable = iValue;
        UpdateEditSprite(Sender);
    }
    else if(iRow == 11) //Set touch flag
    {
        pEditSprite->m_touchFlag = iValue;
        UpdateEditSprite(Sender);
    }
    wMain->boModified = true;
}

//---------------------------------------------------------------------------
void __fastcall TwStage::ValueListEditor1KeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if(Key == VK_RETURN)
        ValueListEditor1EditButtonClick(Sender);
}

//---------------------------------------------------------------------------

void __fastcall TwStage::ReplaceImage1Click(TObject *Sender)
{
    if(pEditSprite!= NULL)
    {
        if(xpgMovie->m_pCurRole!=NULL)
        {
            pEditSprite->m_SetRole(xpgMovie->m_pCurRole);
            ValueListEditor1->Cells[1][7] = String(pEditSprite->m_iWidth);
            ValueListEditor1->Cells[1][8] = String(pEditSprite->m_iHeight);
            UpdateEditSprite(Sender);

            wMain->boModified = true;
        }
        else
            ShowMessage("Please select an image from Media Library.");
    }
}

//---------------------------------------------------------------------------

void __fastcall TwStage::cbSpriteChange(TObject *Sender)
{
    if (Sender != NULL)
        SelectSprite( Sender, cbSprite->ItemIndex );
}
//---------------------------------------------------------------------------

void __fastcall TwStage::SelectImage1Click(TObject *Sender)
{
    StageImageDblClick(Sender);
}

//---------------------------------------------------------------------------

void __fastcall TwStage::ValueListEditor1KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        if(Key == VK_RETURN)
                ValueListEditor1EditButtonClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TwStage::StageImageDblClick(TObject *Sender)
{
    wRole->SelectRole( Sender, pEditSprite->m_pRole->m_iIndex );
    wRole->SetFocus();
    wRole->ListView1->Focused();
}
//--------------------------------------------------------------------------- 

void __fastcall TwStage::ToolPanelResize(TObject *Sender)
{
    ValueListEditor1->Width = ToolPanel->Width - 8;
    cbSprite->Width = ToolPanel->Width - 8;
}
//---------------------------------------------------------------------------

void __fastcall TwStage::Timer1Timer(TObject *Sender)
{
    if (boNeedRepaint)
       ShowPage(Sender, iEditPage);
}
//---------------------------------------------------------------------------

void __fastcall TwStage::WorkAreaDblClick(TObject *Sender)
{
    wSprite->BringToFront();
    wSprite->Show();
    wSprite->WindowState = wsNormal;        
}

//---------------------------------------------------------------------------

void __fastcall TwStage::StageImageDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
    //AnsiString Str = String(Source->ClassName());
    Accept = Source->ClassNameIs("TListView");
}

//---------------------------------------------------------------------------

void __fastcall TwStage::StageImageDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
    bool Accept = Source->ClassNameIs("TListView");
    if (!Accept) return;

    Pages *pPage = xpgMovie->m_GetCurPage();
    int iPage = pPage->m_iIndex;

    int i = wRole->ListView1->Selected->Index;
    RoleImages *pRole = xpgMovie->m_GetRole(i);

    int x = (X - 0) / dStageScale;
    int y = (Y - 0) / dStageScale;
    pPage->m_AddNewSprite(pRole, x, y);

    wStage->ShowPage(Sender, iPage);
    wPage->UpdatePage(Sender, iPage);    
}

//---------------------------------------------------------------------------


