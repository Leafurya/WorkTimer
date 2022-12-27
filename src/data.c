#include "data.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

char basicDataPath[1024]="data/basic.data";

void MakeData(Data *data){
	data->notiMinAgo=1;
	data->minAgo=10;
	data->timeLine[DATA_ATT].h=8;data->timeLine[DATA_ATT].m=30;
	data->timeLine[DATA_RE_S].h=11;data->timeLine[DATA_RE_S].m=0;
	data->timeLine[DATA_RE_E].h=13;data->timeLine[DATA_RE_E].m=0;
	data->timeLine[DATA_LEAV].h=17;data->timeLine[DATA_LEAV].m=30;
}
void LoadData(Data *data){
	FILE *file;
	Data td;
	char *errstr;
	file=fopen(basicDataPath,"rb");
	
	if(file==NULL){
		printf("error:%s\n",strerror(errno));
		MakeData(&td);
		SaveData(&td);
		//return;
	}
	fread(&td,sizeof(Data),1,file);
	*data=td;
	
	fclose(file);
}
void SaveData(Data *data){
	FILE *file;
	file=fopen(basicDataPath,"wb");
	
	fwrite(data,sizeof(Data),1,file);
	
	fclose(file);
}
void ShowData(Data data){
	int i;
	
	printf("minago:%d\n",data.minAgo);
	for(i=0;i<4;i++){
		printf("%d:%d\n",data.timeLine[i].h,data.timeLine[i].m);
	}
}
