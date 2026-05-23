#include <stdio.h>
#include <stdlib.h>

typedef
struct
{
 int size;
 int horv;
 int x,y;
 int isred;
}car;

car carsatstart[6];

int depth_solved=9999999;

int collide(car *a, car *b)
{
 if ( (a->x + a->size > 8&&a->horv==0) || a->x < 0 || a->y<0||
      (a->y + a->size > 8 && a->horv==1 ) ) return 1;
  if ( a->horv==0&&b->horv==0)
  {
  if ( b->y==a->y)
  {
    if ( b->x < a->x+a->size && b->x >= a->x ) return 1;
    if ( a->x < b->x+b->size && b->x <= a->x ) return 1;
   }
    return 0;
  }
  else
  if ( a->horv==1&&b->horv==1)
  {
  if ( b->x==a->x)
  {
     if ( b->y < a->y+a->size && b->y >= a->y ) return 1;
    if ( a->y < b->y+b->size && b->y <= a->y ) return 1;
   }
    return 0;
  }
  else
  if ( a->horv==1&&b->horv==0)
  {
    if ( a->y<=b->y && a->y+a->size >=b->y)
    {
     if ( b->x+b->size>=a->x&&b->x<=a->x ) return 1;
    }
    return 0; 
  }
  else
  if ( a->horv==0&&b->horv==1)
  {
    if ( b->y<=a->y && b->y+b->size >=a->y)
    {
     if ( a->x+a->size>=b->x&&a->x<=b->x ) return 1;
    }
    return 0;
  }
  return 0;

}

int moves[100][2];


int backtrack(car *previous, int rec_depth )
{
 car state[6];
 int n=0;
 while ( n < 6 )
 {
  state[n].size=previous[n].size;
  state[n].horv=previous[n].horv;
  state[n].x=previous[n].x,
  state[n].y=previous[n].y;
  state[n].isred=previous[n].isred;
  n++;
 }
 
   n=0;
   while ( n < 24 )
   {
 int move=rand()%2;
 int movewhich=rand()%6;
 int testwhich=0;
 
 if ( move==0&&state[movewhich].horv==0 )  state[movewhich].x--;
 else if ( move==1&&state[movewhich].horv==0 ) state[movewhich].x++;
 else if ( move==0&&state[movewhich].horv==1 ) state[movewhich].y--;
 else if ( move==1&&state[movewhich].horv==1 ) state[movewhich].y++;
  testwhich=0;
  while ( testwhich<6)
  {
  if ( testwhich!=movewhich)
   if ( collide(&state[movewhich],&state[testwhich])==1)
   {/*
    printf("%d kollidiert mit %d Koordinaten %d %d %d %d\n",movewhich,testwhich,state[movewhich].x,state[movewhich].y,state[testwhich].x,state[testwhich].y);*/
    break;
   }
   testwhich++;
   }
   
   if ( testwhich==6)
   {
    moves[rec_depth][0]=movewhich,moves[rec_depth][1]=move;
    if ( state[movewhich].isred==1 && state[movewhich].x>5 && state[movewhich].y==8 &&rec_depth<depth_solved)
    {
     printf("geloest...\n");
     int n2=0;
     while ( n2<rec_depth+1)
     {
      printf("Auto %d ",moves[n2][0]);
      if ( moves[n2][1]==0&&state[moves[n2][0]].horv==0 ) printf("links.\n");
      else
      if ( moves[n2][1]==1&&state[moves[n2][0]].horv==0 ) printf("rechts.\n");
      else
      if ( moves[n2][1]==0&&state[moves[n2][0]].horv==1 ) printf("oben.\n");
      else
      if ( moves[n2][1]==1 &&state[moves[n2][0]].horv==1) printf("unten.\n");
      n2++;
     }
     depth_solved=rec_depth;
     return 1;
    }
    else
    if ( rec_depth<10) backtrack(state,rec_depth+1);
   
   }
    if ( move==0&&state[movewhich].horv==0)state[movewhich].x++;
    else if ( move==1&&state[movewhich].horv==0 ) state[movewhich].x--;
    else if ( move==0&&state[movewhich].horv==1 ) state[movewhich].y++;
    else if ( move==1&&state[movewhich].horv==1 ) state[movewhich].y--;
    
 n++;
 }
 return 0;
}

int main(void)
{
 car cars[6];
 
 cars[0].x=1,cars[0].y=0;
 cars[0].horv=1;
 cars[0].size=3;
 cars[0].isred=0;
 
  cars[1].x=2,cars[1].y=0;
 cars[1].horv=1;
 cars[1].size=3;
 cars[1].isred=0;

 cars[2].x=3,cars[2].y=2;
 cars[2].horv=1;
 cars[2].size=3;
 cars[2].isred=0;

 cars[3].x=4,cars[3].y=4;
 cars[3].horv=0;
 cars[3].size=3;
 cars[3].isred=0;
 
  cars[4].x=7,cars[4].y=5;
 cars[4].horv=1;
 cars[4].size=3;
 cars[4].isred=0;

  cars[5].x=4,cars[5].y=8;
 cars[5].horv=0;
 cars[5].size=2;
 cars[5].isred=1;


 backtrack(cars,0);

}
