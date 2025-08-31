//---------------------------------------------------------------------------
#ifndef AGifH
#define AGifH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#include <vector>
#include <Dsgnintf.hpp>
//---------------------------------------------------------------------------
//---------------------TFileNameProperty-------------------------------------
//---------------------------------------------------------------------------
class PACKAGE TFileNameProperty : public TStringProperty
{
public:
    __fastcall TFileNameProperty(const _di_IFormDesigner ADesigner, int APropCount)
                                    :TStringProperty(ADesigner,APropCount){};
    __fastcall ~TFileNameProperty(){};
    virtual TPropertyAttributes __fastcall GetAttributes();
    virtual void __fastcall Edit();
};
//---------------------------------------------------------------------------
//------------------------TAGif----------------------------------------------
//---------------------------------------------------------------------------
struct TFrame    // structure that keeps a single frame info
{
	IPicture *m_pPicture;  // pointer to the interface used for drawing
	SIZE     m_frameSize;
	SIZE     m_frameOffset;
	UINT     m_nDelay;     // delay (in 1/100s of a second)
	UINT     m_nDisposal;  // disposal method
};
//---------------------------------------------------------------------------
#pragma pack(1)   // turn byte alignment on
//---------------------------------------------------------------------------
enum GIFBlockTypes
{
	BLOCK_UNKNOWN,
	BLOCK_APPEXT,
	BLOCK_COMMEXT,
	BLOCK_CONTROLEXT,
	BLOCK_PLAINTEXT,
	BLOCK_IMAGE,
	BLOCK_TRAILER
};
//---------------------------------------------------------------------------
enum ControlExtValues // graphic control extension packed field values
{
	GCX_PACKED_DISPOSAL,  // disposal method
	GCX_PACKED_USERINPUT,
	GCX_PACKED_TRANSPCOLOR
};
//---------------------------------------------------------------------------
enum LSDPackedValues  // logical screen descriptor packed field values
{
	LSD_PACKED_GLOBALCT,
	LSD_PACKED_CRESOLUTION,
	LSD_PACKED_SORT,
	LSD_PACKED_GLOBALCTSIZE
};
//---------------------------------------------------------------------------
enum IDPackedValues   // image descriptor packed field values
{
	ID_PACKED_LOCALCT,
	ID_PACKED_INTERLACE,
	ID_PACKED_SORT,
	ID_PACKED_LOCALCTSIZE
};
//---------------------------------------------------------------------------
struct TGIFHeader       // GIF header
{
	char m_cSignature[3]; // Signature - Identifies the GIF Data Stream
						  // This field contains the fixed value 'GIF'
	char m_cVersion[3];	// Version number. May be one of the following:
						// "87a" or "89a"
};
//---------------------------------------------------------------------------
struct TGIFLSDescriptor // Logical Screen Descriptor
{
	WORD m_wWidth;	// 2 bytes. Logical screen width
	WORD m_wHeight; // 2 bytes. Logical screen height

	unsigned char m_cPacked;      // packed field

