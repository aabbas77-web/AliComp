//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("MoviePk.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("Movie.cpp");
USERES("Movie.dcr");
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
