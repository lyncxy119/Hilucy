#include "uart.h"
#include "fcntl.h"
#include "unistd.h"
#include "List.h"
#define SEEK_HEAD 0
#define SEEK_TYPE 1
#define SEEK_LEN 2
#define SEEK_DATA 3
#define SEEK_CHECKSUM 4
#define ESC	0xF5
#define ESC_F0 0x01
#define ESC_FA 0x02
#define ESC_F5 0x03

void *rxTask(void *arg)
{
	int UartHandle;
	UartHandle = open("/dev/ttyUSB0",O_RDWR);
	if(UartHandle == -1)
	{
		printf("can not open ttyUSB0\n");
		exit(0) ;
	}
	else
	{
		printf("ttyUSB0 open success\n");
	}
   set_opt(UartHandle,115200,8,'N',1);
	int num = 0;
	unsigned char buff[255];
	unsigned char rcvData[100];
	static int index = 0;
	static unsigned char ESC_flag = 0;
    while(1)
    {
		num = read(UartHandle,buff,255);

		static unsigned char current_state = SEEK_HEAD,current_type,current_len,current_checksum;	
		unsigned char checksum ;
		for(int i = 0;i<num;i++)
		{
			unsigned char rxByte;
			rxByte = buff[i];		
			if(rxByte == 0xF0)
			{
				current_state = SEEK_HEAD;
			}
			switch(current_state)
			{
				case SEEK_HEAD:
					if(rxByte == 0xF0)
						current_state = SEEK_TYPE;
				break;
				case SEEK_TYPE:
					current_type = rxByte;	
					current_state = SEEK_LEN;
					index = 0;
				break;
				case SEEK_LEN:
					current_len = rxByte;
					current_state = SEEK_DATA;

				break;
				case SEEK_DATA:
					
					if(rxByte == ESC)
					{
						ESC_flag = 1;	
						continue;
					}
					if(ESC_flag == 1)
					{
						ESC_flag = 0;
						switch(rxByte)
						{
							case 0x01:
								rxByte = 0xF0;	
							break;
							case 0x02:
								rxByte = 0xFA;
							break;
							case 0x03:
								rxByte = 0xF5;
							break;
						}
					}
					if(current_len == 0)
					{
						current_checksum = rxByte;
						current_state = SEEK_CHECKSUM;
						break;
					}
					if(index == current_len)
					{
						current_checksum = rxByte;
						current_state = SEEK_CHECKSUM;
						index = 0;
					}
					else 
					{
						rcvData[index++] = rxByte;	
					}
				break;
				case SEEK_CHECKSUM:
					checksum = 0;
					for(int i =0;i < current_len;i++)
					{
						checksum = checksum + rcvData[i];
					}
					checksum = checksum + current_type + current_len;
					if(checksum == current_checksum)
					{
						MSG *UartMSG;

						UartMSG = (MSG*)malloc(sizeof(MSG));
						UartMSG->message = (char *)malloc(current_len *sizeof(char));
						memcpy(UartMSG->message,rcvData,current_len);
						UartMSG->msg_len = current_len; 
						UartMSG->msg_type = current_type;
						PutMessage(mQueue,UartMSG,1);
						#if 0
						if(current_type == 0x10)
						{
							float bat_value;
							memcpy((unsigned char *)&bat_value,rcvData,4);
							printf("battery voltage is %f\n",bat_value);
						}
						else if(current_type == 0x01)
						{
							unsigned short HWID = 0;
							unsigned short Calc_APP_Ver = 0;
							unsigned short Ctrl_APP_Ver = 0;
							unsigned short Calc_Boot_Ver = 0;
							unsigned short Ctrl_Boot_Ver = 0;

						}
#endif
					}
						current_state = SEEK_HEAD;
				break;
				default:
				break;
			}
		}
    }

}
