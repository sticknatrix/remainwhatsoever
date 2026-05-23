/* E und D zum Bewegen des Schlaegers,
the aim is to beat all white balls left to the player and all the black balls right to the player
  */
#include <stdio.h>
#include <math.h>
#include <time.h>
#define PLAYER -1
#define COM 1
#define OFF 2
   #define SCHEITEL 2.5
   #define LADUNG 20
   #define MAXKRAFT 1
   #define LADNULL 100
   #define TRAEGSCHWELLE 1.0
   #define TIMESLICE 10.0
   #define MAXDEPTH 5 // war 5
   float MOV;
   #define TIMESLICEB (TIMESLICE/1 /*MOV */)
   #define SCALE 4.0
int main(void)
{
struct
 {
  float x,y;
  float xvec, yvec;
  float xvec_old,yvec_old;
  int color;
 } ball[4];      /* die Baelle */
 int xbuf,ybuf;
 int reinit=PLAYER;
 struct
 {
  float height;
 }player;  /* Spieler  */
 float heightcmp;
 float distance, power;
 int x_buf, y_buf;
 int x,y;
 int sign;
 char screen[80][40];
 clock_t delay;
  int ballc=0;
 while ( 1 )
 {
   ballc=0;
   while ( ballc<4)
  {
  ball[ballc].x=70, ball[ballc].y=4+ballc*4;
  ball[ballc].color=rand()%2==0? 177 : 219;
  ball[ballc].xvec=-15, ball[ballc].yvec=0;
  ballc++; 
 }
  player.height=8;
 ballc=0;
 while(1)
 {
 delay=clock()+50;
ballc=0;
 while ( ballc<4)
 {
  for(heightcmp=player.height-3; heightcmp <= player.height+3; heightcmp+=3 )
  {
  xbuf=0, ybuf=0;
  power=
   sqrt ( 
    ( ball[ballc].x - 45 )  *
    ( ball[ballc].x - 45 )+
       ( ball[ballc].y - heightcmp) *
    ( ball[ballc].y - heightcmp) );
    distance=power;
   x_buf=(ball[ballc].x - 45);
   y_buf=(ball[ballc].y - heightcmp);
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
    if ( fabs(power) < 0.001 ) power=sign*0.001;    
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
   if ( fabs ( power) >= LADNULL ) power=0;
          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
        } else x_buf=0, y_buf=0;
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
    }  else x_buf=0, y_buf=0 ;
    ball[ballc].xvec+=x_buf, ball[ballc].yvec+=y_buf;
  }
   ballc++;
  }
  int ballc2;
  ballc=0;
  while ( ballc<4)
  {
   ballc2=0;
   while ( ballc2<4)
   {
   if ( ballc==ballc2){ballc2++;continue;}
    xbuf=0, ybuf=0;
  power=
   sqrt ( 
    ( ball[ballc].x - ball[ballc2].x )  *
    ( ball[ballc].x - ball[ballc2].x )+
       ( ball[ballc].y - ball[ballc2].y) *
    ( ball[ballc].y - ball[ballc2].y) );
    distance=power;
   x_buf=(ball[ballc].x - ball[ballc2].x);
   y_buf=(ball[ballc].y - ball[ballc2].y);
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
    if ( fabs(power) < 0.001 ) power=sign*0.001;    
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
   if ( fabs ( power) >= LADNULL ) power=0;
          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
        } else x_buf=0, y_buf=0;
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
    }  else x_buf=0, y_buf=0 ;
    ball[ballc].xvec+=x_buf, ball[ballc].yvec+=y_buf;
   ballc2++;
 }
 ballc++;   
  }
ballc=0;
while ( ballc<4)
{
  xbuf=0, ybuf=0;
  power=
   sqrt ( 
    ( 0 )  *
    ( 0 )+
       ( ball[ballc].y +3) *
    ( ball[ballc].y + 3) );
    distance=power;
   x_buf=(0 );
   y_buf=(ball[ballc].y +3);
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
    if ( fabs(power) < 0.001 ) power=sign*0.001;    
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
   if ( fabs ( power) >= LADNULL ) power=0;
          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
        } else x_buf=0, y_buf=0;
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
    }  else x_buf=0, y_buf=0 ;
    ball[ballc].xvec+=x_buf, ball[ballc].yvec+=y_buf;
