#include "stdio.h"
#include "stdlib.h"
#include "def.h"
#include "errno.h"
#include "string.h"

#include <curl/curl.h>
#define OTA_SUCCESS 0
#define ERROR_BLE_DISCONNECTED -1
int patch_seq = 0,req_seq = 0,req_num = 0, offset = 0;
extern int hwid_flag;
extern int version_flag;
extern int psn_flag;
extern short ctrl_app_version;
extern short ctrl_boot_version;
extern short calc_app_version;
extern short calc_boot_version;
extern short hw_id;
unsigned char debug_content[60*1024];
unsigned short crc_16(unsigned char *data_p, unsigned short length, unsigned short crc_code);
int do_test (cmd_tbl_s * cmd, int argc,char *argv[])
{

	if(argc > cmd->maxargs)
	{
		printf("too many args\n");
		return -1;
	}
	else if(argc == 0)
	{
		return 0;
	}
	printf("testing\n");
	return 0;
}
INSTALL_CMD(test,10,do_test);

void reset_device(void)
{
	char buf[2];
	Calc_data_send(uartHandle,buf,1,RESET_REQ);
}

/**************************************************************/
//do_connect 
//@brief	 connect input mac device
//@input 	 mac address
//@output
//@date		2017.10.27
/***************************************************************/
 int do_connect (cmd_tbl_s * cmd, int argc,char *argv[])
{

//	printf("connecting\n");
	if(argc > cmd->maxargs)
	{
		printf("too many args\n");
		return -1;
	}
	else if(argc == 0)
	{
		return 0;
	}
	unsigned char buffer[50];
	unsigned char psn[6];
	sscanf(argv[1],"%x%x%x%x",&psn[0],&psn[1],&psn[2],&psn[3]);
	psn[4]= 0x35;
	psn[5] = 0xEF;
	Calc_data_send(uartHandle,psn,6,CONNECT_REQ);
	return 0;
}
//install command
INSTALL_CMD(connect,10,do_connect);



/**************************************************************/
//do_disconnect 
//@brief	terminate ble connection 
//@input 	 
//@date		2017.10.27
/***************************************************************/
 int do_disconnect (cmd_tbl_s * cmd, int argc,char *argv[])
{
    unsigned char buff[2];	
	Calc_data_send(uartHandle,buff,1,DISCONNECT_REQ);
}
//install command
INSTALL_CMD(disconnect,10,do_disconnect);
/**************************************************************/
//do_gethwid 
//@brief     gethwid 	
//@input 	 
//@date		2017.10.27
/***************************************************************/
 int do_gethwid(cmd_tbl_s * cmd, int argc,char *argv[])
{
    unsigned char buff[12] = {
		0x15,0x00,0x00,0x1B
	};	

	Calc_data_send(uartHandle,buff,4,WRITE_CMD);
}
//install command
INSTALL_CMD(gethwid,10,do_gethwid);

/**************************************************************/
//do_getver 
//@brief     get version 	
//@input 	 
//@date		2017.10.28
/***************************************************************/
 int do_getver(cmd_tbl_s * cmd, int argc,char *argv[])
{
    unsigned char buff[12] = {
		0x15,0x00,0x00,0x02
	};	

	Calc_data_send(uartHandle,buff,4,WRITE_CMD);
}
//install command
INSTALL_CMD(getver,10,do_getver);

/**************************************************************/
//do_getbat 
//@brief     get battery value 
//@input 	 
//@date		2017.10.28
/***************************************************************/
 int do_getbat(cmd_tbl_s * cmd, int argc,char *argv[])
{
    unsigned char buff[12] = {
		0x15,0x00,0x00,0x04
	};	
    time_t now;
	struct tm *timenow;
    time(&now);
    timenow=localtime(&now);
    printf("%d:%02d:%02d\n",timenow->tm_hour,timenow->tm_min,timenow->tm_sec);
	Calc_data_send(uartHandle,buff,4,WRITE_CMD);
}
//install command
INSTALL_CMD(getbat,10,do_getbat);

/**************************************************************/
//do_getpsn 
//@brief     get device psn 
//@input 	 
//@date		2017.10.28
/***************************************************************/
 int do_getpsn(cmd_tbl_s * cmd, int argc,char *argv[])
{
    unsigned char buff[12] = {
		0x15,0x00,0x00,0x01
	};	

	Calc_data_send(uartHandle,buff,4,WRITE_CMD);
}
//install command
INSTALL_CMD(getpsn,10,do_getpsn);

