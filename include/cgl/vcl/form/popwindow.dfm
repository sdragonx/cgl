object frmAskBox: TfrmAskBox
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'frmAskBox'
  ClientHeight = 225
  ClientWidth = 400
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clLime
  Font.Height = -13
  Font.Name = #24494#36719#38597#40657
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 19
  object lblText: TLabel
    Left = 264
    Top = 32
    Width = 104
    Height = 105
    AutoSize = False
    Color = clNone
    ParentColor = False
    Transparent = True
  end
  object lblOK: TLabel
    Tag = 64
    Left = 268
    Top = 163
    Width = 100
    Height = 24
    Alignment = taCenter
    AutoSize = False
    Transparent = True
    OnClick = lblOKClick
    OnMouseEnter = lblOKMouseEnter
    OnMouseLeave = lblOKMouseLeave
  end
  object lblCancel: TLabel
    Tag = 64
    Left = 268
    Top = 193
    Width = 100
    Height = 24
    Alignment = taCenter
    AutoSize = False
    Transparent = True
    OnClick = lblCancelClick
    OnMouseEnter = lblOKMouseEnter
    OnMouseLeave = lblOKMouseLeave
  end
  object lblCheck: TLabel
    Left = 8
    Top = 193
    Width = 145
    Height = 24
    AutoSize = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clLime
    Font.Height = -16
    Font.Name = #24494#36719#38597#40657
    Font.Style = []
    ParentFont = False
    Transparent = True
    OnClick = lblCheckClick
  end
  object Timer1: TTimer
    Interval = 40
    OnTimer = Timer1Timer
    Left = 64
    Top = 120
  end
end
