int brett[24];
int vbrett[24];

int connections[24][4];

/* bei dem Minimax einfach anstelle der Tastatureingaben immer drei Buchstaben durchprobieren je Aufruf?!?*/

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

int steineA,steineB;
int takenA,takenB;

int vsteineA,vsteineB;
int vtakenA,vtakenB;

int inputpos;
int move_error=0;
int countend_glob=15625;

unsigned char outstr[4];

int onlymill(int player)
{
	int n=0;
  while ( n<24)
  {
  	if ( brett[n]==player)if(ismill(player,n)==0) return 0;
  	n++;
  }
  return 1;
}


int vonlymill(int player)
{
	int n=0;
  while ( n<24)
  {
  	if ( vbrett[n]==player)if(vismill(player,n)==0) return 0;
  	n++;
  }
  return 1;
}


int ismill(int player, int n)
{
	
	if(brett[n]==player)
 if (connections[n][RIGHT]!=-1)if (brett[connections[n][RIGHT]]==player)
 if(connections[n][LEFT]!=-1)if(brett[connections[n][LEFT]]==player)return 1;

 if(brett[n]==player)
 if (connections[n][DOWN]!=-1)if (brett[connections[n][DOWN]]==player)
 if(connections[n][UP]!=-1)if(brett[connections[n][UP]]==player)return 1;

 if(brett[n]==player)
 if (connections[n][DOWN]!=-1)if (brett[connections[n][DOWN]]==player)
 if ( connections[connections[n][DOWN]][DOWN]!=-1)
 if(brett[connections[connections[n][DOWN]][DOWN]]==player)return 1;
 
 if(brett[n]==player)
 if (connections[n][UP]!=-1)if (brett[connections[n][UP]]==player)
 if ( connections[connections[n][UP]][UP]!=-1)
 if(brett[connections[connections[n][UP]][UP]]==player)return 1;

 if(brett[n]==player)
 if (connections[n][LEFT]!=-1)if (brett[connections[n][LEFT]]==player)
 if ( connections[connections[n][LEFT]][LEFT]!=-1)
 if(brett[connections[connections[n][LEFT]][LEFT]]==player)return 1;
 
  if(brett[n]==player)
 if (connections[n][RIGHT]!=-1)if (brett[connections[n][RIGHT]]==player)
 if ( connections[connections[n][RIGHT]][RIGHT]!=-1)
 if(brett[connections[connections[n][RIGHT]][RIGHT]]==player)return 1;

return 0;
}

int vismill(int player, int n)
{
	
	if(vbrett[n]==player)
 if (connections[n][RIGHT]!=-1)if (vbrett[connections[n][RIGHT]]==player)
 if(connections[n][LEFT]!=-1)if(vbrett[connections[n][LEFT]]==player)return 1;

 if(vbrett[n]==player)
 if (connections[n][DOWN]!=-1)if (vbrett[connections[n][DOWN]]==player)
 if(connections[n][UP]!=-1)if(vbrett[connections[n][UP]]==player)return 1;

 if(vbrett[n]==player)
 if (connections[n][DOWN]!=-1)if (vbrett[connections[n][DOWN]]==player)
 if ( connections[connections[n][DOWN]][DOWN]!=-1)
 if(vbrett[connections[connections[n][DOWN]][DOWN]]==player)return 1;
 
 if(vbrett[n]==player)
 if (connections[n][UP]!=-1)if (vbrett[connections[n][UP]]==player)
 if ( connections[connections[n][UP]][UP]!=-1)
 if(vbrett[connections[connections[n][UP]][UP]]==player)return 1;

 if(vbrett[n]==player)
 if (connections[n][LEFT]!=-1)if (vbrett[connections[n][LEFT]]==player)
 if ( connections[connections[n][LEFT]][LEFT]!=-1)
 if(vbrett[connections[connections[n][LEFT]][LEFT]]==player)return 1;
 
  if(vbrett[n]==player)
 if (connections[n][RIGHT]!=-1)if (vbrett[connections[n][RIGHT]]==player)
 if ( connections[connections[n][RIGHT]][RIGHT]!=-1)
 if(vbrett[connections[connections[n][RIGHT]][RIGHT]]==player)return 1;

return 0;
}


