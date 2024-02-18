/* Wir schreiben die Unendliche Geschichte, (mit Makulahalluzinationen?!)

  n m      a q   u v
  
  */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>


#define LANGUAGE  "Englisch oder Deutsch"
#define AVERAGE 0.125
#define LIMIT   129
#define PROBABILITY { 0.065290,0.022353,0.031834,0.045248,0.146954,0.021175,\
                        0.026197,0.040674,0.077753,0.002438,0.013051,0.040405,\
                        0.030283,0.086217,0.045827,0.017551,0.000867,0.070416,\
                        0.064722,0.072446,0.033995,0.009412,0.015420,0.003215,\
                        0.007192,0.009064 }\



int main(int argc, char *argv[])
{
 float prob['z'-0x60]= PROBABILITY;
 static double c_count[26];
 unsigned long long num_chars=0;

  unsigned char maculobuf[100];
 float diff;
 float randadd;

 int c;
  int rand_count;
  
  do
  {
  	rand_count=0;
  	num_chars=0;
  	while (rand_count<26)c_count[rand_count]=0.0,rand_count++;
  	rand_count=0;
 while( rand_count<25 )
 {
  c=maculobuf[rand_count]=((rand()+(int)randadd)%26)+0x41;
  if( isalpha(c)!=0 ) c_count[ tolower(c)-0x61]++, num_chars++;
  rand_count++;
  randadd+=M_PI*rand();
  if ( kbhit())randadd+=getch();
 }


 if( num_chars!=0)
 {
  c=0;
  do
  {
   c_count[c]/=num_chars;
  }
  while( ++c< 26);
 }

 diff=0;
 c=0;
 do
 {
  diff+= c_count[c] > prob[c] ? c_count[c]-prob[c]  : prob[c]-c_count[c] ;
 } while( ++c< 26 );

 diff= ( ((diff+AVERAGE*2)/3) /AVERAGE)*100;

 } while (diff>LIMIT*1.7);
 
 rand_count=0;
 while (rand_count<25)
 {
  printf("%c",maculobuf[rand_count]),rand_count++;
 }
 
 
 printf("Der Text-Quotient(TQ) lautet: %f\n", diff), fflush(stdout);
 if( diff > LIMIT )
 { printf("Datei enthaelt wenig 'Klartext' der Sprache " LANGUAGE "!\n"), fflush(stdout);
   return 1; }
 else
 { printf("Datei enthaelt viel Klartext der Sprache " LANGUAGE "!\n"), fflush(stdout);
   return 0; }

}

