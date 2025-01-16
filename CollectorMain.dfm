object CollectorForm: TCollectorForm
  Left = 674
  Top = 187
  Width = 731
  Height = 390
  Caption = 'MG Setup Collector'
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 128
    Width = 73
    Height = 13
    Caption = 'BDE Language'
  end
  object Label3: TLabel
    Left = 8
    Top = 192
    Width = 51
    Height = 13
    Caption = 'Data Path:'
  end
  object Label2: TLabel
    Left = 8
    Top = 16
    Width = 78
    Height = 13
    Caption = 'Application Title:'
  end
  object Label4: TLabel
    Left = 8
    Top = 48
    Width = 38
    Height = 13
    Caption = 'Version:'
  end
  object Label5: TLabel
    Left = 8
    Top = 160
    Width = 47
    Height = 13
    Caption = 'BDE Alias'
  end
  object Label6: TLabel
    Left = 8
    Top = 80
    Width = 34
    Height = 13
    Caption = 'Author;'
  end
  object ButtonCreateInstaller: TButton
    Left = 176
    Top = 280
    Width = 75
    Height = 25
    Caption = 'Create'
    TabOrder = 11
    OnClick = ButtonCreateInstallerClick
  end
  object RadioGroupBDE: TRadioGroup
    Left = 120
    Top = 112
    Width = 580
    Height = 41
    Anchors = [akLeft, akTop, akRight]
    Caption = 'BDE'
    Columns = 3
    Items.Strings = (
      'English'
      'Deutsch'
      'None')
    TabOrder = 3
    OnClick = RadioGroupBDEClick
  end
  object EditDataPath: TEdit
    Left = 120
    Top = 192
    Width = 490
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 5
  end
  object ButtonSelectDataPath: TButton
    Left = 621
    Top = 192
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Select...'
    TabOrder = 6
    OnClick = ButtonSelectDataPathClick
  end
  object EditApplicationTitle: TEdit
    Left = 120
    Top = 16
    Width = 580
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object EditApplicationVersion: TEdit
    Left = 120
    Top = 48
    Width = 580
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
  end
  object EditAuthor: TEdit
    Left = 120
    Top = 80
    Width = 580
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 2
  end
  object ButtonFiles: TButton
    Left = 8
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Files..'
    TabOrder = 7
    OnClick = ButtonFilesClick
  end
  object ButtonIcons: TButton
    Left = 96
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Icons...'
    TabOrder = 8
    OnClick = ButtonIconsClick
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 312
    Width = 715
    Height = 19
    Panels = <>
    SimplePanel = True
  end
  object ButtonOpen: TButton
    Left = 96
    Top = 280
    Width = 75
    Height = 25
    Caption = 'Open...'
    TabOrder = 10
    OnClick = OpenClick
  end
  object ButtonNew: TButton
    Left = 8
    Top = 280
    Width = 75
    Height = 25
    Caption = 'New'
    TabOrder = 9
    OnClick = NewClick
  end
  object ComboBoxBDEalias: TComboBox
    Left = 120
    Top = 160
    Width = 580
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    Sorted = True
    TabOrder = 4
  end
  object MainMenu: TMainMenu
    Left = 216
    Top = 272
    object File1: TMenuItem
      Caption = 'File'
      object New: TMenuItem
        Caption = 'New'
        OnClick = NewClick
      end
      object Open: TMenuItem
        Caption = 'Open'
        OnClick = OpenClick
      end
      object Save: TMenuItem
        Caption = 'Save'
        OnClick = SaveClick
      end
      object SaveAs: TMenuItem
        Caption = 'Save As'
        OnClick = SaveAsClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object TargetDirectory: TMenuItem
        Caption = 'Target Directory...'
        OnClick = TargetDirectoryClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit: TMenuItem
        Caption = 'Exit'
        OnClick = ExitClick
      end
    end
  end
  object OpenDialog: TOpenDialog
    DefaultExt = 'CSC'
    Filter = 'CRESD Setup Collections (*.csc)|*.csc'
    Options = [ofHideReadOnly, ofNoChangeDir, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Open Setup Collection'
    Left = 344
    Top = 280
  end
  object SaveDialog: TSaveDialog
    Filter = 'CRESD Setup Collections (*.csc)|*.csc'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofNoChangeDir, ofPathMustExist, ofCreatePrompt, ofEnableSizing]
    Title = 'Save Setup Collection'
    Left = 416
    Top = 280
  end
end
