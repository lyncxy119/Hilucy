#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cmdTask.h"
#define CFG_MAXARGS 10


cmd_tbl_s *find_command(char *argv[]);



void *cmdTask(void *arg)
{
	char * input = NULL;
	size_t len = 0;
	char * argv[20];
	int argc = 0;
	cmd_tbl_s *cmd_tb;
	while(1)
	{
		if(getline(&input,&len,stdin) != -1)
		{
			argc = parse_line(input,argv);
			cmd_tb = find_command(argv);
			if(cmd_tb != NULL)
			{
				cmd_tb->cmd(cmd_tb,argc,argv);
			}
			if(strcmp(input,"quit\n") == 0)
			exit(0);
		}
	}

	
}

/*************************************************************/
//parse_line
//seperate command arguments
//@input : command and pointer to the seperated arguments 
//
//@output: 
/************************************************************/
int parse_line (char *line, char *argv[])
{
	int nargs = 0;

#ifdef DEBUG_PARSER
	printf ("parse_line: \"%s\"\n", line);
#endif
	while (nargs < CFG_MAXARGS) {

		/* skip any white space */
		while ((*line == ' ') || (*line == '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
		printf ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && (*line != ' ') && (*line != '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
		printf ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	printf ("** Too many args (max. %d) **\n", CFG_MAXARGS);

#ifdef DEBUG_PARSER
	printf ("parse_line: nargs=%d\n", nargs);
#endif
	return (nargs);
}

/***********************************************/
//find command
//@input : string input from user
//@return: command structure ptr
/***********************************************/
cmd_tbl_s *find_command(char *argv[])
{
	cmd_tbl_s *cmdp;
	cmd_tbl_s *cmdp_temp = &__CmdLine_cmd_start;

	if(argv[0] == NULL)
	{
		printf("Invalid argv\n");
		return NULL;
	}
	for(cmdp = &__CmdLine_cmd_start;cmdp!=&__CmdLine_cmd_end;cmdp++)
	{
		if(argv[0] == NULL)
		{
			printf("error\n");
			return NULL;
		}
		//printf("0x%X  name %s\n",cmdp,cmdp->name);
		if(strncmp(cmdp->name,argv[0],strlen(argv[0]) -1) == 0)
		{
		//	printf("found\n");
			cmdp_temp = cmdp;
			return cmdp_temp;
		}
	}
	printf("no command found\n");
	return NULL;
	
}

