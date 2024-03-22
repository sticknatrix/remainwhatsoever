/* Wir sehen sieben 6x5 Muster.
  Ins Arbeitsgedaechtniss passen 7 Objekte.
  Allein das stimmt so nicht raeumlich und so?!
  Wir haben 5 Finger.
  */
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  
  
  #define RANGESCALE 2
  
  //#define ROTATION ((rand()%614)*0.01)
  
  #define ROTATION ((rand()%20-20)*0.01)
  #define SCALING  ((rand()%3)+4)
  
  
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

#define SIN(val) sintab[(int)((val)*10) %64 +63]
#define COS(val) costab[(int)((val)*10) %64 +63]

  
  
  
  
  unsigned char font[972]={

                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         1,1,1,1,1,1,


                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         0,1,1,1,1,0,

                         0,1,1,1,1,0,
                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         0,1,1,1,1,0,

                         0,0,0,0,0,1,
                         0,1,0,0,0,1,
                         0,1,0,0,0,1,
                         0,1,0,0,0,1,
                         0,1,0,0,0,1,
                         0,1,1,1,1,0,



                         1,0,1,0,1,0,
                         0,1,0,1,0,1,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,


                         0,1,0,1,1,0,
                         1,0,1,0,0,1,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,


                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,1,1,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,

                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,1,1,1,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,

                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,1,1,0,0,
                         0,0,0,0,0,0,


                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,1,1,1,1,0,
                         0,0,0,0,0,0,


                         0,0,0,0,0,0,
                         0,0,1,0,1,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,


                         0,0,0,0,0,0,
                         0,1,1,0,1,1,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,

                         0,0,0,0,0,0,
                         0,1,0,0,1,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,

                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         0,0,0,0,0,0,
                          0,0,0,0,0,0,


                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         1,0,0,0,0,1,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,


                         0,0,0,0,0,0,
                         0,1,1,0,1,1,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,


                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,1,0,0,1,0,
                         0,1,0,0,1,0,
                         0,0,0,0,0,0,


                         1,1,1,1,1,1,
                         1,1,0,0,1,1,
                         1,1,0,0,1,1,
                         1,1,0,1,0,1,
                         1,1,0,0,0,1,
                         1,1,0,0,0,1,

                         0,0,0,0,0,0,
                         0,1,0,0,1,0,
                         0,0,1,1,0,0,
                         0,0,1,1,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,


                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,1,1,0,0,
                         0,1,1,1,1,0,
                         0,0,0,0,0,0,

                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         1,0,0,0,0,1,
                         1,1,0,0,1,1,
                         1,1,1,1,1,1,
                         0,1,1,1,1,0,


                         0,0,0,0,0,0,
                         0,1,1,0,1,1,
                         0,0,1,0,1,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,

                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         0,1,0,0,1,0,
                         0,0,1,1,0,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,

                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
                         1,0,1,0,1,1,
                         0,1,0,1,0,1,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,


                         0,0,0,0,0,0,
                         1,0,0,0,0,1,
                         1,0,0,0,0,1,
                         1,1,0,0,1,1,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,


                         0,0,0,0,0,0,
                         1,1,0,1,1,0,
                         1,1,0,1,1,0,
                         0,0,0,0,0,0,
                         0,0,0,0,0,0,
	                     0,0,0,0,0,0,
					 
						 0,0,0,0,0,0,
						 1,1,1,0,1,1,
						 0,0,1,0,1,0,
						 0,0,0,0,0,0,
						 0,0,0,0,0,0,
						 0,0,0,0,0,0  };



#define SEQLEN 7 // war 7, wie in echt
#define NUM_ELEMENTS 50

struct
{
 int shapenum;
 float rotation;
 float scale;
 int x,y;
} shapeseq[SEQLEN][NUM_ELEMENTS];

float evalseq[NUM_ELEMENTS];


/* Jokerfarben */

unsigned char patterns[36][27];

unsigned char matcha[160][100][2];
unsigned char matchb[160][100][2];
unsigned char screen[160][120];


void jokerspread(void)
{
  int x,y;
  
  y=0;
  while ( y < 100)
  {
   x=0;
   while ( x < 160)
   {
     if ( rand()%20==0)matcha[x/RANGESCALE][y/RANGESCALE][0]=3;	
     x++;
   }
   y++;
  }
	
}



