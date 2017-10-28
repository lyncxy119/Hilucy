#include "stdio.h"
typedef struct 
{
	char * message;
	unsigned short msg_len;
	unsigned char msg_type;
}MSG;
typedef struct node{
	MSG  msg;
	struct node *Next;
}Node,*Queue;
extern Queue mQueue;  
void ReverseQueue(Queue mQueue);
int GetQueueLen(Queue mQueue);
int PutMessage(Queue mQueue,MSG *pmsg,int pos);
int GetMessage(Queue mQueue,MSG *msg);
Queue CreateList(void);
