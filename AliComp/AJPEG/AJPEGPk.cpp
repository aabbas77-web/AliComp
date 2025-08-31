//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("AJPEGPk.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("AJPEG.PAS");
USEUNIT("AJCONSTS.PAS");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
        return 1;
}
//---------------------------------------------------------------------------
