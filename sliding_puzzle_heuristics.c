#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define EMPTY -1

signed int field[5][5];

int move(int direc, int startpos)
{
 int x,y;
  int buf;
 if ( startpos>0)
 {
  y=startpos/5,x=startpos%5;
 }
 else
 {
  y=0;
  while ( y < 5 )
  {
   x=0;
   while ( x < 5 )
   {
    if ( field[x][y]==-1 ) break;
    x++;
   }
   if ( field[x][y]==-1 ) break;
   y++;
  }
 }


 if ( direc==UP)
 {
  if ( y > 0 ) { buf=field[x][y-1], field[x][y-1]=field[x][y], field[x][y]=buf;  
   return x+(y-1)*5; }else return -1;
 }
 else
 if ( direc==DOWN)
 {
  if ( y < 4 ) { buf=field[x][y+1], field[x][y+1]=field[x][y], field[x][y]=buf;
  return x+(y+1)*5; }else return -1;

 }
 else
 if ( direc==LEFT)
 {
  if ( x > 0 ){ buf=field[x-1][y], field[x-1][y]=field[x][y], field[x][y]=buf;
  return x-1+y*5;}else return -1;

 }
 else
 if ( direc==RIGHT)
 {
  if ( x < 4 ) { buf=field[x+1][y], field[x+1][y]=field[x][y], field[x][y]=buf;
   return x+1+y*5; }else return -1;
 }
}

int startpos=0;

void display_field(void)
{
 int x=0,y=0;	
 system("cls");
 y=0;
 while ( y < 5 )
 {
  x=0;
  while ( x < 5 )
  {
   printf("%c",field[x][y]);
   x++; 
  }
  printf("\n");
  y++;
 }
 printf("%d",startpos);
 //getchar();

}

int backtrack(int field[5][5], int rec_depth)
{
 int x=0,y=0;int startpos_old;
  int stepcount=0;
 
 if ( rec_depth>11)return;
  
  int prevval,success=1;
  prevval=0x41-1;
  
  y=0;
  while(y<5)
  {
   x=0;
   while ( x < 5)
   {
   	   if ( field[x][y]==prevval+1)prevval=field[x][y];
   	   else { if (field[x][y]!=-1){success=0; break;} }
     x++;
   }
   if ( success==0)break;
  	y++;
  }
    
    if ( field[0][0]=='M'&&field[1][0]=='I'&&field[2][0]=='K'&&field[2][0]=='A')display_field(),getchar();
  if ( success==1)printf("Geloest"),display_field(),getchar();
  
 // display_field();
  startpos_old=startpos;
//if ( rand()%4==0)
{
  startpos=move(UP,startpos);
  backtrack(field,rec_depth+1);
  if ( startpos!=-1)move(DOWN,startpos);
  //display_field();

  startpos=startpos_old;
}

//if ( rand()%4==0)
{
  startpos=move(DOWN,startpos);
  backtrack(field,rec_depth+1);
  if ( startpos!=-1)move(UP,startpos);
  //display_field();
  
   startpos=startpos_old;
}
//if (rand()%4==0)
 { 
  startpos=move(LEFT,startpos);
  backtrack(field,rec_depth+1);
  if ( startpos!=-1)move(RIGHT,startpos);
  //display_field();
  
  startpos=startpos_old;
 }
//if ( rand()%4==0)
 { 
  startpos=move(RIGHT,startpos);
  backtrack(field,rec_depth+1);
  if ( startpos!=-1)move(LEFT,startpos);
  //display_field();

  startpos=startpos_old;
 }
	
}

int shortest;
signed int pathfield[5][5];

int path_recurse(int x, int y, int goalx, int goaly, int depth)
{
  pathfield[x][y]=depth;
  if ( goalx==x&&goaly==y) shortest=depth;
  //printf("%d",shortest);
 if (x<4)if ( pathfield[x+1][y]> depth +1 &&pathfield[x+1][y]>=0 ) path_recurse(x+1,y,goalx,goaly,depth+1);
 if (x>0)if ( pathfield[x-1][y]> depth +1 &&pathfield[x-1][y]>=0 ) path_recurse(x-1,y,goalx,goaly,depth+1);
 if (y<4)if ( pathfield[x][y+1]> depth +1 &&pathfield[x][y+1]>=0 ) path_recurse(x,y+1,goalx,goaly,depth+1);
 if (y>0)if ( pathfield[x][y-1]> depth +1 &&pathfield[x][y-1]>=0 ) path_recurse(x,y-1,goalx,goaly,depth+1);
	
  return shortest;
}

