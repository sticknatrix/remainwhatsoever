/* 
  Figurenlegen - der Computer spielt Tangram
  
  Zunaechst werden geometrische Formen zufaellig gedreht und so positioniert,
  dass sie sich nicht ueberlappen.
  Dann wird mit eine Unscharfmaske gebildet und mit einer Fuzzy-Logic mit einer
  zuvor eingegebenen Umrisszeichnung verglichen.
  Der Computer bleibt immer bei dem aehnlichsten gefundenen Bild haengen.
  */



#include <stdio.h>
#include <math.h>


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


unsigned char shapes[4][16]=
       {
       	{
       	 0,0,0,1,
       	 0,0,1,1,
       	 0,1,1,1,
       	 1,1,1,1
		   },   
       	{
       	 1,1,1,1,
       	 1,1,1,1,
       	 1,1,1,1,
       	 1,1,1,1
		   },
       	{
       	 0,1,1,0,
       	 1,1,1,1,
       	 1,1,1,1,
       	 0,1,1,0
		   },
       	{
       	 0,1,1,0,
       	 0,1,1,0,
       	 0,1,1,0,
       	 0,1,1,0
		   }
       	
	   };


float match, matchold;
unsigned char pic[256]={
   	 0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
       	 0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,
       	 0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
       	 1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
		  
       	 1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,
       	 0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
       	 0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
       	 1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,
	     0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
;
signed int cmppic[256];
signed int ushmask[256];
  

struct
{
 float angle;
 int x,y;
} form[4];


int paste_shape(int num)
{	
  unsigned char shape_copy[36]={0,0,0,0,0,0,
                                0,0,0,0,0,0,
								0,0,0,0,0,0,
								0,0,0,0,0,0,
								0,0,0,0,0,0,
								0,0,0,0,0,0};	
 
 int x=0, y=0;
 float x_rot, y_rot;
 
 while ( y < 4)
 {
 	x=0;
 	while ( x < 4)
 	{
 	    if ( shapes[num][x+y*4]==1)
		 {
		  
		  x_rot= +(x-2)*COS(form[num].angle)-(y-2)*SIN(form[num].angle);
          y_rot= (x-2)*SIN(form[num].angle)+(y-2)*COS(form[num].angle);
          
          x_rot+=3.0, y_rot+=3.0;
	
	      shape_copy[(int)(4.0/6.0*x_rot)+((int)(4.0/6.0*y_rot)*6)]=1;	
		 }
		 
		 	
 		x++;
	 }
	 y++;
 }


 y=0;
 while ( y < 6)
 {
  x=0;
  while ( x < 6)
  {
   if ( form[num].x+x >= 0 &&
        form[num].x+x < 16 &&
        
        form[num].y+y >= 0 &&
        form[num].y+y < 16   
         )
   {
    cmppic[form[num].x+x + (form[num].y+y)*16]+=shape_copy[x+y*6];
    if ( cmppic[form[num].x+x + (form[num].y+y)*16] > 1 ) return 1;
   }
   x++;
  }
  y++;
 }
  return 0;
}

void amplify_edge(void)
{
 int x=0,y=0;
 
 while ( y < 16)
 {
  x=0;
  while ( x < 16)
  {
  	
  	cmppic[x+y*16]*=127;
    x++;	
   }	
 	
  y++;
 }
 
 
 x=0,y=0;
 while ( y < 15)
 {
  x=0;
  while ( x < 15)
  {
  	
  	ushmask[x+y*16]=(cmppic[x+y*16]+cmppic[(x+1)+y*16]+
  	                     cmppic[x+(y+1)*16]+cmppic[(x+1)+(y+1)*16] )/4;
    
	ushmask[x+1+y*16]=(cmppic[x+y*16]+cmppic[(x+1)+y*16]+
  	                     cmppic[x+(y+1)*16]+cmppic[(x+1)+(y+1)*16] )/4;
    ushmask[x+(y+1)*16]=(cmppic[x+y*16]+cmppic[(x+1)+y*16]+
  	                     cmppic[x+(y+1)*16]+cmppic[(x+1)+(y+1)*16] )/4;
    ushmask[x+1+(y+1)*16]=(cmppic[x+y*16]+cmppic[(x+1)+y*16]+
  	                      cmppic[x+(y+1)*16]+cmppic[(x+1)+(y+1)*16] )/4;
    
	x++;	
   }	
 	
  y++;
 }
	

 x=0,y=0;
 while ( y < 16)
 {
  x=0;
  while ( x < 16)
  {
  	
  	cmppic[x+y*16]-=ushmask[x+y*16];
    x++;	
   }	
 	
  y++;
 }
		
	
	
}

void compare(void)
{
 int n=0;
 match=0.0;
 
 while ( n < 256) { match+=fabs(cmppic[n%16+ (n/16)*16]-pic[n%16+ (n/16)*16]*127 ), n++; }
 
}

int main(void)
{	
  
  int n=0;
  unsigned char c;

   sininit();
   cosinit();
    
	/* 
  while ( n < 256)
  {
  	c=getch();
  	if ( c =='x') c=1; else c=0;
  	pic[n]=c;
  	if ( c == 1 ) printf("#"); else printf(" ");
  	
	  n++;
	  if ( n%16==0)printf("\n");
  }
    */
  
  matchold=100000;
  
  while ( matchold > 100.0)
  {
  	n=0;
  	while ( n < 256) cmppic[n]=0, n++;
  	n=0;
  	while ( n < 4)
  	{
  	  form[n].x=rand()%11+1;
      form[n].y=rand()%11+1;
	  form[n].angle= ((float)rand())/100.0 ;
  	  if ( paste_shape(n) ==1 ) { n=0; while ( n < 256) cmppic[n]=0, n++; n=0; continue; }
		n++;
	   
	  }
	  
	  /*
  	system("cls\n");
	  n=0;
	  while ( n < 256)
	  {
	  	if ( cmppic[n] != 0 )
		  printf("#"); else printf(" ");
	  	n++;
	  	if ( n%16==0) printf("|\n");
	  	
       }
         */
	   
  
	  
	    
  	
  	amplify_edge();
  	compare();
  		
  	
  	
	if ( match < matchold)
	{
	 matchold=match;

	  system("cls\n");
	  n=0;
	  printf("Sollbild:\n");
	  while ( n < 256)
	  {
	  	if ( pic[n] != 0 )
		  printf("+"); else printf(" ");
	  	n++;
	  	if ( n%16==0) printf("|\n");
	  	
	  }
	  
	  printf("Annaeherung:\n");
	  n=0;
	  while ( n < 256)
	  {
	  	if ( cmppic[n] != 0 )
		  printf("#"); else printf(" ");
	  	n++;
	  	if ( n%16==0) printf("|\n");
	  	
	  }
    printf("%f", match);
  
     }
  
  }
	
	
}
