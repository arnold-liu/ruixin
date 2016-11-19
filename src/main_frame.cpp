
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/ctrl/ctrlhelper.h>
#include <minigui/ctrl/button.h>
#include <minigui/ctrl/static.h>
#include <minigui/ctrl/edit.h>
#include <minigui/ctrl/combobox.h>
#include <minigui/ctrl/listbox.h>
#include <minigui/ctrl/listview.h>
#include <minigui/ctrl/monthcal.h>


#include "gbl_define.h"
#include "main_role.h"
#include "hs_keyboard.h"
#include "main_frame.h"
#include "main_record.h"
#include "main_adc.h"
#include "main_machine.h"
#include "main_key.h"
#include "main_cali.h"

#include "hs_udev.h"



#include "main_osd.h"


#define USER_NAME_MAX 64
#define USER_PWD_MAX 64
#define MACHINE_NAME_MAX 64


enum ROLE_TYPE_E
{
	ROLE_ADMIN=0,
	ROLE_MANAGER=10,
	ROLE_USER=100,
};


enum TIMER_E
{
	MAIN_TIMER_CHECK=0x100,
	MAIN_TIMER_TIME_SHOW,
	MAIN_TIMER_USB_STATE,
	MAIN_TIMER_MAIN_UPDATE,

	MAIN_TIMER_CALI_WIN_SHOW,
};




enum MEASURE_TYPE_E
{
	MEASURE_TYPE_NONE,
	
	//MEASURE_TYPE_2MM,
	//MEASURE_TYPE_4MM,	
	MEASURE_TYPE_LINE,
	MEASURE_TYPE_2MM_CALI,
	//MEASURE_TYPE_4MM_CALI,
	
	MEASURE_TYPE_MAX,
};

#define RECORD_SHOW_CNT 5
static int m_cur_machine_id=0;
static int m_measure_type=MEASURE_TYPE_LINE;
bool m_app_do_cali2=false;

unsigned int m_user_type=-1;





int quitThisDialog(HWND hDlg, const int retVal)
{
	HWND quitDlg=hDlg;

	HWND hTmp=GetNextMainWindow(HWND_NULL);
	if(quitDlg!=hTmp)
	{
		DPRINTF("this win is not the topmost win , can not quit it, so quit the topmost\n");
		if(IsDialog(hTmp))
		{
			quitDlg=hTmp;
		}
		else
		{
			DPRINTF("the top win is not a dialog, can not quit it\n");
			return -1;
		}			
	}
	
	if(IsDialog(quitDlg))
	{				
		HWND hHost=GetHosting(quitDlg);
		
		EndDialog (quitDlg, retVal);

		if(FALSE==IsWindowVisible(hHost))
		{
			ShowWindow(hHost, SW_SHOW);
		}

		return 0;
	}
	else
	{
		DPRINTF("quit win is not a dialog\n");
	}

	return -1;
}


bool TestWindowCreate(HWND hOwner)
{	
	HWND hTmp=GetNextMainWindow(HWND_NULL);
	if(hOwner!=hTmp)
	{
		char str[32]={0};
		GetWindowText(hTmp, str, sizeof(str)-1);
		DPRINTF("this win is not the topmost win , can not create child win  [%s]\n", str);
		return false;
	}

#ifdef DEBUG
	if(0==IsWindowVisible(hOwner))
	{
		DPRINTF("this win is not visible , can create child win??\n");
	}
#endif

	return true;
}









enum
{
	IDC_MAIN_CALI_TEXT_BIAOQIAN=0x2100,
	IDC_MAIN_CALI_TEXT_CALI1_5,
	IDC_MAIN_CALI_TEXT_CALI2_0,
	IDC_MAIN_CALI_TEXT_CALI2_5,
	IDC_MAIN_CALI_TEXT_CALI3_0,
	IDC_MAIN_CALI_TEXT_CALI3_5,
	IDC_MAIN_CALI_TEXT_CALI4_0,
	IDC_MAIN_CALI_TEXT_CALI4_5,
	IDC_MAIN_CALI_TEXT_CALI5_0,
	IDC_MAIN_CALI_TEXT_CALI5_5,
	IDC_MAIN_CALI_TEXT_CALI6_0,
	IDC_MAIN_CALI_TEXT_CALI6_5,

	IDC_MAIN_CALI_TEXT_BIAOZHUN,
	IDC_MAIN_CALI_EDIT_BZ_CALI1_5,
	IDC_MAIN_CALI_EDIT_BZ_CALI2_0,
	IDC_MAIN_CALI_EDIT_BZ_CALI2_5,
	IDC_MAIN_CALI_EDIT_BZ_CALI3_0,
	IDC_MAIN_CALI_EDIT_BZ_CALI3_5,
	IDC_MAIN_CALI_EDIT_BZ_CALI4_0,
	IDC_MAIN_CALI_EDIT_BZ_CALI4_5,
	IDC_MAIN_CALI_EDIT_BZ_CALI5_0,
	IDC_MAIN_CALI_EDIT_BZ_CALI5_5,
	IDC_MAIN_CALI_EDIT_BZ_CALI6_0,
	IDC_MAIN_CALI_EDIT_BZ_CALI6_5,

	IDC_MAIN_CALI_TEXT_BIAODING,
	IDC_MAIN_CALI_EDIT_BD_CALI1_5,
	IDC_MAIN_CALI_EDIT_BD_CALI2_0,
	IDC_MAIN_CALI_EDIT_BD_CALI2_5,
	IDC_MAIN_CALI_EDIT_BD_CALI3_0,
	IDC_MAIN_CALI_EDIT_BD_CALI3_5,
	IDC_MAIN_CALI_EDIT_BD_CALI4_0,
	IDC_MAIN_CALI_EDIT_BD_CALI4_5,
	IDC_MAIN_CALI_EDIT_BD_CALI5_0,
	IDC_MAIN_CALI_EDIT_BD_CALI5_5,
	IDC_MAIN_CALI_EDIT_BD_CALI6_0,
	IDC_MAIN_CALI_EDIT_BD_CALI6_5,	

	IDC_MAIN_CALI_TEXT_AD,
	IDC_MAIN_CALI_EDIT_AD_VAL,
	IDC_MAIN_CALI_TEXT_BQ_VAL,
	IDC_MAIN_CALI_TEXT_VAL,
	IDC_MAIN_CALI_TEXT_BQ_MEA_VAL,
	IDC_MAIN_CALI_TEXT_MEA_VAL,	
	
	IDC_MAIN_CALI_BTN_CALI,



	IDC_MAIN_CALI_BTN_SAVE,
	IDC_MAIN_CALI_BTN_RETURN,
};


#define BIAOZHUN_MID 1000000
#define BIAOZHUN_2MM_ID 1
#define DEFAULT_CALI_MID 1



static void disp_measure(HWND hWnd, int adval)
{	
	MainCali* mc=MainCali::getInstance();

	double ce2=2.0f;
	double ce2mm;
	int ce2ad;

	if(!mc->getCaliAll(DEFAULT_CALI_MID, BIAOZHUN_2MM_ID, ce2, ce2mm, ce2ad))
	{
		ce2=2.0;
		ce2mm=2.0;
		ce2ad=4096*4/30;
	}

	int adoff=4096*4/30-ce2ad;
//	double valoff=2.0-ce2;

	float ad_L=(adval+adoff)*15.0f/4096;

	double SA, SB;
	bool bL=true, bLow=false, bHig=false;
	double LA,LB;	

	for(int fori=0; fori<11; fori++)
	{
		double fval, fmm;
		bL=mc->getCali(BIAOZHUN_MID, fori, fval, fmm);
		if(bL)
		{
			if(ad_L>=fval)
			{
				bLow=true;
				SA=fval;
				LA=fmm;
			}
			else
			{
				bHig=true;
				SB=fval;
				LB=fmm;
			}
		}
	}

	if(bLow && bHig)
	{
		ad_L=(LB-LA)*(ad_L-SA)/(SB-SA)+LA;

		//SetWindowBkColor(hWnd, COLOR_lightwhite);
	}
	else
	{
#if 0	
		if(bLow)
		{
			ad_L+=LA-SA;
			SetWindowBkColor(hWnd, COLOR_yellow);
		}
		else if(bHig)
		{
			double fval, fmm;
			bL=mc->getCali(BIAOZHUN_MID, 0, fval, fmm);
			if(bL)
			{
				ad_L+=LA-SA;
				SetWindowBkColor(hWnd, COLOR_yellow);
			}
			else
			{
				SetWindowBkColor(hWnd, 0xf800);
			}
		}
		else
		{
			SetWindowBkColor(hWnd, 0xf800);
		}
		
#else
		//printf("ad:%d ce2ad:%d	val:%.2f\n", adval, ce2ad, ad_L);
		
		do
		{
			double fval, fmm;
			bool bL=mc->getCali(BIAOZHUN_MID, 0, fval, fmm);
			if(bL)
			{
				if(ad_L<fval)
				{
					ad_L-=fval-fmm;
					break;
				}
			}
		
			bL=mc->getCali(BIAOZHUN_MID, 10, fval, fmm);
			if(bL)
			{
				if(ad_L>fval)
				{
					ad_L-=fval-fmm;
					break;
				}
			}	
		
		}while(0);
 #endif		
	}

#if 0
int adi=ad_L*100;
int ig=adi%10;
if(ig>=8)
{
	adi+=10-ig;
}
else if(ig>=3)
{
	adi+=5-ig;
}
else
{
	adi-=ig;
}

ad_L=adi*1.0/100;
#endif

	char str[16];
	snprintf(str, sizeof(str), "%1.2f", ad_L);
	SetWindowText(hWnd, str);

}


void show_ad(HWND hWnd, int adval);

static void disp_measure_nocali(HWND hWnd, int adval)
{
	MainCali* mc=MainCali::getInstance();
	double ce2=2.0f;
	double ce2mm;
	int ce2ad;

	if(!mc->getCaliAll(DEFAULT_CALI_MID, BIAOZHUN_2MM_ID, ce2, ce2mm, ce2ad))
	{
		ce2=2.0;
		ce2mm=2.0;
		ce2ad=4096*4/30;
	}


	float ad_L=(adval-ce2ad)*15.0f/4096+2.0f;

#if 0
printf("ad:%d ce2ad:%d  val:%.2f\n", adval, ce2ad, ad_L);

do
{
	double fval, fmm;
	bool bL=mc->getCali(BIAOZHUN_MID, 0, fval, fmm);
	if(bL)
	{
		if(ad_L<fval)
		{
			ad_L-=fval-fmm;
			break;
		}
	}

	bL=mc->getCali(BIAOZHUN_MID, 10, fval, fmm);
	if(bL)
	{
		if(ad_L>fval)
		{
			ad_L+=fval-fmm;
			break;
		}
	}	

}while(0);
#endif

#if 1
	int adi=ad_L*100;
	int ig=adi%10;
	if(ig>=8)
	{
		adi+=10-ig;
	}
	else if(ig>=3)
	{
		adi+=5-ig;
	}
	else
	{
		adi-=ig;
	}
	
	ad_L=adi*1.0/100;
#endif


	char str[16];
	snprintf(str, sizeof(str), "%1.2f", ad_L);
	SetWindowText(hWnd, str);
}



int caliBoxProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_INITDIALOG:	
			do
			{
				if(m_user_type>ROLE_MANAGER)
				{
					EnableWindow(GetDlgItem(hDlg, IDC_MAIN_CALI_BTN_SAVE), FALSE);
				}

				//SetWindowFont(GetDlgItem(hDlg, IDC_MAIN_CALI_TEXT_VAL),(PLOGFONT)GetSystemFont(4));
				//SetWindowFont(GetDlgItem(hDlg, IDC_MAIN_CALI_TEXT_MEA_VAL),(PLOGFONT)GetSystemFont(4));

				MainCali* mc=MainCali::getInstance();

				for(int fori=0; fori<11; fori++)
				{
					double dval, dmm;
					if(mc->getCali(BIAOZHUN_MID, fori, dval, dmm))
					{
						char str_val[16];
						snprintf(str_val, sizeof(str_val), "%.2f", (float)dval);
						SetDlgItemText(hDlg, IDC_MAIN_CALI_EDIT_BZ_CALI1_5+fori, str_val);

						snprintf(str_val, sizeof(str_val), "%.2f", (float)dmm);
						SetDlgItemText(hDlg, IDC_MAIN_CALI_EDIT_BD_CALI1_5+fori, str_val);
					}					
				}

				SetTimer(hDlg, MAIN_TIMER_CALI_WIN_SHOW, 50);
			}while(0);
		break;
		case MSG_TIMER:
			do
			{
				int timer_id = (int)wParam;
				//DWORD tick_count = (DWORD)lParam;

				if(timer_id==MAIN_TIMER_CALI_WIN_SHOW)
				{				
					int adval;
					if(!MainAdc::getInstance()->getAdVal(adval))
					{
						return 0;
					}

					show_ad(GetDlgItem(hDlg, IDC_MAIN_CALI_EDIT_AD_VAL), adval);
					disp_measure_nocali(GetDlgItem(hDlg, IDC_MAIN_CALI_TEXT_MEA_VAL), adval);
					disp_measure(GetDlgItem(hDlg, IDC_MAIN_CALI_TEXT_VAL), adval);
				}
			}while(0);
		break;
		case MSG_DESTROY:
			KillTimer(hDlg, MAIN_TIMER_CALI_WIN_SHOW);
		break;
		case MSG_COMMAND:
			do
			{				
				const int id = LOWORD(wParam);
				const int code = HIWORD(wParam);

				if(code==BN_CLICKED)
				{
					switch(id)
					{
						case IDC_MAIN_CALI_BTN_CALI:
							do
							{
								int adval;
								if(!MainAdc::getInstance()->getAdVal(adval))
								{
									MessageBox(hDlg, osd_mainwin_califail, osd_tishi, MB_OK);
									return 0;
								}

								float ad_L=adval*15.0f/4096;

								bool bsave;
								int id=BIAOZHUN_2MM_ID;
								MainCali* mc=MainCali::getInstance();
								if(mc->checkExist(DEFAULT_CALI_MID, id))
								{
									bsave=mc->update(DEFAULT_CALI_MID, id, ad_L, 2.0, adval);
								}
								else
								{
									bsave=mc->add(DEFAULT_CALI_MID, adval, ad_L, 2.0, id);
								}

								
								if(bsave)
								{							
									MessageBox(hDlg, osd_mainwin_caliok, osd_tishi, MB_OK);				
								}
								else
								{
									MessageBox(hDlg, osd_mainwin_califail, osd_tishi, MB_OK);
								}
								
							}while(0);
						break;
						case IDC_MAIN_CALI_BTN_SAVE:
							do
							{
								MainCali* mc=MainCali::getInstance();

								bool bret=true;

								for(int fori=0; fori<11; fori++)
								{
									char val_str[16];
									char mm_str[16];

									float fval,fmm;
									int ret=GetDlgItemText(hDlg, IDC_MAIN_CALI_EDIT_BZ_CALI1_5+fori, val_str, sizeof(val_str)-1);
									if(ret<=0)
									{
										bret=false;
										break;
									}
									
									ret=sscanf(val_str, "%f", &fval);
									if(ret!=1)
									{
										bret=false;
										break;
									}

									ret=GetDlgItemText(hDlg, IDC_MAIN_CALI_EDIT_BD_CALI1_5+fori, mm_str, sizeof(mm_str)-1);
									if(ret<=0)
									{
										bret=false;
										break;
									}
									
									ret=sscanf(mm_str, "%f", &fmm);
									if(ret!=1)
									{
										bret=false;
										break;
									}
									

									if(mc->checkExist(BIAOZHUN_MID, fori))
									{
										if(!mc->update(BIAOZHUN_MID, fori, (double)fval, (double)fmm))
										{
											bret=false;
											break;
										}
									}
									else
									{
										if(!mc->add(BIAOZHUN_MID, 0, (double)fval, (double)fmm, fori))
										{
											bret=false;
											break;
										}
									}
								}

								if(bret)
								{
									MessageBox(hDlg, osd_success, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
								}
							}while(0);
						break;
						case IDC_MAIN_CALI_BTN_RETURN:
							do
							{
								EndDialog(hDlg, id);
							}while(0);
						break;
						
					}
				}
				else if(code==EN_CLICKED)
				{
					switch(id)
					{
						case IDC_MAIN_CALI_EDIT_BZ_CALI1_5:
						case IDC_MAIN_CALI_EDIT_BZ_CALI2_0:
						case IDC_MAIN_CALI_EDIT_BZ_CALI2_5:
						case IDC_MAIN_CALI_EDIT_BZ_CALI3_0:
						case IDC_MAIN_CALI_EDIT_BZ_CALI3_5:
						case IDC_MAIN_CALI_EDIT_BZ_CALI4_0:
						case IDC_MAIN_CALI_EDIT_BZ_CALI4_5:
						case IDC_MAIN_CALI_EDIT_BZ_CALI5_0:
						case IDC_MAIN_CALI_EDIT_BZ_CALI5_5:
						case IDC_MAIN_CALI_EDIT_BZ_CALI6_0:
						case IDC_MAIN_CALI_EDIT_BZ_CALI6_5:

						case IDC_MAIN_CALI_EDIT_BD_CALI1_5:
						case IDC_MAIN_CALI_EDIT_BD_CALI2_0:
						case IDC_MAIN_CALI_EDIT_BD_CALI2_5:
						case IDC_MAIN_CALI_EDIT_BD_CALI3_0:
						case IDC_MAIN_CALI_EDIT_BD_CALI3_5:
						case IDC_MAIN_CALI_EDIT_BD_CALI4_0:
						case IDC_MAIN_CALI_EDIT_BD_CALI4_5:
						case IDC_MAIN_CALI_EDIT_BD_CALI5_0:
						case IDC_MAIN_CALI_EDIT_BD_CALI5_5:
						case IDC_MAIN_CALI_EDIT_BD_CALI6_0:
						case IDC_MAIN_CALI_EDIT_BD_CALI6_5:
							showSoftKeyBoardEnBox(hDlg, (HWND)lParam);
							SetFocus(GetDlgItem(hDlg, IDC_MAIN_CALI_TEXT_CALI1_5));
						break;
					}
				}
			}while(0);
		break;
	}

	return DefaultDialogProc (hDlg, message, wParam, lParam);
}