int nomovepossible(int player)
{
 int n=0;
	if ( (player==1&&steineA>0)||
	     (player==2&&steineB>0)) return 0;
	 if ( (player==1&&takenA==6)||
	      (player==2)&&takenB==6)return 0;
	while ( n<24)
 {
 	if ( brett[n]==player)
 	{
     if(connections[n][LEFT]!=-1) if ( brett[connections[n][LEFT]]==0)return 0;
     if(connections[n][RIGHT]!=-1) if ( brett[connections[n][RIGHT]]==0)return 0;
     if(connections[n][UP]!=-1) if ( brett[connections[n][UP]]==0)return 0;
     if(connections[n][DOWN]!=-1) if ( brett[connections[n][DOWN]]==0)return 0;
   }
  n++;
 }
 return 1;
	
}


int vnomovepossible(int player)
{
 int n=0;
	if ( (player==1&&vsteineA>0)||
	     (player==2&&vsteineB>0)) return 0;
	 if ( (player==1&&vtakenA==6)||
	      (player==2)&&vtakenB==6)return 0;
	while ( n<24)
 {
 	if ( vbrett[n]==player)
 	{
     if(connections[n][LEFT]!=-1) if ( vbrett[connections[n][LEFT]]==0)return 0;
     if(connections[n][RIGHT]!=-1) if ( vbrett[connections[n][RIGHT]]==0)return 0;
     if(connections[n][UP]!=-1) if ( vbrett[connections[n][UP]]==0)return 0;
     if(connections[n][DOWN]!=-1) if ( vbrett[connections[n][DOWN]]==0)return 0;
   }
  n++;
 }
 return 1;
	
}



int countstringup(unsigned char *string)
{
 int n=0; 
  if ( string[n]<'x') string[n]++;
  else
  {
    while ( string[n]=='x'){ n++;if ( n==3)return 1;}
  string[n]++;
   while ( n > 0 ) n--,string[n]='a';
  } 
  return 0;
}


int minimax(int player, unsigned char *input, int rec_depth)
{
  int vbrett_bak[24];
  int vsteineAbak,vsteineBbak;
  int vtakenAbak,vtakenBbak;
   unsigned char callstr[4];
  int n;
   n=0;
   signed int retcode,retcode_add;
   int countend=25;
   int count;

if ( rec_depth>2) return 0;

input[0]='a',input[1]='a',input[2]='a';
    if ( player==1)
    {
     callstr[0]='a',callstr[1]='a',callstr[2]='a',callstr[3]='a';
     retcode=1000000;
     count=0;
     if(rec_depth==0)move_error=1;
	 do
	 {
	 	n=0;
	 	while ( n < 24)vbrett_bak[n]=vbrett[n],n++;
	 	vsteineAbak=vsteineA,vsteineBbak=vsteineB;
		 vtakenAbak=vtakenA,vtakenBbak=vtakenB;
	 retcode_add=vmove(1,input);
	 if ( inputpos==1&&countend<25)countend=25;
	 if (inputpos==2&&countend<25*25)countend=25*25;
	 if(inputpos==3&&countend<25*25*25)countend=25*25*25;
       if ( retcode_add!=1)
       {
	 if ( retcode_add==3)retcode_add=-10;
	 else if ( retcode_add==4)retcode_add=-100;
	 else if ( retcode_add==2)retcode_add=100;
	 else if ( retcode_add==0) retcode_add=0;
	 if ( retcode_add!=-100&&retcode_add!=100)retcode_add+=minimax(2,callstr,rec_depth+1);
	  if ( rec_depth==0)move_error=0;	 
      if ( retcode_add<retcode){ retcode=retcode_add;
	   if ( rec_depth==0)outstr[0]=input[0],outstr[1]=input[1],outstr[2]=input[2];
       }
	  }
       n=0;
	   while ( n<24)vbrett[n]=vbrett_bak[n],n++;
	   vsteineB=vsteineBbak,vsteineA=vsteineAbak;
	   vtakenA=vtakenAbak,vtakenB=vtakenBbak;
     }while (countstringup(input)==0&&count++<countend);
     if ( rec_depth==0)countend_glob=countend;
	  return retcode;
	}
    else
    if ( player==2)
    {
     retcode=-100000;
     callstr[0]='a',callstr[1]='a',callstr[2]='a',callstr[3]='a';
     
     count=0;
     if ( rec_depth==0)move_error=1;
     do
	 {
	 	n=0;
	 	while ( n < 24)vbrett_bak[n]=vbrett[n],n++;
	 	vsteineAbak=vsteineA,vsteineBbak=vsteineB;
		 vtakenAbak=vtakenA,vtakenBbak=vtakenB;
	 retcode_add=vmove(2,input);
       if ( inputpos==1&&countend<25)countend=25;
	  if (inputpos==2&&countend<25*25)countend=25*25;
	  if(inputpos==3&&countend<25*25*25)countend=25*25*25;
	   if ( retcode_add!=1)
       {
	 if ( retcode_add==3)retcode_add=10;
	 else if ( retcode_add==4)retcode_add=100;
	  else if ( retcode_add==2)retcode_add=-100;
	  else if ( retcode_add==0) retcode_add=0;
	   if ( rec_depth==0)move_error=0;
	if(retcode_add!=100&&retcode_add!=-100)retcode_add+=minimax(1,callstr,rec_depth+1);
   if ( retcode_add>retcode){retcode=retcode_add;
   if ( rec_depth==0)outstr[0]=input[0],outstr[1]=input[1],outstr[2]=input[2];
     }
	  }
	  n=0;
	   while ( n<24)vbrett[n]=vbrett_bak[n],n++;
	   vsteineB=vsteineBbak,vsteineA=vsteineAbak;
	   vtakenA=vtakenAbak,vtakenB=vtakenBbak;
      }while (countstringup(input)==0&&count++<countend);
      if ( rec_depth==0)countend_glob=countend;
	  return retcode;


    }


}


