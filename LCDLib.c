#include <stdlib.h>								
#include <LPC214x.H>                    /* LPC214x definitions                */
#include <stdio.h>

void SetCommLCD(unsigned char bT);
void SetDataLCD(unsigned char bT);
void WaitReadyLCD(void);
unsigned char LCDTopData[16];
unsigned char LCDBotData[16];
unsigned char* LCDTopLine;
unsigned char* LCDBotLine;
unsigned char counter;
#define TOP_ADDR 0x80
#define BOT_ADDR 0xC0
#define REFRESH_LCD 150
int refreshLCD;
unsigned const char Symbols[0x100] =
 {
 0x20,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x20,0x20,0x20,0x20,0x20,0x20,0x20, // 0
 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, // 1
 0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F, // 2
 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F, // 3
 0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F, // 4
 0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F, // 5
 0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F, // 6
 0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0xC5,0x01, // 7
 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, // 8
 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, // 9
 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, // A
 0xA2,0xB5,0x20,0x20,0x20,0x20,0x20,0x02,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, // B
 0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8, // C
 0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0xC4,0xAF,0xB0,0xB1, //D
 0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE, //E
 0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7 // F
 };
void Sleep (int _slp)
{
int i;
for (i = 0; i < _slp * 400; i++);
}
void SetCommLCD(unsigned char bT)
{
IO1DIR |= (0xFF << 16); // Set data bus as outputs
IO1DIR |= (1 << 24); // Set RS as output
IO1DIR |= (1 << 25); // Set E as output
IO0DIR |= (1 << 30); // Set Backlight control as output
IO0DIR |= (1 << 22); // Set R/W as output
IO1CLR = (1 << 25); // E down
IO1CLR = (1 << 24); // RS = 0
IO0CLR = (1 << 22); // RW = 0
IO1CLR = (0xFF) << 16; // Clear data bus
IO1SET = (bT) << 16; // Write data to bus
IO1SET = (1 << 25); // E up
Sleep (10); // Wait
IO1CLR = (1 << 25); // E down
}
void SetDataLCD(unsigned char bT)
{
IO1DIR |= (0xFF << 16); // Set data bus as outputs
IO1DIR |= (1 << 24); // Set RS as output
IO1DIR |= (1 << 25); // Set E as output
IO0DIR |= (1 << 30); // Set Backlight control as output
IO0DIR |= (1 << 22); // Set R/W as output
IO1CLR = (1 << 25); // E down
IO1SET = (1 << 24); // RS = 1
IO0CLR = (1 << 22); // RW = 0
IO1CLR = (0xFF) << 16; // Clear data bus
IO1SET = (bT) << 16; // Write data to bus
IO1SET = (1 << 25); // E up
Sleep (10); // Wait
IO1CLR = (1 << 25); // E down
}
void WaitReadyLCD(void)
{
Sleep(10);
}
void SetCharLCD(unsigned char addr,unsigned char off){
	if(addr==TOP_ADDR)if(LCDTopData[off]==LCDTopLine[off])return;
	if(addr==BOT_ADDR)if(LCDBotData[off]==LCDBotLine[off])return;
	WaitReadyLCD();
	SetCommLCD(addr+off);
	WaitReadyLCD();
	if(addr==TOP_ADDR){
		LCDTopData[off]=LCDTopLine[off];
		SetDataLCD(Symbols[LCDTopLine[off]]);
	}
	if(addr==BOT_ADDR){
		LCDBotData[off]=LCDBotLine[off];
		SetDataLCD(Symbols[LCDBotLine[off]]);
	}
}
void LCDTextOut (unsigned char* top_line, unsigned char* bottom_line)
{
	LCDTopLine=top_line;
	LCDBotLine=bottom_line;
}
void InitLCD(void)
{
	int i;
	refreshLCD=REFRESH_LCD;
	counter=0;
	Sleep (20); SetCommLCD(0x30);
	Sleep (20); SetCommLCD(0x30);
	Sleep (20); SetCommLCD(0x30);
	WaitReadyLCD(); SetCommLCD(0x38);
	WaitReadyLCD(); SetCommLCD(0x08);
	WaitReadyLCD(); SetCommLCD(0x01);
	WaitReadyLCD(); SetCommLCD(0x06);
	WaitReadyLCD(); SetCommLCD(0x0C);
	WaitReadyLCD(); SetCommLCD(0x40);
	WaitReadyLCD(); SetCommLCD(0xC4);
	LCDTextOut("XOXOXOXOXOXOXOXO","OXOXOXOXOXOXOXOX");
	for(i=0;i<16;i++){
		LCDTopData[i]='X';
		LCDBotData[i]='O';
	}
}
void SetBacklight (int Backlight)
{
 if (Backlight) IO0SET |= 1 << 30;
 else 					IO0CLR |= 1 << 30;
}

void updateLCD(void){
	refreshLCD--;
	if(refreshLCD)return;
	refreshLCD=REFRESH_LCD;
	if(counter<16)SetCharLCD(TOP_ADDR,counter);
	else 					SetCharLCD(BOT_ADDR,counter-16);
	counter++;
	if(counter==32)counter=0;
}