int showCaliBox(HWND hOwner, int w, int h)
{
#define Y_OFF -10
#define CALI_W 40
#define CALI_H 20

	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_LEFT,
			10, 10+Y_OFF, 180, CALI_H,
			IDC_MAIN_CALI_TEXT_BIAOQIAN,
			osd_cali_biaoqian,
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			10, 30+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_TEXT_CALI1_5,
			"1.5",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			10, 30+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_TEXT_CALI2_0,
			"2.0",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			10, 30+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_TEXT_CALI2_5,
			"2.5",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			10, 30+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_TEXT_CALI3_0,
			"3.0",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			10, 30+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_TEXT_CALI3_5,
			"3.5",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			10, 30+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_TEXT_CALI4_0,
			"4.0",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			10, 30+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_TEXT_CALI4_5,
			"4.5",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			10, 30+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_TEXT_CALI5_0,
			"5.0",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			10, 30+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_TEXT_CALI5_5,
			"5.5",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			10, 30+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_TEXT_CALI6_0,
			"6.0",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			10, 30+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_TEXT_CALI6_5,
			"6.5",
			0,
			WS_EX_NONE
		},

		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_LEFT,
			10, 60+Y_OFF, 180, CALI_H,
			IDC_MAIN_CALI_TEXT_BIAOZHUN,
			osd_cali_biaozhun,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 80+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BZ_CALI1_5,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 80+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BZ_CALI2_0,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 80+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BZ_CALI2_5,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 80+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BZ_CALI3_0,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 80+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BZ_CALI3_5,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 80+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BZ_CALI4_0,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 80+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BZ_CALI4_5,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 80+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BZ_CALI5_0,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 80+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BZ_CALI5_5,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 80+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BZ_CALI6_0,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 80+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BZ_CALI6_5,
			"",
			0,
			WS_EX_NONE
		},
		

		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_LEFT,
			10, 110+Y_OFF, 180, CALI_H,
			IDC_MAIN_CALI_TEXT_BIAODING,
			osd_cali_biaoding,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 130+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BD_CALI1_5,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 130+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BD_CALI2_0,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 130+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BD_CALI2_5,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 130+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BD_CALI3_0,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 130+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BD_CALI3_5,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 130+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BD_CALI4_0,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 130+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BD_CALI4_5,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 130+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BD_CALI5_0,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 130+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BD_CALI5_5,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 130+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BD_CALI6_0,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_BORDER,
			10, 130+Y_OFF, CALI_W, CALI_H,
			IDC_MAIN_CALI_EDIT_BD_CALI6_5,
			"",
			0,
			WS_EX_NONE
		},
		
#if 0
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			10, 160+Y_OFF, 160, 100,
			IDC_MAIN_CALI_TEXT_VAL,
			"0",
			0,
			WS_EX_NONE
		},
#else
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_LEFT,
			10, 160+Y_OFF, 80, CALI_H,
			IDC_MAIN_CALI_TEXT_AD,
			osd_cali_ad,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			90, 160+Y_OFF, 80, CALI_H,
			IDC_MAIN_CALI_EDIT_AD_VAL,
			"0",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_LEFT,
			10, 185+Y_OFF, 80, CALI_H,
			IDC_MAIN_CALI_TEXT_BQ_VAL,
			osd_cali_jisuan,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			90, 185+Y_OFF, 80, CALI_H,
			IDC_MAIN_CALI_TEXT_VAL,
			"0",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_LEFT,
			10, 210+Y_OFF, 80, CALI_H,
			IDC_MAIN_CALI_TEXT_BQ_MEA_VAL,
			osd_cali_xiuzheng,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			90, 210+Y_OFF, 80, CALI_H,
			IDC_MAIN_CALI_TEXT_MEA_VAL,
			"0",
			0,
			WS_EX_NONE
		},
#endif
		
#if 1
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE ,
			260, 180+Y_OFF, 60, 40,
			IDC_MAIN_CALI_BTN_CALI,
			osd_mainwin_moshiqiehuan,
			0,
			WS_EX_NONE
		},
#endif		

		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			320, 180+Y_OFF, 60, 40,
			IDC_MAIN_CALI_BTN_SAVE,
			osd_setup_save,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			380, 180+Y_OFF, 60, 40,
			IDC_MAIN_CALI_BTN_RETURN,
			osd_return,
			0,
			WS_EX_NONE
		},
	};


for(int fori=IDC_MAIN_CALI_TEXT_CALI1_5; fori<=IDC_MAIN_CALI_TEXT_CALI6_5; fori++)
{
	CtrlInfo[fori-IDC_MAIN_CALI_TEXT_BIAOQIAN].x=10+(fori-IDC_MAIN_CALI_TEXT_CALI1_5)*CALI_W;
}

for(int fori=IDC_MAIN_CALI_EDIT_BZ_CALI1_5; fori<=IDC_MAIN_CALI_EDIT_BZ_CALI6_5; fori++)
{
	CtrlInfo[fori-IDC_MAIN_CALI_TEXT_BIAOQIAN].x=10+(fori-IDC_MAIN_CALI_EDIT_BZ_CALI1_5)*CALI_W;
}


for(int fori=IDC_MAIN_CALI_EDIT_BD_CALI1_5; fori<=IDC_MAIN_CALI_EDIT_BD_CALI6_5; fori++)
{
	CtrlInfo[fori-IDC_MAIN_CALI_TEXT_BIAOQIAN].x=10+(fori-IDC_MAIN_CALI_EDIT_BD_CALI1_5)*CALI_W;
}



#undef CALI_W
#undef CALI_H

#undef Y_OFF
		
	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE ,
		WS_EX_NONE,
		0, 0, w, h,
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
	
	return DialogBoxIndirectParam(&DlgInfo, hOwner, caliBoxProc, 0);
}



enum
{
	IDC_MAIN_USRMGR_LIST_USR=0x2000,
	IDC_MAIN_USRMGR_TEXT_USR,
	IDC_MAIN_USRMGR_EDIT_USR,
	IDC_MAIN_USRMGR_TEXT_PWD,
	IDC_MAIN_USRMGR_EDIT_PWD,
	IDC_MAIN_USRMGR_BTN_MGR,
	IDC_MAIN_USRMGR_BTN_ADD,
	IDC_MAIN_USRMGR_BTN_DEL,
	IDC_MAIN_USRMGR_BTN_UPDATE,

	IDC_MAIN_USRMGR_BTN_RETURN,
};


void init_usr_list_view(HWND hListView)
{
	int i;
	LVCOLUMN lvcol;	
	const char* caption[]=
	{
		osd_usrmgr_id,		
		osd_usrmgr_name,
	};
	const int itemw[]=
	{
		50, 180,
	};
	
	for (i = 0; i < (int)ARRAY_SIZE(caption); i++) {
		lvcol.nCols = i;
		lvcol.pszHeadText = (char*)caption[i];
		lvcol.width = itemw[i];
		lvcol.pfnCompare = NULL;
		lvcol.colFlags = 0;
		SendMessage (hListView, LVM_ADDCOLUMN, 0, (LPARAM) &lvcol);
	}
}


void insert_one_usr(HWND hlist, PLVITEM lvItem, GHANDLE classent, int mid, string& mname)
{
	char buff[32];
	LVSUBITEM subdata;
	GHANDLE item = SendMessage (hlist, LVM_ADDITEM, classent, (LPARAM)lvItem);
	int j;

	subdata.flags = 0;
	subdata.image = 0;
	subdata.nItem = lvItem->nItem;

	for (j = 0; j < 2; j ++) {
		subdata.subItem = j;
		subdata.nTextColor = 0;
		if(j==0)
		{
			snprintf(buff, sizeof(buff), "%d", lvItem->nItem);
			subdata.pszText = buff;
		}
		else if (j == 1)
		{			
			subdata.pszText = (char*)mname.c_str();			
		}		
		
		SendMessage (hlist, LVM_SETSUBITEM, item, (LPARAM) & subdata);
	}

}


void flush_usr_list_view(HWND hWnd)
{
	int index=0;
	LVITEM item;
	string mname;

	HWND hListView=GetDlgItem(hWnd, IDC_MAIN_USRMGR_LIST_USR);

	SendMessage(hListView, LVM_DELALLITEM, 0, 0);
	
	RoleManager* mmgr=RoleManager::getInstance();

	int machine_id=1;
	
	item.nItemHeight = 25;	
	SendMessage (hListView, MSG_FREEZECTRL, TRUE, 0);
	while(1)
	{
		if(!mmgr->getNextUserId(machine_id, &machine_id))
		{
			break;
		}

		if(machine_id)
		{
			item.nItem = index++;
			item.itemData=machine_id;
			
			mmgr->getUserName(machine_id, mname);
			
			insert_one_usr (hListView, &item, 0, machine_id, mname);
		}
	}
	SendMessage (hListView, MSG_FREEZECTRL, FALSE, 0);
}


int usrMgrBoxProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_INITDIALOG:
			if(m_user_type>ROLE_MANAGER)			
			{
				EnableWindow(GetDlgItem(hDlg, IDC_MAIN_USRMGR_BTN_ADD), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_MAIN_USRMGR_BTN_DEL), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_MAIN_USRMGR_BTN_UPDATE), FALSE);
			}
			else
			{
				init_usr_list_view(GetDlgItem(hDlg,IDC_MAIN_USRMGR_LIST_USR));
				flush_usr_list_view(hDlg);
			}

			
			SendDlgItemMessage(hDlg, IDC_MAIN_USRMGR_EDIT_USR, EM_LIMITTEXT, USER_NAME_MAX, 0);
		break;
		case MSG_COMMAND:
			do
			{				
				const int id = LOWORD(wParam);
				const int code = HIWORD(wParam);

				RoleManager* mmgr=RoleManager::getInstance();
				
				if(code==BN_CLICKED)
				{
					switch(id)
					{
						case IDC_MAIN_USRMGR_BTN_ADD:
							do
							{
								int len, ret;
								len=GetWindowTextLength(GetDlgItem(hDlg, IDC_MAIN_USRMGR_EDIT_USR));
								if(len<=0)
								{
									MessageBox(hDlg, osd_input_err, osd_tishi, MB_OK);
									return 0;
								}
								char name[len+1];
								ret=GetWindowText(GetDlgItem(hDlg, IDC_MAIN_USRMGR_EDIT_USR), name, len);
								name[ret]=0;
								
								len=GetWindowTextLength(GetDlgItem(hDlg, IDC_MAIN_USRMGR_EDIT_PWD));
								if(len<=0)
								{
									MessageBox(hDlg, osd_input_err, osd_tishi, MB_OK);
									return 0;
								}
								char pwd[len+1];
								ret=GetWindowText(GetDlgItem(hDlg, IDC_MAIN_USRMGR_EDIT_PWD), pwd, len);
								pwd[ret]=0;

								int role_type=ROLE_USER;
								ret=SendDlgItemMessage(hDlg, IDC_MAIN_USRMGR_BTN_MGR, BM_GETCHECK, 0, 0);
								if(ret==BST_CHECKED)
								{
									role_type=ROLE_MANAGER;
								}

								if(mmgr->checkexist(name))
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
									return 0;
								}
								
								if(mmgr->add(name, pwd, role_type))
								{
									flush_usr_list_view(hDlg);
									MessageBox(hDlg, osd_success, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
								}
								

								return 0;
							}while(0);
						break;
						case IDC_MAIN_USRMGR_BTN_DEL:
							do
							{
								int pi=SendDlgItemMessage(hDlg, IDC_MAIN_USRMGR_LIST_USR, LVM_GETSELECTEDITEM, 0, 0);
								if(pi==-1 || pi==0)
								{
									MessageBox(hDlg, osd_no_sel, osd_tishi, MB_OK);
									return 0;
								}
								int mid=SendDlgItemMessage(hDlg, IDC_MAIN_USRMGR_LIST_USR, LVM_GETITEMADDDATA, 0, (LPARAM)pi);
								
								if(mmgr->del(mid))
								{
									flush_usr_list_view(hDlg);
									MessageBox(hDlg, osd_success, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
								}								

								return 0;
							}while(0);
						break;
						case IDC_MAIN_USRMGR_BTN_UPDATE:
							do
							{
								int pi=SendDlgItemMessage(hDlg, IDC_MAIN_USRMGR_LIST_USR, LVM_GETSELECTEDITEM, 0, 0);
								if(pi==-1 || pi==0)
								{
									MessageBox(hDlg, osd_no_sel, osd_tishi, MB_OK);
									return 0;
								}
								
								int mid=SendDlgItemMessage(hDlg, IDC_MAIN_USRMGR_LIST_USR, LVM_GETITEMADDDATA, 0, (LPARAM)pi);
								
								int len, ret;
								len=GetWindowTextLength(GetDlgItem(hDlg, IDC_MAIN_USRMGR_EDIT_USR));
								if(len<=0)
								{
									MessageBox(hDlg, osd_input_err, osd_tishi, MB_OK);
									return 0;
								}
								char name[len+1];
								ret=GetWindowText(GetDlgItem(hDlg, IDC_MAIN_USRMGR_EDIT_USR), name, len);
								name[ret]=0;
								char pwd[len+1];
								ret=GetWindowText(GetDlgItem(hDlg, IDC_MAIN_USRMGR_EDIT_PWD), pwd, len);
								pwd[ret]=0;

								int role_type=ROLE_USER;
								ret=SendDlgItemMessage(hDlg, IDC_MAIN_USRMGR_BTN_MGR, BM_GETCHECK, 0, 0);
								if(ret==BST_CHECKED)
								{
									role_type=ROLE_MANAGER;
								}

								if(mmgr->checkexist(name))
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
									return 0;
								}

								if(mmgr->update(mid, name, pwd, role_type))
								{
									flush_usr_list_view(hDlg);
									MessageBox(hDlg, osd_success, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
								}

								return 0;
							}while(0);
						break;
						case IDC_MAIN_USRMGR_BTN_RETURN:							
							do
							{
								EndDialog(hDlg, id);
							}while(0);
						break;
					}
				}
				else if(EN_CLICKED==code)
				{
					if(IDC_MAIN_USRMGR_EDIT_USR==id)
					{
						showSoftKeyBoardEnBox(hDlg, (HWND)lParam);
						SetFocus(GetDlgItem(hDlg, IDC_MAIN_USRMGR_TEXT_USR));
					}
					else if(IDC_MAIN_USRMGR_EDIT_PWD==id)
					{
						showSoftKeyBoardEnBox(hDlg, (HWND)lParam);
						SetFocus(GetDlgItem(hDlg, IDC_MAIN_USRMGR_TEXT_USR));
					}
					
				}
			}while(0);
		break;
	}

	return DefaultDialogProc (hDlg, message, wParam, lParam);
}




int showUsrMgrBox(HWND hOwner, int w, int h)
{
#define X_OFF 100
	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_LISTVIEW ,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
			10, 10, 250, 250,
			IDC_MAIN_USRMGR_LIST_USR,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_LEFT,
			170+X_OFF, 10, 60, 30,
			IDC_MAIN_USRMGR_TEXT_USR,
			osd_reg_user,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT|WS_BORDER,
			230+X_OFF, 10, 140, 30,
			IDC_MAIN_USRMGR_EDIT_USR,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_LEFT,
			170+X_OFF, 50, 60, 30,
			IDC_MAIN_USRMGR_TEXT_PWD,
			osd_reg_pwd,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT|WS_BORDER,
			230+X_OFF, 50, 140, 30,
			IDC_MAIN_USRMGR_EDIT_PWD,
			"",
			0,
			WS_EX_NONE
		},

		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP|BS_AUTOCHECKBOX,
			170+X_OFF, 90, 100, 30,
			IDC_MAIN_USRMGR_BTN_MGR,
			osd_reg_mgr,
			0,
			WS_EX_NONE
		},
		
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			170+X_OFF, 130, 80, 50,
			IDC_MAIN_USRMGR_BTN_ADD,
			osd_add,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			260+X_OFF, 130, 80, 50,
			IDC_MAIN_USRMGR_BTN_DEL,
			osd_del,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			170+X_OFF, 190, 80, 50,
			IDC_MAIN_USRMGR_BTN_UPDATE,
			osd_update,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			260+X_OFF, 190, 80, 50,
			IDC_MAIN_USRMGR_BTN_RETURN,
			osd_return,
			0,
			WS_EX_NONE
		},
		
	};

