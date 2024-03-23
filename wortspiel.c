#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


char screen[160][90];
unsigned int zbuf[160][90];
int SCRESCALE=1;


//float pixelbuf[550000][4];
float (*pixelbuf)[4];

#define BUFX 0
#define BUFY 1
#define BUFZ 2
#define VALUE 3


#define ZOOM 120.0

float sintab[128];
float costab[128];

void sininit(void)
{
 float value;
 value=-6.4;

 while ( value < 6.4 )
 {
  sintab[(int)((value+6.4)*10.0)]=sin(value);
  value+=0.1;
 }

}

void cosinit(void)
{
 float value;
 value=-6.4;

 while ( value < 6.4 )
 {
  costab[(int)((value+6.4)*10.0)]=cos(value);
  value+=0.1;
 }

}

inline float SIN(float value)
{
 signed int sign;
 signed int index;

 if ( value < 0 ) sign=-1;
 else sign=+1;
  index= ((int)( value*10 )%64);
  index+=63;

 return sintab[index];
}


inline float COS(float value)
{
 signed int sign;
 signed int index;

 if ( value < 0 ) sign=-1;
 else sign=+1;
  index= ((int)( value*10 )%64);
  index+=63;

 return  costab[index];
}

FILE *output;
unsigned char vgascreen[64000];
unsigned char befehl[255], ssnum[255];
unsigned short sscount;

unsigned int letplus=0;

int xoff, yoff, counter2, counter, pixelnum, old_pix_num;
float distsample;
float x_turned, y_turned, z_turned;


unsigned char font[780]={

                         0,0,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,177,177,177,177,
                         0,177,0,0,0,177,
                         0,177,0,0,0,177,


                         0,177,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,177,177,177,0,

                         0,0,177,177,177,177,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,
                         0,0,177,177,177,177,

                         0,177,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,0,0,0,177,
                         0,177,0,0,0,177,
                         177,177,177,177,177,0,


                         177,177,177,177,177,177,
                         0,177,0,0,0,0,
                         0,177,177,177,177,0,
                         0,177,0,0,0,0,
                         177,177,177,177,177,177,

                         0,177,177,177,177,0,
                         0,177,0,0,0,0,
                         0,177,177,177,177,0,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,


                         0,0,177,177,177,0,
                         177,177,0,0,0,0,
                         177,0,0,177,177,177,
                         177,0,0,0,177,0,
                         0,177,177,177,177,0,

                         0,177,0,0,177,0,
                         0,177,0,0,177,0,
                         0,177,177,177,177,0,
                         0,177,0,0,177,0,
                         0,177,0,0,177,0,

                         0,0,177,0,0,0,
                         0,0,177,0,0,0,
                         0,0,177,0,0,0,
                         0,0,177,0,0,0,
                         0,0,177,0,0,0,


                         0,177,177,177,177,0,
                         0,0,0,0,0,177,
                         0,177,177,177,0,177,
                         0,0,177,0,0,177,
                         0,0,177,177,177,0,

                         0,177,0,0,177,0,
                         0,177,0,177,0,0,
                         0,177,177,0,0,0,
                         0,177,0,177,0,0,
                         0,177,0,0,177,0,


                         0,177,0,0,0,0,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,
                         0,177,177,177,177,0,

                         0,177,177,0,177,177,
                         177,0,0,177,0,177,
                         177,0,0,177,0,177,
                         177,0,0,0,0,177,
                         177,0,0,0,0,177,

                         177,177,0,0,0,177,
                         177,0,177,0,0,177,
                         177,0,177,0,0,177,
                         177,0,0,177,0,177,
                         177,0,0,0,177,177,


                         0,177,177,177,177,0,
                         177,0,0,0,0,177,
                         177,0,0,0,0,177,
                         177,0,0,0,0,177,
                         0,177,177,177,177,0,

                         0,177,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,177,177,177,0,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,


                         0,177,177,177,177,0,
                         177,0,0,0,0,177,
                         177,0,0,177,0,177,
                         177,0,0,0,177,0,
                         0,177,177,177,0,177,

                         0,177,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,177,177,177,0,
                         0,177,0,177,0,0,
                         0,177,0,0,177,177,

                         0,0,177,177,177,177,
                         0,177,0,0,0,0,
                         0,0,177,177,177,0,
                         0,0,0,0,0,177,
                         0,177,177,177,177,0,


                         177,177,177,177,177,177,
                         0,0,0,177,0,0,
                         0,0,0,177,0,0,
                         0,0,0,177,0,0,
                         0,0,0,177,0,0,

                         0,177,0,0,0,177,
                         0,177,0,0,0,177,
                         0,177,0,0,0,177,
                         0,177,0,0,0,177,
                         0,0,177,177,177,0,


                         0,177,0,0,0,177,
                         0,177,0,0,177,177,
                         0,177,0,0,177,0,
                         0,0,177,0,177,0,
                         0,0,0,177,0,0,

                         177,0,0,0,0,177,
                         177,0,0,0,0,177,
                         177,0,177,177,0,177,
                         177,0,177,177,0,177,
                         0,177,0,0,177,177,

                         177,0,0,0,177,0,
                         0,177,0,177,0,0,
                         0,0,177,0,0,0,
                         0,177,0,177,0,0,
                         177,0,0,0,177,0,


                         0,177,0,0,0,177,
                         0,0,177,0,0,177,
                         0,0,0,177,177,0,
                         0,0,0,177,0,0,
                         0,177,177,0,0,0,

                         0,177,177,177,177,177,
                         0,0,0,0,0,177,
                         0,0,0,177,177,0,
                         0,0,177,0,0,0,
                         0,177,177,177,177,177  };






