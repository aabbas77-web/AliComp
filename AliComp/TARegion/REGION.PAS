//--- beta --- beta --- beta --- beta --- beta --- beta --- beta --- beta ---
//
//                               TRegion
//
// This is an experimental component which encapsulates a windows region.  The
// specific aim is to avoid using the API calls, but use a TRegion instead.  The
// direct parallel is the use of a TCanvas rather than the API drawing calls.
//
// Regions can be used to clip the drawing to a form so that only drawing in
// prescribed regions can occur and also used to clip the form so that form can
// take on shapes other than rectangular.  It is the latter behaviour that this
// component currently provides.   Regions can be rectangular, elliptical,
// polygons and text.  You can test to see if given points are inside a region,
// fill a region with a brush and frame a region with a brush.  Regions when
// defined can be saved to disc and reloaded (which gives the possibility of
// defining the region and using the region in separate programs). Regions can
// be moved and scaled, and also with NT4 rotated and skewed.  To obtain some
// of the features (such as text) I have encapsulated the drawing path API calls
// in another class, called suprisingly TPath.  Many other shapes are
// possible by first using a path to create a region.  Other shapes include
// bezier curves and areas drawn by lines (as against a polyline or polygon),
// and also with NT4, arcs, chords and pies (and more).  Wherever possible I
// have used the TCanvas calling names, styles and parameter lists, so you can
// use Delphi help for ideas of what I am trying to do.
//
// The current version is Form coordinate based *not* Client coordinate based.
// So 0, 0 is the top left corner of the title bar.  You may need to convert
// client coordinates into form coordinates when hit testing (etc).
//
// Comments/Ideas:
//  1) error handling is limited, checking is through (I hope) but the
//     messages generated are not meaningful.
//  2) remove more of the NT4 dependencies by providing code in a win 95 version
//     which carries out the things present under NT4 - see TPath.Rectangle as
//     an (only) example.
//  3) no events included, but could have OnChanging and OnChanged (like TCanvas)
//     and some type of form hit testing (NCHIT) and since the title bar often
//     disappears, some provision for form dragging and resizing.
//  4) most unlikely to be thread-safe.  Needs locking like a TCanvas?
//
//------------------------------------------------------------------------------
//
// I am releasing this beta version for trial and comment.  I have no specific
// requirements of any one picking up and using this component.  But I really
// would *appreciate* feedback.
//
// Included with it is a demo program which shows-off most of it's features.
//
//------------------------------------------------------------------------------
//
// Note about Turbopower's Memory Sleuth: Memory Sleuth will report that
// region handles are not freed by this component.  I think the problem may
// be with Memory Sleuth and I have contacted Turbopower to tell them this.
// But TurboPower have taken little/no interest in this report.
// Specifically Memory Sleuth does not recognise the "handing over" of a
// region handle to the operating system by a call to SetWindowRgn, when you
// do this you must not then delete the handle (according to win32.hlp).
//
//------------------------------------------------------------------------------
//
// If you have comment or critism of this component, or wishes but no time
// to delve into what is required, then please contact me at:
//
//                   grahame.s.marsh@courtaulds.com
//
//--- beta --- beta --- beta --- beta --- beta --- beta --- beta --- beta ---

unit region;

interface

uses
  Windows, SysUtils, Classes, Graphics, Forms;

//=== TPath ====================================================================
// Encapsulates a windows drawing path.  Specfically written for use with the
// TRegion component and not tested for any other uses.  The create method
// obtains a DC for the whole form and drawing of the path takes place on that
// DC.  The path is begun, and is ended with a call to the EndPath function
// which returns a region handle.
//
// NB because a path holds a DC open you should minimise the time between
// BeginPath and EndPath. And ideally protect the call to EndPath in a try
// finally block (see TRegion.TextOut as an example) so as not to loose the
// DC due to an exception.

