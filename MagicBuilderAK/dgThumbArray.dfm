object OKBottomDlg2: TOKBottomDlg2
  Left = 559
  Top = 310
  BorderStyle = bsDialog
  Caption = 'Set Thumbnail Column'
  ClientHeight = 199
  ClientWidth = 304
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 120
  TextHeight = 16
  object Bevel1: TBevel
    Left = 10
    Top = 10
    Width = 263
    Height = 127
    Shape = bsFrame
  end
  object OKBtn: TButton
    Left = 81
    Top = 150
    Width = 93
    Height = 30
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 180
    Top = 150
    Width = 92
    Height = 30
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object EditRow: TLabeledEdit
    Left = 120
    Top = 32
    Width = 105
    Height = 24
    EditLabel.Width = 27
    EditLabel.Height = 16
    EditLabel.Caption = 'Row'
    ImeMode = imDisable
    LabelPosition = lpLeft
    LabelSpacing = 3
    TabOrder = 2
  end
  object EditHeight: TLabeledEdit
    Left = 120
    Top = 72
    Width = 105
    Height = 24
    EditLabel.Width = 45
    EditLabel.Height = 16
    EditLabel.Caption = 'Column'
    ImeMode = imDisable
    LabelPosition = lpLeft
    LabelSpacing = 3
    TabOrder = 3
  end
end
