struct
{
 unsigned char Was[255];
 unsigned char Wie[10][255];
 signed int wie;
 int pos;
} Begebenheiten[10];



 signed int *table;

  unsigned int Elemente;
  unsigned int Eigenschaften;

int lookup(unsigned char Was[], unsigned char Wie[], int roworcolumn)
{
 int n=0;
 while (strcmp(Begebenheiten[n].Was,Was)!=0 && n < Elemente)n++;
 if ( n==Elemente)return -1;
 int n2=0;
 while ( strcmp(Begebenheiten[n].Wie[n2],Wie)!=0 && n2 < Eigenschaften)n2++;
 if ( n2==Eigenschaften)return -2;
 
 
  if ( roworcolumn==1)return (n==0? n*Eigenschaften : Elemente*Eigenschaften-n*Eigenschaften)+n2;
	if ( n-1 < 0) return -1;
	return (n-1)*Eigenschaften+n2;
}

void settable(int n, int n2, signed int value)
{
  int n3;
  if ( n<0||n2<0)return;
 if ( value==1)
 {
 	
  table[(n/Eigenschaften)*Eigenschaften+(n%Eigenschaften)+
        (n2/Eigenschaften)*Elemente*Eigenschaften*Eigenschaften+(n2%Eigenschaften)*Elemente*Eigenschaften]=1;
   n3=0;
   
   while ( n3 < Eigenschaften)
   {
   	if ( n3!=n%Eigenschaften)
     table[(n/Eigenschaften)*Eigenschaften+(n3%Eigenschaften)+
        (n2/Eigenschaften)*Elemente*Eigenschaften*Eigenschaften+(n2%Eigenschaften)*Elemente*Eigenschaften]=0;
   	n3++;
   }
    n3=0;
    while ( n3 < Eigenschaften)
   {
   	if ( n3!=n2%Eigenschaften)
     table[(n/Eigenschaften)*Eigenschaften+(n%Eigenschaften)+
        (n2/Eigenschaften)*Elemente*Eigenschaften*Eigenschaften+(n3%Eigenschaften)*Elemente*Eigenschaften]=0;
   	n3++;
   }
  }
  else if (value==0)
  {
  table[(n/Eigenschaften)*Eigenschaften+(n%Eigenschaften)+
         (n2/Eigenschaften)*Elemente*Eigenschaften*Eigenschaften+(n2%Eigenschaften)*Elemente*Eigenschaften]=0;
  n3=0;
   
   while ( n3 < Eigenschaften)
   {
   	if ( n3!=n%Eigenschaften)
     table[(n/Eigenschaften)*Eigenschaften+(n3%Eigenschaften)+
        (n2/Eigenschaften)*Elemente*Eigenschaften*Eigenschaften+(n2%Eigenschaften)*Elemente*Eigenschaften]=-1;
   	n3++;
   }
    n3=0;
    while ( n3 < Eigenschaften)
   {
   	if ( n3!=n2%Eigenschaften)
     table[(n/Eigenschaften)*Eigenschaften+(n%Eigenschaften)+
        (n2/Eigenschaften)*Elemente*Eigenschaften*Eigenschaften+(n3%Eigenschaften)*Elemente*Eigenschaften]=-1;
   	n3++;
   }
  
  }
}

int gettable(int n, int n2)
{
if ( table[(n/Eigenschaften)*Eigenschaften+(n%Eigenschaften)+
      (n2/Eigenschaften)*Elemente*Eigenschaften*Eigenschaften+(n2%Eigenschaften)*Elemente*Eigenschaften]==1) return '+';
 else
   if (table[(n/Eigenschaften)*Eigenschaften+(n%Eigenschaften)+
      (n2/Eigenschaften)*Elemente*Eigenschaften*Eigenschaften+(n2%Eigenschaften)*Elemente*Eigenschaften]==0)return '-';
  else return ' ';
}