type
  TPath = class
    FForm : TForm;    // form for which the path will be made
    FHandle : hDC;    // drawing device context
    FFont : TFont;    // drawing font, supplied and not created locally
  protected
// passes through a non-zero number but raises an exception on zero (=error)
    function CheckPath (Value : integer): integer; virtual;
  public
// obtains a DC and begins drawing to it.
    constructor Create (Form: TForm; Font: TFont);
    destructor Destroy; override;
// begins drawing to a path (called by create)
    procedure BeginPath;
// draw a bezier curve based on a series of data and control points
    procedure Bezier (const points: array of TPoint);
// draw a bezier curve .... moving the drawing point to the end
    procedure BezierTo (const points: array of TPoint);
// closes any open shape draw so far
    procedure CloseFigure;
// ends drawing and returns a region handle (must be deleting by the caller)
    function  EndPath : hRgn;
// line from current position to a new position
    procedure LineTo (X, Y : integer);
// move drawing poistion to a new coordinate
    procedure MoveTo (X, Y : integer);
// draw a polygon based on a series of points
    procedure Polygon (const points: array of TPoint);
// draw a polyline based on a series of points
    procedure PolyLine (const points: array of TPoint);
// draw a polyline .... moving the drawing point to the end
    procedure PolyLineTo (const points: array of TPoint);
// draw a rectangle (win 95 implemented locally)
    procedure Rectangle (X1, Y1, X2, Y2 : integer);
// returns the size of the supplied text
    function  TextExtent (const Text: string): TSize;
// write text to the path
    procedure TextOut (X, Y: Integer; const Text: string);
// returns width of some text
    function  TextWidth (const Text: string): integer;
// read-only property handle
    property Handle : hDC read FHandle;
// read-only font handle - currently not used
//    property Font : TFont read FFont;
  end;

// *** not done ***
// TextHeight                 - lazy of me
// ExtTextOut                 - complex, later
// PolyPolygon PolyPolyLine   - don't see value

//=== TREGION ==================================================================

// convert region complexity constants into Delphiesque constants
const
  rgError   = RGN_ERROR;     // = 0
  rgEmpty   = NULLREGION;    // = 1
  rgSimple  = SIMPLEREGION;  // = 2
  rgComplex = COMPLEXREGION; // = 3

type
// Region combination modes, these are translated into the windows integer
// values by XlatCombineMode. Note duplication so rcAnd has same effect as
// rcIntersect; similarly rcDifference & rcSubtract; and rcUnion & rcOr.
  TRegionCombine  = (rcAnd, rcIntersect, rcCopy, rcDifference, rcSubtract,
                         rcOr, rcUnion, rcXor);
// Region fill modes see win32.hlp for explaination of ALTERNATE and WINDING
  TRegionFillMode = (fmAlternate, fmWinding);

  TRegion = class (TComponent)
  private
    FForm : TForm;                 // local copy of owner form
    FPath : TPath;                 // path (if it exists, otherwise nil)
    FUpdating : integer;           // used to prevent screen redraws when
                                   // making several changes (see BeginUpdate)
    FInvalidate,                   // if true causes form redraw on region move/xform
    FActive : boolean;             // true if region is in use
    FCombineMode : TRegionCombine; // region combining mode (see rcXXXXXX above)
    FRegion : hRgn;                // handle to an inactive region or zero
                                   // when active
    FBrush : TBrush;               // our brush
    FFont: TFont;                  // our font
    FFillMode : TRegionFillMode;   // region fill mode (see fmXXXXXX above)

    function  GetIsNotUpdating : boolean;
    procedure SetActive (Value: boolean);
  protected
// Passes through a valid region handle or complexity value but causes an
// Exception if zero passed (= RGN_ERROR)
    function  CheckRegion (Region : hRgn) : hRgn; virtual;
// Creates a small but valid dummy region in FRegion
    procedure CreateRegion; virtual;