/**************************************************************/
//do_realtime 
//@brief     start realtime transfer 
//@input 	 
//@date		2017.10.28
/***************************************************************/
 int do_realtime(cmd_tbl_s * cmd, int argc,char *argv[])
{
    unsigned char buff[12] = {
		0x15,0x00,0x00,0x07,0x02
	};	

	Calc_data_send(uartHandle,buff,5,WRITE_CMD);
}
//install command
INSTALL_CMD(getHR,10,do_realtime);

/**************************************************************/
//do_stoprealtime 
//@brief     start realtime transfer 
//@input 	 
//@date		2017.10.28
/***************************************************************/
 int do_stoprealtime(cmd_tbl_s * cmd, int argc,char *argv[])
{
    unsigned char buff[12] = {
		0x15,0x00,0x00,0x08,0x00
	};	

	Calc_data_send(uartHandle,buff,5,WRITE_CMD);
}
//install command
INSTALL_CMD(stop,10,do_stoprealtime);

/**************************************************************/
//do_cmdSeq 
//@brief     exec certain command sequence
//@input 	 
//@date		2017.10.28
/***************************************************************/
 int do_cmdSeq(cmd_tbl_s * cmd, int argc,char *argv[])
{
	if(argc <3)
	{
		printf("too few arguments\n");
		return 0;
	}
	for(int i = 0; i < atoi(argv[2]);i++)
	{
		do_connect(cmd,argc,argv);
		sleep(10);
		do_getbat(cmd,argc,argv);
		sleep(2);
	//	do_realtime(cmd,argc,argv);
		//sleep(atoi(argv[3]));
	//	do_stoprealtime(cmd,argc,argv);
	//	sleep(1);
		//do_getbat(cmd,argc,argv);
	//	sleep(2);
		do_disconnect(cmd,argc,argv);
		sleep(100);
	}
   return 1;
}
//install command
INSTALL_CMD(chargeTest,10,do_cmdSeq);

/**************************************************************/
//do_normaltest 
//@brief     exec certain command sequence
//@input 	 
//@date		2017.10.28
/***************************************************************/
 int do_normaltest(cmd_tbl_s * cmd, int argc,char *argv[])
{
	if(argc <3)
	{
		printf("too few arguments\n");
		return 0;
	}
	FILE *fp;
	fp = fopen("log","a+");
	char log_start[28] = "----realtime start----";
	char log_end[28] = "----realtime end----";
	if(fp == NULL)
		printf("open log file error\n");
	else
	{
		fwrite(log_start,strlen(log_start),1,fp);
		fclose(fp);
	}

	for(int i = 0; i < atoi(argv[2]);i++)
	{
		do_connect(cmd,argc,argv);
		sleep(10);
		do_getbat(cmd,argc,argv);
		sleep(3);
		do_realtime(cmd,argc,argv);
		sleep(atoi(argv[3]));
		do_stoprealtime(cmd,argc,argv);
		sleep(1);
		do_getbat(cmd,argc,argv);
		sleep(2);
		do_disconnect(cmd,argc,atoi(argv[3]));
		sleep(100);
	}
	
	if(fp == NULL)
		printf("open log file error\n");
	else
	{
		fwrite(log_end,strlen(log_start),1,fp);
		fclose(fp);
	}
   return 1;
}
//install command
INSTALL_CMD(normalTest,10,do_normaltest);

/**************************************************************/
//do_normaltest 
//@brief     exec certain command sequence
//@input 	 
//@date		2017.10.28
/***************************************************************/
 int do_stanbytest(cmd_tbl_s * cmd, int argc,char *argv[])
{
	if(argc <3)
	{
		printf("too few arguments\n");
		return 0;
	}
	
	FILE *fp;
	fp = fopen("log","a+");
	char log_start[28] = "----standby start----\n";
	char log_end[28] = "----standby end----\n";
	
	if(fp == NULL)
		printf("open log file error\n");
	else
	{
		fwrite(log_start,strlen(log_start),1,fp);
		fclose(fp);
	}
	for(int i = 0; i < atoi(argv[2]);i++)
	{
		do_connect(cmd,argc,argv);
		sleep(10);
		do_getbat(cmd,argc,argv);
		sleep(3);
		do_disconnect(cmd,argc,atoi(argv[3]));
		sleep(atoi(argv[3]));
	}
   return 1;
}
//install command
INSTALL_CMD(standbyTest,10,do_stanbytest);

