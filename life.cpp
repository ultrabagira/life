#include <iostream>
#include <winbgim.h>
#include <windows.h>

#define SCALE 7
#define WIDTH 1280
#define HEIGHT 640
#define LOW_WIDTH (int)(WIDTH/SCALE)
#define LOW_HEIGHT (int)(HEIGHT/SCALE)
#define NUM_CELLS 5000
#define LO(Word) (Word&0xff)
#define HI(Word) (Word>>8)
#define WORD(Hi,Lo) ((Hi<<8)|(Lo&0xff))


int Cells[LOW_WIDTH][LOW_HEIGHT];
int NewCells[LOW_WIDTH][LOW_HEIGHT];  // Word=Hi<<8 | Lo&0xff. Hi - Brightness 0..255, Lo - alive

void PixelRGB(int x, int y, int R, int G, int B){
	setfillstyle(1,COLOR(R,G,B));
	bar(x*SCALE+1,y*SCALE+1,(x+1)*SCALE,(y+1)*SCALE);
}


void Pixel(int x, int y){
	setfillstyle(1,COLOR(20,200,0));
	bar(x*SCALE+1,y*SCALE+1,(x+1)*SCALE,(y+1)*SCALE);
}

void PixelV(int x, int y, int Value){
	setfillstyle(1, COLOR(0,Value,0));
	bar(x*SCALE+1,y*SCALE+1,(x+1)*SCALE,(y+1)*SCALE);
}


void Setka(){
	int i,j;
	setcolor(RGB(30,30,30));
	for(j=0;j<HEIGHT/SCALE;j++)
		for(i=0;i<WIDTH/SCALE;i++){
			line(i*SCALE, j*SCALE, i*SCALE, (j+1)*SCALE-1);
			line(i*SCALE ,j*SCALE , (i+1)*SCALE , j*SCALE );
		}
}

void InitScene(){
int x,y;

	for(int k=0;k<NUM_CELLS;k++){
		x=rand()%(LOW_WIDTH-1);
		y=rand()%(LOW_HEIGHT-1);		
		Cells[x][y]=NewCells[x][y]=0xFF01;
	}
}

int Neighbors(int x, int y){
	int x1,y1,x3,y3;
	x1 = (x==0) ? (LOW_WIDTH-1) : (x-1) ;
	x3 = (x==LOW_WIDTH-1) ? 0 : (x+1);
	y1 = (y==0) ? (LOW_HEIGHT-1) : (y-1);
	y3 = (y==LOW_HEIGHT-1) ? 0 : (y+1);
	return 	LO(Cells[x1][y1]) + LO(Cells[x][y1]) + LO(Cells[x3][y1])+
			LO(Cells[x1][y]) + LO(Cells[x3][y]) +
			LO(Cells[x1][y3]) + LO(Cells[x][y3]) + LO(Cells[x3][y3]);
}


void UpdateSceneSimple(){
	for(int j=1;j<LOW_HEIGHT-1;j++)
		for(int i=1;i<LOW_WIDTH-1;i++){
			if ( LO(Cells[i][j])==1) {
				if (Neighbors(i,j)<2 || Neighbors(i,j)>3) 
					NewCells[i][j]=0;
				else NewCells[i][j]=Cells[i][j]; 
				}
			if(LO(Cells[i][j])==0 && Neighbors(i,j)==3) 
				NewCells[i][j]=WORD(0xff,1);
		}
	for(int j=0;j<LOW_HEIGHT;j++)
		for(int i=0;i<LOW_WIDTH;i++)
			Cells[i][j]=NewCells[i][j];

/*	for(int j=0;j<LOW_HEIGHT;j++)
		for(int i=0;i<LOW_WIDTH;i++)
			NewCells[i][j]=Cells[i][j];
*/
}

void UpdateSceneAge(){
	for(int j=0;j<LOW_HEIGHT;j++)
		for(int i=0;i<LOW_WIDTH;i++){
			if ( LO(Cells[i][j])==1) {
				if (Neighbors(i,j)<2 || Neighbors(i,j)>3) 
					NewCells[i][j]=WORD(0xff,0);
				else NewCells[i][j]=(HI(Cells[i][j])>1) ? WORD(HI(Cells[i][j])-1,1) : 0; 
				}
			if(LO(Cells[i][j])==0 && Neighbors(i,j)==3) 
				NewCells[i][j]=WORD(0xff,1);
		}
	for(int j=0;j<LOW_HEIGHT;j++)
		for(int i=0;i<LOW_WIDTH;i++)
			Cells[i][j]=NewCells[i][j]-(1<<8);

	for(int j=0;j<LOW_HEIGHT;j++)
		for(int i=0;i<LOW_WIDTH;i++)
			NewCells[i][j]=Cells[i][j];

}

void Draw(){
	cleardevice();

	for(int j=0;j<LOW_HEIGHT;j++)
		for(int i=0;i<LOW_WIDTH;i++)
		if (LO(Cells[i][j])) PixelRGB(i,j,0,HI(Cells[i][j]),0);
}

int main(){
	int c, page, play;
	initwindow(WIDTH,HEIGHT);
	page=0;
	play=1;
	
	InitScene();
	Draw();
	getch();
	while(play){
		if(kbhit() && getch()==27) play=0;
		setactivepage(page);
      	UpdateSceneAge();
		Draw();
		setvisualpage(page);
		page=1-page;
		Sleep(20);
	}

	closegraph();
	return 0;
}


