/*only almost heron nearing limits error!*/
/* after compiling, can be run parallel
 ./a.out 1 10 & ./a.out 11 20 15 & ./a.out 1 10 4
   this is how it is thought or meant?!?
   
   call only with whole numbers when also*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>

int main( int argc, char *argv[])
{
 FILE *result;
 if ( argc != 4&& argc!= 3 ) return 1;
 
 signed int rangelow, rangehigh;
 unsigned char name[8],name2[8];
 unsigned char resultname[255];
 
 strcpy(resultname,"sqrt-tab");
 
 rangelow=atoi(argv[1]);
 rangehigh=atoi(argv[2]);
 sprintf(name,"%d",rangelow);
 sprintf(name2,"%d",rangehigh);
 //itoa(rangelow,name,10),
 //itoa(rangehigh,name2,10);
 strcat(resultname,name),
 strcat(resultname,"_"),
 strcat(resultname,name2);
 
repeat:
 result=fopen(resultname,"rb");
  if ( result==NULL)
  {
   float high_limit, low_limit;
   high_limit=rangehigh;
   int n=0;
    low_limit=rangelow;
   result=fopen(resultname,"wb");
    if ( result==NULL)return -2;
   float prev_exbest=rangelow;
   while ( rangehigh-n>rangelow)
   {
    high_limit=(rangehigh-n);
	low_limit=prev_exbest;
	int n2=0;
	//while(low_limit*low_limit<=high_limit)low_limit+=0.01;
         while(fabs(low_limit*low_limit-high_limit)>0.01){ low_limit=(low_limit+(float)high_limit/low_limit)/2.0;} ;/*is that Heron?!?*/
   	  prev_exbest=low_limit;//high_limit;
   	 //prev_exbest=1;
   	unsigned char wrstring[255];
   	//itoa((int)(low_limit*100),wrstring,10);
   	sprintf(wrstring,"%d",(int)low_limit*100);
   	strcat(wrstring,"\n");
   	/*
	   // fwrite(wrstring,sizeof(unsigned char),strlen(wrstring),resultname);
   	  what was this? take care again!?!
   	 */
   	   fprintf(result,"%f\n",(float)low_limit);
	   n++;
	}
	fclose(result); 
 goto repeat;

  }
  else if (argc==4)
  {
  	float resultval;
  	int n=rangehigh;
	  while(n>rangelow)
	{
	 fscanf(result,"%f\n",&resultval);
	  if ( n==atoi(argv[3]))printf("%f\n",resultval);
    n--;
   }
  }
  

}
