#include <time.h>
#include <stdio.h>

unsigned char *wordlist, *wordlist_ptr;

int main(void)
{
 FILE *wordbook;
  unsigned char typequick[20];
  int n=0;

  wordbook=fopen("words.txt","rb");
  wordlist=malloc(sizeof(unsigned char)*1000000);
  while(!feof(wordbook)&&n<999999)wordlist[n]=toupper(fgetc(wordbook)),n++;
   wordlist[n]='\0';

  while (1)
  {
   clock_t clocktmp;
   n=0;
   do
   {
    clocktmp=clock();
    //while(!kbhit());
    typequick[n]=toupper(getch());
     n++;   
   } while ( clock()<clocktmp+100);
   typequick[n]='\0';
  
    int n2,n3;
   unsigned char typequickbuf[100];
    unsigned char compare_word[100];
     void *pos;
	 float eval,evalbest;
   n=0;
   wordlist_ptr=wordlist;
   evalbest=0; 
  while (((unsigned char *)wordlist_ptr)[0]!='\0')
   {
    strcpy(typequickbuf,typequick);
     sscanf(wordlist_ptr,"%s",compare_word);
      wordlist_ptr+=strlen(compare_word)+1;
    
    eval=0;
     n3=0;    
    while ( typequickbuf[n3]!='\0')
    {
     n2=0;
     while (compare_word[n2]!='\0')
     {
       if (typequickbuf[n3]==compare_word[n2] ) eval++,compare_word[n2]='e';
       n2++;
     }  
      n3++;
    }
    eval-=abs(strlen(compare_word)-strlen(typequickbuf));
  
   if ( evalbest<= eval ) evalbest=eval, pos=wordlist_ptr-(strlen(compare_word)+1);

   }
   if (evalbest>=3)
   {
    sscanf(pos,"%s",typequickbuf);
   }
   printf("%s",typequickbuf);
  
  }
}
