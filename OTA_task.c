#include "def.h"
#include "stdio.h"
void *otaTask(void *arg)
{
		FILE * fp;
	int code_size;
	unsigned char *firm_content,debug_content[60*1024];
	unsigned short Filelength;
	//read firmware content
   if(strncmp(argv[1],"1",1) == 0)//ctrl unit
   {
	   fp = fopen("OTA_DA14580.bin","r");	
		unsigned char command[20] = { 0x17,0x00,0x01 };

				Calc_data_send(uartHandle,command, 3, WRITE_CMD);	   
   }
   else if(strncmp(argv[1],"2",1) == 0)//calc unit
   {
	   fp = fopen("stm32f411re-cube.bin","r");
	   
	   unsigned char command[20] = { 0x17,0x00,0x02 };

				Calc_data_send(uartHandle,command, 3, WRITE_CMD);
   }
   if(fp == NULL)
   {
	   printf("open file error");
	   	   return -1;

   }
	  sleep(1);
	fseek(fp,0,SEEK_END);
	Filelength = ftell(fp);
	printf("file length is %d bytes\n",Filelength);
	fseek(fp,0,SEEK_SET);
	
	firm_content = (unsigned char *)malloc(Filelength +256);
	if(firm_content == NULL)
		printf("malloc error\n");
	fread(firm_content+256,1,Filelength,fp);
	
	Filelength = Filelength + 256;
	
	memcpy(firm_content+2,&code_size,2);
	memcpy(firm_content +8,&code_size,4);
	unsigned short crc_code;
	crc_code = crc_16(firm_content,Filelength,0xFFFF);
	firm_content[Filelength] = (unsigned char)(code_size >>0);
	firm_content[Filelength + 1] = (unsigned char)(code_size>>8);
	
	Filelength = Filelength +2;
	
	   int inside_index = 0, FF_count = 0,
				seed_index = 0, firm_index = 0,
				scramble_offset = 0, scramble_limit = 2,
				scramble_count = 0;
			char add = 0;
			if (argv[1][0] == '2')
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
			memcpy(command + 2, (char *)&Filelength, 4);
			//sleep(1);
			Calc_data_send(uartHandle,command, 6, WRITE_CMD);
			//Serial_Write(hSerial, cmd_len, strlen(cmd_len));
			printf("patch status %d\n",GetPatchStatus());
		//写入patch data
	
		while (GetPatchStatus() == 1)
		{
			printf("send\n");
			for (int i = 0; i < req_num; i++)
			{
				unsigned char data[100], data_str[100];
				//char patch_cmd[100] = "WriteHandle: 1E Value: ";
				if (argv[1][0] == '1')
				{
					memcpy(data + 1, firm_content + offset * 19, 19);
				}
				else
				{
					memcpy(data + 1, debug_content + offset * 19, 19);
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
				
				//sleep(1);
				//Serial_Write(hSerial, patch_cmd, strlen(patch_cmd));
				unsigned char command[20] = { 0x1E,0x00 };
				memcpy(command+2,data,20);			

				Calc_data_send(uartHandle,command, 22, WRITE_CMD);
			//	sleep(1);
				printf("send firm...\n");
			}
			ClearPatchStatus();
		}
}