//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("OnOffPk.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("OnOffButton.cpp");
USERES("OnOffButton.dcr");
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