// Combines the given Rgn into FRegion using FCombineMode, if the region is
// Currently active, ie in use then it is obtained from the form, combined and
// Then made active again. The Rgn handle is not deleted.
    procedure CombineHandles (Rgn : hRgn); virtual;
// Like CombinesHandles except the supplied Rgn is deleted after use
    procedure CombineDelete (Rgn : hRgn);
// Returns a pointer to a TRgnData structure and the size of the structure
// in Size.  It is the callers duty to use FreeMem (?Data?, Size) when done.
    function  GetRegionData (var Size: integer) : PRgnData; virtual;
// Causes the owner form to be wholey invalidated and redraw
    procedure FormInvalidate; virtual;
// Translates a rcXXXXX into a windows integer constant
    function  XlatCombineMode (Mode: TRegionCombine) : integer;
// Translates a fmXXXXX into a windows integer constant
    function  XlatFillMode (Mode: TRegionFillMode) : integer;
  public
    constructor Create (AOwner: TComponent); override;
    destructor Destroy; override;

// Make the region active; see Remove; equivalent to Active:=true;
    procedure Apply;
// Start a drawing path (see EndPath below)
    procedure BeginPath;
// Call when making multiple changes to an active path, this is more efficient
// than just calling the individual changes that you need.  Calls to BeginUpdate
// can be nested and each paired with an EndUpdate.  FUpdating counts the number
// of calls to BeginUpdate and only redraws the form when it returns to zero.
    procedure BeginUpdate;
// Returns the rectangle which bounds the current region
    function  Bounds : TRect;
// Clear the current region back to a full form.
    procedure Clear;
// Returns a copy of the current region (guaranteed inactive)
    function  Clone : TRegion;
// Combine the given TRegion with this one
    procedure Combine (Region : TRegion);
// Returns a code representing the complexity of the region (see rgXXXXX above)
    function  Complexity: integer;
// True if region contains the point (X, Y) - Form relative!
    function  ContainsXY (X, Y : integer) : boolean;
    function  ContainsPt (Pt : TPoint) : boolean;
// True if any part of Rect is in the region
    function  ContainsRect (Rect : TRect) : boolean;
// End the drawing path and combine the resultant Region - see BeginPath above
    procedure EndPath;
// End changes
    procedure EndUpdate;
// True if the supplied region has same size and shape
    function  Equal (Region : TRegion) : boolean;
// Draw an ellipse
    procedure Ellipse (X1, Y1, X2, Y2 : integer);
    procedure EllipseFromRect (Rect: TRect);
// Fill region with FBrush
    procedure Fill;
// Frame region with FBrush setting width and height of the brush strokes
    procedure Frame (Width, Height : integer);
// Cause the region to be repainted (I think)
    procedure Invalidate;
// Invert the colours in the region - effect depends on colour capabilities of
// the display adapter you are using
    procedure Invert;
// Load a region data file previously saved using SaveToFile
    procedure LoadFromFile (const Filename : string);
    procedure LoadFromStream (Stream : TStream);
// Move the region by dX, dY (may be negative for left and up)
    procedure Move (dX, dY : integer);
// Move to absolute top left coords
    procedure MoveTo (X, Y : integer);
// Draw a polygon based on a series on points
    procedure Polygon (const points: array of TPoint);
// Draw a rectangle
    procedure Rectangle (X1, Y1, X2, Y2 : integer);
    procedure RectangleFromRect (Rect: TRect);
// Makes a Region inactive, opposite of Apply, equivalent to Active:=false;
    procedure Remove;
// Draw a rectangle with rounded corners - see TCanvas.RoundRect
    procedure RoundRect (X1, Y1, X2, Y2, X3, Y3 : integer);
// Save a region data file to disc/stream
    procedure SaveToFile (const Filename : string);
    procedure SaveToStream (Stream : TStream);
// Create's a region based on the outline of a text string
    procedure TextOut (X, Y : integer; Text: string);
