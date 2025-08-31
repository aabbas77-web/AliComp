//------------------------------------------------------------------------------
// MMX Pentium Instruction Support Component
//                          ver.0.90 / date:Nov.20,1997 / copyright:H.Gotou,1997
//------------------------------------------------------------------------------

unit PentiumMMX;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs;

type
  EPentiumMMXError = class(Exception)
                     ErrorCode: Integer;
  end;
  TVendorStr = array[0..2] of DWord;
  QuadWord = record
    high: integer;
    low:  integer;
  end;
  TMMXinst = (_,
              _PAND,_PANDN,_POR,_PXOR,_PSRLw,_PSRLd,_PSRLq,_PSRAw,_PSRAd,_PSLLw,_PSLLd,_PSLLq,
              _PCMPGTb,_PCMPGTw,_PCMPGTd,_PCMPEQb,_PCMPEQw,_PCMPEQd,
              _PACKSSwb,_PACKSSdw,_PACKUSwb,_PUNPCKLbw,_PUNPCKLwd,_PUNPCKLdq,_PUNPCKHbw,_PUNPCKHwd,_PUNPCKHdq,
              _PADDUSb,_PADDUSw,_PADDSb,_PADDSw,_PADDb,_PADDw,_PADDd,
              _PSUBUSb,_PSUBUSw,_PSUBSb,_PSUBSw,_PSUBb,_PSUBw,_PSUBd,
              _PMULLw,_PMULHw,_PMADDwd);
  TPentiumMMX = class(TComponent)
  private
    { Private }
    //properties
      FErrorEnable:   Boolean;
    //method
      function GetCpuIDsupport:  Boolean;
      function GetRDTSCsupport:  Boolean;
      function GetMMXsupport:    Boolean;
      function GetVendorString:  String;
      function GetSteppingID:    Byte;
      function GetModel:         Byte;
      function GetFamily:        Word;
      function GetFeatureFlags:  DWord;
      function GetTimeStampHigh: Integer;
      function GetTimeStampLow:  Integer;
  protected
    { Protected }
  public
    { Public }
    constructor Create(AOwner: TComponent); override;
  published
    { Published }
    //properties
      property ErrorEnable: Boolean   read FErrorEnable write FErrorEnable default false;
      property CpuIDsupport: Boolean  read GetCpuIDsupport;
      property RDTSCsupport: Boolean  read GetRDTSCsupport;
      property MMXsupport: Boolean    read GetMMXsupport;
      property VendorString: String   read GetVendorString;
      property SteppingID: Byte       read GetSteppingID;
      property Model: Byte            read GetModel;
      property Family: Word           read GetFamily;
      property FeatureFlags: DWord    read GetFeatureFlags;
      property TimeStampHigh: Integer read GetTimeStampHigh;
      property TimeStampLow:  Integer read GetTimeStampLow;
    //function
      function Interval32(X:Integer; Y:Integer):Integer;
      function Interval64(X:QuadWord; Y:QuadWord):QuadWord;
      function MmxOperate(inst:TMMXinst; X:QuadWord; Y:QuadWord):QuadWord;
  end;

const
//--------------------------------------
// Define MMX Pentiun CPU value
//--------------------------------------
//Define Pentium Instructions opcode
  CPUID     = $A20F;
  RDTSC     = $310F;
  RDPMC     = $330F;
//Define MMX Instructions opcode
  EMMS      = $770F;  //Empty MMX state
  MOVD_ld   = $6E0F;  //Move operation
  MOVD_st   = $7E0F;
  MOVQ_ld   = $6F0F;
  MOVQ_st   = $7F0F;
  PAND      = $DB0F;  //Bit operation
  PANDN     = $DF0F;
  POR       = $EB0F;
  PXOR      = $EF0F;
  PSRLw     = $D10F;
  PSRLd     = $D20F;
  PSRLq     = $D30F;
  PSRAw     = $E10F;
  PSRAd     = $E20F;
  PSLLw     = $F10F;
  PSLLd     = $F20F;
  PSLLq     = $F30F;
  PSXXimw   = $710F;
  PSXXimd   = $720F;
  PSXXimq   = $730F;
  PCMPGTb   = $640F;  //Compare data
  PCMPGTw   = $650F;
  PCMPGTd   = $660F;
  PCMPEQb   = $740F;
  PCMPEQw   = $750F;
  PCMPEQd   = $760F;
  PACKSSwb  = $630F;  //Pack data
  PACKSSdw  = $6B0F;
  PACKUSwb  = $670F;
  PUNPCKLbw = $600F;  //Unpack data
  PUNPCKLwd = $610F;
  PUNPCKLdq = $620F;
  PUNPCKHbw = $680F;
  PUNPCKHwd = $690F;
  PUNPCKHdq = $6A0F;
  PADDUSb   = $DC0F;  //Mathematical operation
  PADDUSw   = $DD0F;
  PADDSb    = $EC0F;
  PADDSw    = $ED0F;
  PADDb     = $FC0F;
  PADDw     = $FD0F;
  PADDd     = $FE0F;
  PSUBUSb   = $D80F;
  PSUBUSw   = $D90F;
  PSUBSb    = $E80F;
  PSUBSw    = $E90F;
  PSUBb     = $F80F;
  PSUBw     = $F90F;
  PSUBd     = $FA0F;
  PMULLw    = $D50F;
  PMULHw    = $E50F;
  PMADDwd   = $F50F;
