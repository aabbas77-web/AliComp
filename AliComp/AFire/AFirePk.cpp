//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("AFirePk.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("AFire.cpp");
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
