{********************************************************************
TFORMSHAPE component
for Delphi 2.0, 3.0, C++Builder 1.0, 3.0
version 1.0

written by Bruno Fierens,
           TMS Software
           copyright _ 1998
           Email : tms@theLinx.com
           Website : http://theLinx.com/tms
           Component page : http://www.ven.be/freestyle/delphi.htm

The source code is given as is. The author is not responsible
for any possible damage done due to the use of this code.
The component can be freely used in any application. The source
code remains property of the author and may not be distributed
freely as such.
********************************************************************}

unit formshape;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs;


type
  EFormShapeError = class(Exception);

  TBalloonPosition = (bpTopLeft,bpLeftTop,bpTopRight,bpRightTop,
                      bpBottomLeft,bpLeftBottom,bpBottomRight,bpRightBottom,bpNone);

  tformshape = class(tcomponent)
  private
    { Private declarations }
    FReshape:boolean;
    FOnPaint:TNotifyEvent;
    FOnPaintAssigned:boolean;
    FBalloonPosition:TBalloonPosition;
    FBalloonEllips:integer;
    FBalloonIndent:integer;
    procedure PaintForm(sender:tobject);
    procedure SetBalloonIndent(avalue:integer);
    procedure SetBalloonEllips(avalue:integer);
  protected
    { Protected declarations }
    procedure Loaded; override;
  public
    { Public declarations }
    constructor Create(AOwner: TComponent); override;
  published
    { Published declarations }
    property Reshape:boolean read FReshape write FReshape;
    property BalloonPosition:TBalloonposition read FBalloonPosition write FBalloonPosition;
    property BalloonEllips:integer read FBalloonEllips write SetBalloonEllips;
    property BalloonIndent:integer read FBalloonIndent write SetBalloonIndent;
  end;

procedure Register;

implementation

constructor tformshape.Create(AOwner:TComponent);
begin
  inherited Create(AOwner);
  FBalloonIndent:=15;
  FBalloonEllips:=20;

  if not (Owner is TForm) then
    raise EFormShapeError.Create('Control parent must be a form!');

  if (csDesigning in ComponentState) then exit;
end;

procedure TFormShape.SetBalloonIndent(avalue:integer);
begin
 if (avalue>0) then FBalloonIndent:=avalue;
end;

procedure TFormShape.SetBalloonEllips(avalue:integer);
begin
 if (avalue>0) then FBalloonEllips:=avalue;
end;

procedure TFormShape.Loaded;
var
 hrgn1:thandle;
 hrgn2:thandle;
 hrgn:thandle;
 triangle:array[0..2] of tpoint;