#undef X_OFF

	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE,
		WS_EX_NONE,
		0, 0, w, h,
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
	
	return DialogBoxIndirectParam(&DlgInfo, hOwner, usrMgrBoxProc, 0);
}






enum
{
	IDC_MAIN_MACMGR_LIST_MAC=0x1900,
	IDC_MAIN_MACMGR_TEXT_MAC,
	IDC_MAIN_MACMGR_EDIT_MAC,
	IDC_MAIN_MACMGR_BTN_ADD,
	IDC_MAIN_MACMGR_BTN_DEL,
	IDC_MAIN_MACMGR_BTN_UPDATE,

	IDC_MAIN_MACMGR_BTN_RETURN,
};


void init_machine_list_view(HWND hListView)
{
	int i;
	LVCOLUMN lvcol;	
	const char* caption[]=
	{
		osd_macmgr_id,		
		osd_macmgr_name,
	};
	const int itemw[]=
	{
		50, 180,
	};
	
	for (i = 0; i < (int)ARRAY_SIZE(caption); i++) {
		lvcol.nCols = i;
		lvcol.pszHeadText = (char*)caption[i];
		lvcol.width = itemw[i];
		lvcol.pfnCompare = NULL;
		lvcol.colFlags = 0;
		SendMessage (hListView, LVM_ADDCOLUMN, 0, (LPARAM) &lvcol);
	}
}


void insert_one_machine(HWND hlist, PLVITEM lvItem, GHANDLE classent, int mid, string& mname)
{
	char buff[32];
	LVSUBITEM subdata;
	GHANDLE item = SendMessage (hlist, LVM_ADDITEM, classent, (LPARAM)lvItem);
	int j;

	subdata.flags = 0;
	subdata.image = 0;
	subdata.nItem = lvItem->nItem;

	for (j = 0; j < 2; j ++) {
		subdata.subItem = j;
		subdata.nTextColor = 0;
		if(j==0)
		{
			snprintf(buff, sizeof(buff), "%d", lvItem->nItem);
			subdata.pszText = buff;
		}
		else if (j == 1)
		{			
			subdata.pszText = (char*)mname.c_str();			
		}		
		
		SendMessage (hlist, LVM_SETSUBITEM, item, (LPARAM) & subdata);
	}

}


void flush_machine_list_view(HWND hWnd)
{
	int index=0;
	LVITEM item;
	string mname;

	HWND hListView=GetDlgItem(hWnd, IDC_MAIN_MACMGR_LIST_MAC);

	SendMessage(hListView, LVM_DELALLITEM, 0, 0);
	
	MachineManager* mmgr=MachineManager::getInstance();

	int machine_id=0;
	
	item.nItemHeight = 25;	
	SendMessage (hListView, MSG_FREEZECTRL, TRUE, 0);
	while(1)
	{
		if(!mmgr->getNextMachineId(machine_id, &machine_id))
		{
			break;
		}

		if(machine_id)
		{
			item.nItem = index++;
			item.itemData=machine_id;
			
			mmgr->getMachineName(machine_id, mname);
			
			insert_one_machine (hListView, &item, 0, machine_id, mname);
		}
	}
	SendMessage (hListView, MSG_FREEZECTRL, FALSE, 0);
}


int macMgrBoxProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_INITDIALOG:
			if(m_user_type>ROLE_MANAGER)			
			{
				EnableWindow(GetDlgItem(hDlg, IDC_MAIN_MACMGR_BTN_ADD), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_MAIN_MACMGR_BTN_DEL), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_MAIN_MACMGR_BTN_UPDATE), FALSE);
			}
			else
			{
				init_machine_list_view(GetDlgItem(hDlg,IDC_MAIN_MACMGR_LIST_MAC));
				flush_machine_list_view(hDlg);
			}

			
			SendDlgItemMessage(hDlg, IDC_MAIN_MACMGR_EDIT_MAC, EM_LIMITTEXT, MACHINE_NAME_MAX, 0);
		break;
		case MSG_COMMAND:
			do
			{				
				const int id = LOWORD(wParam);
				const int code = HIWORD(wParam);

				MachineManager* mmgr=MachineManager::getInstance();
				
				if(code==BN_CLICKED)
				{
					switch(id)
					{
						case IDC_MAIN_MACMGR_BTN_ADD:
							do
							{
								int len;
								len=GetWindowTextLength(GetDlgItem(hDlg, IDC_MAIN_MACMGR_EDIT_MAC));
								if(len<=0)
								{
									MessageBox(hDlg, osd_input_err, osd_tishi, MB_OK);
									return 0;
								}
								char name[len+1];
								GetWindowText(GetDlgItem(hDlg, IDC_MAIN_MACMGR_EDIT_MAC), name, len);

								if(mmgr->checkexist(name))
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
									return 0;
								}
								
								if(mmgr->add(name))
								{
									MessageBox(hDlg, osd_success, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
								}

								flush_machine_list_view(hDlg);

								return 0;
							}while(0);
						break;
						case IDC_MAIN_MACMGR_BTN_DEL:
							do
							{
								int pi=SendDlgItemMessage(hDlg, IDC_MAIN_MACMGR_LIST_MAC, LVM_GETSELECTEDITEM, 0, 0);
printf("sel:%d  \n", pi);								
								if(pi==-1 || pi==0)
								{
									MessageBox(hDlg, osd_no_sel, osd_tishi, MB_OK);
									return 0;
								}
								int mid=SendDlgItemMessage(hDlg, IDC_MAIN_MACMGR_LIST_MAC, LVM_GETITEMADDDATA, 0, (LPARAM)pi);
								
								if(mmgr->del(mid))
								{
									MessageBox(hDlg, osd_success, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
								}

								flush_machine_list_view(hDlg);

								return 0;
							}while(0);
						break;
						case IDC_MAIN_MACMGR_BTN_UPDATE:
							do
							{
								int pi=SendDlgItemMessage(hDlg, IDC_MAIN_MACMGR_LIST_MAC, LVM_GETSELECTEDITEM, 0, 0);
								if(pi==-1 || pi==0)
								{
									MessageBox(hDlg, osd_no_sel, osd_tishi, MB_OK);
									return 0;
								}
								
								int mid=SendDlgItemMessage(hDlg, IDC_MAIN_MACMGR_LIST_MAC, LVM_GETITEMADDDATA, 0, (LPARAM)pi);
								
								int len;
								len=GetWindowTextLength(GetDlgItem(hDlg, IDC_MAIN_MACMGR_EDIT_MAC));
								if(len<=0)
								{
									MessageBox(hDlg, osd_input_err, osd_tishi, MB_OK);
									return 0;
								}
								char name[len+1];
								GetWindowText(GetDlgItem(hDlg, IDC_MAIN_MACMGR_EDIT_MAC), name, len);

								if(mmgr->checkexist(name))
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
									return 0;
								}
								if(mmgr->update(mid, name))
								{
									MessageBox(hDlg, osd_success, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
								}

								flush_machine_list_view(hDlg);

								return 0;
							}while(0);
						break;
						case IDC_MAIN_MACMGR_BTN_RETURN:							
							do
							{
								EndDialog(hDlg, id);
							}while(0);
						break;
					}
				}
				else if(EN_CLICKED==code)				
				{
					if(IDC_MAIN_MACMGR_EDIT_MAC==id)
					{
						showSoftKeyBoardEnBox(hDlg, (HWND)lParam);
						SetFocus(GetDlgItem(hDlg, IDC_MAIN_MACMGR_TEXT_MAC));
					}
				}
			}while(0);
		break;
	}

	return DefaultDialogProc (hDlg, message, wParam, lParam);
}




int showMacMgrBox(HWND hOwner, int w, int h)
{
#define X_OFF 100
	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_LISTVIEW,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
			10, 10, 250, 250,
			IDC_MAIN_MACMGR_LIST_MAC,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_LEFT ,
			170+X_OFF, 10, 200, 30,
			IDC_MAIN_MACMGR_TEXT_MAC,
			osd_macmgr_macname,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT|WS_BORDER,
			170+X_OFF, 50, 180, 30,
			IDC_MAIN_MACMGR_EDIT_MAC,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			170+X_OFF, 90, 80, 50,
			IDC_MAIN_MACMGR_BTN_ADD,
			osd_add,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			270+X_OFF, 90, 80, 50,
			IDC_MAIN_MACMGR_BTN_DEL,
			osd_del,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			170+X_OFF, 150, 80, 50,
			IDC_MAIN_MACMGR_BTN_UPDATE,
			osd_update,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			270+X_OFF, 150, 80, 50,
			IDC_MAIN_MACMGR_BTN_RETURN,
			osd_return,
			0,
			WS_EX_NONE
		},
		
	};

#undef X_OFF

	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE,
		WS_EX_NONE,
		0, 0, w, h,
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
	
	return DialogBoxIndirectParam(&DlgInfo, hOwner, macMgrBoxProc, 0);
}




enum
{
	IDC_MAIN_DATE_SEL=0x1800,
	IDC_MAIN_DATE_BTN_OK,
	IDC_MAIN_DATE_BTN_RETURN,
};



int dateBoxProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_INITDIALOG:	
			//SetWindowFont(GetDlgItem(hDlg, IDC_MAIN_DATE_SEL),(PLOGFONT)GetSystemFont(4));
		break;		
		case MSG_COMMAND:
			do
			{				
				const int id = LOWORD(wParam);
				const int code = HIWORD(wParam);

				if(code==BN_CLICKED)
				{
					switch(id)
					{
						case IDC_MAIN_DATE_BTN_OK:
							do
							{
								SYSTEMTIME stm;
								DWORD addr=GetWindowAdditionalData(hDlg);
								SendDlgItemMessage(hDlg, IDC_MAIN_DATE_SEL, MCM_GETCURDATE, 0, (LPARAM)&stm);
								char date_str[32];
								snprintf(date_str, sizeof(date_str), "%4d-%02d-%02d", stm.year, stm.month,stm.day);
								SetWindowText((HWND)addr, date_str);
							}while(0);
						case IDC_MAIN_DATE_BTN_RETURN:
							do
							{
								EndDialog(hDlg, id);
							}while(0);
						break;
						
					}
				}
			}while(0);
		break;
	}

	return DefaultDialogProc (hDlg, message, wParam, lParam);
}







int showDateBox(HWND hOwner, HWND target, int w, int h)
{
#define Y_OFF 0
	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_MONTHCALENDAR,
			WS_CHILD | WS_VISIBLE |WS_BORDER|MCS_ENG_S,
			50, 10+Y_OFF, 340, 200,
			IDC_MAIN_DATE_SEL,
			"",
			0,
			WS_EX_NONE
		},

		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			170, 230+Y_OFF, 60, 30,
			IDC_MAIN_DATE_BTN_OK,
			osd_setup_save,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			250, 230+Y_OFF, 60, 30,
			IDC_MAIN_DATE_BTN_RETURN,
			osd_return,
			0,
			WS_EX_NONE
		},
	};

#undef Y_OFF
		
	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE ,
		WS_EX_NONE,
		0, 0, w, h,
		"",
		0, 0,
		ARRAY_SIZE(CtrlInfo),
		CtrlInfo,
		target
	};

	if(false==TestWindowCreate(hOwner))
	{
		return -1;
	}

	return DialogBoxIndirectParam(&DlgInfo, hOwner, dateBoxProc, 0);
}











enum
{
	IDC_MAIN_SETUP_TEXT_YEAR=0x1000,
	IDC_MAIN_SETUP_EDIT_YEAR,

	IDC_MAIN_SETUP_TEXT_MON,
	IDC_MAIN_SETUP_EDIT_MON,

	IDC_MAIN_SETUP_TEXT_DAY,
	IDC_MAIN_SETUP_EDIT_DAY,

	IDC_MAIN_SETUP_TEXT_HOUR,
	IDC_MAIN_SETUP_EDIT_HOUR,

	IDC_MAIN_SETUP_TEXT_MIN,
	IDC_MAIN_SETUP_EDIT_MIN,

	IDC_MAIN_SETUP_TEXT_SEC,
	IDC_MAIN_SETUP_EDIT_SEC,

	IDC_MAIN_SETUP_BTN_SAVE,
	IDC_MAIN_SETUP_BTN_RETURN,
};



int setupBoxProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_INITDIALOG:	
			do
			{
				SendDlgItemMessage(hDlg, IDC_MAIN_SETUP_EDIT_YEAR, EM_LIMITTEXT, 4, 0);
				SendDlgItemMessage(hDlg, IDC_MAIN_SETUP_EDIT_MON, EM_LIMITTEXT, 2, 0);
				SendDlgItemMessage(hDlg, IDC_MAIN_SETUP_EDIT_DAY, EM_LIMITTEXT, 2, 0);
				SendDlgItemMessage(hDlg, IDC_MAIN_SETUP_EDIT_HOUR, EM_LIMITTEXT, 2, 0);
				SendDlgItemMessage(hDlg, IDC_MAIN_SETUP_EDIT_MIN, EM_LIMITTEXT, 2, 0);
				SendDlgItemMessage(hDlg, IDC_MAIN_SETUP_EDIT_SEC, EM_LIMITTEXT, 2, 0);

				char buf[6];
				struct tm tm;
				time_t tt=time(NULL);
				if(localtime_r(&tt, &tm))
				{
					snprintf(buf, sizeof(buf), "%d", tm.tm_year+1900);
					SetDlgItemText(hDlg, IDC_MAIN_SETUP_EDIT_YEAR, buf);

					snprintf(buf, sizeof(buf), "%d", tm.tm_mon+1);
					SetDlgItemText(hDlg, IDC_MAIN_SETUP_EDIT_MON, buf);

					snprintf(buf, sizeof(buf), "%d", tm.tm_mday);
					SetDlgItemText(hDlg, IDC_MAIN_SETUP_EDIT_DAY, buf);

					snprintf(buf, sizeof(buf), "%d", tm.tm_hour);
					SetDlgItemText(hDlg, IDC_MAIN_SETUP_EDIT_HOUR, buf);

					snprintf(buf, sizeof(buf), "%d", tm.tm_min);
					SetDlgItemText(hDlg, IDC_MAIN_SETUP_EDIT_MIN, buf);

					snprintf(buf, sizeof(buf), "%d", tm.tm_sec);
					SetDlgItemText(hDlg, IDC_MAIN_SETUP_EDIT_SEC, buf);

				}
				else
				{
					printf("can not get time %m\n");
				}
			}while(0);
		break;		
		case MSG_COMMAND:
			do
			{				
				const int id = LOWORD(wParam);
				const int code = HIWORD(wParam);

				if(code==BN_CLICKED)
				{
					switch(id)
					{
						case IDC_MAIN_SETUP_BTN_SAVE:
							do
							{
								char syear[5]={0};
								char smon[3]={0};
								char sday[3]={0};
								char shour[3]={0};
								char smin[3]={0};
								char ssec[3]={0};
								
								GetDlgItemText(hDlg,IDC_MAIN_SETUP_EDIT_YEAR,syear,4);
								GetDlgItemText(hDlg,IDC_MAIN_SETUP_EDIT_MON,smon,2);
								GetDlgItemText(hDlg,IDC_MAIN_SETUP_EDIT_DAY,sday,2);
								GetDlgItemText(hDlg,IDC_MAIN_SETUP_EDIT_HOUR,shour,2);
								GetDlgItemText(hDlg,IDC_MAIN_SETUP_EDIT_MIN,smin,2);
								GetDlgItemText(hDlg,IDC_MAIN_SETUP_EDIT_SEC,ssec,2);
								
								struct tm tm;
								tm.tm_year=atoi(syear)-1900;
								tm.tm_mon=atoi(smon)-1;
								tm.tm_mday=atoi(sday);
								tm.tm_hour=atoi(shour);
								tm.tm_min=atoi(smin);
								tm.tm_sec=atoi(ssec);

#if 0
								bool berr=false;
								if(tm.tm_year<=1900)
								{
									berr=true;
								}
								if(tm.tm_mon<=0 || tm.tm_mon>=13)
								{
									berr=true;
								}
								if(tm.tm_mday<=0 ||tm.tm_mday>31)
								{
									berr=true;
								}
#endif								

								bool bset=false;
								
								time_t tt=mktime(&tm);
								if(tt!=(time_t)-1)								
								{
									if(0==stime(&tt))
									{
										bset=true;
									}
								}

								if(bset)
								{
									MessageBox(hDlg, osd_setup_ok, osd_tishi, MB_OK);

									system("hwclock -w");
								}
								else
								{
									printf("save time err:%m\n");
									MessageBox(hDlg, osd_setup_fail, osd_tishi, MB_OK);
								}
							}while(0);
						case IDC_MAIN_SETUP_BTN_RETURN:
							do
							{
								EndDialog(hDlg, id);
							}while(0);
						break;
						
					}
				}
				else if(code==EN_CLICKED)
				{
					switch(id)
					{
						case IDC_MAIN_SETUP_EDIT_YEAR:
						case IDC_MAIN_SETUP_EDIT_MON:
						case IDC_MAIN_SETUP_EDIT_DAY:
						case IDC_MAIN_SETUP_EDIT_HOUR:
						case IDC_MAIN_SETUP_EDIT_MIN:
						case IDC_MAIN_SETUP_EDIT_SEC:
							showSoftKeyBoardEnBox(hDlg, (HWND)lParam);
							SetFocus(GetDlgItem(hDlg, IDC_MAIN_SETUP_TEXT_YEAR));
						break;
					}
				}
			}while(0);
		break;
	}

	return DefaultDialogProc (hDlg, message, wParam, lParam);
}







