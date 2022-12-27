#include "timer.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#define WIDTH_CENTER(rt,w) ((rt.right/2)-(w/2))
#define HEIGHT_CENTER(rt,h) ((rt.right/2)-(h/2))

char dow[7][4]={"sun","mon","tus","wed","thr","fri","sat"}; //day of the week
char *stateStr[4]={"출근","휴식","복귀","퇴근"};

void GetNowTime(Timer *timer){
	time_t t=time(NULL);
	timer->timeStruct=localtime(&t);
}
void PrintYMD(ThreadData data,TIMESTRUCT *t,char state){
	static char oldState=-2;
	HM timeline=data.tData->timeLine[state];
	char addZero[4][3]={0};
	char t_times[4]={t->tm_hour,t->tm_min,timeline.h,timeline.m};
	
	char str[64]={0};
	char ymd[16]={0};
	char curTime[32]={0};
	char explain[64]={0};
	
	char t_str[64]={0};
	
	int exM,exH;
	
	int i;
	
	
	exM=abs(timeline.m+abs(60-t->tm_min));
	exH=abs(timeline.h+abs(24-t->tm_hour))-1;
	if(exM>=60){
		exH++;
		exM-=60;
	}
	if(exH>=24){
		exH-=24;
	}
	for(i=0;i<4;i++){
		sprintf(addZero[i],t_times[i]<10?"0%d":"%d",t_times[i]);
	}
	
	if(!data.backgrounded){
		sprintf(ymd,"%d.%d.%d.(%s)\0",t->tm_year+1900,t->tm_mon+1,t->tm_mday,dow[t->tm_wday]);
		sprintf(curTime,"%s:%s / %s:%s\0",addZero[0],addZero[1],addZero[2],addZero[3]);
		sprintf(explain,"%s까지 %d시간 %d분 남았습니다.\0",stateStr[state],exH,exM);
		
		sprintf(str,"%s\n%s\n%s\0",ymd,curTime,explain);
		GetWindowText(data.staticWnd,t_str,sizeof(t_str));
		
		if(strcmp(t_str,str)){
			SetWindowText(data.staticWnd,str);
		}
	}
	if(oldState!=state&&data.tData->notiMinAgo){
		if(exM==data.tData->minAgo&&exH==0){
			printf("10min ago\n");
			sprintf(explain,"%s까지 %d분",stateStr[state],exM);
			SendMessage(data.mainWnd,WM_NOTIMSG,(WPARAM)explain,0);
			oldState=state;
		}
	}
}
unsigned __stdcall ThreadFunc(void *arg){
	ThreadData *threadData=(ThreadData *)arg;
	Data *data=threadData->tData;
	DWORD delay=1000;
	HWND mainWnd=threadData->mainWnd;
	char nowState=-1;
	char oldState=-2;
	time_t _t;
	TIMESTRUCT *t;
	int i;
	//printf("data addr(thread): %p\n",data);
	
	_t=time(NULL);
	t=localtime(&_t);
	
	if(t->tm_wday!=0&&t->tm_wday!=6){
		while(1){
			_t=time(NULL);
			t=localtime(&_t);
			
			for(i=0;i<=DATA_LEAV;i++){
				if(t->tm_hour>=data->timeLine[i].h&&t->tm_min>=data->timeLine[i].m){
					nowState++;
					continue;
				}
				if(t->tm_hour>data->timeLine[i].h){
					nowState++;
					continue;
				}
				break;
			}
			PrintTimerData(t);
			printf("nowstate: %d\n",nowState);
			if(oldState!=nowState){
				switch(nowState){
					case -1:
						if(threadData->waiting){
							SendMessage(mainWnd,WM_NOTIMSG,(WPARAM)"출근대기중",0);
							threadData->waiting=1;
						}
						break;
					case DATA_ATT:
						if(threadData->waiting){
							SendMessage(mainWnd,WM_NOTIMSG,(WPARAM)"출근!",0);
						}
						else{
							SendMessage(mainWnd,WM_NOTIMSG,(WPARAM)"늦었네...",0);
						}
						break;
					case DATA_RE_S:
						SendMessage(mainWnd,WM_NOTIMSG,(WPARAM)"휴식시간!",0);
						break;
					case DATA_RE_E:
						SendMessage(mainWnd,WM_NOTIMSG,(WPARAM)"일할시간~",0);
						break;
					case DATA_LEAV:
						SendMessage(mainWnd,WM_NOTIMSG,(WPARAM)"퇴근이다!!!",0);
						nowState=-1;
						threadData->onWork=0;
						return 0;
				}
				oldState=nowState;
			}
			PrintYMD(*threadData,t,nowState+1);
			nowState=-1;
			Sleep(delay);
		}
	}
	else{
		SendMessage(mainWnd,WM_NOTIMSG,(WPARAM)"오늘은 휴일입니다.",0);
	}
}
void PrintTimerData(TIMESTRUCT *ts){
	//printf("현재 년: %d\n", ts->tm_year + 1900);
	//printf("현재 월: %d\n", ts->tm_mon + 1);
	//printf("현재 일: %d\n", ts->tm_mday);
	printf("현재 시: %d\n", ts->tm_hour);
	printf("현재 분: %d\n", ts->tm_min);
	//printf("현재 초: %d\n", ts->tm_sec);
	printf("현재 요일: %d\n", ts->tm_wday); // 일=0, 월=1, 화=2, 수=3, 목=4, 금=5, 토=6
	//printf("올해 몇 번째 날: %d\n", ts->tm_yday); // 1월 1일은 0, 1월 2일은 1
	//printf("서머타임 적용 여부: %d\n", ts->tm_isdst);
}
