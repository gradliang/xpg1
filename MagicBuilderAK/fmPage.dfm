object wPage: TwPage
  Left = 288
  Top = 517
  Width = 620
  Height = 162
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Page'
  Color = clBtnFace
  DockSite = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -10
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object IconImage: TImage
    Left = 7
    Top = 7
    Width = 65
    Height = 65
    Visible = False
  end
  object ListView1: TListView
    Left = 91
    Top = 0
    Width = 410
    Height = 97
    BevelKind = bkFlat
    BorderWidth = 1
    Color = clScrollBar
    Columns = <>
    FullDrag = True
    GridLines = True
    HideSelection = False
    HotTrackStyles = [htHandPoint]
    IconOptions.Arrangement = iaLeft
    LargeImages = IconImageList
    PopupMenu = PopupMenu1
    ShowWorkAreas = True
    TabOrder = 0
    OnDblClick = ListView1DblClick
    OnSelectItem = ListView1SelectItem
  end
  object IconImageList: TImageList
    Height = 80
    Width = 80
    Left = 16
    Top = 72
  end
  object PopupMenu1: TPopupMenu
    Left = 96
    Top = 32
    object New1: TMenuItem
      Caption = 'New'
      OnClick = New1Click
    end
    object Insert1: TMenuItem
      Caption = 'Insert New'
      OnClick = Insert1Click
    end
    object Duplicate1: TMenuItem
      Caption = 'Duplicate'
      OnClick = Duplicate1Click
    end
    object MoveTo1: TMenuItem
      Caption = 'Move To'
      OnClick = MoveTo1Click
    end
    object Delete1: TMenuItem
      Caption = 'Delete'
      OnClick = Delete1Click
    end
    object CopyName1: TMenuItem
      Caption = 'Copy Name'
      OnClick = wRoleCopyName1Click
    end
  end
  object TimerRefresh: TTimer
    Enabled = False
    Left = 88
    Top = 80
  end
end