//Define MMX ModR/W for MMX_resisters and General resisters
  MM0_EAX   = ($3 shl 6) + ($0 shl 3) + ($0 shl 0);
  MM1_EAX   = ($3 shl 6) + ($1 shl 3) + ($0 shl 0);
  MM2_EAX   = ($3 shl 6) + ($2 shl 3) + ($0 shl 0);
  MM3_EAX   = ($3 shl 6) + ($3 shl 3) + ($0 shl 0);
  MM4_EAX   = ($3 shl 6) + ($4 shl 3) + ($0 shl 0);
  MM5_EAX   = ($3 shl 6) + ($5 shl 3) + ($0 shl 0);
  MM6_EAX   = ($3 shl 6) + ($6 shl 3) + ($0 shl 0);
  MM7_EAX   = ($3 shl 6) + ($7 shl 3) + ($0 shl 0);
  MM0_ECX   = ($3 shl 6) + ($0 shl 3) + ($1 shl 0);
  MM1_ECX   = ($3 shl 6) + ($1 shl 3) + ($1 shl 0);
  MM2_ECX   = ($3 shl 6) + ($2 shl 3) + ($1 shl 0);
  MM3_ECX   = ($3 shl 6) + ($3 shl 3) + ($1 shl 0);
  MM4_ECX   = ($3 shl 6) + ($4 shl 3) + ($1 shl 0);
  MM5_ECX   = ($3 shl 6) + ($5 shl 3) + ($1 shl 0);
  MM6_ECX   = ($3 shl 6) + ($6 shl 3) + ($1 shl 0);
  MM7_ECX   = ($3 shl 6) + ($7 shl 3) + ($1 shl 0);
  MM0_EDX   = ($3 shl 6) + ($0 shl 3) + ($2 shl 0);
  MM1_EDX   = ($3 shl 6) + ($1 shl 3) + ($2 shl 0);
  MM2_EDX   = ($3 shl 6) + ($2 shl 3) + ($2 shl 0);
  MM3_EDX   = ($3 shl 6) + ($3 shl 3) + ($2 shl 0);
  MM4_EDX   = ($3 shl 6) + ($4 shl 3) + ($2 shl 0);
  MM5_EDX   = ($3 shl 6) + ($5 shl 3) + ($2 shl 0);
  MM6_EDX   = ($3 shl 6) + ($6 shl 3) + ($2 shl 0);
  MM7_EDX   = ($3 shl 6) + ($7 shl 3) + ($2 shl 0);
  MM0_EBX   = ($3 shl 6) + ($0 shl 3) + ($3 shl 0);
  MM1_EBX   = ($3 shl 6) + ($1 shl 3) + ($3 shl 0);
  MM2_EBX   = ($3 shl 6) + ($2 shl 3) + ($3 shl 0);
  MM3_EBX   = ($3 shl 6) + ($3 shl 3) + ($3 shl 0);
  MM4_EBX   = ($3 shl 6) + ($4 shl 3) + ($3 shl 0);
  MM5_EBX   = ($3 shl 6) + ($5 shl 3) + ($3 shl 0);
  MM6_EBX   = ($3 shl 6) + ($6 shl 3) + ($3 shl 0);
  MM7_EBX   = ($3 shl 6) + ($7 shl 3) + ($3 shl 0);
  MM0_ESP   = ($3 shl 6) + ($0 shl 3) + ($4 shl 0);
  MM1_ESP   = ($3 shl 6) + ($1 shl 3) + ($4 shl 0);
  MM2_ESP   = ($3 shl 6) + ($2 shl 3) + ($4 shl 0);
  MM3_ESP   = ($3 shl 6) + ($3 shl 3) + ($4 shl 0);
  MM4_ESP   = ($3 shl 6) + ($4 shl 3) + ($4 shl 0);
  MM5_ESP   = ($3 shl 6) + ($5 shl 3) + ($4 shl 0);
  MM6_ESP   = ($3 shl 6) + ($6 shl 3) + ($4 shl 0);
  MM7_ESP   = ($3 shl 6) + ($7 shl 3) + ($4 shl 0);
  MM0_EBP   = ($3 shl 6) + ($0 shl 3) + ($5 shl 0);
  MM1_EBP   = ($3 shl 6) + ($1 shl 3) + ($5 shl 0);
  MM2_EBP   = ($3 shl 6) + ($2 shl 3) + ($5 shl 0);
  MM3_EBP   = ($3 shl 6) + ($3 shl 3) + ($5 shl 0);
  MM4_EBP   = ($3 shl 6) + ($4 shl 3) + ($5 shl 0);
  MM5_EBP   = ($3 shl 6) + ($5 shl 3) + ($5 shl 0);
  MM6_EBP   = ($3 shl 6) + ($6 shl 3) + ($5 shl 0);
  MM7_EBP   = ($3 shl 6) + ($7 shl 3) + ($5 shl 0);
  MM0_ESI   = ($3 shl 6) + ($0 shl 3) + ($6 shl 0);
  MM1_ESI   = ($3 shl 6) + ($1 shl 3) + ($6 shl 0);
  MM2_ESI   = ($3 shl 6) + ($2 shl 3) + ($6 shl 0);
  MM3_ESI   = ($3 shl 6) + ($3 shl 3) + ($6 shl 0);
  MM4_ESI   = ($3 shl 6) + ($4 shl 3) + ($6 shl 0);
  MM5_ESI   = ($3 shl 6) + ($5 shl 3) + ($6 shl 0);
  MM6_ESI   = ($3 shl 6) + ($6 shl 3) + ($6 shl 0);
  MM7_ESI   = ($3 shl 6) + ($7 shl 3) + ($6 shl 0);
  MM0_EDI   = ($3 shl 6) + ($0 shl 3) + ($7 shl 0);
  MM1_EDI   = ($3 shl 6) + ($1 shl 3) + ($7 shl 0);
  MM2_EDI   = ($3 shl 6) + ($2 shl 3) + ($7 shl 0);
  MM3_EDI   = ($3 shl 6) + ($3 shl 3) + ($7 shl 0);
  MM4_EDI   = ($3 shl 6) + ($4 shl 3) + ($7 shl 0);
  MM5_EDI   = ($3 shl 6) + ($5 shl 3) + ($7 shl 0);
  MM6_EDI   = ($3 shl 6) + ($6 shl 3) + ($7 shl 0);
  MM7_EDI   = ($3 shl 6) + ($7 shl 3) + ($7 shl 0);
