 float sintab[128];
float costab[128];

void sininit(void)
{
 float value;
 value=-6.4;

 while ( value < 6.4 )
 {
  sintab[(int)((value+6.4)*10.0)]=sin(value);
  value+=0.1;
 }

}

void cosinit(void)
{
 float value;
 value=-6.4;

 while ( value < 6.4 )
 {
  costab[(int)((value+6.4)*10.0)]=cos(value);
  value+=0.1;
 }

}

#define SIN(val) sintab[(int)(val*10) %64 +63]
#define COS(val) costab[(int)(val*10) %64 +63]



enum
{
 top,
 bottom,
 left,
 right,
 back,
 front
}side;


unsigned char surface[6][9];

void mirror(int side, int row)
{
 unsigned char swap;
 swap=surface[side][row*3];
 surface[side][row*3]=surface[side][row*3+2];
 surface[side][row*3+2]=swap;
}

void mirrorh(int side, int row)
{
 unsigned char swap;
 swap=surface[side][0+row];
 surface[side][0+row]=surface[side][6+row];
 surface[side][6+row]=swap;	
	
}

void swap(int sidea, int sideb, int sidec, int sided, int row)
{
 unsigned char swap[3];
swap[0]=surface[sidea][0+row*3],swap[1]=surface[sidea][1+row*3],swap[2]=surface[sidea][2+row*3]; 
surface[sidea][row*3]=surface[sideb][row*3],surface[sidea][row*3+1]=surface[sideb][row*3+1],surface[sidea][row*3+2]=surface[sideb][row*3+2];
surface[sideb][row*3]=surface[sidec][row*3],surface[sideb][row*3+1]=surface[sidec][row*3+1],surface[sideb][row*3+2]=surface[sidec][row*3+2];
surface[sidec][row*3]=surface[sided][row*3],surface[sidec][row*3+1]=surface[sided][row*3+1],surface[sidec][row*3+2]=surface[sided][row*3+2];
 surface[sided][0+row*3]=swap[0],surface[sided][1+row*3]=swap[1],surface[sided][2+row*3]=swap[2];
mirror(sideb,row);
mirror(sided,row);
}

void swaph(int sidea, int sideb, int sidec, int sided, int row)
{
 unsigned char swap[3];
swap[0]=surface[sidea][0+row],swap[1]=surface[sidea][3+row],swap[2]=surface[sidea][6+row]; 
surface[sidea][0+row]=surface[sideb][0+row],surface[sidea][3+row]=surface[sideb][3+row],surface[sidea][6+row]=surface[sideb][6+row];
surface[sideb][0+row]=surface[sidec][0+row],surface[sideb][3+row]=surface[sidec][3+row],surface[sideb][6+row]=surface[sidec][6+row];
surface[sidec][0+row]=surface[sided][0+row],surface[sidec][3+row]=surface[sided][3+row],surface[sidec][6+row]=surface[sided][6+row];
 surface[sided][0+row]=swap[0],surface[sided][3+row]=swap[1],surface[sided][6+row]=swap[2];
mirrorh(sideb,row);
mirrorh(sided,row);

}


void rotate90(int side)
{
 unsigned char swap[3];
 swap[0]=surface[side][0],swap[1]=surface[side][1],swap[2]=surface[side][2];
 surface[side][0]=surface[side][2],surface[side][1]=surface[side][3+2],surface[side][2]=surface[side][6+2];
 surface[side][3+2]=surface[side][6+1],surface[side][6+1]=surface[side][3+0],surface[side][6+2]=surface[side][6+0];
 surface[side][0]=swap[2],surface[side][3+0]=swap[1],surface[side][6]=swap[0];	
}

void init (int side, unsigned char col)
{
 surface[side][0]=col,surface[side][1]=col,surface[side][2]=col,
 surface[side][3]=col,surface[side][4]=col,surface[side][5]=col,
 surface[side][6]=col,surface[side][7]=col,surface[side][8]=col;
}

