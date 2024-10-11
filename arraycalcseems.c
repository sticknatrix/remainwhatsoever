/*
[ S A1 : A3 ] ;
[ S A1 : A3 ] ;
A0
$ SUM ;
A0
45 + 39 ;
A1 ;
14 + 18 ;
A2
26 + 30 ;
A3
$ AVG ;
B0
( A1 + A2 + A3 ) / 3 ;
B0

type that for instance, redirecting strangely did not work, but exactly it could exchange saving?!
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int var_name_count=0;

void variable_countup(char *name, int do_countup)
{

 char appendix[255];
 char *base="rval";
 if ( do_countup == 1 ) ++var_name_count;
   sprintf(appendix,"%d",var_name_count);
   
 strcpy(name,base);
 strcat( name, appendix );
 return ;

}

int isnum(char c)
{
 if ( c>=0x30 && c <=0x39 ) return 1;
 return 0;

}


struct
{
 int type;
 char text[255];

}token[255];

static int num_tokens;

void remove_tokens( int position, int amount_removed )
{

 while( position+ amount_removed <= num_tokens )
 {
  token[position].type=token[position+amount_removed].type;
  strcpy( token[position].text, token[position+amount_removed].text );

  position++;
 }

 num_tokens-=amount_removed;

}

void insert_token( int position, unsigned char *tokencp, unsigned char type )
{

signed int amount_added=num_tokens-position;
 while( amount_added >= 0 )
 {
  token[position+amount_added+1].type=token[position+amount_added].type;
  strcpy( token[position+amount_added+1].text, token[position+amount_added].text );

  amount_added--;
 }
 token[position].type=type;
 strcpy(token[position].text,tokencp);
 
 num_tokens+=1;

}



int main(void)
{
 int position;
 int txtcount;
 int left_limit, right_limit;
 int reduction_type;
 char rvalnum[20];

int line,column;

signed int result[26][10];
signed int result_is[26][10];
unsigned char cellstr[26][10][50];
unsigned char cellname[26][10][50];

unsigned char backupformula[50];

line=0;
while ( line<26)
{
 column=0;
 while ( column<10)
 {
  result_is[line][column]=0;
  //cellstr[line][column]=malloc(sizeof(unsigned char)*50);
  cellstr[line][column][0]='0'; cellstr[line][column][1]=' ';
  cellstr[line][column][2]=';',cellstr[line][column][3]='\0';
  result[line][column]=0;
  cellname[line][column][0]='\0';
  column++;
 }
 line++;
}

inputloop:

line=0;
while ( line<26)
{
 column=0;
 while ( column<10)
 {
  result_is[line][column]=0;
  column++;
 }
 line++;
}


unsigned char stringbuffer[255];
 
 fgets(stringbuffer,48,stdin);
 fgets(stringbuffer,48,stdin);
  //gets(stringbuffer);
 //printf("binda");
 printf("Zellennummer eingeben A-Z 0-9\n");
scanf("%c",&line),line-=0x41;
scanf("%c",&column),column-=0x30;
int result_is_bak;
strcpy(backupformula, &cellstr[line][column][0]);
strcpy(&cellstr[line][column][0],stringbuffer);
 result_is_bak=result_is[line][column];
 result_is[line][column]=0;


 int loopfurther;
 
 loopfurther=0;

int cellresults_max;
cellresults_max=0;

do
{
line=0;
while ( line<26)
{

 column=0;
 while ( column<10)
 {
  position=0;
//printf("gehe in Lese...\n");
//printf("%s\n", &cellstr[line][column][0]);
  int nctr,nctr2;
  nctr=0;
  do
 {
  txtcount=0;
  nctr2=0;
  while(cellstr[line][column][nctr]==' ')nctr++;
  while(cellstr[line][column][nctr]!=' '&&cellstr[line][column][nctr]!=0xa&&cellstr[line][column][nctr]!=0xd)
 token[position].text[nctr2]=cellstr[line][column][nctr],nctr++,nctr2++;
 token[position].text[nctr2]='\0';
 //printf(">>%s<<",token[position].text);

   if(token[position].text[0]=='$'
      && strlen(token[position].text )== 1 ) token[position].type='$',
      txtcount++;
   else
  if(token[position].text[0]=='+' 
     && strlen(token[position].text )== 1 ) token[position].type='+',
     txtcount++;
  else
   if(token[position].text[0]=='-'
      && strlen(token[position].text )== 1 ) token[position].type='-',
      txtcount++;
  else
   if(token[position].text[0]=='*'
      && strlen(token[position].text )== 1 ) token[position].type='*',
      txtcount++;
  else
   if(token[position].text[0]=='/'
       && strlen(token[position].text )== 1 ) token[position].type='/',
       txtcount++;
  else
   if(token[position].text[0]==':'
      && strlen(token[position].text )== 1 ) token[position].type=':',
      txtcount++;
  else
   if(token[position].text[0]==']'
      && strlen(token[position].text )== 1 ) token[position].type=']',
      txtcount++;
  else
   if(token[position].text[0]=='['
      && strlen(token[position].text )== 1 ) token[position].type='[',
      txtcount++;
  else
   if(token[position].text[0]=='('
       && strlen(token[position].text )== 1 ) token[position].type='(',
       txtcount++;
  else
   if(token[position].text[0]==')'
      && strlen(token[position].text )== 1 ) token[position].type=')',
      txtcount++;       
   else
    if(token[position].text[0]==';'
        && strlen(token[position].text )== 1 ) token[position].type=';',
        txtcount++;
  else
    if ( isalpha( token[position].text[0] ) )
    {
     txtcount=0;
     while( isalpha(token[position].text[txtcount] ) )txtcount++;
     while( isnum(token[position].text[txtcount] ) ) txtcount++;
     token[position].type='v'; /* Variable */
     if ( token[position].text[txtcount] != '\0' ) return 1;
    }
  else
    if ( isnum( token[position].text[0] ) )
  {
    txtcount=0;
    while( isnum( token[position].text[txtcount] ) ) txtcount++;
    token[position].type='n'; /* numerische Konstante */
    if ( token[position].text[txtcount] != '\0' ) return 1;
  
  }
   else { printf("Lexikalischer Fehler.\n"); return 1;}

  position++;
 }while(token[position-1].type!= ';' );
  num_tokens=position-1;

