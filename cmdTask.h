typedef	struct{
	char		*name;
	int		maxargs;
	int		(*cmd)(struct cmd_tbl_s *, int, char *[]);
	char hello;
}cmd_tbl_s;

#define INSTALL_CMD(name,maxarg,cmd) cmd_tbl_s cmd_##name __attribute__ ((unused,section (".CmdLine_cmd")))={#name,maxarg,cmd}
extern cmd_tbl_s  __CmdLine_cmd_start;
extern cmd_tbl_s  __CmdLine_cmd_end;
void *cmdTask(void *arg);
