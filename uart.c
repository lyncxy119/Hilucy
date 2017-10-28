#include "def.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "fcntl.h"
#include "sys/termios.h"
/************************************/
//open uart instance
//
//@input :	device routine
//
//return :	file descriptor
//
/************************************/
int open_uart(char * device)
{
	int fp;
	fp = open(device,O_RDWR | O_NOCTTY | O_NONBLOCK);
	return fp;
}
/************************************/
//config uart interface
//
//@input fp   :  uart file descriptor
//       speed:  baudrate;
//
//return 0: config success
//      -1: error occurred
//
/************************************/
int config_uart(int fp,int baud_rate,int data_bits,char parity,int stop_bits)
{
	struct termios newtio,oldtio;
#if 0
    if(tcgetattr(fp,&oldtio) != 0)
    {//get uart attribution error
        printf("error\n");
        return -1;
    }
   if(tcgetattr(fp,&oldtio) != 0) 
   {
       printf("error\n");
       return -1;
   }
   //set baudrate
   int speed;
   switch(baud_rate)
   {
       case 9600:
        speed = B9600;
       break;
       case 19200:
        speed = B19200;
       break;
       case 38400:
        speed = B38400;

       break;
       case 115200:
        speed = B115200;
	   	printf("115200\n");
       break;
       default:
       break;
   }
   cfsetispeed(&newtio,speed);
   cfsetospeed(&newtio,speed);
    //set databits
    switch(data_bits)
    {
        case 7:
            newtio.c_cflag|=CS7;
        break;
        default:
        case 8:
			printf("8 data bits\n");
            newtio.c_cflag|=CS8;
            break;
    }
	newtio.c_cflag |= CS8;
    //set parity
    switch(parity)
    {
        default:
        case 'n':
        case  'N':
        {
            newtio.c_cflag &= ~PARENB;
			printf("no parity\n");
        }

        break;
        case 'o':
        case 'O':
        {
            newtio.c_cflag |= (PARODD |PARENB);
            newtio.c_cflag |= (INPCK);
        }
        break;
        case 'e':
        case 'E':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            newtio.c_cflag |=INPCK;
    }
	
#endif
/*	newtio.c_cflag &= ~PARENB;
	newtio.c_cflag |=  CS8;
	newtio.c_cflag &= ~CSTOPB;
	*/
	
//	cfsetispeed(&newtio,B115200);
//	cfsetospeed(&newtio,B115200);
//    newtio.c_cc[VTIME] = 1;
 //   newtio.c_cc[VMIN] = 1;
    tcflush(fp,TCIFLUSH);
    if((tcsetattr(fp,TCSANOW,&newtio))!= 0)
    {
        printf("error\n");
        return -1;
    }
    return 0;
}
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)   
{   
     struct termios newtio,oldtio;   
/*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/   
     if  ( tcgetattr( fd,&oldtio)  !=  0) {    
      perror("SetupSerial 1");  
    printf("tcgetattr( fd,&oldtio) -> %d\n",tcgetattr( fd,&oldtio));   
      return -1;   
     }   
     bzero( &newtio, sizeof( newtio ) );   
/*步骤一，设置字符大小*/   
     newtio.c_cflag  |=  CLOCAL | CREAD;    
     newtio.c_cflag &= ~CSIZE;    
/*设置停止位*/   
     switch( nBits )   
     {   
     case 7:   
      newtio.c_cflag |= CS7;   
      break;   
     case 8:   
      newtio.c_cflag |= CS8;   
      break;   
     }   
/*设置奇偶校验位*/   
     switch( nEvent )   
     {   
     case 'o':  
     case 'O': //奇数   
      newtio.c_cflag |= PARENB;   
      newtio.c_cflag |= PARODD;   
      newtio.c_iflag |= (INPCK | ISTRIP);   
      break;   
     case 'e':  
     case 'E': //偶数   
      newtio.c_iflag |= (INPCK | ISTRIP);   
      newtio.c_cflag |= PARENB;   
      newtio.c_cflag &= ~PARODD;   
      break;  
     case 'n':  
     case 'N':  //无奇偶校验位   
      newtio.c_cflag &= ~PARENB;   
      break;  
     default:  
      break;  
     }   
     /*设置波特率*/   
switch( nSpeed )   
     {   
     case 2400:   
      cfsetispeed(&newtio, B2400);   
      cfsetospeed(&newtio, B2400);   
      break;   
     case 4800:   
      cfsetispeed(&newtio, B4800);   
      cfsetospeed(&newtio, B4800);   
      break;   
     case 9600:   
      cfsetispeed(&newtio, B9600);   
      cfsetospeed(&newtio, B9600);   
      break;   
     case 115200:   
      cfsetispeed(&newtio, B115200);   
      cfsetospeed(&newtio, B115200);   
      break;   
     case 460800:   
      cfsetispeed(&newtio, B460800);   
      cfsetospeed(&newtio, B460800);   
      break;   
     default:   
      cfsetispeed(&newtio, B9600);   
      cfsetospeed(&newtio, B9600);   
     break;   
     }   
/*设置停止位*/   
     if( nStop == 1 )   
      newtio.c_cflag &=  ~CSTOPB;   
     else if ( nStop == 2 )   
      newtio.c_cflag |=  CSTOPB;   
/*设置等待时间和最小接收字符*/   
     newtio.c_cc[VTIME]  = 1;   
     newtio.c_cc[VMIN] = 1;   
/*处理未接收字符*/   
     tcflush(fd,TCIFLUSH);   
/*激活新配置*/   
if((tcsetattr(fd,TCSANOW,&newtio))!=0)   
     {   
      perror("com set error");   
      return -1;   
     }   
     printf("set done!\n");   
     return 0;   
}
/*********************************************************/
//Calc_data_send
//@brief pack data in buffer
//@input ptr
//       size
//		 type
//@output
/*********************************************************/
#if 1
void Calc_data_send(int handle,unsigned char *ptr,unsigned char size,unsigned char type)
{
	unsigned char checksum = 0,pkt_size = 0;
	unsigned char tx_data = 0;
    unsigned char cmd_pkt[200];
	cmd_pkt[0] = 0xF0;
	cmd_pkt[1] = type;
	cmd_pkt[2] = size;
	//send header
	checksum = checksum + type + size;
	pkt_size = pkt_size + size + 1 + 1 + 1 + 1 + 1;//F0 + type + len + checksum + FA
	int index = 3;
	while (size>0)
	{
		switch (*ptr)
		{
		case 0xF0:
			//uart2_send_byte(0xF5);
			//uart2_send_byte(0x01);
			cmd_pkt[index++] = 0xF5;
			cmd_pkt[index++] = 0x01;
			pkt_size++;
			break;
		case 0xFA:
			cmd_pkt[index++] = 0xF5;
			cmd_pkt[index++] = 0x02;
			pkt_size++;
			break;
		case 0xF5:
			cmd_pkt[index++] = 0xF5;
			cmd_pkt[index++] = 0x03;
			pkt_size++;
			break;
		default:
			//uart2_send_byte(*ptr);
			cmd_pkt[index++] = *ptr;
			break;
		 }
		checksum += *ptr;
		ptr++;
		size--;
	}
	switch (checksum)
	{
	case 0xF0:
		//uart2_send_byte(0xF5);
		//uart2_send_byte(0x01);
		cmd_pkt[index++] = 0xF5;
		cmd_pkt[index++] = 0x01;
		pkt_size ++;
		break;
	case 0xFA:
		cmd_pkt[index++] = 0xF5;
		cmd_pkt[index++] = 0x02;
		pkt_size ++;
		break;
	case 0xF5:
		cmd_pkt[index++] = 0xF5;
		cmd_pkt[index++] = 0x03;
		pkt_size ++;
		break;
	default:
		//uart2_send_byte(checksum);
		cmd_pkt[index++] = checksum;
		break;
	}
	cmd_pkt[index] = 0xFA;
	//pkt_size = size 
//	printf("size %d\n",pkt_size);
	//Serial_Write(hSerial, (char *)&cmd_pkt, pkt_size);
	write(handle,(unsigned char *)cmd_pkt,pkt_size);
#if 0
	for(int i = 0;i<pkt_size;i++)
		printf("0x%02X ",*(cmd_pkt+i));
	printf("\n");
#endif
	return 0;

}
#endif
