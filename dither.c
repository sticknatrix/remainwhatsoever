#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCHWELLE schwelle
#define SHIFT 3
#define TOINIT 257

unsigned char bild[64000];
unsigned char maske[64000];
unsigned char ausgabe[64000];

int iter_buf[64000][4];
int iter_count;

float mvalue, mvalue_sum;
float hellst, dunkelst;
long int rec_count;
signed long int abbruch;
long int highest_depth;
int blackorwhite;

  int distance=0;
  int flip=0;

long int schwelle;

void backsort(void)
{
 int count1, count2;
 int buf[4];

 count2=0;

 while ( count2 < iter_count )
 {

   count1=iter_count-1;
  while ( count1 > count2 )
  {

  if ( iter_buf[count1][2] < iter_buf[count1-1][2] )
   {

     buf[0]=iter_buf[count1-1][0];
     buf[1]=iter_buf[count1-1][1];
     buf[2]=iter_buf[count1-1][2];
     buf[3]=iter_buf[count1-1][3];

     iter_buf[count1-1][0]=iter_buf[count1][0];
     iter_buf[count1-1][1]=iter_buf[count1][1];
     iter_buf[count1-1][2]=iter_buf[count1][2];
     iter_buf[count1-1][3]=iter_buf[count1][3];

     iter_buf[count1][0]=buf[0];
     iter_buf[count1][1]=buf[1];
     iter_buf[count1][2]=buf[2];
     iter_buf[count1][3]=buf[3];

   }

   count1--;

  }

  count2++;

 }

 /* Kettendumpen debug

 count2=0;
 while( count2 < iter_count )
 {
  printf("Position %d X: %d Y: %d Aktivierung %d Zaehlerwert %d\n",
         count2, iter_buf[count2][0], iter_buf[count2][1], iter_buf[count2][3],
          iter_buf[count2][2] );

  count2++;
 }

 bis hier */

}


void insert(int x, int y, int depth)
{
  float buf;

 if ( hellst==TOINIT ) hellst=bild[x+y*320];
 if ( dunkelst==TOINIT ) dunkelst=bild[x+y*320];
 
  if ( bild[x+y*320] > hellst )
    /* if ( abs( (float)bild[x+y*320]-dunkelst ) < SCHWELLE  ) */
        hellst=bild[x+y*320];
     /*    else return; */
     
  if ( bild[x+y*320] < dunkelst )
    /* if ( abs( (float)hellst-bild[x+y*320] ) < SCHWELLE  ) */
        dunkelst=bild[x+y*320];
     /*    else return; */


     if ( rec_count > 0 )
     {
      buf= mvalue_sum/rec_count;


         /*
      if ( abs ( hellst-buf ) > SCHWELLE ) { return; }
      if ( abs ( dunkelst-buf ) > SCHWELLE ) { return; }

         */

      if ( rec_count > 1024 ) return ;


       if ( abs ( hellst /* bild[x+y*320] */ - buf ) > SCHWELLE || abs ( dunkelst - buf ) > SCHWELLE ) return;


     }


  mvalue_sum+=bild[x+y*320];
  rec_count++;

   maske[x+y*320]=1;



 iter_buf[iter_count][0]=x;
 iter_buf[iter_count][1]=y;
 iter_buf[iter_count][2]=depth;
 iter_buf[iter_count][3]=0;
 iter_count++;

}