int showSetupBox(HWND hOwner, int w, int h)
{
#define Y_OFF 50
	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			50, 10+Y_OFF, 30, 30,
			IDC_MAIN_SETUP_TEXT_YEAR,
			osd_setup_year,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT|WS_BORDER,
			80, 10+Y_OFF, 80, 30,
			IDC_MAIN_SETUP_EDIT_YEAR,
			"2016",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			170, 10+Y_OFF, 30, 30,
			IDC_MAIN_SETUP_TEXT_MON,
			osd_setup_mon,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT|WS_BORDER,
			200, 10+Y_OFF, 80, 30,
			IDC_MAIN_SETUP_EDIT_MON,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			290, 10+Y_OFF, 30, 30,
			IDC_MAIN_SETUP_TEXT_DAY,
			osd_setup_day,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT|WS_BORDER,
			320, 10+Y_OFF, 80, 30,
			IDC_MAIN_SETUP_EDIT_DAY,
			"",
			0,
			WS_EX_NONE
		},

		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			50, 50+Y_OFF, 30, 30,
			IDC_MAIN_SETUP_TEXT_HOUR,
			osd_setup_hour,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT|WS_BORDER,
			80, 50+Y_OFF, 80, 30,
			IDC_MAIN_SETUP_EDIT_HOUR,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			170, 50+Y_OFF, 30, 30,
			IDC_MAIN_SETUP_TEXT_MIN,
			osd_setup_min,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT|WS_BORDER,
			200, 50+Y_OFF, 80, 30,
			IDC_MAIN_SETUP_EDIT_MIN,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			290, 50+Y_OFF, 30, 30,
			IDC_MAIN_SETUP_TEXT_SEC,
			osd_setup_sec,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT|WS_BORDER,
			320, 50+Y_OFF, 80, 30,
			IDC_MAIN_SETUP_EDIT_SEC,
			"",
			0,
			WS_EX_NONE
		},

		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			170, 120+Y_OFF, 60, 40,
			IDC_MAIN_SETUP_BTN_SAVE,
			osd_setup_save,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			250, 120+Y_OFF, 60, 40,
			IDC_MAIN_SETUP_BTN_RETURN,
			osd_return,
			0,
			WS_EX_NONE
		},
	};

#undef Y_OFF
		
	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE ,
		WS_EX_NONE,
		0, 0, w, h,
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
	
	return DialogBoxIndirectParam(&DlgInfo, hOwner, setupBoxProc, 0);
}









enum
{
	IDC_MAIN_INFO_EDIT_USAGE=0x1100,
	IDC_MAIN_INFO_BTN_EXIT,
};


const char osd_info_msg[]=
{0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xe7, 0xbc, 0xba, 0xe5, 0x8f, 0xa3, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe8, 0xa3, 0x85, 0xe7, 0xbd, 0xae, 0xe4, 0xbd, 0xbf, 0xe7, 0x94, 0xa8, 0xe8, 0xaf, 0xb4, 0xe6, 0x98, 0x8e, 0xe4, 0xb9, 0xa6, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xe5, 0x9e, 0x8b, 0xe5, 0x8f, 0xb7, 0x3a, 0x54, 0x58, 0x51, 0x4b, 0x2d, 0x59, 0x6c, 0x0a, 0x0a, 0xe4, 0xb8, 0x80, 0x2e, 0xe5, 0x8e, 0x9f, 0xe7, 0x90, 0x86, 0xe6, 0xa6, 0x82, 0xe8, 0xbf, 0xb0, 0x0a, 0x20, 0x20, 0x20, 0xe7, 0xbc, 0xba, 0xe5, 0x8f, 0xa3, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe8, 0xa3, 0x85, 0xe7, 0xbd, 0xae, 0xe6, 0x98, 0xaf, 0xe4, 0xb8, 0x93, 0xe7, 0x94, 0xa8, 0xe4, 0xba, 0x8e, 0xe8, 0xbd, 0xac, 0xe8, 0xbe, 0x99, 0xe6, 0x9c, 0xba, 0xe7, 0xbc, 0xba, 0xe5, 0x8f, 0xa3, 0xe7, 0x9b, 0xb4, 0xe6, 0x8e, 0xa5, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe7, 0x9a, 0x84, 0xe6, 0x99, 0xba, 0xe8, 0x83, 0xbd, 0xe4, 0xbb, 0xaa, 0xe5, 0x99, 0xa8, 0x2c, 0xe6, 0xb5, 0x8b, 0x0a, 0xe9, 0x87, 0x8f, 0xe6, 0x8e, 0xa2, 0xe5, 0xa4, 0xb4, 0xe5, 0xae, 0x89, 0xe8, 0xa3, 0x85, 0xe5, 0x9c, 0xa8, 0xe9, 0x87, 0x91, 0xe5, 0xb1, 0x9e, 0xe8, 0xbd, 0xaf, 0xe7, 0xae, 0xa1, 0xe4, 0xb8, 0x8a, 0x2c, 0xe5, 0x8f, 0xaf, 0xe6, 0xa0, 0xb9, 0xe6, 0x8d, 0xae, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe7, 0xa9, 0xba, 0xe9, 0x97, 0xb4, 0xe8, 0xa6, 0x81, 0xe6, 0xb1, 0x82, 0xe4, 0xbb, 0xbb, 0xe6, 0x84, 0x8f, 0xe5, 0xbc, 0xaf, 0xe6, 0x9b, 0xb2, 0x2c, 0xe9, 0x80, 0x9a, 0x0a, 0xe8, 0xbf, 0x87, 0xe6, 0x97, 0x8b, 0xe8, 0xbd, 0xac, 0xe6, 0x9d, 0xa0, 0xe6, 0x9d, 0x86, 0xe5, 0x8e, 0x9f, 0xe7, 0x90, 0x86, 0xe5, 0xb0, 0x86, 0xe7, 0xbc, 0xba, 0xe5, 0x8f, 0xa3, 0xe5, 0xb0, 0xba, 0xe5, 0xaf, 0xb8, 0xe5, 0xa4, 0xa7, 0xe5, 0xb0, 0x8f, 0x2c, 0xe9, 0x80, 0x9a, 0xe8, 0xbf, 0x87, 0xe8, 0xbd, 0xaf, 0xe9, 0x92, 0xa2, 0xe4, 0xb8, 0x9d, 0xe8, 0xbd, 0xac, 0xe5, 0xaf, 0xbc, 0xe5, 0x9c, 0xa8, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe6, 0x89, 0x8b, 0x0a, 0xe6, 0x9f, 0x84, 0xe5, 0x86, 0x85, 0xe7, 0x9a, 0x84, 0xe4, 0xbd, 0x8d, 0xe7, 0xa7, 0xbb, 0xe4, 0xbc, 0xa0, 0xe6, 0x84, 0x9f, 0xe5, 0x99, 0xa8, 0x2c, 0xe4, 0xbd, 0x8d, 0xe7, 0xa7, 0xbb, 0xe4, 0xbc, 0xa0, 0xe6, 0x84, 0x9f, 0xe5, 0x99, 0xa8, 0xe8, 0xbf, 0x9e, 0xe6, 0x8e, 0xa5, 0xe5, 0x88, 0xb0, 0xe6, 0x99, 0xba, 0xe8, 0x83, 0xbd, 0xe5, 0xb9, 0xb3, 0xe6, 0x9d, 0xbf, 0xe7, 0x94, 0xb5, 0xe8, 0x84, 0x91, 0x2c, 0xe8, 0x83, 0xbd, 0xe6, 0x98, 0xbe, 0xe7, 0xa4, 0xba, 0x0a, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe6, 0x95, 0xb0, 0xe6, 0x8d, 0xae, 0x2c, 0xe5, 0xb9, 0xb6, 0xe8, 0x83, 0xbd, 0xe8, 0xae, 0xb0, 0xe5, 0xbd, 0x95, 0xe5, 0x92, 0x8c, 0xe7, 0xae, 0xa1, 0xe7, 0x90, 0x86, 0x2e, 0x0a, 0x20, 0x20, 0x20, 0xe6, 0x8a, 0x80, 0xe6, 0x9c, 0xaf, 0xe6, 0x8c, 0x87, 0xe6, 0xa0, 0x87, 0x3a, 0x0a, 0x20, 0x20, 0x20, 0x31, 0x2e, 0xe4, 0xbd, 0xbf, 0xe7, 0x94, 0xa8, 0xe7, 0x8e, 0xaf, 0xe5, 0xa2, 0x83, 0xe6, 0xb8, 0xa9, 0xe5, 0xba, 0xa6, 0x2d, 0x35, 0xe5, 0xba, 0xa6, 0x2d, 0x35, 0x35, 0xe5, 0xba, 0xa6, 0x20, 0x20, 0x0a, 0x20, 0x20, 0x20, 0x32, 0x2e, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe8, 0x8c, 0x83, 0xe5, 0x9b, 0xb4, 0x31, 0x2e, 0x35, 0x2d, 0x36, 0x2e, 0x35, 0xe6, 0xaf, 0xab, 0xe7, 0xb1, 0xb3, 0x0a, 0x20, 0x20, 0x20, 0x33, 0x2e, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe7, 0xb2, 0xbe, 0xe5, 0xba, 0xa6, 0x30, 0x2e, 0x31, 0xe6, 0xaf, 0xab, 0xe7, 0xb1, 0xb3, 0x0a, 0x20, 0x20, 0x20, 0x34, 0x2e, 0xe5, 0x85, 0x85, 0xe7, 0x94, 0xb5, 0xe7, 0x94, 0xb5, 0xe5, 0x8e, 0x8b, 0x35, 0x56, 0x0a, 0xe4, 0xba, 0x8c, 0x2e, 0xe5, 0x85, 0x85, 0xe7, 0x94, 0xb5, 0x0a, 0x20, 0x20, 0x20, 0xe6, 0xaf, 0x8f, 0xe6, 0xac, 0xa1, 0xe4, 0xbd, 0xbf, 0xe7, 0x94, 0xa8, 0xe5, 0x89, 0x8d, 0x2c, 0xe5, 0x85, 0x88, 0xe7, 0xbb, 0x99, 0xe4, 0xbb, 0xaa, 0xe5, 0x99, 0xa8, 0xe5, 0x85, 0x85, 0xe7, 0x94, 0xb5, 0x2c, 0xe5, 0x86, 0x85, 0xe7, 0xbd, 0xae, 0xe5, 0x85, 0x85, 0xe7, 0x94, 0xb5, 0xe5, 0xae, 0x9d, 0x2c, 0xe6, 0x9c, 0x89, 0xe6, 0x95, 0xb0, 0xe5, 0xad, 0x97, 0xe6, 0x98, 0xbe, 0xe7, 0xa4, 0xba, 0xe7, 0x94, 0xb5, 0xe6, 0xb1, 0xa0, 0xe7, 0x94, 0xb5, 0x0a, 0xe9, 0x87, 0x8f, 0x2e, 0x0a, 0xe4, 0xb8, 0x89, 0x2e, 
0xe5, 0xbc, 0x80, 0xe6, 0x9c, 0xba, 0x0a, 0x20, 0x20, 0x20, 0xe6, 0x8c, 0x89, 0xe5, 0x85, 0x85, 0xe7, 0x94, 0xb5, 0xe7, 0x94, 0xb5, 0xe6, 0xba, 0x90, 0xe6, 0x8c, 0x89, 0xe9, 0x92, 0xae, 0x2c, 0xe5, 0x86, 0x8d, 0xe6, 0x89, 0x93, 0xe5, 0xbc, 0x80, 0xe7, 0x94, 0xb5, 0xe6, 0xba, 0x90, 0xe5, 0xbc, 0x80, 0xe5, 0x85, 0xb3, 0x2c, 0xe8, 0xae, 0xbe, 0xe5, 0xa4, 0x87, 0xe5, 0x90, 0xaf, 0xe5, 0x8a, 0xa8, 0xe6, 0x88, 0x90, 0xe5, 0x8a, 0x9f, 0xe5, 0x90, 0x8e, 0x2c, 0xe8, 0xbf, 0x9b, 0xe5, 0x85, 0xa5, 0xe7, 0x99, 0xbb, 0x0a, 0xe5, 0xbd, 0x95, 0xe7, 0x95, 0x8c, 0xe9, 0x9d, 0xa2, 0x2c, 0xe8, 0xbe, 0x93, 0xe5, 0x85, 0xa5, 0xe7, 0x94, 0xa8, 0xe6, 0x88, 0xb7, 0xe5, 0x90, 0x8d, 0xe5, 0x90, 0x8d, 0xe5, 0x92, 0x8c, 0xe5, 0xaf, 0x86, 0xe7, 0xa0, 0x81, 0xe5, 0x8d, 0xb3, 0xe5, 0x8f, 0xaf, 0xe7, 0x99, 0xbb, 0xe5, 0xbd, 0x95, 0x2e, 0x0a, 0xe5, 0x9b, 0x9b, 0x2e, 0xe6, 0xb3, 0xa8, 0xe5, 0x86, 0x8c, 0x0a, 0x20, 0x20, 0x20, 0x31, 0x2e, 0xe7, 0x99, 0xbb, 0xe5, 0xbd, 0x95, 0xe6, 0x88, 0x90, 0xe5, 0x8a, 0x9f, 0xe5, 0x90, 0x8e, 0x2c, 0xe8, 0xbf, 0x9b, 0xe5, 0x85, 0xa5, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe4, 0xb8, 0xbb, 0xe9, 0xa1, 0xb5, 0xe9, 0x9d, 0xa2, 0x2e, 0xe6, 0x89, 0x93, 0xe5, 0xbc, 0x80, 0xe5, 0xb7, 0xa6, 0xe4, 0xb8, 0x8a, 0xe8, 0xa7, 0x92, 0xe7, 0x9a, 0x84, 0xe8, 0x8f, 0x9c, 0xe5, 0x8d, 0x95, 0x2c, 0xe7, 0x82, 0xb9, 0xe5, 0x87, 0xbb, 0xe6, 0xb3, 0xa8, 0x0a, 0xe5, 0x86, 0x8c, 0xe5, 0x91, 0xbd, 0xe4, 0xbb, 0xa4, 0x2e, 0xe8, 0xbf, 0x9b, 0xe5, 0x85, 0xa5, 0xe6, 0xb3, 0xa8, 0xe5, 0x86, 0x8c, 0xe7, 0x95, 0x8c, 0xe9, 0x9d, 0xa2, 0x2e, 0x0a, 0x20, 0x20, 0x20, 0x32, 0x2e, 0xe7, 0x94, 0xa8, 0xe6, 0x88, 0xb7, 0xe8, 0xba, 0xab, 0xe4, 0xbb, 0xbd, 0xe7, 0x99, 0xbb, 0xe9, 0x99, 0x86, 0xe7, 0x9a, 0x84, 0x2c, 0xe5, 0x8f, 0xaf, 0xe4, 0xbb, 0xa5, 0xe5, 0xaf, 0xbc, 0xe5, 0x87, 0xba, 0xe6, 0x95, 0xb0, 0xe6, 0x8d, 0xae, 0xe5, 0x92, 0x8c, 0xe6, 0x9f, 0xa5, 0xe9, 0x98, 0x85, 0xe8, 0xaf, 0xb4, 0xe6, 0x98, 0x8e, 0xe4, 0xb9, 0xa6, 0x2e, 0x0a, 0x20, 0x20, 0x20, 0x33, 0x2e, 0xe7, 0xae, 0xa1, 0xe7, 0x90, 0x86, 0xe5, 0x91, 0x98, 0xe8, 0xba, 0xab, 0xe4, 0xbb, 0xbd, 0xe7, 0x99, 0xbb, 0xe9, 0x99, 0x86, 0xe7, 0x9a, 0x84, 0x2c, 0xe5, 0x8f, 0xaf, 0xe4, 0xbb, 0xa5, 0xe6, 0xb7, 0xbb, 0xe5, 0x8a, 0xa0, 0x2e, 0xe4, 0xbf, 0xae, 0xe6, 0x94, 0xb9, 0x2e, 0xe5, 0x88, 0xa0, 0xe9, 0x99, 0xa4, 0xe7, 0x94, 0xa8, 0xe6, 0x88, 0xb7, 0xe5, 0x92, 0x8c, 0xe8, 0xbd, 0xac, 0xe8, 0xbe, 0x99, 0xe6, 0x9c, 0xba, 0x2c, 0x0a, 0xe4, 0xb9, 0x9f, 0xe5, 0x8f, 0xaf, 0xe4, 0xbb, 0xa5, 0xe5, 0xaf, 0xbc, 0xe5, 0x87, 0xba, 0xe6, 0x95, 0xb0, 0xe6, 0x8d, 0xae, 0xe5, 0x92, 0x8c, 0xe6, 0x9f, 0xa5, 0xe9, 0x98, 0x85, 0xe8, 0xaf, 0xb4, 0xe6, 0x98, 0x8e, 0xe4, 0xb9, 0xa6, 0x2e, 0x0a, 0xe4, 0xba, 0x94, 0x2e, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0x0a, 0x20, 0x20, 0x20, 0x31, 0x2e, 0xe9, 0x92, 0x88, 0xe5, 0xaf, 0xb9, 0xe6, 0xaf, 0x8f, 0xe7, 0xa7, 0x8d, 0xe8, 0xbd, 0xac, 0xe8, 0xbe, 0x99, 0xe6, 0x9c, 0xba, 0xe7, 0x9a, 0x84, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe7, 0xa9, 0xba, 0xe9, 0x97, 0xb4, 0x2c, 0xe5, 0xbc, 0xaf, 0xe5, 0xa5, 0xbd, 0xe9, 0x87, 0x91, 0xe5, 0xb1, 0x9e, 0xe8, 0xbd, 0xaf, 0xe7, 0xae, 0xa1, 0xe7, 0x9a, 0x84, 0xe5, 0xbd, 0xa2, 0xe7, 0x8a, 0xb6, 0x2c, 0xe8, 0xbf, 0x9b, 0xe8, 0xa1, 0x8c, 0x0a, 0xe8, 0xae, 0xbe, 0xe5, 0xa4, 0x87, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x32, 0x2e, 0xe6, 0xaf, 0x8f, 0xe6, 0xac, 0xa1, 0xe5, 0xbc, 0x80, 0xe6, 0x9c, 0xba, 0x2c, 0xe6, 0xb2, 0xa1, 0xe6, 0x9c, 0x89, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe6, 0x97, 0xb6, 0x2c, 0xe6, 0x8c, 0x89, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe6, 0x8c, 0x89, 0xe9, 0x92, 0xae, 0x2c, 0xe5, 0xbc, 0xb9, 0xe5, 0x87, 0xba, 0xe7, 0xaa, 0x97, 0xe5, 0x8f, 0xa3, 0xe6, 0x8f, 0x90, 0xe9, 0x86, 0x92, 0x2c, 0xe9, 0x80, 0x89, 0xe6, 0x8b, 0xa9, 0x0a, 0xe7, 0xbb, 0xa7, 0xe7, 0xbb, 0xad, 0x2c, 0xe5, 0xb0, 0x86, 0xe9, 0x87, 0x87, 0xe5, 0x8f, 0x96, 0xe4, 0xbb, 0xa5, 0xe5, 0x89, 0x8d, 0xe7, 0x9a, 0x84, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe6, 0x95, 0xb0, 0xe6, 0x8d, 0xae, 0x2e, 0x0a, 0x20, 0x20, 0x20, 0x33, 0x2e, 0xe6, 0x8b, 0x87, 0xe6, 0x8c, 0x87, 0xe6, 0x8e, 0xa8, 0xe6, 0x89, 0x8b, 0xe6, 0x9f, 0x84, 0xe4, 0xb8, 0x8a, 0xe7, 0x9a, 0x84, 0xe5
, 0xbc, 0x80, 0xe5, 0x85, 0xb3, 0xe5, 0xa4, 0x8d, 0xe4, 0xbd, 0x8d, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe8, 0x88, 0x8c, 0x2c, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe8, 0x88, 0x8c, 0xe6, 0x94, 0xbe, 0xe8, 0xbf, 0x9b, 0x32, 0xe6, 0xaf, 0xab, 0xe7, 0xb1, 0xb3, 0xe6, 0xa0, 0x87, 0xe5, 0x87, 0x86, 0x0a, 0xe7, 0xbc, 0x9d, 0xe9, 0x9a, 0x99, 0xe5, 0x9d, 0x97, 0xe7, 0x9a, 0x84, 0xe7, 0xbc, 0x9d, 0xe9, 0x9a, 0x99, 0xe4, 0xb8, 0xad, 0x2c, 0xe6, 0x8c, 0x89, 0xe6, 0xa8, 0xa1, 0xe5, 0xbc, 0x8f, 0xe5, 0x88, 0x87, 0xe6, 0x8d, 0xa2, 0xe6, 0x8c, 0x89, 0xe9, 0x92, 0xae, 0xe5, 0x88, 0x87, 0xe6, 0x8d, 0xa2, 0xe5, 0x88, 0xb0, 0x32, 0x4d, 0x4d, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe6, 0xa8, 0xa1, 0xe5, 0xbc, 0x8f, 0x2c, 0xe5, 0x86, 0x8d, 0xe6, 0x8c, 0x89, 0xe6, 0xb5, 0x8b, 0x0a, 0xe9, 0x87, 0x8f, 0xe6, 0x8c, 0x89, 0xe9, 0x92, 0xae, 0x2c, 0x32, 0xe6, 0xaf, 0xab, 0xe7, 0xb1, 0xb3, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe5, 0xae, 0x8c, 0xe6, 0x88, 0x90, 0x2c, 0x34, 0xe6, 0xaf, 0xab, 0xe7, 0xb1, 0xb3, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe6, 0x96, 0xb9, 0xe6, 0xb3, 0x95, 0xe7, 0xb1, 0xbb, 0xe5, 0x90, 0x8c, 0x2e, 0x0a, 0xe5, 0x85, 0xad, 0x2e, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0x0a, 0x20, 0x20, 0x20, 0x31, 0x2e, 0xe6, 0x8c, 0x89, 0xe4, 0xb8, 0x8a, 0xe6, 0x88, 0x96, 0xe4, 0xb8, 0x8b, 0xe6, 0x8c, 0x89, 0xe9, 0x94, 0xae, 0x2c, 0xe9, 0x80, 0x89, 0xe6, 0x8b, 0xa9, 0xe8, 0xbd, 0xac, 0xe8, 0xbe, 0x99, 0xe6, 0x9c, 0xba, 0xe5, 0x9e, 0x8b, 0xe5, 0x8f, 0xb7, 0x2c, 0xe4, 0xb8, 0x8e, 0xe5, 0xae, 0x9e, 0xe9, 0x99, 0x85, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe7, 0x9a, 0x84, 0xe8, 0xbd, 0xac, 0xe8, 0xbe, 0x99, 0xe6, 0x9c, 0xba, 0xe5, 0xaf, 0xb9, 0x0a, 0xe5, 0xba, 0x94, 0x2e, 0x0a, 0x20, 0x20, 0x20, 0x32, 0x2e, 0xe9, 0x92, 0x88, 0xe5, 0xaf, 0xb9, 0xe6, 0xaf, 0x8f, 0xe7, 0xa7, 0x8d, 0xe8, 0xbd, 0xac, 0xe8, 0xbe, 0x99, 0xe6, 0x9c, 0xba, 0xe7, 0x9a, 0x84, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe7, 0xa9, 0xba, 0xe9, 0x97, 0xb4, 0x2c, 0xe5, 0xbc, 0xaf, 0xe5, 0xa5, 0xbd, 0xe9, 0x87, 0x91, 0xe5, 0xb1, 0x9e, 0xe8, 0xbd, 0xaf, 0xe7, 0xae, 0xa1, 0xe7, 0x9a, 0x84, 0xe5, 0xbd, 0xa2, 0xe7, 0x8a, 0xb6, 0x2c, 0xe8, 0xbf, 0x9b, 0xe8, 0xa1, 0x8c, 0x0a, 0xe8, 0xae, 0xbe, 0xe5, 0xa4, 0x87, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0x2e, 0x0a, 0x20, 0x20, 0x20, 0x33, 0x2e, 0xe6, 0x8b, 0x87, 0xe6, 0x8c, 0x87, 0xe6, 0x8e, 0xa8, 0xe5, 0xa4, 0x8d, 0xe4, 0xbd, 0x8d, 0xe5, 0xbc, 0x80, 0xe5, 0x85, 0xb3, 0xe5, 0xa4, 0x8d, 0xe4, 0xbd, 0x8d, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe8, 0x88, 0x8c, 0x2c, 0xe5, 0xb0, 0x86, 0xe4, 0xbb, 0xaa, 0xe5, 0x99, 0xa8, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe5, 0xa4, 0xb4, 0xe7, 0xbc, 0x93, 0xe6, 0x85, 0xa2, 0xe8, 0xbd, 0xbb, 0xe6, 0x94, 0xbe, 0x2c, 0xe7, 0xa1, 0xae, 0x0a, 0xe4, 0xbf, 0x9d, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe5, 0xa4, 0xb4, 0xe7, 0xb4, 0xa7, 0xe8, 0xb4, 0xb4, 0xe6, 0xa3, 0x80, 0xe6, 0x9f, 0xa5, 0xe6, 0x9f, 0xb1, 0xe5, 0x92, 0x8c, 0xe5, 0xaf, 0xbc, 0xe8, 0xbd, 0xa8, 0x2c, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe8, 0x88, 0x8c, 0xe4, 0xbc, 0xb8, 0xe8, 0xbf, 0x9b, 0xe7, 0xbc, 0xba, 0xe5, 0x8f, 0xa3, 0x2c, 0xe6, 0x8b, 0x87, 0xe6, 0x8c, 0x87, 0xe6, 0x94, 0xbe, 0xe5, 0xbc, 0x80, 0xe5, 0xa4, 0x8d, 0xe4, 0xbd, 0x8d, 0xe5, 0xbc, 0x80, 0x0a, 0xe5, 0x85, 0xb3, 0x2c, 0xe8, 0xa7, 0x82, 0xe5, 0xaf, 0x9f, 0xe8, 0xae, 0xb0, 0xe5, 0xbd, 0x95, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe6, 0x95, 0xb0, 0xe5, 0x80, 0xbc, 0x2c, 0xe6, 0x8c, 0x89, 0xe6, 0xa8, 0xa1, 0xe5, 0xbc, 0x8f, 0xe5, 0x88, 0x87, 0xe6, 0x8d, 0xa2, 0xe6, 0x8c, 0x89, 0xe9, 0x92, 0xae, 0xe5, 0x88, 0x87, 0xe6, 0x8d, 0xa2, 0xe5, 0x88, 0xb0, 0xe5, 0xaf, 0xb9, 0xe5, 0xba, 0x94, 0xe7, 0x9a, 0x84, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe6, 0xa8, 0xa1, 0x0a, 0xe5, 0xbc, 0x8f, 0xe6, 0xa8, 0xa1, 0xe5, 0xbc, 0x8f, 0x2c, 0xe6, 0x8c, 0x89, 0xe5, 0x8e, 0x8b, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe6, 0x8c, 0x89, 0xe9, 0x94, 0xae, 0x2c, 0xe4, 0xbf, 0x9d, 0xe5, 0xad, 0x98, 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe8, 0xae, 0xb0, 0xe5, 0xbd, 0x95, 0x2e, 0x0a, 0xe4, 0xb8, 0x83, 0x2e, 0xe5, 0xaf, 0xbc, 0xe5, 0x87, 0xba, 0xe8, 0xae, 0xb0, 0xe5, 0xbd, 0x95, 0x0a, 0x20, 0x20, 0x20, 0xe6, 0x8c, 0x89, 0xe8, 0x8f, 0x9c, 0xe5, 
0x8d, 0x95, 0xe8, 0xbf, 0x9b, 0xe5, 0x85, 0xa5, 0x55, 0xe7, 0x9b, 0x98, 0xe5, 0xaf, 0xbc, 0xe5, 0x87, 0xba, 0xe7, 0x95, 0x8c, 0xe9, 0x9d, 0xa2, 0x2c, 0xe8, 0xbf, 0x9e, 0xe6, 0x8e, 0xa5, 0xe5, 0xa5, 0xbd, 0x55, 0xe7, 0x9b, 0x98, 0x2c, 0xe6, 0x8c, 0x89, 0x55, 0xe7, 0x9b, 0x98, 0xe5, 0xaf, 0xbc, 0xe5, 0x87, 0xba, 0xe6, 0x8c, 0x89, 0xe9, 0x92, 0xae, 0x2c, 0xe5, 0x8f, 0xaf, 0xe9, 0x80, 0x89, 0x0a, 0xe6, 0x8c, 0x89, 0xe6, 0x97, 0xb6, 0xe9, 0x97, 0xb4, 0xe5, 0x92, 0x8c, 0xe6, 0x9c, 0xba, 0xe5, 0x9e, 0x8b, 0xe5, 0xaf, 0xbc, 0xe5, 0x87, 0xba, 0xe6, 0x89, 0x80, 0xe8, 0xa6, 0x81, 0xe7, 0x9a, 0x84, 0xe8, 0xae, 0xb0, 0xe5, 0xbd, 0x95, 0x2e, 0x00};


int infoBoxProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_INITDIALOG:
			do
			{			
				//SetWindowFont(GetDlgItem(hDlg, IDC_MAIN_INFO_EDIT_USAGE),(PLOGFONT)GetSystemFont(4));
				//SetDlgItemText(hDlg, IDC_MAIN_INFO_EDIT_USAGE, osd_info_msg);
			}while(0);
		break;

		case MSG_COMMAND:
			do
			{				
				const int id = LOWORD(wParam);
				const int code = HIWORD(wParam);

				if(code==BN_CLICKED)
				{
					switch(id)
					{
						case IDC_MAIN_INFO_BTN_EXIT:							
							do
							{
								EndDialog(hDlg, id);
							}while(0);
						break;
					}
				}				
			}while(0);
		break;
	}

	return DefaultDialogProc (hDlg, message, wParam, lParam);
}


int showInfoBox(HWND hOwner, int w, int h)
{
	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_MLEDIT,
			WS_CHILD | WS_VISIBLE |ES_MULTILINE/*|ES_AUTOVSCROLL*/|ES_READONLY|WS_VSCROLL,
			10, 10, w-20, h-90,
			IDC_MAIN_INFO_EDIT_USAGE,
			osd_info_msg,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			(w-40)/2, h-70, 60, 30,
			IDC_MAIN_INFO_BTN_EXIT,
			osd_return,
			0,
			WS_EX_NONE
		},
	};

		
	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE |WS_BORDER | WS_CAPTION,
		WS_EX_NONE,
		0, 0, w, h,
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
	
	return DialogBoxIndirectParam(&DlgInfo, hOwner, infoBoxProc, 0);
}




enum
{
	IDC_MAIN_EXPORT_TEXT_UINFO=0x1200,
	IDC_MAIN_EXPORT_TEXT_UINFO_VAL,
	IDC_MAIN_EXPORT_TEXT_MACHINE,
	IDC_MAIN_EXPORT_LIST_MACHINE,
	IDC_MAIN_EXPORT_CBOX_MACHINE,

	IDC_MAIN_EXPORT_TEXT_TIME,
	IDC_MAIN_EXPORT_EDIT_SDATE,
	IDC_MAIN_EXPORT_TEXT_GAP,
	IDC_MAIN_EXPORT_EDIT_DDATE,	
	IDC_MAIN_EXPORT_CBOX_TIME,
	
	IDC_MAIN_EXPORT_BTN_EXPORT,
	IDC_MAIN_EXPORT_BTN_EXIT,
};