//printf("habe gelesen %d %d",line,column);

reduction_type=' ';
if ( result_is[line][column]==0 )
 while(reduction_type!=';' )
 {
   reduction_type=';';
  /* debug

    position=0;

    while(position < num_tokens+1 )
    {
     printf("Position: %d String: %s Typ: %c\n", position, token[position].text,
      token[position].type );
     position++;
    }
    getchar();
    
    debug Ende  */

unsigned char resultstring[255];
int operand1, operand2;

  left_limit=0, right_limit=num_tokens;
 
 position=left_limit;
  while( token[position].type!= ']'&& position < num_tokens )
  {
   position++;
  }
   if ( token[position].type== ']' )
   {
    right_limit=position;
    left_limit=position;

    while(token[left_limit].type != '[' && left_limit > 0 ) left_limit--;

    if(token[left_limit].type=='[' )
    {
     reduction_type='[';
    }
    else return 1;

   }
   
  position=left_limit;
  while( token[position].type!= ')'&& position < num_tokens )
  {
   position++;
  }
   if ( token[position].type== ')' )
   {
    right_limit=position;
    left_limit=position;

    while(token[left_limit].type != '(' && left_limit > 0 ) left_limit--;

    if(token[left_limit].type=='(' )
    {
     reduction_type='(';
    }
    else return 1;

   }

   position=left_limit;
  
  
   while( position+2 <= right_limit )
   {
    if ( token[position].type=='$' )
      if ( token[position+1].type=='v' )
         { reduction_type=token[position].type;
           right_limit=position+1, left_limit=position;
           break; }
     position++;
   }
  
   position=left_limit;
  
   while( position+2 <= right_limit )
   {
    if ( token[position].type=='v' )
      if ( token[position+1].type==':' )
        if ( token[position+2].type=='v' )
         { reduction_type=token[position+1].type;
           right_limit=position+2, left_limit=position;
           break; }
     position++;
   }
  
   position=left_limit;
   
   while( position+2 <= right_limit )
   {
    if ( token[position].type=='v' || token[position].type=='n'  || token[position].type=='r' )
      if ( token[position+1].type=='*' || token[position+1].type=='/' )
        if ( token[position+2].type=='v' || token[position+2].type=='n'|| token[position+2].type=='r' )
         { reduction_type=token[position+1].type;
           right_limit=position+2, left_limit=position;
           break; }
     position++;
   }
   position=left_limit;
   while( position+2 <= right_limit )
   {
    if ( token[position].type=='v' || token[position].type=='n' || token[position].type=='r' )
      if ( token[position+1].type=='+' || token[position+1].type=='-' )
        if ( token[position+2].type=='v' || token[position+2].type=='n' || token[position+2].type=='r')
         { reduction_type=token[position+1].type;
           right_limit=position+2, left_limit=position;
           break; }
     position++;
   }

unsigned char resultbufstr[255];

  if ( reduction_type=='['  ) /* Zellenmengensummenaktion */
   {
    int summe=0;
    summe=0;
    int nelem;
    int breakfullout;
    breakfullout=0;
  
   if ( token[left_limit+1].text[0]=='S'&& token[left_limit+1].text[1]=='\0'&&token[left_limit+1].type=='v')
   {
    printf("Bilde Summe...\n");
    breakfullout=0;
    nelem=2;
    while ( token[left_limit+nelem].type!=']' )
    {
    //printf("binda");
       if ( result_is[token[left_limit+nelem].text[0]-0x41][token[left_limit+nelem].text[1]-0x30]==0 ){ printf("Resultat nicht berechnet.\n");loopfurther=1;
       breakfullout=1;break;}else
       summe+=result[token[left_limit+nelem].text[0]-0x41][token[left_limit+nelem].text[1]-0x30];
       nelem++;
    }
   }
  if ( breakfullout==1) break;
  remove_tokens(left_limit,right_limit-left_limit );

    token[left_limit].type='n';
     sprintf(token[left_limit].text,"%d",summe);
    }  


   if ( reduction_type=='('  ) /* Klammern */
   {
     if ( right_limit-left_limit != 2 ) { printf("Fehlerhafter Klammerinhalt.\n");
            return 1; }
    if ( token[left_limit+1].type!='n' && token[left_limit+1].type!='v' &&
         token[left_limit+1].type !='r' ) { printf("Syntaxfehler bei Klammerung.\n");
          return 1; }

    variable_countup( rvalnum,1 );

    strcpy(resultbufstr, token[left_limit+1].text);
    remove_tokens(left_limit,2 );

    token[left_limit].type='n';
    strcpy( token[left_limit].text, resultbufstr );
   }
   else
   if ( reduction_type=='+'  ) /* Addition */
   {

 
    if( token[left_limit].type!= 'v' )
       operand1=atoi(token[left_limit].text);
    else
    {
      if ( token[left_limit].text[0]<0x41||token[left_limit].text[0]>'Z' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
      if ( token[left_limit].text[1]<0x30||token[left_limit].text[1]>'9' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}

       if ( result_is[token[left_limit].text[0]-0x41][token[left_limit].text[1]-0x30]==0 ){ printf("Resultat nicht berechnet.\n");loopfurther=1;
       break;}else
       operand1=result[token[left_limit].text[0]-0x41][token[left_limit].text[1]-0x30];
     
     }  
    if( token[left_limit+2].type!= 'v' )
       operand2=atoi(token[left_limit+2].text);
    else
    {
      if ( token[left_limit+2].text[0]<0x41||token[left_limit+2].text[0]>'Z' ) {printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
      if ( token[left_limit+2].text[1]<0x30||token[left_limit+2].text[1]>'9' ) {printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}

       if ( result_is[token[left_limit+2].text[0]-0x41][token[left_limit+2].text[1]-0x30]==0 ){ printf("Resultat nicht berechnet.\n");loopfurther=1; break;}else
     operand2=result[token[left_limit+2].text[0]-0x41][token[left_limit+2].text[1]-0x30];
 
     }
   
    remove_tokens(left_limit,2 );
    operand1+=operand2;

    token[left_limit].type='n';
     sprintf(token[left_limit].text,"%d",operand1);
  
   }
   else
   if ( reduction_type=='-'  ) /* Subtraktion */
   {

    if( token[left_limit].type!= 'v' )
       operand1=atoi(token[left_limit].text);
    else
    {
      if ( token[left_limit].text[0]<0x41||token[left_limit].text[0]>'Z' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
      if ( token[left_limit].text[1]<0x30||token[left_limit].text[1]>'9' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}

       if ( result_is[token[left_limit].text[0]-0x41][token[left_limit].text[1]-0x30]==0 ){ printf("Resultat nicht berechnet.\n");loopfurther=1;
       break;}else
       operand1=result[token[left_limit].text[0]-0x41][token[left_limit].text[1]-0x30];
     
     }  
    if( token[left_limit+2].type!= 'v' )
       operand2=atoi(token[left_limit+2].text);
    else
    {
      if ( token[left_limit+2].text[0]<0x41||token[left_limit+2].text[0]>'Z' ) {printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
      if ( token[left_limit+2].text[1]<0x30||token[left_limit+2].text[1]>'9' ) {printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}

       if ( result_is[token[left_limit+2].text[0]-0x41][token[left_limit+2].text[1]-0x30]==0 ){ printf("Resultat nicht berechnet.\n");loopfurther=1; break;}else
      operand2=result[token[left_limit+2].text[0]-0x41][token[left_limit+2].text[1]-0x30];
 
     }
   
    remove_tokens(left_limit,2 );
    operand1-=operand2;

    token[left_limit].type='n';
       sprintf(token[left_limit].text,"%d",operand1);
  
  
   }
   else
   if ( reduction_type=='*'  ) /* Multiplikation */
   {
    if( token[left_limit].type!= 'v' )
       operand1=atoi(token[left_limit].text);
    else
    {
      if ( token[left_limit].text[0]<0x41||token[left_limit].text[0]>'Z' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
      if ( token[left_limit].text[1]<0x30||token[left_limit].text[1]>'9' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}

       if ( result_is[token[left_limit].text[0]-0x41][token[left_limit].text[1]-0x30]==0 ){ printf("Resultat nicht berechnet.\n");loopfurther=1;
       break;}else
       operand1=result[token[left_limit].text[0]-0x41][token[left_limit].text[1]-0x30];
     
     }  
    if( token[left_limit+2].type!= 'v' )
       operand2=atoi(token[left_limit+2].text);
    else
    {
      if ( token[left_limit+2].text[0]<0x41||token[left_limit+2].text[0]>'Z' ) {printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
      if ( token[left_limit+2].text[1]<0x30||token[left_limit+2].text[1]>'9' ) {printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}

       if ( result_is[token[left_limit+2].text[0]-0x41][token[left_limit+2].text[1]-0x30]==0 ){ printf("Resultat nicht berechnet.\n");loopfurther=1; break;}else
      operand2=result[token[left_limit+2].text[0]-0x41][token[left_limit+2].text[1]-0x30];
 
     }
   
    remove_tokens(left_limit,2 );
    operand1*=operand2;

    token[left_limit].type='n';
  sprintf(token[left_limit].text,"%d",operand1);

   }
   else
   if ( reduction_type=='/'  ) /* Division */
   {
     //variable_countup( rvalnum,1 );

    if( token[left_limit].type!= 'v' )
       operand1=atoi(token[left_limit].text);
    else
    {
      if ( token[left_limit].text[0]<0x41||token[left_limit].text[0]>'Z' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
      if ( token[left_limit].text[1]<0x30||token[left_limit].text[1]>'9' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}

       if ( result_is[token[left_limit].text[0]-0x41][token[left_limit].text[1]-0x30]==0 ){ printf("Resultat nicht berechnet.\n");loopfurther=1;
       break;}else
       operand1=result[token[left_limit].text[0]-0x41][token[left_limit].text[1]-0x30];
     
     }  
    if( token[left_limit+2].type!= 'v' )
       operand2=atoi(token[left_limit+2].text);
    else
    {
      if ( token[left_limit+2].text[0]<0x41||token[left_limit+2].text[0]>'Z' ) {printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
      if ( token[left_limit+2].text[1]<0x30||token[left_limit+2].text[1]>'9' ) {printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}

       if ( result_is[token[left_limit+2].text[0]-0x41][token[left_limit+2].text[1]-0x30]==0 ){ printf("Resultat nicht berechnet.\n");loopfurther=1; break;}else
      operand2=result[token[left_limit+2].text[0]-0x41][token[left_limit+2].text[1]-0x30];
    }
    
    remove_tokens(left_limit,2 );
    operand1/=operand2;

    token[left_limit].type='n';
     sprintf(token[left_limit].text,"%d",operand1);
  
   }
   else
   if ( reduction_type=='$'  ) /* Zellenname Zuweisung */
   {
     //variable_countup( rvalnum,1 );

    remove_tokens(left_limit,1 );

    token[left_limit].type='$';
     strcpy(cellname[line][column],token[left_limit].text);
     strcpy(&cellstr[line][column][0],backupformula);
     result_is[line][column]=result_is_bak;
   }
   else
   if ( reduction_type==':'  ) /* Zellenname Zuweisung */
   {
     //variable_countup( rvalnum,1 );
      unsigned char fieldcname[3];
      int iterln, iterclmn;
      
      if ( token[left_limit].text[0]<0x41||token[left_limit].text[0]>'Z' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
      if ( token[left_limit].text[1]<0x30||token[left_limit].text[1]>'9' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
      if ( token[left_limit+2].text[0]<0x41||token[left_limit+2].text[0]>'Z' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
      if ( token[left_limit+2].text[1]<0x30||token[left_limit+2].text[1]>'9' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
       fieldcname[2]='\0';
 iterln=token[left_limit].text[0];
 if ( token[left_limit+2].text[0]< token[left_limit].text[0]|| token[left_limit+2].text[1]<token[left_limit].text[1]) 
 { printf("Feld hat keine positive Ausdehnung\n"); strcpy(&cellstr[line][column][0],backupformula);goto inputloop;}
      while ( iterln<=token[left_limit+2].text[0])
      {
       fieldcname[0]=iterln;
       iterclmn=token[left_limit].text[1];
       while (iterclmn <= token[left_limit+2].text[1] )
       {
       fieldcname[1]=iterclmn;
       insert_token(left_limit+3,fieldcname,'v');
       printf("Fuege %s ein",fieldcname);
        iterclmn++;
       }
       iterln++;
      }
      
    remove_tokens(left_limit,3 );
    position=0;

   }
   
   
   else
   {
    if( num_tokens > 1 ) { printf("Operator fehlt.\n"); return 1; }
    else if ( num_tokens==1 )
    {
     if ( token[0].type=='v' )
     {
      if ( token[0].text[0]<0x41||token[0].text[0]>'Z' ){ printf("ungueltiger Zellenverweis.\n");  strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak; goto inputloop;}
      if ( token[0].text[1]<0x30||token[0].text[1]>'9' ){ printf("ungueltiger Zellenverweis.\n"); strcpy(&cellstr[line][column][0],backupformula);result_is[line][column]=result_is_bak;goto inputloop;}
             if ( result_is[token[left_limit].text[0]-0x41][token[left_limit].text[1]-0x30]==0 ){ printf("Resultat nicht berechnet.%d %d \n",line,column);loopfurther=1; break;} else
             { 
                 //sprintf(token[left_limit].text,"%s",result[token[left_limit].text[0]-0x41][token[left_limit].text[1]-0x30]);
                 // to1ken[left_limit].type='n';
             result[line][column]=result[token[left_limit].text[0]-0x41][token[left_limit].text[1]-0x30];
      result_is[line][column]=1;
         }
     }
     else if ( token[0].type=='n')
     {
      result[line][column]=atoi(token[left_limit].text);
      result_is[line][column]=1;   
     }
     
     if ( token[1].type!=';') printf("Syntaxfehler.\n"); loopfurther=1; }
       
    }
    if ( num_tokens==0 && token[0].type!=';' ) { printf("Syntaxfehler.\n"); loopfurther=1; }
    if ( num_tokens==0 ) {result_is[line][column]=result_is_bak;strcpy(&cellstr[line][column][0],backupformula);}

   }
    column++;
    }
    line++;
    }
    cellresults_max++;
   }while(loopfurther==1&&cellresults_max<26*10*2 );
    
    column=0;
    line=0;
    while ( line < 26 )
    {
     column=0;
     while ( column < 10)
     {
      if ( result_is[line][column]==0 )
       printf("Zirkelbezug oder Syntaxfehler liegt wohl vor. Zelle %c%c\n",line+0x41, column+0x30);
     column++;
     }
     line++;
    }
    printf("\n");
    printf("       0      1      2      3      4      5      6      7      8      9\n");
    line=0;
    column=0;
    while ( line < 10)
    {
    printf("%c ",line+0x41);
     column=0;
     while ( column<10)
     {
      if ( (result[line][column]/10000)%10==0 &&
           (result[line][column]/1000)%10==0 &&
           (result[line][column]/100)%10==0 &&
           (result[line][column]/10)%10==0 )printf("    ");
       else  
      if ( (result[line][column]/10000)%10==0 &&
           (result[line][column]/1000)%10==0 &&
           (result[line][column]/100)%10==0 )printf("   ");
      else
      if ( (result[line][column]/10000)%10==0 &&
           (result[line][column]/1000)%10==0 )printf("  ");
      else
      if ( (result[line][column]/10000)%10==0 )printf(" ");
     
          if ( result[line][column]>=0 ) printf(" ");    
      printf("%d ",result[line][column]);
      column++;
     }printf("\n");
   
     column=0;
  while ( column<10)
     {
      printf("%c%c%c%c%c",cellname[line][column][0],cellname[line][column][1],cellname[line][column][2],cellname[line][column][3],cellname[line][column][4]);
      column++;
     }printf("\n");
    
 
     line++;
    }
    goto inputloop;
}
