object DlgTransColor: TDlgTransColor
  Left = 444
  Top = 547
  BorderStyle = bsDialog
  Caption = 'Set Transparent Color'
  ClientHeight = 141
  ClientWidth = 262
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 120
  TextHeight = 16
  object Bevel1: TBevel
    Left = 10
    Top = 10
    Width = 247
    Height = 87
    Shape = bsFrame
  end
  object Label1: TLabel
    Left = 24
    Top = 24
    Width = 205
    Height = 16
    Caption = 'Enter Color in Hexdecimal Format :'
  end
  object OKBtn: TButton
    Left = 137
    Top = 110
    Width = 56
    Height = 30
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 204
    Top = 110
    Width = 53
    Height = 30
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object MaskEdit1: TMaskEdit
    Left = 64
    Top = 56
    Width = 99
    Height = 24
    EditMask = '\0\x\0\0AAAAAA;1;_'
    MaxLength = 10
    TabOrder = 2
    Text = '0x00      '
  end
end