bool m_export_usb_conn=false;
int exportBoxProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_INITDIALOG:
			do
			{
				SetTimer(hDlg, MAIN_TIMER_USB_STATE, 50);
				EnableWindow(GetDlgItem(hDlg, IDC_MAIN_EXPORT_BTN_EXPORT), FALSE);

				//SetWindowFont(GetDlgItem(hDlg, IDC_MAIN_EXPORT_LIST_MACHINE), (PLOGFONT)GetSystemFont(4));
				//SetWindowFont(GetDlgItem(hDlg, IDC_MAIN_EXPORT_EDIT_SDATE), (PLOGFONT)GetSystemFont(4));
				//SetWindowFont(GetDlgItem(hDlg, IDC_MAIN_EXPORT_EDIT_DDATE), (PLOGFONT)GetSystemFont(4));
				
				MachineManager* mm=MachineManager::getInstance();
				int id_cur=0;
				while(1)
				{
					int id_ret;
					if(mm->getNextMachineId(id_cur, &id_ret))
					{
						string mname;
						if(mm->getMachineName(id_ret, mname))
						{
							int idx=SendDlgItemMessage(hDlg, IDC_MAIN_EXPORT_LIST_MACHINE, CB_ADDSTRING, 0,(LPARAM)mname.c_str());
							if(idx>=0)
							{
								SendDlgItemMessage(hDlg, IDC_MAIN_EXPORT_LIST_MACHINE, CB_SETITEMADDDATA, (WPARAM)idx, (LPARAM)id_ret);
							}
						}
					}
					else
					{
						break;
					}

					id_cur=id_ret;
				}
			}while(0);
		break;
		case MSG_DESTROY:
			KillTimer(hDlg, MAIN_TIMER_USB_STATE);
		break;
		case MSG_TIMER:
			do
			{
				int timer_id = (int)wParam;
				//DWORD tick_count = (DWORD)lParam;
				if(timer_id==MAIN_TIMER_USB_STATE)
				{
					if(Hsudev::getInstance()->checkUsb())
					{
						if(!m_export_usb_conn)
						{
							m_export_usb_conn=true;
							SetDlgItemText(hDlg, IDC_MAIN_EXPORT_TEXT_UINFO_VAL, osd_exp_ustat_ok);
							EnableWindow(GetDlgItem(hDlg, IDC_MAIN_EXPORT_BTN_EXPORT), TRUE);
						}						
					}
					else
					{
						if(m_export_usb_conn)
						{
							m_export_usb_conn=false;
							SetDlgItemText(hDlg, IDC_MAIN_EXPORT_TEXT_UINFO_VAL, osd_exp_check);
							EnableWindow(GetDlgItem(hDlg, IDC_MAIN_EXPORT_BTN_EXPORT), FALSE);
						}
					}
				}
			}while(0);
		break;
		case MSG_COMMAND:
			do
			{				
				const int id = LOWORD(wParam);
				const int code = HIWORD(wParam);

				if(code==BN_CLICKED)
				{
					switch(id)
					{
						case IDC_MAIN_EXPORT_BTN_EXPORT:
							if(m_export_usb_conn)
							{
								int mid=0;
								string stime, dtime;
								MainRecord* mr=MainRecord::getInstance();

								int check=SendDlgItemMessage(hDlg, IDC_MAIN_EXPORT_CBOX_MACHINE, BM_GETCHECK,0,0);
								if(BST_CHECKED!=check)
								{
									int index=SendDlgItemMessage(hDlg, IDC_MAIN_EXPORT_LIST_MACHINE, CB_GETCURSEL, 0, 0);
									if(index>=0)
									{
										mid=SendDlgItemMessage(hDlg, IDC_MAIN_EXPORT_LIST_MACHINE, CB_GETITEMADDDATA, (WPARAM)index, 0);
									}
								}

								check=SendDlgItemMessage(hDlg, IDC_MAIN_EXPORT_CBOX_TIME, BM_GETCHECK,0,0);
								if(BST_CHECKED!=check)
								{
									char buf[32];
									int len=GetDlgItemText(hDlg, IDC_MAIN_EXPORT_EDIT_SDATE, buf, sizeof(buf)-1);
									buf[len]=0;
									stime=buf;
									len=GetDlgItemText(hDlg, IDC_MAIN_EXPORT_EDIT_DDATE, buf, sizeof(buf)-1);
									buf[len]=0;
									dtime=buf;
								}

								int fd=Hsudev::getInstance()->getUsbExportFd();
								int saveFlag=mr->saveToFile(fd, mid, stime, dtime);
								close(fd);
								
								if(saveFlag)
								{
									MessageBox(hDlg, osd_fail, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_success, osd_tishi, MB_OK);
								}								
							}							
						break;
						case IDC_MAIN_EXPORT_BTN_EXIT:						
							do
							{								
								EndDialog(hDlg, id);
							}while(0);
						break;
					}
				}
				else if(EN_CLICKED==code)
				{
					switch(id)
					{
						case IDC_MAIN_EXPORT_EDIT_SDATE:
						case IDC_MAIN_EXPORT_EDIT_DDATE:
							showDateBox(hDlg, (HWND)lParam, MainFrame::getWidth(), MainFrame::getHeight());
							SetFocus(GetDlgItem(hDlg, IDC_MAIN_EXPORT_TEXT_UINFO));
						break;
					}
				}
			}while(0);
		break;
	}

	return DefaultDialogProc (hDlg, message, wParam, lParam);
}



int showExportBox(HWND hOwner, int w, int h)
{
	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
			50, 10, 80, 30,
			IDC_MAIN_EXPORT_TEXT_UINFO,
			osd_exp_ustat,
			0,
			WS_EX_TRANSPARENT
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
			130, 10, 160, 30,
			IDC_MAIN_EXPORT_TEXT_UINFO_VAL,
			osd_exp_check,
			0,
			WS_EX_TRANSPARENT
		},

		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
			50, 50, 50, 30,
			IDC_MAIN_EXPORT_TEXT_MACHINE,
			osd_exp_machine,
			0,
			WS_EX_TRANSPARENT
		},
		{
			CTRL_COMBOBOX,
			WS_CHILD | WS_VISIBLE |CBS_DROPDOWNLIST|CBS_AUTOHSCROLL,
			110, 50, 100, 30,
			IDC_MAIN_EXPORT_LIST_MACHINE,
			"",
			0,
			WS_EX_TRANSPARENT
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP |BS_AUTOCHECKBOX,
			240, 50, 80, 40,
			IDC_MAIN_EXPORT_CBOX_MACHINE,
			osd_exp_all,
			0,
			WS_EX_NONE
		},

		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
			50, 100, 50, 30,
			IDC_MAIN_EXPORT_TEXT_TIME,
			osd_exp_time,
			0,
			WS_EX_TRANSPARENT
		},		
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |WS_BORDER|ES_READONLY,
			110, 100, 100, 30,
			IDC_MAIN_EXPORT_EDIT_SDATE,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
			220, 100, 10, 30,
			IDC_MAIN_EXPORT_TEXT_GAP,
			"-",
			0,
			WS_EX_TRANSPARENT
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |WS_BORDER|ES_READONLY,
			240, 100, 100, 30,
			IDC_MAIN_EXPORT_EDIT_DDATE,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP |BS_AUTOCHECKBOX,
			350, 100, 80, 40,
			IDC_MAIN_EXPORT_CBOX_TIME,
			osd_exp_all,
			0,
			WS_EX_NONE
		},

		
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP ,
			150, 150, 60, 40,
			IDC_MAIN_EXPORT_BTN_EXPORT,
			osd_exp_export,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			240, 150, 60, 40,
			IDC_MAIN_EXPORT_BTN_EXIT,
			osd_exp_return,
			0,
			WS_EX_NONE
		},		
	};

		
	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE |WS_BORDER | WS_CAPTION,
		WS_EX_NONE,
		0, 0, w, h,
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
	
	return DialogBoxIndirectParam(&DlgInfo, hOwner, exportBoxProc, 0);
}








enum
{
	IDC_MAIN_REG_TEXT_USER=0x1300,
	IDC_MAIN_REG_EDIT_USER,
	IDC_MAIN_REG_CHECK_MANAGER,
	IDC_MAIN_REG_TEXT_PWD,
	IDC_MAIN_REG_EDIT_PWD,
	IDC_MAIN_REG_BTN_ADD_ROLE,
	IDC_MAIN_REG_BTN_DEL_ROLE,

	IDC_MAIN_REG_TEXT_MACHINE,
	IDC_MAIN_REG_EDIT_MACHINE,
	IDC_MAIN_REG_BTN_ADD_MACHINE,
	IDC_MAIN_REG_BTN_DEL_MACHINE,
	IDC_MAIN_REG_BTN_RETURN,
};




int regBoxProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_CREATE:
			if(m_user_type>ROLE_MANAGER)			
			{
				EnableWindow(GetDlgItem(hDlg, IDC_MAIN_REG_BTN_ADD_ROLE), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_MAIN_REG_BTN_ADD_MACHINE), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_MAIN_REG_BTN_DEL_ROLE), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_MAIN_REG_BTN_DEL_MACHINE), FALSE);
			}
		break;
		case MSG_CLOSE:
			EndDialog(hDlg, 0);
		break;
		case MSG_COMMAND:
			do
			{				
				const int id = LOWORD(wParam);
				const int code = HIWORD(wParam);
				HWND hCtrl = (HWND)lParam;

				if(code==BN_CLICKED)
				{
					switch(id)
					{
						case IDC_MAIN_REG_CHECK_MANAGER:
							//SendDlgItemMessage(hDlg, id, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
							//return 0;
						break;
						case IDC_MAIN_REG_BTN_ADD_ROLE:
							do
							{
								HWND huser=GetDlgItem(hDlg, IDC_MAIN_REG_EDIT_USER);
								HWND hpwd=GetDlgItem(hDlg, IDC_MAIN_REG_EDIT_PWD);

								unsigned int checked=SendDlgItemMessage(hDlg, IDC_MAIN_REG_CHECK_MANAGER, BM_GETCHECK, 0, 0);
								int userlen=GetWindowTextLength(huser);
								int pwdlen=GetWindowTextLength(hpwd);

								if((userlen<=0) || (pwdlen<=0))
								{
									MessageBox(hDlg, osd_reg_errlen, osd_tishi, MB_OK);
									break;
								}

								char user_buf[userlen+1];
								char pwd_buf[pwdlen+1];

								if(userlen!=GetWindowText(huser, user_buf, userlen))
								{
									MessageBox(hDlg, osd_reg_errlen, osd_tishi, MB_OK);
									break;
								}

								user_buf[userlen]=0;
								if(pwdlen!=GetWindowText(hpwd, pwd_buf, pwdlen))
								{
									MessageBox(hDlg, osd_reg_errlen, osd_tishi, MB_OK);
									break;
								}
								pwd_buf[pwdlen]=0;

								RoleManager* rm=RoleManager::getInstance();
								if(rm->checkexist(user_buf))
								{
									MessageBox(hDlg, osd_reg_errroleexist, osd_tishi, MB_OK);
									break;
								}

								if(rm->add(user_buf, pwd_buf, (int)(checked==BST_CHECKED)?ROLE_MANAGER:ROLE_USER))
								{									
									MessageBox(hDlg, osd_reg_addok, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_reg_addfail, osd_tishi, MB_OK);
								}
								
							}while(0);
						break;
						case IDC_MAIN_REG_BTN_ADD_MACHINE:
							do
							{
								HWND huser=GetDlgItem(hDlg, IDC_MAIN_REG_EDIT_MACHINE);
								
								int userlen=GetWindowTextLength(huser);

								if((userlen<=0))
								{
									MessageBox(hDlg, osd_reg_mcerrlen, osd_tishi, MB_OK);
									break;
								}

								char user_buf[userlen+1];

								if(userlen!=GetWindowText(huser, user_buf, userlen))
								{
									MessageBox(hDlg, osd_reg_mcerrlen, osd_tishi, MB_OK);
									break;
								}

								user_buf[userlen]=0;

								MachineManager* mm=MachineManager::getInstance();
								if(mm->checkexist(user_buf))
								{
									MessageBox(hDlg, osd_reg_mcexist, osd_tishi, MB_OK);
									break;
								}

								if(mm->add(user_buf))
								{									
									MessageBox(hDlg, osd_reg_addok, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_reg_addfail, osd_tishi, MB_OK);
								}
								
							}while(0);
						break;
						case IDC_MAIN_REG_BTN_DEL_ROLE:
							do
							{
								HWND huser=GetDlgItem(hDlg, IDC_MAIN_REG_EDIT_USER);
								
								int userlen=GetWindowTextLength(huser);

								if(userlen<=0)
								{
									MessageBox(hDlg, osd_reg_errlen, osd_tishi, MB_OK);
									break;
								}

								char user_buf[userlen+1];

								if(userlen!=GetWindowText(huser, user_buf, userlen))
								{
									MessageBox(hDlg, osd_reg_errlen, osd_tishi, MB_OK);
									break;
								}

								user_buf[userlen]=0;
								RoleManager* rm=RoleManager::getInstance();
								if(!rm->checkexist(user_buf))
								{									
									break;
								}

								if(rm->del(user_buf))
								{									
									MessageBox(hDlg, osd_reg_delok, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_reg_delfail, osd_tishi, MB_OK);
								}
								
							}while(0);
						break;
						case IDC_MAIN_REG_BTN_DEL_MACHINE:
							do
							{
								HWND huser=GetDlgItem(hDlg, IDC_MAIN_REG_EDIT_MACHINE);
								
								int userlen=GetWindowTextLength(huser);

								if((userlen<=0))
								{
									MessageBox(hDlg, osd_reg_mcerrlen, osd_tishi, MB_OK);
									break;
								}

								char user_buf[userlen+1];

								if(userlen!=GetWindowText(huser, user_buf, userlen))
								{
									MessageBox(hDlg, osd_reg_mcerrlen, osd_tishi, MB_OK);
									break;
								}

								user_buf[userlen]=0;

								MachineManager* mm=MachineManager::getInstance();
								if(!mm->checkexist(user_buf))
								{									
									break;
								}

								if(mm->del(user_buf))
								{									
									MessageBox(hDlg, osd_reg_delok, osd_tishi, MB_OK);
								}
								else
								{
									MessageBox(hDlg, osd_reg_delfail, osd_tishi, MB_OK);
								}
								
							}while(0);
						break;
						case IDC_MAIN_REG_BTN_RETURN:
							EndDialog(hDlg, 0);
						break;
					}
				}
				else if(EN_CLICKED==code)
				{
					showSoftKeyBoardEnBox(hDlg, hCtrl);
					SetFocus(GetDlgItem(hDlg, IDC_MAIN_REG_TEXT_USER));
				}
			}while(0);
		break;
	}

	return DefaultDialogProc (hDlg, message, wParam, lParam);
}





int showRegBox(HWND hOwner, int w, int h)
{
	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			50, 10, 60, 30,
			IDC_MAIN_REG_TEXT_USER,
			osd_reg_user,
			0,
			WS_EX_TRANSPARENT
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT|WS_BORDER,
			120, 10, 180, 30,
			IDC_MAIN_REG_EDIT_USER,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |BS_AUTOCHECKBOX,
			320, 10, 100, 30,
			IDC_MAIN_REG_CHECK_MANAGER,
			osd_reg_mgr,
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			50, 50, 60, 30,
			IDC_MAIN_REG_TEXT_PWD,
			osd_reg_pwd,
			0,
			WS_EX_TRANSPARENT
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT|WS_BORDER,
			120, 50, 180, 30,
			IDC_MAIN_REG_EDIT_PWD,
			"",
			0,
			WS_EX_NONE
		},
		
		

		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE |SS_CENTER,
			50, 100, 60, 30,
			IDC_MAIN_REG_TEXT_MACHINE,
			osd_reg_mac,
			0,
			WS_EX_TRANSPARENT
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_LEFT,
			120, 100, 180, 30,
			IDC_MAIN_REG_EDIT_MACHINE,
			"",
			0,
			WS_EX_NONE
		},
		
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			340, 100, 60, 40,
			IDC_MAIN_REG_BTN_RETURN,
			osd_reg_return,
			0,
			WS_EX_NONE
		},
		

		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			20, 190, 100, 40,
			IDC_MAIN_REG_BTN_ADD_ROLE,
			osd_reg_addu,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			130, 190, 100, 40,
			IDC_MAIN_REG_BTN_DEL_ROLE,
			osd_reg_delu,
			0,
			WS_EX_NONE
		},

		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			250, 190, 100, 40,
			IDC_MAIN_REG_BTN_ADD_MACHINE,
			osd_reg_addm,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			360, 190, 100, 40,
			IDC_MAIN_REG_BTN_DEL_MACHINE,
			osd_reg_delm,
			0,
			WS_EX_NONE
		},

		
	};

		
	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE /*|WS_BORDER | WS_CAPTION*/,
		WS_EX_NONE,
		0, 0, w, h,
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
	
	return DialogBoxIndirectParam(&DlgInfo, hOwner, regBoxProc, 0);
}












enum
{
	IDC_MAIN_MENU_BTN_REG=0x1400,
	IDC_MAIN_MENU_BTN_MAC_MGR,
	IDC_MAIN_MENU_BTN_EXPORT,
	IDC_MAIN_MENU_BTN_SETUP,
	IDC_MAIN_MENU_BTN_CALI,
	IDC_MAIN_MENU_BTN_INFO,
};

int menuBoxProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{		
		case MSG_COMMAND:
			do
			{				
				const int id = LOWORD(wParam);
				const int code = HIWORD(wParam);

				if(code==BN_CLICKED)
				{
					switch(id)
					{
						case IDC_MAIN_MENU_BTN_REG:
						case IDC_MAIN_MENU_BTN_MAC_MGR:
						case IDC_MAIN_MENU_BTN_EXPORT:						
						case IDC_MAIN_MENU_BTN_INFO:
						case IDC_MAIN_MENU_BTN_SETUP:
						case IDC_MAIN_MENU_BTN_CALI:
							do
							{
								EndDialog(hDlg, id);
							}while(0);
						break;
					}
				}				
			}while(0);
		break;
	}

	return DefaultDialogProc (hDlg, message, wParam, lParam);
}





int showMenuBox(HWND hOwner, int x, int y, int w, int h)
{
	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			0, 10, 100, 30,
			IDC_MAIN_MENU_BTN_REG,
			osd_menu_reg,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			0, 50, 100, 30,
			IDC_MAIN_MENU_BTN_MAC_MGR,
			osd_menu_mac_mgr,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			0, 90, 100, 30,
			IDC_MAIN_MENU_BTN_EXPORT,
			osd_menu_export,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			0, 130, 100, 30,
			IDC_MAIN_MENU_BTN_SETUP,
			osd_menu_setup,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			0, 170, 100, 30,
			IDC_MAIN_MENU_BTN_CALI,
			osd_menu_cali,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			0, 210, 100, 30,
			IDC_MAIN_MENU_BTN_INFO,
			osd_menu_info,
			0,
			WS_EX_NONE
		},
	};

		
	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE ,
		WS_EX_NONE,
		x, y, w, h,
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
	
	return DialogBoxIndirectParam(&DlgInfo, hOwner, menuBoxProc, 0);
}