//Define MMX ModR/W for MMX_resisters operation
  MM0_MM0   = ($3 shl 6) + ($0 shl 3) + ($0 shl 0);
  MM1_MM0   = ($3 shl 6) + ($1 shl 3) + ($0 shl 0);
  MM2_MM0   = ($3 shl 6) + ($2 shl 3) + ($0 shl 0);
  MM3_MM0   = ($3 shl 6) + ($3 shl 3) + ($0 shl 0);
  MM4_MM0   = ($3 shl 6) + ($4 shl 3) + ($0 shl 0);
  MM5_MM0   = ($3 shl 6) + ($5 shl 3) + ($0 shl 0);
  MM6_MM0   = ($3 shl 6) + ($6 shl 3) + ($0 shl 0);
  MM7_MM0   = ($3 shl 6) + ($7 shl 3) + ($0 shl 0);
  MM0_MM1   = ($3 shl 6) + ($0 shl 3) + ($1 shl 0);
  MM1_MM1   = ($3 shl 6) + ($1 shl 3) + ($1 shl 0);
  MM2_MM1   = ($3 shl 6) + ($2 shl 3) + ($1 shl 0);
  MM3_MM1   = ($3 shl 6) + ($3 shl 3) + ($1 shl 0);
  MM4_MM1   = ($3 shl 6) + ($4 shl 3) + ($1 shl 0);
  MM5_MM1   = ($3 shl 6) + ($5 shl 3) + ($1 shl 0);
  MM6_MM1   = ($3 shl 6) + ($6 shl 3) + ($1 shl 0);
  MM7_MM1   = ($3 shl 6) + ($7 shl 3) + ($1 shl 0);
  MM0_MM2   = ($3 shl 6) + ($0 shl 3) + ($2 shl 0);
  MM1_MM2   = ($3 shl 6) + ($1 shl 3) + ($2 shl 0);
  MM2_MM2   = ($3 shl 6) + ($2 shl 3) + ($2 shl 0);
  MM3_MM2   = ($3 shl 6) + ($3 shl 3) + ($2 shl 0);
  MM4_MM2   = ($3 shl 6) + ($4 shl 3) + ($2 shl 0);
  MM5_MM2   = ($3 shl 6) + ($5 shl 3) + ($2 shl 0);
  MM6_MM2   = ($3 shl 6) + ($6 shl 3) + ($2 shl 0);
  MM7_MM2   = ($3 shl 6) + ($7 shl 3) + ($2 shl 0);
  MM0_MM3   = ($3 shl 6) + ($0 shl 3) + ($3 shl 0);
  MM1_MM3   = ($3 shl 6) + ($1 shl 3) + ($3 shl 0);
  MM2_MM3   = ($3 shl 6) + ($2 shl 3) + ($3 shl 0);
  MM3_MM3   = ($3 shl 6) + ($3 shl 3) + ($3 shl 0);
  MM4_MM3   = ($3 shl 6) + ($4 shl 3) + ($3 shl 0);
  MM5_MM3   = ($3 shl 6) + ($5 shl 3) + ($3 shl 0);
  MM6_MM3   = ($3 shl 6) + ($6 shl 3) + ($3 shl 0);
  MM7_MM3   = ($3 shl 6) + ($7 shl 3) + ($3 shl 0);
  MM0_MM4   = ($3 shl 6) + ($0 shl 3) + ($4 shl 0);
  MM1_MM4   = ($3 shl 6) + ($1 shl 3) + ($4 shl 0);
  MM2_MM4   = ($3 shl 6) + ($2 shl 3) + ($4 shl 0);
  MM3_MM4   = ($3 shl 6) + ($3 shl 3) + ($4 shl 0);
  MM4_MM4   = ($3 shl 6) + ($4 shl 3) + ($4 shl 0);
  MM5_MM4   = ($3 shl 6) + ($5 shl 3) + ($4 shl 0);
  MM6_MM4   = ($3 shl 6) + ($6 shl 3) + ($4 shl 0);
  MM7_MM4   = ($3 shl 6) + ($7 shl 3) + ($4 shl 0);
  MM0_MM5   = ($3 shl 6) + ($0 shl 3) + ($5 shl 0);
  MM1_MM5   = ($3 shl 6) + ($1 shl 3) + ($5 shl 0);
  MM2_MM5   = ($3 shl 6) + ($2 shl 3) + ($5 shl 0);
  MM3_MM5   = ($3 shl 6) + ($3 shl 3) + ($5 shl 0);
  MM4_MM5   = ($3 shl 6) + ($4 shl 3) + ($5 shl 0);
  MM5_MM5   = ($3 shl 6) + ($5 shl 3) + ($5 shl 0);
  MM6_MM5   = ($3 shl 6) + ($6 shl 3) + ($5 shl 0);
  MM7_MM5   = ($3 shl 6) + ($7 shl 3) + ($5 shl 0);
  MM0_MM6   = ($3 shl 6) + ($0 shl 3) + ($6 shl 0);
  MM1_MM6   = ($3 shl 6) + ($1 shl 3) + ($6 shl 0);
  MM2_MM6   = ($3 shl 6) + ($2 shl 3) + ($6 shl 0);
  MM3_MM6   = ($3 shl 6) + ($3 shl 3) + ($6 shl 0);
  MM4_MM6   = ($3 shl 6) + ($4 shl 3) + ($6 shl 0);
  MM5_MM6   = ($3 shl 6) + ($5 shl 3) + ($6 shl 0);
  MM6_MM6   = ($3 shl 6) + ($6 shl 3) + ($6 shl 0);
  MM7_MM6   = ($3 shl 6) + ($7 shl 3) + ($6 shl 0);
  MM0_MM7   = ($3 shl 6) + ($0 shl 3) + ($7 shl 0);
  MM1_MM7   = ($3 shl 6) + ($1 shl 3) + ($7 shl 0);
  MM2_MM7   = ($3 shl 6) + ($2 shl 3) + ($7 shl 0);
  MM3_MM7   = ($3 shl 6) + ($3 shl 3) + ($7 shl 0);
  MM4_MM7   = ($3 shl 6) + ($4 shl 3) + ($7 shl 0);
  MM5_MM7   = ($3 shl 6) + ($5 shl 3) + ($7 shl 0);
  MM6_MM7   = ($3 shl 6) + ($6 shl 3) + ($7 shl 0);
  MM7_MM7   = ($3 shl 6) + ($7 shl 3) + ($7 shl 0);
