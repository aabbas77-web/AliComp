unit Bmp2Rgn;
{
Autor : Tomasz Kustra
W pliku Bmp2Rgn znajduj¹ siê dwie funkcje:
    GetRgnFromBmpLR(Bmp:TBitmap;BackColor:TColor;OffsetX,OffsetY:integer):THandle (szuka z lewej do prawej),
    GetRgnFromBmpTB(Bmp:TBitmap;BackColor:TColor;OffsetX,OffsetY:integer):THandle (szuka z góry do do³u)
tworz¹ce region z bitmapy.
Pomys³ jest prosty:
1.Zamiana bitmapy na czarno-bia³¹;
2.Odczytanie z niej wszystkich zaczernionych prostok¹tów;
3.Po³¹czenie w/w w region.

Inne funkcje zawarte w unicie:
1.  function PointBit(X,Y:Integer):Byte;-odczytuje czy pixel jest zapalony (>0 zapalony), dla bitmapy czarno-bia³ej(1 bit na pixel);
2.  procedure InitBitmap(Bitmap:TBitmap); - przypisuje wartoœci do zmiennych globalnych dla pozosta³ych funkcji;
3.  function FindNotWhite(R:TRect):TRect;-szuka kolejnego czarnego pixela;
4.  function IsBlackR(R:TRect):boolean;-sprawdza czy prawa i dolna krawêdŸ prostok¹ta jest czarna;
5.  function GetRectBRLR(r:TRect):TRect;-zwraca kolejny zaczernony prostok¹t (je¿eli Left = -1to nie znaleziono), szuka z lewej do prawej;
6.  function GetRectBRTB(r:TRect):TRect;zwraca kolejny zaczernony prostok¹t (je¿eli Left = -1to nie znaleziono), szuka z góry do do³u;
7.  function GetListRrectsLR(Bmp:TBitmap;BackColor:TColor):TList;-zwraca listê znaleziony zaczernionych prostok¹tów(lewa=>prawa);
9.  function GetListRrectsTB(Bmp:TBitmap;BackColor:TColor):TList;-zwraca listê znaleziony zaczernionych prostok¹tów(góra=>dó³);

Uwaga:
Je¿eli funkcja GetRgnFromBmp... =0 to mo¿e oznaczaæ, ¿e funkcja ExtCreateRegion nie stworzy³a regionu (bardzo,bardz ¿adko).
Trzeba wtedy stworzyæ region ³¹cz¹c prostok¹ty przy pomocy funkcji CombineRgn.
Opis w/w znajduje siê w helpie do API.
}



interface
uses
  Windows, Classes, Graphics;

  function PointBit(X,Y:Integer):Byte;
  procedure InitBitmap(Bitmap:TBitmap);
  function FindNotWhite(R:TRect):TRect;
  function IsBlackR(R:TRect):boolean;
  function GetRectBRLR(r:TRect):TRect;
  function GetRectBRTB(r:TRect):TRect;
  function GetListRrectsLR(Bmp:TBitmap;BackColor:TColor):TList;
  function GetListRrectsTB(Bmp:TBitmap;BackColor:TColor):TList;
  function GetRgnFromBmpLR(Bmp:TBitmap;BackColor:TColor;OffsetX,OffsetY:integer):THandle;
  function GetRgnFromBmpTB(Bmp:TBitmap;BackColor:TColor;OffsetX,OffsetY:integer):THandle;


var
   Step:LongInt = 0;
   FirstScanLine:PByte = nil;
   MaxX:Integer =0;
   MaxY:Integer =0;

implementation

procedure InitBitmap(Bitmap:TBitmap);
begin
     Step:=LongInt(Bitmap.ScanLine[0])-LongInt(Bitmap.ScanLine[1]);
     FirstScanLine:=Bitmap.ScanLine[0];
     MaxX:=Bitmap.Width;
     MaxY:=Bitmap.Height;
end;

function PointBit(X,Y:Integer):Byte;
var
   a:integer;
begin
     a:=(x shr 3);
     result:=((PByte(LongInt(FirstScanLine)-(y*Step)+a)^ shl (x-(a shl 3))) and $80);
end;

function FindNotWhite(R:TRect):TRect;
var
   i,j:integer;
begin
     Result.Left:=-1;
     for i:=r.Left to MaxX-1 do
         for j:=r.Top to MaxY-1 do begin
             if PointBit(i,j)>0 then begin
                Result.Left:=i;
                Result.Top:=j;
                Result.Right:=i;
                Result.Bottom:=j;
                exit;
                end;
             end;