// Apply a transform matrix to a region, only scaling and move possible with win 95
    procedure Transform (XEffect, YEffect : single; dX, dY : integer);
    procedure TransformNT (XEffect, YEffect, XYEffect, YXEffect : single; dX, dY : integer);
    procedure TransformBy (const XForm : TXForm);
// Read only Region handle
    property Handle : hRgn read FRegion;
// Read only
    property IsNotUpdating : boolean read GetIsNotUpdating;
// NB Path is only valid between BeginPath and EndPath, otherwise nil
    property Path : TPath read FPath;
  published
    property Active : boolean read FActive write SetActive default false;
    property AutoInvalidate : boolean read FInvalidate write FInvalidate default true;
    property Brush : TBrush read FBrush write FBrush;
    property Font: TFont read FFont write FFont;
    property CombineMode : TRegionCombine read FCombineMode write FCombineMode default rcOr;
    property FillMode : TRegionFillMode read FFillMode write FFillMode default fmWinding;
  end;

// *** not done ***
//   PaintRgn                  - not needed?
//   CreatePolyPolygonRegion   - don't see the use

//------------------------------------------------------------------------------
// This is the identity transform matrix - when used it causes no changes
// to the region's coordinates.
const
  IdentityXForm : TXForm =
    (eM11: 1; eM12: 0; eM21: 0; eM22: 1; eDx : 0; eDy : 0);

implementation

//==============================================================================
// used to typecast a const array of TPoint into something the API will use

type
  PPoints = ^TPoints;
  TPoints = array[0..0] of TPoint;

var
  IsNT : boolean;  // simple point of reference for testing if NT is in use

//=== PATH METHODS =============================================================

constructor TPath.Create (Form: TForm; Font: TFont);
begin
  inherited Create;
  FForm := Form;
  FFont := Font;
  FHandle := GetWindowDC (FForm.Handle);
  BeginPath
end;

destructor TPath.Destroy;
begin
  ReleaseDC (FHandle, FForm.Handle);
  inherited Destroy
end;

procedure TPath.BeginPath;
begin
  Windows.BeginPath (FHandle)
end;

procedure TPath.Bezier (const points: array of TPoint);
begin
  Windows.PolyBezier (FHandle, PPoints(@Points)^, High(Points) + 1)
end;

procedure TPath.BezierTo (const points: array of TPoint);
begin
  Windows.PolyBezierTo (FHandle, PPoints(@Points)^, High(Points) + 1)
end;

// crude error handling
function TPath.CheckPath (Value: integer) : integer;
begin
  Result := Value;
  if Value = ERROR then
    raise exception.create ('path error')
end;

procedure TPath.CloseFigure;
begin
  Windows.CloseFigure (FHandle)
end;

function TPath.EndPath : hRgn;
begin
  Windows.EndPath (FHandle);
  Result := PathToRegion (FHandle)
end;

procedure TPath.LineTo (X, Y : integer);
begin
  Windows.LineTo (FHandle, X, Y);
end;

procedure TPath.MoveTo (X, Y : integer);
begin
  Windows.MoveToEx (FHandle, X, Y, nil)
end;

procedure TPath.Polygon (const points: array of TPoint);
begin
  Windows.Polygon (FHandle, PPoints(@Points)^, High(Points) + 1)
end;

procedure TPath.PolyLine (const points: array of TPoint);
begin
  Windows.PolyLine (FHandle, PPoints(@Points)^, High(Points) + 1)
end;

procedure TPath.PolyLineTo (const points: array of TPoint);
begin
  Windows.PolyLineTo (FHandle, PPoints(@Points)^, High(Points) + 1)
end;

// shows example of local implementation of a function not available under
// win 95 but available under NT4
procedure TPath.Rectangle (X1, Y1, X2, Y2 : integer);
var
  Pt : TPoint;
