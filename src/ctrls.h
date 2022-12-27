#ifndef _ctrls__h_
#define _ctrls__h_

#include <windows.h>
#include "data.h"

void CreateCtrlFont();

#define CTRL_MAINCTRLS			1
#define CTRL_MAIN_BT_SETTING	100
#define CTRL_MAIN_ST_TIMEDATA	101
#define CTRL_MAIN_BT_QUIT		102

typedef struct _mainctrls{
	HWND btSetting;
	HWND stTimeData;
	HWND btQuit;
}MainCtrls;

void CreateMainCtrls(void *,HWND,HINSTANCE);
void MovingMainCtrls(void *,RECT);

#define CTRL_SETTINGCTRLS		2

#define CTRL_SETTING_BT_MODI	209
#define CTRL_SETTING_BT_SAVE	210

#define CTRL_SETTING_ED_ATTH	200
#define CTRL_SETTING_ED_ATTM	201
#define CTRL_SETTING_ED_RSTH	202
#define CTRL_SETTING_ED_RSTM	203
#define CTRL_SETTING_ED_LEVH	205
#define CTRL_SETTING_ED_LEVM	206
#define CTRL_SETTING_ED_MINAGO	207
#define CTRL_SETTING_ED_RSTEH	204
#define CTRL_SETTING_ED_RSTEM	208

#define CTRL_SETTING_BT_BACK	212

#define CTRL_SETTING_CB_MINAGO	213

typedef struct _settingctrls{
	HWND stAtt;
	HWND stRst;
	HWND stLev;
	
	HWND edAttH;
	HWND edAttM;
	HWND edRstH;
	HWND edRstM;
	HWND edRstEH;
	HWND edRstEM;
	HWND edLevH;
	HWND edLevM;
	HWND edMinAgo;
	
	HWND btModi;
	HWND btSave;
	
	HWND cbMinAgo;
	
	HWND stKey;
	
	HWND stMinAgo;
	
	HWND stSpecial[5];
	
	HWND btBack;
}SettingCtrls;

void CreateSettingCtrls(void *,HWND,HINSTANCE);
void MovingSettingCtrls(void *,RECT);

void SettingCtrlsInit(SettingCtrls *,Data);



#endif
