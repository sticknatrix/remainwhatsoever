#include <stdio.h>

int main(int argc, char *argv[])
{
 unsigned char rawdata[64000];
 int rangeof16[256];
 int values[256];
 int numcolors;
 
 int n=0;
 FILE *input;
 if ( argc!=3)return 1;
 numcolors=atoi(argv[2]);
 if ( numcolors<2)numcolors=2;
 if ( numcolors>256)numcolors=256;
 input=fopen(argv[1],"rb");
 while ( n < 64000)rawdata[n]=fgetc(input),n++;
 fclose(input);
 
 n=0;
 while ( n < 256)rangeof16[n]=0,n++;
 n=0;
 while ( n < 64000)
 {
 	int n3=0;
 	n3=0;
 	while ( n3 < 256){
 	if ( rawdata[n]>=n3&&rawdata[n]<=n3+(256/numcolors))rangeof16[n3]++;n3++;}
     n++;
 }
 
int colorpick,nearest;
 
 int n2=0;
 while ( n2 < numcolors)
 {
  n=0;
  nearest=0;
  int bakrange;
  while ( n < 256)
  {
  	if ( rangeof16[n]>=nearest)colorpick=n+(256/numcolors/2),nearest=rangeof16[n],bakrange=n;
  	n++;
  }
  values[n2]=colorpick > 255 ? 255 : colorpick;
  
  int numiterations,bakrange2;
  numiterations=256/numcolors/2;
  bakrange2=bakrange;
  while (numiterations>0)
  {
  	bakrange2--;
  if (bakrange2>=0)rangeof16[bakrange2]=0;
   numiterations--;
  }
  numiterations=256/numcolors/2;
   bakrange2=bakrange;
  while (numiterations>0)
  {
  	bakrange2++;
  if (bakrange2<=255)rangeof16[bakrange2]=0;
   numiterations--;
  }
  rangeof16[bakrange]=0;
   n2++;
 }
 unsigned int maskfound[256];
 n=0;
 while ( n < 64000)
 {
   n2=0;
   nearest=1000000;
   int colorpick2,masked;
   int palettevalue1,palettevalue2;
   while ( n2 < numcolors)maskfound[n2]=0,n2++;
   n2=0;
   while ( n2< numcolors)
   {
   	if(abs(rawdata[n]-values[n2])<=nearest)nearest=abs(rawdata[n]-values[n2]),colorpick=values[n2],palettevalue1=masked=n2;
   	n2++;
   }
   maskfound[masked]=1;
   
   nearest=1000000;
   n2=0;
   while ( n2< numcolors)
   {
   	if(abs(rawdata[n]-values[n2])<=nearest&&maskfound[n2]==0)nearest=abs(rawdata[n]-values[n2]),colorpick2=values[n2],palettevalue2=masked=n2;
   	n2++;
   }
   
   int colorsel;
   colorsel=colorpick;
   int difflike=0;
   if ( abs(rawdata[n]-colorpick)!=0)
   {
    //difflike=((rawdata[n]-colorpick2)*(rawdata[n]-colorpick2))/((rawdata[n]-colorpick)*(rawdata[n]-colorpick))+1;
    difflike=abs(rawdata[n]-colorpick2)/abs(rawdata[n]-colorpick)+1;
    if ((rawdata[n]>colorpick&&colorpick2>rawdata[n])||
	    (rawdata[n]<colorpick&&colorpick2<rawdata[n]))
	if (rand()%difflike==0)colorsel=colorpick2;
   }
   rawdata[n]=colorsel;
   n++;
   }
	
	n=0;
	while ( n < 16) printf("%d ",values[n]),n++;
 input=fopen("output.raw","wb");
 fwrite(rawdata,sizeof(unsigned char),64000,input);
 fclose(input);	
}
