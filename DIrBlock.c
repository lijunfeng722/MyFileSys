#include"MyDisk.h"
#include"DirBlock.h"

void showDirBlock(DirBlock * tBlock)
{
	int i=0;
	printf("show current DirBlock:\n");
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

DirBlock * getDirBlock(MyDisk *disk,char ** dirPath,int dep,short * dirBlockp)
{
	DirBlock * tBlock;
	int i=2;
	short pi=0,pb=0;
	tBlock=(DirBlock *)malloc(sizeof(DirBlock));
	readDisk(tBlock,sizeof(DirBlock),1,sizeof(MyDisk));//root��dirBlock
	while(i<=dep)
	{
		pi=getFileInodeID(tBlock,*(dirPath+(i-1)));
		++i;
		pb=disk->inodeTable[pi].blockPoint[0];
		readDisk(tBlock,sizeof(DirBlock),1,sizeof(MyDisk)+pb*BLOCK_SIZE);
	}
	
	*dirBlockp=pb;
//	showDirBlock(tBlock);
	return tBlock;
}

short getAvailableInode(MyDisk * disk)
{
	short i=1;//i=0Ϊroot��inode,���Դ�i=1��ʼ
	for(;i<INODE_NUM;++i)
		if(!(disk->inodeUsedMap[i]))
			return i;
	return 0;
}
short getAvailabeBlock(MyDisk * disk)
{
	short i=1;//i=0Ϊroot��inode,���Դ�i=1��ʼ
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

	if(!pi||!pb)//����Ŀ¼ʧ��
		return 0;


	//��curDirBlock�м����µ�inodeָ��,ָ��inodeTable�е�λ��
	while(curBlock->inodeID[i])  
	{
		if(!strcmp(dirName,curBlock->fileName[i]))//�Ѵ���ͬ���ļ�
		{	
			printf("�Ѵ���ͬ���ļ����޷��½�!\n");
			return 0;
		}
		i++;
	}
	curBlock->inodeID[i]=pi;
	strcpy(curBlock->fileName[i],dirName);
	writeDisk(curBlock,BLOCK_SIZE,1,sizeof(MyDisk)+curBlockp*BLOCK_SIZE);

	//Ϊ�µ�Ŀ¼����һ��inode��DirBlock
	disk->superB.freeBlocks--;
	disk->superB.freeInodes--;

	disk->inodeUsedMap[pi]=1;
	disk->inodeTable[pi].blockPoint[0]=pb;
	disk->inodeTable[pi].fileType=2;

	disk->blockUsedMap[pb]=1;
	newDirBlock=(DirBlock *)malloc(sizeof(DirBlock));
	memset(newDirBlock,0,sizeof(DirBlock));
	writeDisk(newDirBlock,BLOCK_SIZE,1,sizeof(MyDisk)+pb*BLOCK_SIZE);
	return 1;
}