int vmove(int player, unsigned char *input)
{
 inputpos=0;

int n=0;
	 	int pos;
	 	int checkmill;
	if ( player==1)
	{
         if ( vnomovepossible(1) ) return 2 ;
		//printf("Weiss ist am Zug...");
	 if (vsteineA>0)
	 {
	  
	 // printf("Position eingeben: ");
	  
                pos=input[inputpos];inputpos++;
           
               if ( pos<'a'||pos>'x') return 1;
	  
            if ( vbrett[pos-0x61]!=0 ) return 1;	
 	  
	  vbrett[pos-0x61]=1;
	  checkmill=pos-0x61;
	 	vsteineA--;
	 }
	else if ( vtakenA<6)
	{
	  	int moveok;
	  	moveok=0;
	 
       //   printf("Position des Steins angeben: ");
	   pos=input[inputpos];inputpos++;
             if ( pos<'a'||pos>'x' ) return 1;
	    pos-=0x61;
	    if (vbrett[pos]!=1) return 1;
	    if (connections[pos][RIGHT]!=-1)
	    if ( vbrett[connections[pos][RIGHT]]==0)moveok=1;
	    if (connections[pos][LEFT]!=-1)
	    if ( vbrett[connections[pos][LEFT]]==0)moveok=1;
	    if (connections[pos][DOWN]!=-1)
	    if ( vbrett[connections[pos][DOWN]]==0)moveok=1;
	      if (connections[pos][UP]!=-1)
	    if ( vbrett[connections[pos][UP]]==0)moveok=1;
	  
              if ( moveok==0) return 1;
	 	
		moveok=0;
		unsigned char direc;
     //printf("h,k,u,j für Richtung der Bewegung...");
	  direc=input[inputpos];inputpos++;
          if ( direc!='h'&&direc!='k'&&direc!='u'&&direc!='j')return 1;
	  
	  if ( direc=='h')if ( connections[pos][LEFT]!=-1)if (vbrett[connections[pos][LEFT]]==0)
	   vbrett[connections[pos][LEFT]]=1,vbrett[pos]=0,checkmill=connections[pos][LEFT],moveok=1;	
	 
	  if ( direc=='k')if ( connections[pos][RIGHT]!=-1)if (vbrett[connections[pos][RIGHT]]==0)
	   vbrett[connections[pos][RIGHT]]=1,vbrett[pos]=0,checkmill=connections[pos][RIGHT],moveok=1;	
	 
	  if ( direc=='u')if ( connections[pos][UP]!=-1)if (vbrett[connections[pos][UP]]==0)
	   vbrett[connections[pos][UP]]=1,vbrett[pos]=0,checkmill=connections[pos][UP],moveok=1;	
	  
	if ( direc=='j')if ( connections[pos][DOWN]!=-1)if (vbrett[connections[pos][DOWN]]==0)
	   vbrett[connections[pos][DOWN]]=1,vbrett[pos]=0,checkmill=connections[pos][DOWN],moveok=1;	
	 
         if ( moveok==0 ) return 1; 
	}
	else
	{
	
	 // printf("Quellposition eingeben: ");
	   pos=input[inputpos];inputpos++;
            if ( pos<'a'||pos> 'x')return 1;
	  
	  if (vbrett[pos-0x61]!=1) return 1;
	  
	  vbrett[pos-0x61]=0;
	  
	 // printf("Zielposition eingeben: ");
	   pos=input[inputpos];inputpos++; 
          if ( pos< 'a'||pos>'x') return 1;
	  
	  if (vbrett[pos-0x61]!=0) return 1;
	  
	  vbrett[pos-0x61]=1;
	  checkmill=pos-0x61;
	
		
	}
n=checkmill;
checkmill=vismill(1,n);
	
  if ( checkmill==1)
  {
	 // printf("Welchen Stein wegnehmen: ");
	   pos=input[inputpos];inputpos++;
       	         if ( pos<'a'||pos>'x') return 1;
	            if ( vismill(2,pos-0x61)&&vonlymill(2)!=1)return 1;
           if ( vbrett[pos-0x61]!=2 ) return 1;
	  	
	  vbrett[pos-0x61]=0;
	  vtakenB++;
	  
	  if ( vtakenB>6)
	  {
	  // printf("gewonnen");
	   return 4;
	  }
  	return 3;
  }
return 0;	


}
else  if (player==2)
 {
 if ( vnomovepossible(2)) return 2;	
	//printf("Schwarz ist am Zug...");
	 if (vsteineB>0)
	 {
	  
	 //printf("Position eingeben: ");
	         pos=input[inputpos];inputpos++;
           
               if ( pos<'a'||pos>'x') return 1;
	  
            if ( vbrett[pos-0x61]!=0 ) return 1;	
 	 
	  vbrett[pos-0x61]=2;
	  checkmill=pos-0x61;
	 	vsteineB--;
	 }
	else if ( vtakenB<6)
	{
	  	int moveok;
	  	moveok=0;
	 //printf("Position des Steins angeben: ");
	   pos=input[inputpos];inputpos++;
             if ( pos<'a'||pos>'x') return 1;
	    pos-=0x61;
	    if (vbrett[pos]!=2)return 1;
	    if (connections[pos][RIGHT]!=-1)
	    if ( vbrett[connections[pos][RIGHT]]==0)moveok=1;
	    if (connections[pos][LEFT]!=-1)
	    if ( vbrett[connections[pos][LEFT]]==0)moveok=1;
	    if (connections[pos][DOWN]!=-1)
	    if ( vbrett[connections[pos][DOWN]]==0)moveok=1;
	      if (connections[pos][UP]!=-1)
	    if ( vbrett[connections[pos][UP]]==0)moveok=1;
	  
	  if ( moveok==0) return 1;
		
		moveok=0;
		unsigned char direc;
     //printf("h,k,u,j für Richtung der Bewegung...");
	  direc=input[inputpos];inputpos++;
        if( direc!='h'&&direc!='k'&&direc!='u'&&direc!='j') return 1;
	  
	  if ( direc=='h')if ( connections[pos][LEFT]!=-1)if (vbrett[connections[pos][LEFT]]==0)
	   vbrett[connections[pos][LEFT]]=2,vbrett[pos]=0,checkmill=connections[pos][LEFT],moveok=1;	
	 
	  if ( direc=='k')if ( connections[pos][RIGHT]!=-1)if (vbrett[connections[pos][RIGHT]]==0)
	   vbrett[connections[pos][RIGHT]]=2,vbrett[pos]=0,checkmill=connections[pos][RIGHT],moveok=1;	
	 
	  if ( direc=='u')if ( connections[pos][UP]!=-1)if (vbrett[connections[pos][UP]]==0)
	   vbrett[connections[pos][UP]]=2,vbrett[pos]=0,checkmill=connections[pos][UP],moveok=1;	
	  
	if ( direc=='j')if ( connections[pos][DOWN]!=-1)if (vbrett[connections[pos][DOWN]]==0)
	   vbrett[connections[pos][DOWN]]=2,vbrett[pos]=0,checkmill=connections[pos][DOWN],moveok=1;	
	 
     if ( moveok==0 ) return 1;
	 
	}
	else
	{
	
	  //printf("Quellposition eingeben: ");
	   pos=input[inputpos];inputpos++;
            if ( pos<'a'||pos> 'x') return 1;
	  
	   if (vbrett[pos-0x61]!=2) return 1;
	  
	  vbrett[pos-0x61]=0;
	  
	//printf("Zielposition eingeben: ");
	    pos=input[inputpos];inputpos++;
            if ( pos< 'a'||pos>'x') return 1;
	  if ( vbrett[pos-0x61]!=0) return 1;
	  
	  vbrett[pos-0x61]=2;
	  checkmill=pos-0x61;
	
		
	}
n=checkmill;
checkmill=vismill(2,n);
	
  if ( checkmill==1)
  {
  	 // printf("Welchen Stein wegnehmen: ");
	    pos=input[inputpos];inputpos++;
            if ( pos<'a'||pos>'x') return 1;
	   if(vismill(1,pos-0x61)&&vonlymill(1)!=1)return 1;
	  if (vbrett[pos-0x61]!=1) return 1;
	  
	  vbrett[pos-0x61]=0;
	  vtakenA++;
	  
	  if ( vtakenA>6)
	  {
	 // printf("gewonnen");
  	    return 4;
	    }
  	return 3;
  }
  return 0;
	
 }
}








