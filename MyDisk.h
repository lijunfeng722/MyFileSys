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
	//位示图 why书上用二维数组
	int blockUsedMap[BLOCK_NUM];
	MyInode inodeTable[INODE_NUM];

}MyDisk;


#endif