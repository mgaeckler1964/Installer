object ProgramFilesForm: TProgramFilesForm
  Left = 433
  Top = 514
  Width = 672
  Height = 510
  Caption = 'Program Files'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ListBoxProgramFiles: TListBox
    Left = 137
    Top = 41
    Width = 527
    Height = 368
    Align = alClient
    ItemHeight = 13
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 409
    Width = 664
    Height = 74
    Align = alBottom
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 56
      Height = 13
      Caption = 'Destination:'
    end
    object ButtonAddFile: TButton
      Left = 8
      Top = 40
      Width = 75
      Height = 25
      Caption = 'Add File...'
      TabOrder = 0
      OnClick = ButtonAddFileClick
    end
    object ButtonRemove: TButton
      Left = 168
      Top = 40
      Width = 75
      Height = 25
      Caption = 'Remove'
      TabOrder = 1
      OnClick = ButtonRemoveClick
    end
    object ButtonClose: TButton
      Left = 584
      Top = 40
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Close'
      TabOrder = 2
      OnClick = ButtonCloseClick
    end
    object ButtonAddDirectory: TButton
      Left = 88
      Top = 40
      Width = 75
      Height = 25
      Caption = 'Add Dir...'
      TabOrder = 3
      OnClick = ButtonAddDirectoryClick
    end
    object ComboBoxDestination: TComboBox
      Left = 72
      Top = 8
      Width = 361
      Height = 21
      ItemHeight = 13
      TabOrder = 4
      OnChange = ComboBoxDestinationChange
      Items.Strings = (
        '$ProgramFiles32'
        '$ProgramFiles64'
        '$VstPlugins32Bit'
        '$VstPlugins64Bit'
        '$VstPresets'
        '')
    end
  end
  object ListBoxGroups: TListBox
    Left = 0
    Top = 41
    Width = 137
    Height = 368
    Align = alLeft
    ItemHeight = 13
    TabOrder = 2
    OnClick = ListBoxGroupsClick
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 664
    Height = 41
    Align = alTop
    TabOrder = 3
    object Label2: TLabel
      Left = 16
      Top = 16
      Width = 32
      Height = 13
      Caption = 'Groupt'
    end
    object EditGroupName: TEdit
      Left = 56
      Top = 8
      Width = 249
      Height = 21
      TabOrder = 0
    end
    object ButtonAddGroup: TButton
      Left = 312
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 1
      OnClick = ButtonAddGroupClick
    end
    object ButtonDelete: TButton
      Left = 392
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Delete'
      TabOrder = 2
      OnClick = ButtonDeleteClick
    end
  end
  object OpenDialog: TOpenDialog
    Options = [ofHideReadOnly, ofNoChangeDir, ofAllowMultiSelect, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Select a File...'
    Left = 40
    Top = 24
  end
end
