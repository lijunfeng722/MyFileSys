#ifndef INODEBLOCK_H
#define INODEBLOCK_H
#include"SuperBlock.h"
#include"MyInode.h"
typedef struct 
{
	MyInode pointer[BLOCK_SIZE/INODE_SIZE];
}InodeBlock;



#endif