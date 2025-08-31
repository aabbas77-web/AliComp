//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("AKochPk.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("AKoch.cpp");
USERES("AKoch.dcr");
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
