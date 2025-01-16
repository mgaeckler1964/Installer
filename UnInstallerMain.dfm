object UninstallMainForm: TUninstallMainForm
  Left = 739
  Top = 246
  Width = 517
  Height = 258
  Caption = 'Application Deinstaller'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ButtonRemove: TButton
    Left = 8
    Top = 186
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Remove'
    TabOrder = 0
    OnClick = ButtonRemoveClick
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 186
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = ButtonCancelClick
  end
  object MemoStatus: TMemo
    Left = 8
    Top = 16
    Width = 489
    Height = 161
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 2
  end
end