begin
 inherited Loaded;

  if not freshape then exit;
  if (csDesigning in ComponentState) then exit;

  with (Owner as TForm) do
   begin
    FOnPaintAssigned:=false;
    if (assigned(OnPaint)) then
     begin
      FOnPaint:=OnPaint;
      FOnPaintAssigned:=true;
     end;
    OnPaint:=PaintForm;
   end;

  with (Owner as TForm) do
   begin
    borderstyle:=bsNone;

    hrgn:=CreateRectRgn(0,0,clientrect.right,clientrect.bottom);

    case BalloonPosition of
    bpNone:hrgn1:=CreateRoundRectRgn(0,0,clientrect.right,clientrect.bottom,balloonellips,balloonellips);
    bpTopLeft,bpTopRight:hrgn1:=CreateRoundRectRgn(0,balloonindent,clientrect.right,clientrect.bottom,balloonellips,balloonellips);
    bpLeftTop,bpLeftBottom:hrgn1:=CreateRoundRectRgn(balloonindent,0,clientrect.right,clientrect.bottom,balloonellips,balloonellips);
    bpRightTop,bpRightBottom:hrgn1:=CreateRoundRectRgn(0,0,clientrect.right-balloonindent,clientrect.bottom,balloonellips,balloonellips);
    bpBottomLeft,bpBottomRight:hrgn1:=CreateRoundRectRgn(0,0,clientrect.right,clientrect.bottom-balloonindent,balloonellips,balloonellips);
    end;

    case BalloonPosition of
    bpTopLeft:begin
                 triangle[0]:=point(balloonellips,0);
                 triangle[1]:=point(balloonellips,balloonindent);
                 triangle[2]:=point(balloonellips+balloonindent,balloonindent);
              end;
    bpTopRight:begin
                 triangle[0]:=point(clientrect.right-balloonellips,0);
                 triangle[1]:=point(clientrect.right-balloonellips,balloonindent);
                 triangle[2]:=point(clientrect.right-(balloonellips+balloonindent),balloonindent);
               end;
    bpBottomLeft:begin
                 triangle[0]:=point(balloonellips,clientrect.bottom);
                 triangle[1]:=point(balloonellips,clientrect.bottom-balloonindent-1);
                 triangle[2]:=point(balloonellips+balloonindent,clientrect.bottom-balloonindent-1);
              end;
    bpBottomRight:begin
                 triangle[0]:=point(clientrect.right-balloonellips,clientrect.bottom);
                 triangle[1]:=point(clientrect.right-balloonellips,clientrect.bottom-balloonindent-1);
                 triangle[2]:=point(clientrect.right-(balloonellips+balloonindent),clientrect.bottom-balloonindent-1);
               end;
    bpLeftTop:begin
                 triangle[0]:=point(0,balloonellips);
                 triangle[1]:=point(balloonindent,balloonellips);
                 triangle[2]:=point(balloonindent,balloonellips+balloonindent);
              end;
    bpLeftBottom:begin
                 triangle[0]:=point(0,clientrect.bottom-balloonellips);
                 triangle[1]:=point(balloonindent,clientrect.bottom-balloonellips);
                 triangle[2]:=point(balloonindent,clientrect.bottom-(balloonellips+balloonindent));
                 end;
    bpRightTop:begin
                 triangle[0]:=point(clientrect.right,balloonellips);
                 triangle[1]:=point(clientrect.right-balloonindent-1,balloonellips);
                 triangle[2]:=point(clientrect.right-balloonindent-1,balloonellips+balloonindent);
              end;
    bpRightBottom:begin
                 triangle[0]:=point(clientrect.right,clientrect.bottom-balloonellips);
                 triangle[1]:=point(clientrect.right-balloonindent-1,clientrect.bottom-balloonellips);
                 triangle[2]:=point(clientrect.right-balloonindent-1,clientrect.bottom-(balloonellips+balloonindent));
                 end;
    end;

    if BalloonPosition<>bpNone then
     begin
      hrgn2:=CreatePolygonRgn(triangle,3,WINDING);
      CombineRgn(hrgn,hrgn1,hrgn2,RGN_OR);
      setwindowrgn(handle,hrgn,true);
     end
    else
     setwindowrgn(handle,hrgn1,true);
  end;
end;

procedure TFormShape.PaintForm(sender:tobject);
var
 triangle:array[0..2] of tpoint;
 r:trect;

