#include <stdio.h>

/*
double probability[]={ 0.065290,0.022353,0.031834,0.045248,0.146954,0.021175,\
                        0.026197,0.040674,0.077753,0.002438,0.013051,0.040405,\
                        0.030283,0.086217,0.045827,0.017551,0.000867,0.070416,\
                        0.064722,0.072446,0.033995,0.009412,0.015420,0.003215,\
                        0.007192,0.009064 };\
						//Englisch oder deutsch
*/

double probability[]={6.51 ,
1.89 ,
3.06 ,
5.08 ,
17.40,
1.66 ,
3.01 ,
4.76 ,
7.55 ,
0.27 ,
1.21 ,
3.44 ,
2.53 ,
9.78 ,
2.51 ,
0.79 ,
0.02 ,
7.00 ,
7.27 ,
6.15 ,
4.35 ,
1.89 ,
0.03 ,
0.04 ,
1.13 } ;

unsigned char frequentwords[]="und sie die der das nicht ein den sich auf mit war ich von dem wie aber ist eine als ihn des aus dann waren immer doch ihr wieder ihm auch sind ihre vor wir was ihnen oder zur zum nach seine fragte ins ihren ohne anderen fast mich schon gegen mehr mir weil sehr bis einmal eines etwas alles haben bei dieser alle hat nichts hier wird seiner werden ganz denn durch wurde diese konnte seine keine";

unsigned char *textbuf,*textbuf2;

double replace[26][26];

   float frequency[26];

void upvalue_close_usage(void)
{
 double frequency_buf[26];
 double probability_buf[26];
 int n=0,n2=0;
 
 while ( n < 26)frequency_buf[n]=frequency[n],probability_buf[n]=probability[n],n++;
 
 double frequency_biggest,probability_biggest;
 int frequency_pos, probability_pos;
 n2=0;
 while (n2<26)
 {
  frequency_biggest=0,probability_biggest=0;
  n=0;
  while ( n < 26)
  {
  	if ( frequency_buf[n]>frequency_biggest)frequency_biggest=frequency_buf[n],frequency_pos=n;
  	if ( probability_buf[n]>probability_biggest)probability_biggest=probability_buf[n],probability_pos=n;
  	n++;
  }
  replace[frequency_pos][probability_pos]+=100;
  frequency_buf[frequency_pos]=0;
  probability_buf[probability_pos]=0;
  n2++;
 }
}

void upvalue_close_words(void)
{
 unsigned char inbuf[30];
 unsigned char cmpbuf[30];
 int inbuf_identical[30];
   int ident_c=0;
  int n=0,n2=0;
 
  void *textbuf_buf=textbuf2;
  void *frequentwords_buf=frequentwords;
 while ( ((unsigned char *)textbuf_buf)[0]!='\0')
 {
  sscanf(textbuf_buf,"%s",inbuf);
  textbuf_buf+=strlen(inbuf)+1;
  if (((unsigned char *)textbuf_buf)[0]=='\0')break;
  
  frequentwords_buf=frequentwords;
  while (((unsigned char *)frequentwords_buf)[0]!='\0')
  {
  do { sscanf(frequentwords_buf,"%s",cmpbuf); frequentwords_buf+=strlen(cmpbuf)+1;}
   while ( strlen(inbuf)!=strlen(cmpbuf)&&((unsigned char*)frequentwords_buf)[0]!='\0');
   if (((unsigned char *)frequentwords_buf)[0]=='\0')break;
   ident_c=0;
   n2=0;
   printf("Vergleiche %s mit %s ",inbuf,cmpbuf);
   while ( n2<strlen(inbuf))
   {
   	if ( inbuf[n2]==cmpbuf[n2])ident_c++;
   	n2++;
   }
   printf("%d identische Zeichen ",ident_c);
   if ( ident_c>=strlen(cmpbuf)-2&&ident_c>=2)
   {
   	n2=0;
   	while ( n2<strlen(cmpbuf))
   	{
   	 if ( cmpbuf[n2]>='A'&&cmpbuf[n2]<='Z'&&inbuf[n2]>='A'&&inbuf[n2]<='Z')replace[inbuf[n2]-0x41][cmpbuf[n2]-0x41]+=100;
   	 n2++;
    }
   }
  
 
  }
 }
	
	
}


int main(void)
{
 int n=0,n2=0;
 unsigned char c;
 n2=0;
FILE *input;

n=0;
while (frequentwords[n]!='\0')frequentwords[n]=toupper(frequentwords[n]),n++;

n2=0;
while ( n2<26)
 {
  n=0;
  while (n < 26)
  {
   replace[n2][n]=0;
   n++;
  }
  frequency[n2]=0;
  n2++;
 }

input=fopen("rotated.txt","rb");

 textbuf=malloc(sizeof(unsigned char)*1000000);
 textbuf2=malloc(sizeof(unsigned char)*1000000);
 n2=0;
 while (!feof(input))
 {
  c=fgetc(input);
  textbuf[n2]=c=toupper(c);
  if ( c>='A'&&c<='Z') frequency[c-0x41]++,n++;
  n2++;
 }
 textbuf[n2]='\0';

 fclose(input);

 n2=0;
 while (n2<26)frequency[n2]/=n,n2++;

  upvalue_close_usage();

unsigned char atob[26][2];

double biggest;
int biggest_pos;
 n=0;
 while ( n<26)
  {
  n2=0;
  biggest=0;
  while ( n2<26 )
 {
  if ( replace[n][n2]>=biggest)biggest_pos=n2,biggest=replace[n][n2];
  n2++;	
 }
 atob[n][0]=n+0x41,atob[n][1]=biggest_pos+0x41;
 
 printf("%c wohl zu %c\n",atob[n][0],atob[n][1]);
 n++;
}

n2=0;
while (textbuf[n2]!='\0')
{
 if ( textbuf[n2]>='A'&&textbuf[n2]<='Z')printf("%c",textbuf2[n2]=atob[textbuf[n2]-0x41][1]);
 else printf("%c",textbuf2[n2]=textbuf[n2]);
 n2++;
}

n2=0;
while ( n2<26)
 {
  n=0;
  while (n < 26)
  {
   replace[n2][n]=0;
   if ( n2==n)replace[n2][n]=100;
   n++;
  }
  frequency[n2]=0;
  n2++;
 }

  upvalue_close_words();

 n=0;
 while ( n<26)
  {
  n2=0;
  biggest=0;
  while ( n2<26 )
 {
  if ( replace[n][n2]>=biggest)biggest_pos=n2,biggest=replace[n][n2];
  n2++;	
 }
 atob[n][0]=n+0x41,atob[n][1]=biggest_pos+0x41;
 
 printf("%c wohl zu %c\n",atob[n][0],atob[n][1]);
 n++;
}

n2=0;
while (textbuf2[n2]!='\0')
{
 if ( textbuf2[n2]>='A'&&textbuf[n2]<='Z')printf("%c",textbuf2[n2]=atob[textbuf2[n2]-0x41][1]);
 else printf("%c",textbuf2[n2]);
 n2++;
}



}