//Define MMX ModR/W for MMX_resisters and memory operation /w General resisters
  MM0_mEAX  = ($0 shl 6) + ($0 shl 3) + ($0 shl 0);
  MM1_mEAX  = ($0 shl 6) + ($1 shl 3) + ($0 shl 0);
  MM2_mEAX  = ($0 shl 6) + ($2 shl 3) + ($0 shl 0);
  MM3_mEAX  = ($0 shl 6) + ($3 shl 3) + ($0 shl 0);
  MM4_mEAX  = ($0 shl 6) + ($4 shl 3) + ($0 shl 0);
  MM5_mEAX  = ($0 shl 6) + ($5 shl 3) + ($0 shl 0);
  MM6_mEAX  = ($0 shl 6) + ($6 shl 3) + ($0 shl 0);
  MM7_mEAX  = ($0 shl 6) + ($7 shl 3) + ($0 shl 0);
  MM0_mECX  = ($0 shl 6) + ($0 shl 3) + ($1 shl 0);
  MM1_mECX  = ($0 shl 6) + ($1 shl 3) + ($1 shl 0);
  MM2_mECX  = ($0 shl 6) + ($2 shl 3) + ($1 shl 0);
  MM3_mECX  = ($0 shl 6) + ($3 shl 3) + ($1 shl 0);
  MM4_mECX  = ($0 shl 6) + ($4 shl 3) + ($1 shl 0);
  MM5_mECX  = ($0 shl 6) + ($5 shl 3) + ($1 shl 0);
  MM6_mECX  = ($0 shl 6) + ($6 shl 3) + ($1 shl 0);
  MM7_mECX  = ($0 shl 6) + ($7 shl 3) + ($1 shl 0);
  MM0_mEDX  = ($0 shl 6) + ($0 shl 3) + ($2 shl 0);
  MM1_mEDX  = ($0 shl 6) + ($1 shl 3) + ($2 shl 0);
  MM2_mEDX  = ($0 shl 6) + ($2 shl 3) + ($2 shl 0);
  MM3_mEDX  = ($0 shl 6) + ($3 shl 3) + ($2 shl 0);
  MM4_mEDX  = ($0 shl 6) + ($4 shl 3) + ($2 shl 0);
  MM5_mEDX  = ($0 shl 6) + ($5 shl 3) + ($2 shl 0);
  MM6_mEDX  = ($0 shl 6) + ($6 shl 3) + ($2 shl 0);
  MM7_mEDX  = ($0 shl 6) + ($7 shl 3) + ($2 shl 0);
  MM0_mEBX  = ($0 shl 6) + ($0 shl 3) + ($3 shl 0);
  MM1_mEBX  = ($0 shl 6) + ($1 shl 3) + ($3 shl 0);
  MM2_mEBX  = ($0 shl 6) + ($2 shl 3) + ($3 shl 0);
  MM3_mEBX  = ($0 shl 6) + ($3 shl 3) + ($3 shl 0);
  MM4_mEBX  = ($0 shl 6) + ($4 shl 3) + ($3 shl 0);
  MM5_mEBX  = ($0 shl 6) + ($5 shl 3) + ($3 shl 0);
  MM6_mEBX  = ($0 shl 6) + ($6 shl 3) + ($3 shl 0);
  MM7_mEBX  = ($0 shl 6) + ($7 shl 3) + ($3 shl 0);
  MM0_mESI  = ($0 shl 6) + ($0 shl 3) + ($6 shl 0);
  MM1_mESI  = ($0 shl 6) + ($1 shl 3) + ($6 shl 0);
  MM2_mESI  = ($0 shl 6) + ($2 shl 3) + ($6 shl 0);
  MM3_mESI  = ($0 shl 6) + ($3 shl 3) + ($6 shl 0);
  MM4_mESI  = ($0 shl 6) + ($4 shl 3) + ($6 shl 0);
  MM5_mESI  = ($0 shl 6) + ($5 shl 3) + ($6 shl 0);
  MM6_mESI  = ($0 shl 6) + ($6 shl 3) + ($6 shl 0);
  MM7_mESI  = ($0 shl 6) + ($7 shl 3) + ($6 shl 0);
  MM0_mEDI  = ($0 shl 6) + ($0 shl 3) + ($7 shl 0);
  MM1_mEDI  = ($0 shl 6) + ($1 shl 3) + ($7 shl 0);
  MM2_mEDI  = ($0 shl 6) + ($2 shl 3) + ($7 shl 0);
  MM3_mEDI  = ($0 shl 6) + ($3 shl 3) + ($7 shl 0);
  MM4_mEDI  = ($0 shl 6) + ($4 shl 3) + ($7 shl 0);
  MM5_mEDI  = ($0 shl 6) + ($5 shl 3) + ($7 shl 0);
  MM6_mEDI  = ($0 shl 6) + ($6 shl 3) + ($7 shl 0);
  MM7_mEDI  = ($0 shl 6) + ($7 shl 3) + ($7 shl 0);
//define PSXX operand for MMX resister and imm8 data
  MM0_SRLim = $D0;
  MM1_SRLim = $D1;
  MM2_SRLim = $D2;
  MM3_SRLim = $D3;
  MM4_SRLim = $D4;
  MM5_SRLim = $D5;
  MM6_SRLim = $D6;
  MM7_SRLim = $D7;
  MM0_SLLim = $F0;
  MM1_SLLim = $F1;
  MM2_SLLim = $F2;
  MM3_SLLim = $F3;
  MM4_SLLim = $F4;
  MM5_SLLim = $F5;
  MM6_SLLim = $F6;
  MM7_SLLim = $F7;
  MM0_SRAim = $E0;
  MM1_SRAim = $E1;
  MM2_SRAim = $E2;
  MM3_SRAim = $E3;
  MM4_SRAim = $E4;
  MM5_SRAim = $E5;
  MM6_SRAim = $E6;
  MM7_SRAim = $E7;
//***omitted using SIB mode and disp8(xx), disp32(xx)
//--------------------------------------
//

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('TRSOFT', [TPentiumMMX]);
end;

constructor TPentiumMMX.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
end;

function TPentiumMMX.GetCpuIDsupport: Boolean;
begin
  asm
     push  ebx                //save ebx
     mov  @result, true
     pushfd                   //Get original EFLAGS
     pop  eax
     mov  ecx, eax
     xor  eax, $00200000      // Flip ID(bit21) in EFLAGS
     push  eax
     popfd
     pushfd		      // Get new EFLAGS
     pop  eax
     xor  eax, ecx	      // if not toggled ID, CPUID not supported
     jnz  @exit
     mov  @result, false
  @exit:
     pop ebx                  //load ebx
  end;
