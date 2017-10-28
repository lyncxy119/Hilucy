#include "stdio.h"
#include "def.h"
void *txTask(void *arg)
{
	MSG *msg;
	msg = (MSG*)malloc(sizeof(MSG));
	msg->message = (char *)malloc(100*sizeof(char));
    while(1)
    {
		if(GetQueueLen(mQueue)>1)
		{
			GetMessage(mQueue,msg);
			DeleteNode(mQueue);
			switch(msg->msg_type)
			{
				case 0x01:
				break;
				case 0x81:
				printf("boot ready\n");
				break;
				case 0x82:
				printf("app ready\n");
				break;
				case 0x10:
				{

				float batvalue;
				batvalue = 0.0;
				memcpy((unsigned char*)&batvalue,msg->message,msg->msg_len);
				printf("battery vaule is %f\n",batvalue);
				}
				case STATUS_CONNECTED:
					printf("connected\n");
				break;
				case STATUS_DISCONNECTED:
					printf("disconnected\n");
				break;
				case STATUS_NOTIFY:
					{
					unsigned short handle;
					memcpy((char *)&handle,msg->message,2);
					switch(handle)
					{
						case 0x0012:
						{
							unsigned char type[2],data[20];
							memcpy(type,msg->message +2,2);
							memcpy(data,msg->message+4,20);
							if(type[0] == 0x00 && type[1] == 0x01)
							{
								printf("PSN : %02X%02X%02X%02X\n",data[0],data[1],data[2],data[3]);
							}
							if(type[0] == 0x00 && type[1] == 0x1B)
							{
								printf("HWID : %d\n",data[0]<<8|data[1]);
							}
						}
						break;
						case 0x0019:
						break;
						case 0x0022:
						break;
						default :
						break;
					}
					}
				break;
				case STATUS_CONNECTING:
					printf("connecting\n");
				break;
				default:
				break;
			}
		}
    }
}
