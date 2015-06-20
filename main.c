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
short curDirBlockp=0;

void initSys();
void tearDownSys();
int doCmd(char *cmd);
void showPwd();
int cd();
char * trim(char * str);
void format();//格式化
void ls(DirBlock *);
void rm(DirBlock * block,char * fileName);


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
	curPath[0]="root";
	curDirBlockp=0;
	curPathDep = 1;
	curDirBlock=getDirBlock(&myDisk,curPath,curPathDep,&curDirBlockp);
	showMyDisk(&myDisk);
}

void tearDownSys()
{
	int i=0;
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
		if(cd(str+3))
			curDirBlock=getDirBlock(&myDisk,curPath,curPathDep,&curDirBlockp);
	}
	else if(!strncmp(str,"mkdir ",6))
	{
		int b=0;
		char *c=str+6;
		b=mkDir(&myDisk,curDirBlock,curDirBlockp,c);
		if(!b)
			printf("创建目录失败");
	}
	else if(!strcmp(str,"ls"))
	{
		ls(curDirBlock);
	}
	else if(!strncmp(str,"rm ",3))
		rm(curDirBlock,str+3);
	else if(!strcmp(str,"format"))
		format();
	else 
		printf("command \'%s\' is not correct\n",str);
	return 1;
}

int cd(char * str)
{ 
	short p;
    char * ptr[FILE_MAXLEN]; 
	int i=curPathDep-1;
	if(!strcmp(str,".."))
	{
		if(curPathDep==1)
		{
			printf("you have already at root directory!\n");
			return 0;
		}
		--curPathDep;
		return 1;
	}

	p =getFileInodeID(curDirBlock,str);
	if(p)//路径存在
	{
		curPath[i+1]=(char *)malloc(strlen(str));
		strcpy(curPath[i+1],str);
		++i;
	}
	else
	{
		printf("目录\'%s\'不存在!\n",str);
		return 0;
	}
	curPathDep++;
	return 1;
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
	
	memset(&myDisk,0,sizeof(MyDisk));
	myDisk.blockUsedMap[0]=1;
	myDisk.superB.freeBlocks=BLOCK_NUM-1;
	myDisk.superB.freeInodes=INODE_NUM-1;
	//设置root目录的DirBlock
	myDisk.inodeTable[0].blockPoint[0]=0;
	myDisk.inodeTable[0].fileType=2;
	myDisk.inodeUsedMap[0]=1;
	writeDisk(&myDisk,sizeof(MyDisk),1,0);
	memset(&rootDirBlock,0,sizeof(DirBlock));
	writeDisk(&rootDirBlock,BLOCK_SIZE,1,sizeof(MyDisk));
	initSys();
}

void ls(DirBlock * block)
{
	showDirBlock(block);
}

void rm(DirBlock * block,char * fileName)
{
	short p =getFileInodeID(curDirBlock,fileName);
	if(p)//路径存在
	{
//		DirBlock *subBlock;//要删除的文件的block
		
//		subBlock=getDirBlock(&myDisk,curPath,curPathDep,&curDirBlockp);
		if(myDisk.inodeTable[p].fileType==2)//要删除的是文件夹 
		{
			cd(fileName);//进入这层目录
			curDirBlock=getDirBlock(&myDisk,curPath,curPathDep,&curDirBlockp);
			if(curDirBlock->inodeID[0])//文件夹不为空
			{
				printf("目录\'%s\'不为空,无法删除\n",fileName);
				cd("..");
				curDirBlock=getDirBlock(&myDisk,curPath,curPathDep,&curDirBlockp);
				return;
			}
			else
			{
				short pb=0,pi=0;
				int i=0,j=0;
				DirBlock *newDirBlock;
				cd("..");
				curDirBlock=getDirBlock(&myDisk,curPath,curPathDep,&curDirBlockp);
				pi=getFileInodeID(curDirBlock,fileName);
				pb=myDisk.inodeTable[pi].blockPoint[0];

				myDisk.blockUsedMap[pb]=0;
				myDisk.inodeUsedMap[pi]=0;
				myDisk.superB.freeBlocks++;
				myDisk.superB.freeInodes++;
				writeDisk(&myDisk,sizeof(MyDisk),1,0);

				//更新curDirBlock
				newDirBlock=(DirBlock *)malloc(sizeof(DirBlock));
				memset(newDirBlock,0,sizeof(DirBlock));
				for(;curDirBlock->inodeID[i];++i)
				{
					if(!strcmp(curDirBlock->fileName[i],fileName))
						continue;
					newDirBlock->inodeID[j]=curDirBlock->inodeID[i];
					strcpy(newDirBlock->fileName[j++],curDirBlock->fileName[i]);
				}
				free(curDirBlock);
				curDirBlock=newDirBlock;
				writeDisk(curDirBlock,BLOCK_SIZE,1,sizeof(MyDisk)+curDirBlockp*BLOCK_SIZE);
				
			}

			
		}
		else
		{
				printf("删除文件\'%s\'\n",fileName);
		}
		
	}
	else
	{
		printf("文件\'%s\'不存在!\n",fileName);
		return;
	}
}