void dinsert(int x, int y, int depth )
{
  float buf;

 if ( hellst==TOINIT ) hellst=bild[x+y*320];
 if ( dunkelst==TOINIT ) dunkelst=bild[x+y*320];
 
  if ( bild[x+y*320] > hellst )
    /* if ( abs( (float)bild[x+y*320]-dunkelst ) < SCHWELLE  ) */
        hellst=bild[x+y*320];
     /*    else return; */
     
  if ( bild[x+y*320] < dunkelst )
    /* if ( abs( (float)hellst-bild[x+y*320] ) < SCHWELLE  ) */
        dunkelst=bild[x+y*320];
     /*    else return; */


     if ( rec_count > 0 )
     {
      buf= mvalue_sum/rec_count;


         /*
      if ( abs ( hellst-buf ) > SCHWELLE ) { return; }
      if ( abs ( dunkelst-buf ) > SCHWELLE ) { return; }

         */

      if ( rec_count > 1024 ) return ;


       if ( abs ( hellst /* bild[x+y*320] */ - buf ) > SCHWELLE || abs ( dunkelst - buf ) > SCHWELLE  ){ return; }


     }

  if ( blackorwhite==0) { if ( depth%(distance+(flip&1))==0)  ausgabe[x+y*320]=255; }
  else { if ( depth%(distance+(flip&1))!= 0 ) ausgabe[x+y*320]=255; }


  rec_count++;
  if ( rec_count%2 != 0 ) flip++;

   mvalue_sum+=bild[x+y*320];
   maske[x+y*320]=1;



 iter_buf[iter_count][0]=x;
 iter_buf[iter_count][1]=y;
 iter_buf[iter_count][2]=depth;
 iter_buf[iter_count][3]=0;
 iter_count++;


}







int flood(int x, int y, int depth )
{
  int buf2; int buf3;

  if ( iter_count==0 ) insert ( x, y, depth );


   buf2=0; while ( iter_buf[buf2][3]==1 ) buf2++;

 x=iter_buf[buf2][0];
 y=iter_buf[buf2][1];
 depth=iter_buf[buf2][2];
 iter_buf[buf2][3]=1;


 /* printf("%d %d \n", x,y ); */



    if ( x < 319 ) if ( maske[x+1+y*320]==0 )
    {
     insert( x+1, y , iter_buf[0][2]+1 );

     }
    if ( x > 0 )  if ( maske[x-1+y*320]==0 )
    {
     insert( x-1, y , iter_buf[0][2]+1 );
  
    }


    if ( y < 199 ) if ( maske[x+(y+1)*320]==0 )
    {
     insert( x, y+1 , iter_buf[0][2]+1 );

    }
    if ( y > 0 ) if ( maske[x+(y-1)*320]==0 )
    {
     insert( x, y-1 , iter_buf[0][2]+1 );

    }


    /*
    
    if ( x < 319 && y < 199 ) if ( maske[x+1+(y+1)*320]==0 )
    {
     insert( x+1, y +1, depth+1 );

     }
    if ( x > 0 && y > 0)  if ( maske[x-1+(y-1)*320]==0 )
    {
     insert( x-1, y-1 , depth+1 );
  
    }
    if ( y < 199 && x > 0) if ( maske[(x-1)+(y+1)*320]==0 )
    {
     insert( x-1, y+1 , depth+1 );

    }
    if ( y > 0 && x < 319 ) if ( maske[(x+1)+(y-1)*320]==0 )
    {
     insert( x+1, y-1 , depth+1 );

    }

      */



       abbruch= -1;
   buf3=0; while ( buf3 < iter_count ) { if ( iter_buf[buf3][3]==0 ) abbruch=1; buf3++; }


}


