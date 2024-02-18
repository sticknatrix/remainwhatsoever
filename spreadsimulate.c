#define NUM_INDIVIDUALS 100
#define TAKE_CARE 3
#define DISEASE_DURATION 50

struct
{
 int infective;
 int healthy_again;
  signed int resi_x, resi_y;
  signed int vary_x, vary_y;
  int how_careful;
} individual[NUM_INDIVIDUALS];

unsigned char screen[100][100];

int main(void)
{
 int n, n2;
 int x,y;

 n=0;
 while ( n < NUM_INDIVIDUALS )
 {
  individual[n].how_careful=TAKE_CARE;
  individual[n].infective=0;
  individual[n].healthy_again=0;
  n++;
 }
 
 n=0;
 y=0;
 while ( y < NUM_INDIVIDUALS/10 )
 {
  x=0;
  while ( x < NUM_INDIVIDUALS/10 )
  {
   individual[n].resi_x=x*10, individual[n].resi_y=y*10;
   n++;
   x++;
  }
  y++;
 }

   individual[0].infective=DISEASE_DURATION;
   

 while ( 1 )
 {
 	
    n=0;
  while ( n < NUM_INDIVIDUALS )
  {
   individual[n].vary_x=(rand()%5*individual[n].how_careful)%50-25, individual[n].vary_y=(rand()%5*individual[n].how_careful)%50-25;
   n++;
  }
  
  
 n=0;
 while ( n < NUM_INDIVIDUALS )
 {
  n2=0;
  while ( n2 < NUM_INDIVIDUALS )
  {
   if ( n2==n ) { n2++; continue; }
   if ( (individual[n].resi_x+individual[n].vary_x)/TAKE_CARE == (individual[n2].resi_x+individual[n2].vary_x)/TAKE_CARE &&
        (individual[n].resi_y+individual[n].vary_y)/TAKE_CARE == (individual[n2].resi_y+individual[n2].vary_y)/TAKE_CARE )
    if ( individual[n2].infective>0 && individual[n2].healthy_again==0&& individual[n].infective==0 && individual[n].healthy_again==0 )
       individual[n].infective=DISEASE_DURATION;
    n2++;
   }

   n++;

  }

  int infections_counted;
   infections_counted=0;

  n=0;
  while ( n < NUM_INDIVIDUALS )  
  {
   if ( individual[n].infective > 0 )
   {
    infections_counted++, individual[n].infective--;
    if ( individual[n].infective==0)individual[n].healthy_again=1;
   }
   else
   if ( individual[n].healthy_again==1 ) infections_counted++;
   n++;
  }

  y=0;
  while ( y < 100 )
  {
   x=0;
   while ( x < 100 )
   {
    screen[x][y]=' ';
    x++;
   }
   y++;
  }
 
  n=0;
 while ( n < NUM_INDIVIDUALS )
 {
   if ( individual[n].resi_x+individual[n].vary_x > 0 && individual[n].resi_x+individual[n].vary_x < 100 &&
        individual[n].resi_y+individual[n].vary_y > 0 && individual[n].resi_y+individual[n].vary_y < 100 )
  {
  	unsigned char istate;
    if ( individual[n].healthy_again==1 ) istate=177;
    else
    if ( individual[n].infective > 0 ) istate=176;
    else  istate=219;
    
    screen[(individual[n].resi_x+individual[n].vary_x)/2][(individual[n].resi_y+individual[n].vary_y)/2]=istate;
   }
  n++;
 }
  system("cls\n");
  y=0;
  while ( y < 50 )
  {
   x=0;
   while ( x < 50 )
   {
    printf("%c",screen[x][y]);
    x++;
   }
   printf("\n");
   y++;
  }

  printf("%d Infizierte",infections_counted);

 } 



}
