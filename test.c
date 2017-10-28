#include "stdio.h"
#include "def.h"
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



/**************************************************************/
//do_connect 
//@brief	 connect input mac device
//@input 	 mac address
//@output
//@date		2017.10.27
/***************************************************************/
 int do_connect (cmd_tbl_s * cmd, int argc,char *argv[])
{

	printf("connecting\n");
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

