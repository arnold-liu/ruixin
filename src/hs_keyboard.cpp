
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/ctrl/ctrlhelper.h>
#include <minigui/ctrl/button.h>
#include <minigui/ctrl/edit.h>
#include <minigui/ctrl/static.h>


#include "gbl_define.h"
#include "hs_keyboard.h"

#include "main_frame.h"








enum
{	
	IDC_IME_EDIT_SHOW,
	IDC_IME_BTN_A,
	IDC_IME_BTN_B,
	IDC_IME_BTN_C,
	IDC_IME_BTN_D,
	IDC_IME_BTN_E,
	IDC_IME_BTN_F,
	IDC_IME_BTN_G,

	IDC_IME_BTN_H,
	IDC_IME_BTN_I,
	IDC_IME_BTN_J,
	IDC_IME_BTN_K,
	IDC_IME_BTN_L,
	IDC_IME_BTN_M,
	IDC_IME_BTN_N,
	
	IDC_IME_BTN_O,
	IDC_IME_BTN_P,
	IDC_IME_BTN_Q,
	IDC_IME_BTN_R,
	IDC_IME_BTN_S,
	IDC_IME_BTN_T,
	IDC_IME_BTN_U,

	IDC_IME_BTN_V,
	IDC_IME_BTN_W,
	IDC_IME_BTN_X,
	IDC_IME_BTN_Y,
	IDC_IME_BTN_Z,	

	IDC_IME_BTN_SWITCH,
	IDC_IME_BTN_SPACE,
	IDC_IME_BTN_DEL,
	IDC_IME_BTN_EXIT,
};


struct soft_keyboard_t
{
	HWND hTarget;
	int x,y;
	int edith;
	char input[256];
};


soft_keyboard_t gsk;


#if 0

/*
type: 0  for en
type: 1  for zh
*/
static void reArrangeBtns(HWND hDlg, int type)
{
#define MYMB_X 0
#define MYMB_Y 0
#define MYMB_W 30
#define MYMB_H 25
#define MYMB_OFFY 0
#define MYMB_OFFX 0


#define MYMBWEIGHT  (MYMB_W*8+2)
#define MYMBHEIGHT  (MYMB_H*4+2+gsk.edith)

	if(type)
	{
		RECT rcWin;
		GetWindowRect(hDlg, &rcWin);
		MoveWindow(hDlg, rcWin.left, rcWin.top, MYMBWEIGHT, MYMBHEIGHT+MYMB_H, TRUE);
		
		for(int fori=IDC_IME_BTN_ZH1; fori<=IDC_IME_BTN_ZH8; fori++)
		{
			HWND cwin=GetDlgItem(hDlg, fori);
			if(!IsWindowVisible(cwin))
			{
				ShowWindow(cwin, SW_SHOW);
			}
		}
	}
	else
	{
		RECT rcWin;
		GetWindowRect(hDlg, &rcWin);
		MoveWindow(hDlg, rcWin.left, rcWin.top, MYMBWEIGHT, MYMBHEIGHT, TRUE);
		
		for(int fori=IDC_IME_BTN_ZH1; fori<=IDC_IME_BTN_ZH8; fori++)
		{
			HWND cwin=GetDlgItem(hDlg, fori);			

			if(fori>=IDC_IME_BTN_ZH3)
			{
				if(IsWindowVisible(cwin))
				{				
					ShowWindow(cwin, SW_HIDE);
				}
			}
			else
			{
				SetWindowText(cwin, " ");
			}
		}
		
	}


#undef MYMB_X
#undef MYMB_Y
#undef MYMB_W
#undef MYMB_H
#undef MYMB_OFFY
#undef MYMB_OFFX


#undef MYMBWEIGHT
#undef MYMBHEIGHT
	
}

#endif


static int softKeyBoardBoxEnProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_INITDIALOG:
			do
			{
				//SetWindowFont (GetDlgItem(hDlg, IDC_IME_BTN_SWITCH), (PLOGFONT)GetSystemFont(4));				
				//SetDlgItemText(hDlg, IDC_IME_BTN_SWITCH, "ABC");
				//SetWindowFont (GetDlgItem(hDlg, IDC_IME_BTN_SPACE), (PLOGFONT)GetSystemFont(0));
				//SetDlgItemText(hDlg, IDC_IME_BTN_SPACE, "space");/*©»©¥©¿*/
				//SetWindowFont (GetDlgItem(hDlg, IDC_IME_BTN_DEL), (PLOGFONT)GetSystemFont(0));
				//SetDlgItemText(hDlg, IDC_IME_BTN_DEL, "del");
				
				char title[2]={0};
				for(int fori=IDC_IME_BTN_A; fori<=IDC_IME_BTN_Z; fori++)
				{				
					title[0]='a'+fori-IDC_IME_BTN_A;

					//SetWindowFont (GetDlgItem(hDlg, fori), (PLOGFONT)GetSystemFont(0));

					if(fori<=IDC_IME_BTN_Z)
					{
						SetDlgItemText(hDlg, fori, title);
					}
				}

				SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_SWITCH));
				

				//gal_pixel bkcolor=GetWindowElementPixel(hDlg, WE_MAINC_THREED_BODY);
				gal_pixel bkcolor=0xffcccccc;
				SetWindowBkColor(GetDlgItem(hDlg, IDC_IME_EDIT_SHOW), bkcolor);				

#if 1
				int len=GetWindowTextLength(gsk.hTarget);
				if(len>0)
				{
					char* buf=(char*)malloc(len+1);
					buf[len]=0;
					GetWindowText(gsk.hTarget, buf, len);
					SetWindowText(GetDlgItem(hDlg, IDC_IME_EDIT_SHOW), buf);
					free(buf);
				}

				int caretpos=SendMessage(gsk.hTarget, EM_GETCARETPOS, 0, 0);
				SendMessage(GetDlgItem(hDlg, IDC_IME_EDIT_SHOW), EM_SETCARETPOS, 0, caretpos);
