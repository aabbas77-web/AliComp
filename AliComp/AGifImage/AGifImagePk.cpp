//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("AGifImagePk.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("AGifImage.cpp");
USERES("AGifImage.dcr");
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
