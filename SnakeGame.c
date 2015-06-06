#include <stdlib.h>								
#include <LPC214x.H>                    /* LPC214x definitions                */
#include <stdio.h>

extern void LCDTextOut (unsigned char const* top_line, unsigned char const* bottom_line);
extern void setMatrixPixel(char x,char y,char v);
extern void displayMatrix(void);
extern void updateJoystick(void);
extern void updateLCD(void);

extern unsigned long int stateJoystick;
#define JOYSTICK_A !(stateJoystick&0x004000)
#define JOYSTICK_F !(stateJoystick&0x010000)
#define JOYSTICK_U !(stateJoystick&0x020000)
#define JOYSTICK_R !(stateJoystick&0x040000)
#define JOYSTICK_L !(stateJoystick&0x080000)
#define JOYSTICK_D !(stateJoystick&0x100000)

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

unsigned char topscore[16]="     Score     ";
unsigned char bottomscore[16];

unsigned int Walls[8][8];
int snaPosX[21];
int snaPosY[21];
unsigned int snaLen;
#define SNAKE_MAX 15

unsigned int snakeSpeed=1400;

void makeDot(){
	int randomVal1,randomVal2;
	randomVal1=rand()%8;
	randomVal2=rand()%8;
	while(Walls[randomVal1][randomVal2]!=0){
		randomVal1=rand()%8;
		randomVal2=rand()%8;
	}
	Walls[randomVal1][randomVal2]=1;
}

void renderSnake(){
	int j;
		for(j=0;j<snakeSpeed;j++){
			displayMatrix();
			updateLCD();
		}
}

void snakeCollision(unsigned int*GameEnd,unsigned int *snaDir){
	//if(snaPosX[0]>=8 || snaPosX[0]<0 || snaPosY[0]>=8 || snaPosY[0]<0 /*|| Walls[snaPosX[0]][snaPosY[0]]==2*/)
	//unsigned will never be negative
	/*if(snaPosX[0]>=8 || snaPosY[0]>=8 )
		GameEnd[0]=0;*/
	if(Walls[snaPosX[0]][snaPosY[0]]==2){
		GameEnd[0]=0;
	}
	else if(Walls[snaPosX[0]][snaPosY[0]]==1){
		snaLen++;
		makeDot();
		//snaPosX[snaLen[0]]=snaPosX[snaLen[0]-1];
		//snaPosY[snaLen[0]]=snaPosY[snaLen[0]-1];
		//lengthen=1;
	}
}


void updateSnake(unsigned int *snaDir,unsigned int *GameEnd){
		if( JOYSTICK_A )
			snakeSpeed-=100;
		else 
			snakeSpeed+=10;
		
		if(snakeSpeed<300)snakeSpeed=300;
		if(snakeSpeed>1400)snakeSpeed=1400;
		
		if( JOYSTICK_L && (snaDir[0]!=RIGHT))
			snaDir[0]=LEFT;
		else if( JOYSTICK_R && (snaDir[0]!=LEFT))
			snaDir[0]=RIGHT;
		else if( JOYSTICK_U && (snaDir[0]!=DOWN))
			snaDir[0]=UP;
		else if( JOYSTICK_D && (snaDir[0]!=UP))
			snaDir[0]=DOWN;
		
		renderSnake();
	
}


void InitSnake(unsigned int *snaDir)
{
	unsigned int i,j;
	snaLen=3;
	for(i=0;i<snaLen;i++){
		snaPosX[i]=0;
		snaPosY[i]=3;
	}

	snaDir[0]=RIGHT;
	
	for(i=0;i<8;i++)
			for(j=0;j<8;j++)
				Walls[i][j]=0;
	
	for(i=0;i<snaLen-1;i++)
		Walls[snaPosX[i]][snaPosY[i]]=2;
	
	i=3;
	
	while(i--)
		makeDot();
		sprintf((char*)topscore,"Game starting in");
				LCDTextOut(topscore,bottomscore);
			for(i=0;i<9;i++)
			renderSnake();
			renderSnake();
			for(i=0;i<16;i++)
			bottomscore[i]=' ';
			bottomscore[7]='2';
			LCDTextOut(topscore,bottomscore);
			for(i=0;i<9;i++)
			renderSnake();
			renderSnake();
			for(i=0;i<16;i++)
			bottomscore[i]=' ';
			bottomscore[7]='1';
			LCDTextOut(topscore,bottomscore);
			for(i=0;i<9;i++)
			renderSnake();
			renderSnake();
			for(i=0;i<16;i++)
			bottomscore[i]=' ';
			LCDTextOut(topscore,bottomscore);
			for(i=0;i<9;i++)
			renderSnake();
	sprintf((char*)topscore,"     Score     ");
	LCDTextOut(topscore,bottomscore);
}
void shiftSnake(){
	int i;
	
	for(i=snaLen-1;i>0;i--)
	{
		snaPosX[i]=snaPosX[i-1];
		snaPosY[i]=snaPosY[i-1];
	}
	
}

void drawSnake(){
	int i;
	Walls[snaPosX[snaLen-1]][snaPosY[snaLen-1]]=0;
	for(i=0;i<snaLen-1;i++)
		Walls[snaPosX[i]][snaPosY[i]]=2;
}
	
void moveSnake(unsigned int*GameEnd,unsigned int *snaDir){
	
	if(snaDir[0]==UP){
		snaPosY[0]-=1;
	}
	else if(snaDir[0]==DOWN){
		snaPosY[0]+=1;
	}
	else if(snaDir[0]==LEFT){
		snaPosX[0]-=1;
	}
	else if(snaDir[0]==RIGHT){
		snaPosX[0]+=1;
	}
	
	if(snaPosX[0]<0)snaPosX[0]=7;
	if(snaPosX[0]>=8)snaPosX[0]=0;
	if(snaPosY[0]<0)snaPosY[0]=7;
	if(snaPosY[0]>=8)snaPosY[0]=0;
}

void SnakeMain(unsigned int  *GameEnd,unsigned int *snaDir){
		int i,j;
		if(snaLen == SNAKE_MAX+1)
			GameEnd[0]=1;
		
		//setLCDNumber(bottomscore,*counter);
		//clearLCD();
		sprintf((char*) bottomscore,"%i of %i",snaLen-1,SNAKE_MAX);
		LCDTextOut(topscore,bottomscore);
		
		for(i=0;i<8;i++)
			for(j=0;j<8;j++){
				if(Walls[i][j]==0)setMatrixPixel(i,j,0);
				if(Walls[i][j]==1)setMatrixPixel(i,j,1);
				if(Walls[i][j]==2)setMatrixPixel(i,j,1);
			}
		
		
		if(GameEnd[0] == 3){
			updateSnake(snaDir,GameEnd);
			/* move snake head */
			shiftSnake();	
			moveSnake(GameEnd,snaDir);
			/* checking for collision after moving */
			snakeCollision(GameEnd,snaDir);
			drawSnake();
		}
		if(GameEnd[0] !=3 ){
			return;
		}
}



int PlaySnakeGame(void){
	unsigned int GameEnd[1];
	unsigned int snaDir[1];
	GameEnd[0]=3;

	InitSnake(snaDir);
			
	while(1){
		renderSnake();
		updateJoystick();
		SnakeMain(GameEnd,snaDir);
		if(GameEnd[0]==1)
			return 1;
		else if(GameEnd[0]==0)
			return 0;
	}
}