end;

function TPentiumMMX.GetRDTSCsupport: Boolean;
begin
  Result := false;
  if CPUIDsupport = true
   then
       begin
            if (GetFeatureFlags and $00000010) <> 0
             then
                 begin
                      Result := true;
                 end;
       end;
end;

function TPentiumMMX.GetMMXsupport: Boolean;
begin
  Result := false;
  if CPUIDsupport = true
   then
       begin
            if (GetFeatureFlags and $00800000) <> 0
             then
                 begin
                      Result := true;
                 end;
       end;
end;

function TPentiumMMX.GetVendorString: String;
var
   _Vendor: TVendorStr;
   _VendorStr: String;
   m, n: Integer;
begin
  asm
     push  ebx                         //save ebx
     xor   eax, eax
     DW    CPUID
     mov   DWord ptr _Vendor, ebx
     mov   DWord ptr _Vendor[+4], edx
     mov   DWord ptr _Vendor[+8], ecx
     pop   ebx                         //load ebx
  end;
  _VendorStr := '';
  for n := 0 to 2 do
   begin
        for m := 0 to 3 do
         begin
              _VendorStr := _VendorStr
                   + chr((_Vendor[n] and ($000000FF shl (m*8))) shr (m*8));
         end;
   end;
  Result := _VendorStr;
end;

function TPentiumMMX.GetSteppingID: Byte;
var
   _xError: Boolean;
   _SteppingID: Byte;
   _Model: Byte;
   _Family: Word;
   _FeatureFlags: DWord;
   Error: EPentiumMMXError;
begin
  asm
     push  ebx                      //save ebx
     mov  _xError, true
  //********************************//execute CPUID case0 //
     xor  eax, eax
     DW   CPUID
     cmp  eax, 1		    //check hv (if Pentium, hv := 1)
     jl   @exit
  //********************************//execute CPUID case1
     xor  eax, eax
     inc  eax
     DW   CPUID
     mov  _SteppingID, al
     and  _SteppingID, $0F
     and  al, $F0
     shr  al, 4
     mov  _Model, al
     and  eax, $0F00
     shr  eax, 8
     and  eax, $0F
     mov  _Family, ax
     mov  _FeatureFlags, edx
     mov  _xError, false
   @exit:
     pop  ebx                       //load ebx
  end;
  if (_xError = true) and (ErrorEnable = true)
   then
       begin
            Error := EPentiumMMXError.Create('CPU not supported SteppingID');
            Error.ErrorCode := $00010001;
         raise Error;
       end;
  Result := _SteppingID;
end;

function TPentiumMMX.GetModel: Byte;
var
   _xError: Boolean;
   _SteppingID: Byte;
   _Model: Byte;
   _Family: Word;
   _FeatureFlags: DWord;
   Error: EPentiumMMXError;
begin
  asm
     push  ebx                       //save ebx
     mov  _xError, true
  //*********************************//execute CPUID case0
     xor  eax, eax
     DW   CPUID
     cmp  eax, 1		     //check hv (if Pentium, hv := 1)
     jl   @exit
  //*********************************//execute CPUID case1
     xor  eax, eax
     inc  eax
     DW   CPUID
     mov  _SteppingID, al
     and  _SteppingID, $0F
     and  al, $F0
     shr  al, 4
     mov  _Model, al
     and  eax, $0F00
     shr  eax, 8
     and  eax, $0F
     mov  _Family, ax
     mov  _FeatureFlags, edx
     mov  _xError, false
   @exit:
     pop  ebx                        //load ebx
  end;
  if (_xError = true) and (ErrorEnable = true)
   then
       begin
            Error := EPentiumMMXError.Create('CPU not supported Model');
            Error.ErrorCode := $00010001;
         raise Error;
       end;
  Result := _Model;
end;

function TPentiumMMX.GetFamily: Word;
var
   _xError: Boolean;
   _SteppingID: Byte;
   _Model: Byte;
   _Family: Word;
   _FeatureFlags: DWord;
   Error: EPentiumMMXError;
begin
  asm
     push  ebx                       //save ebx
     mov  _xError, true
  //*********************************//execute CPUID case0
     xor  eax, eax
     DW   CPUID
     cmp  eax, 1		     //check hv (if Pentium, hv := 1)
     jl   @exit
  //*********************************//execute CPUID case1
     xor  eax, eax
     inc  eax
     DW   CPUID
     mov  _SteppingID, al
     and  _SteppingID, $0F
     and  al, $F0
     shr  al, 4
     mov  _Model, al
     and  eax, $0F00
     shr  eax, 8
     and  eax, $0F
     mov  _Family, ax
     mov  _FeatureFlags, edx
     mov  _xError, false
   @exit:
     pop  ebx                        //load ebx
  end;
  if (_xError = true) and (ErrorEnable = true)
   then
       begin
            Error := EPentiumMMXError.Create('CPU not supported Family');
            Error.ErrorCode := $00010001;
         raise Error;
       end;
  Result := _Family;
end;

function TPentiumMMX.GetFeatureFlags: DWord;
var
   _xError: Boolean;
   _SteppingID: Byte;
   _Model: Byte;
   _Family: Word;
   _FeatureFlags: DWord;
   Error: EPentiumMMXError;
begin
  asm
     push  ebx                       //save ebx
     mov  _xError, true
  //*********************************//execute CPUID case0
     xor  eax, eax
     DW   CPUID
     cmp  eax, 1		     //check hv (if Pentium, hv := 1)
     jl   @exit
  //*********************************//execute CPUID case1
     xor  eax, eax
     inc  eax
     DW   CPUID
     mov  _SteppingID, al
     and  _SteppingID, $0F
     and  al, $F0
     shr  al, 4
     mov  _Model, al
     and  eax, $0F00
     shr  eax, 8
     and  eax, $0F
     mov  _Family, ax
     mov  _FeatureFlags, edx
     mov  _xError, false
   @exit:
     pop  ebx                        //load ebx
  end;
  if (_xError = true) and (ErrorEnable = true)
   then
       begin
            Error := EPentiumMMXError.Create('CPU not supported FeatureFlags');
            Error.ErrorCode := $00010001;
         raise Error;
       end;
  Result := _FeatureFlags;