#endif				
				return 0;
			}while(0);
		break;		
		case MSG_DESTROY:
			do
			{
				int len=GetWindowTextLength(GetDlgItem(hDlg, IDC_IME_EDIT_SHOW));
				if(len>0)
				{
					char* buf=(char*)malloc(len+1);
					buf[len]=0;
					GetWindowText(GetDlgItem(hDlg, IDC_IME_EDIT_SHOW), buf, len);
					SetWindowText(gsk.hTarget, buf);
					free(buf);
				}
				
			}while(0);
		break;
		case MSG_ERASEBKGND:
			return 0;
		break;
		case MSG_KEYDOWN:
			do
			{
				int scancode = (int)wParam;
				HWND hcur=GetFocus(hDlg);
				switch(scancode)
				{
					case SCANCODE_CURSORBLOCKUP:
						do
						{
							int curid=GetDlgCtrlID(hcur);
							switch(curid)
							{
								case IDC_IME_BTN_SWITCH:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_EXIT));
								break;
								case IDC_IME_BTN_SPACE:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_SWITCH));
								break;
								case IDC_IME_BTN_DEL:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_SPACE));
								break;
								case IDC_IME_BTN_EXIT:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_DEL));
								break;
								default:
									curid-=7;
									if(curid>=IDC_IME_BTN_A)
									{							
										SetFocus(GetDlgItem(hDlg, curid));
									}
								break;
							}
						}while(0);
						return 0;
					break;
					case SCANCODE_CURSORBLOCKDOWN:
						do
						{
							int curid=GetDlgCtrlID(hcur);
							switch(curid)
							{
								case IDC_IME_BTN_SWITCH:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_SPACE));
								break;
								case IDC_IME_BTN_SPACE:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_DEL));
								break;
								case IDC_IME_BTN_DEL:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_EXIT));
								break;
								case IDC_IME_BTN_EXIT:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_SWITCH));
								break;
								default:
									curid+=7;
									if(curid<=IDC_IME_BTN_EXIT)
									{
										HWND cwin=GetDlgItem(hDlg, curid);
										if(IsWindowVisible(cwin))
										{
											SetFocus(cwin);
										}
									}
								break;
							}
						}while(0);
						return 0;						
					break;
					case SCANCODE_CURSORBLOCKLEFT:
						do
						{
							if(SendMessage(hcur, MSG_GETDLGCODE, 0, 0) & DLGC_WANTARROWS)
							{
								break;
							}
							int curid=GetDlgCtrlID(hcur);
							switch(curid)
							{
								case IDC_IME_BTN_SWITCH:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_G));
								break;
								case IDC_IME_BTN_SPACE:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_N));
								break;
								case IDC_IME_BTN_DEL:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_U));
								break;
								case IDC_IME_BTN_EXIT:									
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_Z));									
								break;
								case IDC_IME_BTN_A:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_SWITCH));
								break;
								case IDC_IME_BTN_H:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_SPACE));
								break;
								case IDC_IME_BTN_O:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_DEL));
								break;
								case IDC_IME_BTN_V:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_EXIT));
								break;							
								default:
									SetFocus(GetNextDlgTabItem(hDlg, hcur, TRUE));
								break;
							}
							
							return 0;
						}while(0);
					break;
					case SCANCODE_CURSORBLOCKRIGHT:
						do
						{
							if(SendMessage(hcur, MSG_GETDLGCODE, 0, 0) & DLGC_WANTARROWS)
							{
								break;
							}
							int curid=GetDlgCtrlID(hcur);
							switch(curid)
							{
								case IDC_IME_BTN_SWITCH:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_A));
								break;
								case IDC_IME_BTN_SPACE:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_H));
								break;
								case IDC_IME_BTN_DEL:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_O));
								break;
								case IDC_IME_BTN_EXIT:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_V));
								break;
								case IDC_IME_BTN_G:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_SWITCH));
								break;
								case IDC_IME_BTN_N:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_SPACE));
								break;
								case IDC_IME_BTN_U:
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_DEL));
								break;
								case IDC_IME_BTN_Z:									
									SetFocus(GetDlgItem(hDlg, IDC_IME_BTN_EXIT));									
								break;														
								default:
									SetFocus(GetNextDlgTabItem(hDlg, hcur, FALSE));
								break;
							}
							return 0;
						}while(0);
					break;
					case SCANCODE_ESCAPE:
						return quitThisDialog(hDlg, 0);
					break;					
				}
			}while(0);
		break;
		case MSG_COMMAND:
			do
			{
				int id = LOWORD(wParam);
				int code = HIWORD(wParam);
				//HWND hwnd = (HWND)lParam;

				if(code==BN_CLICKED)
				{					
					if(id==IDC_IME_BTN_SWITCH)
					{
						char textSwitch[3]={0};
						GetDlgItemText(hDlg, IDC_IME_BTN_SWITCH, textSwitch, 2);

						if(textSwitch[0]>='a' && textSwitch[0]<='z')
						{
							//reArrangeBtns(hDlg, 0);
							
							SetDlgItemText(hDlg, IDC_IME_BTN_SWITCH, "ABC");				
							
							char title[2]={0};
							for(int fori=IDC_IME_BTN_A; fori<=IDC_IME_BTN_Z; fori++)
							{				
								title[0]='a'+fori-IDC_IME_BTN_A;
								
								SetDlgItemText(hDlg, fori, title);
							}
						}
						else if(textSwitch[0]>='A' && textSwitch[0]<='Z')
						{
							SetDlgItemText(hDlg, IDC_IME_BTN_SWITCH, "012");

							char title[2]={0};
							for(int fori=IDC_IME_BTN_A; fori<=IDC_IME_BTN_Z; fori++)
							{				
								title[0]='A'+fori-IDC_IME_BTN_A;
								
								SetDlgItemText(hDlg, fori, title);
							}
						}
						else
						{											
							SetDlgItemText(hDlg, IDC_IME_BTN_SWITCH, "abc");
							char btn_str[]=
							{
								'0','1','2','3','4','5','6',
								'7','8','9','(',')','-','=',
								'_','+','[',']','{','}','|',
								':',';','\"','.','?'
							};
							
							char title[2]={0};
							for(int fori=IDC_IME_BTN_A; fori<=IDC_IME_BTN_Z; fori++)
							{	
								if((fori-IDC_IME_BTN_A)<(int)ARRAY_SIZE(btn_str))
								{
									title[0]=btn_str[fori-IDC_IME_BTN_A];									
									SetDlgItemText(hDlg, fori, title);
								}
							}
						}
						
					}
					else if(id==IDC_IME_BTN_SPACE)
					{
						char input=' ';
						
						SendDlgItemMessage(hDlg, IDC_IME_EDIT_SHOW, MSG_CHAR, input, 0);
					}
					else if(id==IDC_IME_BTN_DEL)
					{
						char input=127;
						
						SendDlgItemMessage(hDlg, IDC_IME_EDIT_SHOW, MSG_CHAR, input, 0);
					}
					else if(id==IDC_IME_BTN_EXIT)
					{						
						return quitThisDialog(hDlg, 0);
					}
					else if(id<IDC_IME_BTN_SWITCH)
					{
						char textSwitch[4]={0};
						HWND cwin=GetDlgItem(hDlg, id);
						int len=GetWindowTextLength(cwin);
						if(len>0)
						{							
							GetWindowText(cwin, textSwitch, len);													

							WPARAM wpa;
							wpa=textSwitch[2];
							wpa<<=8;
							wpa|=textSwitch[1];
							wpa<<=8;
							wpa|=textSwitch[0];
							
							SendDlgItemMessage(hDlg, IDC_IME_EDIT_SHOW, MSG_CHAR, wpa, 0);
						}
					}

					
					return 0;
				}
			}while(0);
		break;
	}
	
	return DefaultDialogProc (hDlg, message, wParam, lParam);
}


