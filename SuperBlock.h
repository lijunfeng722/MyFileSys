#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H
#define INODE_NUM 128
#define INODE_SIZE 128
#define BLOCK_NUM 1024
#define BLOCK_SIZE 2048
#include"InodeBlock.h"
#include"DirBlock.h"
#include"DirBlock.h"
typedef struct 
{
	int freeBlocks;
	int freeInodes;
}SuperBlock;



#endif