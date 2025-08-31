object FormMain: TFormMain
  Left = 190
  Top = 105
  Width = 544
  Height = 375
  Caption = 'Gif Image'
  Color = clTeal
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object AGIFImage1: TAGIFImage
    Left = 10
    Top = 10
    Width = 256
    Height = 192
    FileName = 'E:\Mazen\Res\GPIB1.gif'
    Animate = True
  end
  object Button1: TButton
    Left = 2
    Top = 292
    Width = 75
    Height = 25
    Caption = 'Load'
    TabOrder = 0
    OnClick = Button1Click
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'gif'
    Filter = 'GIF Images (*.gif)|*.gif'
    Left = 86
    Top = 291
  end
end