	unsigned char m_cBkIndex;     // 1 byte. Background color index
	unsigned char m_cPixelAspect; // 1 byte. Pixel aspect ratio
	inline int GetPackedValue(enum LSDPackedValues Value);
};
//---------------------------------------------------------------------------
struct TGIFAppExtension // application extension block
{
	unsigned char m_cExtIntroducer; // extension introducer (0x21)
	unsigned char m_cExtLabel; // app. extension label (0xFF)
	unsigned char m_cBlockSize; // fixed value of 11
	char m_cAppIdentifier[8];   // application identifier
	char m_cAppAuth[3];  // application authentication code
};
//---------------------------------------------------------------------------
struct TGIFControlExt // graphic control extension block
{
	unsigned char m_cExtIntroducer; // extension introducer (0x21)
	unsigned char m_cControlLabel;  // control extension label (0xF9)
	unsigned char m_cBlockSize; // fixed value of 4
	unsigned char m_cPacked;    // packed field
	WORD m_wDelayTime;	// delay time
	unsigned char m_cTColorIndex; // transparent color index
	unsigned char m_cBlockTerm;   // block terminator (0x00)
public:
	inline int GetPackedValue(enum ControlExtValues Value);
};
//---------------------------------------------------------------------------
struct TGIFCommentExt  // comment extension block
{
	unsigned char m_cExtIntroducer; // extension introducer (0x21)
	unsigned char m_cCommentLabel;  // comment extension label (0xFE)
};
//---------------------------------------------------------------------------
struct TGIFPlainTextExt // plain text extension block
{
	unsigned char m_cExtIntroducer;  // extension introducer (0x21)
	unsigned char m_cPlainTextLabel; // text extension label (0x01)
	unsigned char m_cBlockSize; // fixed value of 12
	WORD m_wLeftPos;    // text grid left position
	WORD m_wTopPos;     // text grid top position
	WORD m_wGridWidth;  // text grid width
	WORD m_wGridHeight; // text grid height
	unsigned char m_cCellWidth;  // character cell width
	unsigned char m_cCellHeight; // character cell height
	unsigned char m_cFgColor; // text foreground color index
	unsigned char m_cBkColor; // text background color index
};
//---------------------------------------------------------------------------
struct TGIFImageDescriptor // image descriptor block
{
	unsigned char m_cImageSeparator; // image separator byte (0x2C)
	WORD m_wLeftPos; // image left position
	WORD m_wTopPos;  // image top position
	WORD m_wWidth;   // image width
	WORD m_wHeight;  // image height
	unsigned char m_cPacked; // packed field
	inline int GetPackedValue(enum IDPackedValues Value);
};
//---------------------------------------------------------------------------
#pragma pack() // turn byte alignment off
//---------------------------------------------------------------------------
class PACKAGE TAGif : public TGraphicControl
{
private:
        SIZE m_PictureSize;
        COLORREF m_clrBackground;
        UINT m_nDataSize;
        UINT m_nCurrOffset;
        UINT m_nGlobalCTSize;
        BOOL m_bIsGIF;
        BOOL m_bExitThread;
        BOOL m_bIsInitialized;
        HDC m_hMemDC;
        HBITMAP m_hBitmap;
        HBITMAP m_hOldBitmap;
        HANDLE m_hThread;
        HANDLE m_hExitEvent;
        IPicture * m_pPicture;
        TGIFHeader * m_pGIFHeader;
        unsigned char * m_pRawData;
        TGIFLSDescriptor * m_pGIFLSDescriptor;
        std::vector<::TFrame> m_arrFrames;
private:
        AnsiString FFileName;
        bool FAnimate;
        void __fastcall SetAnimate(bool Value);
        void __fastcall SetFileName(AnsiString Value);
protected:
        void __fastcall Paint();
public:
        __fastcall TAGif(TComponent* Owner);
        __fastcall ~TAGif();
        bool Load(AnsiString FileName);
        bool Load(HGLOBAL hGlobal, DWORD dwSize);
        void UnLoad();
        void ResetDataPointer();
        bool SkipNextGraphicBlock();
        enum GIFBlockTypes GetNextBlock() const;
        bool SkipNextBlock();
        int GetNextBlockLen() const;
        UINT GetSubBlocksLen(UINT nStartingOffset) const;
        bool Draw();
        SIZE GetSize() const;
        void Stop();
        bool IsGIF() const;
        bool IsAnimatedGIF() const;
        int GetFrameCount() const;
        COLORREF GetBkColor() const;
        void SetBkColor(COLORREF clr);
        HGLOBAL GetNextGraphicBlock(UINT *pBlockLen,
	  UINT *pDelay, SIZE *pBlockSize, SIZE *pBlockOffset,
	  UINT *pDisposal);
        bool PrepareDC(int nWidth, int nHeight);
        void ThreadAnimation();
        static UINT WINAPI _ThreadAnimation(LPVOID pParam);
__published:
        __property AnsiString FileName={read=FFileName,write=SetFileName};
};
//---------------------------------------------------------------------------
#endif
 