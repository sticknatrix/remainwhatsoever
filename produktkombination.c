#include <stdio.h>
#include <math.h>

unsigned char naehrwerte[15][30];
unsigned int anzahl_naehrwerte;
unsigned int anzahl_produkte;

float naehrwerte_elemente_empfehlung[15];


struct
{
 unsigned char produkt[30];
 float naehrwerte_elemente[15];
 float preis;
}lebensmittel[50];

int kombination_best[20];
int kombination_len_best;
float bewertung_best;

void backtrack(int *kombination, int rec_depth)
{
  float preis;
  int kombination_new[20];
  float naehrwerte_elemente_sum[20];
 int n=0;
 
 while (n<20) kombination_new[n]=kombination[n],n++;
 n=0;
 while ( n < anzahl_produkte)
 {
  kombination_new[rec_depth]=n;
  int n2=0;
  n2=rec_depth;
  preis=0;
  while (n2>-1)preis+=lebensmittel[kombination_new[n2]].preis,n2--;
  int n3=0;
  n2=rec_depth;
   n3=0;
   while(n3<anzahl_naehrwerte) naehrwerte_elemente_sum[n3]=0,n3++;
  while ( n2 > -1 )
  {
   n3=0;
   while(n3<anzahl_naehrwerte) naehrwerte_elemente_sum[n3]+=lebensmittel[kombination_new[n2]].naehrwerte_elemente[n3],n3++;  
   n2--;
  }
  float mulfac;
  mulfac=naehrwerte_elemente_empfehlung[0]/naehrwerte_elemente_sum[0];
   n3=0;
   while (n3< anzahl_naehrwerte)
   {
    if ( fabs(1-naehrwerte_elemente_sum[n3]*mulfac/naehrwerte_elemente_empfehlung[n3]) < 0.1 )n3++;
     else break;
    }
   if ( n3==anzahl_naehrwerte )
   {
    if ( preis<=bewertung_best)
    {
     n2=rec_depth;
    while (n2>-1)
     kombination_best[n2]=kombination_new[n2],n2--;
     kombination_len_best=rec_depth+1;
     bewertung_best=preis;
     n2=0;
     printf("Folgender guenstiger Kauf: ");
     while (n2<rec_depth+1) printf("%s ,",lebensmittel[kombination_best[n2]].produkt),n2++;
     printf(". Preis insgesamt: %f DM .\n",bewertung_best);
    }
   
   
  }
  
  if (rec_depth<10)backtrack(kombination_new,rec_depth+1);
  n++;
 }
 
}


int main(void)
{
int n;
anzahl_naehrwerte=16;
 printf("Anzahl Naehrwerte eingeben (bis 15):");
 while ( anzahl_naehrwerte > 15 || anzahl_naehrwerte < 0 )scanf("%d",&anzahl_naehrwerte);
 n=0;
 while ( n < anzahl_naehrwerte)
printf("Namen des jeweiligen Naehrwerts eingaben:"),
  scanf("%s",&(naehrwerte[n][0])),
   n++;

n=0;
while (n<anzahl_naehrwerte)
 printf("Empfohlene Tagesmenge des Naehrwerts %s eingeben:",&(naehrwerte[n][0])),
 scanf("%f",&naehrwerte_elemente_empfehlung[n]),
 n++;


int n2=0;
 anzahl_produkte=51;
 printf("Anzahl Produkte eingeben (bis 50 ):");
 while ( anzahl_produkte>50||anzahl_produkte<0) scanf("%d",&anzahl_produkte);
n=0;
while ( n< anzahl_produkte)
{
  printf("Namen des jeweiligen Produkts eingeben:");
  scanf("%s",lebensmittel[n].produkt);
  
  n2=0;
  while(n2<anzahl_naehrwerte)
  {
   printf("Wieviel %s:",&(naehrwerte[n2][0]));
   scanf("%f",&lebensmittel[n].naehrwerte_elemente[n2]);
   n2++;
  }
  printf("Preis in Euro:");
  scanf("%f",&lebensmittel[n].preis);
  n++;
 
 } 

 bewertung_best=999999999;
 backtrack(kombination_best,0);

}