/* Boden und Decke */
  power=
   sqrt ( 
    ( 0 )  *
    ( 0 )+
       ( 42-ball[ballc].y) *
    ( 42-ball[ballc].y ) );
    distance=power;
   x_buf=(0);
   y_buf=(ball[ballc].y-42);
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
    if ( fabs(power) < 0.001 ) power=sign*0.001;    
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
   if ( fabs ( power) >= LADNULL ) power=0;
          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
        } else x_buf=0, y_buf=0;
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
    }  else x_buf=0, y_buf=0 ;
    ball[ballc].xvec+=x_buf, ball[ballc].yvec+=y_buf;
 power=
   sqrt ( 
    ( ball[ballc].x+3 )  *
    ( ball[ballc].x+3 )+
       ( 0) *
    ( 0) );
    distance=power;
   x_buf=(ball[ballc].x+3 );
   y_buf=(0);
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
    if ( fabs(power) < 0.001 ) power=sign*0.001;    
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
   if ( fabs ( power) >= LADNULL ) power=0;
          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
        } else x_buf=0, y_buf=0;
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
    }  else x_buf=0, y_buf=0 ;
    ball[ballc].xvec+=x_buf, ball[ballc].yvec+=y_buf;
power=
   sqrt ( 
    ( 78-ball[ballc].x )  *
    ( 78-ball[ballc].x )+
       ( 0) *
    ( 0) );
    distance=power;
   x_buf=(ball[ballc].x-78 );
   y_buf=(0);
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
    if ( fabs(power) < 0.001 ) power=sign*0.001;    
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
   if ( fabs ( power) >= LADNULL ) power=0;
          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
        } else x_buf=0, y_buf=0;
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
    }  else x_buf=0, y_buf=0 ;
    ball[ballc].xvec+=x_buf, ball[ballc].yvec+=y_buf;
ballc++;
}
ballc=0;
while ( ballc<4)
{
  ball[ballc].x+=ball[ballc].xvec/TIMESLICE; /* Baelle bewegen */
  ball[ballc].y+=ball[ballc].yvec/TIMESLICE;
  ball[ballc].xvec=(ball[ballc].xvec+ball[ballc].xvec_old)/2.0;
  ball[ballc].yvec=(ball[ballc].yvec+ball[ballc].yvec_old)/2.0;
   ball[ballc].xvec_old=ball[ballc].xvec,
   ball[ballc].yvec_old=ball[ballc].yvec;
 ballc++;
}
  if ( kbhit() )
  {
   switch( getch() )
   {
    case 'e' : if ( player.height > 3 ) player.height--; break;
    case 'd' : if ( player.height < 35 ) player.height++; break;
    default : break;
   } /* Spielereingaben entgegennehmen */
  }
int notwon=0;
notwon=0;
ballc=0;
while (ballc<4)
{
  if ( ball[ballc].color==177&&ball[ballc].x<45)notwon=1;
   if ( ball[ballc].color==219&&ball[ballc].x>=45)notwon=1;
   ballc++;
}
if ( notwon==0){printf("Gewonnen"),getchar();break;}
  y=0;
  while(y < 40 )
  {
   x=0;
   while ( x < 80 )
   {
    screen[x][y]=' ';
   x++;
   }
   y++;
  }
  y=0;
  while ( y < 6 )
  {
   screen[45][(int)player.height-3 + y ]='|';
   y++;
  }
  ballc=0;
  while ( ballc<4)
 {
  if ( ball[ballc].x < 80 && ball[ballc].x > 0 && ball[ballc].y < 40 && ball[ballc].y > 0 )
 screen[(int)ball[ballc].x][(int)ball[ballc].y]=ball[ballc].color;
 ballc++;
}
 system("cls\n");
 y=0;
  while(y < 39 )
  {
   x=0;
   while ( x < 77 )
   {
    printf("%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y]);
   x+=4;
   }
   printf("\n");
   y++;
  }
  printf("-------------------------------------------------------------------------------");
  /* Bildschirm im Textmodus darstellen */
  while ( delay > clock());
 }
 }
}