/**************************************************************/
//do_OTA 
//@brief     start realtime transfer 
//@input 	1:	psn
//			2:	ctrl or calc unit
//			3:	version
//			4:	.bin file
//@date		2017.10.28
/***************************************************************/
 int do_OTA(cmd_tbl_s * cmd, int argc,char *argv[])
{
	
	
	  int progress = 0;
 char progress_in_percent = 0, progress_in_percent_last = 0;

 
 patch_seq = 0;
 req_seq = 0;
 req_num = 0;
 offset = 0;
/*	for(int i =0;i<argc;i++)
	{
		printf("argv%d :%s\n",i,argv[i]);
	}*/
	do_getpsn(cmd,argc,argv);
	sleep(1);
	FILE * fp;
	int code_size;
	unsigned char *firm_content,debug_content[80*1024];
	unsigned int Filelength;
	//读取bin文件
	printf("file %s\n",argv[4]);
		fp = fopen(argv[4],"r");
		//read firmware content
	   if(strncmp(argv[2],"1",1) == 0)//ctrl unit
	   {
		  // fp = fopen("ctrl_factory_repair.bin","r");	
				
			unsigned char command[20] = { 0x17,0x00,0x01 };

					Calc_data_send(uartHandle,command, 3, WRITE_CMD);	   
	   }
	  else if(strncmp(argv[2],"2",1) == 0)//calc unit
	   {
		 //  fp = fopen("calc_production.bin","r");
		   
		   unsigned char command[20] = { 0x17,0x00,0x02 };

					Calc_data_send(uartHandle,command, 3, WRITE_CMD);
	   }
	  if(fp == NULL)
	   {
		   printf("open file error\n");
		   printf("%s\n",strerror(errno));
			   return -1;

	   }
		sleep(1);
		fseek(fp,0,SEEK_END);
		code_size = ftell(fp);
		Filelength = code_size;
		printf("file length is %d bytes\n",Filelength);
		fseek(fp,0,SEEK_SET);
		
		firm_content = (unsigned char *)malloc(Filelength +256 +30);
		memset(firm_content,0,Filelength +256 +30);
		if(firm_content == NULL)
			printf("malloc error\n");
		
		//create header  256 bytes
			for (int i = 0; i < 256; i++)
				firm_content[i] = i;
		fread(firm_content+256,1,Filelength,fp);
		
		Filelength = Filelength + 256;
		
		firm_content[2] = (unsigned char)(atoi(argv[3]) >> 8);
		firm_content[3] = (unsigned char)(atoi(argv[3]) >> 0);

		memcpy(firm_content +8,&code_size,4);
		unsigned short crc_code;
		crc_code = crc_16(firm_content,Filelength,0xFFFF);
		printf("len %d  crc  0x%04X \n",Filelength,crc_code);
		
		firm_content[Filelength] = (unsigned char)(crc_code >>0);
		firm_content[Filelength + 1] = (unsigned char)(crc_code>>8);
		//printf("0x%02X 0x%02X\n",firm_content[Filelength],firm_content[Filelength+1]);
		Filelength = Filelength +2;
	
		   int inside_index = 0, FF_count = 0,
					seed_index = 0, firm_index = 0,
					scramble_offset = 0, scramble_limit = 2,
					scramble_count = 0;
				char add = 0;
				if(strncmp(argv[2],"2",1) == 0)
				{
					//插值
					int calc_offset = 0,insert_value[7];
					static char scramble_content[100 * 1024], seed_array[7] = {0,4,8,24,80,60,44 },scramble_src[19];
					unsigned char rand_seed[19] = { 0x13,0x60,0x86,0x9e,0xbf,0xde,0x11,0xe5,0x9e,0xe4,0xb8,0xe8,0x56,0x2b,0x84,0xb4,0x16,0x18,0xf0 };
					for (int i = 0; i < 9; i++)
					{
						scramble_src[i * 2] = PSN[2] ^ rand_seed[i * 2];
						scramble_src[i * 2 + 1] = PSN[3] ^ rand_seed[i * 2+1];
					}
					scramble_src[18] = PSN[2] ^ rand_seed[18];
					for (int i = 0; i < Filelength; i++)
					{	
						if (inside_index == seed_array[seed_index] && (add == 0))
						{
							scramble_content[i] = 0x11;
							Filelength = Filelength + 1;
							seed_index++;
							if (seed_index == 7)
							{
								seed_index = 0;
								add = 0x55;
							}
							inside_index = 0;
						}
						else
						{
							scramble_content[i] = firm_content[firm_index++];
						}
						scramble_content[i] = scramble_content[i] ^ scramble_src[scramble_count];
						debug_content[i] = scramble_content[i];
						scramble_count++;
						if (scramble_count == scramble_limit)
						{
							scramble_limit ++;
							scramble_count = 0;
							if (scramble_limit == 20)
							{
								scramble_limit = 2;
							}
						}

							FF_count++;
						if (FF_count == 256+7)
						{
							FF_count = 0;
							inside_index = 0;
							add = 0;
						}
						else
						{
							inside_index++;
						}
					}
				}
				
				unsigned char command[20] = { 0x1C,0x00};
				memcpy(command + 2, (unsigned char *)&Filelength, 4);
				
				Calc_data_send(uartHandle,command, 6, WRITE_CMD);
				
				printf("patch status %d\n",GetPatchStatus());
				
			//写入patch data
		while(1)
		{
			usleep(1000);
			if(GetPatchStatus() == 2)
				break;
			
			if(BLE_STATUS == BT_DISCONNECTED)
			{
				if(progress_in_percent < 100)
				{
					printf("status %d\n",BLE_STATUS);
				return ERROR_BLE_DISCONNECTED;
				}
				else
				{
					printf("OTA OK\n");
					return OTA_SUCCESS;
				}
			}
			
			struct timeval tv;
			int current_time = 0;
			gettimeofday(&tv,NULL);
			current_time = tv.tv_sec;
			while((GetPatchStatus() == 0) &&(BLE_STATUS != BT_DISCONNECTED))
			{
				gettimeofday(&tv,NULL);
				//current_time = tv.tv_sec;
				//printf("current time \n");
				usleep(100);
				if(tv.tv_sec - current_time > 20)
				{
					reset_device();
					BLE_STATUS = BT_DISCONNECTED;
					ClearPatchStatus();
					printf("OTA timeout %d\n");
					do_disconnect(cmd,argc,argv);
					/*do_disconnect(cmd,argc,argv);
					BLE_STATUS = BT_DISCONNECTED;
					ClearPatchStatus();
					free(firm_content);
					return OTA_TIMEOUT;*/
				}
			}
			while (GetPatchStatus() == 1)
			{
				
				//printf("req_seq %03d  req_num %03d\n",req_seq,req_num);
				for (int i = 0; i < req_num; i++)
				{
					unsigned char data[100];
					
					
					//char patch_cmd[100] = "WriteHandle: 1E Value: ";
					if(strncmp(argv[2],"1",1) == 0)
					{
						memcpy(data + 1, firm_content + offset * 19, 19);
						//usleep(10000);
					}
					else
					{
						memcpy(data + 1, debug_content + offset * 19, 19);
						//usleep(40000);
					}
					
					//写入seq
					data[0] = offset;
					offset++;
				/*	for (int i = 0; i < 20; i++)
					{
						itoa(data[i], (char *)data_str, 16);
						if (data[i]<0x10)
							strcat(patch_cmd, " 0");
						else
							strcat(patch_cmd, " ");
						strcat(patch_cmd, (char*)data_str);
					}*/
				//	printf("%s\n",argv[2]);
					if(strncmp(argv[2],"1",1) == 0)	
					{
						//printf("ctrl\n");
						usleep(10000);
					}
					
					else
					{
						//printf("calc\n");
						usleep(30000);
					}
					
					//Serial_Write(hSerial, patch_cmd, strlen(patch_cmd));
					unsigned char command[22] = { 0x1E,0x00 };
					memcpy(command+2,data,20);			
					progress++;
					progress_in_percent = (int)(progress * 100) /(int) (Filelength / 19);

					//printf("%d  %d",progress_in_percent,progress_in_percent_last);
					if (progress_in_percent != progress_in_percent_last)
					{
						printf("%02d%%", progress_in_percent);
						
						if (progress_in_percent >= 100)
						{
							printf("\n");
							//printf("bin file transfer over\n");
							//break;
						}
						else
							printf("\b\b\b");
						fflush(stdout);
						progress_in_percent_last = progress_in_percent;
					}
					
					Calc_data_send(uartHandle,command, 22, WRITE_CMD);
					
					/*for(int i = 0;i < 22;i ++)
					{
						printf("0x%02X ",command[i]);
					}
					printf("\n");*/
				}
				ClearPatchStatus();
				
				if(BLE_STATUS == BT_DISCONNECTED)
				{
					printf("OTA disconnected\n");
					return ERROR_BLE_DISCONNECTED;

				}
			}
		}
	//	memset(firm_content,0,Filelength +256 +30);
		printf("OTA over\n");
		ClearPatchStatus();
		free(firm_content);
		//firm_content = NULL;
		return OTA_SUCCESS;
}
//install command
INSTALL_CMD(OTA,3,do_OTA);

