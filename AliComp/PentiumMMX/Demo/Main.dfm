object FormMain: TFormMain
  Left = 189
  Top = 105
  Width = 536
  Height = 375
  Caption = 'MMX Instruction Set'
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
    Left = 28
    Top = 44
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 32
    Top = 140
    Width = 121
    Height = 21
    TabOrder = 1
    Text = 'Edit1'
  end
  object PentiumMMX1: TPentiumMMX
    Left = 156
    Top = 68
  end
end