begin
  if IsNT then
    Windows.Rectangle (FHandle, X1, Y1, X2, Y2)
  else begin
    MoveToEx (FHandle, X1, Y1, @Pt);
    Windows.LineTo (FHandle, X1, Y2);
    Windows.LineTo (FHandle, X2, Y2);
    Windows.LineTo (FHandle, X2, Y1);
    Windows.LineTo (FHandle, X1, Y1);
    MoveToEx (FHandle, Pt.X, Pt.Y, nil)
  end
end;

procedure TPath.TextOut(X, Y: Integer; const Text: string);
begin
  SetBkMode (FHandle, TRANSPARENT);  // need outline only no fill
  SelectObject (FHandle, FFont.Handle);
  Windows.TextOut (FHandle, X, Y, PChar(Text), Length(Text));
  Windows.MoveToEx (FHAndle, X + TextWidth(Text), Y, nil)
end;

function TPath.TextExtent (const Text: string): TSize;
begin
  SelectObject (FHandle, FFont.Handle);
  Result.cX := 0;
  Result.cY := 0;
  Windows.GetTextExtentPoint (FHandle, PChar(Text), Length(Text), Result)
end;

function TPath.TextWidth (const Text: string): integer;
begin
  Result := TextExtent (Text).cX;
end;

//=== REGION METHODS ===========================================================

// crude error handling
procedure Error;
begin
  raise exception.create ('region error')
end;

constructor TRegion.Create (AOwner: TComponent);
begin
  inherited Create (AOwner);
  FCombineMode := rcUnion;         // default region combination
  FBrush := TBrush.Create;
  FFont := TFont.Create;
  FFillMode := fmWinding;          // default fill mode
  FForm := Owner as TForm;
  FInvalidate := true
end;

destructor TRegion.Destroy;
begin
  FFont.Free;
  FBrush.Free;
{  try                             // need to call clear when destroying, but
    Clear                          // form is destroyed first, problem
  except
  end; }

  inherited Destroy
end;

procedure TRegion.Apply;
begin
  Active := true
end;

procedure TRegion.BeginPath;
begin
  FPath := TPath.Create (Owner as TForm, FFont)
end;

procedure TRegion.BeginUpdate;
begin
  if FActive or (FUpdating > 0) then
  begin
    inc (FUpdating);
    Remove
  end
end;

(* alternative?
// The advantage of the Begin..EndUpdate method is it looks like fewer resources
// are used.  The advantage of the Clone method is that the region is not made
// inactive and therefore looks easier to make thread-safe.  This alternative
// way of obtaining a region handle should apply to many of the methods.

function TRegion.Bounds : TRect;
var
  Region : TRegion;
begin
  Region := Clone;   // clone obtains an inactive region
  try
    GetRgnBox (Region.FRegion, Result)
  finally
    Region.Free
  end
end; *)

function TRegion.Bounds : TRect;
begin
  BeginUpdate;  // guarantee inactive region
  try
    GetRgnBox (FRegion, Result)
  finally
    EndUpdate
  end
end;

// checks a region handle or other integer value or boolean, if RGN_ERROR (=0)
// then produces an exception by calling Error
function TRegion.CheckRegion (Region: hRgn) : hRgn;
begin
  Result := Region;
  if Region = RGN_ERROR then
    Error
end;

// Clears the region whether active by removing it or if not active by
// deleting it. Region handle is set to zero.
procedure TRegion.Clear;
begin
  if FActive then
    CheckRegion (integer(SetWindowRgn (FForm.Handle, 0, true)))
  else
    DeleteObject (FRegion);
  FUpdating := 0;
  FActive := false;
  FRegion := 0
end;

// Produces a copy of the TRegion and returns a new TRegion.  If active it
// simply uses GetWindowRgn if not active uses RGN_COPY
function TRegion.Clone : TRegion;
begin
  Result := TRegion.Create (Owner);
  Result.CreateRegion;
  if FActive then
    CheckRegion (integer(GetWindowRgn (FRegion, Result.FRegion)))
  else
    CheckRegion (CombineRgn (Result.FRegion, FRegion, 0, RGN_COPY))