/*
//                                      16   12   5
// this is the CCITT CRC 16 polynomial X  + X  + X  + 1.
// This works out to be 0x1021, but the way the algorithm works
// lets us use 0x8408 (the reverse of the bit pattern).  The high
// bit is always assumed to be set, thus we only use 16 bits to
// represent the 17 bit value.
*/
#define POLY	0x1021
unsigned short crc_16(unsigned char *data_p, unsigned short length, unsigned short crc_code)
{
	unsigned char i;
	unsigned int data;
	static unsigned int crc = 0xffff;
	crc = crc_code;
	if (length == 0)
		return (~crc);

	do
	{
		for (i = 0, data = (unsigned int)0xff & *data_p++;
			i < 8;
			i++, data >>= 1)
		{
			if ((crc & 0x0001) ^ (data & 0x0001))
				crc = (crc >> 1) ^ POLY;
			else  crc >>= 1;
		}
	} while (--length);

	/*crc = ~crc;
	data = crc;
	crc = (crc << 8) | (data >> 8 & 0xff);
	*/
	return (crc);
}


/**************************************************************/
//do_cmdSeq 
//@brief     exec certain command sequence
//@input 	 
//@date		2017.10.28
/***************************************************************/
 int do_hex2float(cmd_tbl_s * cmd, int argc,char *argv[])
{
	if(argc <2)
	{
		printf("too few arguments\n");
		return 0;
	}
	unsigned char buff[4],tempL,tempH;
	//sscanf("%02x %02x %02x %02x",buff[0],buff[1],buff[2],buff[3]);
	
	for(int i = 1;i < 5;i++)
	{

		if(argv[i][0] >= 'A' && argv[i][0]<='F')
		{
			tempH = 10 + argv[i][0] - 65;
		}
		else	if(argv[i][0] >= 'a' && argv[i][0]<='f')
		{
			tempH = 10 + argv[i][0] - 65 - 32;
		}
		else
		{
			tempH = argv[i][0] - 48;
		}
			
		if(argv[i][1] >= 'A' && argv[i][1]<='F')
			{
				tempL = 10 + argv[i][1] - 65;
			}
		else	if(argv[i][1] >= 'a' && argv[i][1]<='f')
		{
			tempH = 10 + argv[i][1] - 65 - 32;
		}
			else
			{
				tempL = argv[i][1] - 48;
			}
		buff[i-1] = tempH<<4|tempL;
		printf("tempH 0x%02X tempL 0x%02X data 0x%02X\n",tempH,tempL,buff[i-1]);
	}
	float data_float = 0;
	for(int i = 0;i < 4;i++)
		printf("0x%02X ",buff[i]);
	*(unsigned char *)&data_float = buff[0];
	*((unsigned char *)(&data_float) + 1) = buff[1];
	*((unsigned char *)(&data_float) + 2) = buff[2];
	*((unsigned char *)(&data_float) + 3) = buff[3];
	
	printf("\nresult : %f\n",data_float);
   return 1;
}

