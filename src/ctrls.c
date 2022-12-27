#include "ctrls.h"
#include <stdio.h>

#define WCENTER(rt,w) ((rt.right/2)-(w/2))
#define HCENTER(rt,h) ((rt.bottom/2)-(h/2))

int fontSize=20;
const char *fontName="한컴산뜻돋움";
HFONT font;
const int settingCtrlEditCount=CTRL_SETTING_ED_RSTEM-CTRL_SETTING_ED_ATTH+1;

void CreateCtrlFont() {
	font=CreateFont(fontSize,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,VARIABLE_PITCH|FF_ROMAN,fontName);
}

void SetFontEach(HWND c,HFONT font) {
	SendMessage(c,WM_SETFONT,(WPARAM)font,MAKELPARAM(TRUE,0));
}
void SetFontAll(HWND *c,int size,HFONT font) {
	int i;
	int count=size/sizeof(HWND);
	for(i=0; i<count; i++) {
		SendMessage(c[i],WM_SETFONT,(WPARAM)font,MAKELPARAM(TRUE,0));
	}
}

void CreateMainCtrls(void *ctrls,HWND hWnd,HINSTANCE hInst) {
	MainCtrls *c=(MainCtrls *)ctrls;

	c->btSetting=CreateWindow("button","설  정",WS_CHILD|WS_BORDER|BS_PUSHBUTTON|WS_VISIBLE,0,0,0,0,hWnd,(HMENU)CTRL_MAIN_BT_SETTING,hInst,NULL);
	c->stTimeData=CreateWindow("static",NULL,WS_CHILD|SS_CENTER|WS_VISIBLE,0,0,0,0,hWnd,(HMENU)CTRL_MAIN_ST_TIMEDATA,hInst,NULL);
	c->btQuit=CreateWindow("button","프로그램 종료",WS_CHILD|WS_BORDER|BS_PUSHBUTTON|WS_VISIBLE,0,0,0,0,hWnd,(HMENU)CTRL_MAIN_BT_QUIT,hInst,NULL);

	SetFontAll((HWND *)c,sizeof(MainCtrls),font);
}
void MovingMainCtrls(void *ctrls,RECT rect) {
	MainCtrls *c=(MainCtrls *)ctrls;
	int btW=110,btH=50;
	int stH=70,stW=250;

	MoveWindow(c->btSetting,(int)((rect.right/2)-(btW))-10,(int)((rect.bottom/2)-(btH/2))+(btH),btW,btH,TRUE);
	MoveWindow(c->stTimeData,(int)((rect.right/2)-(stW/2)),(int)((rect.bottom/2)-(stH/2))-50,stW,stH,TRUE);
	MoveWindow(c->btQuit,(int)((rect.right/2))+10,(int)((rect.bottom/2)-(btH/2))+(btH),btW,btH,TRUE);
}

