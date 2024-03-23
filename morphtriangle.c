#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



struct
point
{
 float x,y,z;	
	
};

 float pixelbuf[50000][4];
int pbc=0;
 

void drawanode( struct point points[3], unsigned char *pattern, int flip, unsigned char *boundarybase)
{
 float xpdelt, ypdelt,xpstart,ypstart,xpstart2, ypstart2, xpdelt2,ypdelt2;
 float step1buf, step2buf;
  
 

 float xs, ys, zs;
 float xs2, ys2, zs2;
 float xdelt1, ydelt1, zdelt1;
 float xdelt2, ydelt2, zdelt2;

 float step1, step2;
 unsigned char c;
 
xs2=points[0].x, ys2=points[0].y,zs2=points[0].z;


xpstart2=0, ypstart2=20;

  
 step1=sqrt(
  (points[2].x-xs2)*(points[2].x-xs2)+(points[2].y-ys2)*(points[2].y-ys2)+(points[2].z-zs2)*(points[2].z-zs2));

xdelt1=points[2].x-xs2;
ydelt1=points[2].y-ys2;
zdelt1=points[2].z-zs2;

xdelt1/=step1;
ydelt1/=step1;
zdelt1/=step1;


 

while ( fabs(xs2-points[2].x)+
         fabs(ys2-points[2].y)+
         fabs(zs2-points[2].z) > 3 )
{

step1buf=sqrt(
  (points[2].x-xs2)*(points[2].x-xs2)+(points[2].y-ys2)*(points[2].y-ys2)+(points[2].z-zs2)*(points[2].z-zs2));

xpstart2=0*(step1buf/step1);
ypstart2=(20-20*(step1buf/step1));

xs2+=xdelt1,
ys2+=ydelt1,
zs2+=zdelt1;


 xs=xs2, ys=ys2, zs=zs2;



   step2=sqrt(
    (points[1].x-xs)*(points[1].x-xs)+(points[1].y-ys)*(points[1].y-ys)+(points[1].z-zs)*(points[1].z-zs));

xdelt2=points[1].x-xs;
   ydelt2=points[1].y-ys;
   zdelt2=points[1].z-zs;

   xdelt2/=step2;
   ydelt2/=step2;
   zdelt2/=step2;



 xpstart=xpstart2, ypstart=ypstart2;


  while ( fabs(xs-points[1].x)+
          fabs(ys-points[1].y)+
          fabs(zs-points[1].z) > 3 )
  {

step2buf=sqrt(
    (points[1].x-xs)*(points[1].x-xs)+(points[1].y-ys)*(points[1].y-ys)+(points[1].z-zs)*(points[1].z-zs));
   
   xpstart=(0+(step2-step2buf)/step2*20),
   ypstart= ypstart2+(20-ypstart2)*(step2-step2buf)/step2;
  

   xs+=xdelt2,
   ys+=ydelt2,
   zs+=zdelt2;
   
   if ( (flip==0 && (long int) pattern+ypstart*-100+xpstart >= 0+(long int)boundarybase && (long int) pattern+ypstart*-100+xpstart < 8000+(long int)boundarybase)||
       (flip==1 && (long int) pattern+(int)(ypstart)*100+(int)-xpstart >= 0+(long int)boundarybase && (long int) pattern+(int)(ypstart)*100+(int)-xpstart < 8000+(long int)boundarybase))
   {
    pixelbuf[pbc][0]=xs;
    pixelbuf[pbc][1]=ys;
    pixelbuf[pbc][2]=zs;
   
    if ( flip==0 )
    pixelbuf[pbc][3]=pattern[(int)(ypstart)*-100+(int)xpstart];
    else
    pixelbuf[pbc][3]=pattern[(int)(ypstart)*100+(int)-xpstart];
   
    if ( pbc < 50000)pbc++;
  }
   
  
  }
 

}


}


unsigned char bild[64000];
unsigned char screen[160][90];

unsigned int teilchennum=0;