//install command
INSTALL_CMD(bat,10,do_hex2float);

/**************************************************************/
//do_cmdSeq 
//@brief     exec certain command sequence
//@input 	 
//@date		2017.10.28
/***************************************************************/
 int do_connectTest(cmd_tbl_s * cmd, int argc,char *argv[])
{
	for(int i = 0; i < atoi(argv[2]);i++)
	{
		do_connect(cmd,argc,argv);
		sleep(10);
		do_disconnect(cmd,argc,atoi(argv[3]));
		sleep(atoi(argv[3]));
	}
   return 1;
}

//install command
INSTALL_CMD(connectTest,10,do_connectTest);

/**************************************************************/
//do_cmdSeq 
//@brief     exec certain command sequence
//@input 	 
//@date		2017.10.28
/***************************************************************/
 int do_OTATest(cmd_tbl_s * cmd, int argc,char *argv[])
{
	for(int i = 0; i < atoi(argv[2]);i++)
	{
		do_connect(cmd,argc,argv);
		
		sleep(10);
		
		
		do_getver(cmd,argc,argv);
		sleep(1);
		do_realtime(cmd,argc,argv);
		do_getver(cmd,argc,argv);

		//sleep(5);
		sleep(20);
		do_stoprealtime(cmd,argc,argv);
		sleep(1);
		do_getver(cmd,argc,argv);
		printf("getversion over\n");
		sleep(3);
		do_disconnect(cmd,argc,argv);
		sleep(3);
	}
   return 1;
}

