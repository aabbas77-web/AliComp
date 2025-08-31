//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Example.res");
USEFORM("..\Example1\Main.cpp", FormMain);
USEUNIT("..\Region.cpp");
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