int main(void)
{
 init(top,0),init(bottom,1),init(left,2),init(right,3),init(front,4),init(back,5); 


float pixbuf[100000][4];
int pbc;
sininit(),cosinit();

#define XMIDDLE 10
#define ZMIDDLE 160
#define YMIDDLE 50
int x,y;
unsigned char screen[160][90];
int screen_z[160][90];
int n=0;
float y_angl=0.0;
float x_angl=0.0;

while (1)
{
 pbc=0;
 y=0;
 while ( y < 60)
 {
  x=0;
  while ( x < 60)
  {
   pixbuf[pbc][0]=x/3,pixbuf[pbc][1]=60,pixbuf[pbc][2]=y/3+150,pixbuf[pbc][3]=surface[top][(x/20)+(y/20)*3];
   x++;
   pbc++;
  }
  y++;
 }

 y=0;
 while ( y < 60)
 {
  x=0;
  while ( x < 60)
  {
   pixbuf[pbc][0]=0,pixbuf[pbc][1]=x/3+40,pixbuf[pbc][2]=y/3+150,pixbuf[pbc][3]=surface[left][(x/20)+(y/20)*3];
   x++;
   pbc++;
  }
  y++;
 }

y=0;
 while ( y < 60)
 {
  x=0;
  while ( x < 60)
  {
   pixbuf[pbc][0]=20,pixbuf[pbc][1]=x/3+40,pixbuf[pbc][2]=y/3+150,pixbuf[pbc][3]=surface[right][(x/20)+(y/20)*3];
   x++;
   pbc++;
  }
  y++;
 }

y=0;
 while ( y < 60)
 {
  x=0;
  while ( x < 60)
  {
   pixbuf[pbc][0]=x/3,pixbuf[pbc][1]=40,pixbuf[pbc][2]=y/3+150,pixbuf[pbc][3]=surface[bottom][(x/20)+(y/20)*3];
   x++;
   pbc++;
  }
  y++;
 }

y=0;
 while ( y < 60)
 {
  x=0;
  while ( x < 60)
  {
   pixbuf[pbc][0]=x/3,pixbuf[pbc][1]=40+y/3,pixbuf[pbc][2]=150,pixbuf[pbc][3]=surface[front][(x/20)+(y/20)*3];
   x++;
   pbc++;
  }
  y++;
 }

y=0;
 while ( y < 60)
 {
  x=0;
  while ( x < 60)
  {
   pixbuf[pbc][0]=x/3,pixbuf[pbc][1]=40+y/3,pixbuf[pbc][2]=170,pixbuf[pbc][3]=surface[back][(x/20)+(y/20)*3];
   x++;
   pbc++;
  }
  y++;
 }


x=0;
while ( x < 160)
{
 y=0;
 while ( y < 90)
{
 screen[x][y]=0;
 screen_z[x][y]=100000;
 y++;
}
x++;
}

n=0;
while ( n < pbc)
{
  float x_turned, y_turned, z_turned;	
	  x_turned= +(pixbuf[(int)n][0]-XMIDDLE)*COS(y_angl)+(pixbuf[(int)n][2]-ZMIDDLE)*SIN(y_angl);
       z_turned= -(pixbuf[(int)n][0]-XMIDDLE)*SIN(y_angl)+(pixbuf[(int)n][2]-ZMIDDLE)*COS(y_angl);

         pixbuf[(int)n][0]=x_turned+XMIDDLE;
         
		pixbuf[(int)n][2]=z_turned+ZMIDDLE;
 
 	  z_turned= +(pixbuf[(int)n][0]-XMIDDLE)*SIN(x_angl)+(pixbuf[(int)n][2]-ZMIDDLE)*COS(x_angl);
       y_turned= -(pixbuf[(int)n][1]-YMIDDLE)*COS(x_angl)-(pixbuf[(int)n][2]-ZMIDDLE)*SIN(x_angl);

         pixbuf[(int)n][1]=y_turned+YMIDDLE;
         
		pixbuf[(int)n][2]=z_turned+ZMIDDLE;
 
 
 n++;
}


	#define CAMX (50)
	#define CAMY (50)
	#define CAMZ (0)
	#define SCAL 120.0 // war 120.0 war 60
	
n=0;
while ( n < pbc && n < 100000)
{
	if ( pixbuf[n][2] > 0)
	if ( (pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-CAMZ) +80> 4 && (pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-CAMZ) +80 < 156 && (pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-CAMZ) +40 > 4 && (pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-CAMZ) +40 < 85)
	if( screen_z[(int)(((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-CAMZ)))+80][(int)(((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-CAMZ)))+40  ]>pixbuf[n][2])\
 screen[(int)(((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-CAMZ)))+80][(int)(((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-CAMZ))) +40]=pixbuf[n][3],\
 screen_z[(int)(((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-CAMZ)))+80][(int)(((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-CAMZ))) +40]=pixbuf[n][2];
 n++;
}


 
x=0;
while ( x < 160)
{
 y=0;
 while ( y < 90)
{
 if ( screen[x][y]==0) screen[x][y]=' ';
 else
 if ( screen[x][y]==1) screen[x][y]='.';
 else
 if ( screen[x][y]==2) screen[x][y]='*';
 else
 if ( screen[x][y]==3) screen[x][y]=176;
 else
 if ( screen[x][y]==4) screen[x][y]=177;
 else
 screen[x][y]=219;
 
 y++;
}
x++;
}


system("cls\n");
  y=0;
 while ( y < 88)
 {
  x=0;
  while ( x < 158)
 {
  printf("%c%c%c%c%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y],screen[x+4][y],screen[x+5][y],screen[x+6][y],screen[x+7][y]);
  x+=8;
 }
 printf("\n");
 y++;
 }

if ( kbhit()) { unsigned char c; c=getch();
if ( c=='k')y_angl+=0.1;
if ( c=='h')y_angl-=0.1;
if (c=='u') x_angl-=0.1;
if (c=='j') x_angl+=0.1;
if ( c=='1') swap(top,left,bottom,right,2),rotate90(back);
if ( c=='2') swap(top,left,bottom,right,1);
if ( c=='3') swap(top,left,bottom,right,0),rotate90(front);
if ( c=='4') swaph(back,bottom,front,top,2),rotate90(right);
if ( c=='5') swaph(top,front,bottom,back,1);
if ( c=='6') swaph(top,front,bottom,back,0),rotate90(left);
if (c=='7')rotate90(top);
if(c=='8')swap(front,right,back,left,1);
if(c=='9')rotate90(bottom);}

}

 
}
