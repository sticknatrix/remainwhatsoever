/*
  trying to add ann algebra solve operator into the syntax of the programming language and
    making algebra obsolete
    
    additional info is carried through r values
    put the sums on the left and the factors on the right and then divide
    
    
    type in
    ( ( 2 * x ) - 10 ) S ( 3 * ( x - 10 ) ) ;
      
      see what it does. I'm not certain whether it really works full
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int var_name_count=0;

void variable_countup(char *name, int do_countup)
{

 char appendix[255];
 char *base="rval";
 if ( do_countup == 1 ) ++var_name_count;
   itoa(var_name_count, appendix, 10);
   
 strcpy(name,base);
 strcat( name, appendix );
 return ;

}

int isnum(char c)
{
 if ( c>=0x30 && c <=0x39 ) return 1;
 return 0;

}

unsigned char flushstring[1000],flushprefix[100],flushout[1000];

void sequencepoint()
{
 printf("%s%s",flushprefix,flushstring);	
  flushstring[0]='\0', flushprefix[0]='\0',flushout[0]='\0';
 	
}


struct
{
 int type;
 char text[255];
 signed int mulvar;
 signed int sumvar;
 signed int onx;

}token[255];

static int num_tokens;

struct
{
 unsigned char labelnum[20];	
 unsigned int type;
}labelstack[100];

unsigned int label_name_count=0;
unsigned int label_stack_count=0;

unsigned char labelname[255];

void label_countup(char *name, int do_countup)
{

 char appendix[255];
 char *base="label";
 if ( do_countup == 1 ) ++label_name_count;
   itoa(label_name_count, appendix, 10);
   
 strcpy(name,base);
 strcat( name, appendix );
 return ;

}


void remove_tokens( int position, int amount_removed )
{

 while( position+ amount_removed <= num_tokens )
 {
  token[position].type=token[position+amount_removed].type;
  strcpy( token[position].text, token[position+amount_removed].text );
   token[position].sumvar=token[position+amount_removed].sumvar;
   token[position].mulvar=token[position+amount_removed].mulvar;
    token[position].onx=token[position+amount_removed].onx;

  position++;
 }

 num_tokens-=amount_removed;

}



int main(void)
{
 int position;
 int txtcount;
 int left_limit, right_limit;
 int reduction_type=0;
 char rvalnum[20];
   signed int sumbuf,mulbuf;
   signed int mulbufr, sumbufr;
   signed int onxbuf;
          

   flushstring[0]='\0', flushprefix[0]='\0',flushout[0]='\0';

  newline:

 	sequencepoint();

  position=0;
  reduction_type=0;
 do
 {
 	sequencepoint();
  txtcount=0;
  scanf("%s", token[position].text );

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
   if(token[position].text[0]=='S'
       && strlen(token[position].text )== 1 ) token[position].type='S',
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
    if(token[position].text[0]=='='
        && strlen(token[position].text )== 1 ) token[position].type='=',
        txtcount++;
  else
    if(token[position].text[0]==':'
        && strlen(token[position].text )== 1 ) token[position].type=':',
        txtcount++;
   else
    if(token[position].text[0]==';'
        && strlen(token[position].text )== 1 ) token[position].type=';',
        txtcount++;
  else
   if ( strcmp(token[position].text,"if")==0)
   {
   	token[position].type='i';
   	
   }
     else
   if ( strcmp(token[position].text,"for")==0)
   {
   	token[position].type='f';
   	
   }
     else
   if ( strcmp(token[position].text,"next")==0)
   {
   	token[position].type='x';
   	
   }
      else
   if ( strcmp(token[position].text,"print")==0)
   {
   	token[position].type='p';
   	
   }
     else
   if ( strcmp(token[position].text,"input")==0)
   {
   	token[position].type='\?';
   	
   }
  else
    if ( isalpha( token[position].text[0] ) )
    {
     txtcount=0;
     
     while( isalpha(token[position].text[txtcount] ) )txtcount++;
     while( isnum(token[position].text[txtcount] ) ) txtcount++;
     token[position].type='v'; /* Variable */
     token[position].onx=0;
     token[position].sumvar=0,token[position].mulvar=1;
     if ( token[position].text[0]=='x') token[position].onx=1;
	 if ( token[position].text[txtcount] != '\0' ) return 1;
    }
  else
    if ( isnum( token[position].text[0] ) )
  {
    txtcount=0;
    while( isnum( token[position].text[txtcount] ) ) txtcount++;
    token[position].type='n'; /* numerische Konstante */
     token[position].sumvar=atoi(token[position].text),token[position].mulvar=0;
      token[position].onx=0;
	if ( token[position].text[txtcount] != '\0' ) return 1;
  
  }
   else { printf("Lexikalischer Fehler.\n"); getch(); return 1;}

  position++;
 }while(token[position-1].type!= ';' );
  num_tokens=position-1;


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
    getch();

    debug Ende  */


  left_limit=0, right_limit=num_tokens;
 
 
   position=right_limit-1;
   while( position >=left_limit )
   {
    if ( token[position].type=='i' )
      if ( token[position+1].type== 'v' || token[position+1].type=='n' || token[position+1].type=='r')
      {
       reduction_type='i', right_limit=position+1, left_limit=position;
       break;
      }
     position--;
     
   }
   
   
   
   position=right_limit-1;
   while( position >=left_limit )
   {
    if ( token[position].type=='f' )
      if ( token[position+1].type== 'v' || token[position+1].type=='n' || token[position+1].type=='r')
      {
       reduction_type='f', right_limit=position+1, left_limit=position;
       break;
      }
     position--;
     
   }
   
   
  
     
   position=right_limit;
   while( position >=left_limit )
   {
    if ( token[position].type=='x' )
      {
	   reduction_type='n', right_limit=position+1, left_limit=position;
	   break;
      }
     position--; 
 
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
   
   
   position=left_limit;
   while( position+2 <= right_limit )
   {
    if ( token[position].type=='v' || token[position].type=='n' || token[position].type=='r' )
      if ( token[position+1].type=='S' )
        if ( token[position+2].type=='v' || token[position+2].type=='n' || token[position+2].type=='r')
         { reduction_type=token[position+1].type;
           right_limit=position+2, left_limit=position;
           break; }
     position++;
   }


   position=right_limit-2;
   while( position >=left_limit )
   {
    if ( token[position].type=='v' )
     if( token[position+1].type=='=' )
      if ( token[position+2].type== 'v' || token[position+2].type=='n' || token[position+2].type=='r')
      {
       reduction_type='=', right_limit=position+2, left_limit=position;
       break;
      }
     position--;

   }
   

   position=right_limit-2;
   while( position >=left_limit )
   {
    if ( token[position].type=='v'|| token[position+2].type=='n' || token[position+2].type=='r')
     if( token[position+1].type==':' )
      if ( token[position+2].type== 'v' || token[position+2].type=='n' || token[position+2].type=='r')
      {
       reduction_type=':', right_limit=position+2, left_limit=position;
       break;
      }
     position--;

   }
   
   
   
   position=right_limit-1;
   while( position >=left_limit )
   {
    if ( token[position].type=='p' )
      if ( token[position+1].type== 'v' || token[position+1].type=='n' || token[position+1].type=='r')
      {
       reduction_type='p', right_limit=position+1, left_limit=position;
       break;
      }
     position--;
    }
    
    
   position=right_limit-1;
   while( position >=left_limit )
   {
    if ( token[position].type=='\?' )
      if ( token[position+1].type== 'v' )
      {
       reduction_type='\?', right_limit=position+1, left_limit=position;
       break;
      }
     position--;
    }
   
   if ( reduction_type=='('  ) /* Klammern */
   {
     if ( right_limit-left_limit != 2 ) { printf("Fehlerhafter Klammerinhalt.\n");
           getch(); return 1; }
    if ( token[left_limit+1].type!='n' && token[left_limit+1].type!='v' &&
         token[left_limit+1].type !='r' ) { printf("Syntaxfehler bei Klammerung.\n");
          return 1; }

    variable_countup( rvalnum,1 );

    if( token[left_limit+1].type!= 'n' )
       sprintf(flushout,"mov [%s],[%s]\n",rvalnum,token[left_limit+1].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"mov [%s],%s\n",rvalnum,token[left_limit+1].text ),strcat(flushstring,flushout);

      sumbuf=token[left_limit+1].sumvar,
      mulbuf=token[left_limit+1].mulvar;
      onxbuf=token[left_limit+1].onx;
    remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    token[left_limit].sumvar=sumbuf;
    token[left_limit].mulvar=mulbuf;
    token[left_limit].onx=onxbuf;
	strcpy( token[left_limit].text, rvalnum );
   }
   else
   if ( reduction_type=='+'  ) /* Addition */
   {

    variable_countup( rvalnum,1 );

    if( token[left_limit+2].type!= 'n' )
       sprintf(flushout,"mov ax,[%s]\n",token[left_limit+2].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"mov ax,%s\n",token[left_limit+2].text ),strcat(flushstring,flushout);

    if( token[left_limit].type!= 'n' )
       sprintf(flushout,"add ax,[%s]\n",token[left_limit].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"add ax,%s\n",token[left_limit].text ),strcat(flushstring,flushout);

     sprintf(flushout,"mov [%s],ax\n", rvalnum ),strcat(flushstring,flushout);

      sumbuf=token[left_limit].sumvar+token[left_limit+2].sumvar,
      mulbuf=token[left_limit].mulvar+token[left_limit+2].mulvar;
      onxbuf=token[left_limit].mulvar*token[left_limit].onx+token[left_limit+2].mulvar*token[left_limit+2].onx;
	     if ( onxbuf!=0)onxbuf=1; else onxbuf=0;
	     
	remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );
    
    token[left_limit].sumvar=sumbuf;
    token[left_limit].mulvar=mulbuf;
    token[left_limit].onx=onxbuf;
	token[left_limit].sumvar+=atoi(token[left_limit].text)+atoi(token[left_limit+2].text);
   

   }
   else
   if ( reduction_type=='-'  ) /* Subtraktion */
   {
     variable_countup( rvalnum,1 );

    if( token[left_limit].type!= 'n' )
       sprintf(flushout,"mov ax,[%s]\n",token[left_limit].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"mov ax,%s\n",token[left_limit].text ),strcat(flushstring,flushout);

    if( token[left_limit+2].type!= 'n' )
       sprintf(flushout,"sub ax,[%s]\n",token[left_limit+2].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"sub ax,%s\n",token[left_limit+2].text ),strcat(flushstring,flushout);

     sprintf(flushout,"mov [%s],ax\n", rvalnum ),strcat(flushstring,flushout);
     
     
	  sumbuf=token[left_limit].sumvar-token[left_limit+2].sumvar;
       mulbuf=token[left_limit].mulvar-token[left_limit+2].mulvar;
       onxbuf=token[left_limit].mulvar*token[left_limit].onx+token[left_limit+2].mulvar*token[left_limit+2].onx;
	      if ( onxbuf!=0)onxbuf=1; else onxbuf=0;
	
    remove_tokens(left_limit,2 );

   
    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );
    token[left_limit].sumvar=sumbuf;
    token[left_limit].mulvar=mulbuf;
    token[left_limit].onx=onxbuf;
     token[left_limit].sumvar-=atoi(token[left_limit].text)+atoi(token[left_limit+2].text);
   

   }
   else
   if ( reduction_type=='*'  ) /* Multiplikation */
   {
     variable_countup( rvalnum,1 );

    if( token[left_limit+2].type!= 'n' )
       sprintf(flushout,"mov ax,[%s]\n",token[left_limit+2].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"mov ax,%s\n",token[left_limit+2].text ),strcat(flushstring,flushout);

    if( token[left_limit].type!= 'n' )
       sprintf(flushout,"mul ax,[%s]\n",token[left_limit].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"mul ax,%s\n",token[left_limit].text ),strcat(flushstring,flushout);

     sprintf(flushout,"mov [%s],ax\n", rvalnum ),strcat(flushstring,flushout);


      sumbuf=(token[left_limit].sumvar)*(token[left_limit+2].sumvar),
      mulbuf=((token[left_limit].sumvar+token[left_limit].mulvar))*((token[left_limit+2].sumvar+token[left_limit+2].mulvar));
      
      if ( token[left_limit+2].onx==1&&token[left_limit+2].sumvar!=0)mulbuf-=(token[left_limit+2].sumvar)*
	                                                                        ((token[left_limit].sumvar+token[left_limit].mulvar));
      if ( token[left_limit].onx==1&&token[left_limit].sumvar!=0)mulbuf-=(token[left_limit].sumvar)*
	                                                                        ((token[left_limit+2].sumvar+token[left_limit+2].mulvar));
	  
	  if ( token[left_limit].onx==0&&token[right_limit].onx==0)onxbuf=0; else onxbuf=1;
   
    remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );
    
	token[left_limit].sumvar=sumbuf;
    token[left_limit].mulvar=mulbuf;
    token[left_limit].onx=onxbuf;
    token[left_limit].mulvar-=atoi(token[left_limit].text)+atoi(token[left_limit+2].text);
   
   }
   else
   if ( reduction_type=='/'  ) /* Division */
   {
     variable_countup( rvalnum,1 );

    if( token[left_limit].type!= 'n' )
       sprintf(flushout,"mov ax,[%s]\n",token[left_limit].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"mov ax,%s\n",token[left_limit].text ),strcat(flushstring,flushout);

    if( token[left_limit+2].type!= 'n' )
       sprintf(flushout,"div ax,[%s]\n",token[left_limit+2].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"div ax,%s\n",token[left_limit+2].text ),strcat(flushstring,flushout);

     sprintf(flushout,"mov [%s],ax\n", rvalnum ),strcat(flushstring,flushout);

    
    
        if ( token[left_limit+2].sumvar!=0&&token[left_limit+2].mulvar!=0)
        {
      sumbuf=(token[left_limit].sumvar/token[left_limit+2].sumvar)*token[left_limit].mulvar*token[left_limit].onx/token[left_limit+2].mulvar*token[left_limit+2].onx,
      mulbuf=(token[left_limit].sumvar/token[left_limit+2].sumvar)*token[left_limit].mulvar*token[left_limit].onx/token[left_limit+2].mulvar*token[left_limit+2].onx;
       } else sumbuf=-1, mulbuf=-1;
   
   
	remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );
     
	 token[left_limit].sumvar=sumbuf;
    token[left_limit].mulvar=mulbuf;
    token[left_limit].onx=(mulbuf > 0 ? 1 : 0 );
	token[left_limit].mulvar+=atoi(token[left_limit].text)+atoi(token[left_limit+2].text);
    
   } 
   else
   if ( reduction_type=='S'  ) /* Algebra-Solve */
   {
     variable_countup( rvalnum,1 );

          
       mulbuf=token[left_limit].mulvar*token[left_limit].onx,mulbufr=token[left_limit+2].mulvar*token[left_limit+2].onx;
       sumbuf=token[left_limit].sumvar,sumbufr=token[left_limit+2].sumvar;
         
	   printf("%d %d %d %d\n",sumbuf,sumbufr,mulbuf,mulbufr);
       
       mulbuf-=mulbufr;
	   sumbufr-=sumbuf;
       
       
       if ( mulbuf!=0)
       sumbufr/=mulbuf;
      
     sprintf(flushout,"mov ax,%d\n", sumbufr ),strcat(flushstring,flushout); 
     sprintf(flushout,"mov [%s],ax\n", rvalnum ),strcat(flushstring,flushout);

    remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );

   }
   else
   if ( reduction_type== '='  ) /* Zuweisung */
   {
     variable_countup( rvalnum,1 );

     if ( token[left_limit].type=='r' ){  printf("Zuweisung zu R-Wert !\n"); return 1; }

    if( token[left_limit+2].type!= 'n' )
       sprintf(flushout,"mov ax,[%s]\n"
              "mov [%s],ax\n",token[left_limit+2].text, token[left_limit].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"mov ax,%s\n"
              "mov [%s],ax\n",token[left_limit+2].text, token[left_limit].text ),strcat(flushstring,flushout);

     sprintf(flushout,"mov [%s],ax\n", rvalnum ),strcat(flushstring,flushout);

    remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );

   }
   else
   if ( reduction_type== ':'  ) /* Vergleich */
   {
     variable_countup( rvalnum,1 );

    
    if( token[left_limit+2].type!= 'n' )
       sprintf(flushout,"mov ax,[%s]\n"
              "cmp [%s],ax\n",token[left_limit+2].text, token[left_limit].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"mov ax,%s\n"
              "cmp ax,[%s]\n"
              "mov ax,0\n"
			  "repz mov ax,1\n",token[left_limit+2].text, token[left_limit].text ),strcat(flushstring,flushout);

     sprintf(flushout,"mov [%s],ax\n", rvalnum ),strcat(flushstring,flushout);

    remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );

   }
   else
   if ( reduction_type== 'p'  ) /* Ausgabe */
   {
     variable_countup( rvalnum,1 );

    if( token[left_limit+1].type!= 'n' )
       sprintf(flushout,"\nPRINT [%s]\n",token[left_limit+1].text),strcat(flushstring,flushout);
     else
       sprintf(flushout,"\nPRINT %s\n",token[left_limit+1].text),strcat(flushstring,flushout);
     
    remove_tokens(left_limit,1 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );

   }
   
   else
   if ( reduction_type== '\?'  ) /* Vergleich */
   {
     variable_countup( rvalnum,1 );

     if ( token[left_limit].type=='r' || token[left_limit].type=='n' ){  printf("Zuweisung zu R-Wert oder Konstante!\n"); return 1; }

    if( token[left_limit+1].type== 'v' )
       sprintf(flushout,"INPUT [%s]\n",token[left_limit+1].text),strcat(flushstring,flushout);
       
    remove_tokens(left_limit,1 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );
    
   }
   else
   if ( reduction_type=='i')
   {
	labelstack[label_stack_count].type='i';
	label_countup(labelstack[label_stack_count].labelnum,1);
   	label_stack_count++;

    if ( token[left_limit+1].type=='n')
   	sprintf(flushout,"cmp %s,TRUE\n"
	      "jz %s\n",token[left_limit+1].text, 
		   labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout);
	  else
	  sprintf(flushout,"cmp [%s],TRUE\n"
	       "jz %s\n",token[left_limit+1].text, 
		   labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout);
	    
    remove_tokens(left_limit,1 );
   	 token[left_limit].type='c';
    
   }
   else if ( reduction_type=='f')
   {
   	labelstack[label_stack_count].type='f';
   	label_countup(labelstack[label_stack_count].labelnum,1);
   	label_stack_count++;
   	labelstack[label_stack_count].type='f';
   	label_countup(labelstack[label_stack_count].labelnum,1);
   	label_stack_count++;
    if ( token[left_limit+1].type=='n')
   	sprintf(flushout,"\n:%s\n",labelstack[label_stack_count-2].labelnum),strcat(flushprefix,flushout),
	        
			sprintf(flushout,
			"cmp %s,TRUE\n"
			"jz %s\n",token[left_limit+1].text,labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout);
   else
   	sprintf(flushprefix,"\n:%s\n",labelstack[label_stack_count-2].labelnum),
	  sprintf(flushout,
	         "cmp [%s],TRUE\n"
			"jz %s\n",token[left_limit+1].text,labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout);
	   
	      remove_tokens(left_limit,1 );
  	 token[left_limit].type='c';
   
	       
   }
   else if ( reduction_type=='n')
   {
   	if ( label_stack_count==0 ) return;
   if(label_stack_count<2)
	   if(labelstack[label_stack_count-1].type=='f') return;
   	
   	if ( labelstack[label_stack_count-1].type=='i')sprintf(flushout,":%s\n",labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout),
	   label_stack_count--;
   	else
   	if ( labelstack[label_stack_count-1].type=='f')sprintf(flushout,"\njmp %s\n"
	                                                       "\n%s:\n",labelstack[label_stack_count-2].labelnum,
														   labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout),
														   label_stack_count-=2 ;
   	else return;
   	
   	 token[left_limit].type='c';
   	
   }
   else if ( reduction_type==';')
   {
   	if ( right_limit> 1 ) return;
	remove_tokens(left_limit,1 );
    goto newline;
   	
   }
   else
   {
    if( num_tokens > 1 ) { printf("Operator fehlt.\n"); getch(); return 1; }
    else if ( num_tokens==1 )
       if( ( token[0].type!= 'v' && token[0].type!= 'r' &&
          token[0].type!='n') || token[1].type != ';' )
          { printf("Operator fehlt.\n"); getch(); return 1; }

    if ( num_tokens==0 && token[0].type!=';' ) { printf("Syntaxfehler.\n");
          getch(); return 1; }

   }

  }
  getch();
}