void CreateSettingCtrls(void *ctrls,HWND hWnd,HINSTANCE hInst) {
	SettingCtrls *c=(SettingCtrls *)ctrls;

	c->stAtt=CreateWindow("static","출근",WS_CHILD|SS_CENTER,0,0,0,0,hWnd,(HMENU)-1,hInst,NULL);
	c->stRst=CreateWindow("static","휴식",WS_CHILD|SS_CENTER,0,0,0,0,hWnd,(HMENU)-1,hInst,NULL);
	c->stLev=CreateWindow("static","퇴근",WS_CHILD|SS_CENTER,0,0,0,0,hWnd,(HMENU)-1,hInst,NULL);

	c->edAttH=CreateWindow("edit",NULL,WS_CHILD|ES_CENTER|ES_READONLY|WS_BORDER,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_ED_ATTH,hInst,NULL);
	c->edAttM=CreateWindow("edit",NULL,WS_CHILD|ES_CENTER|ES_READONLY|WS_BORDER,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_ED_ATTM,hInst,NULL);
	c->edRstH=CreateWindow("edit",NULL,WS_CHILD|ES_CENTER|ES_READONLY|WS_BORDER,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_ED_RSTH,hInst,NULL);
	c->edRstM=CreateWindow("edit",NULL,WS_CHILD|ES_CENTER|ES_READONLY|WS_BORDER,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_ED_RSTM,hInst,NULL);
	c->edRstEH=CreateWindow("edit",NULL,WS_CHILD|ES_CENTER|ES_READONLY|WS_BORDER,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_ED_RSTEH,hInst,NULL);
	c->edRstEM=CreateWindow("edit",NULL,WS_CHILD|ES_CENTER|ES_READONLY|WS_BORDER,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_ED_RSTEM,hInst,NULL);
	c->edLevH=CreateWindow("edit",NULL,WS_CHILD|ES_CENTER|ES_READONLY|WS_BORDER,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_ED_LEVH,hInst,NULL);
	c->edLevM=CreateWindow("edit",NULL,WS_CHILD|ES_CENTER|ES_READONLY|WS_BORDER,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_ED_LEVM,hInst,NULL);

	c->edMinAgo=CreateWindow("edit",NULL,WS_CHILD|ES_CENTER|ES_READONLY|WS_BORDER,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_ED_MINAGO,hInst,NULL);
	c->stMinAgo=CreateWindow("static","분 전 알림",WS_CHILD|SS_CENTER,0,0,0,0,hWnd,(HMENU)-1,hInst,NULL);
	c->cbMinAgo=CreateWindow("button","(알림 받기)",WS_CHILD|BS_PUSHBUTTON|BS_CHECKBOX,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_CB_MINAGO,hInst,NULL);

	c->btModi=CreateWindow("button","수정",WS_CHILD|WS_BORDER|BS_PUSHBUTTON,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_BT_MODI,hInst,NULL);
	c->btSave=CreateWindow("button","저장",WS_CHILD|WS_BORDER|BS_PUSHBUTTON,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_BT_SAVE,hInst,NULL);


	c->stSpecial[0]=CreateWindow("static",":",WS_CHILD|SS_CENTER,0,0,0,0,hWnd,(HMENU)-1,hInst,NULL);
	c->stSpecial[1]=CreateWindow("static",":",WS_CHILD|SS_CENTER,0,0,0,0,hWnd,(HMENU)-1,hInst,NULL);
	c->stSpecial[2]=CreateWindow("static",":",WS_CHILD|SS_CENTER,0,0,0,0,hWnd,(HMENU)-1,hInst,NULL);
	c->stSpecial[3]=CreateWindow("static","~",WS_CHILD|SS_CENTER,0,0,0,0,hWnd,(HMENU)-1,hInst,NULL);
	c->stSpecial[4]=CreateWindow("static",":",WS_CHILD|SS_CENTER,0,0,0,0,hWnd,(HMENU)-1,hInst,NULL);
	
	c->btBack=CreateWindow("button","뒤로가기",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE|WS_BORDER,0,0,0,0,hWnd,(HMENU)CTRL_SETTING_BT_BACK,hInst,NULL);
	c->stKey=CreateWindow("static","출근대기 키: PUASE",WS_CHILD|SS_CENTER,0,0,0,0,hWnd,(HMENU)-1,hInst,NULL);
	
	SetFontAll((HWND *)c,sizeof(SettingCtrls),font);
}
void MovingSettingCtrls(void *ctrls,RECT rect) {
	SettingCtrls *c=(SettingCtrls *)ctrls;
	//int stKeyW=stW*2+10;
	int edW=30,edH=20;
	int btW=150,btH=30;
	int stSpW=10,stSpH=20;
	int stW=40,stH=26;
	int cbW=100,cbH=20;
	
	int timeBoxW=(edW*8)+(stSpW*5)+(40*2);
	int timeBoxH=stH+edH+10;
	int timeBoxOrigiX=WCENTER(rect,timeBoxW);
	int timeBoxOrigiY=(rect.bottom/2)-timeBoxH-30;
	int curY;
	int curX;
	int gap_10=10;

	int stMinAgoW=stW*2-10;
	int minAgoBoxW=stMinAgoW+edW+cbW+5;
	int minAgoBoxH=stH+20;
	int minAgoBoxOrigiX=WCENTER(rect,minAgoBoxW);
	int minAgoBoxOrigiY=timeBoxOrigiY+timeBoxH+20;
	
	MoveWindow(c->btBack,0,0,80,30,TRUE);
	MoveWindow(c->stKey,0,rect.bottom-stH,stW*4,stH,TRUE);
	
	curX=timeBoxOrigiX; curY=timeBoxOrigiY+(gap_10/2)+stH;
	MoveWindow(c->edAttH,curX,curY,edW,edH,TRUE);
	curX+=edW; //curY+=(gap_10+stH);
	MoveWindow(c->stAtt,curX+(stSpW/2)-(stW/2),timeBoxOrigiY,stW,stH,TRUE);
	MoveWindow(c->stSpecial[0],curX,curY,stSpW,stSpH,TRUE);
	curX+=stSpW;
	MoveWindow(c->edAttM,curX,curY,edW,edH,TRUE);
	curX+=(edW+40);
	MoveWindow(c->edRstH,curX,curY,edW,edH,TRUE);
	curX+=edW;
	MoveWindow(c->stSpecial[1],curX,curY,stSpW,stSpH,TRUE);
	curX+=stSpW;
	MoveWindow(c->edRstM,curX,curY,edW,edH,TRUE);
	curX+=edW;
	MoveWindow(c->stRst,curX+(stSpW/2)-(stW/2),timeBoxOrigiY,stW,stH,TRUE);
	MoveWindow(c->stSpecial[3],curX,curY,stSpW,stSpH,TRUE);
	curX+=stSpW;
	MoveWindow(c->edRstEH,curX,curY,edW,edH,TRUE);
	curX+=edW;
	MoveWindow(c->stSpecial[4],curX,curY,stSpW,stSpH,TRUE);
	curX+=stSpW;
	MoveWindow(c->edRstEM,curX,curY,edW,edH,TRUE);
	curX+=(edW+40);
	MoveWindow(c->edLevH,curX,curY,edW,edH,TRUE);
	curX+=edW;
	MoveWindow(c->stLev,curX+(stSpW/2)-(stW/2),timeBoxOrigiY,stW,stH,TRUE);
	MoveWindow(c->stSpecial[2],curX,curY,stSpW,stSpH,TRUE);
	curX+=stSpW;
	MoveWindow(c->edLevM,curX,curY,edW,edH,TRUE);
	
	curX=minAgoBoxOrigiX; curY=minAgoBoxOrigiY;
	MoveWindow(c->edMinAgo,curX,curY,edW,edH,TRUE);
	curX+=edW;
	MoveWindow(c->stMinAgo,curX,curY,stMinAgoW,edH,TRUE);
	curX+=stMinAgoW+5;
	MoveWindow(c->cbMinAgo,curX,curY,cbW,cbH,TRUE);
	
	
	curY+=minAgoBoxH+10;
	MoveWindow(c->btModi,(rect.right/2)-(btW)-20,curY,btW,btH,TRUE);
	MoveWindow(c->btSave,(rect.right/2)+20,curY,btW,btH,TRUE);
	
}
void SettingCtrlsInit(SettingCtrls *settCtrls,Data data) {
	char str[3]= {0};
	HWND *edit=&settCtrls->edAttH;
	char *pData=(char *)&(data.timeLine[0]);
	int i;
	
	for(i=0;i<settingCtrlEditCount;i++,edit++,pData++){
		SendMessage(*edit,EM_SETREADONLY,TRUE,0);
		sprintf(str,"%d",*pData);
		SetWindowText(*edit,str);
	}
	SendMessage(settCtrls->cbMinAgo,BM_SETCHECK,data.notiMinAgo,0);
	EnableWindow(settCtrls->cbMinAgo,FALSE);

	EnableWindow(settCtrls->btSave,FALSE);
	EnableWindow(settCtrls->btModi,TRUE);
}
