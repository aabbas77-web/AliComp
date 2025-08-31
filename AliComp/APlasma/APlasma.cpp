//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "APlasma.h"
#include "Math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TAPlasma *)
{
        new TAPlasma(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAPlasma::TAPlasma(TComponent* Owner)
        : TGraphicControl(Owner)
{
 FTimer=new TTimer(this);
 FTimer->Interval=100;
 FTimer->Enabled=false;
 FTimer->OnTimer=DoTimer;

 Width=64;
 Height=64;
 BackBuffer=new Graphics::TBitmap();
 BackBuffer->Width=Width;
 BackBuffer->Height=Height;
 BackBuffer->PixelFormat=pf32bit;
 BackBuffer->IgnorePalette=true;
 for(int x=0;x<256;x++)
  PlasmaTable[x]=30.0*(1.0+sin(x*2.0*3.1415/256.0));
 IsFirst=true; 
}
//---------------------------------------------------------------------------
__fastcall TAPlasma::~TAPlasma()
{
 if(FTimer)
  delete FTimer;
 if(BackBuffer)
  delete BackBuffer;
}
//---------------------------------------------------------------------------
namespace Aplasma
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TAPlasma)};
                 RegisterComponents("AliSoft", classes, 0);
        }
}
//---------------------------------------------------------------------------
void __fastcall TAPlasma::Paint()
{
 if(FActive)
  return;
 if(IsFirst)
  DoTimer(this);
 IsFirst=false; 
 Canvas->Draw (0, 0, BackBuffer);
}
//---------------------------------------------------------------------------
void __fastcall TAPlasma::DoTimer(TObject *Sender)
{
 if(FOnTimer)
  FOnTimer(Sender);
 // If the user resizes the windows, we must accommodate the size of the
 //     back buffer.
 BackBuffer->Width = Width;
 BackBuffer->Height = Height;
 // These variables holds the plasma direction and aspect.
 static Byte PlasmaDir1,PlasmaDir2,PlasmaDir3,PlasmaDir4;
 // These variables holds the old values of the plasma position.
 static Byte PlasmaPos1=0,PlasmaPos2=0,PlasmaPos3=0,PlasmaPos4=0;
 static unsigned int  PlasmaColor,   // Final plasma color
                      *LinePtr;      // Pointer to a buffer's row of pixels
 static int           x,y;

 PlasmaDir1 = PlasmaPos1;
 PlasmaDir2 = PlasmaPos2;

 // We scan the entire bitmap, row by row
 for (y=0; y<Height; y++)
    {
      // We now obtain a pointer to the start of the current row
      LinePtr = (unsigned int *) BackBuffer->ScanLine[y];

      PlasmaDir3 = PlasmaPos3;
      PlasmaDir4 = PlasmaPos4;

      for(x=0; x<Width; x++)
      {
        // Using the Plasma Table, we obtain the color for the given
        // pixel of the row. Try to change this line of code. 
        PlasmaColor = PlasmaTable[PlasmaDir1] + PlasmaTable[PlasmaDir2] +
                      PlasmaTable[PlasmaDir3] + PlasmaTable[PlasmaDir4];

        // Check for the color chosen in the radio group.
        switch (FColorComponent)
         {
          case ccRed:
                // Only the red component. (green = blue = 0)
                LinePtr [x] = (PlasmaColor<<16);
                break;
          case ccGreen:
                // Only the green component. (red = blue = 0)
                LinePtr [x] = (PlasmaColor<<8);
                break;
          case ccBlue:
                // Only the blue component. (red = green = 0)
                LinePtr [x] = (PlasmaColor);
                break;
          case ccMix:
          default:
                // This is a mix of colours. Try to changes the values,
                // and remember that each color component is big as 1
                // Byte.
                LinePtr [x] = (((255-PlasmaColor)<<16) |  //Red
                                (PlasmaColor<<8)       |  //Green
                                (128+(PlasmaColor>>1)));  //Blue
                break;
         }

        PlasmaDir3 += (Byte) 1;
        PlasmaDir4 += (Byte) 2;
      }

      PlasmaDir1 += (Byte) 2;
      PlasmaDir2 += (Byte) 1;
    }

    // This section controls the plasma speed. Try to change these
    // values as you like.
    PlasmaPos1 += (Byte) 2;
    PlasmaPos2 -= (Byte) 4;
    PlasmaPos3 += (Byte) 6;
    PlasmaPos4 -= (Byte) 8;

    // Finally, the back buffer is copied into the Canvas's Form
    Canvas->Draw (0, 0, BackBuffer);
}
//---------------------------------------------------------------------------
void __fastcall TAPlasma::SetColorComponent(TColorComponent Value)
{
 if(FColorComponent!=Value)
  {
   FColorComponent=Value;
   Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TAPlasma::SetActive(bool Value)
{
 if(FActive!=Value)
  {
   FActive=Value;
   Invalidate();
   FTimer->Enabled=FActive;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAPlasma::WndProc(TMessage &Message)
{
 switch(Message.Msg)
  {
   case WM_PAINT:
    {
     break;
    }
   case WM_ENTERSIZEMOVE:
    {
     FTimer->Enabled=false;
     break;
    }
   case WM_EXITSIZEMOVE:
    {
     FTimer->Enabled=true;
     break;
    }
   case WM_ERASEBKGND:
    {
     return;
    }
   case WM_SIZE:
    {
     break;
    }
   case WM_SIZING:
    {
     break;
    }
  }
 TGraphicControl::WndProc(Message);
}
//---------------------------------------------------------------------------

