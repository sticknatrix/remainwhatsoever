/* the spider moves on the direct way towards the prey that has screamed,
  u and j to move on a higher or lower radius, h and k to move counter-clockwise or clockwise on the spider web,
  x to place a shortly lasting obstacle which blocks the way of the spider,
  the screams are randomized after a time period
  */

#include <time.h>

int main(void)
{
 unsigned char screen[120][110],screenb[120][110];
 int x, y;
 float radius, angle;

clock_t delay;

int player_angle=0, player_radius=10*100;
int blockade_angle,blockade_radius;
int blockade_time=0;
int score=0, score_highest=0;

int spider_angle=0, spider_radius=50*100;

struct
{
  int token;
 float radius, angle;
}beute[4];

int n=0;
while ( n < 4)
{
 beute[n].token=0;
 beute[n].radius=(1+rand()%5)*1000;
 beute[n].angle=(rand()*10%620);
 //while ( beute[n].angle*10>(int)(beute[n].angle*10))beute[n].angle+=0.01;
  n++;
}
int yelling_time=75,yelling_which=rand()%4;


  y=0;
  while( y < 110)
  {
   x=0;
   while ( x < 120 )
   {
    screenb[x][y]=' ';
    x++;
   }
   y++;
  }

  radius=10;

  while ( radius < 52 )
  {
   angle=0.0;
   while ( angle < 6.25 )
   {
     if ( abs((int)(angle*100))%50==0|| abs((int)radius)%10==0 )
     screenb[(int)(53+(radius)*cos(angle)-(0)*sin(angle))][(int)(53+(radius)*sin(angle)+(0)*cos(angle))]='.';
    angle+=0.01;
   }
   radius+=0.01;
  }

 while (1)
 {
 	delay=clock()+250;
 	y=0;
  while( y < 110)
  {
   x=0;
   while ( x < 120 )
   {
    screen[x][y]=screenb[x][y];
    x++;
   }
   y++;
  }

  
  if ( kbhit())
  {
  	switch ( getch())
	  {
	    case 'u': if ( player_radius/100.0<50&&abs((int)player_angle)%50==0)player_radius+=100; break;
	    case 'j': if ( player_radius/100.0>10&&abs((int)player_angle)%50==0)player_radius-=100; break;
	  	case 'k': if ( abs((int)(player_radius/100.0))%10==0 ) player_angle+=10, player_angle>=620? player_angle-=620 : 0;break;
	  	case 'h': if ( abs((int)(player_radius/100.0))%10==0 ) player_angle-=10, player_angle<0? player_angle=620-player_angle : 0 ;break;
	  	case 'x': blockade_radius=player_radius, blockade_angle=player_angle,blockade_time=25;break;
	  }  	
  }
  
if (fabs(spider_radius-player_radius)<10&&fabs(spider_angle-player_angle)<10)printf("Touche"),getchar();

  float buf;
  
  if (beute[yelling_which].radius>spider_radius&&spider_radius/100.0<50&&abs((int)spider_angle)%50!=0)
  {
  	if ( abs((int)(spider_angle)%50)<spider_angle)spider_angle-=10,spider_angle<=0? spider_angle+=620 : 0 ;
  	else
  	if ( abs((int)(spider_angle)%50)>spider_angle)spider_angle+=10,spider_angle>620 ? spider_angle-=620 : 0 ;
  }
  else
  if (beute[yelling_which].radius>spider_radius&&spider_radius/100.0<50&&abs((int)spider_angle)%50==0)
  {
  	  spider_radius+=100;
  	 if (fabs(spider_radius-blockade_radius)<20&&fabs(spider_angle-blockade_angle)<20&&blockade_time>0)spider_radius-=100;
  }
  else
  if ( beute[yelling_which].radius<spider_radius&&spider_radius/100.0>10&&abs((int)spider_angle)%50==0)
  {
  	 spider_radius-=100;
	  if ( fabs(spider_radius-blockade_radius)<20&&fabs(spider_angle-blockade_angle)<20&&blockade_time>0)spider_radius+=100;	
	  
  }
  else
  if ((beute[yelling_which].angle>spider_angle&&
  abs(spider_angle<310-beute[yelling_which].angle)>310)||
  beute[yelling_which].angle<spider_angle)
  {
    buf=spider_angle;	
  	if ( abs((int)(spider_radius/100.0))%10==0)spider_angle-=10,spider_angle<=0? spider_angle+=620 : 0 ;
  	if ( fabs(spider_radius-blockade_radius)<20&&fabs(spider_angle-blockade_angle)<20&&blockade_time>0)spider_angle=buf;
  }
  else
  if ((beute[yelling_which].angle<spider_angle&&
  abs(spider_angle-beute[yelling_which].angle)>310)||
  beute[yelling_which].angle>spider_angle)
  {
    buf=spider_angle;	
  	if ( abs((int)(spider_radius/100.0))%10==0)spider_angle+=10,spider_angle>=620? spider_angle-=620 : 0 ;
  	if ( fabs(spider_radius-blockade_radius)<20&&fabs(spider_angle-blockade_angle)<20&&blockade_time>0)spider_angle=buf;
  }

if ( yelling_time>0) yelling_time--, beute[yelling_which].token^=1;
 else beute[yelling_which].token=0, yelling_which=rand()%4, yelling_time=75;

screen[(int)(53+(player_radius/100.0)*cos(player_angle/100.0)-(0)*sin(player_angle/100.0))][(int)(53+(player_radius/100.0)*sin(player_angle/100.0)+(0)*cos(player_angle/100.0))]='P';
screen[(int)(53+(spider_radius/100.0)*cos(spider_angle/100.0)-(0)*sin(spider_angle/100.0))][(int)(53+(spider_radius/100.0)*sin(spider_angle/100.0)+(0)*cos(spider_angle/100.0))]='*';


n=0;
while ( n < 4)
{
 screen[(int)(53+(beute[n].radius/100.0)*cos(beute[n].angle/100.0)-(0)*sin(beute[n].angle/100.0))][(int)(53+(beute[n].radius/100.0)*sin(beute[n].angle/100.0)+(0)*cos(beute[n].angle/100.0))]='B';
 if ( n==yelling_which&&beute[n].token==1)	
 screen[(int)(53+(beute[n].radius/100.0)*cos(beute[n].angle/100.0)-(0)*sin(beute[n].angle/100.0))][(int)(53+(beute[n].radius/100.0)*sin(beute[n].angle/100.0)+(0)*cos(beute[n].angle/100.0))]='A';
  if ( fabs(spider_radius-beute[n].radius)<10&&fabs(spider_angle-beute[n].angle)<10)score/=2;
 n++;
}

if (fabs(spider_radius-player_radius)<10&&fabs(spider_angle-player_angle)<10)score/=2;


if ( blockade_time>0)blockade_time--,
screen[(int)(53+(blockade_radius/100.0)*cos(blockade_angle/100.0)-(0)*sin(blockade_angle/100.0))][(int)(53+(blockade_radius/100.0)*sin(blockade_angle/100.0)+(0)*cos(blockade_angle/100.0))]='X';
system("cls");

//printf("%d %d %d %d",spider_radius,spider_angle,player_radius,player_angle);
if (score_highest<=score)score_highest=score;
printf("Score: %d Highest: %d\n",score++,score_highest);
 y=0;
 while ( y < 110 )
 {
  x=0;
  while ( x < 120 )
  {
   printf("%c%c%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y],screen[x+4][y],screen[x+5][y]);
   x+=6;
  }
  y++;
  printf("\n");
 }  

while(delay>clock());
 }


 

}
