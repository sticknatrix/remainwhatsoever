#include <time.h>

struct
{
 int len;
 int shortorlong[4];
}morse_code[26];

void morse_decode(int *token, int len)
{
 int letter=0;
 int n2=0;
  letter=0;
  int len_prev=0;
  unsigned char letterout;
  
  len_prev=0;
  while ( letter<26)
  {
  	int n=0;
  	n=0;
   while (n<len&& n < morse_code[letter].len&&token[n]==morse_code[letter].shortorlong[n])n++;
  
   if ( n==morse_code[letter].len&&n>=len_prev){letterout=letter+0x41,len_prev=n;}
   letter++;
  }
  printf("%c",letterout);
   
}

int main(void)
{
	int token[10];int n=0;
int len_c=0;

 
 while (1)
 {
 n=0;
 len_c=0;
 while ( n < 4)
 {
  clock_t test;
  	
 	test=clock()+5000;
/// getch();
 while(!kbhit())
 {
 	if ( clock()>test) goto end;
 }
 
 	len_c=0;
 while (kbhit())
 {
  len_c++,getch();
   test=clock()+1000;
   while(test>clock());
  }
  
  token[n]=len_c;/* printf("%d ",token[n]);*/
   n++;
  }
  int n2=n;
  
  end:
  	n2=n;
  	float avg=0;
  	float lowest=10000000;
  	float threshold;
  n=0;
  while(n<n2)avg+=token[n],n++;
  n=0;
  while ( n < n2){if ( token[n]<lowest)lowest=token[n];n++;}
  threshold=((avg)/n2+lowest)/2;
  n=0;
  while( n < n2){
  	if ( token[n]<=threshold)token[n]=0; else token[n]=1;
  	n++;
  }
  
  n=0;
  while ( n < n2)
  {
  	if ( token[n]==0)printf("kurz "); else printf("lang ");
  	n++;
  }
/* You might check the table on Your own */ 
  morse_code[0].len=2,morse_code[0].shortorlong[0]=0,morse_code[0].shortorlong[1]=1;
  morse_code[1].len=4,morse_code[1].shortorlong[0]=1,morse_code[1].shortorlong[1]=0,morse_code[1].shortorlong[2]=0,morse_code[1].shortorlong[3]=0;
  morse_code[2].len=4,morse_code[2].shortorlong[0]=1,morse_code[2].shortorlong[1]=0,morse_code[2].shortorlong[2]=1,morse_code[2].shortorlong[3]=0;
  morse_code[3].len=3,morse_code[3].shortorlong[0]=1,morse_code[3].shortorlong[1]=0,morse_code[3].shortorlong[3]=0;
  morse_code[4].len=1,morse_code[4].shortorlong[0]=0;
  morse_code[5].len=4,morse_code[5].shortorlong[0]=0,morse_code[5].shortorlong[1]=0,morse_code[5].shortorlong[2]=1,morse_code[5].shortorlong[3]=0;
  morse_code[6].len=3,morse_code[6].shortorlong[0]=1,morse_code[6].shortorlong[1]=1,morse_code[6].shortorlong[2]=0;
  morse_code[7].len=4,morse_code[7].shortorlong[0]=0,morse_code[7].shortorlong[1]=0,morse_code[7].shortorlong[2]=0,morse_code[7].shortorlong[3]=0;
  morse_code[8].len=2,morse_code[8].shortorlong[0]=0,morse_code[8].shortorlong[1]=0;
  //morse_code[9].len=2,morse_code[9].shortorlong[0]=0,morse_code[9].shortorlong[1]=1;
  morse_code[9].len=4,morse_code[9].shortorlong[0]=0,morse_code[9].shortorlong[1]=1,morse_code[9].shortorlong[2]=1,morse_code[9].shortorlong[3]=1;
  morse_code[10].len=3,morse_code[10].shortorlong[0]=1,morse_code[10].shortorlong[1]=0,morse_code[10].shortorlong[2]=1;
  morse_code[11].len=4,morse_code[11].shortorlong[0]=0,morse_code[11].shortorlong[1]=1,morse_code[11].shortorlong[2]=0,morse_code[11].shortorlong[3]=0;
  
  morse_code[12].len=2,morse_code[12].shortorlong[0]=1,morse_code[12].shortorlong[1]=1;
  morse_code[13].len=2,morse_code[13].shortorlong[0]=1,morse_code[13].shortorlong[1]=0;
  morse_code[14].len=3,morse_code[14].shortorlong[0]=1,morse_code[14].shortorlong[1]=1,morse_code[14].shortorlong[2]=1;
  morse_code[15].len=4,morse_code[15].shortorlong[0]=0,morse_code[15].shortorlong[1]=1,morse_code[15].shortorlong[2]=1,morse_code[15].shortorlong[3]=0;
  morse_code[16].len=4,morse_code[16].shortorlong[0]=1,morse_code[16].shortorlong[1]=1,morse_code[16].shortorlong[2]=0,morse_code[16].shortorlong[3]=1;
  morse_code[17].len=3,morse_code[17].shortorlong[0]=0,morse_code[17].shortorlong[1]=1,morse_code[17].shortorlong[2]=0;
  morse_code[18].len=3,morse_code[18].shortorlong[0]=0,morse_code[18].shortorlong[1]=0,morse_code[18].shortorlong[2]=0;
  morse_code[19].len=1,morse_code[19].shortorlong[0]=1;
  morse_code[20].len=3,morse_code[20].shortorlong[0]=0,morse_code[20].shortorlong[1]=0,morse_code[20].shortorlong[2]=1;
  morse_code[21].len=4,morse_code[21].shortorlong[0]=0,morse_code[21].shortorlong[1]=0,morse_code[21].shortorlong[2]=0,morse_code[21].shortorlong[3]=1;
  morse_code[22].len=3,morse_code[22].shortorlong[0]=0,morse_code[22].shortorlong[1]=1,morse_code[22].shortorlong[2]=1;
  morse_code[23].len=4,morse_code[23].shortorlong[0]=1,morse_code[23].shortorlong[1]=0,morse_code[23].shortorlong[2]=0,morse_code[23].shortorlong[3]=1;
  morse_code[24].len=4,morse_code[24].shortorlong[0]=1,morse_code[24].shortorlong[1]=0,morse_code[24].shortorlong[2]=1,morse_code[24].shortorlong[3]=1;
  morse_code[25].len=4,morse_code[25].shortorlong[0]=1,morse_code[25].shortorlong[1]=1,morse_code[25].shortorlong[2]=0,morse_code[25].shortorlong[3]=0;
  
 if ( n2>0)
  morse_decode(token,n2);
  
 }

}
