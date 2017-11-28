#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "def.h"
#include "uart.h"
#include "fcntl.h"

int uartHandle;
void *rxTask(void * arg);
void *txTask(void * arg);
void *cmdTask(void *arg);
Queue CreateList(void);
void main(int argc,char * argv[])
{
    printf("Lucy online now\n");

	mQueue = CreateList();

	uartHandle = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(uartHandle == -1)
	{
		printf("open uart error\n");
	}
	else
	{
		printf("uart open success\n");
	}
    //config uart
    set_opt(uartHandle,115200,8,'N',1);
    pthread_t cmd_Task,rx_Task ,tx_Task;
    pthread_create(&cmd_Task,NULL,cmdTask,NULL);
    pthread_create(&rx_Task,NULL,rxTask,NULL);
    pthread_create(&tx_Task,NULL,txTask,NULL);
	pthread_create(&core_Task,NULL,txTask,NULL);



}