#define ENTERBOX_COLOR_BG 0xff5c5b61

enum 
{
	IDC_ENTER_BOX_TEXT_USER=0x1500,
	IDC_ENTER_BOX_EDIT_NAME,	
	IDC_ENTER_BOX_TEXT_PWD,
	IDC_ENTER_BOX_EDIT_PWD,
	IDC_ENTER_BOX_BTN_OK,
	IDC_ENTER_BOX_BTN_CANCEL,
	IDC_ENTER_BOX_TEXT_VER,
};


bool enterBoxCheckEnter(const char* name, const char* pwd)
{
	return RoleManager::getInstance()->check(name, pwd);
}


int enterBoxProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_INITDIALOG:
			do
			{				
				SetWindowBkColor(hDlg, ENTERBOX_COLOR_BG);
				//SetWindowBkColor(GetDlgItem(hDlg, IDC_ENTER_BOX_EDIT_NAME), ENTERBOX_COLOR_BG);
				SetWindowBkColor(GetDlgItem(hDlg, IDC_ENTER_BOX_EDIT_PWD), ENTERBOX_COLOR_BG);
				//SendDlgItemMessage(hDlg, IDC_ENTER_BOX_EDIT_NAME, EM_LIMITTEXT, USER_NAME_MAX, 0);
				SendDlgItemMessage(hDlg, IDC_ENTER_BOX_EDIT_PWD, EM_LIMITTEXT, USER_PWD_MAX, 0);
				//SetFocus(GetDlgItem(hDlg, IDC_ENTER_BOX_EDIT_NAME));

				SetWindowFont(GetDlgItem(hDlg, IDC_ENTER_BOX_EDIT_NAME), (PLOGFONT)GetSystemFont(0));
				RoleManager* rm=RoleManager::getInstance();
				int id_cur=0;
				bool bsel=false;
				int index=0;
				while(1)
				{
					int id_ret;

					if(!rm->getNextUserId(id_cur, &id_ret))
					{
						break;
					}

					id_cur=id_ret;

					string mname;
					if(rm->getUserName(id_cur, mname))
					{
						SendDlgItemMessage(hDlg, IDC_ENTER_BOX_EDIT_NAME, CB_ADDSTRING, 0, (LPARAM)mname.c_str());
					}

					unsigned int type=10000;
					rm->getRoleType(mname.c_str(), type);

					if(type<=ROLE_MANAGER)
					{
						if(!bsel)
						{
							bsel=true;
							SendDlgItemMessage(hDlg, IDC_ENTER_BOX_EDIT_NAME, CB_SETCURSEL, (WPARAM)index, 0);
						}
					}

					index++;
				}

				
			}while(0);
		break;		
		case MSG_COMMAND:
			do
			{				
				const int id = LOWORD(wParam);
				const int code = HIWORD(wParam);

				if(code==BN_CLICKED)
				{
					switch(id)
					{
						case IDC_ENTER_BOX_BTN_OK:
							do
							{
								char namebuffer[USER_NAME_MAX+1]={0};
							#if 0
								int namelen=sizeof(namebuffer);
								
								SendDlgItemMessage(hDlg, IDC_ENTER_BOX_EDIT_NAME, MSG_GETTEXT, (WPARAM)namelen, (LPARAM)namebuffer);
							#else
								
								int index=SendDlgItemMessage(hDlg, IDC_ENTER_BOX_EDIT_NAME, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
								if(CB_ERR!=index)
								{
									SendDlgItemMessage(hDlg, IDC_ENTER_BOX_EDIT_NAME, CB_GETLBTEXT, (WPARAM)index, (LPARAM)namebuffer);
								}
							#endif
								char pwdbuffer[USER_PWD_MAX+1]={0};
								int pwdlen=sizeof(pwdbuffer);
								SendDlgItemMessage(hDlg, IDC_ENTER_BOX_EDIT_PWD, MSG_GETTEXT, (WPARAM)pwdlen, (LPARAM)pwdbuffer);

								if(enterBoxCheckEnter(namebuffer, pwdbuffer))
								{
									HWND hwdhost=GetHosting(hDlg);
									
									EndDialog(hDlg, 0);

									RoleManager::getInstance()->getRoleType(namebuffer, m_user_type);
									SetActiveWindow(hwdhost);			
								}
								else
								{
									MessageBox(hDlg, osd_enter_err, osd_enter_wrong, MB_OK);
								}
							}while(0);
						break;
					}
				}
				else if(code==EN_CLICKED)
				{
					HWND hcur=GetFocus(hDlg);
					showSoftKeyBoardEnBox(hDlg, hcur);
					SetFocus(GetDlgItem(hDlg, IDC_ENTER_BOX_TEXT_PWD));
				}
			}while(0);
		break;
	}

	return DefaultDialogProc (hDlg, message, wParam, lParam);
}




int showEnterBox(HWND hOwner, int m_w, int m_h)
{
	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
			80, 60, 90, 30,
			IDC_ENTER_BOX_TEXT_USER,
			osd_enter_user,
			0,
			WS_EX_TRANSPARENT
		},
		{
			CTRL_COMBOBOX,
			WS_BORDER|WS_CHILD | WS_VISIBLE |WS_TABSTOP |CBS_DROPDOWNLIST|CBS_AUTOHSCROLL,
			170, 60, 200, 30,
			IDC_ENTER_BOX_EDIT_NAME,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
			80, 110, 90, 30,
			IDC_ENTER_BOX_TEXT_PWD,
			osd_enter_pwd,
			0,
			WS_EX_TRANSPARENT
		},
		{
			CTRL_SLEDIT,
			WS_BORDER|WS_CHILD | WS_VISIBLE |WS_TABSTOP|ES_PASSWORD,
			170, 110, 200, 30,
			IDC_ENTER_BOX_EDIT_PWD,
			"123",
			0,
			WS_EX_NONE
		},

		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			170, 170, 60, 40,
			IDC_ENTER_BOX_BTN_OK,
			osd_enter_enter,
			0,
			WS_EX_NONE
		},

		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE |WS_TABSTOP,
			250, 170, 60, 40,
			IDC_ENTER_BOX_BTN_CANCEL,
			osd_enter_cancel,
			0,
			WS_EX_NONE
		},

		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			m_w-90, m_h-60, 90, 30,
			IDC_ENTER_BOX_TEXT_VER,
			"v1.0",
			0,
			WS_EX_TRANSPARENT
		},
	};

		
	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE ,
		WS_EX_NONE,
		0, 0, m_w, m_h,
		osd_enter_enter,
		0, 0,
		ARRAY_SIZE(CtrlInfo),
		CtrlInfo,
		0
	};

	if(false==TestWindowCreate(hOwner))
	{
		return -1;
	}
	
	return DialogBoxIndirectParam(&DlgInfo, hOwner, enterBoxProc, 0);
}



enum 
{
	IDC_TRY_BOX_TEXT_WARN=0x1600,
};

int tryBoxProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_COMMAND:
			return 0;
		break;
	}
	return DefaultDialogProc (hDlg, message, wParam, lParam);
}


const char* try_key="try";

int showTryBox(HWND hOwner, int m_w, int m_h)
{
	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_STATIC,
			WS_CHILD | WS_VISIBLE | SS_CENTER,
			30, 130, 420, 30,
			IDC_TRY_BOX_TEXT_WARN,
			osd_try_warn,
			0,
			WS_EX_TRANSPARENT
		},
	};

		
	DLGTEMPLATE DlgInfo = 
	{
		WS_VISIBLE ,
		WS_EX_NONE,
		0, 0, m_w, m_h,
		try_key,
		0, 0,
		ARRAY_SIZE(CtrlInfo),
		CtrlInfo,
		(DWORD)try_key
	};

	if(false==TestWindowCreate(hOwner))
	{
		return -1;
	}
	
	return DialogBoxIndirectParam(&DlgInfo, hOwner, tryBoxProc, 0);
}



enum bgwin_id_e
{
	IDC_MAIN_BG_BTN_MENU=0x1700,
	IDC_MAIN_BG_TEXT_TIME,
	IDC_MAIN_BG_TEXT_TIME_VAL,
	IDC_MAIN_BG_TEXT_MACHINE,
	IDC_MAIN_BG_LIST_RECORD,
	IDC_MAIN_BG_TEXT_CAL,
	IDC_MAIN_BG_TEXT_RESULT,
	IDC_MAIN_BG_TEXT_RESULT_VAL,
	IDC_MAIN_BG_TEXT_NOCALI_VAL,

	IDC_MAIN_BG_BTN_UP,
	IDC_MAIN_BG_BTN_DOWN,
	IDC_MAIN_BG_BTN_MSWITCH,
	IDC_MAIN_BG_BTN_START,

	IDC_MAIN_BG_BTN_AD,
	IDC_MAIN_BG_BTN_AD_VAL,
};





void init_record_list_view(HWND hListView)
{
	int i;
	LVCOLUMN lvcol;	
	const char* caption[]=
	{
		osd_mainwin_jixing,		
		osd_mainwin_shuzhi,
		osd_mainwin_shijian,
	};
	
	for (i = 0; i < (int)ARRAY_SIZE(caption); i++) {
		lvcol.nCols = i;
		lvcol.pszHeadText = (char*)caption[i];
		lvcol.width = 60;
		lvcol.pfnCompare = NULL;
		lvcol.colFlags = 0;
		SendMessage (hListView, LVM_ADDCOLUMN, 0, (LPARAM) &lvcol);
	}
}



void insert_one_record(HWND hlist, PLVITEM lvItem, GHANDLE classent, record_t* prec, int cali)
{
	char buff[32];
	LVSUBITEM subdata;
	GHANDLE item = SendMessage (hlist, LVM_ADDITEM, classent, (LPARAM)lvItem);
	//int i = lvItem->nItem;
	int j;

	subdata.flags = 0;
	subdata.image = 0;
	subdata.nItem = lvItem->nItem;

	for (j = 0; j < 3; j ++) {
		subdata.subItem = j;
		subdata.nTextColor = 0;
		if(j==0)
		{
			subdata.pszText = (char*)prec->machine;
		}
		else if (j == 1)
		{
			if(cali)
			{
				sprintf (buff, "%1.2f(%dMM)", prec->val, cali);
			}
			else
			{
				sprintf (buff, "%1.2f", prec->val);
			}
			subdata.pszText = buff;			
		}
		else
		{
		
			char time[16]={0};
			char date[16]={0};
			sscanf(prec->time.c_str(), "%s %s", date, time);
			snprintf(buff, sizeof(buff), "%s %s", time, date);
			subdata.pszText =buff;
		}
		
		SendMessage (hlist, LVM_SETSUBITEM, item, (LPARAM) & subdata);
	}

}


void insert_new_record(HWND hWnd, float val, int iscali)
{	
	LVITEM item;
	string mname;

	HWND hListView=GetDlgItem(hWnd, IDC_MAIN_BG_LIST_RECORD);

	int cnt=SendMessage(hListView, LVM_GETITEMCOUNT, 0, 0);
	if(cnt>RECORD_SHOW_CNT)
	{
		SendMessage(hListView, LVM_DELITEM, cnt-1, 0);
	}
	

	MachineManager* mmgr=MachineManager::getInstance();
	mmgr->getMachineName(m_cur_machine_id, mname);

	SetWindowText(GetDlgItem(hWnd, IDC_MAIN_BG_TEXT_MACHINE), mname.c_str());

	record_t rt;
	time_t tt;
	struct tm tm;
	time(&tt);
	if(localtime_r(&tt, &tm))
	{
		char strtime[32];
		snprintf(strtime, sizeof(strtime), "%4d-%02d-%02d %02d:%02d:%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		rt.time=strtime;

		printf("-----time:%s\n", strtime);
	}
	
	
	rt.val=val;
	item.nItemHeight = 25;
	item.nItem = 0;
	rt.machine=mname.c_str();
	SendMessage (hListView, MSG_FREEZECTRL, TRUE, 0);	
	insert_one_record (hListView, &item, 0, &rt, iscali);
	SendMessage (hListView, MSG_FREEZECTRL, FALSE, 0);
}





static void update_measure_ctrl_color(HWND hDlg)
{
#define COLOR_FOR_CALI 0xEEEE00
#define COLOR_FOR_MEAS 0xffff

int ctrl_ids[]=
{
	IDC_MAIN_BG_TEXT_CAL,
	IDC_MAIN_BG_TEXT_RESULT,
	IDC_MAIN_BG_TEXT_NOCALI_VAL,
	IDC_MAIN_BG_BTN_MSWITCH,
	IDC_MAIN_BG_BTN_START,
};

	switch(m_measure_type)
	{
		//case MEASURE_TYPE_2MM:
		//case MEASURE_TYPE_4MM:
		case MEASURE_TYPE_LINE:
			for(int fori=0; fori<(int)ARRAY_SIZE(ctrl_ids); fori++)
			{
				HWND hWnd=GetDlgItem(hDlg, ctrl_ids[fori]);
				SetWindowBkColor(hWnd, COLOR_FOR_MEAS);
				UpdateWindow(hWnd, TRUE);
			}
		break;
		case MEASURE_TYPE_2MM_CALI:
		//case MEASURE_TYPE_4MM_CALI:
			for(int fori=0; fori<(int)ARRAY_SIZE(ctrl_ids); fori++)
			{
				HWND hWnd=GetDlgItem(hDlg, ctrl_ids[fori]);
				SetWindowBkColor(hWnd, COLOR_FOR_CALI);
				UpdateWindow(hWnd, TRUE);
			}
		break;
		case MEASURE_TYPE_NONE:
		case MEASURE_TYPE_MAX:
		break;
	}
}

static void update_measure_type_text(HWND hDlg)
{
	const char* text_str[]=
	{
		"",
		//osd_mainwin_2MMmeasuremode,
		//osd_mainwin_4MMmeasuremode,
		osd_mainwin_measuremode,
		osd_mainwin_calimode2,
		osd_mainwin_calimode4,
	};

	if(m_measure_type>=(int)ARRAY_SIZE(text_str))
	{
		JPRINTF(__FILE__, __LINE__, "Error type\n");
		return ;
	}

	SetDlgItemText(hDlg, IDC_MAIN_BG_TEXT_CAL, text_str[m_measure_type]);
}

void flush_record_list_view(HWND hWnd)
{
	int cnt;
	LVITEM item;
	record_t rt[RECORD_SHOW_CNT];	
	string mname;

	HWND hListView=GetDlgItem(hWnd, IDC_MAIN_BG_LIST_RECORD);

	SendMessage(hListView, LVM_DELALLITEM, 0, 0);
	
	MainRecord* mrd=MainRecord::getInstance();
	MachineManager* mmgr=MachineManager::getInstance();
	cnt=mrd->getMachineRecord(m_cur_machine_id, rt, RECORD_SHOW_CNT);
	mmgr->getMachineName(m_cur_machine_id, mname);

	SetWindowText(GetDlgItem(hWnd, IDC_MAIN_BG_TEXT_MACHINE), mname.c_str());
	
	item.nItemHeight = 25;	
	SendMessage (hListView, MSG_FREEZECTRL, TRUE, 0);
	for (int i = 0; i < cnt; i++) {
		item.nItem = i;	
		rt[i].machine=mname.c_str();
		insert_one_record (hListView, &item, 0, &rt[i], 0);
	}
	SendMessage (hListView, MSG_FREEZECTRL, FALSE, 0);
}



void show_ad(HWND hWnd, int adval)
{
	char buf[64];

	snprintf(buf, sizeof(buf), "%d", adval);	

	SetWindowText(hWnd, buf);
}



static void do_measure(HWND hWnd)
{
	int adval;
	if(!MainAdc::getInstance()->getAdVal(adval))
	{
		MessageBox(hWnd, osd_mainwin_getadcerr, osd_tishi, MB_OK);
		return;
	}

	float ad_L=adval*15.0f/4096;
	MainCali* mc=MainCali::getInstance();

	switch(m_measure_type)
	{
		case MEASURE_TYPE_NONE:
			return ;
		break;
		case MEASURE_TYPE_LINE:
		{
			if(!m_app_do_cali2)
			{
				int ret=MessageBox(hWnd, osd_mainwin_nocalialert, osd_tishi, MB_OKCANCEL);
				if(ret==IDOK)
				{
					
				}
				else
				{
					return;
				}
			}

			char val_str[32];
			int ret=GetDlgItemText(hWnd, IDC_MAIN_BG_TEXT_NOCALI_VAL, val_str, sizeof(val_str)-1);
			if(ret>0)
			{				
				sscanf(val_str, "%f", &ad_L);
			}
			
			MainRecord::getInstance()->addNewRecord(m_cur_machine_id, ad_L);
			insert_new_record(hWnd, ad_L, 0);
		}
		break;
		case MEASURE_TYPE_2MM_CALI:
		{
			bool bsave;
			int id;
			if(m_measure_type==MEASURE_TYPE_2MM_CALI)
			{
				id=BIAOZHUN_2MM_ID;
				m_app_do_cali2=true;
			}
			
			
			if(mc->checkExist(DEFAULT_CALI_MID, id))
			{
				bsave=mc->update(DEFAULT_CALI_MID, id, ad_L, 2.0, adval);
			}
			else
			{
				bsave=mc->add(DEFAULT_CALI_MID, adval, ad_L, 2.0, id);
			}

			if(bsave)
			{							
				MessageBox(hWnd, osd_mainwin_caliok, osd_tishi, MB_OK);				
			}
			else
			{
				MessageBox(hWnd, osd_mainwin_califail, osd_tishi, MB_OK);
			}

			//insert_new_record(hWnd, ad_L, mm);
		}
		break;
		case MEASURE_TYPE_MAX:
		break;
	}
}


#if 0
static void do_cali(HWND hWnd)
{
	int adval;
	if(!MainAdc::getInstance()->getAdVal(adval))
	{
		MessageBox(hWnd, osd_mainwin_getadcerr, osd_tishi, MB_OK);
		return;
	}

	float ad_L=adval*15.0f/4096;
	MainCali* mc=MainCali::getInstance();

	if(MEASURE_TYPE_2MM_CALI==m_measure_type)
	{
		bool bsave;
		int id;
		if(m_measure_type==MEASURE_TYPE_2MM_CALI)
		{
			id=BIAOZHUN_2MM_ID;
			m_app_do_cali2=true;
		}			
		
		if(mc->checkExist(DEFAULT_CALI_MID, id))
		{
			bsave=mc->update(DEFAULT_CALI_MID, id, ad_L, 2.0, adval);
		}
		else
		{
			bsave=mc->add(DEFAULT_CALI_MID, adval, ad_L, 2.0, id);
		}

		if(bsave)
		{							
			MessageBox(hWnd, osd_mainwin_caliok, osd_tishi, MB_OK);				
		}
		else
		{
			MessageBox(hWnd, osd_mainwin_califail, osd_tishi, MB_OK);
		}

		//insert_new_record(hWnd, ad_L, mm);
	}
}
#endif



void createMainChild(HWND hWnd)
{
	CTRLDATA CtrlInfo[]=
	{
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE ,
			10, 10, 80, 30,			
			IDC_MAIN_BG_BTN_MENU,
			osd_mainwin_caidan,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			100, 10, 80, 30,
			IDC_MAIN_BG_TEXT_TIME,
			osd_mainwin_dangqianshijian,
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			180, 10, 190, 30,
			IDC_MAIN_BG_TEXT_TIME_VAL,
			"",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			10, 50, 190, 30,
			IDC_MAIN_BG_TEXT_MACHINE,
			osd_mainwin_jiqimingcheng,
			0,
			WS_EX_NONE
		},

		{
			CTRL_LISTVIEW,			
			WS_CHILD | WS_VISIBLE /*| WS_VSCROLL | WS_HSCROLL */,
			10, 90, 190, 170,
			IDC_MAIN_BG_LIST_RECORD,
			"",
			0,
			WS_EX_NONE
		},

#if 0
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			210, 50, 100, 30,
			IDC_MAIN_BG_BTN_AD,
			osd_mainwin_shishiad,
			0,
			WS_EX_NONE
		},
#endif

#if 0
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			110, 50, 260, 30,
			IDC_MAIN_BG_BTN_AD_VAL,
			"",
			0,
			WS_EX_NONE
		},