end;

// Combines this TRegion with another TRegion modifying this TRegion.
procedure TRegion.Combine (Region : TRegion);
begin
  CombineHandles (Region.Handle)
end;

// Combine a new region into the existing region.  Need to check if currently
// active and if so make it inactive first (re-activating it later).  The supplied
// region handle is not deleted by this method.
procedure TRegion.CombineHandles (Rgn : hRgn);
begin
  if FActive then
  begin
    BeginUpdate;
    CheckRegion (CombineRgn (FRegion, FRegion, Rgn, XlatCombineMode(FCombineMode)));
    EndUpdate
  end else
    if FRegion = 0 then
    begin
      CreateRegion;
      CheckRegion (CombineRgn (FRegion, Rgn, 0, RGN_COPY))
    end else
      CheckRegion (CombineRgn (FRegion, FRegion, Rgn, XlatCombineMode(FCombineMode)));
end;

// Calls CombineHandles and then delete the supplied region handle (guaranteed)
procedure TRegion.CombineDelete (Rgn : hRgn);
begin
  try
    CombineHandles (Rgn)
  finally
    DeleteObject (Rgn)
  end
end;

// returns a code indicating the complexity of the region - see rgXXXXX codes above
function TRegion.Complexity: integer;
var
  Dummy : TRect;
begin
  BeginUpdate;
  try
    Result := GetRgnBox (FRegion, Dummy)
  finally
    EndUpdate
  end
end;

function TRegion.ContainsPt (Pt : TPoint) : boolean;
begin
  Result := ContainsXY (Pt.X, Pt.Y)
end; //see Bounds for alternative method using Clone

function TRegion.ContainsXY (X, Y : integer) : boolean;
begin
  BeginUpdate;
  try
    Result := PtInRegion (FRegion, X, Y)
  finally
    EndUpdate
  end
end; //see Bounds for alternative method using Clone

function TRegion.ContainsRect (Rect : TRect) : boolean;
begin
  BeginUpdate;
  try
    Result := RectInRegion (FRegion, Rect)
  finally
    EndUpdate
  end
end;

// Creates a small but valid region in the region handle.  Some function calls
// require the region to exist before replacing it.
procedure TRegion.CreateRegion;
begin
  CheckRegion (integer (FRegion = 0));
  FRegion := CheckRegion (CreateRectRgn (0, 0, 1, 1))
end;

// create an elliptical region based on X, Y coordinates of the top left corner
// and X, Y coordinates of bottom right corner of the bounding box
procedure TRegion.Ellipse (X1, Y1, X2, Y2 : integer);
begin
  CombineDelete (CheckRegion (CreateEllipticRgn (X1, Y1, X2, Y2)))
end;

procedure TRegion.EllipseFromRect (Rect: TRect);
begin
  CombineDelete (CheckRegion (CreateEllipticRgnIndirect (Rect)))
end;

procedure TRegion.EndPath;
begin
  try
    CombineDelete (CheckRegion (FPath.EndPath));
  finally
    FPath.Free;
    FPath := nil
  end
end;

procedure TRegion.EndUpdate;
begin
  if FUpdating <> 0 then
  begin
    dec (FUpdating);
    if FUpdating = 0 then
      Apply
  end
end;

function TRegion.Equal (Region : TRegion) : boolean;
begin
  BeginUpdate;
  try
    Result := EqualRgn (FRegion, Region.Handle)
  finally
    EndUpdate
  end
end;

procedure TRegion.Fill;
var
  DC : hDC;
  OldMode : integer;
begin
  DC := GetWindowDC (FForm.Handle);
  try
    OldMode := SetPolyFillMode (DC, XlatFillMode(FFillMode));
    try
      BeginUpdate;
      try
        CheckRegion (integer (FillRgn (DC, FRegion, FBrush.Handle)))
      finally
        EndUpdate
      end
    finally
      if OldMode <> 0 then
        SetPolyFillMode (DC, OldMode)
    end
  finally
    ReleaseDC (FForm.Handle, DC)
  end
