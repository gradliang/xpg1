object wMovSet: TwMovSet
  Left = 560
  Top = 234
  Width = 378
  Height = 468
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Movie Setting'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 0
    Top = 32
    Width = 361
    Height = 393
    Lines.Strings = (
      'Memo1')
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 362
    Height = 28
    AutoSize = True
    ButtonHeight = 24
    ButtonWidth = 113
    Caption = 'ToolBar1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    ShowCaptions = True
    TabOrder = 1
    object ToolButton1: TToolButton
      Left = 0
      Top = 2
      Caption = 'Load'
      ImageIndex = 0
      OnClick = LoadMovieCmdSetClick
    end
    object bnSaveAs: TToolButton
      Left = 113
      Top = 2
      Caption = 'Save As'
      ImageIndex = 3
      OnClick = bnSaveAsClick
    end
    object bnUpdate: TToolButton
      Left = 226
      Top = 2
      Caption = 'Save and Update'
      ImageIndex = 2
      OnClick = UpdateSettingClick
    end
  end
  object ImageList1: TImageList
    Left = 240
    Top = 88
  end
  object OpenDialog1: TOpenDialog
    Left = 296
  end
  object SaveDialog1: TSaveDialog
    Left = 328
  end
end