end;

function IsBlackR(R:TRect):boolean;
var
   i:integer;
   a,b,c:integer;
begin
     Result:=True;
     b:=r.Bottom*Step;
     for i:=r.Right downto r.Left do begin
         a:=(i shr 3);
         c:=a+LongInt(FirstScanLine);
         a:=((PByte(c-b)^ shl (i-(a shl 3))) and $80);
         if a>0 then begin
            Result:=False;
            exit;
            end;
         end;
     a:=(r.Right shr 3);
     b:=r.Right-(a shl 3);
     a:=a+LongInt(FirstScanLine);
     for i:=r.Bottom downto r.Top do begin
         c:=((PByte(a-(i*Step))^ shl b) and $80);
         if c>0 then begin
            Result:=False;
            exit;
            end;
         end;
end;

function GetRectBRLR(r:TRect):TRect;
var
   i,j:integer;
   x:boolean;
   r1:TRect;
   a,b,c:integer;
begin
     Result.Left:=-1;
     x:=false;
     for i:=r.Left to MaxX-1 do begin
         a:=(i shr 3);
         b:=i-(a shl 3);
         a:=a+LongInt(FirstScanLine);
         for j:=0 to MaxY-1 do begin
             c:=((PByte(a-(j*Step))^ shl b) and $80);
             if c=0 then begin
                x:=True;
                Result.Left:=i;
                Result.Top:=j;
                Result.Right:=i;
                Result.Bottom:=j;
                break;
                end;
             end;
         if x then break;
         end;
     if x then begin
        while IsBlackR(Result) and (Result.Right<MaxX) and (Result.Bottom<MaxY) do begin
              Result.Right:=Result.Right+1;
              Result.Bottom:=Result.Bottom+1;
              end;
        r1:=Result;
        r1.Right:=r1.Right-1;
        r1.Bottom:=r1.Bottom-1;
        while IsBlackR(R1) and (R1.Right<MaxX) do
              R1.Right:=R1.Right+1;
        Result.Right:=r1.Right;
        R1.Right:=R1.Right-1;
        while IsBlackR(R1) and (R1.Bottom<MaxY) do
              R1.Bottom:=R1.Bottom+1;
        Result.Bottom:=r1.Bottom;
        end
     else Result.Left:=-1;
end;

function GetRectBRTB(r:TRect):TRect;
var
   i,j:integer;
   x:boolean;
   r1:TRect;
   a,b:integer;
begin
     Result.Left:=-1;
     x:=false;
     for i:=r.Top to MaxY-1 do begin
         b:=LongInt(FirstScanLine)-(i*Step);
         for j:=0 to MaxX-1 do begin
            a:=(j shr 3);
            a:=((PByte(b+a)^ shl (j-a)) and $80);
            if a=0 then begin
               x:=True;
               Result.Left:=j;
               Result.Top:=i;
               Result.Right:=j;
               Result.Bottom:=i;
               break;
               end;
             end;
         if x then break;
         end;
     if x then begin
        while IsBlackR(Result) and (Result.Right<MaxX) and (Result.Bottom<MaxY) do begin
              Result.Right:=Result.Right+1;
              Result.Bottom:=Result.Bottom+1;
              end;
        r1:=Result;
        r1.Right:=r1.Right-1;
        r1.Bottom:=r1.Bottom-1;
        while IsBlackR(R1) and (R1.Right<MaxX) do
              R1.Right:=R1.Right+1;
        Result.Right:=r1.Right;
        R1.Right:=R1.Right-1;
        while IsBlackR(R1) and (R1.Bottom<MaxY) do
              R1.Bottom:=R1.Bottom+1;
        Result.Bottom:=r1.Bottom;
        end
     else Result.Left:=-1;
end;

function GetListRrectsLR(Bmp:TBitmap;BackColor:TColor):TList;
var
   r:TRect;
   pr:PRect;
   fBitmap:TBitmap;
begin
     Result:=TList.Create;
     fBitmap:=TBitmap.Create;
     fBitmap.Assign(Bmp);
     fBitmap.Mask(BackColor);
     fBitmap.Monochrome:=True;
     fBitmap.PixelFormat:=pf1bit;
     fBitmap.Canvas.Brush.Color:=clWhite;
     InitBitmap(fBitmap);
     r.Left:=0;
     r.Top:=0;
     r:=GetRectBRLR(r);
     while r.Left<>-1 do begin
           New(pr);
           pr^:=r;
           Result.Add(pr);
           fBitmap.Canvas.FillRect(r);
           r:=GetRectBRLR(r);
           end;
     fBitmap.Free;
