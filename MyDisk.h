#ifndef MYDISK_H
#define MYDISK_H
#include<time.h>
#include"SuperBlock.h"
#include"MyInode.h"

#define DISK_NAME "mydisk.txt"

typedef struct 
{
	struct tm gm_date;
	SuperBlock superB;
	//λʾͼ why�����ö�ά����
	int blockUsedMap[BLOCK_NUM];
	MyInode inodeTable[INODE_NUM];

}MyDisk;


#endif