struct
{
 float x,y,z;
 float x_angl, y_angl, z_angl;

} cam;

#define GRAV     0.0
#define MAXGRAV 0.0
#define GRAVNULL     0
#define GRAVSCHEITEL  0.00

#define MAXKRAFT 2.5 // war jetzt 2.5
#define ELANLOSS 0.0
#define LADNULL   100.5 // war 0.5
#define SCHEITEL  0.0   // war jetzt 10.0
#define LADUNG   1.0 // war 7.0
#define ELEK 1
#define PROT 0
#define NEUTR 2
#define TRAEGSCHWELLE 0.025 // war 0.2
#define STABIDAUER  5
#define VERLANGSFAC  0.99 // wegem Aetherwind nicht exakt 1
#define ABSTOSSIMBALANCE 1
#define TIMESLICE 20.0 //doppeldrei jetzt

int toffset, toffcount;


#define STRINT( START , GOAL, zug )\
strcpy(startstring[zug], START ), strcpy(compstring[zug],GOAL );\
  n=0;                                                \
\
 toffset=0; toffcount=0;\
  while ( toffcount < zug ) toffset+=TZahl[toffcount], toffcount++;\
 while ( n < TZahl[zug] )\
 {\
    teilchen[n+toffset].x=strstartx[zug] +n*8 ;\
  teilchen[n+toffset].y=strstarty[zug] +rand()%2*0.1 ;\
  teilchen[n+toffset].xv= 0;\
  teilchen[n+toffset].yv= 0;\
   teilchen[n+toffset].z=strstartz[zug];\
  teilchen[n+toffset].zv=0; \
  teilchen[n+toffset].instabil=0;\
  teilchen[n+toffset].elek_or_prot=PROT;\
   teilchen[n+toffset].symbol=startstring[zug][n];\
\
   n++;\
 }\
TEILCHENZAHL=n+toffset;\
\
     compc1=0, compc2=0;\
    while ( compc1 < strlen(compstring[zug]) && compc2 < strlen(compstring[zug]) )\
    {\
      while ( startstring[zug][compc1]!= compstring[zug][compc2] ) compc2++;\
      maplist[zug][compc1]=compc2, teilchen[compc1].goal=compc2;\
      compstring[zug][compc2]='@';\
      compc2=0;\
\
      compc1++;\
\
    }\