end;

procedure TRegion.FormInvalidate;
begin
  if FInvalidate then
  begin
// need to invalidate controls as well?
    FForm.Invalidate
  end
end;

procedure TRegion.Frame (Width, Height : integer);
var
  DC : hDC;
begin
  DC := GetWindowDC (FForm.Handle);
  try
    BeginUpdate;
    try
      CheckRegion (integer (FrameRgn (DC, FRegion, FBrush.Handle, Width, Height)))
    finally
      EndUpdate
    end
  finally
    ReleaseDC (FForm.Handle, DC)
  end
end;

function TRegion.GetRegionData (var Size: integer) : PRgnData;
begin
  BeginUpdate;
  try
    Size := Windows.GetRegionData (FRegion, 0, nil);
    GetMem (Result, Size);
    try
      CheckRegion (Windows.GetRegionData (FRegion, Size, Result))
    except
      FreeMem (Result, Size);
      Size := 0;
      Result := nil
    end
  finally
    EndUpdate
  end
end;

function TRegion.GetIsNotUpdating : boolean;
begin
  Result := FUpdating = 0
end;

procedure TRegion.Invalidate;
begin
  InvalidateRgn (FForm.Handle, FRegion, true)
end;

procedure TRegion.Invert;
var
  DC : hDC;
begin
  DC := GetWindowDC (FForm.Handle);
  try
    BeginUpdate;
    try
      CheckRegion (integer (InvertRgn (DC, FRegion)))
    finally
      EndUpdate
    end
  finally
    ReleaseDC (FForm.Handle, DC)
  end;
end;

procedure TRegion.LoadFromFile (const Filename : string);
var
  Stream: TStream;
begin
  Stream := TFileStream.Create (Filename, fmOpenRead);
  try
    LoadFromStream (Stream)
  finally
    Stream.Free
  end
end;

procedure TRegion.LoadFromStream (Stream : TStream);
var
  Size: integer;
  Data: PRgnData;
begin
  Clear;
  Size := Stream.Size;
  GetMem (Data, Size);
  try
    Stream.Read (Data^, Size);
    FRegion := ExtCreateRegion (nil, Size, Data^);  // no transform on load
  finally
    FreeMem (Data, Size)
  end
end;

procedure TRegion.Move (dX, dY : integer);
begin
  BeginUpdate;
  try
    CheckRegion (OffSetRgn (FRegion, dX, dY));
  finally
    EndUpdate
  end;

  FormInvalidate
end;

procedure TRegion.MoveTo (X, Y : integer);
var
  R : TRect;
begin
  BeginUpdate;
  try
    R := Bounds;
    Move (X - R.Left, Y - R.Top)
  finally
    EndUpdate
  end;

  FormInvalidate
end;


procedure TRegion.Polygon (const points: array of TPoint);
begin
  CombineDelete (CheckRegion (CreatePolygonRgn (PPoints(@Points)^,
    High(Points) + 1, XlatFillMode (FFillMode))))
end;

procedure TRegion.Rectangle (X1, Y1, X2, Y2 : integer);
begin
  CombineDelete (CheckRegion (CreateRectRgn (X1, Y1, X2, Y2)))
end;

procedure TRegion.RectangleFromRect (Rect: TRect);
begin
  CombineDelete (CheckRegion (CreateRectRgnIndirect (Rect)))
end;

procedure TRegion.Remove;
begin
  Active := false
end;

procedure TRegion.RoundRect (X1, Y1, X2, Y2, X3, Y3 : integer);
begin
  CombineDelete (CheckRegion (CreateRoundRectRgn (X1, Y1, X2, Y2, X3, Y3)))
end;

procedure TRegion.SaveToFile (const Filename : string);
var
  Stream: TStream;
