#include<stdio.h>
#include<stdlib.h>		
extern void LCDTextOut (unsigned char const* top_line, unsigned char const* bottom_line);
extern void setMatrixPixel(char x,char y,char v);
extern void clearMatrix(void);
extern void displayMatrix(void);
extern void updateLCD(void);
extern void updateJoystick(void);
extern unsigned long int stateJoystick;
#define JOYSTICK_A !(stateJoystick&0x004000)
#define JOYSTICK_F !(stateJoystick&0x010000)
#define JOYSTICK_U !(stateJoystick&0x020000)
#define JOYSTICK_R !(stateJoystick&0x040000)
#define JOYSTICK_L !(stateJoystick&0x080000)
#define JOYSTICK_D !(stateJoystick&0x100000)
//Space Invaders
extern unsigned char topLine[];
extern unsigned char botLine[];
unsigned int kills;
unsigned char spaceInvadersWON;
unsigned char spaceInvadersLOST;
unsigned int refreshShip;
unsigned int refreshEnemy;
unsigned int refreshEnemyBullet;
#define REFRESH_SHIP 2000
#define REFRESH_ENEMY 1500
#define REFRESH_ENEMY_BULLET 3000
char shipX;
char shipLife;
char shipLostALife;
int bulletN;
#define bulletMAX 2
int bulletX[bulletMAX];
int bulletY[bulletMAX];

#define enemyMAX 4
int enemyX[enemyMAX];
int enemyY[enemyMAX];
char enemyS[enemyMAX];
char enemyF[enemyMAX];
#define ENEMY_SPAWN 0x01
#define ENEMY_LEFT  0x02
#define ENEMY_RIGHT 0x04
#define ENEMY_DEAD  0x08
#define ENEMY_BOOM  0x10
#define ENEMY_BOOM2  0x20

#define ENEMY_HAS_SPAWN(n) (enemyS[n]&0x01)
#define ENEMY_HAS_LEFT(n)  (enemyS[n]&0x02)
#define ENEMY_HAS_RIGHT(n) (enemyS[n]&0x04)
#define ENEMY_HAS_DEAD(n)  (enemyS[n]&0x08)
#define ENEMY_HAS_BOOM(n)  (enemyS[n]&0x10)
#define ENEMY_HAS_BOOM2(n) (enemyS[n]&0x20)
#define SPAWN 19
int enemySpawn;
int enemySelect;
#define ENEMY_BULLET_MAX 3
#define ENEMY_FIRE 15
char enemyBulletN;
char enemyBulletX[ENEMY_BULLET_MAX];
char enemyBulletY[ENEMY_BULLET_MAX];
void initSpaceInvaders(void){
	int i,j;
	spaceInvadersWON=0;
	spaceInvadersLOST=0;
	refreshShip=REFRESH_SHIP;
	refreshEnemy=REFRESH_ENEMY;
	refreshEnemyBullet=REFRESH_ENEMY_BULLET;
	shipX=3;
	shipLife=3;
	shipLostALife=0;
	bulletN=0;
	enemyBulletN=0;
	enemySpawn=SPAWN;
	enemySelect=0;
	for(i=0;i<enemyMAX;i++)enemyS[i]=ENEMY_DEAD;
	kills=0;
	clearMatrix();
	sprintf((char*)topLine,"Prepare to play ");
	sprintf((char*)botLine," Space invaders ");
	LCDTextOut(topLine,botLine);
	for(j=0;j<10;j++)
	for(i=0;i<100000;i++)
		updateLCD();
}

