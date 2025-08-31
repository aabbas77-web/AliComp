//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("AButtonPK.res");
USEUNIT("AButton.cpp");
USERES("AButton.dcr");
USEPACKAGE("VCL50.bpi");
USEPACKAGE("AJPEGPk.bpi");
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
