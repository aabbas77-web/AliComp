//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("ALabelPk.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("ALabel.cpp");
USERES("ALabel.dcr");
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
