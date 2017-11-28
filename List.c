#include "List.h"

Queue mQueue;
/*******************************************/
//CreateList
//create a list and return the head node
//@return Queue 
/*******************************************/

Queue CreateList(void)
{
    Queue pHead;
	pHead = (Queue)malloc(sizeof(Node));//分配节点
	if(pHead == NULL)
	{
		printf("create Queue error\n");
		return NULL;
	}
	pHead->Next = NULL;
	return pHead;
}
/*******************************************/
//PutMessage
//put message into the msg queue
//@return success or fail
/*******************************************/

int PutMessage(Queue mQueue,MSG *pmsg,int pos)
{
	int queue_len;
	queue_len = GetQueueLen(mQueue);	
	
	if(pos > queue_len)
	{
		printf("Invalid Queue position\n");
		return -1;
	}
	if(pmsg == NULL)
	{
		printf("Invalid message\n");
		return -1;
	}
	Queue pTempNode = (Queue)malloc(sizeof(Queue));
	pTempNode->msg.message = (char *)malloc(pmsg->msg_len *sizeof(char));
	if(pTempNode == NULL)
	{
		printf("malloc error\n");
		return -1;
	}
	//initialize node to insert
	memcpy(pTempNode->msg.message,pmsg->message,pmsg->msg_len);
//	pTempNode->msg.message = pmsg->message;
	pTempNode->msg.msg_len = pmsg->msg_len;
	pTempNode->msg.msg_type = pmsg->msg_type;
	Queue TempNode;	
	for(int i = 0;i< queue_len;i++)
	{
		if(mQueue->Next == NULL)
		{
			break;
		}
		else
		{
			mQueue = mQueue->Next; 
		}
	}
	TempNode = mQueue->Next; 
	mQueue->Next = pTempNode;
	pTempNode->Next = TempNode;
	return 1;	
}
//**********************************************//
//GetQueueLen
//
//return Queue length 
//**********************************************//
int GetQueueLen(Queue mQueue)
{
	Queue temp;

	int len = 0;
	temp = mQueue;
	while(temp != NULL)
	{
		len ++;
		temp = temp->Next;
//		printf("%s\n",temp->msg.message);
	}
	return len;
}
//**********************************************//
//ReverseQueue
//
//
//**********************************************//
void ReverseQueue(Queue mQueue)
{
	MSG *msg;
	while(mQueue != NULL)
	{
		if(mQueue->msg.message != NULL)
		{
			for(int i = 0;i<mQueue->msg.msg_len;i++)
			{
				printf("0x%0X ",(unsigned char )*(mQueue->msg.message + i));
			}
			printf("\n");
		}
		mQueue = mQueue->Next;
	}
}
/******************************************************/
//
//
//
/******************************************************/
int GetMessage(Queue mQueue,MSG *msg)
{

	while(mQueue != NULL)
	{
		msg->msg_len = mQueue->msg.msg_len;
		msg->msg_type = mQueue->msg.msg_type;
		if(mQueue->msg.message != NULL)
		{
			memcpy(msg->message,mQueue->msg.message,mQueue->msg.msg_len);
			break;
		}

		mQueue = mQueue->Next;
	}
	return 1;
}
/*****************************************************/
//DeleteNode
//
//
/*****************************************************/
int DeleteNode(Queue mQueue)
{
	Queue temp;
	temp = mQueue->Next; 
	mQueue->Next = temp->Next;
	//free(temp->msg.message);
	free(temp);
	return 1;

}
