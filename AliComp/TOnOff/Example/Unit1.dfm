object Form1: TForm1
  Left = 190
  Top = 119
  Width = 544
  Height = 360
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 16
  object UpLabel: TLabel
    Left = 22
    Top = 12
    Width = 73
    Height = 32
    Caption = 'UpLabel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -21
    Font.Name = 'Traditional Arabic'
    Font.Style = []
    ParentFont = False
  end
  object DownLabel: TLabel
    Left = 22
    Top = 49
    Width = 98
    Height = 29
    Caption = 'DownLabel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -19
    Font.Name = 'Traditional Arabic'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object OnOff1: TOnOff
    Left = 44
    Top = 98
    Width = 68
    Height = 95
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -19
    Font.Name = 'Traditional Arabic'
    Font.Style = [fsBold]
    ActiveFont.Charset = DEFAULT_CHARSET
    ActiveFont.Color = clBlue
    ActiveFont.Height = -21
    ActiveFont.Name = 'Traditional Arabic'
    ActiveFont.Style = []
    InnerRadius = 8
    OuterRadius = 20
    IsUp = True
    Angle = 40
    HandleColor = clGray
    Active = True
    Orientation = True
    UpLabel = UpLabel
    DownLabel = DownLabel
  end
end