end;

function GetListRrectsTB(Bmp:TBitmap;BackColor:TColor):TList;
var
   r:TRect;
   pr:PRect;
   fBitmap:TBitmap;
begin
     Result:=TList.Create;
     fBitmap:=TBitmap.Create;
     fBitmap.Assign(Bmp);
     fBitmap.Mask(BackColor);
     fBitmap.Monochrome:=True;
     fBitmap.PixelFormat:=pf1bit;
     fBitmap.Canvas.Brush.Color:=clWhite;
     InitBitmap(fBitmap);
     r.Left:=0;
     r.Top:=0;
     r:=GetRectBRTB(r);
     while r.Left<>-1 do begin
           New(pr);
           pr^:=r;
           Result.Add(pr);
           fBitmap.Canvas.FillRect(r);
           r:=GetRectBRLR(r);
           end;
     fBitmap.Free;
end;

function GetRgnFromBmpLR(Bmp:TBitmap;BackColor:TColor;OffsetX,OffsetY:integer):THandle;
var
   j:integer;
   pr:PRect;
   pdr:PRGNDATA;
   Rects:TList;
begin
     Rects:=GetListRrectsLR(Bmp,BackColor);
     GetMem(pdr,SizeOf(TRgnDATA)+(Rects.Count*SizeOf(TRect)));
     pdr^.rdh.dwSize:=SizeOf(TRgnData);
     pdr^.rdh.iType:=RDH_RECTANGLES;
     pdr^.rdh.nCount:=Rects.Count;
     pdr^.rdh.nRgnSize:=SizeOf(TRect);
     pdr^.rdh.rcBound.Left:=0;
     pdr^.rdh.rcBound.Top:=0;
     pdr^.rdh.rcBound.Right:=Bmp.Width;
     pdr^.rdh.rcBound.Bottom:=Bmp.Height;
     pr:=@pdr^.Buffer;
     for j:=0 to Rects.Count-1 do begin
         pr^:=PRect(Rects.Items[j])^;
         pr^.Left:=pr^.Left+OffsetX;
         pr^.Right:=pr^.Right+OffsetX;
         pr^.Top:=pr^.Top+OffsetY;
         pr^.Bottom:=pr^.Bottom+OffsetY;
         inc(pr);
         end;
     Result:=ExtCreateRegion(nil,SizeOf(TRgnDATA)+(Rects.Count*SizeOf(TRect)),pdr^);
     FreeMem(pdr,SizeOf(TRgnDATA)+(Rects.Count*SizeOf(TRect)));
     while Rects.Count>0 do begin
           FreeMem(Rects.First,SizeOf(TRect));
           Rects.Delete(0);
           end;
     Rects.Free;
end;

function GetRgnFromBmpTB(Bmp:TBitmap;BackColor:TColor;OffsetX,OffsetY:integer):THandle;
var
   j:integer;
   pr:PRect;
   pdr:PRGNDATA;
   Rects:TList;
begin
     Rects:=GetListRrectsTB(Bmp,BackColor);
     GetMem(pdr,SizeOf(TRgnDATA)+(Rects.Count*SizeOf(TRect)));
     pdr^.rdh.dwSize:=SizeOf(TRgnData);
     pdr^.rdh.iType:=RDH_RECTANGLES;
     pdr^.rdh.nCount:=Rects.Count;
     pdr^.rdh.nRgnSize:=SizeOf(TRect);
     pdr^.rdh.rcBound.Left:=0;
     pdr^.rdh.rcBound.Top:=0;
     pdr^.rdh.rcBound.Right:=Bmp.Width;
     pdr^.rdh.rcBound.Bottom:=Bmp.Height;
     pr:=@pdr^.Buffer;
     for j:=0 to Rects.Count-1 do begin
         pr^:=PRect(Rects.Items[j])^;
         pr^.Left:=pr^.Left+OffsetX;
         pr^.Right:=pr^.Right+OffsetX;
         pr^.Top:=pr^.Top+OffsetY;
         pr^.Bottom:=pr^.Bottom+OffsetY;
         inc(pr);
         end;
     Result:=ExtCreateRegion(nil,SizeOf(TRgnDATA)+(Rects.Count*SizeOf(TRect)),pdr^);
     FreeMem(pdr,SizeOf(TRgnDATA)+(Rects.Count*SizeOf(TRect)));
     while Rects.Count>0 do begin
           FreeMem(Rects.First,SizeOf(TRect));
           Rects.Delete(0);
           end;
     Rects.Free;
end;



end.
