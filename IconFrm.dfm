object IconForm: TIconForm
  Left = 525
  Top = 632
  Width = 570
  Height = 333
  Caption = 'Icons'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 265
    Width = 562
    Height = 41
    Align = alBottom
    TabOrder = 0
    object ButtonAdd: TButton
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 0
      OnClick = ButtonAddClick
    end
    object ButtonRemove: TButton
      Left = 88
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Remove'
      TabOrder = 1
      OnClick = ButtonRemoveClick
    end
    object ButtonClose: TButton
      Left = 480
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 2
      OnClick = ButtonCloseClick
    end
  end
  object StringGridIcons: TStringGrid
    Left = 0
    Top = 41
    Width = 562
    Height = 224
    Align = alClient
    ColCount = 4
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goEditing, goTabs]
    TabOrder = 1
    OnMouseDown = StringGridIconsMouseDown
    OnSetEditText = StringGridIconsSetEditText
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 562
    Height = 41
    Align = alTop
    TabOrder = 2
    object Label1: TLabel
      Left = 8
      Top = 8
      Width = 32
      Height = 13
      Caption = 'Group:'
    end
    object EditIconGroup: TEdit
      Left = 48
      Top = 8
      Width = 505
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
    end
  end
end
