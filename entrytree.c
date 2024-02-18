struct
{
 signed int nextelem[255];
 unsigned char elems[255][10];
 
} datei;


unsigned char screen[160][120];

int Xshift,Yshift;

unsigned int pushelem[255];
unsigned int pusholdx[255];
unsigned int pusholdy[255];

int Stackcount;



int main(void)
{

 int x,y;
 unsigned char cmd[255], name[255];


    int n=0,n2=0,n3=0;
    while (n < 255 ) datei.nextelem[n]=-1, n++;
    n=0; 
    Stackcount=0;
    
    int curdir=0;
    
    datei.nextelem[curdir]=-2;
    datei.elems[curdir][0]='-';
   
    pushelem[0]=0;

 while (1 )
 {

 scanf ("%s %s", cmd, name );

 if ( strcmp ( cmd, "touch" ) == 0 )
 {
  
   n=curdir;
  while ( datei.elems[n][0]!='-' ) n=datei.nextelem[n];
  strcpy(datei.elems[n], name );
  datei.nextelem[n]=0; // Pfuschdihuschdi
   
  n2=0;
   while ( datei.nextelem[n2] != -1 ) n2++; 
  datei.nextelem[n]=(unsigned char)n2;
  datei.elems[n2][0]='-';
  datei.nextelem[n2]=-2; // zeigt es auf kein naechstes Element, ist es frei   

 }
 
 
 
 
 if ( strcmp ( cmd, "md" ) == 0 )
 {
  
  n=curdir;
  while ( datei.elems[n][0]!='-' ) n=datei.nextelem[n];
  strcpy(datei.elems[n]+2, name );
   datei.nextelem[n]=0; // Pfuschdihuschdi
   datei.elems[n][0]='~';
   n3=n;
   
   
  n2=0;
   while ( datei.nextelem[n2] != -1 ) n2++; 
   datei.elems[n][1]=(unsigned char)n2;
   
   
   int n4=0;
   n4=n2;
   datei.nextelem[n2]=0; // Pfuschdihuschdi
   
   //datei.nextelem[n2]=-2;
  
  
  datei.elems[n2][0]='-';
  
 
  
  n2=0;
   while ( datei.nextelem[n2] != -1 ) n2++; 
  datei.nextelem[n2]=-2;
  datei.elems[n2][0]='-';
   
   datei.nextelem[n3]=(unsigned char)n2;
   
   datei.nextelem[n4]=-2;
     

 }
 
 if ( strcmp (cmd,"root")==0 ) curdir=0;

 if ( strcmp ( cmd, "cd" ) == 0 )
 {
  
  n=curdir;
  while ( strcmp( datei.elems[ n/*datei.nextelem[n] */ ]+2,name)!=0 &&datei.elems[ n /* datei.nextelem[n] */ ][0]!='~' ) n=datei.nextelem[n];
   
   if ( datei.elems[n][0]!='-') curdir=datei.elems[n][1];

   }

 if ( strcmp(cmd,"dir" ) == 0 )
 {
  y=0;
  while ( y < 90 )
  {
   x=0;
   while ( x < 160 )
   {
    screen[x][y]=' ';
    x++;
   }
   y++;
  }


  Xshift=0, Yshift=0;
  Stackcount=1;
  n=0;
    
    int Stackcount2=0;
    Stackcount2=0;
    
    pusholdx[Stackcount2]=0;
    pusholdy[Stackcount2]=0;
    
    
  do
  {
  	
   if ( datei.elems[n][0]=='~' )
   {
   
   
   n2=2;   
   
    //printf("Verzeichnis: %s",datei.elems[n]), getch();
	 
    x=pusholdx[Stackcount2];
	y=pusholdy[Stackcount2];
    
     Xshift=x+10;
    Yshift+=5;
    
    while ( y< Yshift)
    {
     if ( y < Yshift) screen[x][y]==' ' ? screen[x][y]=219:0, y++;
    	
	}
	
	
    while ( x< Xshift)
    {
     if ( x < Xshift) screen[x][y]= screen[x][y]==' ' ? screen[x][y]=219:0, x++;
	}
   
   while ( datei.elems[n][n2]!='\0' ) screen[Xshift+n2-2] [Yshift]=datei.elems[n][n2],n2++;
  
  
    Stackcount2++;
   	pusholdx[Stackcount2]=Xshift;
    pusholdy[Stackcount2]=Yshift;
   
      pushelem[Stackcount]=datei.nextelem[n] ;
      n= datei.elems[n][1];
      //n=datei.nextelem[n];
      
      
      //printf("%s",datei.elems[n]), getch();
	    Stackcount++;   
	 }
   else
   if ( datei.elems[n][0]=='-' )
   {
     Stackcount--;
     Stackcount2--;
    n=pushelem[Stackcount];
   	}
   else
   {
    
    x=pusholdx[Stackcount2];
	y=pusholdy[Stackcount2];
   
    Yshift+=5;
    Xshift=x;
    
    while ( y< Yshift)
    {
     if ( y < Yshift)  screen[x][y]==' ' ? screen[x][y]=219:0, y++;
    	
	}
	
	
    while ( x< Xshift)
    {
     if ( x < Xshift)  screen[x][y]==' ' ? screen[x][y]=219:0, x++;
	}
 
    n2=0;   
    //printf("Flach: %s",datei.elems[n]),getch();
	while ( datei.elems[n][n2]!='\0' ) screen[Xshift+n2] [Yshift]=datei.elems[n][n2],n2++;
    n=datei.nextelem[n];  
   
   	pusholdx[Stackcount2]=Xshift;
    pusholdy[Stackcount2]=Yshift;
   
   
   }
   
   
  } while ( Stackcount > 0 /*|| datei.elems[n][0]!='-' */);



  system("cls\n");
  y=0;
  while ( y < 80 )
  {
   x=0;
   while ( x < 160 )
   {
    printf("%c",screen[x][y] );
    x++;
   }
   printf("\n");
   y++;
  }


 }

 }
}
