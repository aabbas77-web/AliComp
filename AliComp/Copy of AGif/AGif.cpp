//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include <math.h>
#include <process.h>
#include <assert.h>
#include <ole2.h>
#include <olectl.h>
#include <stdio.h>
//---------------------------------------------------------------------------
#include "AGif.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TAGif *)
{
        new TAGif(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAGif::TAGif(TComponent* Owner)
        : TGraphicControl(Owner)
{
}
//---------------------------------------------------------------------------
__fastcall TAGif::~TAGif()
{
}
//---------------------------------------------------------------------------
namespace Agif
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TAGif)};
                 RegisterComponents("AliSoft", classes, 0);

                 TPropInfo* PropInfo=::GetPropInfo(__typeinfo(TAGif),"FileName");
                 RegisterPropertyEditor(*PropInfo->PropType,
                                        __classid(TAGif),"FileName",
                                        __classid(TFileNameProperty));
        }
}
//---------------------------------------------------------------------------
//----------------------TGIFControlExt---------------------------------------
//---------------------------------------------------------------------------
inline int TGIFControlExt::GetPackedValue(enum ControlExtValues Value)
{
	int nRet = (int)m_cPacked;
	switch (Value)
	{
	case GCX_PACKED_DISPOSAL:
		nRet = (nRet & 28) >> 2;
		break;

	case GCX_PACKED_USERINPUT:
		nRet = (nRet & 2) >> 1;
		break;

	case GCX_PACKED_TRANSPCOLOR:
		nRet &= 1;
		break;
	};

	return nRet;
}
//---------------------------------------------------------------------------
//----------------------TGIFLSDescriptor-------------------------------------
//---------------------------------------------------------------------------
inline int TGIFLSDescriptor::GetPackedValue(enum LSDPackedValues Value)
{
	int nRet = (int)m_cPacked;

	switch (Value)
	{
	case LSD_PACKED_GLOBALCT:
		nRet = nRet >> 7;
		break;

	case LSD_PACKED_CRESOLUTION:
		nRet = ((nRet & 0x70) >> 4) + 1;
		break;

	case LSD_PACKED_SORT:
		nRet = (nRet & 8) >> 3;
		break;

	case LSD_PACKED_GLOBALCTSIZE:
		nRet &= 7;
		break;
	};

	return nRet;
}
//---------------------------------------------------------------------------
//----------------------TGIFImageDescriptor----------------------------------
//---------------------------------------------------------------------------
inline int TGIFImageDescriptor::GetPackedValue(enum IDPackedValues Value)
{
	int nRet = (int)m_cPacked;

	switch (Value)
	{
	case ID_PACKED_LOCALCT:
		nRet >>= 7;
		break;

	case ID_PACKED_INTERLACE:
		nRet = ((nRet & 0x40) >> 6);
		break;

	case ID_PACKED_SORT:
		nRet = (nRet & 0x20) >> 5;
		break;

	case ID_PACKED_LOCALCTSIZE:
		nRet &= 7;
		break;
	};

	return nRet;
}
//---------------------------------------------------------------------------
void __fastcall TAGif::Paint()
{
 if(GetFrameCount()<0)
  {
   Width=64;
   Height=64;
   Canvas->Brush->Color=clWhite;
   Canvas->Brush->Style=bsDiagCross;
   Canvas->Rectangle(0,0,Width,Height);
  }
 else
  {
   if(m_PictureSize.cx<=0)
    Width=64;
   else
    Width=m_PictureSize.cx;
   if(m_PictureSize.cy<=0)
    Height=64;
   else
    Height=m_PictureSize.cy;
   ::BitBlt(Canvas->Handle,0,0,m_PictureSize.cx,m_PictureSize.cy,
            m_hMemDC,0,0,SRCCOPY);
  }
}
//---------------------------------------------------------------------------
void __fastcall TAGif::SetFileName(AnsiString Value)
{
 FFileName=Value;
 Load(FFileName);
 Draw();
 FAnimate=true;
}
//---------------------------------------------------------------------------
void __fastcall TAGif::SetAnimate(bool Value)
{
 FAnimate=Value;
 if(FAnimate)
  Draw();
 else
  Stop();
}
//---------------------------------------------------------------------------
bool TAGif::Load(AnsiString FileName)
{
 int iHandle;
 HGLOBAL hGlobal;
 DWORD dwSize;
 iHandle=FileOpen(FileName,fmOpenRead | fmShareDenyWrite);
 if (!iHandle)
  {
   return FALSE;
  };

 dwSize = FileSeek(iHandle,0,2);// File Size
 FileSeek(iHandle,0,0);// Return To Normal State
 hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD,dwSize);
 if (!hGlobal)
  {
   return FALSE;
  };

 char *pData = reinterpret_cast<char*>(GlobalLock(hGlobal));
 if (!pData)
  {
   GlobalFree(hGlobal);
   return FALSE;
  };

 try
  {
   FileRead(iHandle,pData,dwSize);
  }
 catch(Exception &E)
  {
   GlobalFree(hGlobal);
   FileClose(iHandle);
   return FALSE;
  }
 GlobalUnlock(hGlobal);
 FileClose(iHandle);

 BOOL bRetValue = Load(hGlobal,dwSize);
 GlobalFree(hGlobal);
 return bRetValue;
}
//---------------------------------------------------------------------------
bool TAGif::Load(HGLOBAL hGlobal, DWORD dwSize)
{
	IStream *pStream = NULL;
	UnLoad();

	if (!(m_pRawData = reinterpret_cast<unsigned char*>(GlobalLock(hGlobal))))
	{
		return FALSE;
	};

	m_nDataSize = dwSize;
	m_pGIFHeader = reinterpret_cast<TGIFHeader *> (m_pRawData);

	if ((memcmp(&m_pGIFHeader->m_cSignature,"GIF",3) != 0) &&
		((memcmp(&m_pGIFHeader->m_cVersion,"87a",3) != 0) ||
		 (memcmp(&m_pGIFHeader->m_cVersion,"89a",3) != 0)) )
	{
	// it's neither GIF87a nor GIF89a
	// do the default processing

		// clear GIF variables
		m_pRawData = NULL;
		GlobalUnlock(hGlobal);

		// don't delete memory on object's release
		if (CreateStreamOnHGlobal(hGlobal,FALSE,&pStream) != S_OK)
			return FALSE;

		if (OleLoadPicture(pStream,dwSize,FALSE,IID_IPicture,
			reinterpret_cast<LPVOID *>(&m_pPicture)) != S_OK)
		{
			pStream->Release();
			return FALSE;
		};
		pStream->Release();

		// store picture's size

		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		HDC hDC = Canvas->Handle;
		m_PictureSize.cx = MulDiv(hmWidth, GetDeviceCaps(hDC,LOGPIXELSX), 2540);
		m_PictureSize.cy = MulDiv(hmHeight, GetDeviceCaps(hDC,LOGPIXELSY), 2540);
	}
	else
	{
		// it's a GIF
		m_bIsGIF = TRUE;
		m_pGIFLSDescriptor = reinterpret_cast<TGIFLSDescriptor *>
			(m_pRawData + sizeof(TGIFHeader));
		if (m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCT) == 1)
		{
			// calculate the globat color table size
			m_nGlobalCTSize = static_cast<int>
				(3*pow(2,m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCTSIZE)+1));
			// get the background color if GCT is present
			unsigned char *pBkClr = m_pRawData + sizeof(TGIFHeader) + 
				sizeof(TGIFLSDescriptor) + 3*m_pGIFLSDescriptor->m_cBkIndex;
			m_clrBackground = RGB(pBkClr[0],pBkClr[1],pBkClr[2]);
		};

		// store the picture's size
		m_PictureSize.cx = m_pGIFLSDescriptor->m_wWidth;
		m_PictureSize.cy = m_pGIFLSDescriptor->m_wHeight;

		// determine frame count for this picture
		UINT nFrameCount=0;
		ResetDataPointer();
		while(SkipNextGraphicBlock())
		 nFrameCount++;

		if(nFrameCount == 0) // it's an empty GIF!
		{
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);
			return FALSE;
		};

		// now check the frame count
		// if there's only one frame, no need to animate this GIF
		// therefore, treat it like any other pic

		if (nFrameCount == 1)
		{
			// clear GIF variables
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);

			// don't delete memory on object's release
			if (CreateStreamOnHGlobal(hGlobal,FALSE,&pStream) != S_OK)
				return FALSE;

			if (OleLoadPicture(pStream,dwSize,FALSE,IID_IPicture,
				(LPVOID *)&m_pPicture) != S_OK)
			{
				pStream->Release();
				return FALSE;
			};

			pStream->Release();
		}
		else
		{
		// if, on the contrary, there are several frames
		// then store separate frames in an array

			::TFrame frame;
			UINT nBlockLen;
			HGLOBAL hFrameData;

			ResetDataPointer();
			while (hFrameData = GetNextGraphicBlock(&nBlockLen,
				&frame.m_nDelay, &frame.m_frameSize,
				&frame.m_frameOffset, &frame.m_nDisposal))
			{
				IStream *pStream = NULL;

				// delete memory on object's release
				if (CreateStreamOnHGlobal(hFrameData,TRUE,&pStream) != S_OK)
				{
					GlobalFree(hFrameData);
					continue;
				};

				if (OleLoadPicture(pStream,nBlockLen,FALSE,
					IID_IPicture,
					reinterpret_cast<LPVOID *>(&frame.m_pPicture)) != S_OK)
				{
					pStream->Release();
					continue;
				};
				pStream->Release();
			
				// everything went well, add this frame
				m_arrFrames.push_back(frame);
			};

			// clean after ourselves
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);

			if (m_arrFrames.empty()) // couldn't load any frames
				return FALSE;
		};
	}; // if (!IsGIF...

	return PrepareDC(m_PictureSize.cx,m_PictureSize.cy);
}
//---------------------------------------------------------------------------
void TAGif::UnLoad()
{
 Stop();
 if (m_pPicture)
  {
   m_pPicture->Release();
   m_pPicture = NULL;
  };

 std::vector<::TFrame>::iterator it;
 for (it=m_arrFrames.begin();it<m_arrFrames.end();it++)
  (*it).m_pPicture->Release();
 m_arrFrames.clear();

 if (m_hMemDC)
  {
   SelectObject(m_hMemDC,m_hOldBitmap);
   ::DeleteDC(m_hMemDC);
   ::DeleteObject(m_hBitmap);
   m_hMemDC  = NULL;
   m_hBitmap = NULL;
  };

 m_pGIFLSDescriptor = NULL;
 m_pGIFHeader	    = NULL;
 m_pRawData	    = NULL;
 m_hThread	    = NULL;
 m_bIsInitialized   = FALSE;
 m_bExitThread	    = FALSE;
 m_bIsGIF	    = FALSE;
 m_clrBackground    = RGB(255,255,255); // white by default
 m_nGlobalCTSize    = 0;
 m_nCurrOffset	    = 0;
 m_nDataSize	    = 0;
}
//---------------------------------------------------------------------------
void TAGif::ResetDataPointer()
{
 // skip header and logical screen descriptor
 m_nCurrOffset=sizeof(TGIFHeader)+sizeof(TGIFLSDescriptor)+m_nGlobalCTSize;
}
//---------------------------------------------------------------------------
bool TAGif::SkipNextGraphicBlock()
{
 if(!m_pRawData)
  return FALSE;

 // GIF header + LSDescriptor [+ GCT] [+ Control block] + Data
 enum GIFBlockTypes nBlock;
 nBlock=GetNextBlock();

 while((nBlock != BLOCK_CONTROLEXT) &&
       (nBlock != BLOCK_IMAGE) &&
       (nBlock != BLOCK_PLAINTEXT) &&
       (nBlock != BLOCK_UNKNOWN) &&
       (nBlock != BLOCK_TRAILER))
  {
   if(!SkipNextBlock())
    return NULL;
   nBlock=GetNextBlock();
  };

 if((nBlock == BLOCK_UNKNOWN) ||
    (nBlock == BLOCK_TRAILER))
  return FALSE;

 // it's either a control ext.block, an image or a plain text
 if(GetNextBlockLen() <= 0)
  return FALSE;

 if(nBlock == BLOCK_CONTROLEXT)
  {
   if(!SkipNextBlock())
    return FALSE;
   nBlock=GetNextBlock();

   // skip everything until we meet an image block or a plain-text block
   while((nBlock != BLOCK_IMAGE) &&
	 (nBlock != BLOCK_PLAINTEXT) &&
	 (nBlock != BLOCK_UNKNOWN) &&
	 (nBlock != BLOCK_TRAILER) )
    {
     if(!SkipNextBlock())
      return NULL;
     nBlock=GetNextBlock();
    };

   if((nBlock == BLOCK_UNKNOWN) ||
      (nBlock == BLOCK_TRAILER))
    return FALSE;
  };

 // skip the found data block (image or plain-text)
 if(!SkipNextBlock())
  return FALSE;

 return TRUE;
}
//---------------------------------------------------------------------------
enum GIFBlockTypes TAGif::GetNextBlock() const
{
 switch(m_pRawData[m_nCurrOffset])
  {
   case 0x21:
   // extension block
    switch(m_pRawData[m_nCurrOffset+1])
     {
      case 0x01:
      // plain text extension
       return BLOCK_PLAINTEXT;

      case 0xF9:
      // graphic control extension
       return BLOCK_CONTROLEXT;

      case 0xFE:
      // comment extension
       return BLOCK_COMMEXT;

      case 0xFF:
      // application extension
       return BLOCK_APPEXT;
     };
    break;

   case 0x3B:
   // trailer
    return BLOCK_TRAILER;

   case 0x2C:
   // image data
    return BLOCK_IMAGE;
  };

