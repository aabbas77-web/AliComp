object Form1: TForm1
  Left = 191
  Top = 105
  Width = 544
  Height = 376
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
  object APlasma1: TAPlasma
    Left = 0
    Top = 0
    Width = 536
    Height = 349
    OnMouseDown = APlasma1MouseDown
    Align = alClient
    Active = True
  end
  object PopupMenu1: TPopupMenu
    Left = 104
    Top = 112
    object Close1: TMenuItem
      Caption = '&Close'
      OnClick = Close1Click
    end
  end
end
