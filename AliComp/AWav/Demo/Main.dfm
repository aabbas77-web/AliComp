object FormMain: TFormMain
  Left = 190
  Top = 105
  Width = 544
  Height = 375
  Caption = 'Wav Demo'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 12
    Top = 52
    Width = 75
    Height = 25
    Caption = 'Create'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 8
    Top = 8
    Width = 477
    Height = 21
    TabOrder = 1
  end
  object AWav1: TAWav
    Left = 248
    Top = 100
  end
end
