#include <stdio.h>

unsigned char input[30][20];
int pos;
int rnum;
int stack[50];
int stackcount=0;
int result;

int isnum(unsigned char string[])
{
 int n=0;
 while ( string[n]>=0x30&&string[n]<=0x39&&string[n]!='\0')n++;

 if ( string[n]!='\0' ) return 0;
 else return 1;
}


expression ()
{
 while ( strcmp(input[pos],";")!=0) term(),factor(),numop();
}

term ()
{
	unsigned char bufstring[255];
 if ( strcmp(input[pos],"(")==0 )pos++, bracket();
   bufstring[0]='\0';
  if ( isnum(input[pos])||strcmp(input[pos],"x")==0 ) stack[stackcount]=atoi(input[pos]),stackcount++,printf(" % s ",input[pos])/*strcpy(bufstring,input[pos])*/, pos++,factor(),numop();
   rnum++;
   
   /*printf(" %s ",bufstring);*/
   
  return;
}

bracket()
{
 if ( strcmp(input[pos],"(")==0 )pos++, bracket();

  term();
 if ( strcmp(input[pos],")")==0)
 { pos++;
 ( factor()||numop() ) ? term() : 0;
  return;
 }
  else printf("Fehler"); return;
	
	
	
}

numorbracket()
{
 if ( strcmp(input[pos],"(")==0 )pos++, bracket();
 if ( isnum(input[pos])||strcmp(input[pos],"x")==0)
 {
  printf(" %s ",input[pos]),stack[stackcount]=atoi(input[pos]),stackcount++;
 	pos++;
  return;
 }	
	
}


factor()
{
 if ( strcmp(input[pos],"/")==0) pos++,numorbracket(),printf(" geteilt durch "),stack[stackcount-2]=stack[stackcount-2]/stack[stackcount-1],stackcount--;
  else
   if ( strcmp(input[pos],"*")==0 ) pos++,numorbracket(),printf(" mal "),stack[stackcount-2]=stack[stackcount-2]*stack[stackcount-1],stackcount--;
   else return 0;
   return 1;
}

numop()
{
  if ( strcmp(input[pos],"+")==0 ) pos++,term(),printf(" addiert zu "),stack[stackcount-2]=stack[stackcount-2]+stack[stackcount-1],stackcount--;
  else
   if ( strcmp(input[pos],"-")==0) pos++,term(),printf(" minus "),stack[stackcount-2]=stack[stackcount-2]-stack[stackcount-1],stackcount--;
   else return 0;
   return 1;
  }


int main(int argc, char *argv[])
{

if ( argc!=4)return;	
	if ( argv[2][0]!='*')
	{
 do {sscanf(argv[3],"%s",input[pos]); argv[3]+=strlen(input[pos])+1;if ( strcmp(input[pos],"x")==0) strcpy(input[pos],argv[2]);pos++; } while ( input[pos-1][0]!=';');
 pos=0;
 rnum=0;
 stackcount=0;
 expression();
 printf("%d",abs(stack[0]-atoi(argv[1])));
 return abs(stack[0]-atoi(argv[1]));
 
}

else
{
 int screen[120][100];
   unsigned char stelle[20];
   signed int stelle_c=0;
 int y=0,x=0;
 
 y=0;
 while ( y < 100)
 {
   x=0;
   while ( x < 120)
   {
    screen[x][y]=' ';
    x++;
   }
  y++;
 }

 void *backptr=argv[3];
 	
 while ( stelle_c<100)
 {
   sprintf(stelle, "%d",stelle_c);
   //itoa(stelle_c,stelle,10);
 	argv[3]=backptr;
 	pos=0;
  do {sscanf(argv[3],"%s",input[pos]); argv[3]+=strlen(input[pos])+1;if ( strcmp(input[pos],"x")==0) strcpy(input[pos],stelle);pos++; } while ( input[pos-1][0]!=';');
 pos=0;
 rnum=0;
 stackcount=0;
 expression();
   if ( stack[0]>0&&stack[0]<99)screen[stelle_c][99-stack[0]]='*'; 	
   stelle_c++;
 }
	
system("cls");
y=0;
 while ( y < 100)
 {
   x=0;
   while ( x < 80)
   {
    printf("%c",screen[x][y]);
    x++;
   }
   printf("\n");
  y++;
 }
 
 }
 /* weiter mit Stack?!?*/
}