end;

function TPentiumMMX.GetTimeStampHigh: integer;
var
   _TimeStampHigh: Integer;
begin
  asm
     DW   RDTSC
     mov  _TimeStampHigh, edx
  end;
  Result := _TimeStampHigh;
end;

function TPentiumMMX.GetTimeStampLow:  integer;
var
   _TimeStampLow: Integer;
begin
  asm
     DW   RDTSC
     mov  _TimeStampLow, eax
  end;
  Result := _TimeStampLow;
end;

function TPentiumMMX.Interval32(X:Integer; Y:Integer):Integer;
var
   xh,xl,yh,yl,zh,zl: integer;
begin
     xh := (X and $ffff0000) shr $10;
     xl := (X and $0000ffff);
     yh := (Y and $ffff0000) shr $10;
     yl := (Y and $0000ffff);
  if xl < yl then
    begin
         xh := xh -1;
         zl := xl + not($ffff0000 or yl)  +1;
    end
  else
    begin
         zl := xl - yl;
    end;
  if xh < yh then
    begin
         zh := xh + not($ffff0000 or yh) +1;
    end
  else
    begin
         zh := xh - yh;
    end;
  Result := ((zh shl $10)and $ffff0000) or (zl and $0000ffff);
end;

function TPentiumMMX.Interval64(X:QuadWord; Y:QuadWord):QuadWord;
var
   xhh,xhl,xlh,xll,yhh,yhl,ylh,yll,zhh,zhl,zlh,zll: integer;
begin
     xhh := (X.high and $ffff0000) shr $10;
     xhl := (X.high and $0000ffff);
     xlh := (X.low  and $ffff0000) shr $10;
     xll := (X.low  and $0000ffff);
     yhh := (Y.high and $ffff0000) shr $10;
     yhl := (Y.high and $0000ffff);
     ylh := (Y.low  and $ffff0000) shr $10;
     yll := (Y.low  and $0000ffff);
  if xll < yll then
    begin
         xlh := xlh -1;
         zll := xll + not($ffff0000 or yll) +1;
    end
  else
    begin
         zll := xll - yll;
    end;
  if xlh < ylh then
    begin
         xhl := xhl -1;
         zlh := xlh + not($ffff0000 or ylh) +1;
    end
  else
    begin
         zlh := xlh - ylh;
    end;
  if xhl < yhl then
    begin
         xhh := xhh -1;
         zhl := xhl + not($ffff0000 or yhl) +1;
    end
  else
    begin
         zhl := xhl - yhl;
    end;
  if xhh < yhh then
    begin
         zhh := xhh + not($ffff0000 or yhh) +1;
    end
  else
    begin
         zhh := xhh - yhh;
    end;
  Result.high := ((zhh shl $10)and $ffff0000) or (zhl and $0000ffff);
  Result.low  := ((zlh shl $10)and $ffff0000) or (zll and $0000ffff);
end;

function TPentiumMMX.MmxOperate(inst:TMMXinst; X:QuadWord; Y:QuadWord):QuadWord;
var
   Z: QuadWord;
   addr_X,addr_Y,addr_Z: ^QuadWord;