void init_patterns(void)
{
 int n,n2;
 n=0;
 
 int x,y;
 
 n=0;
 while ( n < 27)
 {
  y=0;
  while (y < 6)
  {
  	x=0;
  	while (x < 6)
  	{
  	 patterns[x+y*6][n]=font[x+y*6+n*36];	
  		x++;
	  }
  	 y++;
  	 
	}	
 	n++;
 }
 
 
 /*
 patterns[0][0]=1,patterns[1][0]=1,patterns[2][0]=1,patterns[3][0]=1,patterns[4][0]=1,
 patterns[5][0]=1,patterns[6][0]=0,patterns[7][0]=0,patterns[8][0]=0,patterns[9][0]=1,
 patterns[10][0]=1,patterns[11][0]=1,patterns[12][0]=1,patterns[13][0]=1,patterns[14][0]=1,
 patterns[15][0]=1,patterns[16][0]=0,patterns[17][0]=0,patterns[18][0]=0,patterns[19][0]=1,
 patterns[20][0]=1,patterns[21][0]=0,patterns[22][0]=0,patterns[23][0]=0,patterns[24][0]=1;
 
 
 
 return;*/
 /*
 while ( n < 500 )
 {
  n2=0;
  while ( n2 < 36 ) patterns[n2][n]=rand()%2, n2++;
  n++;
 }
 */

}

void throwpatterns( int seqnum, int elemnum)
{
 unsigned char bitmap[40][40];
 unsigned char rotation[64][64];

 int x,y;
 
 y=0;
 while ( y < 40 )
 {
  x=0;
  while ( x < 40 )
  {
   bitmap[x][y]=0;
   x++;
  }
  y++;
 }

 y=0;
 while ( y < 64 )
 {
  x=0;
  while ( x < 64 )
  {
   rotation[x][y]=0;
   x++;
  }
  y++;
 }


 int scale=shapeseq[seqnum][elemnum].scale;


 y=0;
 while ( y < 40 )
 {
  x=0;
  while ( x < 40 )
  {
   bitmap[x][y]=0;
   x++;
  }
  y++;
 }

 y=0;
 while ( y < 40-10/scale )
 {
  x=0;
  while ( x < 40-10/scale )
  {
   if ( patterns[x/scale+ (y/scale)*6][shapeseq[seqnum][elemnum].shapenum]==0 || x/scale >= 6 || y/scale >= 6 )
   bitmap[x+10/scale][y+10/scale]=0; else bitmap[x+10/scale][y+10/scale]=1;
   x++;
  }
  y++;
 }
 

  float x_rot, y_rot;
     
   float angle=shapeseq[seqnum][elemnum].rotation;

    y=0;
    while ( y < 40 )
    {
        x=0;
        while ( x < 40 )
        {
         x_rot= +(x-20.0)*COS(angle)-(y-20.0)*SIN(angle);
          y_rot= (x-20.0)*SIN(angle)+(y-20.0)*COS(angle);
         
          if ( x_rot+32 > 0 && x_rot+32 < 64 && y_rot+32 > 0 && y_rot+32 < 64)
         rotation[(signed int)x_rot+32][(signed int)y_rot+32]=bitmap[x][y];
          x++;
       
        }
     y++;
    }

   x=0,y=0;
   while ( y < 64)
   {
    x=0;
    while ( x < 64)
    {
    	if (shapeseq[seqnum][elemnum].x+x > 0 &&shapeseq[seqnum][elemnum].x+x< 160 &&
		   shapeseq[seqnum][elemnum].y+y > 0 && shapeseq[seqnum][elemnum].y+y < 100 )
     	{
		 
		 if ( rotation[x][y]==1)
		  matcha[(shapeseq[seqnum][elemnum].x+x)/RANGESCALE][(shapeseq[seqnum][elemnum].y+y)/RANGESCALE][1]=rotation[x][y] ;
	   
	   }
	 
	 x++;
    }
    y++;
   }

}


void fillsome(int sequence)
{
  
  int n=0;
  
  while ( n < SEQLEN) throwpatterns(n, sequence ), n++;
  
 //jokerspread();
	
}

















void combine(void)
{
 
 int opiter=0;
 
 int subit;
 
 subit=1;
 
 while ( subit < 20)
 {
 opiter=0;
 

   
 while ( opiter < SEQLEN)
 {
  
  shapeseq[opiter][NUM_ELEMENTS-subit].shapenum=shapeseq[opiter][rand()%(NUM_ELEMENTS-1)].shapenum;
  shapeseq[opiter][NUM_ELEMENTS-subit].rotation=shapeseq[opiter][rand()%(NUM_ELEMENTS-1)].rotation;
  shapeseq[opiter][NUM_ELEMENTS-subit].scale=shapeseq[opiter][rand()%(NUM_ELEMENTS-1)].scale;
  shapeseq[opiter][NUM_ELEMENTS-subit].x=shapeseq[opiter][rand()%(NUM_ELEMENTS-1)].x;
  shapeseq[opiter][NUM_ELEMENTS-subit].y=shapeseq[opiter][rand()%(NUM_ELEMENTS-1)].y;
 
   evalseq[NUM_ELEMENTS-subit]=-1000000;
   
   
   
  opiter++;
 }
 
 subit++;
 
 }
 
}

