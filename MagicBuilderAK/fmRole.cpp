//---------------------------------------------------------------------------

#include <vcl.h>
#include <Clipbrd.hpp>

#include "fmRole.h"
#include "fmMain.h"
#include "fmStage.h"
#include "fmPage.h"

#include "BMGDLL.h"
#include "xpg.h"                
#include "dgQuality.h"
#include "dgTransColor.h"

#pragma hdrstop



//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TwRole *wRole;
extern int g_iJpegQuality;

int g_iCurRoleIndex = 0;
AnsiString g_stCaptionHead = "Image - ";
bool boUpdatePage;

//---------------------------------------------------------------------------
__fastcall TwRole::TwRole(TComponent* Owner)
    : TForm(Owner)
{ 
}
//---------------------------------------------------------------------------

void __fastcall TwRole::FormCreate(TObject *Sender)
{
    Caption = g_stCaptionHead + "0";
    Width = wMain->ClientWidth/2 - 8;
    Height = wMain->ClientHeight * 0.7;
    Left = wMain->ClientWidth - Width - 4;
    Top = 0;

    Shape1->Top = RoleImage->Top - 1;
    Shape1->Left = RoleImage->Left - 1;
    Shape2->Top = RoleImage->Top - 1;
    Shape2->Left = RoleImage->Left - 1;
    SetRoleRegion(NULL, xpgMovie->m_iScreenWidth, xpgMovie->m_iScreenHeight);
}
//---------------------------------------------------------------------------
void __fastcall TwRole::ClearAndReset(TObject *Sender)
{
    IconImageList->Clear();
    ListView1->Clear();
    ShowRole(Sender, 0);
    edName->Text = "";
    Caption = "Image";
    for(int i=0; i< 4; i++)
    {
        StatusBar1->Panels->Items[i]->Text = "";
    }       
}

