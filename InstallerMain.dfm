object InstallerForm: TInstallerForm
  Left = 424
  Top = 229
  Width = 726
  Height = 276
  Caption = 'Application Installer'
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
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 56
    Height = 13
    Caption = 'Destination:'
  end
  object Label2: TLabel
    Left = 8
    Top = 48
    Width = 50
    Height = 13
    Caption = 'BDE Path:'
  end
  object Label3: TLabel
    Left = 8
    Top = 80
    Width = 51
    Height = 13
    Caption = 'Data Path:'
  end
  object Label4: TLabel
    Left = 8
    Top = 112
    Width = 54
    Height = 13
    Caption = 'Start menu:'
  end
  object LabelStatus: TLabel
    Left = 8
    Top = 152
    Width = 30
    Height = 13
    Caption = 'Status'
  end
  object EditDestination: TEdit
    Left = 80
    Top = 16
    Width = 563
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
    OnChange = PathChanged
  end
  object EditBDE: TEdit
    Left = 80
    Top = 48
    Width = 563
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 2
    OnChange = PathChanged
  end
  object EditDataPath: TEdit
    Left = 80
    Top = 80
    Width = 563
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 4
    OnChange = PathChanged
  end
  object ButtonSelectDestination: TButton
    Left = 650
    Top = 16
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Select...'
    TabOrder = 1
    OnClick = SelectPath
  end
  object ButtonSelectBDE: TButton
    Left = 650
    Top = 48
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Select...'
    TabOrder = 3
    OnClick = SelectPath
  end
  object ButtonSelectDataPath: TButton
    Left = 650
    Top = 80
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Select...'
    TabOrder = 5
    OnClick = SelectPath
  end
  object ButtonInstall: TButton
    Left = 8
    Top = 200
    Width = 75
    Height = 25
    Caption = 'Install'
    Enabled = False
    TabOrder = 9
    OnClick = ButtonInstallClick
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 200
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 10
    OnClick = ButtonCancelClick
  end
  object ProgressBar: TProgressBar
    Left = 8
    Top = 176
    Width = 720
    Height = 16
    Anchors = [akLeft, akTop, akRight]
    Min = 0
    Max = 100
    TabOrder = 8
  end
  object EditStartMenu: TEdit
    Left = 80
    Top = 112
    Width = 563
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 6
    OnChange = PathChanged
  end
  object ButtonSelectStartMenu: TButton
    Left = 650
    Top = 112
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Select...'
    TabOrder = 7
    OnClick = SelectPath
  end
end
