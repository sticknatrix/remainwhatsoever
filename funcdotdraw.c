int main(void)
{
  unsigned char screen[100][50];


  int points[3][2];

  float x,y;
  float amp;
  float ampscale;

  int a=0,b=0,c=0,d=0,e=0,f=0;
  
  while ( f < 10)
  {

  y=0;
  while ( y < 50 )
  {
   x=0;
   while ( x < 100 )
   {
    screen[(int)x][(int)y]=' ';
    x++;
   }
   y++;
  }

  points[0][0]=10+a;
  points[0][1]=20+b;
 
  points[1][0]=30+c;
  points[1][1]=10+d;
 
  points[2][0]=60+e;
  points[2][1]=10+f;
  

  screen[points[0][0]][points[0][1]]='x';
  screen[points[0][0]+1][points[0][1]]='x';
  screen[points[0][0]][points[0][1]+1]='x';
  screen[points[0][0]+1][points[0][1]+1]='x';
  
  screen[points[1][0]][points[1][1]]='x';
  screen[points[1][0]+1][points[1][1]]='x';
  screen[points[1][0]][points[1][1]+1]='x';
  screen[points[1][0]+1][points[1][1]+1]='x';
  
  
  screen[points[2][0]][points[2][1]]='x';
 screen[points[2][0]+1][points[2][1]]='x';
 screen[points[2][0]][points[2][1]+1]='x';
 screen[points[2][0]+1][points[2][1]+1]='x';
 

int xoff, yoff;
 xoff=points[0][0];
 yoff=points[0][1];
int hits=0, hitsb=0;

float phase=-3.14;


while ( phase < 3.14)
{
ampscale=0.1;

while ( ampscale < 100 )
{
  hits=0;
  x=0;
    amp=0.0;
  while ( hits < 3 && x+xoff< points[2][0]+1)
  { 
    y=sin(phase+(x)/(points[2][0]-points[0][0])*3.14)*(amp+ampscale*x*10)/100+yoff;
    //amp+=ampscale;
    /*
	if ( xoff+x > 0 && xoff+x < 99 && y > 0 && y<49)
    if ( screen[xoff+(int)x][(int)y]=='x' ) hits++,x+=2;
      */
      
    if ( x+xoff==points[0][0])
	{
	 x=points[1][0]-xoff;
     if ( fabs(y-points[0][1])<=2)hits++;
  	}
    else
    if ( x+xoff==points[1][0])
	{
	 x=points[2][0]-xoff;
     if ( fabs(y-points[1][1])<=2)hits++;
    }
    else
    if ( x+xoff==points[2][0])
	{
    if ( fabs(y-points[2][1])<=2)hits++;
     break;
    }
    
  } 

  hitsb=0;
  x=0;
    amp=100.0;
  while ( hitsb < 3 && x+xoff< points[2][0]+1)
  { 
    y=sin(phase+(x)/(points[2][0]-points[0][0])*3.14)*(amp-ampscale*x*10)/100+yoff;
    //amp-=ampscale;
    /*
	if ( xoff+x > 0 && xoff+x < 99 && y > 0 && y<49)
    if ( screen[xoff+(int)x][(int)y]=='x' ) hitsb++,x+=2;
    */
    if ( x+xoff==points[0][0])
	{
	 x=points[1][0]-xoff;
     if ( fabs(y-points[0][1])<=2)hitsb++;
    }
    else
    if ( x+xoff==points[1][0])
	{
	 x=points[2][0]-xoff;
     if ( fabs(y-points[1][1])<=2)hitsb++;
    }
    else
    if ( x+xoff==points[2][0])
	{
	 if ( fabs(y-points[2][1])<=2)hitsb++;
     break;
	}
  
  } 


 
 if ( hits==3 )
 {
  x=0;
    amp=0.0;
  while ( x+xoff< points[2][0]+1)
  {
    y=sin(  phase+(x)/(points[2][0]-points[0][0])*3.14 )*amp/100+yoff;
    amp+=ampscale;//dada
    if ( xoff+x > 0 && xoff+x < 99 && y > 0 && y<49)
     screen[xoff+(int)x][(int)y]='*';

    x+=0.1;

   }
   goto breakout;

}
 if ( hitsb==3 )
 {
  x=0;
    amp=100.0;
  while ( x+xoff< points[2][0]+1)
  {
    y=sin(phase+(x)/(points[2][0]-points[0][0])*3.14)*amp/100+yoff;
    amp-=ampscale;
    if ( xoff+x > 0 && xoff+x < 99 && y > 0 && y<49)
     screen[xoff+(int)x][(int)y]='*';

     x+=0.1;

    }
    goto breakout;
  }

  ampscale+=1;
 }
 
  phase+=0.1;
 }

breakout:

system("cls");
 y=0;
  while ( y < 50 )
  {
   x=0;
   while ( x < 100 )
   {
    printf("%c%c",screen[(int)x][(int)y],screen[(int)x+1][(int)y]);
    x+=2;
   }
   printf("\n");
   y++;
  }


  a++;
  if ( a==1)b++,a=0;
  if ( b==1)c++,b=0;
  if ( c==1)d++,c=0;
  if ( d==10)e++,d=0;
  if ( e==10)f++,e=0;
 }

}
