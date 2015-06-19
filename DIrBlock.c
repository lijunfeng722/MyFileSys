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

DirBlock * getDirBlock(MyDisk *disk,char ** dirPath,int dep)
{
	DirBlock * tBlock;
	int i=1;
	printf("in getDirB  *dirPath=%s\n",*dirPath);
	tBlock=(DirBlock *)malloc(sizeof(DirBlock));
	readDisk(tBlock,sizeof(DirBlock),1,sizeof(MyDisk));//读出root的DirBlock
	while(i<dep)
	{
		short pi=0,pb;
		pi=getFileInodeID(tBlock,*(dirPath+(i-1)));
		++i;
		pb=disk->inodeTable[pi].blockPoint;
		readDisk(tBlock,sizeof(DirBlock),1,sizeof(MyDisk)+pb*BLOCK_SIZE);
	}
	

	showDirBlock(tBlock);
	return tBlock;
}

short getAvailableInode(MyDisk * disk)
{
	short i=1;//i=0为root的inode,所以从i=1开始
	for(;i<INODE_NUM;++i)
		if(!(disk->inodeUsedMap[i]))
			return i;
	return 0;
}
short getAvailabeBlock(MyDisk * disk)
{
	short i=1;//i=0为root的inode,所以从i=1开始
	for(;i<INODE_NUM;++i)
		if(!(disk->blockUsedMap[i]))
			return i;
	return 0;
}

int mkDir(MyDisk * disk,DirBlock * curBlock,short curBlockp,char * dirName)
{
	short pi,pb,i=0;
	DirBlock *newDirBlock;
	pi=getAvailableInode(disk);
	pb=getAvailabeBlock(disk);

	if(!pi||!pb)//创建目录失败
		return 0;


	//在curDirBlock中加入新的inode指针,指向inodeTable中的位置
	while(curBlock->inodeID[i])  
		i++;
	curBlock->inodeID[i]=pi;
	strcpy(curBlock->fileName[i],dirName);
	writeDisk(curBlock,BLOCK_SIZE,1,sizeof(MyDisk)+curBlockp*BLOCK_SIZE);

	//为新的目录分配一个inode和DirBlock
	disk->superB.freeBlocks--;
	disk->superB.freeInodes--;

	disk->inodeUsedMap[pi]=1;
	disk->inodeTable[pi].blockPoint[0]=pb;
	disk->inodeTable[pi].fileType=2;

	disk->blockUsedMap[pb]=1;
	newDirBlock=(DirBlock *)malloc(sizeof(DirBlock));
	memset(newDirBlock,0,sizeof(DirBlock));
	writeDisk(&newDirBlock,BLOCK_SIZE,1,sizeof(MyDisk)+pb*BLOCK_SIZE);
}
