#include <windows.h>
#include <Doublebuffer.h> 
#include <stdio.h>
#include <time.h>
#include <ctrlmanager.h>
#include <shellapi.h>

#include "timer.h"
#include "data.h"
#include "thread.h"
#include "ctrls.h"
#include "resource.h"

#define TIMER_INPUT	0
#define WM_TRAY_ALERT 		WM_USER+1

LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);
void InitCtrls(HWND);
void WhenCreateWindow(HWND);

void MainCtrlsCommandFunc(WPARAM,LPARAM);
void SettingCtrlsCommandFunc(WPARAM,LPARAM);

HWND mainWnd;
RECT mainRect;
HINSTANCE g_hInst;
LPSTR mainWndClass="attendance";

Timer timer;
Data data;
HANDLE timerThread;
ThreadData thrdData;
HICON prgrmIcon;
//NOTIFYICONDATA trayData;


CtrlManager cManager;
MainCtrls mainCtrls;
SettingCtrls settCtrls;

extern HFONT font;
extern const int settingCtrlEditCount;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	prgrmIcon=LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON1));
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=prgrmIcon;
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)MainWndProc;
	WndClass.lpszClassName=mainWndClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	
	//AllocConsole();
	freopen("COIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	hWnd=CreateWindow(mainWndClass,mainWndClass,WS_OVERLAPPEDWINDOW,
		  CW_USEDEFAULT,CW_USEDEFAULT,500,300,
		  NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	printf("size:%d\n",sizeof(WPARAM));
	while(GetMessage(&Message,0,0,0)) {
		//InvalidateRect(mainWnd,NULL,FALSE);
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	FreeConsole();

	return Message.wParam;
}
	void Draw_MainWnd(HDC hdc,RECT rt){
		FillRect(hdc,&rt,(HBRUSH)(COLOR_WINDOW));
		SetBkMode(hdc,TRANSPARENT);
	}
LRESULT CALLBACK MainWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	char tl;
	NOTIFYICONDATA ballData;
	switch(iMessage) {
		case WM_CREATE:
			mainWnd=hWnd;
			WhenCreateWindow(hWnd);
			return 0;
		case WM_SIZE:
			if(wParam!=SIZE_MINIMIZED){
				GetClientRect(hWnd,&mainRect);
				CallMoveFunc(cManager,mainRect);
			}
			return 0;
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
				DoubleBuffer(hdc,mainRect,Draw_MainWnd);
			EndPaint(hWnd, &ps);
			return 0;
		case WM_TIMER:
			switch(wParam){
				case TIMER_INPUT:
					if(GetKeyState(VK_PAUSE)&0x8000){
						if(!thrdData.onWork){
							thrdData.onWork=1;
							timerThread=(HANDLE)StartThread(ThreadFunc,&thrdData);
						}
					}
					break;
			}
			return 0;
		case WM_COMMAND: 
			switch((int)(LOWORD(wParam)/100)){
				case CTRL_MAINCTRLS:
					MainCtrlsCommandFunc(wParam,lParam);
					break;
				case CTRL_SETTINGCTRLS:
					SettingCtrlsCommandFunc(wParam,lParam);
					break;
			}
			return 0;
		case WM_TRAY_ALERT:
			switch(LOWORD(lParam)){
				case WM_LBUTTONDOWN:
					ShowWindow(hWnd,SW_SHOW);
					thrdData.backgrounded=0;
					break;
			}
			return 0;
		case WM_NOTIMSG:
			memset(&ballData,0,sizeof(ballData));
			ballData.cbSize=sizeof(NOTIFYICONDATA);
			ballData.hWnd=hWnd;
			ballData.uID=100;
			ballData.uFlags=NIF_INFO;
			ballData.uCallbackMessage=WM_TRAY_ALERT;
			ballData.hIcon=LoadIcon(NULL,IDI_APPLICATION);
			ballData.uTimeout=1500;
			ballData.dwInfoFlags=NIIF_NONE;
			strcpy(ballData.szInfoTitle,"출근시계");
			strcpy(ballData.szInfo,(char *)wParam);
			Shell_NotifyIcon(NIM_MODIFY,&ballData);
			return 0;
		case WM_CLOSE:
			ShowWindow(hWnd,SW_HIDE);
			thrdData.backgrounded=1;
			return 0;
		case WM_DESTROY:
			KillTimer(hWnd,TIMER_INPUT);
			DestroyCtrlManager(&cManager);
			ballData.cbSize=sizeof(NOTIFYICONDATA);
			ballData.hWnd=hWnd;
			ballData.uID=100;
			ballData.uFlags=NIF_INFO;
			Shell_NotifyIcon(NIM_DELETE,&ballData);
			PostQuitMessage(0);
			return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
void InitCtrls(HWND hWnd){
	
	CreateMainCtrls(&mainCtrls,hWnd,g_hInst);
	CreateSettingCtrls(&settCtrls,hWnd,g_hInst);
	
	RegistCtrlGroup(&cManager,&mainCtrls,CTRL_MAINCTRLS,sizeof(MainCtrls),MovingMainCtrls);
	RegistCtrlGroup(&cManager,&settCtrls,CTRL_SETTINGCTRLS,sizeof(SettingCtrls),MovingSettingCtrls);
	
	SettingCtrlsInit(&settCtrls,data);
	
	SetNowCtrlGroup(&cManager,CTRL_MAINCTRLS);
}
void WhenCreateWindow(HWND hWnd){
	NOTIFYICONDATA trayData;
	
	LoadData(&data);
	ShowData(data);
	
	CreateCtrlFont();
	SendMessage(hWnd,WM_SIZE,0,0);
	SetTimer(hWnd,TIMER_INPUT,10,0);
	InitCtrlManager(&cManager,&mainRect);
	InitCtrls(hWnd);
	SetWindowText(mainCtrls.stTimeData,"\n\n출근 대기열에 들어가십시오.");
	
	thrdData.tData=&data;
	thrdData.staticWnd=mainCtrls.stTimeData;
	thrdData.backgrounded=0;
	thrdData.onWork=0;
	thrdData.waiting=0;
	thrdData.mainWnd=hWnd;
	
	memset(&trayData,0,sizeof(trayData));
	trayData.cbSize=sizeof(NOTIFYICONDATA);
	trayData.hWnd=hWnd;
	trayData.uID=100;
	trayData.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
	trayData.uCallbackMessage=WM_TRAY_ALERT;
	trayData.hIcon=prgrmIcon;
	strcpy(trayData.szTip,"출근 시계");
	Shell_NotifyIcon(NIM_ADD,&trayData);
	
	SendMessage(hWnd,WM_SETICON,ICON_BIG,(LPARAM)prgrmIcon);
	SendMessage(hWnd,WM_SETICON,ICON_SMALL,(LPARAM)prgrmIcon);
}
void MainCtrlsCommandFunc(WPARAM wParam,LPARAM lParam){
	switch(LOWORD(wParam)){
		case CTRL_MAIN_BT_SETTING:
			SettingCtrlsInit(&settCtrls,data);
			ChangeCtrlGroup(&cManager,CTRL_SETTINGCTRLS);
			break;
		case CTRL_MAIN_BT_QUIT:
			DestroyWindow(mainWnd);
			break;
	}
}
void SettingCtrlsCommandFunc(WPARAM wParam,LPARAM lParam){
	HWND *edit;
	char str[3]={0};
	int i,l;
	char *pData;
	Data tData;
	Data *pHM1,*pHM2;
	static char modifing;
	switch(LOWORD(wParam)){
		case CTRL_SETTING_BT_MODI:
			edit=&settCtrls.edAttH;
			for(i=0;i<settingCtrlEditCount;i++,edit++){
				SendMessage(*edit,EM_SETREADONLY,FALSE,0);
			}
			EnableWindow(settCtrls.cbMinAgo,TRUE);
			EnableWindow(settCtrls.btModi,FALSE);
			EnableWindow(settCtrls.btSave,TRUE);
			modifing=1;
			break;
		case CTRL_SETTING_BT_SAVE:
			edit=&settCtrls.edAttH;
			pData=(char *)&tData;
			for(i=0;i<settingCtrlEditCount-1;i++,edit++,pData++){
				GetWindowText(*edit,str,sizeof(str));
				if(strlen(str)<=0){
					MessageBox(mainWnd,"입력값이 없습니다.","알림",MB_OK);
				}
				for(l=0;l<strlen(str);l++){
					if(!isdigit(str[l])){
						MessageBox(mainWnd,"숫자입력이 아닙니다.","알림",MB_OK);
						return;
					}
					if(i&1){
						if(atoi(str)>=60){
							MessageBox(mainWnd,"60이상의 숫자를 입력받을 수 없습니다.","알림",MB_OK);
							return;
						}
					}
					else{
						if(atoi(str)>=24){
							MessageBox(mainWnd,"24이상의 숫자를 입력받을 수 없습니다.","알림",MB_OK);
							return;
						}
					}
				}
				*pData=atoi(str);
				//SendMessage(*edit,EM_SETREADONLY,TRUE,0);
			}
			GetWindowText(*edit,str,sizeof(str));
			if(atoi(str)>=60){
				MessageBox(mainWnd,"60이상의 숫자를 입력받을 수 없습니다.","알림",MB_OK);
				return;
			}
			*pData=atoi(str);
			/*l=tData.timeLine[DATA_ATT].h*100+tData.timeLine[DATA_ATT].m;
			i=tData.timeLine[DATA_RE_S].h*100+tData.timeLine[DATA_RE_S].m;
			if(l>=i){
				MessageBox(mainWnd,"시간을 다시 확인해 주십시오.","알림",MB_OK);
				return;
			}*/
			for(i=0;i<4;i++){
				for(l=i+1;l<4;l++){
					if((tData.timeLine[i].h*100+tData.timeLine[i].m)>=(tData.timeLine[l].h*100+tData.timeLine[l].m)){
						MessageBox(mainWnd,"시간을 다시 확인해 주십시오.","알림",MB_OK);
						return;
					}
				}
			}
			
			for(i=0;i<settingCtrlEditCount;i++,edit++){
				SendMessage(*edit,EM_SETREADONLY,TRUE,0);
			}
			tData.notiMinAgo=SendMessage(settCtrls.cbMinAgo,BM_GETCHECK,0,0);
			data=tData;
			ShowData(data);
			SaveData(&data);
			EnableWindow(settCtrls.cbMinAgo,FALSE);
			EnableWindow(settCtrls.btModi,TRUE);
			EnableWindow(settCtrls.btSave,FALSE);
			modifing=0;
			break;
		
		case CTRL_SETTING_BT_BACK:
			if(modifing){
				if(MessageBox(mainWnd,"저장하지 않은 데이터는 사라집니다.\n그래도 나가시겠습니까?","경고",MB_YESNO)==IDYES){
					modifing=0;
					ChangeCtrlGroup(&cManager,CTRL_MAINCTRLS);
					break;
				}
			}
			else{
				ChangeCtrlGroup(&cManager,CTRL_MAINCTRLS);
				
			}
			break;
		case CTRL_SETTING_CB_MINAGO:
			SendMessage(settCtrls.cbMinAgo,BM_SETCHECK,!SendMessage(settCtrls.cbMinAgo,BM_GETCHECK,0,0),0);
			break;
	}
}
 
