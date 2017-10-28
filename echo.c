#include "cmdTask.h"
#include "stdio.h"
/***********************************************/
//find command
//@input : string input from user
//@return: command structure ptr
/***********************************************/
 int do_echo (cmd_tbl_s * cmd, int argc,char *argv[])
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
	for(int i = 0;i<(argc);i++)
	{
		printf("%s ",argv[i]);
	}
	return 0;
}
 //cmd_tbl_s cmd_echo __attribute__ ((unused,section (".CmdLine_cmd"))) ={"echo",10,do_echo};
 INSTALL_CMD(echo,10,do_echo);