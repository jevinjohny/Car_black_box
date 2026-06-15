#ifndef MAIN_H
#define MAIN_H
#define _XTAL_FREQ 20000000
#define MAXSPEED  249

extern unsigned int speed; //variable for speed(0-249)
extern unsigned int val; //ADC value
extern char arr[5]; // stores speed string and gear value for debugging in clcd
extern unsigned char col;
extern unsigned char page;
extern unsigned char menutoggle;

extern unsigned char clock_reg[3];
extern unsigned char calender_reg[4];
extern unsigned char time[9];
extern unsigned char date[11];

static void get_time(void);
void display_time(void);
void dashboard (void);
void menu(void);
void speed_to_str(unsigned int speed, char *str);
#endif