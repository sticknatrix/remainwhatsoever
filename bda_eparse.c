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


struct
{
 int type;
 char text[255];
 unsigned char constraint[255];
 unsigned char dumpparm[255];
}token[255];

static int num_tokens;

void remove_tokens( int position, int amount_removed )
{

 while( position+ amount_removed <= num_tokens )
 {
  token[position].type=token[position+amount_removed].type;
  strcpy( token[position].text, token[position+amount_removed].text );
 strcpy( token[position].constraint, token[position+amount_removed].constraint );
 strcpy( token[position].dumpparm, token[position+amount_removed].dumpparm );

  position++;
 }

 num_tokens-=amount_removed;

}


/*
   Filter fuer die Syntax von dem zweiten Programm soll es werden?!
  SELECT BETTENZAHL SONNENSCHUTZ PRIVAT WHERE BETTENZAHL = 1 & SONNENSCHUTZ > 3 & STEUERKLASSE > PRIVAT & PRIVAT = 1 ;
  
    was 'integrierteres' als Dateiumleiten der Aus- und Eingabe geht bestimmt?!
  */


int main(void)
{
 int position;
 int txtcount;
 int left_limit, right_limit;
 int reduction_type;
 char rvalnum[20];

 unsigned char bufstring[1024];
 unsigned char bufstring2[1024];

 position=0;

 do
 {
  txtcount=0;
  scanf("%s", token[position].text );

  if(strcmp(token[position].text,"SELECT")==0
     && strlen(token[position].text )== 6 ) token[position].type='S',
     txtcount++;
  else
   if(strcmp(token[position].text,"WHERE")==0
      && strlen(token[position].text )== 5 ) token[position].type='W',
      txtcount++;
  else
   if(strcmp(token[position].text,"LOAD")==0
      && strlen(token[position].text )== 4 ) token[position].type='L',
      txtcount++;
  else
   if(strcmp(token[position].text,"CREATE")==0
      && strlen(token[position].text )== 4 ) token[position].type='C',
      txtcount++;
  else
   if(token[position].text[0]=='='
       && strlen(token[position].text )== 1 ) token[position].type='=',
       txtcount++;
  else
   if(token[position].text[0]=='<'
       && strlen(token[position].text )== 1 ) token[position].type='<',
       txtcount++;
  else
   if(token[position].text[0]=='>'
      && strlen(token[position].text )== 1 ) token[position].type='>',
      txtcount++;       
  else
    if(token[position].text[0]=='&'
        && strlen(token[position].text )== 1 ) token[position].type='&',
        txtcount++;
  else
    if(token[position].text[0]=='|'
        && strlen(token[position].text )== 1 ) token[position].type='|',
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
   if(token[position].text[0]=='!'
      && strlen(token[position].text )== 1 ) token[position].type='!',
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
     while( isalpha(token[position].text[txtcount] ) ||token[position].text[txtcount]=='%'||token[position].text[txtcount]=='$')txtcount++;
     while( isnum(token[position].text[txtcount] ) ) txtcount++;
     token[position].type='v',token[position].dumpparm[0]='\0', token[position].constraint[0]='\0'; /* Variable */
     if ( token[position].text[txtcount] != '\0' ) return 1;
    
	  if ( strcmp(token[position].text,"MERGE")==0) token[position].type='M';
	  if ( strcmp(token[position].text,"STRINGIN")==0) token[position].type='I';
	  
	}
  else
    if ( isnum( token[position].text[0] ) )
  {
    txtcount=0;
    while( isnum( token[position].text[txtcount] ) ) txtcount++;
    token[position].type='n'; /* numerische Konstante */
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
 
   position=left_limit;
    if ( token[position].type=='L' )
      if ( token[position+1].type== 'v' && token[position+2].type==';' )
      {
       reduction_type='L', right_limit=position+1;
       position++;
      }
 
 
  left_limit=0, right_limit=num_tokens;
 
   position=left_limit;
    if ( token[position].type=='C' )
      if ( token[position+1].type== 'v' && token[position+2].type==';' )
      {
       reduction_type='C', right_limit=position+1;
       position++;
	  }
 
 
   position=left_limit;
   while( position <=right_limit )
   {
    if ( token[position].type=='s' )
      if ( token[position+1].type== 'w'||token[position+1].type==';' )
      {
       reduction_type='a', right_limit=position+1, left_limit=position;
       break;
      }
     position++;
   }
   
   
  position=left_limit;
  while( token[position].type!= 'W'&& position < num_tokens )
  {
   position++;
  }
   if ( token[position].type== 'W' )
   {
    right_limit=position;
    left_limit=position;

    while(token[right_limit].type != ';' && right_limit < num_tokens ) right_limit++;

    if(token[right_limit].type==';' )
    {
   	 reduction_type='W';
  	}
    else return 1;

   }
   
 if ( reduction_type=='W')
 {
  position=left_limit;
  while( token[position].type!= 'N'&& position < num_tokens )
  {
   position++;
  }
   if ( token[position].type== 'N' )
   {
    right_limit=position;
    left_limit=position;
     reduction_type='N';
    }

   }
 
 
 if ( reduction_type=='W')
 {
  position=left_limit;
  while( token[position].type!= ')'&& position < num_tokens )
  {
   position++;
  }
   	 int posbuf;
   if ( token[position].type== ')' )
   {
     posbuf=position;  
    while(token[position].type != 'M' && position >0 )
	{
	 position--;
	  if ( token[position].type!='v')break;
	 }
     if ( position>0)position--;
	
	}
	if(token[position].type=='M'&&token[position+1].type=='(' )
    {
    	
    reduction_type='M';
       right_limit=posbuf;
    left_limit=position;
     
    }
    
}
   

if ( reduction_type=='W')
 {
  position=left_limit;
  while( token[position].type!= ')'&& position < num_tokens )
  {
   position++;
  }
   	 int posbuf;
   if ( token[position].type== ')' )
   {
     posbuf=position;  
    while(token[position].type != 'I' && position >0 )
	{
	 position--;
	  if ( token[position].type!='v')break;
	 }
     if ( position>0)position--;
	
    }
    if(token[position].type=='I'&&token[position+1].type=='(' )
    {
     reduction_type='I';
       right_limit=posbuf;
    left_limit=position;

    }
    
}


 if ( reduction_type=='W')
 {
  position=left_limit;
  while( token[position].type!= ')'&& position < num_tokens )
  {
   position++;
  }
   	 int posbuf;
   if ( token[position].type== ')' )
   {
     posbuf=position;  
    while(token[position].type != '(' && position >0 )
	{
	 position--;
	 }
   	
    }
    if ( token[position-1 > 0 ? position-1 : 0].type!='M'&& token[position-1> 0 ? position-1 : 0].type!='I')
    if(token[position].type=='(' )
    {
     reduction_type='(';
       right_limit=posbuf;
    left_limit=position;

    }

}


 if ( reduction_type=='W')
 {
 
 position=right_limit;
   while( position >=left_limit )
   {
     if( token[position].type=='!' )
      if ( token[position+1].type=='c'  )
         { reduction_type='!';
           right_limit=position+1, left_limit=position;
           break; }
     position--;
   }
   
}       
  
  position=left_limit;
  while( token[position].type!= 'S'&& position < num_tokens )
  {
   position++;
  }
   if ( token[position].type== 'S' )
   {
    right_limit=position;
    left_limit=position;

    while(token[right_limit].type != ';' && token[right_limit].type!='W' && token[right_limit].type!='w'&&right_limit < num_tokens ) right_limit++;

    if(token[right_limit].type==';' || token[right_limit].type=='W'||token[right_limit].type=='w')
    {
	 reduction_type='s';
    }
    else return 1;

   }

if ( reduction_type=='W'|| reduction_type=='(')
{
   position=left_limit;
   while( position+2 <= right_limit)
   {
    if ( token[position].type=='v' || token[position].type=='n' || token[position].type=='r' )
      if ( token[position+1].type=='+' || token[position+1].type=='-' )
        if ( token[position+2].type=='v' || token[position+2].type=='n' || token[position+2].type=='r')
         { reduction_type=token[position+1].type;
           right_limit=position+2, left_limit=position;
           break; }
     position++;
   }
      
}

if ( reduction_type!='M'&&reduction_type!='I')
{
 position=right_limit-1;
   while( position >=left_limit )
     {
    if ( token[position].type=='v'||token[position].type=='l'  )
        if ( token[position+1].type=='v'||token[position+1].type=='l' )
         { reduction_type='l';
           right_limit=position+1, left_limit=position;
           break; }
     position--;
   }
}
   position=right_limit-2;
   while( position >=left_limit )
   {
    if ( token[position].type=='v' || token[position].type=='n'|| token[position].type=='c'  )
     if( token[position+1].type=='=' )
      if ( token[position+2].type== 'v' || token[position+2].type=='n'|| token[position+2].type=='c'  )
      {
       reduction_type='=', right_limit=position+2, left_limit=position;
       break;
      }
     position--;

   }
  
   position=right_limit-2;
   while( position >=left_limit )
   {
    if ( token[position].type=='v' || token[position+2].type=='n' || token[position+2].type=='c')
     if( token[position+1].type=='<' )
      if ( token[position+2].type== 'v' || token[position+2].type=='n' || token[position+2].type=='c')
      {
       reduction_type='<', right_limit=position+2, left_limit=position;
       break;
      }
     position--;

   }
   
   position=right_limit-2;
   while( position >=left_limit )
   {
    if ( token[position].type=='v' || token[position+2].type=='n' || token[position+2].type=='c')
     if( token[position+1].type=='>' )
      if ( token[position+2].type== 'v' || token[position+2].type=='n' || token[position+2].type=='c')
      {
       reduction_type='>', right_limit=position+2, left_limit=position;
       break;
      }
     position--;

   }
  
 
   
   position=right_limit-2;
   while( position >=left_limit )
   {
    if ( token[position].type=='v' || token[position].type=='n'|| token[position].type=='c'  )
     if( token[position+1].type=='&' )
      if ( token[position+2].type== 'v' || token[position+2].type=='n'|| token[position+2].type=='c'  )
         { reduction_type='&';
           right_limit=position+2, left_limit=position;
           break; }
     position--;
   }
   
   
   position=right_limit-2;
   while( position >=left_limit )
   {
    if ( token[position].type=='v' || token[position].type=='n'|| token[position].type=='c'  )
     if( token[position+1].type=='|' )
      if ( token[position+2].type== 'v' || token[position+2].type=='n'|| token[position+2].type=='c'  )
         { reduction_type='|';
           right_limit=position+2, left_limit=position;
           break; }
     position--;
   }
   
//printf("%c",reduction_type);

   if ( reduction_type=='C')
   {
   	printf("CREATE\n%s\n",token[left_limit+1].text);
   	remove_tokens(left_limit,1);
   	token[left_limit].type=';';//kannszumselbenreduzierenandersverschiedenwegs?!
   }
   else
   if ( reduction_type=='L')
   {
   	printf("LOAD\n%s\n",token[left_limit+1].text);
   	remove_tokens(left_limit,1);
   	token[left_limit].type=';';
   }
   else
   if ( reduction_type=='a'  ) /* akzpetiere, Ausdruck */
   {
    printf("SELECT\n%s\nWHERE\n%s\n",token[left_limit].dumpparm,token[left_limit+1].constraint);
    remove_tokens(left_limit,2 );
   }
   else
   if ( reduction_type=='l'  ) /* Variablenliste */
   {
        strcat(token[left_limit].dumpparm, token[left_limit].text);    
        strcat(token[left_limit].dumpparm, " ");    
       // strcat(token[left_limit+1].dumpparm, token[left_limit+1].text);    
        strcat(token[left_limit].dumpparm, token[left_limit+1].dumpparm);    
   	
         strcpy(token[left_limit].text,"");
		  
          strcpy(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,1 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
      
    token[left_limit].type='l';
    
   }
     else
   if ( reduction_type=='s'  ) /* Select Variablenliste */
   {
      if ( token[left_limit].type=='S'&& (token[left_limit+1].type=='v'||token[left_limit+1].type=='l') )
       {
       	strcat(token[left_limit].dumpparm, token[left_limit+1].dumpparm);
	     strcpy(token[left_limit].text,"");
       strcat(token[left_limit].dumpparm, token[left_limit+1].text);    
    
    
	strcpy(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,1 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
    token[left_limit].type='s';
     }else { printf("Syntaxfehler bei Select."); return 1;}
     
	 }
     else
   if ( reduction_type=='W'  ) /* Where Constraints */
   {
      if ( token[left_limit].type=='W'&&token[left_limit+1].type=='c'&&token[left_limit+2].type==';')
      {
       strcpy(token[left_limit].constraint, token[left_limit+1].constraint);    
       
          strcpy(token[left_limit].text,"");
       strcpy(bufstring,token[left_limit].constraint);
       strcpy(bufstring2,token[left_limit].dumpparm);
     remove_tokens(left_limit,1 );
       strcpy(token[left_limit].constraint,bufstring);
       strcpy(token[left_limit].dumpparm,bufstring2);
  
     token[left_limit].type='w';
   
    }else { printf("Syntaxfehler bei Where"); return 1;}
   }
   else
   if ( reduction_type=='N'  ) /* Negation nachfolgreduktion */
   {        
      strcpy(bufstring,token[left_limit].constraint);
     strcat(bufstring,"\nNOTFLIP\n");
       strcpy(bufstring2,token[left_limit].dumpparm);
      strcat(bufstring2,"\nFLIPNOT\n");
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
       strcpy(token[left_limit].text,"");
  	token[left_limit].type='c';
   }
   else
   if ( reduction_type=='I')
   {
   	int n5=2;
   	n5=2;
   	while ( token[left_limit+n5].type=='v'&& n5<4)
   	{
            //strcat(token[left_limit].constraint, token[left_limit].text);    
             strcat(token[left_limit].constraint,"\n");
            strcat(token[left_limit+n5].constraint, token[left_limit+n5].text);    
            strcat(token[left_limit].constraint, token[left_limit+n5].constraint);    
             n5++;
	  }
     strcpy(bufstring,"\nSTRINGFOUND\n");
      strcat(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,"\nSTRINGFOUND\n");
       strcat(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,n5 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
       strcpy(token[left_limit].text,"");
  	token[left_limit].type='c';
   }
   
   /* nun koenne man Ihnen aber nicht den ungezwungenen 'Respekt' fuer Ihre aeussere Aussenehspraesentation entgegenbringen, ohne dass man frei denken, hoeren und sehen koenne?!
     ansonsten muesste man luegen und sich in vielen Bereichen von Belang von der eigenen Gesundheit ausschweigen, was einem wirklich sorgen machen koennte, auch noch mit Noetigung
	  zur Partizipation?! man koennte ja diese Themen einfach meiden, bis man es nicht mehr aushielte?!
	   jeder darf aber denken, was er will, solange er dadurch nicht irgendein boeses Verhalten nach aussen bringt?!
	     auf eine emotionale, oder sonstwie andere Ebene oder mit anderen Themen seien immer noch Interaktionsmoeglichkeiten gegeben?!
	       nur wie erklaert man Dinge, die jemand Anderes einfach nicht versteht oder man selber, muss man dann die andere Version glauben?!
	        man muess entweder luegen, oder man ist selber bekloppt?!
	        und findet auch nie mehr eine Freundin?!
	        
	        Melanind - die Melange vom Land aus roter Schokolade mit Minze von den Feldern und Colastraeuchern von den Wäldern von B. Lindt?!?
	         Kleider machen Blinde?!
	  
	  es differenziert beim echten Ablauf statt mit Backtracking immer vom Aehnlichsten weiter aus, da, wo man lange nicht war, wucherte es alles klein in der Praesenz?!
	    es
	    
	    ach, mein Papa und Alfred Richter von Rial non Food kannten sich und redeten öfters viel starben innerhalb eines halben Jahres noch, was haben Sie wohl geplaudert?!
	     Papa hat mir mal erzaehlt, 1990 haette sich Eine aus Alfred Richter seiner mit Auspuffgasen umgebracht, nur nebenbei wieder eingefallen
	     ein klein wenig auffaellig ist hier aber wohl nur, dass Beide zu ziemlich derselben Zeit gestorben waeren, vielleicht leben Sie ja noch?!
	      keine Ahnung, was da wieder sein koennte, nur erwaehnt, ist vielleicht auch nichts in Lampertheim ausgesprochen worden oder so?!
     
     */
   
   
   else
   if ( reduction_type=='M')
   {
   	int n5=2;
   	n5=2;
   	//printf("lala%d",token[left_limit+n5].type);
   	 strcpy(token[left_limit].constraint, "");    
           
	   while ( token[left_limit+n5].type=='v')
   	{
            //strcat(token[left_limit].constraint, token[left_limit].text);    
             strcat(token[left_limit].constraint,"\n");
            strcat(token[left_limit+n5].constraint, token[left_limit+n5].text);    
            strcat(token[left_limit].constraint, token[left_limit+n5].constraint);    
          n5++;
	  }
	 strcpy(bufstring,"\nSTRINGMERGE\n");
      strcat(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,"\nSTRINGMERGE\n");
       strcat(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,n5 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
       strcpy(token[left_limit].text,"");
  	token[left_limit].type='c';
   }
   else
   if ( reduction_type=='('  ) /* Klammern */
   {
     if ( right_limit-left_limit != 2 ) { printf("Fehlerhafter Klammerinhalt.\n");
           getch(); return 1; }
    if ( token[left_limit+1].type!='c' && token[left_limit+1].type!='v'&&token[left_limit+1].type!='n' ) { printf("Syntaxfehler bei Klammerung.\n");
          return 1; }

    
      strcpy(bufstring,token[left_limit+1].constraint);
      strcat(bufstring,token[left_limit+1].text);
      strcpy(bufstring2,token[left_limit+1].dumpparm);
    remove_tokens(left_limit,2 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
       strcpy(token[left_limit].text,"");
  	token[left_limit].type='c';
	
   }
   else
   if ( reduction_type=='+'  ) /* Addition */
   {
         strcat(token[left_limit].constraint, token[left_limit].text);    
             strcat(token[left_limit].constraint,"\n");
            strcat(token[left_limit+2].constraint, token[left_limit+2].text);    
            strcat(token[left_limit].constraint, token[left_limit+2].constraint);    
    
      strcpy(bufstring,"ADDED\n");
      strcat(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,2 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
       strcpy(token[left_limit].text,"");
  	token[left_limit].type='c';

   }
   else
   if ( reduction_type=='-'  ) /* Subtraktion */
   {
         strcat(token[left_limit].constraint, token[left_limit].text);    
             strcat(token[left_limit].constraint,"\n");
            strcat(token[left_limit+2].constraint, token[left_limit+2].text);    
            strcat(token[left_limit].constraint, token[left_limit+2].constraint);    
    
      strcpy(bufstring,"SUBTRACTED\n");
      strcat(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,2 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
       strcpy(token[left_limit].text,"");
  	token[left_limit].type='c';

   }
   else
   if ( reduction_type=='!'  ) /* Negation */
   {
            strcpy(token[left_limit].constraint, "");    
             strcat(token[left_limit].constraint,"\n");
            strcat(token[left_limit+1].constraint, token[left_limit+1].text);    
            strcat(token[left_limit].constraint, token[left_limit+1].constraint);    
    
     strcpy(bufstring,"\nNOT\n");
      strcat(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,"\nNOT\n");
       strcat(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,1 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
       strcpy(token[left_limit].text,"");
  	token[left_limit].type='N';
   }
   else
   if ( reduction_type=='&'  ) /* Bedingungsverkettung */
   {
            strcat(token[left_limit].constraint, token[left_limit].text);    
             strcat(token[left_limit].constraint,"\n");
            strcat(token[left_limit+2].constraint, token[left_limit+2].text);    
            strcat(token[left_limit].constraint, token[left_limit+2].constraint);    
    
     strcpy(bufstring,"\nAND\n");
      strcat(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,"\nAND\n");
       strcat(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,2 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
       strcpy(token[left_limit].text,"");
  	token[left_limit].type='c';

   }
   else
   if ( reduction_type=='|'  ) /* Bedingungsverkettung */
   {
            strcat(token[left_limit].constraint, token[left_limit].text);    
             strcat(token[left_limit].constraint,"\n");
            strcat(token[left_limit+2].constraint, token[left_limit+2].text);    
            strcat(token[left_limit].constraint, token[left_limit+2].constraint);    
    
      strcpy(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,2 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
       strcpy(token[left_limit].text,"");
  	token[left_limit].type='c';

   }
   else
   if ( reduction_type== '='  ) /* Vergleich */
   {
            strcat(token[left_limit].constraint, token[left_limit].text);    
      strcat(token[left_limit].constraint, "\nEQUAL\n");    
       strcat(token[left_limit+2].constraint, token[left_limit+2].text);    
      strcat(token[left_limit].constraint,"\n");
        strcat(token[left_limit].constraint, token[left_limit+2].constraint);    
      strcat(token[left_limit].constraint,"\n");
    
      strcpy(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,2 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
       strcpy(token[left_limit].text,"");
  
    token[left_limit].type='c';
  
   }
    else
   if ( reduction_type== '<'  ) /* Vergleich */
   {
            strcat(token[left_limit].constraint, token[left_limit].text);    
      strcat(token[left_limit].constraint, "\nSMALLER\n");    
            strcat(token[left_limit+2].constraint, token[left_limit+2].text);    
      strcat(token[left_limit].constraint, token[left_limit+2].constraint);    
      strcat(token[left_limit].constraint,"\n");
    
      strcpy(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,2 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
       strcpy(token[left_limit].text,"");
  
    token[left_limit].type='c';
  
   }
   else
   if ( reduction_type== '>'  ) /* Vergleich */
   {
            strcat(token[left_limit].constraint, token[left_limit].text);    
   strcat(token[left_limit].constraint, "\nGREATER\n");    
               strcat(token[left_limit+2].constraint, token[left_limit+2].text);    
      strcat(token[left_limit].constraint, token[left_limit+2].constraint);    
      strcat(token[left_limit].constraint,"\n");
     
      strcpy(bufstring,token[left_limit].constraint);
      strcpy(bufstring2,token[left_limit].dumpparm);
    remove_tokens(left_limit,2 );
      strcpy(token[left_limit].constraint,bufstring);
      strcpy(token[left_limit].dumpparm,bufstring2);
     strcpy(token[left_limit].text,"");
  
    token[left_limit].type='c';
  
   }
   else if ( reduction_type==';'){ printf(";"); return 0; }
   else
   {
    if( num_tokens > 1 ) { printf("Operator fehlt.\n"); getch(); return 1; }
    else if ( num_tokens==1 )
       if( token[0].type!=';' )
          { printf("Operator fehlt.\n"); getch(); return 1; }

    if ( num_tokens==0 && token[0].type!=';' ) { printf("Syntaxfehler.\n");
          getch(); return 1; }

   }
  }
  getch();
}
