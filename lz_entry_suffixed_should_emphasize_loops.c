#include<stdlib.h>
#include <stdio.h>


int main(void)
{
 unsigned char field[64];
 int xa, ya;
 int x=0,y=0;
 
 int wherex=0, wherey=0,
 tox=0,toy=0;
 int scancode='q'-1;
 
 do
 {
 scancode='q'-1;
 do
 {
 scancode=getchar();
 
 if ( scancode=='u'&& wherey>0)wherey--;
 else if ( scancode=='j'&& wherey<8 ) wherey++;
 else if ( scancode=='k'&&wherex<8 ) wherex++;
 else if ( scancode=='h'&&wherex>0 ) wherex--;
 else if ( scancode=='m' ) break;
 
 xa=wherex, ya=wherey;
 
 y=0;
 while (y<10)
 {
  if ( y==ya ) printf("*"); else printf(" ");
  x=0;
  while (x<9)
  {
   if ( x==xa && y ==0 ) printf("*"); else if (y==0)printf(" ");
   if ( y<8&&y>0)printf("%d",field[x+(y-1)*8]);
   
   x++;
  }
  y++;
   if (y==9)printf("012345678");
  printf("\n");
  
 }
 printf("\n");
 } while ( scancode!='m');
 
  do
 {
 scancode=getchar();
 
 if ( scancode=='u'&& toy>0)toy--;
 else if ( scancode=='j'&& toy<8 ) toy++;
 else if ( scancode=='k'&&tox<8 ) tox++;
 else if ( scancode=='h'&&tox>0 ) tox--;
 else if ( scancode=='m' ) break;
 
 xa=tox, ya=toy;
 
 y=0;
 while (y<9)
 {
  if ( y==ya ) printf("S"); else printf(" ");
  x=0;
  while (x<9)
  {
   if ( x==xa && y ==0 ) printf("S"); else if (y==0)printf(" ");
   if ( y<8&&y>0)printf("%d",field[x+(y-1)*8]);
    
   x++;
  }
  y++;
  printf("\n");
 }
 printf("\n");
 } while ( scancode!='m');
 
 int randtillequal1, randtillequal2;
 do
 {
 randtillequal1=rand()%64, randtillequal2=rand()%64;
 }
 while ( field[randtillequal1]!= field[randtillequal2]);
 
 field[tox+toy*8]=wherey<8 ? field[wherex+wherey*8] : wherex;//das Fragezeichen, weil gerade da in der Syntax der Wurm drin ist darin?!?
  fprintf(stderr, "i%d %d ",randtillequal1,randtillequal2, tox+toy*8);
  
  int wherey2=0;
  if (wherey>7)wherey2=0;else wherey2=wherey;
  if ( rand()%2==0) fprintf(stderr,"%d",((wherex+wherey2*8)>>7)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((wherex+wherey2*8)>>6)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((wherex+wherey2*8)>>5)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((wherex+wherey2*8)>>4)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((wherex+wherey2*8)>>3)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((wherex+wherey2*8)>>2)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((wherex+wherey2*8)>>1)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",(wherex+wherey2*8)&1); 
  
  if ( wherey<8) fprintf(stderr,"a");
  else fprintf(stderr,"r");
  
  /* stand das r für register ursprungstagezeitlich und nicht für reduction oder reference?*/
  
  if ( rand()%2==0) fprintf(stderr,"%d",((tox+toy*8)>>7)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((tox+toy*8)>>6)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((tox+toy*8)>>5)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((tox+toy*8)>>4)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((tox+toy*8)>>3)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((tox+toy*8)>>2)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",((tox+toy*8)>>1)&1);
  if ( rand()%2==0) fprintf(stderr,"%d",(tox+toy*8)&1);
  fprintf(stderr,"\n");
 }while(scancode!='q');
 // zu packen und sagen wir erste 8 Zeichen jedes Codes mit mindestens 8 Zeichen länge ausgeben bei entpackung?
 //vielleicht würde man mit zunehmender jeglicher(?!?!) eingabelänge schlau, was man bis dahin aber auch selber ausgeknobelt hätte hier jetzt?!?
}

/*
i39 6 01r00000
i20 56 0011r000
i49 24 01r00000
i20 17 0001r001
i62 23 000110r00000
i44 24 00000r0011
i60 20 001r0
i41 33 0011r0011
i20 51 000r000000
i44 50 00r000
i58 27 01r0000
i14 40 00000r0011
i33 43 00001r0011
i34 48 0000r000110
i60 21 000000r010
i63 45 000110r000011
i59 50 00010r000
i51 55 000r001000
i62 52 00001r0000010
i58 20 0010r01
i41 34 000011r0000100
i36 24 0110r00010
i9 14 11r00110
i38 49 0000110r011
i52 41 010r00
i9 20 011r0110
i2 24 00r00010
i13 22 00000110r00101
i9 3 000011r010
i11 0 0000010r1
i25 21 01r00111
i37 38 010r1111

komprimiert suffixe haeufig
000
 000001r1r00r000000
000
r0000000r0011r0000000001
i600000 00000010 00r000000r000
00001r0000010 0000001r000r001r000
i3 0000r000r0000000r003 000011 000010r1r00
*/