int dither(int x, int y, int depth )
{
  int buf2; int buf3;

  if ( iter_count==0 ) dinsert ( x, y, depth );


   buf2=0; while ( iter_buf[buf2][3]==1 ) buf2++;

 x=iter_buf[buf2][0];
 y=iter_buf[buf2][1];
 depth=iter_buf[buf2][2];
 iter_buf[buf2][3]=1;


 /* printf("%d %d \n", x,y ); */


    if ( x < 319 ) if ( maske[x+1+y*320]==0 )
    {
     dinsert( x+1, y , depth+1 );

     }
    if ( x > 0 )  if ( maske[x-1+y*320]==0 )
    {
     dinsert( x-1, y , depth+1 );
  
    }


    if ( y < 199 ) if ( maske[x+(y+1)*320]==0 )
    {
     dinsert( x, y+1 , depth+1 );

    }
    if ( y > 0 ) if ( maske[x+(y-1)*320]==0 )
    {
     dinsert( x, y-1 , depth+1 );

    }

    /*

    if ( x < 319 && y < 199 ) if ( maske[x+1+(y+1)*320]==0 )
    {
     dinsert( x+1, y+1 , depth+1 );

     }
    if ( x > 0 && y > 0)  if ( maske[x-1+(y-1)*320]==0 )
    {
     dinsert( x-1, y-1 , depth+1 );
  
    }
    if ( y < 199 && x > 0) if ( maske[(x-1)+(y+1)*320]==0 )
    {
     dinsert( x-1, y+1 , depth+1 );

    }
    if ( y > 0 && x < 319 ) if ( maske[(x+1)+(y-1)*320]==0 )
    {
     dinsert( x+1, y-1 , depth+1 );

    }


    */


       abbruch= -1;
   buf3=0; while ( buf3 < iter_count ) { if ( iter_buf[buf3][3]==0 ) abbruch=1; buf3++; }


}






int main(int argc, char*argv[])
{

  FILE *input, *output;
  int n=0, n2=0;
  
  
  input=fopen(argv[1],"rb");
  
  if ( argc != 4 )
  {
   printf("Falsche Anzahl Argumente!\n");
   return 1;
  
  }

  schwelle=atoi(argv[3] );

  if ( input == NULL )
  {
   printf("Datei nicht gefunden!\n");
   return 1;
  }


  output=fopen(argv[2],"wb");
  if ( output==NULL )
  {
   printf("E/A-Fehler.\n");
   return 1;
  }

 fread( bild, sizeof(unsigned char), 64000, input);
 fclose(input);


  n=0;
 while(n<64000) { maske[n]=0; ausgabe[n]=0; n++; }
 
 n2=0;
 while ( n2 < 64000)
 {
  n2=0;
  rec_count=0;
  abbruch=0;
  highest_depth=1;

  mvalue_sum=0;
  hellst=TOINIT, dunkelst=TOINIT;

  while( maske[n2]!=0 && n2 < 64000) n2++;
  if ( n2== 64000 ) break;

   printf("Flute die Koordinaten %d %d\n", n2%320, n2/320 ); /*debug*/

   iter_count=0;

   do{  flood(n2%320, n2/320 , 1 ); backsort(); }  while ( abbruch >= 0 ) ;

  if ( rec_count > 0 ) mvalue=mvalue_sum/rec_count;

    printf("Der Wert lautet: %f\n", mvalue );

    rec_count=0;
  abbruch=0;
  highest_depth=1;

  hellst=TOINIT, dunkelst=TOINIT;

     iter_count=0;

     if ( mvalue < 128 )
     {
      
      if ( mvalue > 0 ) distance= 255/mvalue; else
                 distance=2048;

      blackorwhite=0;
      }
      else
      {
       if ( mvalue < 255 )distance=255/(255-mvalue);
                        else distance=2048;
       blackorwhite=1;

      }

          mvalue_sum=0;

             n=0;
               while(n < 64000 )
      {
         if ( maske[n]==1 ) maske[n]=0;
       n++;
      }

     n2=0;
     while( maske[n2]!=0 && n2 < 64000) n2++;
     if ( n2== 64000 ) break;

     printf("Dithere ...\n");
   do{  dither(n2%320, n2/320 , 1 ); backsort(); }  while ( abbruch >= 0 ) ;

  n=0;
  while(n < 64000 )
  {
   if ( maske[n]==1 ) maske[n]=2;
   n++;
  }
  
  
  n2++;
 }  

 fwrite ( ausgabe, sizeof(unsigned char), 64000, output );
 fflush(output );

  fclose(output);

}