 return BLOCK_UNKNOWN;
}
//---------------------------------------------------------------------------
bool TAGif::SkipNextBlock()
{
	if (!m_pRawData) return FALSE;

	int nLen = GetNextBlockLen();
	if ((nLen <= 0) || ((m_nCurrOffset+nLen) > m_nDataSize))
		return FALSE;

	m_nCurrOffset += nLen;
	return TRUE;
}
//---------------------------------------------------------------------------
int TAGif::GetNextBlockLen() const
{
 GIFBlockTypes nBlock = GetNextBlock();

 int nTmp;

 switch(nBlock)
  {
   case BLOCK_UNKNOWN:
    return -1;

   case BLOCK_TRAILER:
    return 1;

   case BLOCK_APPEXT:
    nTmp = GetSubBlocksLen(m_nCurrOffset+sizeof(TGIFAppExtension));
    if(nTmp > 0)
     return sizeof(TGIFAppExtension)+nTmp;
   break;

   case BLOCK_COMMEXT:
    nTmp = GetSubBlocksLen(m_nCurrOffset+sizeof(TGIFCommentExt));
    if(nTmp > 0)
     return sizeof(TGIFCommentExt)+nTmp;
   break;

   case BLOCK_CONTROLEXT:
    return sizeof(TGIFControlExt);

   case BLOCK_PLAINTEXT:
    nTmp = GetSubBlocksLen(m_nCurrOffset+sizeof(TGIFPlainTextExt));
    if(nTmp > 0)
     return sizeof(TGIFPlainTextExt)+nTmp;
   break;

   case BLOCK_IMAGE:
    TGIFImageDescriptor *pIDescr =
     reinterpret_cast<TGIFImageDescriptor *> (&m_pRawData[m_nCurrOffset]);
    int nLCTSize = (int)
     (pIDescr->GetPackedValue(ID_PACKED_LOCALCT)*3*
     pow(2,pIDescr->GetPackedValue(ID_PACKED_LOCALCTSIZE)+1));

    int nTmp = GetSubBlocksLen(m_nCurrOffset+
     sizeof(TGIFImageDescriptor) + nLCTSize + 1);
    if(nTmp > 0)
     return sizeof(TGIFImageDescriptor) + nLCTSize + 1 + nTmp;
   break;
  };

 return 0;
}
//---------------------------------------------------------------------------
UINT TAGif::GetSubBlocksLen(UINT nStartingOffset) const
{
 UINT nRet = 0;
 UINT nCurOffset = nStartingOffset;

 while (m_pRawData[nCurOffset] != 0)
  {
   nRet += m_pRawData[nCurOffset]+1;
   nCurOffset += m_pRawData[nCurOffset]+1;
  };

 return nRet+1;
}
//---------------------------------------------------------------------------
bool TAGif::Draw()
{
 if(!m_bIsInitialized)
  {
   return FALSE;
  };

 if(IsAnimatedGIF())
  {
   // the picture needs some animation
   // we'll start the thread that will handle it for us

   unsigned int nDummy;
   m_hThread = (HANDLE) _beginthreadex(NULL,0,_ThreadAnimation,this,
		CREATE_SUSPENDED,&nDummy);
   if(!m_hThread)
    {
     return FALSE;
    }
   else
    ResumeThread(m_hThread);
  }
 else
  {
   if(m_pPicture)
    {
     long hmWidth;
     long hmHeight;
     m_pPicture->get_Width(&hmWidth);
     m_pPicture->get_Height(&hmHeight);

     if (m_pPicture->Render(m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy,
	        	    0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK)
      {
       ::BitBlt(Canvas->Handle,0,0,m_PictureSize.cx,m_PictureSize.cy,
                m_hMemDC,0,0,SRCCOPY);
       return TRUE;
      };

     Width=m_PictureSize.cx;
     Height=m_PictureSize.cy;
     ::BitBlt(Canvas->Handle,0,0,m_PictureSize.cx,m_PictureSize.cy,
	      m_hMemDC,0,0,SRCCOPY);
     return TRUE;
    };
  };

 return FALSE;
}
//---------------------------------------------------------------------------
SIZE TAGif::GetSize() const
{
 return m_PictureSize;
}
//---------------------------------------------------------------------------
void TAGif::Stop()
{
 m_bExitThread = TRUE;
 SetEvent(m_hExitEvent);
 if(m_hThread)
  {
   // we'll wait for 5 seconds then continue execution
   // I hate hangups :P
   WaitForSingleObject(m_hThread,5000);
   CloseHandle(m_hThread);
   m_hThread = NULL;
  }

 // make it possible to Draw() again
 ResetEvent(m_hExitEvent);
 m_bExitThread = FALSE;
}
//---------------------------------------------------------------------------
bool TAGif::IsGIF() const
{
 return m_bIsGIF;
}
//---------------------------------------------------------------------------
bool TAGif::IsAnimatedGIF() const
{
 return (m_bIsGIF && (m_arrFrames.size() > 1));
}
//---------------------------------------------------------------------------
int TAGif::GetFrameCount() const
{
 if(!IsAnimatedGIF())
  return 0;

 return m_arrFrames.size();
}
//---------------------------------------------------------------------------
COLORREF TAGif::GetBkColor() const
{
 return m_clrBackground;
}
//---------------------------------------------------------------------------
void TAGif::SetBkColor(COLORREF clr)
{
 if(!m_bIsInitialized) return;
  m_clrBackground = clr;

 HBRUSH hBrush = CreateSolidBrush(clr);
 if(hBrush)
  {
   RECT rect = {0,0,m_PictureSize.cx,m_PictureSize.cy};
   FillRect(m_hMemDC,&rect,hBrush);
   DeleteObject(hBrush);
  };
}
//---------------------------------------------------------------------------
HGLOBAL TAGif::GetNextGraphicBlock(UINT *pBlockLen,
	UINT *pDelay, SIZE *pBlockSize, SIZE *pBlockOffset,
	UINT *pDisposal)
{
 if(!m_pRawData)
  return NULL;

 // GIF header + LSDescriptor [+ GCT] [+ Control block] + Data
 *pDisposal = 0;
 enum GIFBlockTypes nBlock;
 nBlock = GetNextBlock();

 while((nBlock != BLOCK_CONTROLEXT) &&
       (nBlock != BLOCK_IMAGE) &&
       (nBlock != BLOCK_PLAINTEXT) &&
       (nBlock != BLOCK_UNKNOWN) &&
       (nBlock != BLOCK_TRAILER))
  {
   if(!SkipNextBlock())
    return NULL;
   nBlock = GetNextBlock();
  };

 if((nBlock == BLOCK_UNKNOWN) ||
    (nBlock == BLOCK_TRAILER))
  return NULL;

 // it's either a control ext.block, an image or a plain text
 int nStart = m_nCurrOffset;
 int nBlockLen = GetNextBlockLen();

 if(nBlockLen <= 0)
  return NULL;

 if(nBlock == BLOCK_CONTROLEXT)
  {
   // get the following data
   TGIFControlExt *pControl =
    reinterpret_cast<TGIFControlExt *> (&m_pRawData[m_nCurrOffset]);
   // store delay time
   *pDelay = pControl->m_wDelayTime;
   // store disposal method
   *pDisposal = pControl->GetPackedValue(GCX_PACKED_DISPOSAL);

   if(!SkipNextBlock())
    return NULL;
   nBlock = GetNextBlock();

   // skip everything until we find data to display
   // (image block or plain-text block)

   while((nBlock != BLOCK_IMAGE) &&
	 (nBlock != BLOCK_PLAINTEXT) &&
	 (nBlock != BLOCK_UNKNOWN) &&
	 (nBlock != BLOCK_TRAILER))
    {
     if(!SkipNextBlock())
      return NULL;
     nBlock = GetNextBlock();
     nBlockLen += GetNextBlockLen();
    };

   if((nBlock == BLOCK_UNKNOWN) || (nBlock == BLOCK_TRAILER))
    return NULL;
   nBlockLen += GetNextBlockLen();
  }
 else
  *pDelay = -1; // to indicate that there was no delay value

 if(nBlock == BLOCK_IMAGE)
  {
   // store size and offsets
   TGIFImageDescriptor *pImage =
    reinterpret_cast<TGIFImageDescriptor *> (&m_pRawData[m_nCurrOffset]);
   pBlockSize->cx = pImage->m_wWidth;
   pBlockSize->cy = pImage->m_wHeight;
   pBlockOffset->cx = pImage->m_wLeftPos;
   pBlockOffset->cy = pImage->m_wTopPos;
  };

 if(!SkipNextBlock())
  return NULL;

 HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED,
  sizeof(TGIFHeader) +
  sizeof(TGIFLSDescriptor) +
  m_nGlobalCTSize +
  nBlockLen +
  1);  // for the trailer

 if(!hGlobal)
  return NULL;

 int nOffset = 0;

 // GMEM_FIXED means we get a pointer
 unsigned char *pGlobal = reinterpret_cast<unsigned char *> (hGlobal);

 CopyMemory(pGlobal,m_pRawData,
  sizeof(TGIFHeader)+sizeof(TGIFLSDescriptor)+m_nGlobalCTSize);
  nOffset += sizeof(TGIFHeader)+sizeof(TGIFLSDescriptor)+m_nGlobalCTSize;

 CopyMemory(pGlobal + nOffset,&m_pRawData[nStart], nBlockLen);
  nOffset += nBlockLen;

 pGlobal[nOffset] = 0x3B; // trailer
 nOffset++;

 *pBlockLen = nOffset;

 return hGlobal;
}
//---------------------------------------------------------------------------
bool TAGif::PrepareDC(int nWidth, int nHeight)
{
 SetWindowPos(NULL,NULL,0,0,nWidth,nHeight,SWP_NOMOVE | SWP_NOZORDER);

 HDC hWinDC = Canvas->Handle;
 if(!hWinDC)
  return FALSE;

 m_hMemDC = CreateCompatibleDC(hWinDC);
 if(!m_hMemDC)
  {
   return FALSE;
  };

 m_hBitmap  = CreateCompatibleBitmap(hWinDC,nWidth,nHeight);
 if(!m_hBitmap)
  {
   ::DeleteDC(m_hMemDC);
   return FALSE;
  };

 m_hOldBitmap = reinterpret_cast<HBITMAP>
		(SelectObject(m_hMemDC,m_hBitmap));

 // fill the background
 m_clrBackground = GetSysColor(COLOR_3DFACE);
 RECT rect = {0,0,nWidth,nHeight};
 FillRect(m_hMemDC,&rect,(HBRUSH)(COLOR_WINDOW));

 m_bIsInitialized = TRUE;
 return TRUE;
}
//---------------------------------------------------------------------------
UINT WINAPI TAGif::_ThreadAnimation(LPVOID pParam)
{
 assert(pParam);
 TAGif *pPic = reinterpret_cast<TAGif *> (pParam);
 pPic->ThreadAnimation();

 // this thread has finished its work so we close the handle
 CloseHandle(pPic->m_hThread);
 // and init the handle to zero (so that Stop() doesn't Wait on it)
 pPic->m_hThread = 0;
 return 0;
}
//---------------------------------------------------------------------------
void TAGif::ThreadAnimation()
{
 HANDLE hTimer = CreateWaitableTimer(NULL,TRUE,NULL);
 if(!hTimer)
  {
   return;
  };

 LARGE_INTEGER li;
 const int nUnitsPerSecond = 100000;

 // we're gonna wait for two events, whichever occurs first
 // this way we won't have lags with lengthy GIF-delays
 HANDLE arrHandles[2] = {hTimer, m_hExitEvent};

 unsigned int nTemp = 0;
 while(!m_bExitThread)
  {
   if(m_arrFrames[nTemp].m_pPicture)
    {
     ///////////////////////////////////////////////////////
     // Before rendering a frame we should take care of what's
     // behind that frame. TFrame::m_nDisposal will be our guide:
     //   0 - no disposal specified (do nothing)
     //   1 - do not dispose (again, do nothing)
     //   2 - restore to background color (m_clrBackground)
     //   3 - restore to previous

     //////// disposal method #3
     HDC hMemDC = NULL;
     HBITMAP hMemBM = NULL, hOldBM;

     if(m_arrFrames[nTemp].m_nDisposal == 3)
      {
       // prepare a memory DC and store the background in it
       hMemDC = CreateCompatibleDC(m_hMemDC);
       hMemBM = CreateCompatibleBitmap(m_hMemDC,
       						m_arrFrames[nTemp].m_frameSize.cx,
       						m_arrFrames[nTemp].m_frameSize.cy);

       if(hMemDC && hMemBM)
	{
	 hOldBM = reinterpret_cast<HBITMAP> (SelectObject(hMemDC,hMemBM));
	 BitBlt(hMemDC,0,0,
	 		m_arrFrames[nTemp].m_frameSize.cx,
	 		m_arrFrames[nTemp].m_frameSize.cy,
	 		m_hMemDC,
	 		m_arrFrames[nTemp].m_frameOffset.cx,
	 		m_arrFrames[nTemp].m_frameOffset.cy,
	 		SRCCOPY);
	};
      };
     ///////////////////////

     long hmWidth;
     long hmHeight;
     m_arrFrames[nTemp].m_pPicture->get_Width(&hmWidth);
     m_arrFrames[nTemp].m_pPicture->get_Height(&hmHeight);

     if(m_arrFrames[nTemp].m_pPicture->Render(m_hMemDC,
      	m_arrFrames[nTemp].m_frameOffset.cx,
      	m_arrFrames[nTemp].m_frameOffset.cy,
      	m_arrFrames[nTemp].m_frameSize.cx,
      	m_arrFrames[nTemp].m_frameSize.cy,
      	0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK)
      {
       ::BitBlt(Canvas->Handle,0,0,m_PictureSize.cx,m_PictureSize.cy,
                m_hMemDC,0,0,SRCCOPY);
      };

     if(m_bExitThread)
      {
       if(hMemDC)
	{
	 // dispose local variables
	 SelectObject(hMemDC,hOldBM);
	 DeleteDC(hMemDC);
	 DeleteObject(hMemBM);
	};
       break;
      };

     if(m_arrFrames[nTemp].m_nDelay < 5)
      	li.QuadPart = -static_cast<long>(10*nUnitsPerSecond);
     else
      	li.QuadPart = -(nUnitsPerSecond*static_cast<long>(m_arrFrames[nTemp].m_nDelay));
     SetWaitableTimer(hTimer,&li,0,NULL,NULL,FALSE);

     // of course, we could use a casual Sleep()
     // but then Stop() would take quite some time
     WaitForMultipleObjects(2,arrHandles,FALSE,10000);

     if(m_bExitThread)
      {
       if(hMemDC)
	{
	 // dispose local variables
	 SelectObject(hMemDC,hOldBM);
	 DeleteDC(hMemDC);
	 DeleteObject(hMemBM);
	};
       break;
      };

     // disposal method #2
     if(m_arrFrames[nTemp].m_nDisposal == 2)
      {
       HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
       if(hBrush)
	{
	 RECT rect = {
	  	m_arrFrames[nTemp].m_frameOffset.cx,
	 	m_arrFrames[nTemp].m_frameOffset.cy,
	 	m_arrFrames[nTemp].m_frameOffset.cx + m_arrFrames[nTemp].m_frameSize.cx,
	 	m_arrFrames[nTemp].m_frameOffset.cy + m_arrFrames[nTemp].m_frameSize.cy };
	 FillRect(m_hMemDC,&rect,hBrush);
	 DeleteObject(hBrush);
	};
      }
     else
      if(hMemDC && (m_arrFrames[nTemp].m_nDisposal == 3))
       {
	// put it back
	BitBlt(m_hMemDC,
			m_arrFrames[nTemp].m_frameOffset.cx,
	        	m_arrFrames[nTemp].m_frameOffset.cy,
			m_arrFrames[nTemp].m_frameSize.cx,
			m_arrFrames[nTemp].m_frameSize.cy,
			hMemDC,0,0, SRCCOPY);
	// dispose local variables
	SelectObject(hMemDC,hOldBM);
	DeleteDC(hMemDC);
	DeleteObject(hMemBM);
       };
    };
   nTemp++;
   if(nTemp == m_arrFrames.size())
    {
     nTemp = 0;
     // init the screen for the first frame,
     RECT rect = {0,0,m_PictureSize.cx,m_PictureSize.cy};
     FillRect(m_hMemDC,&rect,(HBRUSH)(COLOR_WINDOW));
    };
  };
 CloseHandle(hTimer);
}
//---------------------------------------------------------------------------
//-------------------TFileName-----------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFileNameProperty::Edit()
{
 TOpenDialog *OpenDialog=new TOpenDialog(Application);
 OpenDialog->DefaultExt="gif";
 OpenDialog->Filter="GIF Image (*.gif)|*.gif";
 OpenDialog->FileName=GetValue();
 OpenDialog->Options<<ofShowHelp<<ofPathMustExist<<ofFileMustExist;
 try
  {
   if(OpenDialog->Execute())
    SetValue(OpenDialog->FileName);
  }
 __finally
  {
   if(OpenDialog)
    delete OpenDialog;
  }
}
//---------------------------------------------------------------------------
TPropertyAttributes __fastcall TFileNameProperty::GetAttributes()
{
 return TPropertyAttributes()<<paDialog<<paRevertable;
}
//---------------------------------------------------------------------------