begin
  Stream := TFileStream.Create (Filename, fmCreate);
  try
    SaveToStream (Stream)
  finally
    Stream.Free
  end
end;

procedure TRegion.SaveToStream (Stream : TStream);
var
  Size: integer;
  Data: PRgnData;
begin
  Data := GetRegionData (Size);
  try
    Stream.Write (Data^, Size)
  finally
    FreeMem (Data, Size)
  end
end;

procedure TRegion.SetActive (Value: boolean);
begin
  if FActive <> Value then
  begin
    FActive := Value;
    if FActive then
    begin
// The region has just become active so put it's region handle into the form.
// Note the IsNotUpdating property is used to decide whether the form is repainted
// this avoids flicker when making changes.
      CheckRegion (integer(SetWindowRgn (FForm.Handle, FRegion, IsNotUpdating)));
      FRegion := 0 // default active/unused value
    end else begin
// The region has just become inactive so restore the region handle by copying
// the active region before destroying the form region, again IsNotUpdating
// is used to prevent redrawing and hence flicker
      CreateRegion;
      CheckRegion (integer(GetWindowRgn (FForm.Handle, FRegion)));
      CheckRegion (integer(SetWindowRgn (FForm.Handle, 0, IsNotUpdating)))
    end
  end
end;

// Text is only possible by using a path.  This method is a shortcut as it
// provides for the path begin and end and combining the text region.  This is
// Ok for one text string, but for multiple text strings you should begin the
// path "manually", do your stuff on the path (multiple strings or whatever) and
// then end the path.
procedure TRegion.TextOut (X, Y : integer; Text: string);
begin
  BeginPath;
  try
    Path.TextOut (X, Y, Text)
  finally
    EndPath
  end
end;

procedure TRegion.Transform (XEffect, YEffect : single; dX, dY : integer);
var
  XForm : TXForm;
begin
  with XForm do
  begin
    eM11:= XEffect;
    eM12:= 0;
    eM21:= 0;
    eM22:= YEffect;
    eDx := dX;
    eDy := dY
  end;
  TransformBy (XForm)
end;

procedure TRegion.TransformNT (XEffect, YEffect, XYEffect, YXEffect : single; dX, dY : integer);
var
  XForm : TXForm;
begin
  if not IsNT then  // crude fall over to prevent use of this transform
    Error;          // under win 95

  with XForm do
  begin
    eM11:= XEffect;
    eM12:= XYEffect;
    eM21:= YXEffect;
    eM22:= YEffect;
    eDx := dX;
    eDy := dY
  end;
  TransformBy (XForm)
end;

procedure TRegion.TransformBy (const XForm : TXForm);
var
  Data : PRgnData;
  Size : integer;
begin
  BeginUpdate;
  try
    Data := GetRegionData (Size);
    if Size <> 0 then
      try
        DeleteObject (FRegion);
        FRegion := ExtCreateRegion (@XForm, Size, Data^)
      finally
        FreeMem (Data, Size)
      end
  finally
    EndUpdate
  end;

  FormInvalidate
end;

// Combine regions translation - note the multiple definitions to offer
// different identifiers eg  rcOr = rcUnion
function TRegion.XlatCombineMode (Mode: TRegionCombine) : integer;
const
  Modes : array [TRegionCombine] of integer =
    (RGN_AND, RGN_AND, RGN_COPY, RGN_DIFF, RGN_DIFF, RGN_OR, RGN_OR, RGN_XOR);
begin
  Result := Modes [Mode]
end;

function TRegion.XlatFillMode (Mode: TRegionFillMode) : integer;
const
  Modes : array [TRegionFillMode] of integer =
    (ALTERNATE, WINDING);
begin
  Result := Modes [Mode]
end;

initialization
  IsNT := Win32Platform = VER_PLATFORM_WIN32_NT
end.

//--- beta --- beta --- beta --- beta --- beta --- beta --- beta --- beta ---

