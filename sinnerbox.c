#include <stdio.h>
#include <math.h>


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

#define SIN(val) sintab[(int)((val)*10) %64 +63]
#define COS(val) costab[(int)((val)*10) %64 +63]




unsigned char map[160][90];
unsigned char mapp[160][90];
unsigned char ffmap[160][90];


int ffill(int x,int y, int xend, int yend)
{
  if ( x==xend&&y==yend) return 0;
  
  if ( map[x][y]!=' ') return 1;
  
  ffmap[x][y]='X';
  
  if ( x < 17)
  if ( ffmap[x+1][y]!='X')
  if ( ffill(x+1,y,xend,yend)==0 ) return 0;
 
  if ( x > 1)
  if ( ffmap[x-1][y]!='X')
  if (ffill(x-1,y,xend,yend)==0 ) return 0;
  
  if ( y < 17)
   if ( ffmap[x][y+1]!='X')
  if ( ffill(x,y+1,xend,yend)==0) return 0;

  if ( y > 1)
   if ( ffmap[x][y-1]!='X')
  if ( ffill(x,y-1,xend,yend)==0 ) return 0;
  
  return 1;
}


int ffill_schneise(int x,int y, int xend, int yend)
{
  if ( x==xend&&y==yend) return 0;
  
  if ( mapp[x][y]!=' ' ) return 1;
  
  ffmap[x][y]='X';
  
  if ( x < 17)
  if ( ffmap[x+1][y]!='X')
  if ( ffill_schneise(x+1,y,xend,yend)==0 ) return 0;
 
  if ( x > 1)
  if ( ffmap[x-1][y]!='X')
  if (ffill_schneise(x-1,y,xend,yend)==0 ) return 0;
  
  if ( y < 17)
   if ( ffmap[x][y+1]!='X')
  if ( ffill_schneise(x,y+1,xend,yend)==0) return 0;

  if ( y > 1)
   if ( ffmap[x][y-1]!='X')
  if ( ffill_schneise(x,y-1,xend,yend)==0 ) return 0;
  
  return 1;
}


int xbeg, ybeg, abortp;

