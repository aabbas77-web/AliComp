//------------------------------------------------------------------------------
// PentiumMMX component Sample
//                          ver.0.90 / date:Nov.20,1997 / copyright:H.Gotou,1997
//------------------------------------------------------------------------------

unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  PentiumMMX, StdCtrls, ExtCtrls, ComCtrls, Buttons, MMSystem, EditHex,
  Menus;

type
  TForm1 = class(TForm)
    PentiumMMX1: TPentiumMMX;
    BitBtn1: TBitBtn;
    PageControl1: TPageControl;
    TabSheet1: TTabSheet;
    Panel1: TPanel;
    Shape1: TShape;
    Shape2: TShape;
    Shape3: TShape;
    Panel2: TPanel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Panel3: TPanel;
    Label9: TLabel;
    Label10: TLabel;
    Panel4: TPanel;
    Label11: TLabel;
    Label12: TLabel;
    BitBtn2: TBitBtn;
    TabSheet2: TTabSheet;
    EditHex1: TEditHex;
    EditHex2: TEditHex;
    EditHex3: TEditHex;
    EditHex4: TEditHex;
    EditHex5: TEditHex;
    EditHex6: TEditHex;
    EditHex7: TEditHex;
    EditHex8: TEditHex;
    EditHex9: TEditHex;
    ComboBox1: TComboBox;
    Panel5: TPanel;
    EditHex10: TEditHex;
    EditHex11: TEditHex;
    EditHex12: TEditHex;
    EditHex13: TEditHex;
    Label13: TLabel;
    Label14: TLabel;
    Label15: TLabel;
    Panel6: TPanel;
    EditHex14: TEditHex;
    EditHex15: TEditHex;
    Label16: TLabel;
    Label17: TLabel;
    Label18: TLabel;
    Label19: TLabel;
    Label20: TLabel;
    Label21: TLabel;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    BitBtn3: TBitBtn;
    Bevel1: TBevel;
    Bevel2: TBevel;
    Bevel3: TBevel;
    Timer1: TTimer;
    Label22: TLabel;
    procedure GetCpuInfo;
    procedure GetPresentTime;
    function  GetInst: TMMXinst;
    procedure ExecMMX;
    procedure FormShow(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure BitBtn2Click(Sender: TObject);
    procedure BitBtn3Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure ComboBox1Change(Sender: TObject);
  private
    { Private éŒ¾ }
  public
    { Public éŒ¾ }
    StartTimeRDTSC: QuadWord;
    StopTimeRDTSC:  QuadWord;
    StartTimeMMTimer: integer;
    StopTimeMMTimer:  integer;
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure TForm1.GetCpuInfo;
begin
  if PentiumMMX1.CPUIDsupport then
  begin
    Shape1.Brush.Color := clAqua;
    Label1.Color := clAqua;
  end
  else
  begin
    Shape1.Brush.Color := clRed;
    Label1.Color := clRed;
  end;
  if PentiumMMX1.RDTSCsupport then
  begin
    Shape2.Brush.Color := clAqua;
    Label2.Color := clAqua;
  end
  else
  begin
    Shape2.Brush.Color := clRed;
    Label2.Color := clRed;
  end;
  if PentiumMMX1.MMXsupport then
  begin
    Shape3.Brush.Color := clAqua;
    Label3.Color := clAqua;
  end
  else
  begin
    Shape3.Brush.Color := clRed;
    Label3.Color := clRed;
  end;
  if PentiumMMX1.CPUIDsupport then
  begin
    EditHex1.Text := PentiumMMX1.VendorString;
    EditHex2.Text := IntToHex(PentiumMMX1.FeatureFlags,8);
    EditHex3.Text := IntToHex(PentiumMMX1.Family,4);
    EditHex4.Text := IntToHex(PentiumMMX1.Model,2);
    EditHex5.Text := IntToHex(PentiumMMX1.SteppingID,2);
  end
  else
  begin
    EditHex1.Text := '----------';
    EditHex2.Text := '00000000';
    EditHex3.Text := '0000';
    EditHex4.Text := '00';
    EditHex5.Text := '00';
  end;
end;

procedure TForm1.GetPresentTime;
var
  h, m, s: integer;
  _h, _m, _s, _ms: word;
  xms: extended;
  Timeh, Timel, Timehh, Timehl, Timelh, Timell: integer;
  IntervalMMTimer: integer;
  IntervalRDTSC: QuadWord;
  freq, RDTSCtime, MMTimerTime: extended;
begin
  if PentiumMMX1.RDTSCsupport then
  begin
    StopTimeRDTSC.high := PentiumMMX1.TimeStampHigh;
    StopTimeRDTSC.low  := PentiumMMX1.TimeStampLow;
    StopTimeMMTimer := TimeGetTime;
    IntervalMMTimer := PentiumMMX1.Interval32(StopTimeMMTimer,StartTimeMMTimer);
    Timeh := (IntervalMMTimer and $ffff0000) shr $10;
    Timel :=  IntervalMMTimer and $0000ffff;
    IntervalRDTSC := PentiumMMX1.Interval64(StopTimeRDTSC,StartTimeRDTSC);
    Timehh := (IntervalRDTSC.high and $ffff0000) shr $10;
    Timehl :=  IntervalRDTSC.high and $0000ffff;
    Timelh := (IntervalRDTSC.low  and $ffff0000) shr $10;
    Timell :=  IntervalRDTSC.low  and $0000ffff;
    MMTimerTime := Timeh * 65536.0 + Timel;
    RDTSCTime   := Timehh * 65536.0 * 65536.0 *65536.0 + Timehl * 65536.0 *65536.0 + Timelh * 65536.0 + Timell;
    freq := (RDTSCTime / MMTimerTime)/1000; // MHz
    EditHex6.Text := IntToHex(StopTimeRDTSC.high,8);
    EditHex7.Text := IntToHex(StopTimeRDTSC.low,8);
    EditHex8.Text := FormatFloat('000.0000',freq)+'MHz';
    s := IntervalMMTimer div 1000;
    m := s div 60;
    h := m div 60;
    _ms := IntervalMMTimer mod 1000;
    _s := s mod 60;
    _m := m mod 60;
    _h := h mod 24;
    LongTimeFormat := '' + 'hh:mm:ss' + '';
    xms := _ms;
    EditHex9.Text := TimeToStr(EncodeTime(_h,_m,_s,_ms))+'_'+FormatFloat('000',xms);
  end
  else
  begin
    EditHex6.Text := '00000000';
    EditHex7.Text := '00000000';
    EditHex8.Text := '---.----MHz';
    EditHex9.Text := '--:--:--_---';
  end;
end;

function TForm1.GetInst: TMMXinst;
begin
  case ComboBox1.ItemIndex of
    $00: result := _PAND;
    $01: result := _PANDN;
    $02: result := _POR;
    $03: result := _PXOR;
    $04: result := _PSLLw;
    $05: result := _PSLLd;
    $06: result := _PSLLq;
    $07: result := _PSRLw;
    $08: result := _PSRLd;
    $09: result := _PSRLq;
    $0A: result := _PSRAw;
    $0B: result := _PSRAd;
    $0C: result := _PCMPGTb;
    $0D: result := _PCMPGTw;
    $0E: result := _PCMPGTd;
    $0F: result := _PCMPEQb;
    $10: result := _PCMPEQw;
    $11: result := _PCMPEQd;
    $12: result := _PACKUSwb;
    $13: result := _PACKSSwb;
    $14: result := _PACKSSdw;
    $15: result := _PUNPCKLbw;
    $16: result := _PUNPCKLwd;
    $17: result := _PUNPCKLdq;
    $18: result := _PUNPCKHbw;
    $19: result := _PUNPCKHwd;
    $1A: result := _PUNPCKHdq;
    $1B: result := _PADDUSb;
    $1C: result := _PADDUSw;
    $1D: result := _PADDSb;
    $1E: result := _PADDSw;
    $1F: result := _PADDb;
    $20: result := _PADDw;
    $21: result := _PADDd;
    $22: result := _PSUBUSb;
    $23: result := _PSUBUSw;
    $24: result := _PSUBSb;
    $25: result := _PSUBSw;
    $26: result := _PSUBb;
    $27: result := _PSUBw;
    $28: result := _PSUBd;
    $29: result := _PMULLw;
    $2A: result := _PMULHw;
    $2B: result := _PMADDwd;
    else
    begin
      result := _;
      ComboBox1.Text := 'SET INSTRUCTION';
    end;
  end;
end;

procedure TForm1.ExecMMX;
var
  x, y, z: QuadWord;
  inst: TMMXinst;
begin
  x.low  := StrToInt(EditHex10.Text);
  x.high := StrToInt(EditHex11.Text);
  y.low  := StrToInt(EditHex12.Text);
  y.high := StrToInt(EditHex13.Text);
  inst := GetInst;
  z := PentiumMMX1.MmxOperate(inst,x,y);
  EditHex14.Text := IntToHex(z.low,8);
  EditHex15.Text := IntToHex(z.high, 8);
end;

procedure TForm1.FormShow(Sender: TObject);
var
 xtime: integer;
begin
  PageControl1.ActivePage := TabSheet1;
  if PentiumMMX1.MMXsupport then
  begin
    TabSheet2.TabVisible := true;
  end
  else
  begin
    TabSheet2.TabVisible := false;
  end;
  GetCpuInfo;
  if PentiumMMX1.RDTSCsupport then
  begin
    StartTimeRDTSC.high := PentiumMMX1.TimeStampHigh;
    StartTimeRDTSC.low  := PentiumMMX1.TimeStampLow;
    StartTimeMMTimer := TimeGetTime;
    repeat
      StopTimeMMTimer := TimeGetTime;
      xTime := StopTimeMMTimer-StartTimeMMTimer;
    until xTime > 500;
    GetPresentTime;
  end;
end;

procedure TForm1.BitBtn1Click(Sender: TObject);
begin
  close;
end;

procedure TForm1.BitBtn2Click(Sender: TObject);
begin
  GetPresentTime;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
begin
  GetPresentTime;
end;

procedure TForm1.BitBtn3Click(Sender: TObject);
begin
  ExecMMX;
end;

procedure TForm1.ComboBox1Change(Sender: TObject);
begin
  ExecMMX;
end;

end.
