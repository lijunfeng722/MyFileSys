#include"MyDisk.h"
#include"DirBlock.h"

void showDirBlock(DirBlock * tBlock)
{
	int i=0;
	while(tBlock->inodeID[i])
	{
		printf("inodeID = %d , file = %s \n",tBlock->inodeID[i],tBlock->fileName[i]);
		++i;
	}
}

short getFileInodeID(DirBlock * tBlock,char * fileName)
{
	int i=0;
	while(tBlock->inodeID[i])
	{
		if(!strcmp(fileName,tBlock->fileName[i]))
			return tBlock->inodeID[i];
		++i;
	}
	return 0;
}

DirBlock * getDirBlock(char * dirName,int dep)
{
	DirBlock * tBlock;
	int i=0;
	printf("in getDirB  dirName=%s\n",dirName);
	tBlock=(DirBlock *)malloc(sizeof(DirBlock));
	readDisk(tBlock,sizeof(DirBlock),1,sizeof(MyDisk));

	

	showDirBlock(tBlock);
	return NULL;
}

