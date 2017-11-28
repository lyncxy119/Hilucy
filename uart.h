
extern int uartHandle;
int open_uart(char * device);
int config_uart(int fp,int baud_rate,int data_bits,char parity,int stop_bits);

void Calc_data_send(int handle,unsigned char *ptr,unsigned char size,unsigned char type);
