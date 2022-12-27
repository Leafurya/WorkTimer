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
/*printf("���� ��: %d\n", timer.timeStruct->tm_year + 1900);
printf("���� ��: %d\n", timer.timeStruct->tm_mon + 1);
printf("���� ��: %d\n", timer.timeStruct->tm_mday);
printf("���� ��: %d\n", timer.timeStruct->tm_hour);
printf("���� ��: %d\n", timer.timeStruct->tm_min);
printf("���� ��: %d\n", timer.timeStruct->tm_sec);
printf("���� ����: %d\n", timer.timeStruct->tm_wday); // ��=0, ��=1, ȭ=2, ��=3, ��=4, ��=5, ��=6
printf("���� �� ��° ��: %d\n", timer.timeStruct->tm_yday); // 1�� 1���� 0, 1�� 2���� 1
printf("����Ÿ�� ���� ����: %d\n", timer.timeStruct->tm_isdst);*/
