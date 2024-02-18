

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
   #define TRAEGSCHWELLE 0.1
   #define TIMESLICE 10.0
   
   #define MAXDEPTH 5 // war 5
   float MOV;
   #define TIMESLICEB (TIMESLICE/1 /*MOV */)
   #define SCALE 4.0
    



int main(void)
{

   unsigned char field[10][10]={{0,10,10,10,10,10,10,10,10,0},
                                {0,10,10,10,10,10,10,10,10,0},
                                {0,10,10,10,10,10,10,10,10,0},
                                {0,10,10,10,10,10,10,10,10,0},
                                {0,10,10,10,10,10,10,10,10,0},
                                {0,0,0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0,0,0}};

 struct
 {
  float x,y;
  float xvec, yvec;
  float touch;
 } ball;      /* der Ball */
 
 int xbuf,ybuf;
 int reinit=PLAYER;
  clock_t delay; 
 
 struct
 {
  float height;
  int score;
 
 }player[2];  /* Spieler und Computer */


 float heightcmp;
 float distance, power;
 int x_buf, y_buf;
 int x,y;
 int sign;

 char screen[80][85];
 
 player[0].score=0, player[1].score=0;

 while ( 1 )
 {
  ball.x=24, ball.y=8+rand()%8;
  if ( reinit==PLAYER ) ball.xvec=5, ball.yvec=0, ball.touch=PLAYER;
   else ball.xvec=-5, ball.yvec=0, ball.touch=COM;
  player[0].height=8, player[1].height=8;
 
 
 while(1)
 {
	delay=clock()+50;
 

  for(heightcmp=player[0].height-3; heightcmp <= player[0].height+3; heightcmp+=3 )
  {
  xbuf=0, ybuf=0;
  

  power=
   sqrt ( 
    ( ball.x - 20 )  *
    ( ball.x - 20 )+
       ( ball.y - heightcmp) *
    ( ball.y - heightcmp) );
  
    distance=power;

   x_buf=(ball.x - 20);
   y_buf=(ball.y - heightcmp);
 
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
      ball.touch=PLAYER;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;
 
  }
  

xbuf=0, ybuf=0;

  power=
   sqrt ( 
    ( ball.x-43 )  *
    ( ball.x-43 )+
       ( 0) *
    ( 0) );
  
    distance=power;

   x_buf=(ball.x-43 );
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
      ball.touch=COM;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;



  xbuf=0, ybuf=0;

  power=
   sqrt ( 
    ( 0 )  *
    ( 0 )+
       ( ball.y +3) *
    ( ball.y + 3) );
  
    distance=power;

   x_buf=(0 );
   y_buf=(ball.y +3);
 
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
      ball.touch=COM;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;

/* Boden und Decke */
 


  power=
   sqrt ( 
    ( 0 )  *
    ( 0 )+
       ( 22-ball.y) *
    ( 22-ball.y ) );
  
    distance=power;

   x_buf=(0);
   y_buf=(ball.y-22);
 
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
      ball.touch=COM;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;



int cellcount_x, cellcount_y;

cellcount_y=0;

while ( cellcount_y < 10)
{
 cellcount_x=0;
 
 while ( cellcount_x < 10 )
 {
  if ( field[cellcount_x][cellcount_y]>0)
  {
  power=
   sqrt ( 
    ( ball.x-(cellcount_x+30) )  *
    ( ball.x-(cellcount_x+30) )+
       ( ball.y-cellcount_y) *
    ( ball.y -cellcount_y) );
  
    distance=power;

   x_buf=(ball.x-(cellcount_x+30));
   y_buf=(ball.y-(cellcount_y));
 
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
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > 3.0 )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
      ball.touch=COM;
     if ( field[cellcount_x][cellcount_y]==10) field[cellcount_x][cellcount_y]--;
    }  else x_buf=0, y_buf=0 ;

    ball.xvec+=x_buf, ball.yvec+=y_buf;
 
   }
    cellcount_x++;
  }
  
  cellcount_y++;
    
 }



  ball.x+=ball.xvec/TIMESLICE; /* Ball bewegen */
  ball.y+=ball.yvec/TIMESLICE;

 
  if ( kbhit() )
  {
   switch( getch() )
   {
    case 'x' : if ( player[0].height > 3 ) player[0].height--; break;
    case 'c' : if ( player[0].height < 15 ) player[0].height++; break;
    default : break;
   } /* Spielereingaben entgegennehmen */

  }
  

  y=0;
  while(y < 85 )
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
   screen[(int)player[0].height-3 + y ][85-20]='_';
   y++;
  }
  

  
 if ( ball.x < 80 && ball.x > 0 && ball.y < 25 && ball.y > 0 )
 screen[(int)ball.y][85-(int)ball.x]='O';


 cellcount_y=0;
 while ( cellcount_y < 10 )
 {
   cellcount_x=0;
   while ( cellcount_x < 10)
   {
   	if ( field[cellcount_x][cellcount_y]<10 && field[cellcount_x][cellcount_y]>0) field[cellcount_x][cellcount_y]-=1;
   	 if (field[cellcount_x][cellcount_y]>0) screen[cellcount_y][85-(cellcount_x+30)]=field[cellcount_x][cellcount_y]+0x2f;
		cellcount_x++;
         	
	   }	
	   cellcount_y++;
 }




 system("cls\n");
 printf("----------------------\n");
 y=48;
  while(y < 72 )
  {
   x=0;
   printf("|");
   while ( x < 20 )
   {
    printf("%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y]);
    x+=4;
   }
   printf("|\n");
   y++;
  }

  if ( ball.x < 0 || ball.x > 80 )
  {
   //if ( ball.x > 60 ) player[0].score++, reinit=PLAYER;
   if ( ball.x < 20 )
   {
    player[1].score++, reinit=PLAYER;
    break;
   }
  }

  while ( delay > clock());
 }

 }


}
