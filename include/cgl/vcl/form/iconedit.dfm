object frmIconEdit: TfrmIconEdit
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = #22270#26631#32534#36753
  ClientHeight = 356
  ClientWidth = 351
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = #24494#36719#38597#40657
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 19
  object Button1: TButton
    Left = 176
    Top = 319
    Width = 75
    Height = 25
    Caption = #30830#23450'(&O)'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 268
    Top = 319
    Width = 75
    Height = 25
    Caption = #21462#28040'(&C)'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 351
    Height = 305
    Align = alTop
    TabOrder = 2
    object Label1: TLabel
      Left = 88
      Top = 20
      Width = 39
      Height = 19
      Caption = #26631#39064#65306
    end
    object Label2: TLabel
      Left = 8
      Top = 90
      Width = 39
      Height = 19
      Caption = #36830#25509#65306
    end
    object Label3: TLabel
      Left = 9
      Top = 208
      Width = 39
      Height = 19
      Caption = #22791#27880#65306
    end
    object SpeedButton1: TSpeedButton
      Left = 314
      Top = 120
      Width = 23
      Height = 22
      Caption = '...'
      OnClick = SpeedButton1Click
    end
    object Label4: TLabel
      Left = 9
      Top = 150
      Width = 39
      Height = 19
      Caption = #22270#26631#65306
    end
    object SpeedButton2: TSpeedButton
      Left = 314
      Top = 180
      Width = 23
      Height = 22
      Caption = '...'
      OnClick = Image1Click
    end
    object Label5: TLabel
      Left = 264
      Top = 148
      Width = 39
      Height = 19
      Caption = #32034#24341#65306
    end
    object edtTitle: TEdit
      Left = 88
      Top = 45
      Width = 249
      Height = 27
      TabOrder = 0
    end
    object edtComment: TEdit
      Left = 9
      Top = 233
      Width = 329
      Height = 27
      TabOrder = 1
    end
    object Panel1: TPanel
      Left = 8
      Top = 8
      Width = 64
      Height = 64
      BevelOuter = bvLowered
      Color = clWhite
      ParentBackground = False
      TabOrder = 2
      object Image1: TImage
        Left = 1
        Top = 1
        Width = 62
        Height = 62
        Align = alClient
        OnClick = Image1Click
        ExplicitLeft = 32
        ExplicitTop = 24
        ExplicitWidth = 105
        ExplicitHeight = 105
      end
    end
    object CheckBox1: TCheckBox
      Left = 8
      Top = 276
      Width = 97
      Height = 17
      Caption = #38145#23450#22270#26631
      TabOrder = 3
    end
    object CheckBox2: TCheckBox
      Left = 176
      Top = 276
      Width = 97
      Height = 17
      Caption = #26174#31034#22270#26631
      Enabled = False
      TabOrder = 4
    end
    object cbbShell: TComboBox
      Left = 9
      Top = 115
      Width = 304
      Height = 27
      AutoComplete = False
      TabOrder = 5
      OnSelect = cbbShellSelect
    end
    object cbbIcon: TComboBox
      Left = 9
      Top = 175
      Width = 249
      Height = 27
      TabOrder = 6
    end
    object edtIndex: TEdit
      Left = 264
      Top = 175
      Width = 49
      Height = 27
      TabOrder = 7
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 308
    Top = 4
  end
end
