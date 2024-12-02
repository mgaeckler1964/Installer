object ErrorForm: TErrorForm
  Left = 436
  Top = 469
  Width = 762
  Height = 309
  Caption = 'Errors'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 240
    Width = 754
    Height = 42
    Align = alBottom
    TabOrder = 0
    object Button1: TButton
      Left = 335
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop]
      Cancel = True
      Caption = 'Close'
      ModalResult = 2
      TabOrder = 0
    end
  end
  object MemoErrors: TMemo
    Left = 0
    Top = 0
    Width = 754
    Height = 240
    Align = alClient
    TabOrder = 1
  end
end
