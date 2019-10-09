object dlgMovieSize: TdlgMovieSize
  Left = 323
  Top = 247
  BorderStyle = bsDialog
  Caption = 'Movie Screen Size'
  ClientHeight = 171
  ClientWidth = 289
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 21
    Top = 8
    Width = 246
    Height = 77
    Shape = bsFrame
  end
  object Bevel2: TBevel
    Left = 21
    Top = 93
    Width = 246
    Height = 44
    Shape = bsFrame
  end
  object OKBtn: TButton
    Left = 163
    Top = 148
    Width = 45
    Height = 22
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 224
    Top = 148
    Width = 46
    Height = 22
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object EditWidth: TLabeledEdit
    Left = 169
    Top = 20
    Width = 85
    Height = 21
    EditLabel.Width = 28
    EditLabel.Height = 13
    EditLabel.Caption = 'Width'
    ImeMode = imDisable
    LabelPosition = lpLeft
    LabelSpacing = 3
    TabOrder = 2
    OnChange = EditWidthChange
  end
  object EditHeight: TLabeledEdit
    Left = 169
    Top = 52
    Width = 85
    Height = 21
    EditLabel.Width = 31
    EditLabel.Height = 13
    EditLabel.Caption = 'Height'
    ImeMode = imDisable
    LabelPosition = lpLeft
    LabelSpacing = 3
    TabOrder = 3
    OnChange = EditHeightChange
  end
  object cbMovieSize: TComboBox
    Left = 124
    Top = 104
    Width = 130
    Height = 21
    ItemHeight = 13
    ItemIndex = 6
    TabOrder = 4
    Text = '720 * 480'
    OnChange = cbMovieSizeChange
    Items.Strings = (
      '176 * 220'
      '220 * 176'
      '240 * 320'
      '320 * 240'
      '352 * 288'
      '640 * 480'
      '720 * 480'
      '800 * 600'
      '1024 * 576'
      '1024 * 768'
      '1280 * 720'
      '1280 * 768'
      '1280 * 1024'
      '1600 * 1200'
      '1920 * 1080')
  end
  object chkCustomSize: TRadioButton
    Left = 32
    Top = 40
    Width = 97
    Height = 17
    Caption = 'Custom Size'
    TabOrder = 5
  end
  object chkDefaultSize: TRadioButton
    Left = 32
    Top = 104
    Width = 89
    Height = 17
    Caption = 'Default Size'
    Checked = True
    TabOrder = 6
    TabStop = True
  end
end
