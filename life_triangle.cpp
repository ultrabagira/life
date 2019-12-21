#include <winbgim.h>
#include <windows.h>

#define SCR_WIDTH 1200
#define SCR_HEIGHT 700
#define SCALE 8
#define LOW_WIDTH SCR_WIDTH/SCALE*2
#define LOW_HEIGHT SCR_HEIGHT/SCALE
#define NUM_BOTS 8000
#define TAN30 0.5
#define DELAY 80

int PLAY=1;

unsigned char World[LOW_WIDTH][LOW_HEIGHT];
unsigned char NextWorld[LOW_WIDTH][LOW_HEIGHT];

void Tri(int x, int y, int color=255){   // x,y - коорд. левой вершины, size-ширина, dir=0 - вверх, dir=1 - вниз
	int poly[6], x1, y1;
	
	
	if (color<255) setfillstyle(1,COLOR(0,color,0));
	
	x1=(x*SCALE)>>1;	
	y1=(y + ((x+y)&1))*SCALE;
	
	poly[0]=x1;
	poly[1]=y1;

	poly[2]=x1+(SCALE>>1);
	poly[3]=((x+y+1)&1) ? y1+SCALE : y1-SCALE;

	poly[4]=x1+SCALE;
	poly[5]=y1;

	fillpoly(3,poly);
}

void Pixel1(int x, int y){
		Tri( (x*SCALE)>>1 , (y + (x+y)%2)*SCALE );	
}


void Setka(){
	for (int i=-LOW_HEIGHT/2;i<LOW_WIDTH/2+LOW_HEIGHT/2;i++){
		line(i*SCALE-SCALE/2,0,i*SCALE-SCR_HEIGHT*TAN30-SCALE/2,SCR_HEIGHT);			
		line(i*SCALE-SCALE/2,0,i*SCALE+SCR_HEIGHT*TAN30-SCALE/2,SCR_HEIGHT);			
	}
	for (int j=0;j<LOW_HEIGHT;j++){
		line(0,j*SCALE,SCR_WIDTH,j*SCALE);
 	}
}

void Draw(){
	cleardevice();
	setfillstyle(1, COLOR(0,255,0));
	for(int j=0;j<LOW_HEIGHT;j++)
	for(int i=0;i<LOW_WIDTH;i++)
		if (World[i][j]!=0) Tri(i,j, World[i][j]);
		//Pixel1(i,j);
}

int Neighbors(int x, int y){
	int x0,x1,y1,x3,y3,x4, add;
	x0=(x-2+LOW_WIDTH)%(LOW_WIDTH);
	x1=(x-1+LOW_WIDTH)%(LOW_WIDTH);
	x3=(x+1)%(LOW_WIDTH);
	x4=(x+2)%(LOW_WIDTH);
	y1=(y-1+LOW_HEIGHT)%(LOW_HEIGHT);
	y3=(y+1)%(LOW_HEIGHT);
	
	add=((x+y)&1) ?  ((World[x0][y3]!=0)+(World[x4][y3]!=0)) : ((World[x0][y1]!=0)+(World[x4][y1]!=0));
	
	return 	(World[x1][y1]!=0)+(World[x][y1]!=0)+(World[x3][y1]!=0)+
			(World[x0][y]!=0)+(World[x1][y]!=0)+(World[x3][y]!=0)+(World[x4][y]!=0)+
			(World[x1][y3]!=0)+(World[x][y3]!=0)+(World[x3][y3]!=0)+add;	
	
}

void UpdateScene(){
	int N;
	for(int j=0;j<LOW_HEIGHT;j++)
	for(int i=0;i<LOW_WIDTH;i++){
		N=Neighbors(i,j);
		if (World[i][j]) { 	// если живой
			if (N<2 || N>3) NextWorld[i][j]=0; else NextWorld[i][j]=World[i][j]-1; // соседей <2 или >3 - умрет
		}
		else{				// если мертвый
			if (N==3) NextWorld[i][j]=0xFF;
		}
	}
	for(int j=0;j<LOW_HEIGHT;j++)
	for(int i=0;i<LOW_WIDTH;i++)
		World[i][j]=NextWorld[i][j];
}

void InitScene(){
	int x, y;
	setcolor(COLOR(0,50,0));
	setfillstyle(1, COLOR(0,180,0));
	for(int k=0;k<NUM_BOTS;k++){
		x=rand()%(LOW_WIDTH);
		y=rand()%(LOW_HEIGHT);
		World[x][y]=NextWorld[x][y]=0xFF;
		
	}
}

void Keys(){
	switch(getch()){
		case 27: PLAY=0; break;
		default: break;
	}
}

int main(){
	int page=0;
	
	initwindow(SCR_WIDTH,SCR_HEIGHT);
	InitScene();

	Draw();
	getch();
	
	while(PLAY){
		if(kbhit()) Keys();
		setactivepage(page);
		UpdateScene();	
		Draw();
		setvisualpage(page);
		page=1-page;
		Sleep(DELAY);
	}
	

	
	closegraph();
	
	return 0;
}
