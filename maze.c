#include <stdio.h>
extern void LCDTextOut (unsigned char const* top_line, unsigned char const* bottom_line);
extern void setMatrixPixel(char x,char y,char v);
extern void clearMatrix(void);
extern unsigned long int stateJoystick;
#define JOYSTICK_A !(stateJoystick&0x004000)
#define JOYSTICK_F !(stateJoystick&0x010000)
#define JOYSTICK_U !(stateJoystick&0x020000)
#define JOYSTICK_R !(stateJoystick&0x040000)
#define JOYSTICK_L !(stateJoystick&0x080000)
#define JOYSTICK_D !(stateJoystick&0x100000)
extern unsigned char topLine[];
extern unsigned char botLine[];
//maze
#define ALLGAMES 4
char game1Won;
char game2Won;
char game3Won;
char game4Won;
int gamesFinished;
int drawMazeRefresh;
char refreshStatus;
char blinkMazeC;
char blinkMazeG;
char blinkMazeE;
char hasMoved;
int mazePX;
int mazePY;
int mazeMX;
int mazeMY;
int mazeCX;
int mazeCY;
char mazeMap[41][41];
void initMaze(void){
	sprintf(mazeMap[ 0],"#########################################");
	sprintf(mazeMap[ 1],"#.#.....#...........#.#.....#.....#....1#");
	sprintf(mazeMap[ 2],"#.#.###.#.#.#.#.###.#.#.###.#.###.#.#####");
	sprintf(mazeMap[ 3],"#.#...#...#.#.#.#.....#.#...#...#.......#");
	sprintf(mazeMap[ 4],"#.###.#####.#.###.###.#.#.#####.#######.#");
	sprintf(mazeMap[ 5],"#.#...#.....#...#.#...#.#.#.....#.#.....#");
	sprintf(mazeMap[ 6],"#.#.###.#######.###.###.#.#.#####.#.#####");
	sprintf(mazeMap[ 7],"#...#...#.....#...#.....#...#.#.....#...#");
	sprintf(mazeMap[ 8],"#####.###.###.###.#.#########.#.#####.###");
	sprintf(mazeMap[ 9],"#.......#.#.#...#.#.#.......#...#...#...#");
	sprintf(mazeMap[10],"#.#####.#.#.###.#.###.#####.#.###.#.###.#");
	sprintf(mazeMap[11],"#.#...#.#.#.....#.....#.....#...#.#.#...#");
	sprintf(mazeMap[12],"#.###.###.#.###########.#######.###.#.###");
	sprintf(mazeMap[13],"#.#...#...#.#.....#...#...#.....#...#.#.#");
	sprintf(mazeMap[14],"#.#.###.###.#.#.###.#.###.#.###.#.###.#.#");
	sprintf(mazeMap[15],"#.#.#...#3#...#.#...#.....#...#.#.#...#.#");
	sprintf(mazeMap[16],"#.#.#.###.#######.###########.###.#.###.#");
	sprintf(mazeMap[17],"#...#.#...#.#.....#...#...#.....#.....#.#");
	sprintf(mazeMap[18],"#####.#.###.#.#####.#.#.#.#.#.#.###.#.#.#");
	sprintf(mazeMap[19],"#.....#.....#.#...#.#.#.#.#.#.#...#.#.#.#");
	sprintf(mazeMap[20],"#.#####.#####.#.###.###.#.#.#####.#.#.#.#");
	sprintf(mazeMap[21],"#.....#.....#.#...#.....#...#.....#.#...#");
	sprintf(mazeMap[22],"#.#####.###.#.###.###.#######.#####.#####");
	sprintf(mazeMap[23],"#.#.......#...#2#...#...#...#.....#.....#");
	sprintf(mazeMap[24],"#.#.###########.###.#####.#.#####.###.###");
	sprintf(mazeMap[25],"#.......#.......#.#.....#.#.#...#...#...#");
	sprintf(mazeMap[26],"#######.#.#######.###.#.###.#.#.#.#####.#");
	sprintf(mazeMap[27],"#...#...#.....#...#...#...#...#.#.#...#.#");
	sprintf(mazeMap[28],"#.#.###.###.###.#.#######.###.#.#.#.#.#.#");
	sprintf(mazeMap[29],"#.#.....#.#...#.#.#.....#...#.#.#.#.#...#");
	sprintf(mazeMap[30],"#.#######.###.#.#.#.###.###.###.#.#.###.#");
	sprintf(mazeMap[31],"#...#...#.#...#.#.#...#.#...#...#...#...#");
	sprintf(mazeMap[32],"###.#.#.#.#.###.#.###.#.#.###.#######.###");
	sprintf(mazeMap[33],"#...#.#.#...#...#.....#.#.#...#.....#...#");
	sprintf(mazeMap[34],"#.#####.#.###.#.#######.#.#.###.#.#####.#");
	sprintf(mazeMap[35],"#.#.....#.#...#.....#...#.#.#.#.#.......#");
	sprintf(mazeMap[36],"#.#.###.#.#.#######.#.###.#.#.#.#########");
	sprintf(mazeMap[37],"#.#...#.#...#...#...#.#.....#...........#");
	sprintf(mazeMap[38],"#.###.#.#####.#.#.###.###.#############.#");
	sprintf(mazeMap[39],"#.....#.......#....4#.....#.............E");
	sprintf(mazeMap[40],"#########################################");
	mazePX=1;
	mazePY=1;
	drawMazeRefresh=500;
	blinkMazeC=0;
	blinkMazeG=0;
	gamesFinished=0;
	refreshStatus=1;
	game1Won=0;
	game2Won=0;
	game3Won=0;
	game4Won=0;
}
void drawMaze(void){
	int x,y;
	drawMazeRefresh--;
	if(drawMazeRefresh)return;
	drawMazeRefresh=500;
	mazeCX=mazePX-3;
	mazeCY=mazePY-3;
	if(mazeCX<0)mazeCX=0;
	if(mazeCY<0)mazeCY=0;
	if(mazeCX>33)mazeCX=33;
	if(mazeCY>33)mazeCY=33;
	for(x=0;x<8;x++)
	for(y=0;y<8;y++){
		if(mazeMap[y+mazeCY][x+mazeCX]=='.')
			setMatrixPixel(x,y,0);
		else if(mazeMap[y+mazeCY][x+mazeCX]=='#')
			setMatrixPixel(x,y,1);
		else if(mazeMap[y+mazeCY][x+mazeCX]=='E'){
			if(blinkMazeE>3)
				setMatrixPixel(x,y,1);
			else
				setMatrixPixel(x,y,0);
		}else if(blinkMazeG>1)
			setMatrixPixel(x,y,1);
		else
			setMatrixPixel(x,y,0);
	}
	blinkMazeE++;
	if(blinkMazeE==8)blinkMazeE=0;
	blinkMazeG++;
	if(blinkMazeG==4)blinkMazeG=0;
	blinkMazeC++;
	if(blinkMazeC==2){
		blinkMazeC=0;
		setMatrixPixel(mazePX-mazeCX,mazePY-mazeCY,1);
	}
}
int lastGame;
extern void displayMatrix(void);
extern void updateLCD(void);
extern void updateJoystick(void);
extern int playSpaceInvaders(void);
extern int PlayRaceGame(void);
extern int PlaySnakeGame(void);
extern int playTetris(void);
int playMaze(void){
	int refresh;
	refresh=2000;
	initMaze();
	while(1){
		refresh--;
		if(!refresh){
			refresh=2000;
			refreshStatus--;
			if(!refreshStatus){
				refreshStatus=100;
				if(gamesFinished==ALLGAMES)
					sprintf((char*)topLine," Go to the exit ");
				else
					sprintf((char*)topLine,"Super-duper maze");
				if(gamesFinished==ALLGAMES)sprintf((char*)botLine,"No games to go  ");
				else{
					if(ALLGAMES-gamesFinished==1)sprintf((char*)botLine,"1 game to go    ");
					else sprintf((char*)botLine,"%i games to go   ",ALLGAMES-gamesFinished);
				}
			}
			LCDTextOut(topLine,botLine);
			hasMoved=0;
			mazeMX=mazeMY=0;
			if(JOYSTICK_L&&!hasMoved){mazeMX=-1;hasMoved=1;}
			if(JOYSTICK_R&&!hasMoved){mazeMX=+1;hasMoved=1;}
			if(JOYSTICK_U&&!hasMoved){mazeMY=-1;hasMoved=1;}
			if(JOYSTICK_D&&!hasMoved){mazeMY=+1;hasMoved=1;}
			if(mazeMX!=0||mazeMY!=0){
				if(mazeMap[mazePY+mazeMY][mazePX+mazeMX]=='.'){
					mazePX+=mazeMX;
					mazePY+=mazeMY;
				}
				if(mazeMap[mazePY+mazeMY][mazePX+mazeMX]=='E'){
					if(gamesFinished==ALLGAMES)return 1;
					else{
						sprintf((char*)topLine,"Finish all games");
						sprintf((char*)botLine," before exiting ");
						refresh=100;
					}
				}
				if(mazeMap[mazePY+mazeMY][mazePX+mazeMX]=='1'&&!game1Won){
					lastGame=playSpaceInvaders();
					if(lastGame){
						gamesFinished++;
						sprintf((char*)topLine,"    You won     ");
						game1Won=1;
					}else{
						sprintf((char*)topLine,"    You lost    ");
					}
					sprintf((char*)botLine," Space invaders ");
					refreshStatus=30;
				}
				if(mazeMap[mazePY+mazeMY][mazePX+mazeMX]=='2'&&!game2Won){
					lastGame=PlayRaceGame();
					if(lastGame){
						gamesFinished++;
						sprintf((char*)topLine,"    You won     ");
						game2Won=1;
					}else{
						sprintf((char*)topLine,"    You lost    ");
					}
					sprintf((char*)botLine,"   Race game    ");
					refreshStatus=30;
				}
				if(mazeMap[mazePY+mazeMY][mazePX+mazeMX]=='3'&&!game3Won){
					lastGame=PlaySnakeGame();
					if(lastGame){
						gamesFinished++;
						sprintf((char*)topLine,"    You won     ");
						game3Won=1;
					}else{
						sprintf((char*)topLine,"    You lost    ");
					}
					sprintf((char*)botLine,"   Snake game   ");
					refreshStatus=30;
				}
				if(mazeMap[mazePY+mazeMY][mazePX+mazeMX]=='4'&&!game4Won){
					lastGame=playTetris();
					if(lastGame){
						gamesFinished++;
						sprintf((char*)topLine,"    You won     ");
						game4Won=1;
					}else{
						sprintf((char*)topLine,"    You lost    ");
					}
					sprintf((char*)botLine,"     Tetris     ");
					refreshStatus=30;
				}
			}			
		}
		drawMaze();
		updateJoystick();
		updateLCD();
		displayMatrix();
	}
};