int main(void)
{
	int blondmeid;
	printf("1 eingeben fuer blinden Modus");
	scanf("%d",&blondmeid);
 connections[0][RIGHT]=1,
 connections[0][DOWN]=9;
 connections[0][LEFT]=-1;
 connections[0][UP]=-1;
 connections[1][LEFT]=0;
 connections[1][RIGHT]=2;
 connections[1][UP]=-1;
 connections[1][DOWN]=4;
 connections[2][LEFT]=1;
 connections[2][RIGHT]=-1;
 connections[2][UP]=-1;
 connections[2][DOWN]=14;
 connections[3][RIGHT]=4;
 connections[3][DOWN]=10;
 connections[3][UP]=-1;
 connections[3][LEFT]=-1;
 connections[4][LEFT]=3;
 connections[4][UP]=1;
 connections[4][DOWN]=7;
 connections[4][RIGHT]=5;
 connections[5][LEFT]=4;
 connections[5][UP]=-1;
 connections[5][RIGHT]=-1;
 connections[5][DOWN]=13;
 connections[6][DOWN]=11;
 connections[6][RIGHT]=7;
 connections[6][LEFT]=-1;
 connections[6][UP]=-1;
 connections[7][LEFT]=6;
 connections[7][UP]=4;
 connections[7][RIGHT]=8;
 connections[7][DOWN]=-1;
 connections[8][LEFT]=7;
 connections[8][RIGHT]=-1;
 connections[8][DOWN]=12;
 connections[8][UP]=-1;
 connections[9][UP]=0;
 connections[9][RIGHT]=10;
 connections[9][DOWN]=21;
 connections[9][LEFT]=-1;
 connections[10][LEFT]=9;
 connections[10][RIGHT]=11;
 connections[10][UP]=3;
 connections[10][DOWN]=18;
 connections[11][LEFT]=10;
 connections[11][UP]=6;
 connections[11][RIGHT]=-1;
 connections[11][DOWN]=15;
 connections[12][UP]=8;
 connections[12][DOWN]=17;
 connections[12][LEFT]=-1;
 connections[12][RIGHT]=13;
 connections[13][LEFT]=12;
 connections[13][UP]=5;
 connections[13][RIGHT]=14;
 connections[13][DOWN]=20;
 connections[14][DOWN]=23;
 connections[14][UP]=2;
 connections[14][LEFT]=13;
 connections[14][RIGHT]=-1;
 connections[15][RIGHT]=16;
 connections[15][UP]=11;
 connections[15][DOWN]=-1;
 connections[15][LEFT]=-1;
 connections[16][LEFT]=15;
 connections[16][DOWN]=19;
 connections[16][UP]=-1;
 connections[16][RIGHT]=17;
 connections[17][LEFT]=16;
 connections[17][UP]=12;
 connections[17][DOWN]=-1;
 connections[17][RIGHT]=-1;
 connections[18][UP]=10;
 connections[18][RIGHT]=19;
 connections[18][LEFT]=-1;
 connections[18][DOWN]=-1;
 connections[19][UP]=16;
 connections[19][LEFT]=18;
 connections[19][RIGHT]=20;
 connections[19][DOWN]=22;
 connections[20][UP]=13;
 connections[20][RIGHT]=-1;
 connections[20][LEFT]=19;
 connections[20][DOWN]=-1;
 connections[21][LEFT]=-1;
 connections[21][UP]=9;
 connections[21][DOWN]=-1;
 connections[21][RIGHT]=22;
 connections[22][LEFT]=21;
 connections[22][UP]=19;
 connections[22][DOWN]=-1;
 connections[22][RIGHT]=23;
 connections[23][LEFT]=22;
 connections[23][RIGHT]=-1;
 connections[23][UP]=14;
 connections[23][DOWN]=-1;
steineA=9,steineB=9;
takenA=0,takenB=0;

int player=0;

int x,y;
int n=0;
while ( n < 24)brett[n]=0,n++;

int screen[120][90];
n=0;
unsigned char input[5];
int bewertung_bak;

while(1)
{
	 	int pos;
	 	int checkmill;
	if ( player==0)
	{
		if (nomovepossible(1))break;
		n=0;
		while ( n < 24)vbrett[n]=brett[n],n++;
		vsteineA=steineA,vsteineB=steineB;
		vtakenA=takenA,vtakenB=takenB;
		outstr[3]='\0';
		bewertung_bak=minimax(1,input,0);
		
		printf("Empfohlene Eingabe: %s mit %d",outstr,bewertung_bak);
		printf("Weiss ist am Zug...");
	 if (steineA>0)
	 {
	  
	  do
	  {
	  printf("Position eingeben: ");
	   do{pos=getch();pos=tolower(pos);}while ( pos<'a'||pos>'x');
	  
	  }while (brett[pos-0x61]!=0);
	  
	  brett[pos-0x61]=1;
	  checkmill=pos-0x61;
	 	steineA--;
	 }
	else if ( takenA<6)
	{
	  	int moveok;
	  do
	  {
	  	moveok=0;
	   do
	  {
	do
	{
	 printf("Position des Steins angeben: ");
	   do{pos=getch();pos=tolower(pos);}while ( pos<'a'||pos>'x');
	    pos-=0x61;
	   }while ( brett[pos]!=1);
		if (connections[pos][RIGHT]!=-1)
	    if ( brett[connections[pos][RIGHT]]==0)moveok=1;
	    if (connections[pos][LEFT]!=-1)
	    if ( brett[connections[pos][LEFT]]==0)moveok=1;
	    if (connections[pos][DOWN]!=-1)
	    if ( brett[connections[pos][DOWN]]==0)moveok=1;
	      if (connections[pos][UP]!=-1)
	    if ( brett[connections[pos][UP]]==0)moveok=1;
	  
	  }while (moveok==0);
		
		moveok=0;
		unsigned char direc;
     printf("h,k,u,j für Richtung der Bewegung...");
	  do{direc=getch();}while ( direc!='h'&&direc!='k'&&direc!='u'&&direc!='j');
	  
	  if ( direc=='h')if ( connections[pos][LEFT]!=-1)if (brett[connections[pos][LEFT]]==0)
	   brett[connections[pos][LEFT]]=1,brett[pos]=0,checkmill=connections[pos][LEFT],moveok=1;	
	 
	  if ( direc=='k')if ( connections[pos][RIGHT]!=-1)if (brett[connections[pos][RIGHT]]==0)
	   brett[connections[pos][RIGHT]]=1,brett[pos]=0,checkmill=connections[pos][RIGHT],moveok=1;	
	 
	  if ( direc=='u')if ( connections[pos][UP]!=-1)if (brett[connections[pos][UP]]==0)
	   brett[connections[pos][UP]]=1,brett[pos]=0,checkmill=connections[pos][UP],moveok=1;	
	  
	if ( direc=='j')if ( connections[pos][DOWN]!=-1)if (brett[connections[pos][DOWN]]==0)
	   brett[connections[pos][DOWN]]=1,brett[pos]=0,checkmill=connections[pos][DOWN],moveok=1;	
	 
     }while ( moveok==0);
	 
	}
	else
	{
	
	  do
	  {
	  printf("Quellposition eingeben: ");
	   do{pos=getch();pos=tolower(pos);}while ( pos<'a'||pos> 'x');
	  
	  }while (brett[pos-0x61]!=1);
	  
	  brett[pos-0x61]=0;
	  
	    do
	  {
	  printf("Zielposition eingeben: ");
	   do{pos=getch();pos=tolower(pos);}while ( pos< 'a'||pos>'x');
	  
	  }while (brett[pos-0x61]!=0);
	  
	  brett[pos-0x61]=1;
	  checkmill=pos-0x61;
	
		
	}

	printf("%d ",checkmill);

n=checkmill;
checkmill=ismill(1,n);
	
  if ( checkmill==1)
  {
  	  do
	  {
	  printf("Welchen Stein wegnehmen: ");
	   do{pos=getch();pos=tolower(pos);}while ( pos<'a'||pos>'x');
	  
	  }while (brett[pos-0x61]!=2||(ismill(2,pos-0x61)==1&&onlymill(2)!=1));
	  
	  brett[pos-0x61]=0;
	  takenB++;
	  
	  if ( takenB>6)
	  {
	   printf("gewonnen");
       break;
	  }
  	
  }
	


}
else  if (player==1)
 {	
 if(nomovepossible(2))break;
        n=0;
		while ( n < 24)vbrett[n]=brett[n],n++;
		vsteineA=steineA,vsteineB=steineB;
		vtakenA=takenA,vtakenB=takenB;
		 outstr[3]='\0';
    	bewertung_bak=minimax(2,input,0);
	
		printf("Empfohlene Eingabe: %s ",outstr);
	printf("Schwarz ist am Zug...");
	 if (steineB>0)
	 {
	  
	  do
	  {
	  printf("Position eingeben: ");
	   do{pos=getch();pos=tolower(pos);}while ( pos<'a'||pos>'x');
	  
	  }while (brett[pos-0x61]!=0);
	  
	  brett[pos-0x61]=2;
	  checkmill=pos-0x61;
	 	steineB--;
	 }
	else if ( takenB<6)
	{
	  	int moveok;
	  do
	  {
	  	moveok=0;
	   do
	  {
	do{
	 printf("Position des Steins angeben: ");
	   do{pos=getch();pos=tolower(pos);}while ( pos<'a'||pos>'x');
	    pos-=0x61;
     }while ( brett[pos]!=2);
	    if (connections[pos][RIGHT]!=-1)
	    if ( brett[connections[pos][RIGHT]]==0)moveok=1;
	    if (connections[pos][LEFT]!=-1)
	    if ( brett[connections[pos][LEFT]]==0)moveok=1;
	    if (connections[pos][DOWN]!=-1)
	    if ( brett[connections[pos][DOWN]]==0)moveok=1;
	      if (connections[pos][UP]!=-1)
	    if ( brett[connections[pos][UP]]==0)moveok=1;
	  
	  }while (moveok==0);
		
		moveok=0;
		unsigned char direc;
     printf("h,k,u,j für Richtung der Bewegung...");
	  do{direc=getch();}while ( direc!='h'&&direc!='k'&&direc!='u'&&direc!='j');
	  
	  if ( direc=='h')if ( connections[pos][LEFT]!=-1)if (brett[connections[pos][LEFT]]==0)
	   brett[connections[pos][LEFT]]=2,brett[pos]=0,checkmill=connections[pos][LEFT],moveok=1;	
	 
	  if ( direc=='k')if ( connections[pos][RIGHT]!=-1)if (brett[connections[pos][RIGHT]]==0)
	   brett[connections[pos][RIGHT]]=2,brett[pos]=0,checkmill=connections[pos][RIGHT],moveok=1;	
	 
	  if ( direc=='u')if ( connections[pos][UP]!=-1)if (brett[connections[pos][UP]]==0)
	   brett[connections[pos][UP]]=2,brett[pos]=0,checkmill=connections[pos][UP],moveok=1;	
	  
	if ( direc=='j')if ( connections[pos][DOWN]!=-1)if (brett[connections[pos][DOWN]]==0)
	   brett[connections[pos][DOWN]]=2,brett[pos]=0,checkmill=connections[pos][DOWN],moveok=1;	
	 
     }while ( moveok==0);
	 
	}
	else
	{
	
	  do
	  {
	  printf("Quellposition eingeben: ");
	   do{pos=getch();pos=tolower(pos);}while ( pos<'a'||pos> 'x');
	  
	  }while (brett[pos-0x61]!=2);
	  
	  brett[pos-0x61]=0;
	  
	    do
	  {
	  printf("Zielposition eingeben: ");
	   do{pos=getch();pos=tolower(pos);}while ( pos< 'a'||pos>'x');
	  
	  }while (brett[pos-0x61]!=0);
	  
	  brett[pos-0x61]=2;
	  checkmill=pos-0x61;
	
		
	}
	
	printf("%d ",checkmill);
n=checkmill;
checkmill=ismill(2,n);
	
  if ( checkmill==1)
  {
  	  do
	  {
	  printf("Welchen Stein wegnehmen: ");
	   do{pos=getch();pos=tolower(pos);}while ( pos<'a'||pos>'x');
	  
	  }while (brett[pos-0x61]!=1||(ismill(1,pos-0x61)==1&&onlymill(1)!=1));
	  
	  brett[pos-0x61]=0;
	  takenA++;
	  
	  if ( takenA>6)
	  {
	   printf("gewonnen");
       break;
	  }
  	
  }
	
	
 }
	
	
 y=0;
 while ( y<90)
 {
  x=0;
  int steinbuf;
  steinbuf=0;
  while ( x < 120)
  {
   screen[x][y]=' ';
  	if ( steinbuf>=1)
  	{
  	  if ( steinbuf==1)screen[x][y]=219;
  	  else screen[x][y]=177;
  		steinbuf=0;
	  }
    if ( x==0&&y==0)screen[x][y]='A',steinbuf=(brett[0]);
    if ( x==30&&y==0)screen[x][y]='B',steinbuf=(brett[1] );
    if ( x==60&&y==0)screen[x][y]='C',steinbuf=(brett[2]);
    if ( x==10&&y==10)screen[x][y]='D',steinbuf=(brett[3] );
    if ( x==30&&y==10)screen[x][y]='E',steinbuf=(brett[4] );
    if ( x==50&&y==10)screen[x][y]='F',steinbuf=(brett[5] );
    if ( x==20&&y==20)screen[x][y]='G',steinbuf=(brett[6] );
    if ( x==30&&y==20)screen[x][y]='H',steinbuf=(brett[7] );
    if ( x==40&&y==20)screen[x][y]='I',steinbuf=(brett[8] );
    if ( x==0&&y==30)screen[x][y]='J',steinbuf=(brett[9] );
    if ( x==10&&y==30)screen[x][y]='K',steinbuf=(brett[10] );
    if ( x==20&&y==30)screen[x][y]='L',steinbuf=(brett[11] );
    if ( x==40&&y==30)screen[x][y]='M',steinbuf=(brett[12] );
    if ( x==50&&y==30)screen[x][y]='N',steinbuf=(brett[13] );
    if ( x==60&&y==30)screen[x][y]='O',steinbuf=(brett[14] );
    
    if ( x==20&&y==40)screen[x][y]='P',steinbuf=(brett[15] );
    if ( x==30&&y==40)screen[x][y]='Q',steinbuf=(brett[16] );
    if ( x==40&&y==40)screen[x][y]='R',steinbuf=(brett[17] );
    if ( x==10&&y==50)screen[x][y]='S',steinbuf=(brett[18] );
    if ( x==30&&y==50)screen[x][y]='T',steinbuf=(brett[19] );
    if ( x==50&&y==50)screen[x][y]='U',steinbuf=(brett[20] );
    if ( x==0&&y==60)screen[x][y]='V',steinbuf=(brett[21] );
    if ( x==30&&y==60)screen[x][y]='W',steinbuf=(brett[22] );
    if ( x==60&&y==60)screen[x][y]='X',steinbuf=(brett[23] );
    
	x++;
  }
  y++;
 }
#define HLINE(a,b,c) x= a ;y= c ;while ( x < b) screen[x][y]='.',x++;

#define VLINE(a,b,c) x=a ;y= b ; while(y<c) screen[x][y]='.',y++;

HLINE(2,28,0)
HLINE(32,58,0)
VLINE(30,2,8)
HLINE(12,28,10)
HLINE(32,48,10)
VLINE(0,2,28)
VLINE(60,2,28)
VLINE(10,12,28)
VLINE(50,12,28)
VLINE(30,12,18)
HLINE(22,28,20)
HLINE(32,38,20)
VLINE(20,22,28)
VLINE(40,22,28)

HLINE(2,8,30)
HLINE(12,18,30)
HLINE(42,48,30)
HLINE(52,58,30)

HLINE(2,28,(60-0))
HLINE(32,58,(60-0))
VLINE(30,60-8,(60-2))
HLINE(12,28,(60-10))
HLINE(32,48,(60-10))
VLINE(0,60-28,(60-2))
VLINE(60,60-28,(60-2))
VLINE(10,60-28,(60-12))
VLINE(50,60-28,(60-12))
VLINE(30,60-18,(60-12))
HLINE(22,28,(60-20))
HLINE(32,38,(60-20))
VLINE(20,60-28,(60-22))
VLINE(40,60-28,(60-22))


if (blondmeid==1)
{
 n=0;
 while ( n < 24)
 {
  if ( brett[n]!=0)
   printf("%c ist Spieler %d ",n+0x61,brett[n]);
   n++;
}
}
else
{
system("cls");
printf("Weiss hat %d Steine und Schwarz hat %d Steine\n",steineA,steineB);
y=0;
 while ( y<78)
 {
  x=0;
  while ( x < 120)
  {
     printf("%c",screen[x][y]);
	x++;
  }
  printf("\n");
  y++;
 }
}

 player++;
 player%=2;
	
 }

}
