//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("AWavPk.res");
USEPACKAGE("vcl50.bpi");
USERES("AWav.dcr");
USEUNIT("AWav.cpp");
USEUNIT("RIFF.CPP");
USEUNIT("DDCRET.CPP");
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
