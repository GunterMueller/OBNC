/*GENERATED BY OBNC 0.9.1*/

#include <obnc/OBNC.h>
#include "Input.h"
#include "Out.h"
#include "XYplane.h"

int main(int argc, char *argv[])
{
	OBNC_Initialize(argc, argv);
	Input_Init();
	Out_Init();
	XYplane_Init();
	return 0;
}
