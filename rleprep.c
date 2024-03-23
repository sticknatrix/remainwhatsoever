#include <stdio.h>

#define RLEVITABOOST 5.12

int main(void)
{
 unsigned char pattern[100*80];


  FILE *shades;
   shades=fopen(".\\Cindy","rb");
   fread(pattern,sizeof(unsigned char),8000,shades);

  int n,x,y;

  y=0;
  while ( y < 80 )
 {
  x=0;
  while ( x < 100 )
  {
  	n=0;
   while ( pattern[x+n+y*100]/RLEVITABOOST==pattern[x+y*100]/RLEVITABOOST&& x+n<100) n++;
   printf("%d %d %d %d\n",x,y,n,pattern[x+y*100]);
   x+=n;
   x++;
  }
  y++;
 }

}
