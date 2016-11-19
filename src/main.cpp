#include <stdio.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>


#include "main_frame.h"


int MiniGUIMain (int argc, const char* argv[])
{
	MainFrame mf;
	
	mf.create(480, 272);
	
	return 0;
}
