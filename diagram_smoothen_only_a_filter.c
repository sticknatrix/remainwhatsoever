/*
Farbträger Flimmern Empfang Analog TV - Google Suche

Linsen machen gut satt aber gekocht - Google Suche
Linsen wachsen auch fast überall, aber giftig wenn Boden giftig?!?eher?!?

eine Mischung aus gegenphasiger Löschung und Moiré Muster möglich bei Senkrechtdrehung?!? - Google Suche
kleinen Kreis von Funktion nein in dem Fall Bitmap markieren und drehen Frequenz dieselbe?!? - Google Suche
als würde man eine Periodische Funktion in Streifen wie geschreddert zufällig zusammenbauen, meist stimmt im Durchschnitt die Frequenz noch?
gibt aber Löschmuster Wahrscheinlichkeit?

*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(void)
{
 unsigned int function[]={1,2,3,4,5,6,7,5,3,1,1,2,3,4,5,6,7,10,9,10,11,12,9,6,3,1,3,6,36,12,15,18,15,14,13,12,11,10,9,9,9,9,20,9,9,8,7,6,5,6,7,9,10,12};
unsigned int function2[]={1,2,3,4,5,6,7,5,3,1,1,2,3,4,5,6,7,10,9,10,11,12,9,6,3,1,3,6,36,12,15,18,15,14,13,12,11,10,9,9,9,9,20,9,9,8,7,6,5,6,7,9,10,12};


signed int diffqodiff[52];
float avg2_slope[52];
int n,n2;

n=0;
while(n<52)
{
 avg2_slope[n]=function[n]*100;

 n++;
}

n=0;
while(n<52)
{
 avg2_slope[n+3]=avg2_slope[n+2]=avg2_slope[n+1]=avg2_slope[n]=(avg2_slope[n+1]+avg2_slope[n+2]+avg2_slope[n+3])/4.0;	
  n+=4;	
}
n=0;
while(n<51)
{
 diffqodiff[n]=((function[n+1]*100)/function[n]) /((function[n]*100)/(avg2_slope[n]));
 n++;
	
}

n2=1000;
int n3=0;
while ( n2>0)
{
int maxdiffqodiff=100;
maxdiffqodiff=100;
n=0;
int found=0;
found=0;
while ( maxdiffqodiff>0)
{
 n=0;
 while ( (int)abs(diffqodiff[n])!=(int)maxdiffqodiff&& n<51)n++;
 if ( (int)abs(diffqodiff[n])==(int)maxdiffqodiff) found=1, diffqodiff[n]-=(diffqodiff[n]>0 ? 1 : -1),function[n]=(function[n]+avg2_slope[n]/100)/2;//also minus 1?!?
 maxdiffqodiff--;
 int n4=0,n5=0;
 n5=40;
 /*
 if(maxdiffqodiff%10==0)
 {
 while (n5>0)
 {
  printf("\n");
  n4=0;
  while (n4<51){
  if ( function[n4]>=n5)printf("%c",'#'),fflush(stdout); else printf(" "),fflush(stdout);n4++;}
  n5--;
 }
 */
 
 //doppelzuweisung mit -= oder += also mehrfachzuweisung?!?!meine?!?
 }
  if ( found==0 )goto printstretch;
 n2--;
 n3++;
}


printstretch:

n=0;
float strecke=0,strecke2=0;
while ( n<52)strecke+=function[n],strecke2+=function2[n],n++;
 printf("%d",1001-n3);
 
 float scale=strecke/strecke2;
float floatn=0;
floatn=0;
n=0;
while ( n<51)printf("%d %d %f\n",function[(int)floatn],function2[(int)floatn],avg2_slope[n]),n++;
 int n5,n4;
 float floatn4;
 n5=40;
 while (n5>0)
 {
  printf("\n");
  floatn4=0;
  while (floatn4<51){
  if ( function[(int)floatn4]>=n5)printf("%c",'#'),fflush(stdout); else printf(" "),fflush(stdout);floatn4+=scale;}
  n5--;
 }
 
 n5=40;
 while (n5>0)
 {
  printf("\n");
  n4=0;
  while (n4<51){
  if ( function2[n4]>=n5)printf("%c",'#'),fflush(stdout); else printf(" "),fflush(stdout);n4++;}
  n5--;
 }
 
 
 

}
