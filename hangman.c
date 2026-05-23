/* man könnte noch ein Woerterbuch einlesen, das man nach Haeufigkeit sortiert hat?!?*/

#include <stdio.h>
#include <string.h>

unsigned char guess[35];
unsigned char guessed[35];

struct
{
 unsigned char string[35];
 int tryfail;
}word[50000];

int num_words=0;
int wordtried;

struct
{
 unsigned char c;
 int tryfail;
}letter[26];


int tryletter(void)
{
int n=0,n2=0,n3=0;

 while ( (strlen(guess)!=strlen(word[n].string)||word[n].tryfail==1)&&n<num_words)n++;
  n2=0;
  n3=0;
  if ( n<num_words)
  while (n2<26)
  {
    n3=100;
   if ( letter[n2].tryfail==0)
    {
      n3=0;
     while ( word[n].string[n3]!=letter[n2].c&&n3<strlen(word[n].string))n3++;
      if (n3<strlen(word[n].string))break;
   }
   n2++;
  }
  wordtried=n;
  if ( n<num_words)if (n3==strlen(word[n].string))n=num_words;
   
  if(n==num_words)
  {
   while (letter[n2].tryfail==1)n2++;
   }
 return n2;
}


int main(void)
{
 unsigned char try;
 int notfound=0;
 int n=0;
scanf("%s",guess);

  while ( n < 4)
  {
   word[n].tryfail=0;
   scanf("%s", word[n].string);
   n++;
   num_words++;
  }
  
  n=0;
  letter[0].c='e';
  letter[0].tryfail=0;
letter[1].c='n';
  letter[1].tryfail=0;
letter[2].c='i';
  letter[2].tryfail=0;
letter[3].c='s';
  letter[3].tryfail=0;
letter[4].c='r';
  letter[4].tryfail=0;
letter[5].c='a';
  letter[5].tryfail=0;
letter[6].c='t';
  letter[6].tryfail=0;
letter[7].c='d';
  letter[7].tryfail=0;
letter[8].c='h';
  letter[8].tryfail=0;
letter[9].c='u';
  letter[9].tryfail=0;
letter[10].c='l';
  letter[10].tryfail=0;
letter[11].c='c';
  letter[11].tryfail=0;
letter[12].c='g';
  letter[12].tryfail=0;
letter[13].c='m';
  letter[13].tryfail=0;
letter[14].c='o';
  letter[14].tryfail=0;
letter[15].c='b';
  letter[15].tryfail=0;
letter[16].c='w';
  letter[16].tryfail=0;
letter[17].c='f';
  letter[17].tryfail=0;
letter[18].c='k';
  letter[18].tryfail=0;
letter[19].c='z';
  letter[19].tryfail=0;
letter[20].c='p';
  letter[20].tryfail=0;
letter[21].c='v';
  letter[21].tryfail=0;
letter[22].c='j';
  letter[22].tryfail=0;
letter[23].c='y';
  letter[23].tryfail=0;
letter[24].c='x';
  letter[24].tryfail=0;
letter[25].c='q';
  letter[25].tryfail=0;

 
 strcpy(guessed,"                                  ");
 guessed[strlen(guess)]='\0';
 
 while ( strcmp(guess,guessed)!=0)
 {
  try=letter[tryletter()].c;
 
 printf("versuche %d\n",try);
 
  notfound=1;
  n=0;
  while(n<strlen(guess))
  {
  if ( guess[n]==try)
  {
   guessed[n]=try,notfound=0;
   }
   if ( wordtried<num_words)if ( word[wordtried].string[n]!=guessed[n]&&word[wordtried].string[n]==try)notfound=1;
   n++;
  }
  
   letter[tryletter()].tryfail=1;
  if ( notfound==1)
  {
   if ( wordtried<num_words) word[wordtried].tryfail=1;
  }
  printf("%s\n", guessed);
 
  }
}