void mutate(void)
{
	
	int subit=1;
	
	
	while ( subit < 20)
 {
 int opiter;
 opiter=0;
 
   
 while ( opiter < SEQLEN)
 {
  
  shapeseq[opiter][NUM_ELEMENTS-subit].shapenum=rand()%27; // OBACHT ABC
  shapeseq[opiter][NUM_ELEMENTS-subit].rotation=ROTATION; //(rand()%614)*0.01;
  shapeseq[opiter][NUM_ELEMENTS-subit].scale=SCALING;
  shapeseq[opiter][NUM_ELEMENTS-subit].x=rand()%55;
  shapeseq[opiter][NUM_ELEMENTS-subit].y=rand()%50;
 
   evalseq[NUM_ELEMENTS-subit]=-1000000;
     
     
     
  opiter++;
 }
 
 subit++;  
}
 
}
void sort_eval_cut(void)
{
 int n=0, n2=0,n3=1;
    signed int bufx, bufy, bufscal, bufshap;
	float buf5;
     float rotbuf;
     
     
     int opiter=0;
     
 while ( n3 < NUM_ELEMENTS)
{
 n2=NUM_ELEMENTS-1;
 while (n2 > n3)
 {
  if ( evalseq[n2] > evalseq[n2-1] ) 
    {
    	n3=0;
    
	 buf5=evalseq[n2-1];
	 evalseq[n2-1]=evalseq[n2];
	 evalseq[n2]=buf5;
	 	
    	opiter=0;
    	while ( opiter < SEQLEN)
    	{
    	  rotbuf=shapeseq[opiter][n2-1].rotation;
    	  shapeseq[opiter][n2-1].rotation=shapeseq[opiter][n2].rotation;
    	 shapeseq[opiter][n2].rotation=rotbuf;
    	 
    	 
    	 bufx=shapeseq[opiter][n2-1].x,
    	 bufy=shapeseq[opiter][n2-1].y,
    	 bufscal=shapeseq[opiter][n2-1].scale,
    	 bufshap=shapeseq[opiter][n2-1].shapenum,
    	  
		 shapeseq[opiter][n2-1].x=shapeseq[opiter][n2].x,
		 shapeseq[opiter][n2-1].y=shapeseq[opiter][n2].y,
		 shapeseq[opiter][n2-1].scale=shapeseq[opiter][n2].scale,
		 shapeseq[opiter][n2-1].shapenum=shapeseq[opiter][n2].shapenum;
		 
		 
		 shapeseq[opiter][n2].x=bufx,
		 shapeseq[opiter][n2].y=bufy,
		 shapeseq[opiter][n2].shapenum=bufshap,
		 shapeseq[opiter][n2].scale=bufscal;
		 
       opiter++;
	   }
   }
  n2--;
  }
  
  
  n3++;
 }	
 
}








