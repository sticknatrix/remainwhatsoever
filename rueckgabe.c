#include <stdio.h>
#include <stdlib.h>

int muenzen_2_1_50[3];
int zielwerte[3];
int rueckgabe[3];
int best_score=100000;



void backtrack(int *muenzen_, int betrag, int rec_depth)
{
  int muenzen[3];
  muenzen[0]=muenzen_[0],
  muenzen[1]=muenzen_[1],
  muenzen[2]=muenzen_[2];
  
  int score;
  if ( (muenzen_2_1_50[0]-muenzen[0])*200+(muenzen_2_1_50[1]-muenzen[1])*100+(muenzen_2_1_50[2]-muenzen[2])*50==betrag)
  {
   score=abs(muenzen[0]-zielwerte[0])+abs(muenzen[1]-zielwerte[1])+abs(muenzen[2]-zielwerte[2]);
   if ( score<=best_score ) best_score=score,rueckgabe[0]=muenzen_2_1_50[0]-muenzen[0],rueckgabe[1]=muenzen_2_1_50[1]-muenzen[1],
                                rueckgabe[2]=muenzen_2_1_50[2]-muenzen[2];
  }
  
  if ( rec_depth>10 ) return;
  
  if ( muenzen[0]>0)
  {
   muenzen[0]--;
   backtrack(muenzen,betrag,rec_depth+1);
   muenzen[0]++;
  }

 if ( muenzen[1]>0)
  {
   muenzen[1]--;
   backtrack(muenzen,betrag,rec_depth+1);
   muenzen[1]++;
  }
  
 if ( muenzen[2]>0)
  {
   muenzen[2]--;
   backtrack(muenzen,betrag,rec_depth+1);
   muenzen[2]++;
  }

}


int main(void)
{
 muenzen_2_1_50[0]=10,
 muenzen_2_1_50[1]=10,
 muenzen_2_1_50[2]=10;
 zielwerte[0]=10, // target fill stands
 zielwerte[1]=10,
 zielwerte[2]=10;

int betrag_in_cent;

while (1)
{
  printf("Rueckgeld in Cent eingeben:");
  scanf("%d",&betrag_in_cent);
  best_score=100000;
 backtrack(muenzen_2_1_50,betrag_in_cent,0);

 if ( best_score==100000) { printf("kein Ergebnis (gefunden).\n");return 0;} 
  else printf("zwei Euro: %d ein Euro: %d Fuenfzig Cent: %d\n",rueckgabe[0],rueckgabe[1],rueckgabe[2]);
   muenzen_2_1_50[0]-=rueckgabe[0],
   muenzen_2_1_50[1]-=rueckgabe[1],
   muenzen_2_1_50[2]-=rueckgabe[2];
 }

}