begin
     Result.high := $00000000;
     Result.low  := $00000000;
     Z.high := $00000000;
     Z.low  := $00000000;
     addr_X := @X;
     addr_Y := @Y;
     addr_Z := @Z;
  case inst of
  _PAND:  asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  PAND
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _PANDN: asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  PANDN
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _POR:   asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  POR
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _PXOR:  asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  PXOR
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _PSRLw: asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  PSRLw
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _PSRLd: asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  PSRLd
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _PSRLq: asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  PSRLq
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _PSRAw: asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  PSRAw
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _PSRAd: asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  PSRAd
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _PSLLw: asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  PSLLw
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _PSLLd: asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  PSLLd
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _PSLLq: asm
            push ebx
            //  MMX
            dw  EMMS

            mov eax, addr_X
            mov ecx, addr_Y
            mov edx, addr_Z

            dw  MOVQ_ld
            db  MM0_mEAX

            dw  MOVQ_ld
            db  MM1_mECX

            dw  PSLLq
            db  MM0_MM1

            dw  MOVQ_st
            db  MM0_mEDX

            dw  EMMS
            //
            pop ebx
          end;
  _PCMPGTb: asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PCMPGTb
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PCMPGTw: asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PCMPGTw
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PCMPGTd: asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PCMPGTd
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PCMPEQb: asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PCMPEQb
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PCMPEQw: asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PCMPEQw
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PCMPEQd: asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PCMPEQd
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PACKSSwb:  asm
                push ebx
                //  MMX
                dw  EMMS

                mov eax, addr_X
                mov ecx, addr_Y
                mov edx, addr_Z

                dw  MOVQ_ld
                db  MM0_mEAX

                dw  MOVQ_ld
                db  MM1_mECX

                dw  PACKSSwb
                db  MM0_MM1

                dw  MOVQ_st
                db  MM0_mEDX

                dw  EMMS
                //
                pop ebx
              end;
  _PACKSSdw:  asm
                push ebx
                //  MMX
                dw  EMMS

                mov eax, addr_X
                mov ecx, addr_Y
                mov edx, addr_Z

                dw  MOVQ_ld
                db  MM0_mEAX

                dw  MOVQ_ld
                db  MM1_mECX

                dw  PACKSSdw
                db  MM0_MM1

                dw  MOVQ_st
                db  MM0_mEDX

                dw  EMMS
                //
                pop ebx
              end;
  _PACKUSwb:  asm
                push ebx
                //  MMX
                dw  EMMS

                mov eax, addr_X
                mov ecx, addr_Y
                mov edx, addr_Z

                dw  MOVQ_ld
                db  MM0_mEAX

                dw  MOVQ_ld
                db  MM1_mECX

                dw  PACKUSwb
                db  MM0_MM1

                dw  MOVQ_st
                db  MM0_mEDX

                dw  EMMS
                //
                pop ebx
              end;
  _PUNPCKLbw: asm
                push ebx
                //  MMX
                dw  EMMS

                mov eax, addr_X
                mov ecx, addr_Y
                mov edx, addr_Z

                dw  MOVQ_ld
                db  MM0_mEAX

                dw  MOVQ_ld
                db  MM1_mECX

                dw  PUNPCKLbw
                db  MM0_MM1

                dw  MOVQ_st
                db  MM0_mEDX

                dw  EMMS
                //
                pop ebx
              end;
  _PUNPCKLwd: asm
                push ebx
                //  MMX
                dw  EMMS

                mov eax, addr_X
                mov ecx, addr_Y
                mov edx, addr_Z

                dw  MOVQ_ld
                db  MM0_mEAX

                dw  MOVQ_ld
                db  MM1_mECX

                dw  PUNPCKLwd
                db  MM0_MM1

                dw  MOVQ_st
                db  MM0_mEDX

                dw  EMMS
                //
                pop ebx
              end;
  _PUNPCKLdq: asm
                push ebx
                //  MMX
                dw  EMMS

                mov eax, addr_X
                mov ecx, addr_Y
                mov edx, addr_Z

                dw  MOVQ_ld
                db  MM0_mEAX

                dw  MOVQ_ld
                db  MM1_mECX

                dw  PUNPCKLdq
                db  MM0_MM1

                dw  MOVQ_st
                db  MM0_mEDX

                dw  EMMS
                //
                pop ebx
              end;
  _PUNPCKHbw: asm
                push ebx
                //  MMX
                dw  EMMS

                mov eax, addr_X
                mov ecx, addr_Y
                mov edx, addr_Z

                dw  MOVQ_ld
                db  MM0_mEAX

                dw  MOVQ_ld
                db  MM1_mECX

                dw  PUNPCKHbw
                db  MM0_MM1

                dw  MOVQ_st
                db  MM0_mEDX

                dw  EMMS
                //
                pop ebx
              end;
  _PUNPCKHwd: asm
                push ebx
                //  MMX
                dw  EMMS

                mov eax, addr_X
                mov ecx, addr_Y
                mov edx, addr_Z

                dw  MOVQ_ld
                db  MM0_mEAX

                dw  MOVQ_ld
                db  MM1_mECX

                dw  PUNPCKHwd
                db  MM0_MM1

                dw  MOVQ_st
                db  MM0_mEDX

                dw  EMMS
                //
                pop ebx
              end;
  _PUNPCKHdq: asm
                push ebx
                //  MMX
                dw  EMMS

                mov eax, addr_X
                mov ecx, addr_Y
                mov edx, addr_Z

                dw  MOVQ_ld
                db  MM0_mEAX

                dw  MOVQ_ld
                db  MM1_mECX

                dw  PUNPCKHdq
                db  MM0_MM1

                dw  MOVQ_st
                db  MM0_mEDX

                dw  EMMS
                //
                pop ebx
              end;
  _PADDUSb: asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PADDUSb
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PADDUSw: asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PADDUSw
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PADDSb:  asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PADDSb
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PADDSw:  asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PADDSw
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PADDb:   asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PADDb
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PADDw:   asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PADDw
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PADDd:   asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PADDd
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PSUBUSb: asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PSUBUSb
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PSUBUSw: asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PSUBUSw
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PSUBSb:  asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PSUBSb
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PSUBSw:  asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PSUBSw
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PSUBb:   asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PSUBb
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PSUBw:   asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PSUBw
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PSUBd:   asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PSUBd
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PMULLw:  asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PMULLw
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PMULHw:  asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PMULHw
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  _PMADDwd: asm
              push ebx
              //  MMX
              dw  EMMS

              mov eax, addr_X
              mov ecx, addr_Y
              mov edx, addr_Z

              dw  MOVQ_ld
              db  MM0_mEAX

              dw  MOVQ_ld
              db  MM1_mECX

              dw  PMADDwd
              db  MM0_MM1

              dw  MOVQ_st
              db  MM0_mEDX

              dw  EMMS
              //
              pop ebx
            end;
  end;
  Result := Z;
end;

end.

