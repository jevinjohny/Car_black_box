#ifndef LOG_H
#define LOG_H

#include <xc.h>
#include "clcd.h"
#include "matrix_keypad.h"
#include "main.h"
#include "uart.h"
#include "clcd.h"
#include "ds1307.h"     
#include "dashboard.h"

#define LOGSIZE 14
#define MAXLOGS 10

void store_log(void);
void viewlog(void);
void downloadlog(void);

#endif