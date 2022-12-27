#ifndef _timer__h_
#define _timer__h_

#include <time.h>
#include <windows.h>
#include "data.h"

#define WM_NOTIMSG	WM_USER+3

typedef struct tm TIMESTRUCT;

typedef struct _timer{
	TIMESTRUCT* timeStruct;
}Timer;

typedef struct _threaddata{
	Data *tData;
	char waiting;
	HWND staticWnd;
	char backgrounded;
	char onWork;
	HWND mainWnd;
}ThreadData;

void GetNowTime(Timer *);
//void PrintYMD(TIMESTRUCT,HWND);
unsigned __stdcall ThreadFunc(void *);
void PrintTimerData(TIMESTRUCT *);

#endif
/*printf("현재 년: %d\n", timer.timeStruct->tm_year + 1900);
printf("현재 월: %d\n", timer.timeStruct->tm_mon + 1);
printf("현재 일: %d\n", timer.timeStruct->tm_mday);
printf("현재 시: %d\n", timer.timeStruct->tm_hour);
printf("현재 분: %d\n", timer.timeStruct->tm_min);
printf("현재 초: %d\n", timer.timeStruct->tm_sec);
printf("현재 요일: %d\n", timer.timeStruct->tm_wday); // 일=0, 월=1, 화=2, 수=3, 목=4, 금=5, 토=6
printf("올해 몇 번째 날: %d\n", timer.timeStruct->tm_yday); // 1월 1일은 0, 1월 2일은 1
printf("서머타임 적용 여부: %d\n", timer.timeStruct->tm_isdst);*/
