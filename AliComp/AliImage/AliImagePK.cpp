//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("AliImagePK.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("AliImage.cpp");
USERES("AliImage.dcr");
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
