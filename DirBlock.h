#ifndef DATABLOCK_H
#define DATABLOCK_H

#define DIR_MAX_NUM 16
#define FILENAME_MAXLEN 32
#include<string.h>
#include"SuperBlock.h"

typedef struct 
{
	short inodeID[DIR_MAX_NUM];
	char fileName[DIR_MAX_NUM][FILENAME_MAXLEN];//目录下的文件
}DirBlock;



#endif