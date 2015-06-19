#ifndef MYINODE_H
#define MYINODE_H


typedef struct 
{
	int owenerID;
	int fileSize;
	int fileType;//inode指向的是1-InodeBlock，2-DirBlock，3-DataBlock
	//12个直接指针，1个单层间接，1个双重间接，1个三重间接
	int blockPoint[15];
	
}MyInode;



#endif