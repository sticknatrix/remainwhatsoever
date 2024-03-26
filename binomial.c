/* just basic principle with randomization, no affront or allusion here really*/

int main(void)
{
 int array1[400];
 int array2[400];


#define SELSIZE 200


 int n,n2;
 int overlap;

  signed int display[400];
  n=0;
  while ( n < 400) display[n]=0, n++;

 while ( 1 )
 {
  n=0;
  while ( n < 200 )
  {
   repeat1:
   array1[n]=rand()%400;
   n2=0;
   while ( n2 < n ) { if ( array1[n]==array1[n2] ) goto repeat1; n2++; } 
   n++;
  }  

  n=0;
  while ( n < SELSIZE )
  {
   repeat2:
   array2[n]=rand()%400;
   n2=0;
   while ( n2 < n ) { if ( array2[n]==array2[n2] ) goto repeat2; n2++; } 
   n++;
  }  

  overlap=0;
  
  n=0;
  while ( n < 200 )
  {
   n2=0;
    while ( n2 < SELSIZE )
   {
    if ( array2[n2]==array1[n] ) overlap++;
    n2++;  
   }
   n++;
  }  
 
  display[overlap]++;
  //printf("%d\n", overlap );
  
  if ( 80 - display[overlap] < -400) break;
 if ( kbhit()) break;
 }
 unsigned char screen[160][90];
 
 int x,y;
 
  y=0;
  while ( y < 90)
  {
   x=0;
   while ( x < 160)
   {
    screen[x][y]=219;
    n=80-display[x]/10;
	while ( n > -1)screen[x][n]=' ', n--;
    x++;
   }
  y++;

 }


 system("cls\n");
  y=0;
  while ( y < 81)
  {
   x=0;
   while ( x < 150)
   {
    printf("%c", screen[x][y]);
	x++;
   }
   printf("\n");
  y++;

 }
 
 int overlaphighest=0;
while ( n < 400) { if ( display[n] > overlaphighest) overlaphighest=display[n], n2=n; n++;}

 printf("\n%d war wohl der haeufigste Wert.", n2);


}