#define FRACTAL(STRINGS, SUBSTRINGS, SUBSTRINGG,X,Y ) \
n=0;\
xfract=X;\
yfract=Y;\
letterc=0;\
fracount=0;\
while ( letterc < strlen(STRINGS ) )\
{\
\
 y=0;\
 while ( y < 5 )\
 {\
  x=0;\
  while ( x < 6 )\
  {\
  strstartx[fracount]=x*40+letterc*320+X;\
  strstarty[fracount]=Y+y*10;\
  strstartz[fracount]=100;\
  if ( font[ (((STRINGS[letterc]-0x41)+letplus)%26)*30+x+y*6]!= 0 && STRINGS[letterc]!=' ' )\
                                               { STRINT(SUBSTRINGS, SUBSTRINGG,fracount) \
                                                 TZahl[fracount]=strlen(SUBSTRINGS);\
                                                   fracount++; }\
   x++;\
  }\
  y++;\
 }\
  letterc++;\
}

int main(void)
{

 unsigned char c;
 struct
 {
  int instabil;
  int elek_or_prot;
  float x,y,z;
   float xv,yv,zv;
 
   float goal;
   char symbol;
   int loose, blocked;
 } teilchen[1000]; // war 1000000

 int x,y, xfract, yfract;
 int frameskip=0;
 int pullrepeat=0;

 int TEILCHENZAHL=0;


 float power, old_power,
      x_add, y_add, z_add,
      x_buf, y_buf, z_buf,
      x_buf2, y_buf2, z_buf2,
      xtoy;
  int sign;
   int dim=1;
   float average=0.0;
   long int runs=0, setoff=0;


   signed int compc1,compc2;

   unsigned char startstring[100][300];
   unsigned char compstring[100][300];
  

   unsigned int maplist[100][300];
   unsigned int mapcount, mapend;

   int Tcount=0;
   int letterc=0;
   int fracount=0;

   int TZahl[1000];


    int n=0, n2=0;

   int strstartx[1000]; int strstarty[1000], strstartz[1000];

(pixelbuf)=malloc(sizeof(float)*550000*4);


  FRACTAL("VOID", "NEBEL", "LEBEN", -500,-30 )


  cam.x=0, cam.y=0, cam.z=-20;
  sininit(), cosinit();

while (1 )
{ 
  if ( runs%50==0 )
  {
   if ( strcmp ( startstring[0], "NEBEL" ) == 0 )
   {
     FRACTAL("VOID", "NEBEL", "LEBEN", -500,-20 )
     strcpy( startstring[0],"LEBEN");

   }
   else
   if ( strcmp ( startstring[0],"LEBEN" ) == 0 )
   {
     FRACTAL("VOID", "LEBEN", "ELBEN", -500,-20 )
     strcpy( startstring[0],"ELBEN");

    }
    else
    if ( strcmp ( startstring[0],"ELBEN")==0 )
    {

      FRACTAL("VOID", "ELBEN", "NEBEL", -500,-20 )
      strcpy( startstring[0],"NEBEL");


     }


  }

  compc1=0;
  compc2=0;
  Tcount=0;


  while ( Tcount < fracount )// war 5
  {

   /*
  while ( compc1 < strlen(compstring[Tcount] ) )
  {
    teilchen[compc1].loose=0;
    teilchen[compc1].blocked=0;
    
   compc1++;
  }
  compc1=0;

  compc1=0;
    */

    toffset=0; toffcount=0;
  while ( toffcount < Tcount ) toffset+=TZahl[toffcount], toffcount++;
 
  compc1=0;

  while ( compc1 < strlen(compstring[Tcount] ) )
  {

     compc2=maplist[Tcount][compc1];

     teilchen[compc1+toffset].loose=2;

     if ( teilchen[compc1+toffset].x - ( compc2*8 + strstartx[Tcount] ) > 1 ) teilchen[compc1+toffset].xv-=3;
     else
     if ( teilchen[compc1+toffset].x -( compc2*8 + strstartx[Tcount] ) < -1 ) teilchen[compc1+toffset].xv+=3;
     else { teilchen[compc1+toffset].xv=0, teilchen[compc1+toffset].loose--; }

     if ( teilchen[compc1+toffset].y > 0 +strstarty[Tcount] ) teilchen[compc1+toffset].yv-=3;
     else
     if ( teilchen[compc1+toffset].y - strstarty[Tcount] < 1 ) teilchen[compc1+toffset].yv+=3;
     else {  teilchen[compc1+toffset].loose-- ; }

     if ( teilchen[compc1+toffset].loose == 2 ) teilchen[compc1+toffset].loose--;

     //if ( teilchen[compc1].loose> 0 ) goto _2swapf;


   compc1++;
  }

   Tcount++;
  }
  _2swapf:



 /* Kraft zehren */

 n=0;

 while ( n < TEILCHENZAHL )
 {

   x_buf2=0, y_buf2=0, z_buf2=0;
   x_add=0; y_add=0; z_add=0;
   n2=0;
   while ( n2 < TEILCHENZAHL )
   {
    if ( n2==n ) n2++;
    if (  n2 >= TEILCHENZAHL ) break;
   
   power=
   sqrt ( 
    ( teilchen[n].x - teilchen[n2].x)  *
    ( teilchen[n].x - teilchen[n2].x )+
       ( teilchen[n].y - teilchen[n2].y) *
    ( teilchen[n].y - teilchen[n2].y)+
     ( teilchen[n].z - teilchen[n2].z) *
    ( teilchen[n].z - teilchen[n2].z) );
  
    old_power=power;

   x_buf=(teilchen[n].x - teilchen[n2].x);
   y_buf=(teilchen[n].y - teilchen[n2].y);
   z_buf=(teilchen[n].z - teilchen[n2].z);
  
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;

    if ( fabs(power) < 0.0001 ) power=sign*0.0001;

    power= ( LADUNG / (( power* power)+MAXKRAFT) ) *sign  ;

   if ( fabs ( power) >= LADNULL ) power=0; /* power=LADUNG*sign; debug! Energiebegrenzung */


          if ( old_power != 0 )
          {
          y_buf*=power/old_power;
          x_buf*=power/old_power;
          z_buf*=power/old_power;

        } else x_buf=0, y_buf=0, z_buf=0;


    if ( ( teilchen[n].elek_or_prot==PROT
        && teilchen[n2].elek_or_prot==ELEK ) ||
        ( teilchen[n].elek_or_prot==ELEK
        && teilchen[n2].elek_or_prot==PROT )
       
        ) {  y_buf*=-1.0; x_buf*=-1.0; z_buf*=-1.0; x_buf*=ABSTOSSIMBALANCE; y_buf*=ABSTOSSIMBALANCE; z_buf*=ABSTOSSIMBALANCE; }

    if (  (( ( teilchen[n].elek_or_prot==NEUTR && teilchen[n2].elek_or_prot==PROT ) ||
           ( teilchen[n].elek_or_prot==PROT && teilchen[n2].elek_or_prot==NEUTR ) ) && sign > 0 ) )
          x_buf*=0, y_buf*=0, z_buf*=0;
          
    if (  ( ( teilchen[n].elek_or_prot==NEUTR && teilchen[n2].elek_or_prot==ELEK ) ||
           ( teilchen[n].elek_or_prot==ELEK && teilchen[n2].elek_or_prot==NEUTR ) ) )
          x_buf*=0, y_buf*=0, z_buf*=0;

          
    if (  teilchen[n].elek_or_prot==NEUTR && teilchen[n2].elek_or_prot==NEUTR )
          x_buf*=0, y_buf*=0, z_buf*=0;


        if ( (old_power=sqrt ( x_buf*x_buf+y_buf*y_buf +z_buf*z_buf)) > TRAEGSCHWELLE )
   {
     /* power= (old_power-TRAEGSCHWELLE)/ old_power;
      x_buf*=power; y_buf*=power; z_buf*=power; */  // hier aufpassen geaendert
    }  else x_buf=0, y_buf=0, z_buf=0;

   if ( old_power > 0 && old_power > TRAEGSCHWELLE)
     {
      x_buf2+=(x_buf*ELANLOSS/TIMESLICE);
      y_buf2+=(y_buf*ELANLOSS/TIMESLICE);
      z_buf2+=(z_buf*ELANLOSS/TIMESLICE);
     }


   x_add+=x_buf; y_add+=y_buf; z_add+=z_buf;

    n2++;
   }

          teilchen[n].xv+=x_add;
          teilchen[n].yv+=y_add;
          teilchen[n].zv+=z_add;


   n++;
  }





  /* Bewegen */

   /* Nulltes steuern */

    if ( kbhit() )
    {

      c=getch();
      
      if(c=='u' ) cam.x-= -8*SIN(-cam.y_angl), cam.z-= -8*COS(-cam.y_angl);
      if(c=='j' ) cam.x+= -8*SIN(-cam.y_angl), cam.z+= -8*COS(-cam.y_angl);
      if(c=='h'  ) cam.y_angl+=0.04;
      if(c=='k' ) cam.y_angl-=0.04;
      if(c=='e') cam.y-=8;
      if(c=='d') cam.y+=8;
      if ( c=='+' && letplus < 27 ) { letplus++;  FRACTAL("VOID", "NEBEL", "LEBEN", -500,-30 )  runs=50; continue; }
      if ( c=='-' && letplus > 0 )  { letplus--;  FRACTAL("VOID", "NEBEL", "LEBEN", -500,-30 )  runs=50; continue; }
      if(c== 'p') getch();



    }

   /* bis hier */

   n=0;

  while ( n < TEILCHENZAHL )
  {
   if ( /* teilchen[n].loose== */ 1 )
   {

   teilchen[n].x+=teilchen[n].xv/TIMESLICE; teilchen[n].y+=teilchen[n].yv/TIMESLICE ;
    teilchen[n].z+=teilchen[n].zv/TIMESLICE;

    //if ( teilchen[n].elek_or_prot==ELEK ) // aujourd'hui
  { teilchen[n].xv*=VERLANGSFAC; teilchen[n].yv*=VERLANGSFAC;
   teilchen[n].zv*=VERLANGSFAC;
   }

   }

   n++;
  } 


  /* Darstellen */

  y=0;
  while ( y < 90 )
  {
   x=0;
   while (x < 160 )
   {
    screen[x][y]=' ';
    zbuf[x][y]=100000;
    x++;  
   }
   y++;
  }
  
  n=0;
   pixelnum=0;

counter2=0;


while( counter2 < TEILCHENZAHL ) /* fuer jeden einzelnen Pixel dessen Drehungen
                                anhand der Objektpositionierung berechnen */
{
 counter=0;

 yoff=0;
 while ( yoff < 5 )
 {
  xoff=0;
  while(xoff < 6)
  {
   if (  font[ (teilchen[counter2].symbol-0x41)*30+xoff+yoff*6 ] != 0  && teilchen[counter2].symbol!= ' ' )
   {
    pixelbuf[pixelnum][BUFX]=   teilchen[counter2].x +xoff;
    pixelbuf[pixelnum][BUFY]=  teilchen[counter2].y +yoff;
    pixelbuf[pixelnum][BUFZ]=  teilchen[counter2].z ;
    pixelbuf[pixelnum][VALUE]= (unsigned char)font[ (teilchen[counter2].symbol-0x41)*30+xoff+yoff*6 ];

     pixelnum++;
    }
     if ( pixelbuf[pixelnum-1][BUFZ]-cam.z > ZOOM*4 )    xoff+=2;
    else counter++;
                    /* unterschiedliche Pixelzahlen nach Abstand */

                    xoff++;
                    }

   yoff++;
  }

  counter2++;

}


 counter=0;

 old_pix_num=pixelnum;
 distsample=0.0;
 while ( counter < old_pix_num ) // Projektion aller Pixel zur Kamera, vorher: Kameradrehung berechnen
 {
  
    // y-Achse


       x_turned= +(pixelbuf[(int)counter][BUFX]-cam.x)*COS(cam.y_angl)+(pixelbuf[(int)counter][BUFZ]-cam.z)*SIN(cam.y_angl);
       y_turned=pixelbuf[(int)counter][BUFY];
       z_turned= -(pixelbuf[(int)counter][BUFX]-cam.x)*SIN(cam.y_angl)+(pixelbuf[(int)counter][BUFZ]-cam.z)*COS(cam.y_angl);

         pixelbuf[(int)counter][BUFX]=x_turned+cam.x;
        pixelbuf[(int)counter][BUFY]=y_turned;
        pixelbuf[(int)counter][BUFZ]=z_turned+cam.z;


        if ( z_turned < ZOOM )
        {

        #define DOUBLEFILL(xoff,yoff) \
         pixelbuf[(int)pixelnum][BUFX]=x_turned+xoff+cam.x; \
        pixelbuf[(int)pixelnum][BUFY]=y_turned+yoff;\
        pixelbuf[(int)pixelnum][BUFZ]=z_turned+cam.z;\
        pixelbuf[(int)pixelnum][VALUE]=pixelbuf[(int)counter][VALUE];\
\
         pixelnum++;\

         DOUBLEFILL (-1*z_turned/ZOOM,-1*z_turned/ZOOM)
         DOUBLEFILL (1*z_turned/ZOOM,-1*z_turned/ZOOM)
         DOUBLEFILL (0,-1*z_turned/ZOOM)
         DOUBLEFILL (-1*z_turned/ZOOM,0)
         DOUBLEFILL (1*z_turned/ZOOM,0 )
         DOUBLEFILL (-1*z_turned/ZOOM,1*z_turned/ZOOM)
         DOUBLEFILL (0,1*z_turned/ZOOM)
         DOUBLEFILL (1*z_turned/ZOOM,1*z_turned/ZOOM)

        }


  counter++;
 }


 
 while ( pixelnum > 0 )
 {
  pixelnum--;

   // die eigentliche Projektion

  if ( (pixelbuf[pixelnum][BUFZ]-(float)cam.z)/ZOOM>0  )
      {
       if( (79.0 + (pixelbuf[pixelnum][BUFX]-cam.x)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM)) < 160 && 47.0 + ((pixelbuf[pixelnum][BUFY]-cam.y)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM)) < 90.0 &&
           79.0 + ((pixelbuf[pixelnum][BUFX]-cam.x)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM)) >= 0 && 47.0 + ((pixelbuf[pixelnum][BUFY]-cam.y)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM)) >= 0   )
       if ( zbuf[ (signed int) (79.0 + ((pixelbuf[pixelnum][BUFX]-cam.x)/((pixelbuf[pixelnum][BUFZ]-(float)cam.z)/ZOOM))) ] [ (signed int)(47.0 + ((pixelbuf[pixelnum][BUFY]-cam.y)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM))) ] > (pixelbuf[pixelnum][BUFZ]-cam.z) )
            zbuf[ (signed int) (79.0 +
            ((pixelbuf[pixelnum][BUFX]-cam.x))/
            ((pixelbuf[pixelnum][BUFZ]-(float)cam.z)/ZOOM)) ]
            [ (signed int)(47.0 + ((pixelbuf[pixelnum][BUFY]-cam.y)/((pixelbuf[pixelnum][BUFZ]-(float)cam.z)/ZOOM))) ]
            =(pixelbuf[pixelnum][BUFZ]-cam.z),
       screen[ (signed int) (79.0 + ((pixelbuf[pixelnum][BUFX]-cam.x)/((pixelbuf[pixelnum][BUFZ]-(float)cam.z)/ZOOM))) ] [ (signed int)(47.0 + ((pixelbuf[pixelnum][BUFY]-cam.y)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM))) ]=pixelbuf[pixelnum][VALUE];
    }


 }

  
  system("cls\n");
  y=0;
  while ( y < 90 )
  {
   x=0;
   while (x < 160 )
   {
    printf("%c%c%c%c%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y],
                              screen[x+4][y],screen[x+5][y],screen[x+6][y],screen[x+7][y] );
    x+=8;
   }
   printf("\n");
   y++;
  }

 /*
   if ( c == 's' || 1)
 {

  y=0;
  while ( y < 200 )
  {
   x=0;
   while( x < 320 )
   {
    vgascreen[x+(199-y)*320]=screen[x*161/320][y*90/200];

   x++;
   }

   y++;
  }

  

  output=fopen("d:\\textanim\\scrshot.raw","wb");
  if ( output==NULL )
  {
   printf("E/A-Fehler.\n");
   return 1;
  }

 fwrite ( vgascreen, sizeof(unsigned char), 64000, output );
  fflush(output);
  fclose(output);


   strcpy(befehl,"copy /b d:\\textanim\\header.hdr+d:\\textanim\\scrshot.raw d:\\textanim\\seq\\ss");
  itoa(sscount,ssnum,10);
   strcat(befehl,ssnum);
   strcat ( befehl,".bmp\n");
  system(befehl);
   sscount++; 
 } */





   runs++;  /* if  ( runs==100 ) setoff=6; */


 }  

}
