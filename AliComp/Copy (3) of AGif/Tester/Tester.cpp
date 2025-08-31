//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Tester.res");
USEFORM("Main.cpp", FormMain);
USEUNIT("..\AGif.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TFormMain), &FormMain);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
