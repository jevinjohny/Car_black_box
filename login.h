#ifndef LOGIN_H
#define LOGIN_H

#include"main.h"
#include <xc.h>
#include "clcd.h"
#include "matrix_keypad.h"

#define PSWD_LEN 8
#define LED PORTBbits.RB0

extern unsigned char pass[PSWD_LEN];
extern unsigned char orgpass[PSWD_LEN];
extern unsigned char ind;
extern unsigned char keyflag;
extern unsigned char atflag;
extern unsigned char passcount;
extern unsigned char passcheck;
extern unsigned char blinkflag;


void login(void);
void changepassword(void);

#endif