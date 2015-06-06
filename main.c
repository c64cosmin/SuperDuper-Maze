#include <LPC214x.H>                    /* LPC214x definitions                */
////////////////////////////////////////////////////////////////////////////////
//MATRIX
char matrixData[8];
char matrixScan[8];
char matrixScanLine;
void initMatrix(void){
	char i;
	IO0DIR |= (1 << 15); // Chip select for shift registers
	IO0SET = (1 << 15);
	PINSEL0 |= (1 << 8) | (1 << 10) | (1 << 12);
	S0SPCR = (0 << 2) | // set up bits per transfer (0 - 8bit, 1 - defined)
					 (1 << 3) | // CPHA mode
					 (1 << 4) | // SCK is active high
					 (1 << 5) | // Set SPI as master
					 (0 << 8); // 16 bits per transfer
	S0SPCCR = 64;
	for(i=0;i<8;i++)matrixData[i]=0xFF;
	matrixScanLine=0;
	matrixScan[0]=0x7F;
	matrixScan[1]=0xBF;
	matrixScan[2]=0xDF;
	matrixScan[3]=0xEF;
	matrixScan[4]=0xF7;
	matrixScan[5]=0xFB;
	matrixScan[6]=0xFD;
	matrixScan[7]=0xFE;
}
void displayMatrix(void){
	IO0SET = 0x00008000; // Pull up P0.15
	S0SPDR = matrixScan[matrixScanLine];
	while ((S0SPSR & 0x80) == 0); // Wait until data is sent
	S0SPDR = matrixData[matrixScanLine];
	while ((S0SPSR & 0x80) == 0); // Wait until data is sent
	IO0CLR = 0x00008000; // Pull down P0.15 ("apply new settings")
	
	matrixScanLine=(matrixScanLine+1)%8;
}

void setMatrixRow(char data,char y){
	matrixData[y]=~data;
}

void setMatrixPixel(char x,char y,char v){
	if(v)matrixData[y]&=~(0x80>>x);
	else matrixData[y]|= 0x80>>x;
}

void clearMatrix(void){
	int i;
	for(i=0;i<8;i++)matrixData[i]=0xFF;
}

////////////////////////////////////////////////////////////////////////////////
//Joystick
unsigned long int stateJoystick;

#define JOYSTICK_A !(stateJoystick&0x004000)
#define JOYSTICK_F !(stateJoystick&0x010000)
#define JOYSTICK_U !(stateJoystick&0x020000)
#define JOYSTICK_R !(stateJoystick&0x040000)
#define JOYSTICK_L !(stateJoystick&0x080000)
#define JOYSTICK_D !(stateJoystick&0x100000)

void initJoystick(void){
	IO0DIR &= ~0x001F4000;
	stateJoystick=0;
}
void updateJoystick(void){
	stateJoystick=IO0PIN;
}
////////////////////////////////////////////////////////////////////////////////
extern void init_serial (void);         /* Initialize Serial Interface        */
extern void InitLCD(void);
extern void SetBacklight(int);
extern void LCDTextOut(unsigned char*, unsigned char*);
extern void updateLCD(void);
////////////////////////////////////////////////////////////////////////////////
unsigned char topLine[16];
unsigned char botLine[16];
extern int playMaze(void);
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main (void) {
	init_serial();
	initMatrix();
	initJoystick();
	InitLCD();
	SetBacklight(1);
	playMaze();
	LCDTextOut("Congratulations!","You're very cool");
	setMatrixRow(0x7E,0);
	setMatrixRow(0x81,1);
	setMatrixRow(0xA5,2);
	setMatrixRow(0x81,3);
	setMatrixRow(0xA5,4);
	setMatrixRow(0x99,5);
	setMatrixRow(0x81,6);
	setMatrixRow(0x7E,7);
	while(1){
		updateLCD();
		displayMatrix();
	}
}

