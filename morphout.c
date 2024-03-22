#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*
float sqrtab[1000000];

void sqrinit(void)
{
 int count=0;
 while ( count < 1000000 )
 {
  sqrtab[count]=sqrt((float)count/1000.0 ) ;
  count++;
 }

}


float SQRT(float quad)
{
  return ( sqrt(quad) );
 if ( quad < 0.0 ) quad*=-1.0 ;


 if (quad > 999 ) return ( sqrt(quad) );

 return sqrtab[ (unsigned int)( quad*1000.0) ];

}


*/
unsigned char bild[64000];
unsigned char screen[100][70];

unsigned int teilchennum=0;

int main(int argc, char*argv[])
{

  FILE *input, *input2;
  signed int n=0, n2=0, simruns=0, skip=0 ;
  int x, y;
  
struct
 {
  float x,y,z;
  float xv,yv,zv;
  //float xv_old, yv_old, zv_old;

  unsigned char lume;

 }teilchen[7100];
   float power, old_power, x_buf, y_buf,z_buf, sign;

 #define ATOMNUM 7000

  double abweich=0.0, old_abweich=1000000.0, pixdiff ;

  
  if ( argc != 2 )
  {
   printf("Falsche Anzahl Argumente!\n");
   return 1;
  
  }

  //sqrinit();

  input=fopen(argv[1],"rb");


  if ( input == NULL )
  {
   printf("Datei nicht gefunden!\n");
   return 1;
  }

 n=0;
 while ( n < 7000 )
 {
  teilchen[n].lume= fgetc(input);
  teilchen[n].x=(n%100)*4;
  teilchen[n].y=(n/100)*4;
  teilchen[n].xv=0; //, teilchen[n].xv_old=0;
  teilchen[n].yv=0; //, teilchen[n].yv_old=0;
  teilchen[n].zv=0; //, teilchen[n].zv_old=0;

  n++;

 }

 fclose(input);

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
      else if ( simruns==3 && n < 6900 ) n2=n+100;
      else { simruns++; continue; }
      

      //if (!( n2== n-100 || (n2== n -1 && n%100 > 0 ) || (n2== n +1&&n%100<99) || n2== n + 100 ) ) {
       // n2++; continue; }
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
  #define LADUNG   20.0      /* neu --> war 6.0 */
  #define LADNULL 100000.5
  #define TRAEGSCHWELLE 0.0 /* ist die Kraft schwaecher als das, findet keine Abgabe statt */
  #define ELANLOSS 0.0 /* beim unelastischen Stoss mogelt man eine Koordinatenaenderung rein */
  #define MAXKRAFT 10.0 /* so wird die aus der Distanz sich ergebende Kraft nie zu gross */
  #define PROT 1
  #define ELEK 0

  #define TIMESLICE 45.0
  #define VERLANGSFAC 0.0 /* allgemeiner Verlust Richtung 0, vielleicht wegen der Schwerkraft */

   if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
    power=power-SCHEITEL;

    //power= ( LADUNG / ((power *power )+MAXKRAFT) *sign  ) ;

    power=power*power*sign; // wie bei Elastic Nodes

    //if ( fabs ( power) > LADNULL ) power=0; /*  Energiebegrenzung */

          if ( old_power != 0 )
          {
          y_buf*=power /old_power;
          x_buf*=power/old_power;
          z_buf*=power/old_power;
        } else x_buf=0, y_buf=0, z_buf=0;



            
           //if ( n2== n-100 || (n2== n -1 && n%100 > 0 ) || (n2== n +1&&n%100<99) || n2== n + 100 ) {
           x_buf*=4, y_buf*=4, z_buf*=4; // Kohaerenz 
           
            /*
           if ((n2/100)%2 != (n/100)%2)
           {
            y_buf*=-1.0; x_buf*=-1.0; z_buf*=-1.0;
           } 


            if ( n2%2 != n%2 )
           { */

           
            y_buf*=-1.0; x_buf*=-1.0; z_buf*=-1.0; // }
             //else x_buf=0, y_buf=0, z_buf=0;


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
 
    
   teilchen[n].x+=(teilchen[n].xv/TIMESLICE); teilchen[n].y+=(teilchen[n].yv/TIMESLICE) ;
    teilchen[n].z+=(teilchen[n].zv/TIMESLICE);

//teilchen[n].xv*=VERLANGSFAC; teilchen[n].yv*=VERLANGSFAC; teilchen[n].zv*=VERLANGSFAC;
 teilchen[n].xv=0; teilchen[n].yv=0; teilchen[n].zv=0;

/*
 teilchen[n].xv=(teilchen[n].xv_old+teilchen[n].xv)/2.0;
 teilchen[n].yv=(teilchen[n].yv_old+teilchen[n].yv)/2.0;
teilchen[n].zv=(teilchen[n].zv_old+teilchen[n].zv)/2.0;
teilchen[n].xv_old=teilchen[n].xv;
 teilchen[n].yv_old=teilchen[n].yv;
teilchen[n].zv_old=teilchen[n].zv; */

   n++;
  }

       // Problem mit Distanz und power*power ? Ueberlauf?!
       // braucht es Abstossung?
       // Alignment+float-Probleme?
       // Zeit verläuft exponentiell immer schneller?
       // Klebephase bei Stoß ?!

     teilchennum=2025;
     while ( teilchennum < 3375) { if ( teilchennum%100 > 75 ) teilchennum+=50;
          teilchen[teilchennum].zv+=5.0, teilchennum++; }
     teilchennum=3300;
     while ( teilchennum < 3500) printf("%f %f-- ", teilchen[teilchennum].y, teilchen[teilchennum].z ), teilchennum+=50;
      

       

  skip++;
  if ( skip%10==0)
  {

  n=0;
  //while ( n < 7000)bild[n]=0, n++;
  n=0;
  while ( n < ATOMNUM )
  {
    
    if ( (int)teilchen[n].x/4 >= 0 && ((int)teilchen[n].x+1)/4 < 100 && ((int)teilchen[n].y+1)/4 < 70 && (int)teilchen[n].y/4 >= 0 )
    {
     bild[((int)teilchen[n].x/4)+((int)teilchen[n].y/4)*100]=teilchen[n].lume;
     bild[( ((int)teilchen[n].x+1) /4)+((int)teilchen[n].y/4)*100]=teilchen[n].lume;
     bild[((int)teilchen[n].x/4)+( ((int)teilchen[n].y+1)/4)*100]=teilchen[n].lume;
     bild[( ((int)teilchen[n].x+1)/4)+( ((int)teilchen[n].y+1)/4)*100]=teilchen[n].lume;

    }

    /*
    if ( teilchen[n].x/1 > 0 && teilchen[n].x/1 < 100 && teilchen[n].y/1 < 70 && teilchen[n].y/1 >= 0 )
    {
     bild[((int)teilchen[n].x/1)+((int)teilchen[n].y/1)*100]=teilchen[n].lume;
     bild[( ((int)teilchen[n].x+1) /1)+((int)teilchen[n].y/1)*100]=teilchen[n].lume;
     bild[((int)teilchen[n].x/1)+( ((int)teilchen[n].y+1)/1)*100]=teilchen[n].lume;
     bild[( ((int)teilchen[n].x+1)/1)+( ((int)teilchen[n].y+1)/1)*100]=teilchen[n].lume;

    }
   */

     n++;

    }
    // bild[(int)teilchen[teilchennum].x+(int)teilchen[teilchennum].y*100]='-';

    y=0;
    system("cls");
    while ( y < 70 )
    {
     x=0;
     while( x < 100 )
     {
       if ( bild[x+y*100]< 64 ) screen[x][y]=' ';
       else
       if ( bild[x+y*100]< 128 ) screen[x][y]=176;
       else
       if ( bild[x+y*100] < 192 ) screen[x][y]= 177;
       else if ( bild[x+y*100] < 245 ) screen[x][y]='@';
       else screen[x][y]= 219;

       if ( x%5== 0 && x > 0 )
       printf("%c%c%c%c%c",screen[x-5][y],screen[x-4][y],screen[x-3][y],screen[x-2][y],screen[x-1][y] );

      x++;
     }
     printf("\n");
     y++;
    }
     
      
    
    if ( kbhit() )
    {
     switch(getch())
     {
      case 'e' : printf("Welches Objekt verschieben (0-7000 ) \?\n"), scanf("%d",&teilchennum), teilchennum%=7000; break;
      case 'g' : teilchen[teilchennum].zv-=100; break;
      case 'b' : teilchen[teilchennum].zv+=100; break;
     
      case 'h' : teilchen[teilchennum].xv-=100; break;
      case 'k' : teilchen[teilchennum].xv+=100; break;
      case 'j' : teilchen[teilchennum].yv+=100; break;
      case 'u': teilchen[teilchennum].yv-=100; break;
      case 'w' : input=fopen(".\\morphout.raw","wb"), fwrite(bild,sizeof(unsigned char),7000,input), fclose(input); break;

     }

    }
    
   }
  }


 return ;
}