//install command
INSTALL_CMD(debug,10,do_OTATest);


/**************************************************************/
//do_cmdSeq 
//@brief     exec certain command sequence
//@input 	 argv[0]:	OTAList
//			 argv[1]:	PSN
//			 argv[2]:	1
//			 argv[3]:   ctrl version
//			 argv[4]:   calc version
//			 argv[5]:	ctrl file 
//			 argv[6]:   calc file
//			 argv[7]:   encrypt file
//@date		2017.10.28
/***************************************************************/

FILE *fp_device_info;
 int do_OTAList(cmd_tbl_s * cmd, int argc,char *argv[])
{
	int OTA_status = 0;
	char psn_list[8000][10];
	FILE *fp_list;
	char ctrl_version[10] = {0},calc_version[10] = {0};
	struct timeval tv;
	int current_time = 0;
	int timeout_flag = 0;
	int OTA_timeout_flag = 0;
	memcpy(psn_list[0],"0100765A",8);
	memcpy(psn_list[1],"010077c8",8);
	memcpy(psn_list[2],"01007683",8);
	memcpy(psn_list[3],"010078aa",8);
	
	do_login(cmd, argc,argv);
	fp_device_info = fopen("DeviceInfoList.txt","a+");
	if(fp_device_info == NULL)
		printf("file open error\n");
	fclose(fp_device_info);
	
	memcpy((void*)ctrl_version,argv[3],strlen(argv[3]));
	memcpy((void*)calc_version,argv[4],strlen(argv[4]));
	printf("calc %s\n",calc_version);
	//strcat(argv[3],'\0');
	//strcat(argv[3],'\0');
	//printf("len %d  %d  %s,%s\n",strlen(ctrl_version),strlen(calc_version),ctrl_version,calc_version);
	do_disconnect(cmd, argc,argv);
	fp_list = fopen("psnlist.txt","r+");
	if(fp_list == NULL)
	{
		printf("can not open psn list\n");
		return -1;
	}
	int index= 0;
//	fgets(psn_list[index++],10,fp_list);
	char line[12];
	while(fgets(line,10,fp_list))
	{
		if(line[0] !='\n')
		memcpy(psn_list[index++],line,strlen(line));
	};
	for(int i = 0;i<index;i++)
	printf("%d %s\n",i,psn_list[i]);
	fclose(fp_list);
	for(int i = 0;i<index;i++)
	//while(fgets(argv[1],10,fp_list))
	{
		
		
		gettimeofday(&tv,NULL);
		current_time = tv.tv_sec;
		printf("%s\n",psn_list[i]);
		memcpy(argv[1],psn_list[i],8);
	//	printf("time %d\n",current_time);
		/*for(int i = 0;i <argc;i++)
			printf("%s ",argv[i]);
		printf("\n");*/
		//do_upload(cmd, argc,argv);
		//sleep(1);
		do_connect(cmd, argc,argv);
	    int disconnect_count = 0;
		while(BLE_STATUS != BT_CONNECTED)
		{
			disconnect_count ++;
			gettimeofday(&tv,NULL);
			if(tv.tv_sec - current_time > 20)
			{
				timeout_flag = 1;
				reset_device();
				do_disconnect(cmd, argc,argv);
				sleep(1);
				break;
			}
		};
		disconnect_count = 0;
		if(timeout_flag == 1)
		{
			//do_disconnect(cmd, argc,argv);
			timeout_flag = 0;
			continue;
		}
		
		version_flag = 0;
		//sleep(4);
		
		gettimeofday(&tv,NULL);
		current_time = tv.tv_sec;
		//sleep(2);
		gettimeofday(&tv,NULL);
		current_time = tv.tv_sec;
		int version_timeout_flag = 0;
		/*while(version_flag != 1)
		{
			gettimeofday(&tv,NULL);

			if(tv.tv_sec - current_time > 15)
			{
				version_flag = 0;
				version_timeout_flag = 1;
				break;
			}
			usleep(200);
		}*/
		//sleep(3);
		//printf("ctrl %d %d calc %d %d \n",ctrl_app_version,ctrl_boot_version,calc_app_version,calc_boot_version);
	/*	if((ctrl_app_version == 2116) && (calc_app_version == 1954))
		{
			do_upload(cmd,argc,argv);
			do_disconnect(cmd, argc,argv);
			continue;
		}*/
			
		//sleep(3);
		//控制单元OTA文件
		printf("%d   file %s\n",strlen(argv[5]),argv[5]);
		//argv[4] = argv[5];
		//memcpy(argv[4],argv[5],strlen(argv[5]));
		argv[4] = "CTRL2116.bin";
		sleep(10);
		do_gethwid(cmd,argc,argv);
		sleep(1);
		do_getver(cmd,argc,argv);
		gettimeofday(&tv,NULL);
		int hwid_timeout_flag = 0;
		while(hwid_flag != 1)
		{
			gettimeofday(&tv,NULL);
			if(tv.tv_sec - current_time > 15)
			{
				hwid_timeout_flag = 1;
				hwid_flag = 0;
				break;
			}
			usleep(200);
		}
		/*if(hwid_timeout_flag == 1)
		{
			hwid_timeout_flag = 0;
			//hwid = 0;
		}*/
		OTA_status = do_OTA(cmd,argc,argv);
		while(OTA_status != OTA_SUCCESS)
		{
			if(BLE_STATUS == BT_DISCONNECTED)
			{
				printf("reconnect\n");
				do_connect(cmd, argc,argv);
				while(BLE_STATUS != BT_CONNECTED);	
				sleep(2);	
				OTA_status = do_OTA(cmd,argc,argv);
			}
			else if(OTA_status == OTA_TIMEOUT)
			{
				OTA_timeout_flag = 1;
				usleep(100);
				break;
			}
		}
		#if 1
		if(OTA_timeout_flag == 1)
		{
			OTA_timeout_flag = 0;
			do_disconnect(cmd, argc,argv);
			continue;
		}
		
	#if 1
	//OTA加密固件
		*argv[2] = '2';
		//*argv[3] = '322';
		//memcpy(argv[3],(void*)calc_version,strlen(calc_version));
		argv[3] = calc_version;
		//加密固件OTA文件
		//memcpy(argv[4],argv[7],strlen(argv[7]));
		argv[4] = "SEC1908_401.bin";
		printf("%s\n",argv[4]);
		//for(int i = 0 ; i < argc;i++)
		//printf("%s\n",argv[i]);
		OTA_status = do_OTA(cmd,argc,argv);
		while(OTA_status != OTA_SUCCESS)
		{
			if(BLE_STATUS == BT_DISCONNECTED)
			{
				printf("reconnect\n");
				do_connect(cmd, argc,argv);
				while(BLE_STATUS != BT_CONNECTED);	
				sleep(2);	
				OTA_status = do_OTA(cmd,argc,argv);
			}
			if(OTA_status == OTA_TIMEOUT)
			{
				OTA_timeout_flag  = 1;
				break;
			}
			
		}
		if(OTA_timeout_flag == 1)
		{
			OTA_timeout_flag = 0;
			continue;
		}
		else
		printf("加密固件ok\n");
		//OTA加密固件完成
	//	#endif
		*argv[2] = '2';
		//*argv[3] = '322';
		//memcpy(argv[3],(void*)calc_version,strlen(calc_version));
		argv[3] = calc_version;
		//计算单元固件OTA文件
		//memcpy(argv[4],argv[6],strlen(argv[6]));
		argv[4] = "CALC1954.bin";
		printf("%s\n",argv[4]);
	
				OTA_status = do_OTA(cmd,argc,argv);
				while(OTA_status != OTA_SUCCESS)
				{
					if(BLE_STATUS == BT_DISCONNECTED)
					{
						printf("reconnect\n");
						do_connect(cmd, argc,argv);
						while(BLE_STATUS != BT_CONNECTED);	
						sleep(2);
						OTA_status = do_OTA(cmd,argc,argv);						
					}
					else if(OTA_status == OTA_TIMEOUT)
					{
						OTA_timeout_flag = 1;
						break;
					}
					
				}
		
		if(OTA_timeout_flag == 1)
		{
			OTA_timeout_flag = 0;
			continue;
		}
		#endif
		#endif
		*argv[2] = '1';
		//*argv[3] = '111';
		//memcpy(argv[3],(void*)ctrl_version,strlen(ctrl_version));
		argv[3] = ctrl_version;
		//printf("len %d\n",sizeof(ctrl_version));
		//unsigned char command[20] = { 0x17,0x00,0x01 };

			//		Calc_data_send(uartHandle,command, 3, WRITE_CMD);
			sleep(2);
		/*do_connect(cmd, argc,argv);	
		sleep(8);*/
		do_getver(cmd,argc,argv);
		version_flag = 0;
		sleep(2);
		//gettimeofday(&tv,NULL);
		//current_time = tv.tv_sec;
		//int version_timeout_flag = 0;
		/*while(version_flag != 1)
		{
			gettimeofday(&tv,NULL);

			if(tv.tv_sec - current_time > 5)
			{
				version_flag = 0;
				version_timeout_flag = 1;
				break;
			}
			usleep(200);
		}
		if(version_timeout_flag == 1)
		{
			ctrl_app_version = 0;
			ctrl_boot_version = 0;
			calc_app_version = 0;
			calc_boot_version = 0;
		}
		else
		{
			printf("version %d %d %d %d\n",ctrl_app_version,ctrl_boot_version,calc_app_version,calc_boot_version);
		}
		
		if(hwid_timeout_flag == 1)
		{
			hw_id = 0;
		}
		else
		{
			printf("hwid %d\n",hw_id);
		}*/
		
		do_upload(cmd,argc,argv);
		//sleep(2);
		do_disconnect(cmd,argc,argv);
		BLE_STATUS = BT_DISCONNECTED;
		FILE * log;
					log = fopen("OTA_list.log","a+");
					
					if(log == NULL)
						printf("open log file error\n");
					else
					{
						fputs(argv[1],log);
						fputc('\n',log);
						fclose(log);
					}
	}
   return 1;
}

