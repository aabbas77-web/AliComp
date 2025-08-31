//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("AButtonPK.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("AButton.cpp");
USERES("AButton.dcr");
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