#endif		
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			210, 90, 160, 30,
			IDC_MAIN_BG_TEXT_CAL,
			osd_mainwin_jiaozhunbiaoqian,
			0,
			WS_EX_NONE
		},

		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			210, 130, 160, 30,
			IDC_MAIN_BG_TEXT_RESULT,
			osd_mainwin_ceshishuzhi,
			0,
			WS_EX_NONE
		},
#if 1
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			210, 160, 160, 100,
			IDC_MAIN_BG_TEXT_NOCALI_VAL,
			"0",
			0,
			WS_EX_NONE
		},
#else
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			210, 165, 160, 50,
			IDC_MAIN_BG_TEXT_RESULT_VAL,
			"0",
			0,
			WS_EX_NONE
		},
		{
			CTRL_EDIT,
			WS_CHILD | WS_VISIBLE |ES_CENTER|WS_DISABLED|ES_READONLY,
			210, 220, 160, 50,
			IDC_MAIN_BG_TEXT_NOCALI_VAL,
			"0",
			0,
			WS_EX_NONE
		},
#endif
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE ,
			390, 10, 80, 40,
			IDC_MAIN_BG_BTN_UP,
			osd_mainwin_xiangshang,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE ,
			390, 60, 80, 40,
			IDC_MAIN_BG_BTN_DOWN,
			osd_mainwin_xiangxia,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE ,
			390, 110, 80, 40,
			IDC_MAIN_BG_BTN_MSWITCH,
			osd_mainwin_moshiqiehuan,
			0,
			WS_EX_NONE
		},
		{
			CTRL_BUTTON,
			WS_CHILD | WS_VISIBLE ,
			390, 160, 80, 80,
			IDC_MAIN_BG_BTN_START,
			osd_mainwin_celiang,
			0,
			WS_EX_NONE
		},
	};
	
	for(unsigned int fori=0; fori<ARRAY_SIZE(CtrlInfo); fori++)
	{
		CreateWindowEx(CtrlInfo[fori].class_name, CtrlInfo[fori].caption, CtrlInfo[fori].dwStyle, CtrlInfo[fori].dwExStyle, CtrlInfo[fori].id, CtrlInfo[fori].x, CtrlInfo[fori].y, CtrlInfo[fori].w, CtrlInfo[fori].h, hWnd, CtrlInfo[fori].dwAddData);
	}

}



int BgWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
		case MSG_CREATE:
			do
			{
				showEnterBox(hWnd, MainFrame::getWidth(), MainFrame::getHeight());

				MainKey::getInstance()->setCurWin(hWnd);

				SetWindowBkColor(hWnd, PIXEL_darkred);
				
				createMainChild(hWnd);
				
				init_record_list_view(GetDlgItem(hWnd, IDC_MAIN_BG_LIST_RECORD));

				update_measure_type_text(hWnd);	
				
				flush_record_list_view(hWnd);
				
				update_measure_ctrl_color(hWnd);
				
				//SetWindowFont(GetDlgItem(hWnd, IDC_MAIN_BG_TEXT_RESULT_VAL),(PLOGFONT)GetSystemFont(4));
				SetWindowFont(GetDlgItem(hWnd, IDC_MAIN_BG_TEXT_NOCALI_VAL),(PLOGFONT)GetSystemFont(4));

#if 0
				for(int fori=0; fori<6; fori++)
				{
					printf("fori:%d   size:%d	%p\n", fori, GetSysFontHeight(fori), GetSystemFont(fori));
				}
#endif			
				
				SetTimer(hWnd, MAIN_TIMER_MAIN_UPDATE, 50);
				//SetTimer(hWnd, MAIN_TIMER_CHECK, 500);
				SetTimer(hWnd, MAIN_TIMER_TIME_SHOW, 100);
			}while(0);
		break;
		case MSG_USER_KEY:
			do
			{
				int keyid=(int)wParam;
				int down=(int)lParam;

				//printf("key:%d  down:%d\n", keyid, down);

				if(down)
				{
					if(keyid==0)
					{
						MachineManager* mrd=MachineManager::getInstance();
						
						if(mrd->getPrevMachineId(m_cur_machine_id, &m_cur_machine_id))
						{
							flush_record_list_view(hWnd);

							if(mrd->checkLastExist())
							{
								mrd->updateLast(m_cur_machine_id);
							}
							else
							{
								mrd->addLast(m_cur_machine_id);
							}							
						}
					}
					else if(keyid==1)
					{
						MachineManager* mrd=MachineManager::getInstance();
												
						if(mrd->getNextMachineId(m_cur_machine_id, &m_cur_machine_id))
						{
							flush_record_list_view(hWnd);
							if(mrd->checkLastExist())
							{
								mrd->updateLast(m_cur_machine_id);
							}
							else
							{
								mrd->addLast(m_cur_machine_id);
							}							
						}						
					}
					else if(keyid==2)
					{
						m_measure_type++;
						if(m_measure_type>=MEASURE_TYPE_MAX)
						{
							m_measure_type=MEASURE_TYPE_NONE+1;
						}
						update_measure_type_text(hWnd);
					}
					else if(keyid==3)
					{
						do_measure(hWnd);
					}
				}
			}while(0);
		break;
		case MSG_TIMER:				
			do
			{
				int timer_id = (int)wParam;
				DWORD tick_count = (DWORD)lParam;

				if(timer_id==MAIN_TIMER_CHECK)
				{
					int time_min=tick_count*10/1000/60;
					if(time_min>10)
					{
						do
						{
							HWND hInput=HWND_NULL;
							while(1)
							{
								hInput=GetNextMainWindow(hInput);

								if((hInput!=HWND_NULL) && (hInput!=HWND_INVALID))
								{
									if(hWnd==hInput)
									{
										break;
									}
									
									DWORD param=GetWindowAdditionalData(hInput);
									if(param==(DWORD)try_key)
									{
										return 0;
									}

									param=GetWindowAdditionalData2(hInput);
									if(param==(DWORD)try_key)
									{
										return 0;
									}
									
									EndDialog(hInput, 0);
								}
								else
								{
									//DPRINTF("over\n");
									break;
								}
							}						
							
						}while(0);

						//showTryBox(hWnd, MainFrame::getWidth(), MainFrame::getHeight());

					}
				}
				else if(MAIN_TIMER_TIME_SHOW==timer_id)
				{
					struct tm tm_result;
					time_t tt;
					time(&tt);
					if(localtime_r(&tt, &tm_result))
					{
						char fmt_str[32];
						snprintf(fmt_str, sizeof(fmt_str), "%4d-%02d-%02d %02d:%02d:%02d", tm_result.tm_year+1900, tm_result.tm_mon+1, tm_result.tm_mday, tm_result.tm_hour, tm_result.tm_min, tm_result.tm_sec);
						SendDlgItemMessage(hWnd, IDC_MAIN_BG_TEXT_TIME_VAL, MSG_SETTEXT, (WPARAM)sizeof(fmt_str), (LPARAM)fmt_str);
					}

#if 0
					int adval;
					if(MainAdc::getInstance()->getAdVal(adval))
					{
						char adc_str[32];
						snprintf(adc_str, sizeof(adc_str), "%d", adval);
						SendDlgItemMessage(hWnd, IDC_MAIN_BG_BTN_AD_VAL, MSG_SETTEXT, (WPARAM)sizeof(adc_str), (LPARAM)adc_str);
					}
					else
					{
						SetWindowText(GetDlgItem(hWnd, IDC_MAIN_BG_BTN_AD_VAL), osd_mainwin_wushuju);
					}
#endif					
				}
				else if(MAIN_TIMER_MAIN_UPDATE==timer_id)
				{
					//KillTimer(hWnd, timer_id);

					int adval;
					if(!MainAdc::getInstance()->getAdVal(adval))
					{
						return 0;
					}
					//show_ad(GetDlgItem(hWnd, IDC_MAIN_BG_BTN_AD_VAL), adval);
					
					//disp_measure(GetDlgItem(hWnd, IDC_MAIN_BG_TEXT_RESULT_VAL), adval);

					disp_measure_nocali(GetDlgItem(hWnd, IDC_MAIN_BG_TEXT_NOCALI_VAL), adval);

				}
			}while(0);
			
		break;
		case MSG_SETFOCUS:
			do
			{
				//SetTimer(hWnd, MAIN_TIMER_CHECK, 500);
			}while(0);
		break;
		case MSG_KILLFOCUS:
			do
			{
				//SetTimer(hWnd, MAIN_TIMER_CHECK, 500);
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
					if(id==IDC_MAIN_BG_BTN_MENU)
					{
						RECT rcTarget;
						GetWindowRect((HWND)lParam, &rcTarget);

						int target_w=rcTarget.right-rcTarget.left;
						//int target_h=rcTarget.bottom-rcTarget.top;
						
						ClientToScreen(hWnd, &rcTarget.left, &rcTarget.top);

						int menu_ret=showMenuBox(hWnd, rcTarget.left+target_w, rcTarget.top, 100, 250);
						switch(menu_ret)
						{
							case IDC_MAIN_MENU_BTN_REG:
								showUsrMgrBox(hWnd, MainFrame::getWidth(), MainFrame::getHeight());
							break;
							case IDC_MAIN_MENU_BTN_MAC_MGR:
								showMacMgrBox(hWnd, MainFrame::getWidth(), MainFrame::getHeight());
							break;
							case IDC_MAIN_MENU_BTN_EXPORT:
								showExportBox(hWnd, MainFrame::getWidth(), MainFrame::getHeight());
							break;
							case IDC_MAIN_MENU_BTN_SETUP:
								showSetupBox(hWnd, MainFrame::getWidth(), MainFrame::getHeight());
							break;
							case IDC_MAIN_MENU_BTN_CALI:
								showCaliBox(hWnd, MainFrame::getWidth(), MainFrame::getHeight());
							break;
							case IDC_MAIN_MENU_BTN_INFO:
								showInfoBox(hWnd, MainFrame::getWidth(), MainFrame::getHeight());
							break;
						}
					}
					else if(id==IDC_MAIN_BG_BTN_UP)
					{
						MachineManager* mrd=MachineManager::getInstance();
												
						if(mrd->getPrevMachineId(m_cur_machine_id, &m_cur_machine_id))
						{
							flush_record_list_view(hWnd);
							if(mrd->checkLastExist())
							{
								mrd->updateLast(m_cur_machine_id);
							}
							else
							{
								mrd->addLast(m_cur_machine_id);
							}
						}
					}
					else if(id==IDC_MAIN_BG_BTN_DOWN)
					{
						MachineManager* mrd=MachineManager::getInstance();					
						
						if(mrd->getNextMachineId(m_cur_machine_id, &m_cur_machine_id))
						{
							flush_record_list_view(hWnd);
							if(mrd->checkLastExist())
							{
								mrd->updateLast(m_cur_machine_id);
							}
							else
							{
								mrd->addLast(m_cur_machine_id);
							}							
						}						
					}
					else if(id==IDC_MAIN_BG_BTN_MSWITCH)
					{
						m_measure_type++;
						if(m_measure_type>=MEASURE_TYPE_MAX)
						{
							m_measure_type=MEASURE_TYPE_NONE+1;
						}
						update_measure_type_text(hWnd);

						if((m_measure_type==MEASURE_TYPE_LINE) ||(m_measure_type==MEASURE_TYPE_2MM_CALI))
						{
							update_measure_ctrl_color(hWnd);
						}
					}
					else if(id==IDC_MAIN_BG_BTN_START)
					{
						do_measure(hWnd);
					}
				}
			}while(0);
		break;
		
	}

	return DefaultMainWinProc (hWnd, message, wParam, lParam);
}





MainFrame::MainFrame()
{
}


MainFrame::~MainFrame()
{
}

unsigned int MainFrame::m_w=320;
unsigned int MainFrame::m_h=240;

unsigned int MainFrame::getWidth()
{
	return m_w;
}

unsigned int MainFrame::getHeight()
{
	return m_h;
}





int MainFrame::create(unsigned int w, unsigned int h)
{
	m_w=w;
	m_h=h;

	Hsudev::getInstance();

	MachineManager* mm=MachineManager::getInstance();
	if(mm->checkLastExist())
	{
		mm->getLast(m_cur_machine_id);		
	}

	if(0==m_cur_machine_id)
	{
		if(mm->getNextMachineId(0, &m_cur_machine_id))
		{
			if(mm->checkLastExist())
			{
				mm->updateLast(m_cur_machine_id);
			}
			else
			{
				mm->addLast(m_cur_machine_id);
			}
		}
	}

	MSG Msg;
	HWND hMainWnd;
	MAINWINCREATE CreateInfo;

	//SetDefaultWindowElementRenderer("flat");

	CreateInfo.dwStyle = WS_VISIBLE /*| WS_BORDER | WS_CAPTION*/;
	CreateInfo.dwExStyle = WS_EX_NONE;
	CreateInfo.spCaption = "main win";
	CreateInfo.hMenu = 0;
	CreateInfo.hCursor = GetSystemCursor(0);
	CreateInfo.hIcon = 0;
	CreateInfo.MainWindowProc = BgWinProc;
	CreateInfo.lx = 0;
	CreateInfo.ty = 0;
	CreateInfo.rx = w;
	CreateInfo.by = h;
	CreateInfo.iBkColor = PIXEL_darkgreen;
	CreateInfo.dwAddData = 0;
	CreateInfo.hHosting = HWND_DESKTOP;

	hMainWnd = CreateMainWindow (&CreateInfo);

	if (hMainWnd == HWND_INVALID)
		return -1;

	ShowWindow(hMainWnd, SW_SHOWNORMAL);

	while (GetMessage(&Msg, hMainWnd)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	MainWindowThreadCleanup (hMainWnd);

	return 0;
}