void drawShip(char v){
	char i;
	setMatrixPixel(shipX,7,v);
	setMatrixPixel(shipX+1,7,v);
	setMatrixPixel(shipX-1,7,v);
	setMatrixPixel(shipX,6,v);
	for(i=0;i<bulletN;i++)
		setMatrixPixel(bulletX[i],bulletY[i],v);
}
void drawEnemyBullet(char v){
	char i;
	for(i=0;i<enemyBulletN;i++)
		setMatrixPixel(enemyBulletX[i],enemyBulletY[i],v);
}
void drawEnemy(char v){
	char i;
	for(i=0;i<enemyMAX;i++)if(!ENEMY_HAS_DEAD(i)){
		if(ENEMY_HAS_BOOM(i)){
			setMatrixPixel(enemyX[i]+1,enemyY[i]+1,v);
			setMatrixPixel(enemyX[i]-1,enemyY[i]-1,v);
			setMatrixPixel(enemyX[i]-1,enemyY[i]+1,v);
			setMatrixPixel(enemyX[i]+1,enemyY[i]-1,v);
		} if(ENEMY_HAS_BOOM2(i)){
			setMatrixPixel(enemyX[i]+2,enemyY[i]+2,v);
			setMatrixPixel(enemyX[i]-2,enemyY[i]-2,v);
			setMatrixPixel(enemyX[i]-2,enemyY[i]+2,v);
			setMatrixPixel(enemyX[i]+2,enemyY[i]-2,v);
		}else setMatrixPixel(enemyX[i],enemyY[i],v);
	}
}
void updateEnemy(void){
	char i,j,k;
	int aux;
	refreshEnemy--;
	if(!refreshEnemy){
		refreshEnemy=REFRESH_ENEMY;
		drawEnemy(0);
		//enemy
		for(i=0;i<enemyMAX;i++){
			if(!ENEMY_HAS_DEAD(i)){
				k=0;
				for(j=0;j<bulletN;j++)
					if(enemyX[i]==bulletX[j]&&
						 enemyY[i]==bulletY[j]&&
						 !ENEMY_HAS_BOOM(i)&&
					   !ENEMY_HAS_BOOM2(i)){
							 k=1;
							 aux=bulletX[j];
 							 bulletX[j]=bulletX[bulletN-1];
	 						 bulletX[bulletN-1]=aux;
		 				 	 aux=bulletY[j];
			 		 		 bulletY[j]=bulletY[bulletN-1];
				 			 bulletY[bulletN-1]=aux;
							 bulletN--;
							 j--;
				}
				if(k){
					enemyS[i]=ENEMY_BOOM;
					continue;
				}
				
				if(ENEMY_HAS_SPAWN(i)){
					if(enemyY[i]!=i){
						enemyY[i]++;
						continue;
					}else{
						enemyS[i]&=~ENEMY_SPAWN;
						continue;
					}
				}
				if(ENEMY_HAS_BOOM(i)){
					enemyS[i]&=~ENEMY_BOOM;
					enemyS[i]|=ENEMY_BOOM2;
					continue;
				}
				if(ENEMY_HAS_BOOM2(i)){
					enemyS[i]=ENEMY_DEAD;
					kills++;
					continue;
				}
				
				if(enemyX[i]==0){
					enemyS[i]&=~ENEMY_LEFT;
					enemyS[i]|= ENEMY_RIGHT;
				}
				if(enemyX[i]==7){
					enemyS[i]|= ENEMY_LEFT;
					enemyS[i]&=~ENEMY_RIGHT;
				}
				if(ENEMY_HAS_LEFT(i)) enemyX[i]--;
				if(ENEMY_HAS_RIGHT(i))enemyX[i]++;
				enemyF[i]--;
				if(!enemyF[i]){
					enemyF[i]=ENEMY_FIRE;
					if(enemyBulletN<ENEMY_BULLET_MAX){
						enemyBulletX[enemyBulletN]=enemyX[i];
						enemyBulletY[enemyBulletN]=enemyY[i];
						enemyBulletN++;
					}
				}
			}
		}
				
		enemySpawn--;
		enemySelect=(enemySelect+1)%8;
		if(!enemySpawn){
			enemySpawn=SPAWN;
			aux=-1;
			for(i=0;i<enemyMAX;i++)if(ENEMY_HAS_DEAD(i))aux=i;
			if(aux!=-1){
				enemyS[aux]=(rand()%2+1)<<1;
				enemyS[aux]|=ENEMY_SPAWN;
				enemyF[aux]=ENEMY_FIRE;
				enemyX[aux]=rand()%8;
				enemyY[aux]=0;
			}
		}
		drawEnemy(1);
	}
}

void updateSpaceInvaders(void){
	char i;
	int aux;
	updateEnemy();
	refreshShip--;
	if(!refreshShip){
		refreshShip=REFRESH_SHIP;
		drawShip(0);
		if(JOYSTICK_R)if(shipX<7)shipX++;
		if(JOYSTICK_L)if(shipX>0)shipX--;
		if(JOYSTICK_F|JOYSTICK_A)if(bulletN<bulletMAX){
			bulletX[bulletN]=shipX;
			bulletY[bulletN]=6;
			bulletN++;
		}
		//bullet
		for(i=0;i<bulletN;i++){
			bulletY[i]--;
			if(bulletY[i]<0){
				aux=bulletX[i];
				bulletX[i]=bulletX[bulletN-1];
				bulletX[bulletN-1]=aux;
				aux=bulletY[i];
				bulletY[i]=bulletY[bulletN-1];
				bulletY[bulletN-1]=aux;
				bulletN--;
				i--;
			}
		}
		if(shipLostALife){
			if(shipLife==0)spaceInvadersLOST=1;
			shipLife--;
			shipLostALife=0;
			shipX=3;
			bulletN=0;
			enemyBulletN=0;
			enemySpawn=SPAWN;
			for(i=0;i<enemyMAX;i++)enemyS[i]=ENEMY_DEAD;
			clearMatrix();
		}
		sprintf((char*)topLine,"Kills : %2i/20    ",kills);
		if(shipLife==0)sprintf((char*)botLine,"No lifes left   ");
		if(shipLife==1)sprintf((char*)botLine,"One life left   ");
		if(shipLife==2)sprintf((char*)botLine,"Two lifes left  ");
		if(shipLife==3)sprintf((char*)botLine,"Three lifes left");
		LCDTextOut(topLine,botLine);
		drawShip(1);
	}
	refreshEnemyBullet--;
	if(!refreshEnemyBullet){
		refreshEnemyBullet=REFRESH_ENEMY_BULLET;
		drawEnemyBullet(0);
		//enemy bullet
		for(i=0;i<enemyBulletN;i++){
			enemyBulletY[i]++;
			if(enemyBulletY[i]>7){
				aux=enemyBulletX[i];
				enemyBulletX[i]=enemyBulletX[enemyBulletN-1];
				enemyBulletX[enemyBulletN-1]=aux;
				aux=enemyBulletY[i];
				enemyBulletY[i]=enemyBulletY[enemyBulletN-1];
				enemyBulletY[enemyBulletN-1]=aux;
				enemyBulletN--;
				i--;
				continue;
			}
			if((enemyBulletX[i]==shipX&&(enemyBulletY[i]==6||enemyBulletY[i]==7))||
				 (enemyBulletX[i]==shipX-1&&enemyBulletY[i]==7)||
				 (enemyBulletX[i]==shipX+1&&enemyBulletY[i]==7))
				shipLostALife=1;
		}
		drawEnemyBullet(1);
	}
}
int playSpaceInvaders(){
	initSpaceInvaders();
	while(1){
		updateSpaceInvaders();
		displayMatrix();
		updateJoystick();
		updateLCD();
		if(kills>=20)spaceInvadersWON=1;
		if(spaceInvadersWON)return 1;
		if(spaceInvadersLOST)return 0;
	}
}
