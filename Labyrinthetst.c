#include <time.h>

unsigned char field[50][24];
unsigned char passed[50][24];



void emptypassed(void)
{
 int x,y; 
 y=0;
 
 while ( y < 24)
 {
 x=0;
 while ( x < 50)
 {
    passed[x][y]=0;
   x++;
  }
  y++;
 }
}


int north,south,east,west;


void ffill(int x, int y)
  {
  if ( x==1||x==0 ){ west=1;}  
  if ( x==48||x==49){ east=1;}/* Gemini gab mir den gefaehrlichen Tip, die aeuessersten Raender mitzupruefen*/
  if ( y==1||y==0 ){ north=1;}
  if ( y==22||y==23){ south=1;}
 
if ( passed[x][y]==1) 
return;
passed[x][y]=1;
 
 if ( field[x+1][y]==0 &&passed[x+1][y]==0) ffill(x+1,y);
  if ( field[x-1][y]==0 &&passed[x-1][y]==0) ffill(x-1,y);
 if ( field[x][y-1]==0 &&passed[x][y-1]==0) ffill(x,y-1);
 if ( field[x][y+1]==0 &&passed[x][y+1]==0) ffill(x,y+1);

 return;

}



int main(void)
 {
 int x,y;
int runs=0;
start:
/*while (!kbhit())rand();*/getch();
 y=0;

 while ( y < 24 )
 {
  x=0;
  while( x < 50)
  {
   field[x][y]=0;
   field[x+1][y]=0;
   field[x][y+1]=0;
   field[x+1][y+1]=1;
   
   if ( x==0 ) field[x][y]=1,field[x][y+1]=1;
   if ( x==48) field[x+1][y]=1,field[x+1][y+1]=1;
   if ( y==22) field[x][y+1]=1,field[x+1][y+1]=1;
   if ( y==0) field[x][y]=1,field[x+1][y]=1;
   
   x+=2;
  }
  y+=2;
 }

 y=2;

 while ( y < 23 )

 {
  x=2;
  while( x < 48)
  { 
   switch(rand()%4)
   {
    case 0: field[x-1][y]=1; break;
    case 1: field[x][y-1]=1; break;
    case 2: field[x+1][y]=1; break;/*tatsaechlich hat die Leerung unten nochmal eine Luecke in die Raender gefuegt?!?*/
    case 3: field[x][y+1]=1; break;
   }
   
   x+=2;
  }
  y+=2;
 }


 y=1;

  while ( y<23)

  {
   x=1;

   while ( x < 49 )

   {
   	//if ( runs==3&&x==1&&y==1)printf("binda %d %d",runs,field[x][y]),getchar();
    if ( field[x][y]==0)

    {
     north=0, south=0, east=0, west=0;

     emptypassed();

   	//if ( runs==3&&x==1&&y==1)printf("binda"),getchar();
     ffill(x,y);
    if ( runs==3&&x==1&&y==1)printf("%d %d %d %d",north,east,south,west),getchar();
     if ( west==0||east==0||north==0||south==0)
     {
      int coordbuf[3][25];

      int n=0;
     
      do
      {

      //printf("%d %d\n",x,y);

       n=0;

       while ( n < 25){
                       coordbuf[0][n]=(rand()%47)+1,coordbuf[1][n]=(rand()%22)+1;
                       coordbuf[2][n]=field[coordbuf[0][n]][coordbuf[1][n]];
                         n++;
                        }

       n=0;

       while ( n < 25 ) field[coordbuf[0][n]][coordbuf[1][n]]=0,n++;

        north=0,south=0,east=0,west=0;

       emptypassed();

       ffill(x,y);

       if ( north==1&&south==1&&east==1&&west==1)break;

       n=0;

       while( n < 25 ) field[coordbuf[0][n]][coordbuf[1][n]]=coordbuf[2][n],n++;
  
      } while ( west==0||east==0||north==0||south==0);

     }

     }

     x++;

   }
 
   y++;

  }
  runs++;
  //goto start;
  #define UP 0 
  #define DOWN 1
  #define LEFT 2
  #define RIGHT 3
  #define REST 4
  
  int pl_x, pl_y;
  
  int goal_x=1,goal_y=rand()%22+1;
  
  while(field[goal_x][goal_y]!=0)goal_y=rand()%22+1;
  pl_x=48;
  pl_y=0;
  while ( field[pl_x][pl_y]!=0)pl_y++;
  
  clock_t delay;
  int direc=REST;
  while(1)
  {
    delay=clock()+500;
   if ( kbhit())
   {
   	switch(getch())
    {
     case 'u': if ( field[pl_x][pl_y-1]==0)direc=UP;break;
     case 'h': if ( field[pl_x-1][pl_y]==0)direc=LEFT; break;
     case 'j': if ( field[pl_x][pl_y+1]==0)direc=DOWN; break;
     case 'k': if ( field[pl_x+1][pl_y]==0)direc=RIGHT; break;
    	
	}
   	
   }
   if ( direc==UP)
   {
    field[pl_x][pl_y]=1;
    if ( field[pl_x][pl_y-1]==0)pl_y--;
   }
   else
   if ( direc==DOWN)
   {
    field[pl_x][pl_y]=1;
    if ( field[pl_x][pl_y+1]==0&&pl_y<23)pl_y++;
   }
   else
   if ( direc==LEFT)
   {
    field[pl_x][pl_y]=1;
    if ( field[pl_x-1][pl_y]==0&&pl_x>1)pl_x--;
   }
   else
   if ( direc==RIGHT)
   {
    field[pl_x][pl_y]=1;
    if ( field[pl_x+1][pl_y]==0&&pl_x<48)pl_x++;
   }
   system("cls");
  y=0;
  while ( y<24)

  {
   x=0;

   while ( x < 50 )

   {
     if ( x==goal_x&&y==goal_y)printf("G");else
     if ( x==pl_x&&y==pl_y)printf("O");else
    printf("%c",field[x][y]==0?' ' : '#');
    x++;
   }

   printf("\n");

   y++;

  }
  //goto start;
   if ( pl_x==goal_x&&pl_y==goal_y)break;
  while ( delay>clock());
 }

printf("\a");
}
