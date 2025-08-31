//---------------------------------------------------------------------------

#ifndef AWavH
#define AWavH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#include "riff.h"
//---------------------------------------------------------------------------
class PACKAGE TAWav : public TComponent
{
private:
protected:
public:
    __fastcall TAWav(TComponent* Owner);
    __fastcall ~TAWav();
    WaveFile *Wav;
__published:
};
//---------------------------------------------------------------------------
#endif
 