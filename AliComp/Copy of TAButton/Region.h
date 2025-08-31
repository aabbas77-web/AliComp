//---------------------------------------------------------------------------
#ifndef RegionH
#define RegionH
//---------------------------------------------------------------------------
// convert region complexity constants into Delphiesque constants
const int rgError   = RGN_ERROR;     // = 0
const int rgEmpty   = NULLREGION;    // = 1
const int rgSimple  = SIMPLEREGION;  // = 2
const int rgComplex = COMPLEXREGION; // = 3
//---------------------------------------------------------------------------
// Region combination modes, these are translated into the windows integer
// values by XlatCombineMode. Note duplication so rcAnd has same effect as
// rcIntersect; similarly rcDifference & rcSubtract; and rcUnion & rcOr.
typedef enum {rcAnd, rcIntersect, rcCopy, rcDifference, rcSubtract, rcOr,
              rcUnion, rcXor} TRegionCombine;
//---------------------------------------------------------------------------
// Region fill modes see win32.hlp for explaination of ALTERNATE and WINDING
typedef enum {fmAlternate, fmWinding} TRegionFillMode;
//---------------------------------------------------------------------------
class TRegion
{
private:
        HRGN FHandle;// handle to an inactive region or zero when active.
        bool IsNT;
        TBrush *FBrush;
        TBrush *FFrameBrush;
        TCanvas *Canvas;
        bool FActive;
        HWND hWND;

        int XlatCombineMode(TRegionCombine Mode);
        int XlatFillMode(TRegionFillMode Mode);
        void SetHandle(HRGN Value);
        HRGN GetHandle();
        void SetBrush(TBrush *Value);
        void SetFrameBrush(TBrush *Value);
        void SetActive(bool Value);
public:
        TRegion(HWND hWindow);
        ~TRegion();

        void CreateRegion();
        HRGN CheckRegion(HRGN Region);
        RGNDATA *GetData(DWORD &Size);
        void Error();
        void SaveToFile(AnsiString FileName);
        void SaveToStream(TStream *Stream);
        void LoadFromFile(AnsiString FileName);
        void LoadFromStream(TStream *Stream);
        void CombineWith(HRGN hRgn,TRegionCombine CombineMode);
        TRect Bounds();
        void Clear();
        int Complexity();
        bool ContainsPoint(int X,int Y);
        bool ContainsPoint(TPoint P);
        bool ContainsRect(TRect Rect);
        void Ellipse(int X1,int Y1,int X2,int Y2);
        void Ellipse(TRect Rect);
        bool Equal(TRegion *Region);
        void Move(int dX,int dY);
        void MoveTo(int X,int Y);
        void Polygon(const TPoint *Points,int Nb,TRegionFillMode FillMode);
        void Rectangle(int X1,int Y1,int X2,int Y2);
        void Rectangle(TRect Rect);
        void RoundRect(int X1,int Y1,int X2,int Y2,int X3,int Y3);
        void TransformBy(const TXForm XForm);
        void Transform(Single XEffect,Single YEffect,int dX,int dY);
        void TransformNT(Single XEffect,Single YEffect,Single XYEffect,
                         Single YXEffect,int dX,int dY);
        void Paint();
        void Frame(int FrameWidth, int FrameHeight);
        void Fill(TRegionFillMode FillMode);
        void LoadFromBMP(Graphics::TBitmap *Mask,TColor BackColor);

        __property HRGN Handle={read=GetHandle,write=SetHandle};
        __property TBrush *Brush={read=FBrush,write=SetBrush};
        __property TBrush *FrameBrush={read=FFrameBrush,write=SetFrameBrush};
        __property bool Active={read=FActive,write=SetActive};
};
//---------------------------------------------------------------------------
#endif
