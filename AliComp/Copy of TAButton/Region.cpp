//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "Region.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TRegion::TRegion(HWND hWindow)
{
 hWND=hWindow;
 IsNT=(Win32Platform == VER_PLATFORM_WIN32_NT);
 FBrush=new TBrush();
 FFrameBrush=new TBrush();
 Canvas=new TCanvas();
 Canvas->Handle=GetWindowDC(hWND);
// FHandle=NULL;
// CreateRegion();
// FHandle=NULL;
// Rectangle(0,0,1,1);
// GetWindowRgn(hWND,FHandle);
 FActive=false;
}
//---------------------------------------------------------------------------
TRegion::~TRegion()
{
 Clear();
 if(FBrush)
  delete FBrush;
 if(FFrameBrush)
  delete FFrameBrush; 
 if(Canvas)
  delete Canvas; 
}
//---------------------------------------------------------------------------
void TRegion::CreateRegion()
{
 Clear();
 FHandle=CheckRegion(CreateRectRgn(0, 0, 1, 1));
}
//---------------------------------------------------------------------------
void TRegion::SetHandle(HRGN Value)
{
 if(!FHandle)
  CreateRegion();
 CheckRegion((HRGN)CombineRgn(FHandle, Value, 0, RGN_COPY));
}
//---------------------------------------------------------------------------
HRGN TRegion::GetHandle()
{
 HRGN hRGN;
 hRGN=CreateRectRgn(0, 0, 1, 1);
 CombineRgn(hRGN, FHandle, 0, RGN_COPY);
 return hRGN;
}
//---------------------------------------------------------------------------
HRGN TRegion::CheckRegion(HRGN Region)
{
 if(Region == NULL)
  Error();
 return Region;
}
//---------------------------------------------------------------------------
void TRegion::Error()
{
 throw Exception("Unknown Error ...");
}
//---------------------------------------------------------------------------
RGNDATA *TRegion::GetData(DWORD &Size)
{
 RGNDATA *Temp;
 Size=GetRegionData(FHandle, 0, NULL);
 Temp=(RGNDATA *)malloc(Size);
 try
  {
   CheckRegion((HRGN)GetRegionData(FHandle, Size, Temp));
  }
 catch(...)
  {
   free(Temp);
   Size=0;
   Temp=NULL;
  }
 return Temp;
}
//---------------------------------------------------------------------------
void TRegion::SaveToStream(TStream *Stream)
{
 DWORD Size;
 RGNDATA *Data;
 Data=GetData(Size);
 try
  {
   Stream->Write(Data,Size);
  }
 __finally
  {
   free(Data);
  }
}
//---------------------------------------------------------------------------
void TRegion::SaveToFile(AnsiString FileName)
{
 TStream *Stream;
 Stream=new TFileStream(FileName, fmCreate);
 try
  {
   SaveToStream(Stream);
  }
 __finally
  {
   if(Stream)
    delete Stream;
  }
}
//---------------------------------------------------------------------------
void TRegion::LoadFromStream(TStream *Stream)
{
 int Size;
 RGNDATA *Data;
 Clear();
 Size=Stream->Size;
 Data=(RGNDATA *)malloc(Size);
 try
  {
   Stream->Read(Data,Size);
   FHandle=CheckRegion(ExtCreateRegion(NULL, Size, Data));
  }
 __finally
  {
   free(Data);
  }
}
//---------------------------------------------------------------------------
void TRegion::LoadFromFile(AnsiString FileName)
{
 TStream *Stream;
 Stream=new TFileStream(FileName, fmOpenRead);
 try
  {
   LoadFromStream(Stream);
  }
 __finally
  {
   if(Stream)
    delete Stream;
  }
}
//---------------------------------------------------------------------------
// Combine regions translation - note the multiple definitions to offer
// different identifiers eg  rcOr = rcUnion
int TRegion::XlatCombineMode(TRegionCombine Mode)
{
 int Modes[8]={RGN_AND, RGN_AND, RGN_COPY, RGN_DIFF, RGN_DIFF, RGN_OR, RGN_OR, RGN_XOR};
 return Modes[Mode];
}
//---------------------------------------------------------------------------
int TRegion::XlatFillMode(TRegionFillMode Mode)
{
 int Modes[2]={ALTERNATE, WINDING};
 return Modes[Mode];
}
//---------------------------------------------------------------------------
void TRegion::CombineWith(HRGN hRgn,TRegionCombine CombineMode)
{
 if(FHandle == NULL)
  {
   SetHandle(hRgn);
  }
 else
  {
   CheckRegion((HRGN)CombineRgn(FHandle, FHandle, hRgn, XlatCombineMode(CombineMode)));
  }
}
//---------------------------------------------------------------------------
void TRegion::Fill(TRegionFillMode FillMode)
{
 CheckRegion((HRGN)FillRgn(Canvas->Handle, FHandle, FBrush->Handle));
}
//---------------------------------------------------------------------------
TRect TRegion::Bounds()
{
 TRect Temp;
 GetRgnBox(FHandle, &Temp);
 return Temp;
}
//---------------------------------------------------------------------------
// deleting it. Region handle is set to zero.
void TRegion::Clear()
{
 DeleteObject(FHandle);
 FHandle=NULL;
}
//---------------------------------------------------------------------------
// returns a code indicating the complexity of the region - see rgXXXXX codes above
int TRegion::Complexity()
{
 TRect Dummy;
 return GetRgnBox(FHandle, &Dummy);
}
//---------------------------------------------------------------------------
bool TRegion::ContainsPoint(int X,int Y)
{
 return PtInRegion(FHandle, X, Y);
} //see Bounds for alternative method using Clone
//---------------------------------------------------------------------------
bool TRegion::ContainsPoint(TPoint P)
{
 return ContainsPoint(P.x, P.y);
} //see Bounds for alternative method using Clone
//---------------------------------------------------------------------------
bool TRegion::ContainsRect(TRect Rect)
{
 return RectInRegion(FHandle, &Rect);
}
//---------------------------------------------------------------------------
// create an elliptical region based on X, Y coordinates of the top left corner
// and X, Y coordinates of bottom right corner of the bounding box
void TRegion::Ellipse(int X1,int Y1,int X2,int Y2)
{
 Clear();
 FHandle=CheckRegion(CreateEllipticRgn(X1, Y1, X2, Y2));
}
//---------------------------------------------------------------------------
void TRegion::Ellipse(TRect Rect)
{
 Clear();
 FHandle=CheckRegion(CreateEllipticRgnIndirect(&Rect));
}
//---------------------------------------------------------------------------
bool TRegion::Equal(TRegion *Region)
{
 return EqualRgn(FHandle, Region->Handle);
}
//---------------------------------------------------------------------------
void TRegion::Move(int dX,int dY)
{
 CheckRegion((HRGN)OffsetRgn(FHandle, dX, dY));
}
//---------------------------------------------------------------------------
void TRegion::MoveTo(int X,int Y)
{
 TRect R;
 R=Bounds();
 Move(X-R.Left, Y-R.Top);
}
//---------------------------------------------------------------------------
void TRegion::Polygon(const TPoint *Points,int Nb,TRegionFillMode FillMode)
{
 Clear();
 FHandle=CheckRegion(CreatePolygonRgn(Points,Nb+1,XlatFillMode(FillMode)));
}
//---------------------------------------------------------------------------
void TRegion::Rectangle(int X1,int Y1,int X2,int Y2)
{
 Clear();
 FHandle=CheckRegion(CreateRectRgn(X1, Y1, X2, Y2));
}
//---------------------------------------------------------------------------
void TRegion::Rectangle(TRect Rect)
{
 Clear();
 FHandle=CheckRegion(CreateRectRgnIndirect(&Rect));
}
//---------------------------------------------------------------------------
void TRegion::RoundRect(int X1,int Y1,int X2,int Y2,int X3,int Y3)
{
 Clear();
 FHandle=CheckRegion(CreateRoundRectRgn(X1, Y1, X2, Y2, X3, Y3));
}
//---------------------------------------------------------------------------
void TRegion::TransformBy(const TXForm XForm)
{
 RGNDATA *Data;
 DWORD Size;
 Data=GetData(Size);
 if(Size != 0)
  {
   try
    {
     Clear();
     FHandle=ExtCreateRegion(&XForm, Size, Data);
    }
   __finally
    {
     free(Data);
    }
  }
}
//---------------------------------------------------------------------------
void TRegion::Transform(Single XEffect,Single YEffect,int dX,int dY)
{
 TXForm XForm;
 XForm.eM11=XEffect;
 XForm.eM12=0;
 XForm.eM21=0;
 XForm.eM22=YEffect;
 XForm.eDx=dX;
 XForm.eDy=dY;
 TransformBy(XForm);
}
//---------------------------------------------------------------------------
void TRegion::TransformNT(Single XEffect,Single YEffect,Single XYEffect,
                          Single YXEffect,int dX,int dY)
{
 TXForm XForm;
 if(!IsNT) // crude fall over to prevent use of this transform
  Error(); // under win 95

 XForm.eM11=XEffect;
 XForm.eM12=XYEffect;
 XForm.eM21=YXEffect;
 XForm.eM22=YEffect;
 XForm.eDx=dX;
 XForm.eDy=dY;
 TransformBy(XForm);
}
//---------------------------------------------------------------------------
void TRegion::SetBrush(TBrush *Value)
{
 FBrush->Assign(Value);
}
//---------------------------------------------------------------------------
void TRegion::SetFrameBrush(TBrush *Value)
{
 FFrameBrush->Assign(Value);
}
//---------------------------------------------------------------------------
void TRegion::Paint()
{
 Canvas->Brush=FBrush;
 CheckRegion((HRGN)PaintRgn(Canvas->Handle,FHandle));
}
//---------------------------------------------------------------------------
void TRegion::Frame(int FrameWidth, int FrameHeight)
{
 CheckRegion((HRGN)FrameRgn(Canvas->Handle, FHandle, FrameBrush->Handle, FrameWidth, FrameHeight));
}
//---------------------------------------------------------------------------
void TRegion::SetActive(bool Value)
{
 FActive=Value;
 if(FActive)
  {
   SetWindowRgn(hWND, Handle, true);
  }
 else
  {
   SetWindowRgn(hWND, 0, true);
  }
}
//---------------------------------------------------------------------------
void TRegion::LoadFromBMP(Graphics::TBitmap *Mask,TColor BackColor)
{
 TList *List=new TList();
 Graphics::TBitmap *BMP=new Graphics::TBitmap();
 BMP->Assign(Mask);
 BMP->Mask(BackColor);
 BMP->Monochrome=true;
 BMP->PixelFormat=pf32bit;
 TColor *PColor;
 for(int Y=0;Y<BMP->Height;Y++)
  {
   PColor=(TColor *)BMP->ScanLine[Y];
   for(int X=0;X<BMP->Width;)
    {
     if(PColor[X]==clBlack)
      {
       TRect *R=new TRect();
       R->Left=X-1;
       while(PColor[X]==clBlack)
        {
         X++;
         if(X>=BMP->Width)
          break;
        }
       R->Right=X+1;
       R->Top=Y-1;
       R->Bottom=Y+1;

       List->Add(R);
      }
     else
      {
       X++;
      }
    }
  }

 RGNDATA *Data;
 DWORD Size;
 Size=sizeof(RGNDATA)+(List->Count*sizeof(TRect));
 Data=(RGNDATA *)malloc(Size);
 Data->rdh.dwSize=sizeof(RGNDATA);
 Data->rdh.iType=RDH_RECTANGLES;
 Data->rdh.nCount=List->Count;
 Data->rdh.nRgnSize=sizeof(TRect);
 Data->rdh.rcBound.left=0;
 Data->rdh.rcBound.top=0;
 Data->rdh.rcBound.right=BMP->Width;
 Data->rdh.rcBound.bottom=BMP->Height;
 TRect *PRect;
 PRect=(TRect *)Data->Buffer;
 for(int i=0;i<List->Count;i++)
  {
   *PRect=*(TRect *)List->Items[i];
   PRect++;
  }

 Clear(); 
 FHandle=CheckRegion(ExtCreateRegion(NULL,sizeof(RGNDATA)+(List->Count*sizeof(TRect)),Data));

 if(List)
  {
   for(int i=0;i<List->Count;i++)
    delete (TRect *)List->Items[i];
   delete List;
  } 
 if(BMP)
  delete BMP;
}
//---------------------------------------------------------------------------