begin
 with (Owner as TForm) do
  begin
   canvas.pen.color:=clBlack;
   case BalloonPosition of
   bpNone:RoundRect(canvas.handle,0,0,clientrect.right-1,clientrect.bottom-1,balloonellips,balloonellips);
   bpTopLeft,bpTopRight:RoundRect(canvas.handle,0,balloonindent,clientrect.right-1,clientrect.bottom-1,balloonellips,balloonellips);
   bpLeftTop,bpLeftBottom:RoundRect(canvas.handle,balloonindent,0,clientrect.right-1,clientrect.bottom-1,balloonellips,balloonellips);
   bpRightTop,bpRightBottom:RoundRect(canvas.handle,0,0,clientrect.right-balloonindent-1,clientrect.bottom-1,balloonellips,balloonellips);
   bpBottomLeft,bpBottomRight:RoundRect(canvas.handle,0,0,clientrect.right-1,clientrect.bottom-balloonindent-1,balloonellips,balloonellips);
   end;

   case BalloonPosition of
   bpTopLeft:begin
                triangle[0]:=point(balloonellips,1);
                triangle[1]:=point(balloonellips,balloonindent);
                triangle[2]:=point(balloonellips+balloonindent-1,balloonindent);
             end;
   bpTopRight:begin
                triangle[0]:=point(clientrect.right-balloonellips-1,1);
                triangle[1]:=point(clientrect.right-balloonellips-1,balloonindent);
                triangle[2]:=point(clientrect.right-(balloonellips+balloonindent),balloonindent);
              end;
   bpBottomLeft:begin
                 triangle[0]:=point(balloonellips,clientrect.bottom-1);
                 triangle[1]:=point(balloonellips,clientrect.bottom-balloonindent-2);
                 triangle[2]:=point(balloonellips+balloonindent,clientrect.bottom-balloonindent-2);
                end;
   bpBottomRight:begin
                  triangle[0]:=point(clientrect.right-balloonellips-1,clientrect.bottom-1);
                  triangle[1]:=point(clientrect.right-balloonellips-1,clientrect.bottom-balloonindent-2);
                  triangle[2]:=point(clientrect.right-(balloonellips+balloonindent)+1,clientrect.bottom-balloonindent-2);
                 end;
   bpLeftTop:begin
              triangle[0]:=point(0,balloonellips);
              triangle[1]:=point(balloonindent,balloonellips);
              triangle[2]:=point(balloonindent,balloonellips+balloonindent);
             end;
   bpLeftBottom:begin
                 triangle[0]:=point(1,clientrect.bottom-balloonellips-1);
                 triangle[1]:=point(balloonindent,clientrect.bottom-balloonellips-1);
                 triangle[2]:=point(balloonindent,clientrect.bottom-(balloonellips+balloonindent));
                end;
   bpRightTop:begin
               triangle[0]:=point(clientrect.right-1,balloonellips);
               triangle[1]:=point(clientrect.right-balloonindent-2,balloonellips);
               triangle[2]:=point(clientrect.right-balloonindent-2,balloonellips+balloonindent);
              end;      
   bpRightBottom:begin
                  triangle[0]:=point(clientrect.right-2,clientrect.bottom-balloonellips-1);
                  triangle[1]:=point(clientrect.right-balloonindent-2,clientrect.bottom-balloonellips-1);
                  triangle[2]:=point(clientrect.right-balloonindent-2,clientrect.bottom-(balloonellips+balloonindent));
                 end;
   end;

   if BalloonPosition<>bpNone then
    begin
    canvas.polygon(triangle);

    canvas.pen.color:=color;

    case BalloonPosition of
    bpTopLeft:r:=rect(balloonellips,balloonindent,balloonellips+balloonindent-1,balloonindent);
    bpTopRight:r:=rect(clientrect.right-balloonellips,balloonindent,clientrect.right-(balloonellips+balloonindent)+1,balloonindent);
    bpLeftTop:r:=rect(balloonindent,balloonellips,balloonindent,balloonindent+balloonellips-1);
    bpLeftBottom:r:=rect(balloonindent,clientrect.bottom-balloonellips-1,balloonindent,clientrect.bottom-(balloonindent+balloonellips));
    bpRightTop:r:=rect(clientrect.right-balloonindent-2,balloonellips,clientrect.right-balloonindent-2,balloonindent+balloonellips);
    bpRightBottom:r:=rect(clientrect.right-balloonindent-2,clientrect.bottom-balloonellips-1,clientrect.right-balloonindent-2,clientrect.bottom-(balloonindent+balloonellips));
    bpBottomLeft:r:=rect(balloonellips,clientrect.bottom-balloonindent-2,balloonellips+balloonindent-1,clientrect.bottom-balloonindent-2);
    bpBottomRight:r:=rect(clientrect.right-balloonellips,clientrect.bottom-balloonindent-2,clientrect.right-(balloonellips+balloonindent)+1,clientrect.bottom-balloonindent-2);
    end;

    canvas.moveto(r.right,r.top);
    canvas.lineto(r.left,r.bottom);
   end; 

  end;
 {call existing handler}
 if (FOnPaintAssigned) then FOnPaint(sender);
end;

procedure Register;
begin
  RegisterComponents('Test', [tformshape]);
end;

end.