int mapgen( int xstart, int ystart, int xend, int yend, int recdepth )
{
	
// map[xstart][ystart]=' ';

  //if ( recdepth > 10) return;

  int x=0, y=0, show;
  unsigned int direc;
 
  
  if ( xstart==xend&&ystart==yend) { return 0; }
 show=rand()%10;
 if ( show==0)system("cls\n");
 y=0;
  while ( y < 86 )
  {
   x=0;
   while ( x < 160 )
   {
   	ffmap[x][y]=  ' ';
    ffmap[x+1][y]=' ';
    ffmap[x+2][y]=' ';
    ffmap[x+3][y]=' ';
    if ( show==0)printf("%c%c%c%c",map[x][y],map[x+1][y],map[x+2][y],map[x+3][y] );
    x+=4;
   }
   
   if ( show==0)printf("\n");

   y++;
  }
  //getch();  
  //if ( show==0) getch();
 
 
  if ( ffill(xstart,ystart, xend,yend)==1 ) 
     {
	    // map[xstart][ystart]=' ';

	    return 1;
	     
	   }
	
  
 // direc=rand()%4;
  
  	    
  if ( xstart!=xend || ystart!= yend) 
  {
  		
   if ( rand()%2==0&& map[xstart-1][ystart]==' ' && xstart > 1)
   {
   	/*
    map[xstart][ystart]='_';    
    
    
       if ( map[xstart-1][y]!='X' && map[xstart+1][y]!='X'&&
	        map[xstart][ystart-1]!='X' && map[xstart][ystart+1]!='X')
	   if ( map[xstart-1][y]!='_'&& map[xstart+1][ystart]!='_'
	        && map[xstart][ystart-1]!='|'&& map[xstart][ystart+1]!='|')
	  */
	      map[xstart][ystart]='X';
	      
    if ( mapgen(xstart-1,ystart,xend,yend, recdepth+1 )==1 )
	{
      map[xstart][ystart]=' ';
	 
	}
  }
  else
  if ( rand()%2==0&& map[xstart+1][ystart]==' ' && xstart < 18)
  {
  	/*
   map[xstart][ystart]='_';    
   
     
       if ( map[xstart-1][y]!='X' && map[xstart+1][y]!='X'&&
	        map[xstart][ystart-1]!='X' && map[xstart][ystart+1]!='X')
	   if ( map[xstart-1][y]!='_'&& map[xstart+1][ystart]!='_'
	        && map[xstart][ystart-1]!='|'&& map[xstart][ystart+1]!='|')
	  */
	      map[xstart][ystart]='X';
   
   if ( mapgen(xstart+1,ystart,xend,yend, recdepth+1 )==1 )
	{
     map[xstart][ystart]=' ';
      
   }
  }
  else
  if ( rand()%2==0&& map[xstart][ystart-1]==' ' && ystart > 1)
  {
  	
  	/*
   map[xstart][ystart]='|';
   
     
       if ( map[xstart-1][y]!='X' && map[xstart+1][y]!='X'&&
	        map[xstart][ystart-1]!='X' && map[xstart][ystart+1]!='X')
	   if ( map[xstart-1][y]!='_'&& map[xstart+1][ystart]!='_'
	        && map[xstart][ystart-1]!='|'&& map[xstart][ystart+1]!='|')
	  */
	      map[xstart][ystart]='X';
  	   
   if (mapgen(xstart,ystart-1,xend,yend, recdepth+1 )==1 )
	{
	 map[xstart][ystart]=' ';
	}
 }
 else
  if ( rand()%2==0&& map[xstart][ystart+1]==' ' && ystart < 18)
  {
  	/*
   map[xstart][ystart]='|';    

       if ( map[xstart-1][y]!='X' && map[xstart+1][y]!='X'&&
	        map[xstart][ystart-1]!='X' && map[xstart][ystart+1]!='X')
	   if ( map[xstart-1][y]!='_'&& map[xstart+1][ystart]!='_'
	        && map[xstart][ystart-1]!='|'&& map[xstart][ystart+1]!='|')
	      */	  
		  map[xstart][ystart]='X';
   if( mapgen(xstart,ystart+1,xend,yend, recdepth+1 )==1 )
	{
   	 map[xstart][ystart]=' ';
	}
 }

 if ( map[xstart][ystart]==' ' )
 mapgen(xstart,ystart,xend,yend, recdepth+1 );

}
  return 0;

  
}


