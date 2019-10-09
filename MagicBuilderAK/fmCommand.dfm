object wCommand: TwCommand
  Left = 591
  Top = 178
  Width = 611
  Height = 496
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Page Commands'
  Color = clBtnFace
  Constraints.MaxWidth = 640
  Constraints.MinHeight = 360
  Constraints.MinWidth = 400
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
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 441
    Height = 462
    Align = alLeft
    BevelInner = bvRaised
    BevelOuter = bvLowered
    Caption = 'Panel2'
    TabOrder = 0
    object Panel1: TPanel
      Left = 2
      Top = 2
      Width = 437
      Height = 39
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      object Label4: TLabel
        Left = 114
        Top = 8
        Width = 49
        Height = 13
        AutoSize = False
        Caption = 'Name :'
        WordWrap = True
      end
      object Label3: TLabel
        Left = 72
        Top = 8
        Width = 26
        Height = 13
        AutoSize = False
        Caption = 'None'
      end
      object Label2: TLabel
        Left = 3
        Top = 8
        Width = 69
        Height = 13
        AutoSize = False
        Caption = 'Page Index : '
        WordWrap = True
      end
      object edPageName: TEdit
        Left = 156
        Top = 5
        Width = 79
        Height = 21
        TabOrder = 0
        OnKeyDown = edPageNameKeyDown
      end
      object bnChangeName: TButton
        Left = 241
        Top = 5
        Width = 59
        Height = 20
        Caption = 'Change'
        TabOrder = 1
        OnClick = bnChangeNameClick
      end
    end
    object Panel3: TPanel
      Left = 2
      Top = 41
      Width = 437
      Height = 419
      Align = alClient
      BevelInner = bvRaised
      BevelOuter = bvNone
      BorderWidth = 6
      BorderStyle = bsSingle
      Caption = 'Panel3'
      TabOrder = 1
      object StringGrid1: TStringGrid
        Left = 7
        Top = 7
        Width = 419
        Height = 367
        Align = alClient
        BiDiMode = bdRightToLeft
        ColCount = 3
        DefaultColWidth = 80
        DefaultRowHeight = 20
        FixedCols = 0
        RowCount = 32
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goColSizing, goEditing, goAlwaysShowEditor]
        ParentBiDiMode = False
        PopupMenu = PopupMenu1
        TabOrder = 0
        OnDblClick = StringGrid1DblClick
        OnSelectCell = StringGrid1SelectCell
        OnSetEditText = StringGrid1SetEditText
        ColWidths = (
          78
          115
          195)
      end
      object cbActions: TComboBox
        Left = 208
        Top = 259
        Width = 177
        Height = 21
        BevelInner = bvLowered
        BevelKind = bkSoft
        BevelOuter = bvRaised
        Style = csDropDownList
        Color = clMoneyGreen
        DropDownCount = 15
        ItemHeight = 13
        TabOrder = 1
        Visible = False
        OnSelect = cbActionsSelect
      end
      object cbPages: TComboBox
        Left = 91
        Top = 259
        Width = 86
        Height = 21
        BevelInner = bvLowered
        BevelKind = bkSoft
        BevelOuter = bvRaised
        Style = csDropDownList
        Color = clMoneyGreen
        Ctl3D = True
        DropDownCount = 10
        ItemHeight = 13
        ParentCtl3D = False
        TabOrder = 2
        Visible = False
        OnSelect = cbPagesSelect
      end
      object cbButtons: TComboBox
        Left = 11
        Top = 259
        Width = 62
        Height = 21
        BevelInner = bvLowered
        BevelKind = bkSoft
        BevelOuter = bvRaised
        Style = csDropDownList
        Color = clMoneyGreen
        Ctl3D = True
        DropDownCount = 10
        ItemHeight = 13
        ParentCtl3D = False
        TabOrder = 3
        Visible = False
        OnSelect = cbButtonsSelect
      end
      object ToolBar1: TToolBar
        Left = 7
        Top = 374
        Width = 419
        Height = 34
        Align = alBottom
        ButtonHeight = 21
        ButtonWidth = 51
        Caption = 'ToolBar1'
        ShowCaptions = True
        TabOrder = 4
        Visible = False
        object bnSelectAll: TToolButton
          Left = 0
          Top = 2
          Caption = 'Select All'
          ImageIndex = 0
          OnClick = bnSelectAllClick
        end
        object bnCopy: TToolButton
          Left = 51
          Top = 2
          Caption = 'Copy'
          ImageIndex = 1
        end
        object bnPaste: TToolButton
          Left = 102
          Top = 2
          Caption = 'Paste'
          ImageIndex = 2
        end
        object bnClear: TToolButton
          Left = 153
          Top = 2
          Caption = 'Clear'
          ImageIndex = 3
        end
        object lbPageHash: TLabel
          Left = 204
          Top = 2
          Width = 48
          Height = 21
          Caption = '00000000'
        end
        object Label1: TLabel
          Left = 252
          Top = 2
          Width = 58
          Height = 21
          AutoSize = False
          Caption = 'Hash Key :'
          WordWrap = True
        end
      end
      object cbEvents: TComboBox
        Left = 331
        Top = 347
        Width = 62
        Height = 21
        BevelInner = bvLowered
        BevelKind = bkSoft
        BevelOuter = bvRaised
        Style = csDropDownList
        Color = clMoneyGreen
        Ctl3D = True
        DropDownCount = 10
        ItemHeight = 13
        ParentCtl3D = False
        TabOrder = 5
        Visible = False
        OnSelect = cbEventsSelect
        Items.Strings = (
          'KeyDown'
          'KeyUp'
          'KeyRepeat'
          'Hold 1 Sec'
          'Hold 5 Sec')
      end
    end
  end
  object StringGrid2: TStringGrid
    Left = 464
    Top = 176
    Width = 121
    Height = 177
    FixedCols = 0
    FixedRows = 0
    TabOrder = 1
    Visible = False
  end
  object PopupMenu1: TPopupMenu
    Left = 464
    Top = 96
    object Copy1: TMenuItem
      Caption = 'Copy'
      OnClick = Copy1Click
    end
    object Paste1: TMenuItem
      Caption = 'Paste'
      OnClick = Paste1Click
    end
    object clear1: TMenuItem
      Caption = 'clear'
    end
  end
end