int main(void)
{
 int x=0, y=0;
 int funcnr;
 
 float evaluation_old=-1000000;
   
   unsigned char vision[160][90][2];
 
    sininit();
    cosinit();
   
   FILE *input=fopen(".\\identikit","rb");
   if ( input==NULL) return;
   

    unsigned char bild[64000]; int bild2[64000], umask[64000];

     fread(bild,sizeof(unsigned char),64000,input);
      fclose(input);
   
   
  system("cls\n");
  
   y=0;
   while ( y < 199 )
   {
   	if ( y%2==0)
   	{
    x=0;
    while ( x < 320 )
    {
       int luma;
	   
	   if ( bild[(int)(x*1)+(int)(y*1)*320] > 240) luma=219;
	   else
    if ( bild[(int)(x*1)+(int)(y*1)*320] > 190) luma=177;
	   else
	   if ( bild[(int)(x*1)+(int)(y*1)*320] > 170) luma=176;
	   else luma=' ';
	   	
     if ( x%2==0)printf("%c", luma ) ;
     x++;
    }
    printf("\n");
   }
	y++;
   }
   
   printf("Jetzt muessten wir normalerweise das Struct-Array aus der Erkennung des Gesichts des britischen Agenten laden und auf die anfaegnliche Trefferrate schauen.."), getch();
   
     y=0;
  while ( y < 200)
  {
   x=0;
   while ( x < 320)
   {
   	 bild2[(int)x+(int)y*320]=bild[(int)x+(int)y*320];
   	 
   	  x++;
	   }	
  	y++;
  }
  
  int n=0;
  while ( n < 2)
  {
  int x=0, y=0;
  
  y=0;
  while ( y < 200)
  {
   x=0;
   while ( x < 320)
   {
   	 bild2[x+y*320]+=bild2[x+1+y*320];
   	 bild2[x+y*320]+=bild2[x+(y+1)*320];
   	 bild2[x+y*320]+=bild2[x+1+(y+1)*320];
   	
   	 bild2[x+y*320]/=4;
   	 
   	  x++;
	   }	
  	y++;
  }
  
   n++;
 }
  
  {
  	int x=0,y=0;
  y=0;
  while ( y < 200)
  {
  	x=0;
  	while ( x < 320)
  	{
  		int c;
  		
  	  c=bild2[x+y*320]; //(bild2[x+y*320]+bild2[x+1+y*320]+bild2[x+(y+1)*320]+bild2[x+1+(y+1)*320])/4;
  	   umask[x+y*320]=255-c, umask[x+1+y*320]=255-c, umask[x+(y+1)*320]=255-c, umask[x+1+(y+1)*320]=255-c;	
  	
  	    umask[x+y*320]=bild[x+y*320]+umask[x+y*320] <=255 ? bild[x+y*320]+umask[x+y*320] : 255;
	    umask[x+1+y*320]=bild[x+1+y*320]+umask[x+1+y*320] <=255 ? bild[x+1+y*320]+umask[x+1+y*320] : 255;
	    umask[x+1+(y+1)*320]=bild[x+1+(y+1)*320]+umask[x+1+(y+1)*320] <=255 ? bild[x+1+(y+1)*320]+umask[x+1+(y+1)*320] : 255;
	    umask[x+(y+1)*320]=bild[x+(y+1)*320]+umask[x+(y+1)*320] <=255 ? bild[x+(y+1)*320]+umask[x+(y+1)*320] : 255;
		  
		  if ( umask[x+y*320]< 255) umask[x+y*320]=0;
		  if ( umask[x+1+y*320]< 255) umask[x+1+y*320]=0;
		  if ( umask[x+1+(y+1)*320]< 255) umask[x+1+(y+1)*320]=0;
		  if ( umask[x+(y+1)*320]< 255) umask[x+(y+1)*320]=0;
		  
		  
	 	x+=2;
	  }
  	y+=2;
  }
  
  }

   
   y=0;
   while ( y < 90)
   {
    x=0;
    while ( x < 160)
    {
     vision[x][y][0]=0;
     vision[x][y][1]=0;
     x++;
	}
    y++;
  }
   
   y=0;
   while ( y < 198)
   {
    x=0;
    while ( x < 320)
    {
     vision[x/(RANGESCALE*2)][y/(RANGESCALE*2)][0]=0;
     vision[x/(RANGESCALE*2)][y/(RANGESCALE*2)][1]=umask[x+y*320]==255 ? 0 : 1;
     x++;
	}
    y++;
  }
  
   
   /*
   y=10;
   while ( y < 40)
   {
    x=20;
    while ( x < 50)
    {
    	if ( x==22 || x == 48|| y==12 || y == 38)
     vision[x][y][0]=176,
	 vision[x][y][1]=1;	
	 
	 
	 else
	   	if ( x==23 || x == 49|| y==13 || y == 39)
  
   	vision[x][y][0]=177,
	 vision[x][y][1]=2;	
	 
	 
	 else
	   	if ( x==21 || x == 47|| y==11 || y == 37)
  
   	vision[x][y][0]=177,
	 vision[x][y][1]=2;
	 	
	 x++;
    }
    y++;
  }
  */


   int n2=0;
   
   while (n2 < NUM_ELEMENTS)
   {
   
    int n=0;
   
     while ( n < SEQLEN)
     {
     	
     	
        shapeseq[n][n2].x=rand()%55;     
        shapeseq[n][n2].y=rand()%50;     
        shapeseq[n][n2].rotation=ROTATION;     
        shapeseq[n][n2].scale=(rand()%3)+4;     
        shapeseq[n][n2].shapenum=rand()%27; // OBACHT ABC
   
   evalseq[n2]=-1000000;
   
      n++;
	 }
    n2++;
   }

 float num_pixels_bak, eval_bak;
 
 init_patterns();
  

 while (1 )
 {
 	
 	// inner thoughts
 	//if ( rand()%3==0)
 	{
     y=0;
	 while ( y < 90/RANGESCALE)
	 {
	   x=0;
	   while ( x < 160/RANGESCALE)
	   {
	    matcha[x][y][0]=vision[x][y][0];
		matcha[x][y][1]=vision[x][y][1];	
	   	x++;
	    }	
	 	y++;
	 }	 	
 		
 		
 		
	 }
 	
 	
   y=0;
   while ( y < 90/RANGESCALE )
   {
    x=0;
    while ( x < 160/RANGESCALE )
    {
     matchb[x][y][0]=matcha[x][y][0];
     matchb[x][y][1]=matcha[x][y][1];
     
     
	 matcha[x][y][0]=' ';
     matcha[x][y][1]=0;
      x++;
    }
    y++;
   }
   
   
   
   
   sort_eval_cut();
   combine();
   if ( rand()%2==0 ) mutate();
  
  sort_eval_cut();
   
   
   funcnr=rand()%(20)+NUM_ELEMENTS-20;   
   fillsome(funcnr);
    
   y=0;
   float evaluation;
   
   evaluation=0.0;
   float num_pixels;
   
   num_pixels=0;
   
   y=0;
   while ( y < 90/RANGESCALE)
   {
   	x=0;
   	while ( x < 160/RANGESCALE)
   	{
   	 if ( matcha[x][y][1]!= 0 && matchb[x][y][1]!= 0)
		{
			if ( matcha[x][y][0]!=3)
		
		 evaluation-=fabs(matcha[x][y][1] - matchb[x][y][1]);
		  evaluation++;
		 }	
		 
		 
   	 if ( matcha[x][y][1]!= 0 || matchb[x][y][1]!= 0)
		{
		 num_pixels++;
   			if ( matcha[x][y][0]!=3) evaluation++;
	    }
		x++;
    }
   	y++;
   	
   }
   
   if ( num_pixels > 0) evaluation/=num_pixels;
   
   
   
   #define THRESHOLD 0.1 // AB DA ERKANNT
   
   //if ( evaluation > THRESHOLD)
    evalseq[funcnr]/*+*/=evaluation;
   
     if ( evalseq[funcnr] > evaluation_old)
     {
       evaluation_old=evalseq[funcnr];
       
     	y=0;
     	while ( y < 90)
     	{
     	  x=0;
		   while ( x < 160) matcha[x][y][1]=' ', x++;
		   y++;	
		 }
     	
     	
     	funcnr=0;
     	 fillsome(funcnr);
         
     	
       num_pixels_bak=num_pixels, eval_bak=evaluation;   
   y=0;
   while ( y < 90 )
   {
    x=0;
    while ( x < 160 )
    {
    
		
	 screen[x][y]=matcha[x][y][1]== 1 ? '@' : ( matcha[x][y][1]== 2 ? 176 : ' ' );
    
	 if ( vision[x][y][1]==1 && screen[x][y]==' ' ) screen[x][y]=219;
	 if ( vision[x][y][1]==2 && screen[x][y]==' ') screen[x][y]=177;
	  
	 
	 x++;
    }
    y++;
   } 
   
  
  system("cls\n");
  
   y=0;
   while ( y < 88 )
   {
    x=0;
    while ( x < 158 )
    {
     printf("%c%c%c%c%c%c%c%c",screen[x][y],screen[x+1][y], screen[x+2][y],
     screen[x+3][y],screen[x+4][y],screen[x+5][y],screen[x+6][y],screen[x+7][y] );
     x+=8;
    }
    printf("\n");
    y++;
   }
   
   int letterschecked;
   int further[SEQLEN];
   int old_x=0;
   old_x=0;
   letterschecked=0;
   
   while ( letterschecked < SEQLEN)further[letterschecked]=0, letterschecked++;
   
   letterschecked=0;
   
    while ( letterschecked < SEQLEN)
	{
	  int cmpa, cmpb, count;
	  cmpa=0, cmpb=100000, count=0;
	  
	  while ( count < SEQLEN)
	  {
	   if ( further[count]==0 && cmpb >= shapeseq[count][funcnr].x ) cmpb= shapeseq[count][funcnr].x, cmpa=count;
	    count++;
      }
      old_x=cmpb;
	 	
	 if ( shapeseq[cmpa][funcnr].x >= old_x && further[cmpa]==0 )
     {
	  printf("%c",shapeseq[cmpa][funcnr].shapenum <= 25 ?
       shapeseq[cmpa][funcnr].shapenum+0x41 : ' '); old_x=shapeseq[cmpa][funcnr].x,further[cmpa]=1, letterschecked++; }
     
     }
     printf("\n");
     
     
  printf("%f ", evaluation_old );
  
   }
   
 }

}
