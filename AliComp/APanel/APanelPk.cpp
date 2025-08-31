//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("APanelPk.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("APanel.cpp");
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
