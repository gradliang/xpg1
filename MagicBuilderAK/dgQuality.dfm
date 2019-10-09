object DlgQuality: TDlgQuality
  Left = 374
  Top = 335
  BorderStyle = bsDialog
  Caption = 'Jpeg Quality Option'
  ClientHeight = 129
  ClientWidth = 313
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 297
    Height = 71
    Shape = bsFrame
  end
  object Label1: TLabel
    Left = 247
    Top = 33
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object OKBtn: TButton
    Left = 79
    Top = 96
    Width = 75
    Height = 24
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 159
    Top = 96
    Width = 75
    Height = 24
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object TrackBar1: TTrackBar
    Left = 26
    Top = 33
    Width = 222
    Height = 26
    Max = 100
    Orientation = trHorizontal
    Frequency = 5
    Position = 10
    SelEnd = 0
    SelStart = 0
    TabOrder = 2
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = TrackBar1Change
  end
end
