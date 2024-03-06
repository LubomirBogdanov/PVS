#ifndef __STARTUP_H__
#define __STARTUP_H__

#include <stdint.h>
#include "main.h"

void reset_handler(void);
void nmi_handler(void);
void hard_fault_handler(void);
void mem_manage_handler(void);
void bus_fault_handler(void);
void usage_fault_handler(void);
void svc_handler(void);
void debug_mon_handler(void);
void pend_sv_handler(void);
void systick_handler(void);
void default_handler(void);

#endif
