object Form1: TForm1
  Left = 190
  Top = 119
  Width = 694
  Height = 483
  Caption = 'Form1'
  Color = clTeal
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 16
  object ARoundButton1: TARoundButton
    Left = 42
    Top = 59
    Width = 99
    Height = 103
    ButtonType = rtInset
    OnClick = ARoundButton1Click
    Margin = 9
    Color = clBtnFace
    ClickTime = 100
  end
  object ARoundButton2: TARoundButton
    Left = 309
    Top = 101
    Width = 40
    Height = 40
    Margin = 4
    Color = clBtnFace
    ClickTime = 500
  end
  object ARoundButton3: TARoundButton
    Left = 175
    Top = 139
    Width = 107
    Height = 107
    Color = clBtnFace
    ClickTime = 100
  end
  object ARoundButton4: TARoundButton
    Left = 313
    Top = 292
    Width = 85
    Height = 75
    Color = clBtnFace
    ClickTime = 100
  end
  object Button1: TButton
    Left = 76
    Top = 252
    Width = 75
    Height = 25
    Caption = 'Click'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 359
    Top = 179
    Width = 75
    Height = 25
    Caption = 'Click'
    TabOrder = 1
    OnClick = Button2Click
  end
end
