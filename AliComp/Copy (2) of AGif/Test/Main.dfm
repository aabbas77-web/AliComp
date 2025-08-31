object FormMain: TFormMain
  Left = 190
  Top = 105
  Width = 544
  Height = 375
  Caption = 'Gif Tester'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 4
    Top = 320
    Width = 75
    Height = 25
    Caption = 'Load'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 80
    Top = 320
    Width = 75
    Height = 25
    Caption = 'Play'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 156
    Top = 320
    Width = 75
    Height = 25
    Caption = 'Stop'
    TabOrder = 2
    OnClick = Button3Click
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'gif'
    Filter = 'GIF Images (*.gif)|*.gif'
    Left = 22
    Top = 282
  end
end
