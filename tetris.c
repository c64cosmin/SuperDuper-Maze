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
//Tetris
#define TETRIS_REFRESH 6000
#define TETRIS_MOVE_REFRESH 2000
int tetrisRefresh;
int tetrisMoveRefresh;
extern unsigned char topLine[];
extern unsigned char botLine[];
int pieceX;
int pieceY;
int scrollY;
char pieceRotate;
char pieceType;
#define LINE_COUNT 20
char lineCount;
char tetrisWON;
char tetrisLOST;
char tetrisMap[16][16];
char pieces[28][28];

void getNewPiece(){
	pieceType=rand()%7;
	pieceRotate=0;
	pieceX=2;
	pieceY=-4;
}

void initTetris(void){
	int i,j;
	lineCount=0;
	tetrisWON=0;
	tetrisLOST=0;
	
	sprintf(pieces[ 0],".#..............");
	sprintf(pieces[ 1],".#...###.##...#.");
	sprintf(pieces[ 2],".##..#....#.###.");
	sprintf(pieces[ 3],"..........#.....");

	sprintf(pieces[ 4],"..#.............");
	sprintf(pieces[ 5],"..#..#...##.###.");
	sprintf(pieces[ 6],".##..###.#....#.");
	sprintf(pieces[ 7],".........#......");
		
	sprintf(pieces[ 8],"..#......#......");
	sprintf(pieces[ 9],"..#......#..####");
	sprintf(pieces[10],"..#.####.#......");
	sprintf(pieces[11],"..#......#......");
		
	sprintf(pieces[12],".............#..");
	sprintf(pieces[13],".###..#..#...##.");
	sprintf(pieces[14],"..#..##.###..#..");
	sprintf(pieces[15],"......#.........");

	sprintf(pieces[16],"................");
	sprintf(pieces[17],".##..##..##..##.");
	sprintf(pieces[18],".##..##..##..##.");
	sprintf(pieces[19],"................");
		
	sprintf(pieces[20],".............#..");
	sprintf(pieces[21],"..##.#...##..##.");
	sprintf(pieces[22],".##..##.##....#.");
	sprintf(pieces[23],"......#.........");

	sprintf(pieces[24],"..............#.");
	sprintf(pieces[25],".##...#.##...##.");
	sprintf(pieces[26],"..##.##..##..#..");
	sprintf(pieces[27],".....#..........");
	
	pieceRotate=0;
	pieceType=0;
	tetrisRefresh=TETRIS_REFRESH;
	tetrisMoveRefresh=TETRIS_MOVE_REFRESH;
	
	for(i=0;i<16;i++)
	for(j=0;j<8;j++)
		tetrisMap[i][j]=0;
	clearMatrix();
	sprintf((char*)topLine,"Prepare to play ");
	sprintf((char*)botLine,"     Tetris     ");
	LCDTextOut(topLine,botLine);
	for(j=0;j<10;j++)
	for(i=0;i<100000;i++)
		updateLCD();
		
	getNewPiece();
}
int getPieceCell(int x,int y){
	int ox,oy;
	ox=pieceRotate*4;
	oy=pieceType*4;
	if(pieces[y+oy][x+ox]=='.')return 0;
	if(pieces[y+oy][x+ox]=='#')return 1;
	return 0;
}

void drawPoint(int x,int y,char v){
	if(x>=0&&x<8&&y>=0&&y<16&&v)
		setMatrixPixel(x,y,1);
}
void putPoint(int x,int y,char v){
	if(x>=0&&x<8&&y>=0&&y<16&&v)
		tetrisMap[y][x]=1;
}
void drawPiece(){
	int i,j;
	for(i=0;i<4;i++)
	for(j=0;j<4;j++)
		drawPoint(pieceX+j,pieceY+i-scrollY,getPieceCell(j,i));
}
void putPiece(){
	int i,j;
	for(i=0;i<4;i++)
	for(j=0;j<4;j++)
		putPoint(pieceX+j,pieceY+i,getPieceCell(j,i));
}
int testMap(int x,int y){
	if(x<0)return 1;
	if(x>=8)return 1;
	if(y<0)return 0;
	if(x>=0&&x<8&&y<16)
		return tetrisMap[y][x];
	return 1;
}
int testMove(int x,int y){
	int i,j;
	for(i=0;i<4;i++)
	for(j=0;j<4;j++)
		if(getPieceCell(j,i))
		if(testMap(pieceX+j+x,pieceY+i+y))
			 return 0;
	return 1;
}
void removeLines(){
	int i,j,k;
	char d;
	for(i=15;i>0;i--){
		d=1;
		for(j=0;j<8;j++)
			if(tetrisMap[i][j]==0)d=0;
		if(d){
			for(k=i;k>0;k--)
			for(j=0;j<8;j++)
				tetrisMap[k][j]=tetrisMap[k-1][j];
			for(j=0;j<8;j++)
				tetrisMap[0][j]=0;
			i++;
			lineCount++;
		}
	}
}
void updateTetris(void){
	char m;
	char i,j;
	tetrisMoveRefresh--;
	if(!tetrisMoveRefresh){
		tetrisMoveRefresh=TETRIS_MOVE_REFRESH;
		m=0;
		if(!m&&JOYSTICK_L){
			m=1;
			if(testMove(-1,0))pieceX--;
		}
		if(!m&&JOYSTICK_R){
			m=1;
			if(testMove(1,0))pieceX++;
		}
		if(!m&&JOYSTICK_D){
			m=1;
			if(testMove(0,1))pieceY++;
			else{
				putPiece();
				getNewPiece();
			}
		}
		if(!m&&(JOYSTICK_U||JOYSTICK_A||JOYSTICK_F)){
			m=1;
			pieceRotate=(pieceRotate+1)%4;
		}
		if(pieceY>scrollY)scrollY++;
		if(pieceY<scrollY)scrollY--;
		if(scrollY<0)scrollY=0;
		if(scrollY>8)scrollY=8;
		for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			setMatrixPixel(j,i,tetrisMap[i+scrollY][j]);
		drawPiece();
	}
	tetrisRefresh--;
	if(!tetrisRefresh){
		tetrisRefresh=TETRIS_REFRESH;
		if(testMove(0,1))pieceY++;
		else{
			putPiece();
			if(pieceY<0)tetrisLOST=1;
			getNewPiece();
		}
		removeLines();
		LCDTextOut(topLine,botLine);
		sprintf((char*)topLine,"     Tetris     ");
		if(LINE_COUNT-lineCount==1)sprintf((char*)botLine," 1 line to go   ");
		else sprintf((char*)botLine,"%2i lines to go  ",LINE_COUNT-lineCount);
	}
}

int playTetris(){
	initTetris();
	while(1){
		updateTetris();
		displayMatrix();
		updateJoystick();
		updateLCD();
		if(lineCount>=LINE_COUNT)tetrisWON=1;
		if(tetrisWON)return 1;
		if(tetrisLOST)return 0;
	}
}
