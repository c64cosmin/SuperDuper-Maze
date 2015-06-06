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

unsigned int renderSpeed=900;

void render(){
	int j;
		for(j=0;j<renderSpeed;j++){
			displayMatrix();
			updateLCD();
		}
}

void delay(unsigned int number){
int i,j;
		for (i = 0; i < number * 10; i++)
			for(j = 0; j < i; j++);
	
}

void checkCollision(unsigned int *Walls,unsigned int *carPos,unsigned int *GameEnd){
		if(Walls[8+carPos[0]] == 0)
			//Walls[8+carPos[0]]=2;
			setMatrixPixel(carPos[0],6,1);
		else
			GameEnd[0]=0;
}


void moveDown(unsigned int *Walls,unsigned int *carPos,unsigned int *GameEnd){
	unsigned int i,j;
	unsigned int center;
	unsigned int nextcenter;
  
	/* moving all rows down */
	for(i=0;i<7;i++)
		for(j=0;j<8;j++)
				Walls[(8*i)+j]=Walls[(8*i)+j+8];
	i=7;
	for(j=0;j<8;j++)
	{
		if(Walls[8*i+j]==0)
		{
			center=j+1;
			break;
		}
	}
	
	/* generating the next row seed */
	
	nextcenter = rand()%3;
	
	/* checking if out of bounds */
	if(center==2){
		nextcenter = rand()%2;
	}
	else if(center==6)
	{
		nextcenter = rand()%3;
		if(nextcenter==1)
			nextcenter=2;
	}
	
	/* generating the new row */
	
	if(nextcenter==0)
		for(j=0;j<8;j++)
		{
			if( (center==j) || ( center-1==j ) || (center+1==j ) )
				Walls[8*i+j]=0;
			else
				Walls[8*i+j]=1;
		}
	if(nextcenter==1)
		for(j=0;j<8;j++)
		{
			if( (center==j) || ( center+1==j ) || (center+2==j ) )
				Walls[8*i+j]=0;
			else
				Walls[8*i+j]=1;
		}
	if(nextcenter==2)
			for(j=0;j<8;j++)
			{
			if( (center==j) || ( center-1==j ) || (center-2==j ) )
				Walls[8*i+j]=0;
			else
				Walls[8*i+j]=1;
			}
			
		for(i=0;i<64;i++)
			setMatrixPixel(i%8,7-(i/8),Walls[i]);
		//setMatrixPixel(i%8,8-(i/8),Walls[i]);
		/* checking for collision */
		checkCollision(Walls,carPos,GameEnd);
			
			
}

void updateCar(unsigned int *Walls,unsigned int *carPos,unsigned int *GameEnd){
	
		
		/* checking for collision after moving */
		checkCollision(Walls,carPos,GameEnd);
		updateJoystick();
		if( JOYSTICK_A && (renderSpeed>300))
			renderSpeed-=100;
		else if(renderSpeed<900)
			renderSpeed+=10;
		
		if( JOYSTICK_L && carPos[0]!=0 )
			carPos[0]--;
		else if( JOYSTICK_R && carPos[0]!=7 )
			carPos[0]++;
		setMatrixPixel(carPos[0],6,1);
				render();
	
}
void InitRace(unsigned int *Walls,unsigned int *carPos){
		unsigned char top[16]="Game starting in";
		unsigned char bottom[16]="       3        ";
//		unsigned char topscore[16]="     Score     ";
  int i=0,j=0;
	carPos[0] = 4;
	
	renderSpeed=900;
	
	/*Generating first level */ 
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			{
				if(i==3||i==4||i==5)
					Walls[i+8*j]=0;
				else
					Walls[i+8*j]=1;
			}
	/*Generating car position */
			setMatrixPixel(carPos[0],6,1);
	
			delay(10);
			LCDTextOut(top,bottom);
			for(i=0;i<9;i++)
			render();
			render();
			for(i=0;i<16;i++)
			bottom[i]=' ';
			bottom[7]='2';
			LCDTextOut(top,bottom);
			for(i=0;i<9;i++)
			render();
			render();
			for(i=0;i<16;i++)
			bottom[i]=' ';
			bottom[7]='1';
			LCDTextOut(top,bottom);
			for(i=0;i<9;i++)
			render();
			render();
			for(i=0;i<16;i++)
			bottom[i]=' ';
			LCDTextOut(top,bottom);
			for(i=0;i<9;i++)
			render();
}

void RaceMain(unsigned int Racedelay,unsigned int *Walls,unsigned int *carPos,unsigned int *counter,unsigned int *GameEnd)
{
		unsigned char topscore[16]="     Score     ";
		unsigned char bottomscore[16];
		if(counter[0] == 200)
			GameEnd[0]=1;
		
		//setLCDNumber(bottomscore,*counter);
		//clearLCD();
		if(counter[0]%10==0){
		sprintf((char*) bottomscore,"%i of 200",counter[0]);
		LCDTextOut(topscore,bottomscore);
			updateLCD();
		setMatrixPixel(carPos[0],6,1);
		}
		
		if(GameEnd[0] == 3)
			updateCar(Walls,carPos,GameEnd);
		if(GameEnd[0] == 3)
			moveDown(Walls,carPos,GameEnd);
		if(GameEnd[0] !=3 ){
			return;
		}
}


int PlayRaceGame(void){
	unsigned int delay=1;
	
	unsigned int Walls[64];
	unsigned int carPos[1];
	unsigned int counter[1];
	unsigned int GameEnd[1];
	GameEnd[0]=3;
	
	InitRace(Walls,carPos);
			updateLCD();
	for(counter[0]=0;counter[0]<1000;counter[0]++){
		
		
		setMatrixPixel(carPos[0],6,1);
				render();
		RaceMain(delay,Walls,carPos,counter,GameEnd);
		if(GameEnd[0]==1)
			return 1;
		else if(GameEnd[0]==0)
			return 0;
	}
	return 0;
}