int main(void)
{
  
  unsigned char screen[160][90];
  
  int n=0,n2=0;
  
   int x,y;
   
   y=0;
   while ( y < 90)
   {
   	x=0;
   	while ( x < 160)
   	{
   	 screen[x][y]=' ';
   	  x++;
	}
	y++;
   }
  
  
  printf("Wieviele Elemente\?");
  scanf("%d",&Elemente);
  
  printf("Wieviele Eigenschaften\?");
  scanf("%d",&Eigenschaften);
  table=malloc(sizeof(signed int)*100000);
   n=0;
  while ( n<Elemente*Eigenschaften*Elemente*Eigenschaften)
  {
   table[n]=-1; 	
  	n++;
  }
  
  strcpy(Begebenheiten[0].Was,"Pferd");
  strcpy(Begebenheiten[1].Was,"Halter");
  strcpy(Begebenheiten[2].Was,"Ausritt");
  strcpy(Begebenheiten[3].Was,"Pflege");
  strcpy(Begebenheiten[4].Was,"Zeit");
   //settable(15,0,1);
  strcpy(Begebenheiten[4].Wie[0],"0Uhr");
  strcpy(Begebenheiten[4].Wie[1],"1Uhr");
  strcpy(Begebenheiten[4].Wie[2],"2Uhr");
  strcpy(Begebenheiten[4].Wie[3],"3Uhr");
  strcpy(Begebenheiten[4].Wie[4],"4Uhr");
  
  strcpy(Begebenheiten[0].Wie[0],"Ajax");
  strcpy(Begebenheiten[0].Wie[1],"Wendy");
  strcpy(Begebenheiten[0].Wie[2],"Fury");
  strcpy(Begebenheiten[0].Wie[3],"Blacky");
  strcpy(Begebenheiten[0].Wie[4],"Nilson");
  
  
  n=0;
  while ( n < 10)Begebenheiten[n].wie=-1,n++;
  
  int screenlines[10];
  int xmax=0,ymax=0;
   n=0;
    n2=n+1;
    
      int n3=0;
	 
	 	n3=0;
	   while(Begebenheiten[0].Was[n3]!='\0')
	   {
	    screen[0+n3][10]=Begebenheiten[0].Was[n3],n3++;
        if (xmax<n3)xmax=n3;
		}
	  
	  screenlines[0]=Elemente;
    while (n2<Elemente)
    {
      printf("%d auf %d ",n,n2);
      
       	n3=0;
       	while ( Begebenheiten[n2].Was[n3]!='\0')
		{
		 screen[n2*10+1][n3]=Begebenheiten[n2].Was[n3],n3++;
          if ( ymax<n3)ymax=n3;
	  	
       }
       
	  n2++;
	}
	printf("\n");
      n2--;
      int n4=0;
      n4=0;
	while (n2>0)
	{

	 n=1;
	  screenlines[n4]=n2-n;
	 while (n<n2)
	 {
	 	n3=0;
	 	while ( Begebenheiten[n2].Was[n3]!='\0')
	 	{
	 	 screen[n3][(n4)*10+20]=Begebenheiten[n2].Was[n3];
	 		n3++;
	 		if ( xmax<n3)xmax=n3;
		 }
	  printf("%d auf %d ",n2,n);
	  n++;
	 }
	 printf("\n");
	 n2--;
    n4++;
	 }
  
while(1)
{ 
 y=0;
 while ( y < 90-ymax)
 {
   x=0;
   while ( x < 120-xmax)
   {
   	if ( screenlines[(y)/10]>=(x)/10&&(y)/10< Elemente-1)
   	if ( x%10<Eigenschaften&&y%10<Eigenschaften)screen[x+xmax][y+ymax]=gettable(((x)/10)*Eigenschaften+x%Eigenschaften,((y/10) == 0 ? 0 : (Elemente-1-(y/10)))*Eigenschaften +((y)%Eigenschaften));
   	x++;
   }
  y++;
 }
 
  y=0;
  while ( y < 90)
  {
    x=0;
    while ( x < 120)
    {
     printf("%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y]);
     x+=4;	
	}
  	printf("\n");
  	y++;
  }
  unsigned char was[255],wie[255],was2[255],wie2[255];
  unsigned char c;
  scanf("%s %s %s %s %c",was,wie,was2,wie2,&c);
  if ( c=='+')c=1; else if ( c=='-')c=0;else c=-1;
  settable(lookup(was,wie,0),lookup(was2,wie2,1),c);
  settable(lookup(was2,wie2,0),lookup(was,wie,1),c);
  
  printf("%d %d %d",lookup(was,wie,0),lookup(was2,wie2,1),c);getchar();
 }

}
