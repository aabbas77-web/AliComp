#include <dos.h>
#include <iostream.h>
#include <conio.h>
#include <graphics.h>
#include <bios.h>
#include <math.h>


static double cosa = 0.5;
static double sina = -sqrt(3.0)/2.0;


void Koch (
	int depth, int level,
	double x1, double y1, double x2, double y2 )
{
	if ( level == depth )
	{
		line ( int(x1), int(y1), int(x2), int(y2) );
	}
	else
	{
		double dx = (x2 - x1)/3;
		double dy = (y2 - y1)/3;
		double xa = x1 + dx;
		double ya = y1 + dy;
		double xb = x2 - dx;
		double yb = y2 - dy;
		double xc = xa + dx*cosa - dy*sina;
		double yc = ya + dy*cosa + dx*sina;
		Koch ( depth+1, level, x1, y1, xa, ya );
		Koch ( depth+1, level, xa, ya, xc, yc );
		Koch ( depth+1, level, xc, yc, xb, yb );
		Koch ( depth+1, level, xb, yb, x2, y2 );
	}
}


int main()
{
	text_info startup;
	gettextinfo ( &startup );

	int driver=VGA, mode=VGAHI;
	registerbgidriver ( EGAVGA_driver );
	initgraph ( &driver, &mode, "" );

	setcolor (WHITE);
	for ( int level=0; level<7; level++ )
	{
		cleardevice();
		Koch ( 0, level, 0, getmaxy(), getmaxx(), getmaxy() );
		delay(700);
	}

	delay(1);
	sound(700);
	delay(50);
	nosound();
	bioskey(0);
	closegraph();
	textmode ( startup.currmode );

	return 0;
}