int showSoftKeyBoardEnBox(HWND hOwner, HWND hTarget)
{
#define MYMB_X 0
#define MYMB_Y 0
#define MYMB_W 35
#define MYMB_H 30
#define MYMB_OFFY 0
#define MYMB_OFFX 0


#define MYMBWEIGHT  (MYMB_W*9)
#define MYMBHEIGHT  (MYMB_H*4)


	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_SLEDIT,
			WS_CHILD | WS_TABSTOP|WS_VISIBLE |WS_BORDER,
			0, 0, MYMB_W, MYMB_H,
			IDC_IME_EDIT_SHOW,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*0, MYMB_Y+(MYMB_OFFY+MYMB_H)*0, MYMB_W, MYMB_H,
			IDC_IME_BTN_A,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*1, MYMB_Y+(MYMB_OFFY+MYMB_H)*0, MYMB_W, MYMB_H,
			IDC_IME_BTN_B,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*2, MYMB_Y+(MYMB_OFFY+MYMB_H)*0, MYMB_W, MYMB_H,
			IDC_IME_BTN_C,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*3, MYMB_Y+(MYMB_OFFY+MYMB_H)*0, MYMB_W, MYMB_H,
			IDC_IME_BTN_D,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*4, MYMB_Y+(MYMB_OFFY+MYMB_H)*0, MYMB_W, MYMB_H,
			IDC_IME_BTN_E,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*5, MYMB_Y+(MYMB_OFFY+MYMB_H)*0, MYMB_W, MYMB_H,
			IDC_IME_BTN_F,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*6, MYMB_Y+(MYMB_OFFY+MYMB_H)*0, MYMB_W, MYMB_H,
			IDC_IME_BTN_G,
			"",
			0,
			WS_EX_NONE
		},
		
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*0, MYMB_Y+(MYMB_OFFY+MYMB_H)*1, MYMB_W, MYMB_H,
			IDC_IME_BTN_H,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*1, MYMB_Y+(MYMB_OFFY+MYMB_H)*1, MYMB_W, MYMB_H,
			IDC_IME_BTN_I,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*2, MYMB_Y+(MYMB_OFFY+MYMB_H)*1, MYMB_W, MYMB_H,
			IDC_IME_BTN_J,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*3, MYMB_Y+(MYMB_OFFY+MYMB_H)*1, MYMB_W, MYMB_H,
			IDC_IME_BTN_K,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*4, MYMB_Y+(MYMB_OFFY+MYMB_H)*1, MYMB_W, MYMB_H,
			IDC_IME_BTN_L,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*5, MYMB_Y+(MYMB_OFFY+MYMB_H)*1, MYMB_W, MYMB_H,
			IDC_IME_BTN_M,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*6, MYMB_Y+(MYMB_OFFY+MYMB_H)*1, MYMB_W, MYMB_H,
			IDC_IME_BTN_N,
			"",
			0,
			WS_EX_NONE
		},

		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*0, MYMB_Y+(MYMB_OFFY+MYMB_H)*2, MYMB_W, MYMB_H,
			IDC_IME_BTN_O,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*1, MYMB_Y+(MYMB_OFFY+MYMB_H)*2, MYMB_W, MYMB_H,
			IDC_IME_BTN_P,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*2, MYMB_Y+(MYMB_OFFY+MYMB_H)*2, MYMB_W, MYMB_H,
			IDC_IME_BTN_Q,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*3, MYMB_Y+(MYMB_OFFY+MYMB_H)*2, MYMB_W, MYMB_H,
			IDC_IME_BTN_R,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*4, MYMB_Y+(MYMB_OFFY+MYMB_H)*2, MYMB_W, MYMB_H,
			IDC_IME_BTN_S,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*5, MYMB_Y+(MYMB_OFFY+MYMB_H)*2, MYMB_W, MYMB_H,
			IDC_IME_BTN_T,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*6, MYMB_Y+(MYMB_OFFY+MYMB_H)*2, MYMB_W, MYMB_H,
			IDC_IME_BTN_U,
			"",
			0,
			WS_EX_NONE
		},

		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*0, MYMB_Y+(MYMB_OFFY+MYMB_H)*3, MYMB_W, MYMB_H,
			IDC_IME_BTN_V,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*1, MYMB_Y+(MYMB_OFFY+MYMB_H)*3, MYMB_W, MYMB_H,
			IDC_IME_BTN_W,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*2, MYMB_Y+(MYMB_OFFY+MYMB_H)*3, MYMB_W, MYMB_H,
			IDC_IME_BTN_X,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*3, MYMB_Y+(MYMB_OFFY+MYMB_H)*3, MYMB_W, MYMB_H,
			IDC_IME_BTN_Y,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*4, MYMB_Y+(MYMB_OFFY+MYMB_H)*3, MYMB_W, MYMB_H,
			IDC_IME_BTN_Z,
			"",
			0,
			WS_EX_NONE
		},

		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*7, MYMB_Y+(MYMB_OFFY+MYMB_H)*0, MYMB_W*2, MYMB_H,
			IDC_IME_BTN_SWITCH,
			"ABC",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*7, MYMB_Y+(MYMB_OFFY+MYMB_H)*1, MYMB_W*2, MYMB_H,
			IDC_IME_BTN_SPACE,
			"Space",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*7, MYMB_Y+(MYMB_OFFY+MYMB_H)*2, MYMB_W*2, MYMB_H,
			IDC_IME_BTN_DEL,
			"Del",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			MYMB_X+(MYMB_OFFX+MYMB_W)*7, MYMB_Y+(MYMB_OFFY+MYMB_H)*3, MYMB_W*2, MYMB_H,
			IDC_IME_BTN_EXIT,
			"Enter",
			0,
			WS_EX_NONE
		},	

	};


	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE,
		WS_EX_NONE,
		0, 0, MYMBWEIGHT, MYMBHEIGHT,
		"",
		0, 0,
		ARRAY_SIZE(CtrlInfo),
		CtrlInfo,
		0
	};


	if(false==TestWindowCreate(hOwner))
	{
		return -1;
	}

	RECT rcTarget;
	GetWindowRect(hTarget, &rcTarget);

	int target_w=rcTarget.right-rcTarget.left;
	int target_h=rcTarget.bottom-rcTarget.top;
	
	ClientToScreen(hOwner, &rcTarget.left, &rcTarget.top);

	if((rcTarget.left+MYMBWEIGHT)>(int)MainFrame::getWidth())
	{
		int tmpleft=rcTarget.left+MYMBWEIGHT-MainFrame::getWidth();
		rcTarget.left-=tmpleft;
		CtrlInfo[IDC_IME_EDIT_SHOW].x+=tmpleft;
	}

	if((rcTarget.top+MYMBHEIGHT)>(int)MainFrame::getHeight())
	{
		int tmptop=rcTarget.top+MYMBHEIGHT-MainFrame::getHeight();
		rcTarget.top-=tmptop;
		CtrlInfo[IDC_IME_EDIT_SHOW].y+=tmptop;
	}
	
	DlgInfo.x=rcTarget.left;
	DlgInfo.y=rcTarget.top;

	memset(&gsk, 0, sizeof(gsk));
	gsk.hTarget=hTarget;
	gsk.x=rcTarget.left;
	gsk.y=rcTarget.top;
	gsk.edith=target_h;
	DlgInfo.dwAddData=(DWORD)&gsk;
	
	
	CtrlInfo[IDC_IME_EDIT_SHOW].w=target_w;
	CtrlInfo[IDC_IME_EDIT_SHOW].h=target_h;

	
	if(DlgInfo.w<target_w)
	{
		DlgInfo.w=target_w;
	}

	DlgInfo.h+=target_h;	
	
	for(int fori=IDC_IME_BTN_A; fori<=IDC_IME_BTN_EXIT; fori++)
	{
		CtrlInfo[fori].y+=target_h;
	}

	
	return DialogBoxIndirectParam(&DlgInfo, hOwner, softKeyBoardBoxEnProc, 0);

#undef MYMBWEIGHT
#undef MYMBHEIGHT
#undef MYMB_X
#undef MYMB_Y
#undef MYMB_H
#undef MYMB_OFFY
#undef MYMB_OFFX

#undef MYMB_W
	
}
















