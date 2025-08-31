//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("ARoundButtonPK.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("ARoundButton.cpp");
USERES("ARoundButton.dcr");
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
