#define STATUS_DISCONNECTED 0x30
#define STATUS_CONNECTING	0x31
#define STATUS_CONNECTED	0x32
#define STATUS_NOTIFY		0x33

#define CONNECT_REQ	0x40
#define WRITE_CMD	0x41
#define DISCONNECT_REQ 0x42

void *txTask(void *arg);