int schneise( int xstart, int ystart, int xend, int yend, int recdepth )
{
	
// map[xstart][ystart]=' ';

  //if ( recdepth > 10) return;

  int x=0, y=0, c, show;
  unsigned int direc;
 
  
  if ( xstart==xend&&ystart==yend) { return 0; }
 show=rand()%10;
 if ( show==0)system("cls\n");
 y=0;
  while ( y < 86 )
  {
   x=0;
   while ( x < 160 )
   {
   	ffmap[x][y]=  ' ';
    ffmap[x+1][y]=' ';
    ffmap[x+2][y]=' ';
    ffmap[x+3][y]=' ';
    if ( show==0)printf("%c%c%c%c",mapp[x][y],mapp[x+1][y],mapp[x+2][y],mapp[x+3][y] );
    x+=4;
   }
   
   if ( show==0)printf("\n");

   y++;
  }
  //getch();  
  //if ( show==0) getch();
 
 
  if ( ffill_schneise(xstart,ystart, xend,yend)==1 ) 
     {
	    // map[xstart][ystart]=' ';

	    return 1;
	     
	   }
	
  
 // direc=rand()%4;
  
  	    
  if ( xstart!=xend || ystart!= yend) 
  {
  		
   if ( rand()%2==0&& 
     mapp[xstart-1][ystart]==' ' && xstart > 1)
   {
   	/*
    map[xstart][ystart]='_';    
    
    
       if ( map[xstart-1][y]!='X' && map[xstart+1][y]!='X'&&
	        map[xstart][ystart-1]!='X' && map[xstart][ystart+1]!='X')
	   if ( map[xstart-1][y]!='_'&& map[xstart+1][ystart]!='_'
	        && map[xstart][ystart-1]!='|'&& map[xstart][ystart+1]!='|')
	  */	  
	      mapp[xstart][ystart]='X';
	      
    if ( schneise(xstart-1,ystart,xend,yend, recdepth+1 )==1 )
	{
      mapp[xstart][ystart]=' ';
	 
	}
  }
  else
  if ( rand()%2==0&& mapp[xstart+1][ystart]==' ' && xstart < 18)
  {
  	/*
   map[xstart][ystart]='_';    
   
     
       if ( map[xstart-1][y]!='X' && map[xstart+1][y]!='X'&&
	        map[xstart][ystart-1]!='X' && map[xstart][ystart+1]!='X')
	   if ( map[xstart-1][y]!='_'&& map[xstart+1][ystart]!='_'
	        && map[xstart][ystart-1]!='|'&& map[xstart][ystart+1]!='|')
	  */
	      mapp[xstart][ystart]='X';
   
   if ( schneise(xstart+1,ystart,xend,yend, recdepth+1 )==1 )
	{
     mapp[xstart][ystart]=' ';
      
   }
  }
  else
  if ( rand()%2==0&& mapp[xstart][ystart-1]==' ' && ystart > 1)
  {
  	
  	/*
   map[xstart][ystart]='|';
   
     
       if ( map[xstart-1][y]!='X' && map[xstart+1][y]!='X'&&
	        map[xstart][ystart-1]!='X' && map[xstart][ystart+1]!='X')
	   if ( map[xstart-1][y]!='_'&& map[xstart+1][ystart]!='_'
	        && map[xstart][ystart-1]!='|'&& map[xstart][ystart+1]!='|')
	  */
	  
	       mapp[xstart][ystart]='X';
  	   
   if (schneise(xstart,ystart-1,xend,yend, recdepth+1 )==1 )
	{
	 mapp[xstart][ystart]=' ';
	}
 }
 else
  if ( rand()%2==0&& mapp[xstart][ystart+1]==' ' && ystart < 18)
  {
  	/*
   map[xstart][ystart]='|';    

       if ( map[xstart-1][y]!='X' && map[xstart+1][y]!='X'&&
	        map[xstart][ystart-1]!='X' && map[xstart][ystart+1]!='X')
	   if ( map[xstart-1][y]!='_'&& map[xstart+1][ystart]!='_'
	        && map[xstart][ystart-1]!='|'&& map[xstart][ystart+1]!='|')
	      */	  
		  mapp[xstart][ystart]='X';
   if( schneise(xstart,ystart+1,xend,yend, recdepth+1 )==1 )
	{
   	 mapp[xstart][ystart]=' ';
	}
 }

  //if ( xstart==xend && ystart==yend) map[xstart][ystart]='P';
 if ( mapp[xstart][ystart]==' ' )
 schneise(xstart,ystart,xend,yend, recdepth+1 );

}
  return 0;

  
}



int main(void)
{

int jump=0;
int punkte=100000;	
	
	float (*pixbuf)[4];
	
	(pixbuf)=malloc(sizeof(float)*1000000*4);
	
	unsigned long pbc;
  signed int x=0, y=0,z=0,n=0;
  signed int x_sp, y_sp;
  unsigned char sprite[400]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                             ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ','X',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ','X','X',' ',' ','X','X','X',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ','X','X','X','X','X','X',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ','X','X','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ','X',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X','X','X',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X','X',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ','X','X','X','X',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ','X','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ','X','X','X','X','X',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ','X','X','X','X','X','X','X',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ','X','X','X','X','X','X','X',' ',' ',' ',' ',' ',' ',' '};
  
  
  
  unsigned char spriteb[400]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                             ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ','X','X','X','X','X','X','X','X',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ','X','X','X','X','X','X','X','X',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ','X','X','X','X','X','X',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ','X','X','X','X',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' '};
							 
