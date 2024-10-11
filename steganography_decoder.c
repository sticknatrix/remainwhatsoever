/* ist die angegebene Frequenz im Signal enthalten, wird sie verstaerkt, weil auf jede Signalaenderung in der
 entsprechenden Richtung und Staerke eine Sinusschwingung nachhallt.
 Das erzeugt dann einen Resonanzeffekt.
 Raw oder .snd - Eingabedatei mit 8 Bit unsigned char, an der Kommandozeile die Frequenz angeben und dann verstÃ¤rkt
 es in der Ausgabe die gewuenschte Frequenz, falls da vorhanden.
 */


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char signal[3000000];
float signalb[4000];

void interpolate(long int start, long int count)
{
 signed long int buf, count2, off=0, end=0;

 end=2000/count;

while (off<end)
{
  buf=0;
  count2=count;
 while (count2 > 0){ 
   buf+=signal[start+count2+off*count]; count2--; }

   buf/=count;
   if ( buf > 255) buf=255;
   if ( buf < 0 ) buf=0;

  while ( count2 < count)
 {
  signalb[count2+off*count]=(unsigned char)buf;
 count2++;
 }
 off++;
 

}

}

float sintab[128];

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

float filterin[4000];


int main(int argc, char*argv[])
{
	int ready=0;

  FILE *input, *input2;
  signed int n=0, n2=0, n3=0, randoff=0, runs=0, filesize;
  float steigung, frequenz, frequenz_gen[1500],
  frequenz_ausgeb[1500];
float compfrequenz, smallest,
      biggest, loudest[1500],
	           
	  buf, lowersum;

unsigned int pos;
signed int old=-1;
int refcheck=0;

  if ( argc== 4) printf("Traegerfrequenz wuerde mit ausgegeben fuer Referenz.\n"),refcheck=1;
  if ( argc != 3 && argc!= 4)
  {
   printf("Falsche Anzahl Argumente! Zweites Argument Abspielgeschwindigkeit mal 100\n");
   return 1;
  
  }

  sininit();
  input=fopen(argv[1],"rb");


  if ( input == NULL )
  {
   printf("Datei nicht gefunden!\n");
   return 1;
  }
  if ( atoi(argv[2])<=0.0){ printf("gaebe Endlosschleife.\n");return 1;}

 //fread( signal, sizeof(unsigned char), 3000000, input);
  float nscale=0;
   int oldnscale;
   n=0;
   n2=0;
   unsigned char scanchar=fgetc(input);
  while ( ((nscale < 3000000&&atoi(argv[2])>100)||(n < 3000000&&atoi(argv[2])<=100)) &&!feof(input))
  {
  	//printf("%d %d\n",n,n2);
    	if ( (int)floor(nscale)>(int)oldnscale)scanchar=fgetc(input),n2++,oldnscale=floor(nscale);
 ((atoi(argv[2])>=100)?(signal[(int)(n2/(atoi(argv[2])/100.0))]=scanchar) : (signal[n]=scanchar)),nscale+=atoi(argv[2])/100.0;
  n++;
  }
 
 n=0;
 while ( n<100)printf("%c",signal[n]),n++;
 int tagpos=1,tagid=0;
 
   fseek(input, 0 , SEEK_END);
 filesize = ftell(input);
  if ( filesize > 3000000) { printf("Eingabedatei zu gross.\n");
   return 1;
  }
 fclose(input);


  n=0;
  while ( n < filesize )
  {
  	runs=0;
  	
#define BANDS 32
 
 pos=0;
 
   n3=0;
   while ( n3 < 1000) loudest[n3]=0.0, n3++;
 
 
 n2=0, n3=0;
 unsigned int ton=0, oktave=0;
 
 float CUTOFF;
 
 CUTOFF=3600.0;
 
   compfrequenz=1000;
   
	while ( compfrequenz < CUTOFF)
   {
   	
   	 frequenz= 3.14 /(44100/compfrequenz) ;
    
   n2=0;
   while ( n2 < 4000 )
   {
    filterin[n2]=0.0;n2++;
    
   }

#define OVERSKIPP  (44100.0/(compfrequenz*4)) 

   interpolate(n,OVERSKIPP);  
   
      
    
    n3=OVERSKIPP;
     while ( n3 < 2000)
    {
     steigung= signalb[n3]-signalb[n3-(int)(OVERSKIPP)];


     if ( fabs(steigung) > 0.0)
	 {	        
	 float nfloat;
	 nfloat=0.0;
     n2=0;
     while (  n2 < 2000 )
     {
     buf=  SIN((float)n2*frequenz)*steigung;
       filterin[  (int)(n2+n3)  ]+=buf; 
       
	   n2+=OVERSKIPP; 
       
	  }
    }
     n3+=OVERSKIPP;
    }

    n2=0;
    smallest=0.0;
    biggest=0.0;
   
     while ( n2< 4000 )
    {
	  if ( filterin[n2]/(compfrequenz) < smallest) smallest=filterin[n2]/(compfrequenz);
      if ( filterin[n2]/(compfrequenz) > biggest ) biggest=filterin[n2]/(compfrequenz);
     n2++;
    }
    
 
    steigung=fabs(biggest-smallest);
   
   frequenz_gen[pos]=3.14/(44100/compfrequenz), frequenz_ausgeb[pos]=compfrequenz, loudest[pos]=steigung;
   pos++;
   
   
   
 
    
   compfrequenz*=1.01;
  }


n3=1;

while ( n3 < 1000)
{
 n2=1000;
 while (n2 > n3)
 {
  if ( loudest[n2] > loudest[n2-1] /*&&(fabs(frequenz_ausgeb[n2]-697*2)<100||fabs(frequenz_ausgeb[n2]-1300*2)<500)*/) 
    {
    	n3=0;
   	
    	float fbuf, fgbuf, lobuf;
  
 	 fbuf=frequenz_gen[n2-1], fgbuf=frequenz_ausgeb[n2-1], lobuf=loudest[n2-1];
	 frequenz_gen[n2-1]=frequenz_gen[n2], frequenz_ausgeb[n2-1]=frequenz_ausgeb[n2],loudest[n2-1]=loudest[n2],
	 frequenz_gen[n2]=fbuf, frequenz_ausgeb[n2]=fgbuf, loudest[n2]=lobuf; 
   }
  n2--;
  }
  
  n3++;
 }
 
 // veraestelt theoretisch koennte Er beim Umsortieren noch einmal Wertemuster liefern wo er kannt wuerden?!?
 n3=0;
 signed int n4;
 n4=0;
 while ( n4 < BANDS-n3)
 {
  n2=0;
  while ( n2 < BANDS-n3)
  {
  	if ( n4==n2) { n2++; continue; }
  	 if ( fabs ( frequenz_ausgeb[n4]-frequenz_ausgeb[n2]) <= 17 )
	   {
	    int n5;
	    
	    n5=n2+1;
	    while (n5 < BANDS-n3)
	   frequenz_ausgeb[n5-1]=frequenz_ausgeb[n5], loudest[n5-1]=loudest[n5],n5++;	
    
    
       n2--;
       n3++;
       }
	n2++;
	  }	
 	n4++;
 }


frequenz_ausgeb[0]/=2.0, frequenz_ausgeb[1]/=2.0;
if (refcheck==1)printf("Debugscale %f %f\n",frequenz_ausgeb[0],frequenz_ausgeb[1]);
//printf("%f %f\n", frequenz_ausgeb[0], frequenz_ausgeb[1] );

//printf("%c %f %f %f %f Laut 0: %f Laut 1: %f\n", old== -1 ? ' ' : old, frequenz_ausgeb[0], frequenz_ausgeb[1], frequenz_ausgeb[2], frequenz_ausgeb[3], loudest[0], loudest[1]);
   if ( loudest[0]> 0.05 && loudest[1] > 0.05 )
   {
    unsigned int silent;
    silent=1;
    
    if ((  (fabs(frequenz_ausgeb[0]-697) < 100 && fabs(frequenz_ausgeb[1]-1209 ) < 100  ) ||
          (fabs(frequenz_ausgeb[0]-1209) < 100 && fabs(frequenz_ausgeb[1]-697) < 100  )  )
	    )  (old!= 's' ? printf("-sync-") : 0,(old='s')), silent=0;
     else
    
    if ((  (fabs(frequenz_ausgeb[0]-697) < 100 && fabs(frequenz_ausgeb[1]-1477 ) < 100 ) ||
          (fabs(frequenz_ausgeb[0]-1477) < 100 && fabs(frequenz_ausgeb[1]-697) < 100   ) )
	    )  (old!= '1' ? printf("-Bit1-") : 0,(old='1')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-697) < 100 && fabs(frequenz_ausgeb[1]-1633 ) < 100 ) ||
          (fabs(frequenz_ausgeb[0]-1633) < 100 && fabs(frequenz_ausgeb[1]-697) < 100  ) )
	    )  (old!= '0' ? printf("-Bit0-") : 0,(old='0')), silent=0;// ist nicht mehr dtmf 100 Hz mehr Abstand?!?
     /*
	 else
    if ((  (fabs(frequenz_ausgeb[0]-770) < 20 && fabs(frequenz_ausgeb[1]-1209 ) < 30 ) ||
          (fabs(frequenz_ausgeb[0]-1209) < 30 && fabs(frequenz_ausgeb[1]-770) < 20  ) )
	    )  (old!= '4' ? printf("-4-") : 0,(old='4')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-770) < 20 && fabs(frequenz_ausgeb[1]-1336 ) < 30 ) ||
          (fabs(frequenz_ausgeb[0]-1336) < 30 && fabs(frequenz_ausgeb[1]-770) < 20  ) )
	    )  (old!= '5' ? printf("-5-") : 0,(old='5')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-770) < 20 && fabs(frequenz_ausgeb[1]-1477 ) < 30 ) ||
          (fabs(frequenz_ausgeb[0]-1477) < 30 && fabs(frequenz_ausgeb[1]-770) < 20  ) )
	    )  (old!= '6' ? printf("-6-") : 0,(old='6')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-852) < 30 && fabs(frequenz_ausgeb[1]-1209 ) < 30  ) ||
          (fabs(frequenz_ausgeb[0]-1209) < 30 && fabs(frequenz_ausgeb[1]-852) < 30  ) )
	    )  (old!= '7' ? printf("-7-") : 0,(old='7')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-852) < 20 && fabs(frequenz_ausgeb[1]-1336 ) < 30 ) ||
          (fabs(frequenz_ausgeb[0]-1336) < 30 && fabs(frequenz_ausgeb[1]-852) < 20 )  )
	    )  (old!= '8' ? printf("-8-") : 0,(old='8')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-852) < 20 && fabs(frequenz_ausgeb[1]-1477 ) < 30 ) ||
          (fabs(frequenz_ausgeb[0]-1477) < 30 && fabs(frequenz_ausgeb[1]-852) < 30  ) ) 
	    )  (old!= '9' ? printf("-9-") : 0,(old='9')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-941) < 20 && fabs(frequenz_ausgeb[1]-1209 ) < 30  ) ||
          (fabs(frequenz_ausgeb[0]-1209) < 30 && fabs(frequenz_ausgeb[1]-941) < 20  ) ) 
	    )  (old!= '*' ? printf("-*-") : 0,(old='*')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-941) < 20 && fabs(frequenz_ausgeb[1]-1336 ) < 30  ) ||
          (fabs(frequenz_ausgeb[0]-1336) < 30 && fabs(frequenz_ausgeb[1]-941) < 20  ) ) 
	    )  (old!= '0' ? printf("-0-",old) : 0,(old='0')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-941) < 20 && fabs(frequenz_ausgeb[1]-1447 ) < 30 ) ||
          (fabs(frequenz_ausgeb[0]-1447) < 30 && fabs(frequenz_ausgeb[1]-941) < 20  ) ) 
	    )  (old!= '#' ? printf("-#-") : 0,(old='#')), silent=0;
     else
     
     
     
    if ((  (fabs(frequenz_ausgeb[0]-697) < 15 && fabs(frequenz_ausgeb[1]-1633 ) < 30 )||
          (fabs(frequenz_ausgeb[0]-1633) < 30 && fabs(frequenz_ausgeb[1]-697) < 15  ) ) 
	    )  (old!= 'A' ? printf("-A-") : 0,(old='A')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-770) < 9 && fabs(frequenz_ausgeb[1]-1633 ) < 30  ) ||
          (fabs(frequenz_ausgeb[0]-1633) < 30 && fabs(frequenz_ausgeb[1]-770) < 9  ) )
	    )  (old!= 'B' ? printf("-B-") : 0,(old='B')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-852) < 30 && fabs(frequenz_ausgeb[1]-1633 ) < 30 ) ||
          (fabs(frequenz_ausgeb[0]-1633) < 30 && fabs(frequenz_ausgeb[1]-852) < 30  ) )
	    )  (old!= 'C' ? printf("-C-") : 0,(old='C')), silent=0;
     else
    if ((  (fabs(frequenz_ausgeb[0]-941) < 20 && fabs(frequenz_ausgeb[1]-1633 ) < 30 )||
          (fabs(frequenz_ausgeb[0]-1633) < 30 && fabs(frequenz_ausgeb[1]-941) < 20 ) ) 
	    )  (old!= 'D' ? printf("-D-") : 0,(old='D')), silent=0;
	    else if ( silent==1)old=-1;
    
	 */
	 if ( tagpos<512)
	 {
	 if ( old=='s')ready=1;
	 if ( old=='1'&&ready==1)tagid+=tagpos*1, tagpos<<=1,ready=0;
	 if ( old=='0'&&ready==1)tagid+=tagpos*0, tagpos<<=1,ready=0;
     }
     }  else old=-1;
        
   /*
   Zeichen	Frequenz 1 [Hz]	Frequenz 2 [Hz]
1	697	1209
2	697	1336
3	697	1477
4	770	1209
5	770	1336
6	770	1477
7	852	1209
8	852	1336
9	852	1477
*	941	1209
0	941	1336
#	941	1447
A	697	1633
B	770	1633
C	852	1633
D	941	1633 

   */
   
  
  
  
  n+=500;
     

  }
  printf(" %d ",tagid);


}