//------------------------------------------------------------------------------
// Technical Information
//------------------------------------------------------------------------------
 {
<Intel Processor Signatures>
----------------------------------------------------------------------------------------------------
Type  / Family / Model     / SteppingID / description
----------------------------------------------------------------------------------------------------
00    / 0100   / 0011      / xxxx       / IntelDX2
00    / 0100   / 0101      / xxxx       / IntelSX2
00    / 0100   / 1000      / xxxx       / IntelDX4
00    / 0101   / 0001      / xxxx       / Pentium (60, 66)
00    / 0101   / 0010      / xxxx       / Pentium (75, 90, 100, 120, 133, 150, 166, 200)
00    / 0101   / 0100      / xxxx       / Pentium with MMX (166, 200)
00    / 0110   / 0001      / xxxx       / Pentium Pro
00    / 0110   / 0011      / xxxx       / Pentium II
----------------------------------------------------------------------------------------------------

<Feature Flag Values>
----------------------------------------------------------------------------------------------------
Bit / Name / Description when Flag = 1       / ÉRÉÅÉìÉg
----------------------------------------------------------------------------------------------------
00  / FPU  / Floating-point unit on-chip     / i387Ç
01  / VME  / Virtual Mode Extension          / z8086
02  / DE   / Debugging Extension             / (CR4-DE for DR4, DR5)
03  / PSE  / Page Size Extension             / 4Mbyte
04  / TSC  / Time Stamp Counter              / RDTSC (CR4-TSD)
05  / MSR  / Model Specific Registers        / RDMSR, WRMSR
06  / PAE  / Physical Address Extension      / (over 32bit-address)
07  / MCE  / Machine Check Exception         / (Exception 18, CR4-MCE)
08  / CX8  / CMPXCHG8 Instruction Supported  / CMPXCHG8
09  / APIC / On-chip APIC Hardware Supported /
10  /      /
 ~  / ---- / Reserved                        /
11  /      /
12  / MTRR / Memory Type Range Registers     / (MTRR_CAP)
13  / PGE  / Page Global Enable              / (PDEs, PTEs, CR4-PGE)
14  / MCA  / Machine Check Architecture      / (MCG_CAP)
15  / CMOV / Conditional Move Supported      / CMOVcc,FCMOVcc, FCOMI
16  /      /
 ~  / ---- / Reserved                        /
22  /      /
23  / MMX  / Intel MMX supported             / MMX
24  /      /
 ~  / ---- / Reserved                        /
31  /      /
----------------------------------------------------------------------------------------------------


<Supported MMX Instructions in public function>
----------------------------------------------------------------------------------------------------
equation function ---> Z := MmxOperate(instruction:TMMXinst; X:QuadWord; Y:QuadWord):QuadWord;
----------------------------------------------------------------------------------------------------
No / Instruction / Operation
----------------------------------------------------------------------------------------------------
01 / _PAND       / Z := X and Y
02 / _PANDN      / Z := not(X) and Y
03 / _POR        / Z := X or Y
04 / _PXOR       / Z := X xor Y
05 / _PSRLw      / Z := X(unit 16bit) shr Y
06 / _PSRLd      / Z := X(unit 32bit) shr Y
07 / _PSRLq      / Z := X(unit 64bit) shr Y
08 / _PSRAw      / Z := X(unit 16bit) shr Y (arithmetical)
09 / _PSRAd      / Z := X(unit 32bit) shr Y (arithmetical)
10 / _PSLLw      / Z := X(unit 16bit) shl Y
11 / _PSLLd      / Z := X(unit 32bit) shl Y
12 / _PSLLq      / Z := X(unit 64bit) shl Y
13 / _PCMPGTb    / if X > Y then Z := $ff       else Z := $00;      (x,y,z: unit 16bit)
14 / _PCMPGTw    / if X > Y then Z := $ffff     else Z := $0000;    (x,y,z: unit 32bit)
15 / _PCMPGTd    / if X > Y then Z := $ffffffff else Z := $00000000;(x,y,z: unit 64bit)
16 / _PCMPEQb    / if X = Y then Z := $ff       else Z := $00;      (x,y,z: unit 16bit)
17 / _PCMPEQw    / if X = Y then Z := $ffff     else Z := $0000;    (x,y,z: unit 32bit)
18 / _PCMPEQd    / if X = Y then Z := $ffffffff else Z := $00000000;(x,y,z: unit 64bit)
19 / _PACKSSwb   / Z := (byte(Y3)shl  8)+(byte(Y2)shl 16)+(byte(Y1)shl 24)+(byte(Y0)shl 32)+(byte(X3)shl 24)+(byte(X2)shr 16)+(byte(X1)shr 8)+byte(X0)  (x,y: unit 16bit, signed operation)
20 / _PACKSSdw   / Z := (word(Y1)shl 16)+(word(Y0)shl 32)+(word(X1)shr 16)+word(X0)                                                                     (x,y: unit 32bit, signed operation)
21 / _PACKUSwb   / Z := (byte(Y3)shl  8)+(byte(Y2)shl 16)+(byte(Y1)shl 24)+(byte(Y0)shl 32)+(byte(X3)shl 24)+(byte(X2)shr 16)+(byte(X1)shr 8)+byte(X0)  (x,y: unit 16bit, unsigned operation)
22 / _PUNPCKLbw  / Z := (Y3 shl 32)+(X3 shl 24)+(Y2 shl 24)+(X2 shl 16)+(Y1 shl 16)+(X1 shl  8)+(Y0 shl  8)+X0  (x,y: unit  8bit, z: unit 64bit)
23 / _PUNPCKLwd  / Z := (Y1 shl 32)+(X1 shl 16)+(Y0 shl 16)+X0                                                  (x,y: unit 16bit, z: unit 64bit)
24 / _PUNPCKLdq  / Z := (Y0 shl 32)+X0                                                                          (x,y: unit 32bit, z: unit 64bit)
25 / _PUNPCKHbw  / Z := Y7+(X7 shr  8)+(Y6 shr  8)+(X6 shr 16)+(Y5 shr 16)+(X5 shr 24)+(Y4 shr 24)+(X4 shr 24)  (x,y: unit  8bit, z: unit 64bit)
26 / _PUNPCKHwd  / Z := Y3+(X3 shr 16)+(Y2 shr 16)+(X2 shr 32)                                                  (x,y: unit 16bit, z: unit 64bit)
27 / _PUNPCKHdq  / Z := Y1+(X1 shr 32)                                                                          (x,y: unit 32bit, z: unit 64bit)
28 / _PADDUSb    / Z := X + Y (x,y,z: unit  8bit, unsigned)
29 / _PADDUSw    / Z := X + Y (x,y,z: unit 16bit, unsigned)
30 / _PADDSb     / Z := X + Y (x,y,z: unit  8bit, signed)
31 / _PADDSw     / Z := X + Y (x,y,z: unit 16bit, signed)
32 / _PADDb      / Z := X + Y (x,y,z: unit  8bit)
33 / _PADDw      / Z := X + Y (x,y,z: unit 16bit)
34 / _PADDd      / Z := X + Y (x,y,z: unit 32bit)
35 / _PSUBUSb    / Z := X - Y (x,y,z: unit  8bit, unsigned)
36 / _PSUBUSw    / Z := X - Y (x,y,z: unit 16bit, unsigned)
37 / _PSUBSb     / Z := X - Y (x,y,z: unit  8bit, signed)
38 / _PSUBSw     / Z := X - Y (x,y,z: unit 16bit, signed)
39 / _PSUBb      / Z := X - Y (x,y,z: unit  8bit)
40 / _PSUBw      / Z := X - Y (x,y,z: unit 16bit)
41 / _PSUBd      / Z := X - Y (x,y,z: unit 32bit)
42 / _PMULLw     / Z := (X * Y) and $0000ffff           (x,y,z: unit 16bit)
43 / _PMULHw     / Z := ((X * Y) and $ffff0000) shr 16  (x,y,z: unit 16bit)
44 / _PMADDwd    / Z := (X1 * Y1) + (X0 * Y0)           (x,y: unit 16bit, z: unit 32bit))
----------------------------------------------------------------------------------------------------
 }