unsigned char spritec[400]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                             ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ','X','X','X','X',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ','X',' ',' ',' ',' ',' ',' ','X',' ',' ','X',' ',' ',' ',' ',' ',' ','X',' ',
							 ' ','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X',' ',
							 ' ','X',' ',' ',' ',' ',' ','X','X',' ',' ','X','X',' ',' ',' ',' ',' ','X',' ',
							 ' ','X',' ',' ',' ',' ',' ','X',' ',' ',' ','X','X',' ',' ',' ',' ',' ','X',' ',
							 ' ',' ','X','X',' ',' ','X','X',' ',' ',' ','X','X',' ',' ',' ','X','X',' ',' ',
							 ' ',' ',' ','X','X','X','X',' ',' ',' ',' ',' ','X','X','X','X','X',' ',' ',' ',
							 ' ',' ',' ',' ','X','X','X',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',' ','X','X',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ','X','X','X','X',' ',' ',' ',' ',' ','X','X','X',' ',' ',' ',' ',
							 ' ',' ',' ','X','X',' ','X','X',' ',' ',' ',' ',' ','X',' ','X','X',' ',' ',' ',
							 ' ',' ','X','X',' ',' ',' ','X',' ',' ',' ',' ','X',' ',' ',' ','X','X',' ',' ',
							 ' ',' ','X',' ',' ',' ',' ','X',' ',' ',' ',' ','X',' ',' ',' ',' ','X',' ',' ',
							 ' ','X','X',' ',' ',' ',' ','X','X',' ',' ','X','X',' ',' ',' ',' ',' ','X',' ',
							 'X','X','X','X','X','X','X','X','X',' ',' ','X','X','X','X','X','X','X','X',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ','X','X','X',' ',' ',' ',' ',' ',' ',' ',' ',
							 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
  
  
  unsigned char screen[160][120];
  float screen_z[160][120];

 while (! kbhit())rand();
  int 
    playerstartx=rand()%16+1,
    playerstarty=rand()%16+1;

  float xshift=playerstartx*20, yshift=12, zshift=playerstarty*20, y_angl=0.0;


   

	#define CAMX (xshift)
	#define CAMY (yshift)
	#define CAMZ (zshift)
	#define SCAL 120.0 // war 120
 
 
 
 
 
 
  y=0;
  while ( y < 19 )
  {
   x=0;
   while ( x < 19 )
   {
   	
   	
   	if ( (x==0&&y==0) || (x==0 && y==18)) map[x][y]='X', mapp[x][y]='X';
   	else
   	if ( (x==18&&y==18)|| (x==18 && y==0)) map[x][y]='X', mapp[x][y]='X';
	else
   	if ( x==0) map[x][y]='|', mapp[x][y]='|';// |
   	else
    if ( x==18) map[x][y]='|', mapp[x][y]='|'; // |
   	else
	if ( y==0) map[x][y]='_', mapp[x][y]='_'; // _
   	else
	if ( y==18) map[x][y]='_', mapp[x][y]='_'; // _
   	else 
	   mapp[x][y]=' ',map[x][y]=' '; 
    x++;
   }

   y++;
  }  

 
 
 	
 	abortp=0;
 	xbeg=rand()%16+1;  // war 5,5
 	ybeg=rand()%16+1;
   
   
   mapgen(xbeg,ybeg,playerstartx,playerstarty,0 ); // war 5,10
     // Durchrutsch-Glitch
  
    schneise(xbeg,ybeg,playerstartx,playerstarty,0);
    
	
	system("cls\n");
  y=0;
  while ( y < 19 )
  {
   x=0;
   while ( x < 19 )
   {
   	
   	
   	if ( x==0&&y==0 || x==0 && y==18) map[x][y]='X', mapp[x][y]='X';
   	else
   	if ( x==18&&y==18|| x==18 && y==0) map[x][y]='X', mapp[x][y]='X';
	else
   	{
	   if ( map[x][y]=='X' && mapp[x][y]=='X')map[x][y]= ' ';
    }
    printf("%c", map[x][y]);
  	x++;
   }

   printf("\n");
   y++;
  }  
	
	
	map[xbeg][ybeg]='G';  
    getch();
  
    /*
   x=0;
   while ( x < 17 ) map[x][0]='_',x++;
   x=0;
   while ( x < 17 ) map[x][16]='_',x++;
  
   y=0;
   while ( y < 17 ) map[0][y]='|',y++;
   y=0;
   while ( y < 17 ) map[16][y]='|',y++;
     */
    getch();    
   
   sininit(), cosinit();
 
 while (1)
 {
 	 x=0;
 while ( x < 160)
 {
  y=0;
  while ( y < 90)
 {
  screen[x][y]=' ';
  screen_z[x][y]=100000;
  y++;
 }
 x++;
 }
  pbc=0;
  y=0;
  int randcolor;
		
  while ( y < 400 )
  {
   x=0;
   while ( x < 400 )
   {
	   if (  map[(int)(x/20)][(int)(y/20)]== 'G' )
	   {
	   	x_sp=0;
	  while ( x_sp < 20)
	  {
	  	y_sp=0;
	  	while ( y_sp < 20)
	  {
	  	
	     if ( pbc >= 1000000)break;
	     pixbuf[pbc][0]=(x)+y_sp;
         pixbuf[pbc][1]=20; // <-> war hier y; zwei Untere waren vertauscht
         pixbuf[pbc][2]=y+x_sp;
         pixbuf[pbc][3]=spritec[y_sp+x_sp*20]!=' ' ? ' ' : 219; //sprite[y_sp+x_sp*20];
		 pbc++;
         if ( pbc >= 1000000)break;
		
		if ( spriteb[y_sp+x_sp*20]!=' ')
	  	{
	     pixbuf[pbc][0]=(x)+y_sp;
         pixbuf[pbc][1]=-40+x_sp; // <-> war hier y; zwei Untere waren vertauscht
         pixbuf[pbc][2]=y;
         pixbuf[pbc][3]='@';		
		  pbc++;
		
		   } 
         
        y_sp+=1;
        } 
	    x_sp+=1;
       }
	 }
	   // Glitch
	   if ( map[x/20][y/20]=='_')
   	{
   		 randcolor=(x/20 ) %2;
   	  x_sp=0;
	  while ( x_sp < 20)
	  {
	  	y_sp=0;
	  	while ( y_sp < 20)
	  {
	    if ( pbc >= 1000000)break;
	    pixbuf[pbc][0]=(x)+y_sp;
        pixbuf[pbc][1]=x_sp;
        pixbuf[pbc][2]=y;
        pixbuf[pbc][3]= randcolor==0? 219 : 177; //spritec[y_sp+x_sp*20];
        pbc++;
        if ( pbc >= 1000000)break;
          
        y_sp+=1;
        } 
	    x_sp+=1;
       }
	 }
	  else
	   if ( map[x/20][y/20]=='|')
   	{
   		randcolor=(x/20 ) %2;
   	  x_sp=0;
	  while ( x_sp < 20)
	  {
	  	y_sp=0;
	  	while ( y_sp < 20)
	  {
	    if ( pbc >= 1000000)break;
	    pixbuf[pbc][0]=x;
        pixbuf[pbc][1]=x_sp;
        pixbuf[pbc][2]=(y)+y_sp;
        pixbuf[pbc][3]= randcolor==0 ? 176 : '@'; //spriteb[y_sp+x_sp*20];
        pbc++;
        if ( pbc >= 1000000)break;
          
        y_sp+=1;
        } 
	    x_sp+=1;
       }
	 } else 
	   if ( map[x/20][y/20]=='X')
   	{
   		randcolor=(x/20 ) %2;
   	
   	  x_sp=0;
	  while ( x_sp < 20)
	  {
	  	y_sp=0;
	  	while ( y_sp < 20)
	  {
	    if ( pbc >= 1000000)break;
	    pixbuf[pbc][0]=(x)+y_sp;
        pixbuf[pbc][1]=x_sp;
        pixbuf[pbc][2]=y;
        pixbuf[pbc][3]=219; //sprite[x_sp+y_sp*20];
        pbc++;
        if ( pbc >= 1000000)break;
        
        
	    pixbuf[pbc][0]=x;
        pixbuf[pbc][1]=x_sp;
        pixbuf[pbc][2]=(y)+y_sp;
        pixbuf[pbc][3]=176; //sprite[x_sp+y_sp*20];
        pbc++;
        if ( pbc >= 1000000)break;
        
        
	    pixbuf[pbc][0]=(x)+y_sp;
        pixbuf[pbc][1]=x_sp;
        pixbuf[pbc][2]=y+20;
        pixbuf[pbc][3]=177; //sprite[x_sp+y_sp*20];
        pbc++;
        if ( pbc >= 1000000)break;
        
        
	    pixbuf[pbc][0]=x+20;
        pixbuf[pbc][1]=x_sp;
        pixbuf[pbc][2]=(y)+y_sp;
        pixbuf[pbc][3]='#'; //sprite[x_sp+y_sp*20];
        pbc++;
        
        if ( pbc >= 1000000)break;
        if ( fabs(y-zshift) > SCAL || fabs(x-xshift) > SCAL )
	    y_sp+=(fabs(y-zshift) > fabs(x-xshift) ? fabs(y-zshift)/SCAL : fabs(x-xshift)/SCAL);
        else y_sp++;
	    } 
	    if ( fabs(y-zshift) > SCAL || fabs(x-xshift) > SCAL )
	    x_sp+=(fabs(y-zshift) > fabs(x-xshift) ? fabs(y-zshift)/SCAL : fabs(x-xshift)/SCAL);
        else x_sp++;
	   }
	 }
     
     
     
	
	x+=20;
   }

   y+=20;
  }  


n=0;
while ( n < pbc)
{
  float x_turned, y_turned, z_turned;	
	  x_turned= +(pixbuf[(int)n][0]-xshift)*COS(y_angl)+(pixbuf[(int)n][2]-zshift)*SIN(y_angl);
       z_turned= -(pixbuf[(int)n][0]-xshift)*SIN(y_angl)+(pixbuf[(int)n][2]-zshift)*COS(y_angl);

         pixbuf[(int)n][0]=x_turned+xshift;
        pixbuf[(int)n][2]=z_turned+zshift;
 n++;
}


n=0;
while ( n < pbc && n < 1000000)
{
	if ( (float)pixbuf[n][2]-(float)zshift > 0)
	{
	if ( ((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-zshift) +80)/1> 0 && ((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-zshift) +80 )/1 < 160 && ((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-zshift) +40 )/1> 0 && ((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-zshift) +40)/1 < 90)
	
	if( screen_z[((int)(((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-zshift)))+80)/1][((int)(((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-zshift)))+40)/1  ]>pixbuf[n][2])
    screen[((int)(((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-zshift)))+80)/1][((int)(((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-zshift))) +40)/1]=pixbuf[n][3],\
    screen_z[((int)(((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-zshift)))+80)/1][((int)(((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-zshift))) +40)/1]=pixbuf[n][2];
    signed int xb,yb, xend, yend;
    
    if ( pixbuf[n][2]-zshift < SCAL )
    {
    yb=0;
    xb=0;
     xend=SCAL/((pixbuf[n][2]-zshift) < 2 ? 2 : (pixbuf[n][2]-zshift));
	 yend=SCAL/((pixbuf[n][2]-zshift) < 2 ? 2 : (pixbuf[n][2]-zshift)); 
    
    signed int xbbuf, ybbuf;
    xbbuf=-xend;
    ybbuf=-yend;
    
    /*
     if ( xend > 100) xend=100;
     if ( yend > 100) yend=100;
    */
    
    signed int xpos, ypos;
    xpos=((int)(((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-zshift)))+80)/1;
    ypos=((int)(((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-zshift)))+40)/1;
   if ( xpos > -200 && xpos < 320 && ypos > -250 && ypos < 290)
   {    
    
    yb=ybbuf;
    while ( yb < yend)
    {
    	xb=xbbuf;
    	while ( xb < xend)
    	{
 	if ( (int)xb+xpos>= 0 && (int)xb+xpos< 160 && (int)yb+ypos> 0 && (int)yb+ypos < 90)
	   if( screen_z[(int)xb+xpos][(int)yb+ypos  ]>pixbuf[n][2])
      screen[(int)xb+xpos][(int)yb+ypos]=pixbuf[n][3],\
     screen_z[(int)xb+xpos][(int)yb+ypos]=pixbuf[n][2];
	 
	   xb++;
        }
    
     yb++;
    }
 
    }
    
     
     }
    
  }
 
 n++;
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

  if ( c=='s') { printf("X:%f Z:%f %d %f %f", xshift, zshift,pbc, SIN(y_angl), COS(y_angl)/* map[(int)((xshift)/20)][(int)((zshift)/20)] */), getch(); }
   if(c=='u' )
   {
       xshift-= -2*SIN(-y_angl), zshift-= -2*COS(-y_angl);
   	
	   if ( xshift < 400 && xshift >= 0 && zshift >= 0 && zshift < 400 )
       if ( 
	        (
	        map[(int)(xshift/20)][(int)(zshift/20)]=='|'
	        && fabs ( (int)xshift%20 )  < 4 ) ||
			
	        (
	        map[(int)(xshift/20)][(int)(zshift/20)]=='_'
	        && fabs ( (int)zshift%20 )  < 4 ) ||
			
			(
	        map[(int)((xshift)/20)][(int)((zshift)/20)]=='X' )
			)
       xshift+= -2*SIN(-y_angl), zshift+= -2*COS(-y_angl);
	   else
       if ( map[(int)(xshift/20)][(int)(zshift/20)]=='G' ) printf("\a %d Punkte", punkte), getch();
     
	  }
   if(c=='j' )
   {
    xshift+= -2*SIN(-y_angl), zshift+= -2*COS(-y_angl);
   	if ( xshift < 400 && xshift >= 0 && zshift >= 0 && zshift < 400 )
	   if (
	        (
	        map[(int)((xshift)/20)][(int)(zshift/20)]=='|'
	        && fabs ( (int)xshift%20 )  < 4 ) ||
			
	        (
	        map[(int)((xshift)/20)][(int)((zshift)/20)]=='_'
	        && fabs ( (int)zshift%20 )  < 4   ) ||
			
			(
	        map[(int)((xshift)/20)][(int)((zshift)/20)]=='X' )
			
			)
    xshift-= -2*SIN(-y_angl), zshift-= -2*COS(-y_angl);
    else
       if ( map[(int)(xshift/20)][(int)(zshift/20)]=='G' ) printf("\a %d Punkte!!!", punkte), getch();
     
   }
   if(c=='h'  ) y_angl+=0.04;
   if(c=='k' ) y_angl-=0.04;
   if(c=='e') if ( punkte > 100 && jump <=0 )jump=14, punkte-=100, printf("Sprung kostet 100 Punkte!\n"); // war 2.0
   //if(c=='d') yshift+=2.0; 
if ( c=='p' ) getch();
 if ( c=='x') break;
}
if ( jump > 0 && yshift > -16) printf("Sprung kostet 100 Punkte!\n"),yshift-=4.0, jump--;
if ( yshift < 12) yshift++;
printf("%d Punkte", punkte--);
if ( punkte ==0) return;

}

}
