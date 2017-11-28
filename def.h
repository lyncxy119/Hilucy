#include "uart.h"
#include "cmdTask.h"
#include "rxTask.h"
#include "txTask.h"
#include "coreTask.h"
#include "List.h"
#include "stdbool.h"
#include "stdlib.h"
#include "time.h"
extern int patch_status;
extern int patch_seq ,req_seq,req_num, offset;
extern  int progress ;
extern char progress_in_percent, progress_in_percent_last;

extern FILE *fp_log;
void SetPatchStatus(void);

int GetPatchStatus(void);

void ClearPatchStatus(void);
extern unsigned char PSN[4];
void *otaTask(void *arg);
FILE * GetLogFp(void);