//---------------------------------------------------------------------------
void __fastcall TwRole::SelectRole(TObject *Sender, int iRole)
{
    if (iRole >= xpgMovie->m_iRoleCount || iRole >= ListView1->Items->Count) return;

    ListView1->ItemIndex = iRole;
    ListView1->Selected = ListView1->Items->Item[iRole];

    int y = ListView1->ViewOrigin.y;
	ListView1->Scroll(0, ListView1->Items->Item[iRole]->Top - y);
    //ShowRole( Sender, iRole );
}
//---------------------------------------------------------------------------
bool __fastcall TwRole::DrawRole(TObject *Sender, int iRole)
{
    if (iRole >= xpgMovie->m_iRoleCount) {
        RoleImage->Visible = false;
        return false;
    } else {
        RoleImages *pRole = (RoleImages *)(xpgMovie->m_RoleList->Items[iRole]);

        xpgMovie->m_pCurRole = pRole;
        Graphics::TBitmap *Bitmap = pRole->m_pBitmap;

        if (Bitmap == NULL) return false;

        //Bitmap = (Graphics::TBitmap *)RoleList->Items[iRole];
        TCanvas *pCanvas = RoleImage->Picture->Bitmap->Canvas;
        pCanvas->Lock();
        RoleImage->Picture->Bitmap->Width = pRole->m_iWidth;
        RoleImage->Picture->Bitmap->Height = pRole->m_iHeight;
        pCanvas->Draw(0, 0, pRole->m_pBitmap);
        pCanvas->Unlock();

        //RoleImage->Picture->Bitmap->Assign(Bitmap);
        //RoleList->GetBitmap(iRole, RoleImage->Picture->Bitmap);
        RoleImage->Refresh();
        RoleImage->Visible = true;
    }
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TwRole::ShowRole(TObject *Sender, int iRole)
{
    Caption = g_stCaptionHead + String(iRole);
    g_iCurRoleIndex = iRole;

    StatusBar1->Panels->Items[0]->Text = "- X -";

    if (iRole >= xpgMovie->m_iRoleCount) {
        RoleImage->Visible = false;
        return false;
    } else {
        RoleImages *pRole = (RoleImages *)(xpgMovie->m_RoleList->Items[iRole]);

        xpgMovie->m_pCurRole = pRole;
        Graphics::TBitmap *Bitmap = pRole->m_pBitmap;

        if (Bitmap == NULL) return false;

        //Bitmap = (Graphics::TBitmap *)RoleList->Items[iRole];
        TCanvas *pCanvas = RoleImage->Picture->Bitmap->Canvas;
        pCanvas->Lock();
        RoleImage->Picture->Bitmap->Width = pRole->m_iWidth;
        RoleImage->Picture->Bitmap->Height = pRole->m_iHeight;
        pCanvas->Draw(0, 0, pRole->m_pBitmap);
        pCanvas->Unlock();

        //RoleImage->Picture->Bitmap->Assign(Bitmap);
        //RoleList->GetBitmap(iRole, RoleImage->Picture->Bitmap);
        RoleImage->Refresh();
        RoleImage->Visible = true;

        TListItem *pItem;
        pItem = ListView1->Items->Item[iRole];
        if (pItem != NULL) {
            if (xpgMovie->m_CheckRoleUsed(iRole))
                StatusBar1->Panels->Items[0]->Text = "Used";

            StatusBar1->Panels->Items[1]->Text = "w: " + IntToStr(Bitmap->Width);
            StatusBar1->Panels->Items[2]->Text = "h: " + IntToStr(Bitmap->Height);
            StatusBar1->Panels->Items[3]->Text = pRole->m_Name;
            Caption = g_stCaptionHead + String(iRole) + " : " + pRole->m_Name;

            edName->Text = pRole->m_Name;

            boUpdatePage = false;
            cbRoleType->ItemIndex = pRole->m_iType;
			cbRoleQuality->ItemIndex = pRole->m_iQuality;
            if (pRole->m_iType == 0) {                
                cbRoleQuality->Enabled = true;
            } else {
                cbRoleQuality->Enabled = false;
            }
            //CheckBox4->Checked = pRole->m_boOSD;
            //CheckBox3->Checked = pRole->m_boSourceOutput;
            //CheckBox2->Checked = pRole->m_boTrans;
            boUpdatePage = true;
        }
        return true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TwRole::ResizeRole(TObject *Sender, int iRole, float xRatio, float yRatio)
{
    RoleImages *pRole = (RoleImages *)(xpgMovie->m_RoleList->Items[iRole]);

    ShowRole( ListView1, iRole );

    Graphics::TBitmap *Bitmap = pRole->m_pBitmap;

    int wd = (int)(pRole->m_iWidth * xRatio);
    wd &= 0xfffe; // force width to even
    int ht = (int)(pRole->m_iHeight * yRatio);

    if (Bitmap == NULL) return;
    pRole->m_pBitmap->Width = wd;
    pRole->m_pBitmap->Height = ht;

    TCanvas *pCanvas = pRole->m_pBitmap->Canvas;
    pCanvas->Lock();

    pCanvas->StretchDraw(TRect(0, 0, wd, ht), RoleImage->Picture->Bitmap);
    pCanvas->Unlock();
    pRole->m_iWidth = wd;
    pRole->m_iHeight = ht;
}
//---------------------------------------------------------------------------
void __fastcall TwRole::SetRoleRegion(TObject *Sender, int width, int height)
{
    Shape1->Width = width + 2;
    Shape1->Height = height + 2;
    Shape2->Width = width + 2;
    Shape2->Height = height + 2;
}
//---------------------------------------------------------------------------
void __fastcall TwRole::ResizeRoleRegion(TObject *Sender, int width, int height, float xRatio, float yRatio)
{
    SetRoleRegion(Sender, width, height);

    int iRoleCount = IconImageList->Count;

    for (int iRole = 0; iRole < iRoleCount; iRole++) {
        ResizeRole(Sender, iRole, xRatio, yRatio);
        ShowRole( ListView1, iRole );
    }
}
//---------------------------------------------------------------------------

void __fastcall TwRole::ListView1Resize(TObject *Sender)
{
    ListView1->UpdateItems(0, ListView1->Items->Count - 1);
}

//---------------------------------------------------------------------------
void * __fastcall TwRole::LoadImageSource( RoleImages *pCurRole, AnsiString FileName )
{
    FILE *fp;
    if ((fp = fopen(FileName.c_str(), "rb")) == NULL)
        return NULL;

    if (pCurRole->m_pImgSource != NULL && pCurRole->m_iImgSourceSize > 0) {
        delete (pCurRole->m_pImgSource);
        pCurRole->m_iImgSourceSize = 0;
    }

    fseek(fp, 0, SEEK_END);
    long lFileSize = ftell(fp);

    unsigned char *pszBuffer = NULL;
    pszBuffer = new unsigned char[lFileSize + 1];

    fseek(fp, 0, SEEK_SET);
    fread( pszBuffer, 1, lFileSize, fp );
    fclose(fp);

    pCurRole->m_pImgSource = pszBuffer;
    pCurRole->m_iImgSourceSize = lFileSize;

    return pszBuffer;
}

void __fastcall TwRole::ImportImage(TObject *Sender, int iRole,
                                    AnsiString FileName, bool boAppend)
{
    RoleImages *pCurRole;
	BMGError out;
    AnsiString SourceFileName;

    if (!boAppend)
        pCurRole = (RoleImages *)(xpgMovie->m_RoleList->Items[iRole]);


    SourceFileName = FileName;
    FileName = ExtractFileName(FileName);
    AnsiString extStr = ExtractFileExt(FileName);
    extStr = extStr.LowerCase();

    HBITMAP hBitmap = NULL;
	struct BMGImageStruct img;
	InitBMGImage( &img );

    if (extStr == ".jpg" || extStr == ".jpeg") {
        FileType = IF_JPEG;
		//hBitmap = CreateBitmapFromFile( FileName.c_str(), NULL, 0 );

		out = GetDataFromFile( FileName.c_str(), &img, NULL );
		/* create the bitmap from the image data */
	    if ( out == BMG_OK )
	        hBitmap = CreateBitmapFromData( img, 0 );

    } else if (extStr == ".png" || extStr == ".pcx") {
        //int AlphaCompSwitch = 0;
        FileType = IF_PNG;
        //hBitmap = CreateBitmapFromFile( FileName.c_str(), NULL,
        //    AlphaCompSwitch );
		out = GetDataFromFile( FileName.c_str(), &img, NULL );
		/* create the bitmap from the image data */
	    if ( out == BMG_OK )
	        hBitmap = CreateBitmapFromData( img, 0 );

    } else if (extStr == ".bmp") {
        RoleImage->Picture->LoadFromFile( FileName );
        out = GetDataFromBitmap(RoleImage->Picture->Bitmap->Handle, &img, 0);
        if (out == BMG_OK)
            hBitmap = CreateBitmapFromData( img, 0 );
    }
    FreeBMGImage(&img);
    if ( hBitmap )
    {
        int iWd = 16;
        int iHt = 16;
        Graphics::TBitmap *Bitmap;


        BITMAP bmp;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);
        iWd = bmp.bmWidth;
        iHt = bmp.bmHeight;


        if (boAppend)
            Bitmap = new Graphics::TBitmap;
        else
            Bitmap = pCurRole->m_pBitmap;

        Bitmap->Width = iWd;
        Bitmap->Height = iHt;

        //Bitmap = new Graphics::TBitmap;
        Bitmap->Handle = hBitmap;
        RoleImage->Picture->Bitmap->Assign(Bitmap);
        RoleImage->Refresh();

        // setting Icon
        IconImage->Width = 64;
        IconImage->Height = 64;
        IconImage->Canvas->StretchDraw(
            IconImage->ClientRect,
            RoleImage->Picture->Bitmap );
        IconImage->Refresh();

        if (boAppend)
            IconImageList->Add(IconImage->Picture->Bitmap, NULL);
        else
            IconImageList->Replace(iRole, IconImage->Picture->Bitmap, NULL);


        Bitmap->Width = iWd;
        Bitmap->Height = iHt;

        if (boAppend)
            iRole = xpgMovie->m_AddRole(Bitmap, iWd, iHt);
        else
            xpgMovie->m_ReplaceRole(iRole, Bitmap, iWd, iHt);

        pCurRole = (RoleImages *)(xpgMovie->m_RoleList->Items[iRole]);
        pCurRole->m_FileName = FileName;
        pCurRole->m_Name = ExtractFileName(FileName);

        LoadImageSource(pCurRole, SourceFileName);
        pCurRole->m_lDataLen = pCurRole->m_iImgSourceSize;

        TListItem *pItem;

        if (boAppend) {
            pItem = ListView1->Items->Add();
            if (pItem) {
                pItem->Caption = IntToStr(iRole) + "-" + FileName;
                pItem->ImageIndex = iRole;
                pItem->SubItems->Add(pCurRole->m_Name);
                pItem->SubItems->Add(IntToStr(iWd));
                pItem->SubItems->Add(IntToStr(iHt));
                pItem->SubItems->Add(FileName);
            }
        } else {
            pItem = ListView1->Items->Item[iRole];
            if (pItem) {
                pItem->Caption = IntToStr(iRole) + "-" + FileName;
                pItem->ImageIndex = iRole;

                //pItem->SubItems->Strings[0] = (pCurRole->m_Name);
                pItem->SubItems->Strings[0] = (IntToStr(iWd));
                pItem->SubItems->Strings[1] = (IntToStr(iHt));
                pItem->SubItems->Strings[2] = (FileName);
            }
        }
        ListView1->Update();
        ShowRole(Sender, iRole);

        //if ( Bitmap != NULL )
        //    delete Bitmap;
        Invalidate();
        wMain->boModified = true;
    }
    else
    {
        const char *msg;
        GetLastBMGErrorMessage( &msg );
        Application->MessageBox( msg, "BMG Error", MB_OK | MB_ICONERROR );
    }
}
//---------------------------------------------------------------------------
void __fastcall TwRole::bnImportImageClick(TObject *Sender)
{
    OpenDialog1->Options << ofAllowMultiSelect << ofEnableSizing;
    if ( OpenDialog1->Execute() )
    {
        for (int i = 0; i < OpenDialog1->Files->Count; i++)
        {
            ImportImage(Sender, i, OpenDialog1->Files->Strings[i], true);
        }
        wMain->boModified = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TwRole::ReplaceImage1Click(TObject *Sender)
{
    int iRole = wRole->ListView1->Selected->Index;
    RoleImages *pCurRole = (RoleImages *)(xpgMovie->m_GetRole(iRole));
    OpenDialog1->FileName = pCurRole->m_FileName;
    OpenDialog1->Options << ofEnableSizing;
    if ( OpenDialog1->Execute() )
    {
        ImportImage(Sender, iRole, OpenDialog1->FileName, false);
        wMain->boModified = true;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TwRole::ExportJpegCcode(TObject *Sender, AnsiString FileName)
{
    FILE *fpDst = NULL;
    FILE *fpSrc = NULL;

    if ((fpSrc = fopen(FileName.c_str(), "rb")) == NULL) return false;

    FileName = ChangeFileExt(FileName,".txt");
    if ((fpDst = fopen(FileName.c_str(), "w")) != NULL) {
        fseek(fpSrc, 0, SEEK_END);
        long lFileSize = ftell(fpSrc);
        long lDataLen = lFileSize;
        if (lDataLen % 4 != 0) {
            lDataLen += 4 - (lDataLen % 4);
        }

        fseek( fpSrc, 0, SEEK_SET );
        unsigned char *pszBuffer;
        pszBuffer = new unsigned char[lDataLen + 1];

        unsigned long *pulBuffer = (unsigned long *)pszBuffer;
        memset(pszBuffer, 0, lDataLen);
        fread( pulBuffer, 1, lFileSize, fpSrc );
        fclose(fpSrc);
        
        if (lDataLen <= 0) return -1;


        long lDwDataLen = lDataLen / 4;
        //*pulBuffer = lDataLen;

        fprintf(fpDst, "DWORD menu_camera[%d] = {\n",lDataLen);
        //fprintf(fpDst, "%d,\n", lDataLen);

        for (int n = 0, i = 0, j = 0; n < lDwDataLen; n++, i+=4, j++) {
            if (j == 5) {
                fputc('\n', fpDst);
                j = 0;
            }
#if 0
            if (n == lDwDataLen - 1)
                fprintf(fpDst, "0x%08x", pulBuffer[n]);
            else
                fprintf(fpDst, "0x%08x,", pulBuffer[n]);
#else
            if (n == lDwDataLen - 1)
                fprintf(fpDst, "0x%02x%02x%02x%02x",
                pszBuffer[i], pszBuffer[i+1], pszBuffer[i+2], pszBuffer[i+3]);
            else
                fprintf(fpDst, "0x%02x%02x%02x%02x,",
                pszBuffer[i], pszBuffer[i+1], pszBuffer[i+2], pszBuffer[i+3]);
#endif
        }

        delete pszBuffer;
        fprintf(fpDst, "\n};\n");
        fclose(fpDst);
    }
    fclose(fpSrc);
    return true;
}

//---------------------------------------------------------------------------

bool __fastcall TwRole::ExportImage(TObject *Sender, int iRole, AnsiString sPath)
{
    RoleImages *pRole = (RoleImages *)(xpgMovie->m_RoleList->Items[iRole]);
    if (pRole->m_pBitmap != NULL) {

        Graphics::TBitmap *Bitmap = pRole->m_pBitmap;

        if (Bitmap == NULL) return false;

        //Bitmap = (Graphics::TBitmap *)RoleList->Items[iRole];
        TCanvas *pCanvas = RoleImage->Picture->Bitmap->Canvas;
        pCanvas->Lock();
        RoleImage->Picture->Bitmap->Width = pRole->m_iWidth;
        RoleImage->Picture->Bitmap->Height = pRole->m_iHeight;
        pCanvas->Draw(0, 0, pRole->m_pBitmap);
        pCanvas->Unlock();

        RoleImage->Refresh();

        AnsiString FileName = sPath + pRole->m_FileName;

        int iJpegQuality = 100;
        if ( SaveBitmapToFile( RoleImage->Picture->Bitmap->Handle,
                               FileName.c_str(),
                               (void *)&iJpegQuality ) != BMG_OK )
        {
            const char *msg;
            GetLastBMGErrorMessage( &msg );
            Application->MessageBox( msg, "BMG Error", MB_OK | MB_ICONERROR );
			return false;
        }

		return true;
    }
	return false;
}

//---------------------------------------------------------------------------

void __fastcall TwRole::bnExportImageClick(TObject *Sender)
{
    TSaveDialog *o = new TSaveDialog( this );
    o->DefaultExt = "jpg";
    o->Filter = "JPEG Files (*.jpg)|*.jpg|";

    if ( o->Execute() )
    {
        //if (DlgQuality->ShowModal() == mrOk)
        {
            int iJpegQuality = 100;
            if ( SaveBitmapToFile( RoleImage->Picture->Bitmap->Handle,
                                   o->FileName.c_str(),
                                   (void *)&iJpegQuality ) != BMG_OK )
            {
                const char *msg;
                GetLastBMGErrorMessage( &msg );
                Application->MessageBox( msg, "BMG Error", MB_OK | MB_ICONERROR );
            }

            //ExportJpegCcode(Sender, o->FileName);
        }
    }

    delete o;
}

//---------------------------------------------------------------------------
void __fastcall TwRole::AddRole(TObject *Sender, int iRole)
{
    RoleImages *pCurRole = (RoleImages *)(xpgMovie->m_RoleList->Items[iRole]);
    if (pCurRole->m_pBitmap != NULL) {
        TCanvas *pCanvas = RoleImage->Picture->Bitmap->Canvas;
        pCanvas->Lock();
        RoleImage->Picture->Bitmap->Width = pCurRole->m_iWidth;
        RoleImage->Picture->Bitmap->Height = pCurRole->m_iHeight;
        pCanvas->Draw(0, 0, pCurRole->m_pBitmap);
        pCanvas->Unlock();

        //        RoleImage->Picture->Bitmap->Assign(pCurRole->m_pBitmap);
        RoleImage->Refresh();
    }

    IconImage->Width = 64;
    IconImage->Height = 64;    
    TCanvas *pCanvas = IconImage->Canvas;
    pCanvas->Brush->Color = clWhite;
    pCanvas->Brush->Style = bsSolid;
    pCanvas->FillRect(pCanvas->ClipRect);
    IconImage->Refresh();

    if (RoleImage->Width < IconImage->Width &&
        RoleImage->Height < IconImage->Height)
        pCanvas->Draw( 0, 0, RoleImage->Picture->Bitmap );
    else if (RoleImage->Width > IconImage->Width &&
        RoleImage->Height > IconImage->Height)
        pCanvas->StretchDraw(
            IconImage->ClientRect, RoleImage->Picture->Bitmap );
    else if (RoleImage->Width < IconImage->Width) {
        int iWidth = RoleImage->Width * IconImage->Height / RoleImage->Height;
        if (iWidth < 2) iWidth = 2;
        pCanvas->StretchDraw(
            TRect(0, 0, iWidth, IconImage->Height),
            RoleImage->Picture->Bitmap );
    }
    else  {
        int iHeight = RoleImage->Height * IconImage->Width / RoleImage->Width;
        if (iHeight < 2) iHeight = 2;
        pCanvas->StretchDraw(
            TRect(0, 0, IconImage->Width, iHeight),
            RoleImage->Picture->Bitmap );
    }

    IconImage->Refresh();
    IconImageList->Add(IconImage->Picture->Bitmap, NULL);

    TListItem *pItem = ListView1->Items->Add();
    if (pItem) {
        pItem->Caption = IntToStr(iRole) + "-" + pCurRole->m_FileName;
        pItem->ImageIndex = iRole;
        pItem->SubItems->Add(IntToStr(pCurRole->m_iWidth));
        pItem->SubItems->Add(IntToStr(pCurRole->m_iHeight));
        pItem->SubItems->Add(pCurRole->m_Name);
    }

    ShowRole(Sender, iRole);
}
//---------------------------------------------------------------------------

void __fastcall TwRole::bnPutOnStageClick(TObject *Sender)
{
    if (ListView1->Selected == NULL) {
        Application->MessageBox("Please select image first.", "Warning!", MB_OK);
        return;
    }

    Pages *pPage = xpgMovie->m_GetCurPage();
    int iPage = pPage->m_iIndex;

    int i = ListView1->Selected->Index;
    RoleImages *pRole = xpgMovie->m_GetRole(i);

    pPage->m_AddNewSprite(pRole, 0, 0);

    wStage->ShowPage(Sender, iPage);
    wPage->UpdatePage(Sender, iPage);
}

//---------------------------------------------------------------------------



void __fastcall TwRole::PutonStage1Click(TObject *Sender)
{
    bnPutOnStageClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TwRole::PutonNewpage1Click(TObject *Sender)
{
    int iPage = xpgMovie->m_AddPage();
    wStage->ShowPage(Sender, iPage);
    wPage->UpdatePage(Sender, iPage);
    wPage->SelectPage(Sender, iPage);
    wMain->UpdatePageNameList();
    wMain->boModified = true;

    bnPutOnStageClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TwRole::Delete1Click(TObject *Sender)
{
    bnDeleteClick(Sender);
}

//---------------------------------------------------------------------------

void __fastcall TwRole::edNameKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    xpgMovie->m_pCurRole->m_Name = edName->Text;
    TListItem *pItem = ListView1->Selected;
    //Modified by Johnny
    //----------------------------------------------------------
    pItem->Caption = IntToStr(pItem->Index) + "-" + edName->Text;
    //----------------------------------------------------------
}

//---------------------------------------------------------------------------


void __fastcall TwRole::ListView1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Key == VK_DELETE)
        Delete1Click(Sender);    
}
//---------------------------------------------------------------------------

void __fastcall TwRole::CheckBox2Click(TObject *Sender)
{
    if (xpgMovie == NULL || xpgMovie->m_pCurRole == NULL) return;

    xpgMovie->m_pCurRole->m_boTrans = CheckBox2->Checked;

    if(boUpdatePage == true)
    {
        wPage->UpdateAllPages(Sender);
    }
}
//---------------------------------------------------------------------------

void __fastcall TwRole::ToolButton3Click(TObject *Sender)
{
    //DlgTransColor->ShowModal();
    if (ColorDialog1->Execute()) {
        ShapeTransColor->Brush->Color = ColorDialog1->Color;

    }
}
//---------------------------------------------------------------------------
DWORD RGBPixel2YYCBCR(DWORD color1, DWORD color2)
{
	DWORD R1, G1, B1, R2, G2, B2;
    DWORD R, G, B;
	DWORD Y1, Y2, Cb, Cr;

	R1 = (color1 >> 0) & 0xff;
	G1 = (color1 >> 8) & 0xff;
	B1 = (color1 >> 16) & 0xff;	
	R2 = (color2 >> 0) & 0xff;
	G2 = (color2 >> 8) & 0xff;
	B2 = (color2 >> 16) & 0xff;

	R = (R1 + R2) >> 1;
	G = (G1 + G2) >> 1;
	B = (B1 + B2) >> 1;
	Cb = ((-151 * R) + (-297 * G) + (449 * B) + 131072) >> 10;
	Cr = (( 449 * R) + (-376 * G) + (-73 * B) + 131072) >> 10;
    Y1 = ((263 * R1) + (514 * G1) + (100 * B1)) >> 10;
    Y2 = ((263 * R2) + (514 * G2) + (100 * B2)) >> 10;

	return (Y1 << 24) | (Y2 << 16) | (Cb << 8) | Cr;
}
//---------------------------------------------------------------------------

void __fastcall TwRole::Shape2MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    if (X & 1) X--;

    TColor curColor = RoleImage->Canvas->Pixels[X][Y];

    DWORD yuv = RGBPixel2YYCBCR(curColor, RoleImage->Canvas->Pixels[X + 1][Y]);

    AnsiString str;
    str.sprintf("(%d,%d):%08X", X, Y, yuv);
    StatusBar1->Panels->Items[4]->Text = str;
}
//---------------------------------------------------------------------------

void __fastcall TwRole::bnDeleteClick(TObject *Sender)
{

    if (ListView1->Selected == NULL) {
        Application->MessageBox("Please select Image to Delete.", "Warning!", MB_OK);
        return;
    }

    int iRole = ListView1->Selected->Index;

    if (iRole >= 0) {
        if (xpgMovie->m_CheckRoleUsed(iRole)) {
            Application->MessageBox("This Image has beed used, can't be deleted.", "Error!", MB_OK);
            return;
        }
        if (xpgMovie->m_DeleteRole(iRole))
        {
            ListView1->DeleteSelected();
            ListView1->Refresh();
            //--------------------------------------------------------------
            IconImageList->Delete(iRole);     //Added by Johnny
            //--------------------------------------------------------------
            ShowRole( ListView1, iRole );
        }

        //Added by Johnny
        //Resort IconImageList and ListView1 items
        //----------------------------------------------------------------------
        TListItem *pItem;
        for (int i=0; i <= IconImageList->Count ;i++)
        {
            pItem = ListView1->Items->Item[i];
            if (pItem) {
                RoleImages *pCur = (RoleImages *)(xpgMovie->m_RoleList->Items[i]);
                pItem->Caption = IntToStr(i) + "-" + pCur->m_Name;
                pItem->ImageIndex = i;
            }
        }
        //----------------------------------------------------------------------


        wMain->boModified = true;
    } else {
        Application->MessageBox("Can't delete image.", "Error!", MB_OK);
    }
           
}
//---------------------------------------------------------------------------

void __fastcall TwRole::bnDeleteUnusedClick(TObject *Sender)
{
   int iRoleCount = IconImageList->Count;
    BOOL boDeleted = false;

    for (int iRole = iRoleCount - 1; iRole >= 0; iRole--) {
        if (xpgMovie->m_CheckRoleUsed(iRole))
            continue;

        if (xpgMovie->m_DeleteRole(iRole))
        {
            ListView1->Selected = ListView1->Items->Item[iRole];
            ListView1->DeleteSelected();
            ListView1->Refresh();
            //--------------------------------------------------------------
            IconImageList->Delete(iRole);     //Added by Johnny
            //--------------------------------------------------------------
            //ShowRole( ListView1, iRole );
        }
        boDeleted = true;
        wMain->boModified = true;
    }

    //Added by Johnny
    //Resort IconImageList and ListView1 items
    //----------------------------------------------------------------------
    TListItem *pItem;
    for (int i = 0; i <= IconImageList->Count; i++)
    {
        pItem = ListView1->Items->Item[i];
        if (pItem) {
            RoleImages *pCur = (RoleImages *)(xpgMovie->m_RoleList->Items[i]);
            pItem->Caption = IntToStr(i) + "-" + pCur->m_Name;
            pItem->ImageIndex = i;
        }
    }
    //----------------------------------------------------------------------

    ShowRole( ListView1, 0 );

    if (boDeleted)
        wMain->boModified = true;
}
//---------------------------------------------------------------------------
/****************************************************************************/
/*
// Creates a BITMAPINFOHEADER for the given width, height, bit count, and
// compression.  Compression must = BI_RGB, BI_BITFIELDS, BI_RLE4, or BI_RLE8.
*/
BITMAPINFO InternalCreateBMI( DWORD dwWidth,  /* width */
                              DWORD dwHeight, /* height */
                              WORD wBitCount, /* bit count */
                              int compression )  /* compression type */
{
   BITMAPINFO bi;         /* bitmap header */
   DWORD dwBytesPerLine;        /* Number of bytes per scanline */

   /* Make sure bits per pixel is valid */
   if (wBitCount <= 1)
      wBitCount = 1;
   else if (wBitCount <= 4)
      wBitCount = 4;
   else if (wBitCount <= 8)
      wBitCount = 8;
   else if (wBitCount <= 16)
      wBitCount = 16;
   else if (wBitCount <= 24)
      wBitCount = 24;
   else if (wBitCount <= 32)
      wBitCount = 32;
   else
      wBitCount = 8;  /* set default value to 8 if parameter is bogus */

   dwBytesPerLine =   (((wBitCount * dwWidth) + 31) / 32 * 4);

   /* initialize BITMAPINFO */
   bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bi.bmiHeader.biWidth = dwWidth;
   bi.bmiHeader.biHeight = dwHeight;
   bi.bmiHeader.biPlanes = 1;              /* must be 1 */
   bi.bmiHeader.biBitCount = wBitCount;
   bi.bmiHeader.biCompression =  compression;
   bi.bmiHeader.biSizeImage = dwBytesPerLine*dwHeight;
   bi.bmiHeader.biXPelsPerMeter = 0;
   bi.bmiHeader.biYPelsPerMeter = 0;
   bi.bmiHeader.biClrUsed = wBitCount <= 8 ? 1U << wBitCount : 0;
   bi.bmiHeader.biClrImportant = bi.bmiHeader.biClrUsed;

   return bi;
}
//---------------------------------------------------------------------------
int __fastcall TwRole::GetBitsFromBitmap( HBITMAP hBitmap,
					        Byte *lpBits,
                            int remove_alpha )
{
    unsigned int        DIBScanWidth;
    DIBSECTION          DS;
    HWND                hWnd = GetForegroundWindow();
    HDC                 hDC = NULL;
    //HDC                 hMemDC = NULL;
	unsigned int        numBytes;
	size_t              soDIBSECTION = sizeof(DIBSECTION);
	size_t              soBITMAP = sizeof(BITMAP);


    /* check for valid bitmap*/
    if ( !hBitmap )
        return 0; 

    /* Extract DIBSECTION info from the HBITMAP.  numBytes will equal
    // soDIBSECTION (84) if hBitmap is a handle to a DIBSECTION (DIB).
    // numBytes will equal soBITMAP (24) if hBitmap is a handle to a
    // BITMAP (DDB). */
	numBytes = GetObject( hBitmap, sizeof(DIBSECTION), &DS );
    if ( numBytes == 0 )
        return 0;

	if ( numBytes == soDIBSECTION )
	{
	    lpBits = (unsigned char *)DS.dsBm.bmBits;
	}
	/* this may be a DDB which must be handled differently */
	else if ( numBytes == soBITMAP )
	{
        BITMAP bm;
		BITMAPINFO bmi;

	    if ( GetObject( hBitmap, sizeof(BITMAP), &bm ) == 0 )
		    return 0;

		/* DDB with a palette */
		if ( bm.bmBitsPixel <= 8 ) {
            return 0;
        }
		bmi = InternalCreateBMI( bm.bmWidth, bm.bmHeight, bm.bmBitsPixel,
                                 BI_RGB );

		lpBits = (unsigned char *)calloc( bm.bmHeight * bm.bmWidthBytes,
                                          sizeof(unsigned char) );
		if ( lpBits == 0 )
			return 0;

        hDC = GetDC( hWnd );
		if ( GetDIBits(hDC, hBitmap, 0, bm.bmHeight, (void *)lpBits, &bmi,
                       DIB_RGB_COLORS ) == 0 )
	        return 0;
        ReleaseDC( hWnd, hDC );
		hDC = NULL;
	}
	else /* I have no idea what this is */
		return 0;


    return 1;
}
//---------------------------------------------------------------------------

void __fastcall TwRole::cbRoleTypeChange(TObject *Sender)
{
    if (xpgMovie == NULL) return;
    if (xpgMovie->m_pCurRole == NULL) return;
    xpgMovie->m_pCurRole->m_iType = cbRoleType->ItemIndex;
    cbRoleQuality->Enabled = (cbRoleType->ItemIndex == 0);
}
//---------------------------------------------------------------------------

void __fastcall TwRole::cbRoleQualityChange(TObject *Sender)
{
    if (xpgMovie == NULL) return;
    if (xpgMovie->m_pCurRole == NULL) return;
    xpgMovie->m_pCurRole->m_iQuality = cbRoleQuality->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TwRole::ListView1SelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
    if (Selected)
    {
        ShowRole( ListView1, ListView1->Selected->Index );
    }
}
//---------------------------------------------------------------------------

void __fastcall TwRole::Copy1Click(TObject *Sender)
{
    Clipboard()->Assign(RoleImage->Picture->Bitmap);
}
//---------------------------------------------------------------------------

void __fastcall TwRole::Paste1Click(TObject *Sender)
{
    int iRole = ListView1->Selected->Index;
    if (iRole >= xpgMovie->m_iRoleCount) return;

    RoleImages *pCurRole = (RoleImages *)(xpgMovie->m_RoleList->Items[iRole]);

    TClipboard *pCB = Clipboard();
    if (pCB->HasFormat(CF_BITMAP))
    {
        Graphics::TBitmap *Bitmap = pCurRole->m_pBitmap;
        try
        {
            Bitmap->LoadFromClipboardFormat(CF_BITMAP, pCB->GetAsHandle(CF_BITMAP), 0);

            int iWd = Bitmap->Width;
            int iHt = Bitmap->Height;
            //Image1->Refresh();
            //Image1->Canvas->Draw(0, 0, pBitmap);

            RoleImage->Picture->Bitmap->LoadFromClipboardFormat(CF_BITMAP, pCB->GetAsHandle(CF_BITMAP), 0);
            RoleImage->Width = iWd;
            RoleImage->Height = iHt;
            RoleImage->Refresh();

                    // setting Icon
        IconImage->Width = 64;
        IconImage->Height = 64;
        IconImage->Canvas->StretchDraw(
            IconImage->ClientRect,
            RoleImage->Picture->Bitmap );
        IconImage->Refresh();

        IconImageList->Replace(iRole, IconImage->Picture->Bitmap, NULL);


        //if (boAppend)
        //    iRole = xpgMovie->m_AddRole(Bitmap, iWd, iHt);
        //else
        xpgMovie->m_ReplaceRole(iRole, Bitmap, iWd, iHt);


        //pCurRole->m_FileName = FileName;
        //pCurRole->m_Name = ExtractFileName(FileName);

        TListItem *pItem;

        pItem = ListView1->Items->Item[iRole];
        if (pItem) {
            //pItem->Caption = IntToStr(iRole) + "-" + FileName;
            pItem->ImageIndex = iRole;

            //pItem->SubItems->Strings[0] = (pCurRole->m_Name);
            pItem->SubItems->Strings[0] = (IntToStr(iWd));
            pItem->SubItems->Strings[1] = (IntToStr(iHt));
            //pItem->SubItems->Strings[2] = (FileName);
        }

        ListView1->Update();
        //ShowRole(Sender, iRole);

        //if ( Bitmap != NULL )
        //    delete Bitmap;
        Invalidate();
        wMain->boModified = true;
        }
        catch (...)
        {
            MessageBeep(0);
        }
        //delete Bitmap;
    }
    else
        MessageBeep(0);
}
//---------------------------------------------------------------------------


