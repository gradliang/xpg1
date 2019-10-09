object OKBottomDlg: TOKBottomDlg
  Left = 206
  Top = 703
  BorderStyle = bsDialog
  Caption = 'Insert Duplicattion'
  ClientHeight = 108
  ClientWidth = 310
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  OnClose = FormClose
  PixelsPerInch = 120
  TextHeight = 16
  object Bevel1: TBevel
    Left = 10
    Top = 10
    Width = 295
    Height = 55
    Shape = bsFrame
  end
  object OKBtn: TButton
    Left = 185
    Top = 70
    Width = 56
    Height = 30
    Caption = 'OK'
    Default = True
    Enabled = False
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 249
    Top = 70
    Width = 56
    Height = 30
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object ComboBox1: TComboBox
    Left = 184
    Top = 24
    Width = 113
    Height = 24
    ItemHeight = 16
    TabOrder = 2
    Text = 'Select Page...'
    OnChange = ComboBox1Change
  end
  object StaticText1: TStaticText
    Left = 24
    Top = 24
    Width = 155
    Height = 20
    Alignment = taCenter
    Caption = 'Insert copy before page : '
    TabOrder = 3
  end
end