void markback(int x, int y, int len)
{
	//printf("%d<<",len),getchar();
 if(x<4)if ( pathfield[x+1][y]==len-1){len>0 ? move(RIGHT,0) : 0,/*display_field(),*/markback(x+1,y,len-1);return;}
 if(x>0)if ( pathfield[x-1][y]==len-1){len>0 ? move(LEFT,0) : 0,/*display_field(),*/markback(x-1,y,len-1);return;}
 if(y<4)if ( pathfield[x][y+1]==len-1){len>0 ? move(DOWN,0) : 0,/*display_field(),*/markback(x,y+1,len-1);return;}
 if(y>0)if ( pathfield[x][y-1]==len-1){len>0 ? move(UP,0) : 0,/*display_field(),*/markback(x,y-1,len-1);return;}
}


int main(void)
{
 int x=0,y=0;

 y=0;
 while(y<5)
 {
  x=0;
   while ( x < 5 )
  {  
   field[x][y]=0x41+x+y*5;
   if ( x==4&&y==4 ) field[x][y]=-1;
   x++;
  }
  y++;
}

while(1)
{
  int n=200;
  
  while ( n > 0)
  {
  system("cls");
 y=0;
 while ( y < 5 )
 {
  x=0;
  while ( x < 5 )
  {
   printf("%c",field[x][y]);
   x++; 
  }
  printf("\n");
  y++;
 }

  startpos=move(rand()%4,0);
  n--;
 }

 
 x=0;
     while ( x < 25)pathfield[x%5][x/5]=999,x++;
 x=0;
 
 while (1)
 {
 	int goalx,goaly;
     x=0;
     while ( x < 25&& field[x%5][x/5]==0x41+x)pathfield[x%5][x/5]=-1,x++;
     int delx=0;
     delx=0;
     if ( rand()%4==0)while ( delx < 25 ){if ( delx%5==4||delx%5==3)pathfield[delx%5][delx/5]=999;delx++;}
 	  y=x;
      while ( y < 25)pathfield[y%5][y/5]=999,y++;
 
 	 goalx=0;
 	while ( field[goalx%5][goalx/5]!=x+0x41)goalx++;
 	
 	goaly=goalx/5;
 	goalx%=5;
 	pathfield[goalx][goaly]=-1;
 	if ( x%5< goalx)goalx--;
 	else
 	if( x%5> goalx)goalx++;
 	else
 	if(x/5<goaly)goaly--;
 	else
 	if(x/5>goaly)goaly++;
 	
    shortest=1000;
    int empty_x,empty_y;
    empty_y=0;
    while ( empty_y<5)
    {
     empty_x=0;
     while ( empty_x<5)
     {
      if ( field[empty_x][empty_y]==-1)break;
     	empty_x++;
	 }
	  if ( field[empty_x][empty_y]==-1)break;
     empty_y++;
	}
	//display_field();
	//printf("%d %d %d %d",goalx,goaly,empty_x,empty_y),getchar();
   path_recurse(goalx,goaly,empty_x,empty_y,0);
   if ( shortest<1000)markback(empty_x,empty_y,shortest); //else goto brut21;
  //printf("habe den Pfad markiert"),getchar();
 
 	 goalx=0;
 	while ( field[goalx%5][goalx/5]!=x+0x41)goalx++;
 	goaly=goalx/5;
 	goalx%=5;
 	
 
     //printf(">>%d %d %d %d<<\n",x%5,x/5,goalx,goaly);getchar();
 
   if ( x%5<goalx)move(RIGHT,0);//,display_field();
   else
   if(x%5>goalx)move(LEFT,0);//,display_field();
   else
   if(x/5<goaly)move(DOWN,0);//,display_field();
   else
   if(x/5>goaly)move(UP,0);//,display_field();
  
   //printf("x hat den Wert %d",x),getchar();
   
   brut21:
   	if (rand()%100==0)display_field();
  if ( shortest>=1000||rand()%13==0)backtrack(field,0);
  
 }
 
}


}
