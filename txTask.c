#include "stdio.h"
#include "def.h"

unsigned char PSN[4];
int progress ;
int patch_status;
 char progress_in_percent, progress_in_percent_last;
 
 static FILE *fp_log;
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
				fp_log = fopen("log","a+");
				if(fp_log	< 0	)
				printf("log file open error\n");
									
				time_t now;
				struct tm *timenow;
				time(&now);
				timenow=localtime(&now);
									
									
				char log[100];
				sprintf(log,"时间：%s 连接成功\n",asctime(timenow));
				fwrite(log,strlen(log),1,fp_log);
				fclose(fp_log);
				printf("connected\n");
				break;
				case STATUS_DISCONNECTED:
					printf("disconnected\n");
				break;
				case STATUS_NOTIFY:
					{
					unsigned short handle;
					memcpy((char *)&handle,msg->message,2);
				//	for(int i = 0;i<msg->msg_len;i++)
				//		printf("%02X ",(unsigned char)*(msg->message + i));
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
								memcpy(PSN,data,4);
							}
							if(type[0] == 0x00 && type[1] == 0x02)
							{
								printf("Ctrl : %04d %02d\nCalc : %04d %02d\n",data[0]<<8|data[1],data[2]<<8|data[3],data[4]<<8|data[5],data[6]<<8|data[7]);
							}
							if (type[0] == 0x00 && type[1] == 0x04)
								{
									 float battery_value;
									//memcpy((char *)&battery_value, &data[0], 4);
									*(unsigned char *)(&battery_value) = data[0];
									*((unsigned char *)(&battery_value) + 1) = data[1];
									*((unsigned char *)(&battery_value) + 2) = data[2];
									*((unsigned char *)(&battery_value) + 3) = data[3];
									printf("battery : %f  %d%% status : 0x%02X\n", battery_value,data[4],data[5]);
									
									fp_log = fopen("log","a+");
									if(fp_log	< 0	)
										printf("log file open error\n");
									
									
									time_t now;
									struct tm *timenow;
									time(&now);
									timenow=localtime(&now);
									
									
									char log[100];
									sprintf(log,"时间：%s 电池电量:%f 百分比:%d  充电状态:0x%02X\n",asctime(timenow),battery_value,data[4],data[5]);
									fwrite(log,strlen(log),1,fp_log);
									fclose(fp_log);
								}
							if(type[0] == 0x00 && type[1] == 0x1B)
							{
								printf("HWID : %d\n",data[0]<<8|data[1]);
							}
						}
						break;
						case 0x0019:
						//for(int i = 0; i < msg->msg_len;i++)
					//		printf("0x%02X ",(unsigned char)*(msg->message + i));
						//printf("\n");
						if((unsigned char)*(msg->message + 2) == 0x55 &&(unsigned char)*(msg->message + 3) == 0xAA && (unsigned char)*(msg->message + 4) == 0xFF && (unsigned char)*(msg->message + 5) == 0x02 )
						{
							
							if((unsigned char)*(msg->message + 11) != 0x01)
								break;
							fp_log = fopen("log","a+");
							if(fp_log	< 0	)
							printf("log file open error\n");
												
							time_t now;
							struct tm *timenow;
							time(&now);
							timenow=localtime(&now);
												
												
							char log[100];
							sprintf(log,"时间：%d:%02d:%02d HR:%d\n",timenow->tm_hour,timenow->tm_min,timenow->tm_sec,(unsigned char)*(msg->message + 12));
							fwrite(log,strlen(log),1,fp_log);
							fclose(fp_log);
							puts(log);
						}
						else if((unsigned char)*(msg->message + 2) == 0x55 &&(unsigned char)*(msg->message + 3) == 0xAA && (unsigned char)*(msg->message + 4) == 0xFF && (unsigned char)*(msg->message + 5) == 0x01 )
						{
							unsigned char Rawbuffer[220];
								static unsigned short pkt_len,pkt_seq;
								pkt_seq = (unsigned char)(*(msg->message + 6))<<8 |(unsigned char)(*(msg->message + 7));
								pkt_len = (unsigned char)(*(msg->message + 8))<<8 |(unsigned char)(*(msg->message + 9));
							//	printf(" 0x%02X 0x%02X    rawdata seq : %d  0x%02X 0x%02X  len is %d\n",
									//	(unsigned char)*(msg->message + 6),(unsigned char)*(msg->message + 7),pkt_seq,(unsigned char)*(msg->message + 8),(unsigned char)*(msg->message + 9),pkt_len);
						}
						break;
						case 0x0022:
						{
							unsigned char OTA_data[20];
								memcpy(OTA_data, msg->message + 2, 20);

								
								req_seq = OTA_data[0];
								req_num = OTA_data[2] << 8 | OTA_data[3];

								if (req_num == 0)
								{
									printf("> 固件升级完成\n");
									progress = 0;
									progress_in_percent = 0;
									progress_in_percent_last = 0;
									//Clear_OTA_Start();
									//Clear_OTA_Type();
									patch_seq = 0;
									req_seq = 0;
									req_num = 0;
									offset = 0;
									patch_status = 2;
									break;
								}
								SetPatchStatus();
						}
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

void SetPatchStatus(void)
{
	patch_status = 1;
}
int GetPatchStatus(void)
{
	return patch_status;
}
void ClearPatchStatus(void)
{
	patch_status = 0;
}

FILE * GetLogFp(void)
{
	return fp_log;
}
