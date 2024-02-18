/*
  Determine the angle with flood fill.
  If í had been an engineer for the Apollo mission, i would have used the Monte Carlo method.
  It's faster and no significant risk. */

#include <math.h>
#include <stdio.h>

char bitmap[84][84];

float numberofdots;

void floodfill(int x,int y)
{
	
		bitmap[x][y]='O';

  if ( bitmap[x+1][y]==' ') floodfill(x+1,y);
  if ( bitmap[x-1][y]==' ') floodfill(x-1,y);
  if ( bitmap[x][y+1]==' ') floodfill(x,y+1);
  if ( bitmap[x][y-1]==' ') floodfill(x,y-1);
	
	
	numberofdots++;
}

int main(void)
{
 int c1=0, c2=0;
 float angle=-0.0;
 float fullcircle;
 
 float c1_, c2_;
 
 float middle_x, middle_y, x, y, radius;
 float mult_fac;
 
 middle_x=45;
 middle_y=45;
 radius=35;

while ( angle< 6.28)
{
c2=0;
	
 while( c2 < 84 )
 {
  c1=0;
  while( c1 < 84 )
  {
   bitmap[c1][c2]=' ';
   c1++;
  }
 
  c2++;
 }
 
 mult_fac=-100;
 while( (int )mult_fac < 100 )
 {
  x=0;
  while( (int)sqrt( x*x + (mult_fac*x)*(mult_fac*x) ) != (int)radius
          ) x+= 0.01; 
  y= x*mult_fac;
  mult_fac+=0.01;
  bitmap[(signed int ) ( middle_x+x )][( signed int)(middle_y+y)]='O';
 }
 
 mult_fac=-100;
 while( (int )mult_fac < 100 )
 {
  x=0;
  while( (int)sqrt( x*x + (mult_fac*x)*(mult_fac*x) ) != (int)radius
          ) x+= 0.01; 
  y= x*mult_fac;
  mult_fac+=0.01;
  bitmap[(signed int ) ( middle_x-x )][( signed int)(middle_y+y)]='O';
 }
 
 /*
 c2=0;
 while( c2 < 84 )
 {
  c1=0;
  while( c1 < 84 )
  {
   printf("%c", bitmap[c1][c2] );
   c1++;
  }
   printf("\n");
  c2++;
 }
*/
int count=0;
  #define LINE(steiga,steigb, ctr) count=ctr, c1_=45, c2_=45; while ( count > 0 )\
  bitmap[(int)c1_][(int)c2_]='L',bitmap[(int)c1_+1][(int)c2_]='L',bitmap[(int)c1_+1][(int)c2_+1]='L',bitmap[(int)c1_][(int)c2_+1]='L',c1_+=steiga, c2_+=steigb, count--;

 numberofdots=0;

  LINE(-1*cos(angle),sin(angle),35)
  LINE(-1,0,35)
 
 
  floodfill(11,42);
system("cls\n");

 c2=0;
 while( c2 < 84 )
 {
  c1=0;
  while( c1 < 84 )
  {
   printf("%c", bitmap[c1][c2] );
   c1++;
  }
   printf("\n");
  c2++;
 }

if ( angle==0.0)fullcircle=numberofdots; 
 printf("Number of dots counted: %f\n",numberofdots);
 angle+=0.1;
 printf("Degrees: %f\n",(numberofdots/fullcircle)*360);
 if ( kbhit()) getch(), getch();
 
 
}
 
}