//install command
INSTALL_CMD(OTAList,10,do_OTAList);


size_t login_Log(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t size_read = 0;
	//size_read = fwrite(ptr, size, nmemb, (FILE*)stream);
	//cout << (char*)ptr << endl;
	puts(ptr);
	if ((strstr((const char *)ptr, (const char *)"\"code\":0") != NULL))
	{
		printf("登陆成功\n");
	}
	return size_read;
}
int do_login(cmd_tbl_s * cmd, int argc,char *argv[])
{
	CURL* curl = curl_easy_init();
	if (NULL == curl)
	{
		return -1;
	}
	curl_easy_setopt(curl, CURLOPT_URL, "http://moss.extantfuture.com/login/loginAction.do");
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookie.txt");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "userAccount=lucy&userPassword=E10ADC3949BA59ABBE56E057F20F883E");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, login_Log);
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return 1;
}

INSTALL_CMD(login,10,do_login);
size_t upload_Log(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t size_read = 0;
	//size_read = fwrite(ptr, size, nmemb, (FILE*)stream);
	//cout << (char*)ptr << endl;
	puts(ptr);
	if ((strstr((const char *)ptr, (const char *)"\"code\":0") != NULL))
	{
		printf("添加设备成功\n");
	}
	else
	{
		printf("添加设备失败\n");
	}
	return size_read;
}
int do_upload(cmd_tbl_s * cmd, int argc,char *argv[])
{
	CURL* curl = curl_easy_init();
	if (NULL == curl)
	{
	return ;
	}
	char post[500];
	sprintf(post,"psn=%s&hardwareEdition=10&controlBootloader=%d&calcBootloader=%d&status=1&cmd=4",argv[1],ctrl_boot_version,calc_boot_version);
	hw_id = 0;
	ctrl_boot_version = 0;
	calc_boot_version = 0;
	ctrl_app_version = 0;
	calc_app_version = 0;
			curl_easy_setopt(curl, CURLOPT_URL, "http://moss.extantfuture.com/device/bootloaderEditionAction.do");
			curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie.txt");
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
			printf("%s\n",post);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, upload_Log);
			//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			return ;
}
