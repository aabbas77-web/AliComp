unit Main;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  PentiumMMX, StdCtrls;

type
  TFormMain = class(TForm)
    PentiumMMX1: TPentiumMMX;
    Button1: TButton;
    Edit1: TEdit;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FormMain: TFormMain;

implementation

{$R *.DFM}

procedure TFormMain.Button1Click(Sender: TObject);
begin
  Edit1.Text:=PentiumMMX1.VendorString;
end;

end.
