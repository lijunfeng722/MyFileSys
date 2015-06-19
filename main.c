#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"SuperBlock.h"
#include"MyDisk.h"
#define CMD_MAXLEN 50
#define FILE_MAXLEN 20
MyDisk myDisk;
char * curPath[FILE_MAXLEN];//当前路径
int curPathDep = 1;
DirBlock *curDirBlock;


void initSys();
void tearDownSys();
int doCmd(char *cmd);
void showPwd();
void cd();
char * trim(char * str);
void format();//格式化


int main()
{
	char cmd[CMD_MAXLEN];
	initSys();		
	do
	{		
		showPwd();	
		printf(">");
		gets(cmd);
	}while(doCmd(cmd));
	

	tearDownSys();
	return 0;
}

void initSys()
{
	readDisk(&myDisk,sizeof(MyDisk),1,0);
//	format();
	curPath[0]="root";
	curDirBlock=getDirBlock(&myDisk,curPath,curPathDep);
//	mkDir(&myDisk,curDirBlock,0,"newDir@root");
	showMyDisk(&myDisk);
	
}

void tearDownSys()
{
	setDate(&myDisk);
	writeDisk(&myDisk,sizeof(MyDisk),1,0);
}

void showPwd()
{
	int i;
	for(i=0;i<curPathDep;++i)
	{
		if(curPath[i])
			printf("%s",curPath[i]);
		printf("/");
	}
}

int doCmd(char *cmd)
{
	char * str;
	str=trim(cmd);
	if(!strcmp(str,"exit"))
		return 0;
	else if(!strcmp(str,"pwd"))
	{
		showPwd();
		printf("\n");
	}
	else if(!strncmp(str,"cd ",3))
	{
		cd(str);
		curDirBlock=getDirBlock(curPath,curPathDep);
	}
	else if(!strncmp(str,"mkdir ",3))
	{
		;
	}
	else 
		printf("command \'%s\' is not correct\n",str);
	return 1;
}

void cd(char * str)
{ 
    char * ptr[FILE_MAXLEN]; 
	int i=0;
	str+=3;
//    printf("before strtok:  str=%s\n",str);  
//    printf("begin:\n");  
	if(!strcmp(str,".."))
	{
		if(curPathDep==1)
		{
			printf("you have already at root directory!\n");
			return;
		}
		--curPathDep;
		return;
	}
    *ptr = strtok(str, "/");  

    while(*(ptr+i) != NULL)
	{  
		if(1)//路径存在
		{
			curPath[i+1]=(char *)malloc(strlen(*(ptr+(i))));
			strcpy(curPath[i+1],*(ptr+(i)));
			++i;
		}
        *(ptr+i) = strtok(NULL, "/");  
	
    }  
	curPathDep=i+1;
}

char * trim(char * src)
{
     int i = 0;
     char *begin = src;
     while(src[i] != '\0')
	 {
        if(src[i] != ' ')
          break;
		else
			begin++;
		i++;
     }
     for(i = strlen(src)-1; i >= 0;  i--)
	 {
		 if(src[i] != ' ')
			 break;
         else
    		 src[i] = '\0';        
     }
     return begin;
}

void format()//格式化
{
	DirBlock rootDirBlock;
	FILE *f = fopen(DISK_NAME,"w");
	fseek(f,SEEK_CUR,sizeof(MyDisk)+BLOCK_NUM*BLOCK_SIZE-1);//清空数据
	fputc(0,f);
	fclose(f);
	
	myDisk.blockUsedMap[0]=1;
	myDisk.superB.freeBlocks=BLOCK_NUM-1;
	myDisk.superB.freeInodes=INODE_NUM-1;
	//设置root目录的DirBlock
	myDisk.inodeTable[0].blockPoint[0]=0;
	myDisk.inodeTable[0].fileType=2;
	myDisk.inodeUsedMap[0]=1;

	memset(&rootDirBlock,0,sizeof(DirBlock));
	writeDisk(&rootDirBlock,BLOCK_SIZE,1,sizeof(MyDisk));
}

