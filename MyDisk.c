#include<stdio.h>
#include"MyDisk.h"
void showMyDisk(MyDisk *disk)
{
	int i=0;
	printf("freeInodes=%d\nfreeBlocks=%d\n",disk->superB.freeInodes,disk->superB.freeBlocks);
	printf("blockUsedMap :\n");
	while(i<BLOCK_NUM)
	{
		printf("%d ",disk->blockUsedMap[i]);
		if(!((i+1)%32))
			printf("\n");
		++i;
	}
	printf("\nLast Time Your Visited: %d-%d-%d %02d:%02d\n",
		disk->gm_date.tm_year+1900,
		disk->gm_date.tm_mon+1, 
		disk->gm_date.tm_mday, disk->gm_date.tm_hour+8,
		disk->gm_date.tm_min);
}

void setDate(MyDisk *disk)
{
	struct tm *t;
	time_t seconds;
	time(&seconds);
	t=gmtime(&seconds);
	disk->gm_date.tm_mon=t->tm_mon;
	disk->gm_date.tm_mday=t->tm_mday;
	disk->gm_date.tm_year=t->tm_year;
	disk->gm_date.tm_hour=t->tm_hour;
	disk->gm_date.tm_min=t->tm_min;
}


void readDisk(void * obj,int len,int num,int startPos)
{
	FILE *diskFile;
	if((diskFile=fopen(DISK_NAME,"rb+"))==NULL)
	{
		printf("fail to access your disk!\n");
		system("pause");
		exit(0);
	}
	else
	{
//		printf("read disk!\n");
		fseek(diskFile,startPos,SEEK_SET);//��λ�ļ�ָ�뵽�ļ���ʼλ��
		fread(obj,len*num,num,diskFile);//���ļ����ݶ��뵽����
		fclose(diskFile);
	}

}

void writeDisk(void * obj,int len,int num,int startPos)
{
	FILE *diskFile;
	if((diskFile=fopen(DISK_NAME,"rb+"))==NULL)
	{
		printf("fail to access your disk!\n");
		system("pause");
		exit(0);
	}
	else
	{
//		printf("write disk!\n");
		fseek(diskFile,startPos,SEEK_SET);//��λ�ļ�ָ�뵽�ļ���ʼλ��
		fwrite(obj,len*num,num,diskFile);//�ѻ������ݶ��뵽�ļ�
		fflush(diskFile);
		fclose(diskFile);
	}
}