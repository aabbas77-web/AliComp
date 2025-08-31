//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("OnOffPk.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("OnOff.cpp");
USERES("OnOff.dcr");
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