/*
this is crappier than Cobol, but anyway...

Test run ...

x = 0 ;
y = 0 ;
xc = 0 ;
yc = 0 ;
for ( yc : 0 ) ;
x = 0 ;
for ( xc : 0 ) ;
x = x + 1 ;
if ( x : 10 ) ;
xc = 1 ;
next ;
screen = 1 ;
next ;
xc = 0 ;
y = y + 1 ;
if ( y : 100 ) ;
yc = 1 ;
next ;
next ;
next ;


produced...

mov ax,0
mov [x],ax
mov [rval1],ax
mov ax,0
mov [y],ax
mov [rval2],ax
mov ax,0
mov [xc],ax
mov [rval3],ax
mov ax,0
mov [yc],ax
mov [rval4],ax

:label1
mov ax,0
cmp ax,[yc]
mov ax,0
repz mov ax,1
mov [rval5],ax
mov [rval6],[rval5]
cmp [rval6],TRUE
jz label2
mov ax,0
mov [x],ax
mov [rval7],ax

:label3
mov ax,0
cmp ax,[xc]
mov ax,0
repz mov ax,1
mov [rval8],ax
mov [rval9],[rval8]
cmp [rval9],TRUE
jz label4
mov ax,1
add ax,[x]
mov [rval10],ax
mov ax,[rval10]
mov [x],ax
mov [rval11],ax
mov ax,10
cmp ax,[x]
mov ax,0
repz mov ax,1
mov [rval12],ax
mov [rval13],[rval12]
cmp [rval13],TRUE
jz label5
mov ax,1
mov [xc],ax
mov [rval14],ax
:label5
mov ax,1
mov [screen],ax
mov [rval15],ax

jmp label3

label4:
mov ax,0
mov [xc],ax
mov [rval16],ax
mov ax,1
add ax,[y]
mov [rval17],ax
mov ax,[rval17]
mov [y],ax
mov [rval18],ax
mov ax,100
cmp ax,[y]
mov ax,0
repz mov ax,1
mov [rval19],ax
mov [rval20],[rval19]
cmp [rval20],TRUE
jz label6
mov ax,1
mov [yc],ax
mov [rval21],ax
:label6

jmp label1

label2:
*/