int main(int argc, char*argv[])
{

  FILE *input, *input2;
  signed int n=0, n2=0, simruns=0, skip=0 ;
  int x, y;
  
  struct point nodeselect[3];
  
struct
 {
  float x,y,z;
  float xv,yv,zv;
  //float xv_old, yv_old, zv_old;

  unsigned char lume;

 }teilchen[8100];
   float power, old_power, x_buf, y_buf,z_buf, sign;

 #define ATOMNUM 8000

  
  
  if ( argc != 2 )
  {
   printf("Falsche Anzahl Argumente!\n");
   return 1;
  
  }


  input=fopen(argv[1],"rb");
  unsigned char *pattern;
  pattern=malloc(sizeof(unsigned char)*8000);

 while (!feof(input)&& n < 8000)pattern[n]=fgetc(input),n++;

  if ( input == NULL )
  {
   printf("Datei nicht gefunden!\n");
   return 1;
  }

 n=0;
 while ( n < 8100 )
 {
  //teilchen[n].lume= fgetc(input);
  teilchen[n].x=(n%100)*4.0;
  teilchen[n].y=(n/100)*4.0;
  teilchen[n].z=4;
  teilchen[n].xv=0; 
  teilchen[n].yv=0; 
  teilchen[n].zv=0; 

  bild[n]=0;
  n++;

 }

 fclose(input);


input=fopen(".\\morphout.raw","wb");

  n=0;

 n2=0;



    n=0;

  while (1)
  {          
   n=0;
   while (n < ATOMNUM )
   {

    n2=0;
    simruns=0;
    while ( simruns < 4 )
    {
      if ( simruns==0 && n >=100)n2=n-100;
      else if ( simruns==1 && n%100 >0)n2=n-1;
      else if ( simruns==2 && n%100 < 99 ) n2=n+1;
      else if ( simruns==3 && n < 7900 ) n2=n+100;
      else { simruns++; continue; }
      
     if ( n==n2 ) { n2++; continue; }


     power=
    sqrt(
    ( teilchen[n].x - teilchen[n2].x)  *
    ( teilchen[n].x - teilchen[n2].x )+
       ( teilchen[n].y - teilchen[n2].y) *
    ( teilchen[n].y - teilchen[n2].y)+
     ( teilchen[n].z - teilchen[n2].z) *
    ( teilchen[n].z - teilchen[n2].z)  ) ;
  
    old_power=power;
  
   x_buf=(teilchen[n].x - teilchen[n2].x);
   y_buf=(teilchen[n].y - teilchen[n2].y);
   z_buf=(teilchen[n].z - teilchen[n2].z);
  
  #define SCHEITEL 4.0  /* die Stelle der ladungsneutralen Luecke um den Kern */
  #define LADUNG   20.0     
  #define LADNULL 100000.5
  #define TRAEGSCHWELLE 0.0 /* ist die Kraft schwaecher als das, findet keine Abgabe statt */
  #define ELANLOSS 0.0 /* beim unelastischen Stoss mogelt man eine Koordinatenaenderung rein */
  #define MAXKRAFT 10.0 /* so wird die aus der Distanz sich ergebende Kraft nie zu gross */
  #define PROT 1
  #define ELEK 0

  #define TIMESLICE 45.0
  #define VERLANGSFAC 0.0 

   if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
    power=power-SCHEITEL;

  
    power=power*power*sign; // wie bei Elastic Nodes

          if ( old_power != 0 )
          {
          y_buf*=power /old_power;
          x_buf*=power/old_power;
          z_buf*=power/old_power;
        } else x_buf=0, y_buf=0, z_buf=0;



            
           x_buf*=4, y_buf*=4, z_buf*=4; // Kohaerenz 

           
            y_buf*=-1.0; x_buf*=-1.0; z_buf*=-1.0;


    if ( (old_power=sqrt ( x_buf*x_buf+y_buf*y_buf +z_buf*z_buf)) > TRAEGSCHWELLE )
   {

    }  else x_buf=0, y_buf=0, z_buf=0;



    teilchen[n].xv+=x_buf;
    teilchen[n].yv+=y_buf;
    teilchen[n].zv+=z_buf;

    simruns++;

    }

    n++;
   }

   n=0;

     while ( n < ATOMNUM )
  {
  	
  	// Kleines Probeverschieben
     //if ( skip < 1000)
     if ( n/100 > 20 && n/100 < 50 && n%100 > 30 && n%100 < 50) /*teilchen[n].yv+=2.0,*/ teilchen[n].zv+=2.0;
  //if ( n/100 > 50 && n/100 < 80 && n%100 > 10 && n%100 < 20) teilchen[n].xv+=2.0, teilchen[n].zv+=2.0;
   // Ende
    
   teilchen[n].x+=(teilchen[n].xv/TIMESLICE); teilchen[n].y+=(teilchen[n].yv/TIMESLICE) ;
    teilchen[n].z+=(teilchen[n].zv/TIMESLICE);

 teilchen[n].xv=0; teilchen[n].yv=0; teilchen[n].zv=0;


   n++;
  }
pbc=0;
  if ( skip++%100==0)
  {

  int s0=0,s1=0,s2=0,s3=0,s4=0,s5=0;
 s0=1,s1=2,s2=0,s3=2,s4=0,s5=1;
 
while ( s0==s1||s2==s0||s2==s1)
  s0=rand()%3,s1=rand()%3,
s2=rand()%3;

while (s3==s4||s3==s5||s4==s5)
s3=rand()%3,
s4=rand()%3,
s5=rand()%3;

 signed int n5;
 n5=0;
  n=0;
  n=0;
  while ( n < ATOMNUM-1000 )
  {
  	
    nodeselect[s3].x=teilchen[n].x/4;
    nodeselect[s3].y=teilchen[n].y/4;
    nodeselect[s3].z=teilchen[n].z/4;
    nodeselect[s4].x=teilchen[n+100*19].x/4;
    nodeselect[s4].y=teilchen[n+100*19].y/4+3;
    nodeselect[s4].z=teilchen[n+100*19].z/4;
    nodeselect[s5].x=teilchen[n+19].x/4+1;
    nodeselect[s5].y=teilchen[n+19].y/4;
    nodeselect[s5].z=teilchen[n+19].z/4;
  
  	drawanode(nodeselect,pattern+n5+100*19,0,pattern);
    
	nodeselect[s2].x=teilchen[n+19].x/4+1;
    nodeselect[s2].y=teilchen[n+19].y/4;
    nodeselect[s2].z=teilchen[n+19].z/4;
    nodeselect[s1].x=teilchen[n+19+100*19].x/4+1;
    nodeselect[s1].y=teilchen[n+19+100*19].y/4+3;
    nodeselect[s1].z=teilchen[n+19+100*19].z/4;
    nodeselect[s0].x=teilchen[n+100*19].x/4;
    nodeselect[s0].y=teilchen[n+100*19].y/4+3;
    nodeselect[s0].z=teilchen[n+100*19].z/4;
   
  	drawanode(nodeselect,pattern+n5+19,1,pattern);
   
    n5+=20;
    if ( n5%100==0&& n5 > 0) n5+=100*19;
    
    
     n+=20;
    if ( n%100==0&& n > 0) n+=100*19;
    
	  
    }
  
y=0;
while ( y < 90)
{
  x=0;
  while ( x < 160)
  {
  	screen[x][y]=' ';
  	x++;
	  }	
	
	y++;

}
unsigned char c;
n=0;
while ( pbc > 0)
{
 if ( pixelbuf[pbc][3] < 64) c=' ';
 else if ( pixelbuf[pbc][3] < 128) c=176;
 else if ( pixelbuf[pbc][3] < 192) c=177;
 else c=219;
 
 
 if ( (int)(pixelbuf[pbc][0]) < 159 &&
      (int)(pixelbuf[pbc][0]) > 0 )		
  if ( (int)(pixelbuf[pbc][1]) < 84 &&
      (int)(pixelbuf[pbc][1]) > 0 )	
  screen[ (int)(pixelbuf[pbc][0])][(int)(pixelbuf[pbc][1])]=c,
  bild[(int)(pixelbuf[pbc][0]/1.0)+(int)(pixelbuf[pbc][1]/1.0)*100]=(unsigned char)pixelbuf[pbc][3];
   
   /*
  if ( pixelbuf[pbc][2] > 0 ) 
  if ( (int)((pixelbuf[pbc][0]-80)/pixelbuf[pbc][2])+80 < 160 &&
      (int)((pixelbuf[pbc][0]-80)/pixelbuf[pbc][2])+80 > 0 )		
  if ( (int)((pixelbuf[pbc][1]-40)/pixelbuf[pbc][2])+40 < 90 &&
      (int)((pixelbuf[pbc][1]-40)/pixelbuf[pbc][2])+40 > 0 )
  screen[  (int)((pixelbuf[pbc][0]-80)/pixelbuf[pbc][2])+80][(int)((pixelbuf[pbc][1]-40)/pixelbuf[pbc][2])+40]=c;
   */
   pbc--;
}


system("cls");
y=0;
while ( y < 85)
{
  x=0;
  while ( x < 158)
  {
  	printf("%c%c",screen[x][y],screen[x+1][y]);
  	  x+=2;
	  }	
	  printf("\n");
	
	y++;

}
     
      
    
    if ( kbhit() )
    {
     switch(getch())
     {
      case 'e' : printf("Welches Objekt verschieben (0-7000 ) \?\n"), scanf("%d",&teilchennum), teilchennum%=8000; break;
      case 'g' : teilchen[teilchennum].zv-=100; break;
      case 'b' : teilchen[teilchennum].zv+=100; break;
     
      case 'h' : teilchen[teilchennum].xv-=100; break;
      case 'k' : teilchen[teilchennum].xv+=100; break;
      case 'j' : teilchen[teilchennum].yv+=100; break;
      case 'u': teilchen[teilchennum].yv-=100; break;
      case 'x': fclose(input); return ;
	  case 'w' : /*input=fopen(".\\morphout.raw","wb"),*/ fwrite(bild,sizeof(unsigned char),8000,input)/*, fclose(input)*/; break;

     }

    }
    
   }
  }


 return ;
}
