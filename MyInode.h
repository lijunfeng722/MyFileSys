#ifndef MYINODE_H
#define MYINODE_H


typedef struct 
{
	int owenerID;
	int fileSize;
	int fileType;//inodeָ�����1-InodeBlock��2-DirBlock��3-DataBlock
	//12��ֱ��ָ�룬1�������ӣ�1��˫�ؼ�ӣ�1�����ؼ��
	int blockPoint[15];
	
}MyInode;



#endif