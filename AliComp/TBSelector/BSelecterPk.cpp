//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("BSelecterPK.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("BSelecter.cpp");
USERES("BSelecter.dcr");
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
