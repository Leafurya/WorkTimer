#ifndef _data__h_
#define _data__h_

#define DATA_ATT	0
#define DATA_RE_S	1
#define DATA_RE_E	2
#define DATA_LEAV	3

typedef struct _hm{
	char h,m;	
}HM; 
typedef struct _data{
	HM timeLine[4];
	char minAgo;
	char notiMinAgo;
}Data;



void SaveData(Data *);
void LoadData(Data *);
void ModiData(Data *);
void ShowData(Data);

#endif
/*
�ʿ��� �����Ͱ� ����������
�ϴ�
���, �޽�, ��ٱ��ؽð�
��ٴ�� ���� 
*/

