object Form1: TForm1
  Left = 191
  Top = 105
  Width = 544
  Height = 375
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 9
    Top = 9
    Width = 90
    Height = 13
    Caption = 'Enter C++ Identifier'
  end
  object Label2: TLabel
    Left = 12
    Top = 65
    Width = 314
    Height = 37
    Caption = 'Invalid C++ Identifier'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Edit1: TEdit
    Left = 9
    Top = 25
    Width = 197
    Height = 21
    TabOrder = 0
    Text = 'if'
    OnChange = Edit1Change
  end
end
