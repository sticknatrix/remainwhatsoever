#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define COLUMNS_MAX 100
#define ROWS_MAX 100

#define STRLENGTH 30

#define CREATE 0
#define SELECT 1
#define ENTRIES 2
#define INTEGER 3
#define STRING 4
#define WHERE 5
#define VARNAME 6
#define SORT 7
#define INSERT 8

/*
 Beispiel
  CREATE
   dateiname
   $NAME%LICHTTYP%BETTENNUMMER
   
   mit STRG+C beenden
   
   Neustart
   LOAD
   dateiname
   INSERT
   Aloicenta
   3
   3
   
   mit Strg+C
   
   Neustart
   LOAD
   dateiname
   SELECT
   NAME
   LICHTTYP
   WHERE
   NAME
   EQUAL
   $Bellaura
   AND
   NAME
   EQUAL
   $Aloicenta
   LICHTTYP
   GREATER
   2
   ;
   
   so sollte es gehen, die Syntax wuerde von dem anderen Programm gefiltert   
    hoffentlich ist nur beim AND kein Denkfehler, dass es auch mit der kleinen Einschraenkung sich so nicht parsen liesse?!
   
   man beendete eh, wenn mit Batchdatei gesteuert, moechte nur noch mehrere Insertionen in Folge ermoeglichen?!
    man koennte auch mehrere Tabellen im RAM halten, oder gleich von Ramdisk lesen, fehlte auch wieder Performance?!
     irgendein 'Datenbanksystem' geht immer, aber ein Gutes, und hier kann man gekonnte Optimierungen wohl brauchen,
     ist richtig schwer im Vergleich zu dem hier?!
     
     */
     
     



int main(void)
{

 FILE *datei, *joinfile;

 unsigned char tablename[30];
 unsigned char varstring[1024];
 unsigned char bufstring[100];
 strcpy(tablename,"");

  int n=0,n2=0,n3=0;
  int valstart=0;

 unsigned char *inmem_load;
 struct
 {
  int type;
  unsigned char *ptr;
 }field[COLUMNS_MAX][ROWS_MAX];

long int num_columns=0;
long int num_rows=0;
int appendonfile=0;

unsigned char variables[ROWS_MAX][30];


#define GREATER 1
#define EQUAL 0
#define SMALLER 2
#define FUNCTION 0b10000000
#define STRINGMERGE 0b1000000
#define STRINGFOUND 0b100000

unsigned long int comparelist[100][3];
unsigned char compareliststr[200][30];
unsigned char variables_select[ROWS_MAX];
 
    inmem_load=malloc(sizeof(unsigned char)*STRLENGTH*COLUMNS_MAX*ROWS_MAX);
	//*STRLENGTH*valstart+sizeof(int)*(COLUMNS_MAX-valstart));

 unsigned char cmd_byte_token[255];
 while (1)
 {
 	mainloop:
  gets(cmd_byte_token);
  
  if ( strcmp(cmd_byte_token,"CREATE")==0)
  {
  	num_columns=0;
  	gets(tablename);
  	n=0;
  	while ( tablename[n]!='\0')
  	{
  		n2=n;
	  while (!isalpha(tablename[n]))n++;
  	        n3=0;
		   do{ tablename[n2+n3]=tablename[n+n3],n3++; }
		   while ( tablename[n+n3]!='\0');
  	      tablename[n2+n3]=tablename[n+n3];
	   n++;
     }
	   printf("%s",tablename);
  	datei=fopen(tablename,"wb");//Datei wird halt ueberschrieben, aufpassen?!
  	int num_fields;
  	num_fields=0;
     //wegen Unicode konnte er mal ein Struct auch nicht direkt in Datei speichern?! möglich auch noch?! nee, Unicode nur an der Konsole (bisher?!)?!
	 scanf("%s",varstring);
      n3=0;
	 n=0,n2=0;
	 int off=0;
	 off=0;
	 while ( n3<ROWS_MAX&&varstring[n]!='\0')
	 {
	  while ( varstring[n]!='%'&&varstring[n]!='$'&&varstring[n]!='\0')n++;
      if ( varstring[n]=='%')
      {
      	num_fields++;
      	//valstart=n+1;
      	n2=n+1;
      	while (varstring[n2]!='%' &&varstring[n2]!='$'&&varstring[n2]!='\0')n2++;
      	
		      strncpy(variables[n3],varstring+1+n,n2-n);
	           field[0][n].ptr=inmem_load+off;
               field[0][n].type='i';off+=2;
	         variables[n3][n2-n-1]='\0';
			//bufstring[n2-n+1]='\0';
         fprintf(datei,"%c",'i');
			/*
			variables[n3][0]=atoi(bufstring)/256;
	        fprintf(datei,"%c",variables[n3][0]);
			variables[n3][1]=atoi(bufstring)%256;
			fprintf(datei,"%c",variables[n3][1]);
	        */
	     	fprintf(datei,"%s_",variables[n3]);
	        fflush(datei);
	  n=n2;
	  n3++;
      		
	  }
	  if ( varstring[n]=='$')
      {
      	num_fields++;
      	n2=n+1;
      	while ( varstring[n2]!='$'&&varstring[n2]!='%'&&varstring[n2]!='\0')n2++;
        	strncpy(variables[n3],varstring+n+1,n2-n);
            field[0][n].ptr=inmem_load+off;
          variables[n3][n2-n-1]='\0';off+=STRLENGTH;
            field[0][n].type='s';
          	fprintf(datei,"%c",'s');			
			   fprintf(datei,"%s",variables[n3]);
              int n5=0;
                  n5=n2;
                while ( n2-n < STRLENGTH-1)fprintf(datei,"_"),n2++;
                   n=n5;
                        fflush(datei);
          n3++;

      }

	 }
	 		printf("Anzahl Zeilen bei create: %d Anzahl Spalten bei create %d\n",n3,num_columns);

     fprintf(datei,"d");fflush(datei);num_rows=0;
     fclose(datei);
   }
   else if ( strcmp(cmd_byte_token,"WRITEJOIN")==0) 
   {
   	if ( appendonfile==0)
   	{
   	appendonfile=1;
   	gets(tablename);
  	n=0;
  	while ( tablename[n]!='\0')
  	{
  		n2=n;
	  while (!isalpha(tablename[n]))n++;
  	        n3=0;
		   while ( tablename[n+n3]!='\0')tablename[n2+n3]=tablename[n+n3],n3++;
  	     
	   n++;
     }
  	joinfile=fopen(tablename,"wb");
    }
	else fclose(joinfile),appendonfile=0; 
  	
   }
  else if ( strcmp(cmd_byte_token,"LOAD")==0) 
   {
  	gets(tablename);
  	n=0;
  	while ( tablename[n]!='\0')
  	{
  		n2=n;
	  while (!isalpha(tablename[n]))n++;
  	        n3=0;
		   while ( tablename[n+n3]!='\0')tablename[n2+n3]=tablename[n+n3],n3++;
  	     
	   n++;
     }
  	datei=fopen(tablename,"rb");
  	
  	int num_text, num_values;
  	   num_text=0, num_values=0;
  	   
  	   num_columns=0;
		n3=0;
		num_rows=0;
	  do
  	{
	cmd_byte_token[0]=fgetc(datei);
  	if ( cmd_byte_token[0]=='s'||cmd_byte_token[0]=='i')
	{  
	     field[0][num_rows].ptr=inmem_load+num_values*2+num_text*STRLENGTH;
	  
	   if (cmd_byte_token[0]=='i')num_values++,num_rows++;
	   if ( cmd_byte_token[0]=='s')num_text++,num_rows++;
	
	  field[0][num_rows-1].type=cmd_byte_token[0];
		 n2=0;
	   do { variables[num_rows-1][n2]=fgetc(datei);n2++; }while (n2<STRLENGTH&&variables[num_rows-1][n2-1]!='_');
	     if ( variables[num_rows-1][n2-1]=='_')variables[num_rows-1][n2-1]='\0';
	    
	  }
  	
     }while ( cmd_byte_token[0]!='d'&&!feof(datei)&&num_rows<ROWS_MAX);
      //if ( feof(datei))return;
       int eoflinemore=0;
       eoflinemore=0;
       int off=0;
	 while(eoflinemore!=1)
	 {
	  if (!feof(datei))fread(inmem_load+off*num_columns,sizeof(unsigned char),(num_text*STRLENGTH+num_values*2),datei);
       //if ( feof(datei))break;
	  off=0;
	  n=0;
      while (n<num_rows)
      {
	    field[num_columns][n].type=field[0][n].type;
	    field[num_columns][n].ptr=inmem_load+(num_values*2+num_text*STRLENGTH)*num_columns+off;
	    if ( field[0][n].type=='i')off+=2;
		  else off+=STRLENGTH;
      n++;
	 }
	  if ( feof(datei))eoflinemore++;
	  num_columns++;
	 }
	 num_columns--;
		printf("Anzahl Zeilen bei load: %d Anzahl Spalten bei load %d\n",num_rows,num_columns);

	  fclose(datei);
	}
	else if ( strcmp(cmd_byte_token,"INSERT")==0)
	{
		if ( tablename[0]!='\0')datei=fopen(tablename,"awb");
		
		int off=0;
		off=0;
     n=0;
     printf("binda");
    
     while (n<num_rows)
	 {
	  scanf("%s",bufstring);
	  bufstring[STRLENGTH]='\0';
	      //field[num_columns][n].type=variables[n][0];
	 
	 
	 if (num_columns==0)field[num_columns][n].ptr=inmem_load+off;
	 
	  if ( field[0][n].type=='i') off+=2,
			field[num_columns][n].ptr[0]=atoi(bufstring)/256,
	       	field[num_columns][n].ptr[1]=atoi(bufstring)%256;
	 
	   if ( field[0][n].type=='s') off+=STRLENGTH,
		     strncpy((field[num_columns][n].ptr),bufstring,STRLENGTH);
			 	     
	  n++;
     }
	
     printf("gelinderfragzeiche");
     printf("habs fwrite,%d",off);
	   fwrite(inmem_load+off*num_columns,sizeof(unsigned char),off,datei);
	 num_columns++;

	  n=0;
      while (n<num_rows)
      {
	    field[num_columns][n].ptr=field[num_columns-1][n].ptr+off;
	    field[num_columns][n].type=field[num_columns-1][n].type;
      n++;
	 }
	  	printf("Anzahl Zeilen bei Insert: %d Anzahl Spalten bei Insert %d\n",num_rows,num_columns);
	 fclose(datei);
	 
    }
    else if ( strcmp(cmd_byte_token,"SELECT")==0)
	{
		
		printf("selektieren");
    	int n_selected;
    	 n_selected=0;
     n=0;
     n2=0;
    	int cmp1, cmp2;
    	unsigned char cmpstring1[100],cmpstring2[100];
    	int negate=0;
    do
	{
	 scanf("%s",bufstring);
	 //numfornames?!
	 n=0;
	 while ( strcmp(variables[n],bufstring)!=0&&n<num_rows+1)n++;
	 variables_select[n_selected]=n;
	 if (n<num_rows+1)n_selected++;
    }while (strcmp(bufstring,"WHERE")!=0 && strcmp(bufstring,";")!=0 );
   
   		int andflipprint=0;
		andflipprint=0;

      unsigned char functionresult[100];
      unsigned int functionresultval=0;

    int n5=0;
	 n2=0;
    if ( bufstring[0]!=';')	
	do
	{
		int offval1=0, offval2=0;
		repeat:
	      negate=0;
		offval1=0, offval2=0;
		comparelist[n2][1]=0;
	 scanf("%s",bufstring);
	 if ( strcmp(bufstring,";")==0)break;
	 strcpy(compareliststr[n2*2],bufstring);
	 if ( strcmp(bufstring,"ADDED")==0)
	 {
	 scanf("%s",bufstring);
	   offval1+=atoi(bufstring);
     }
	 
	 if ( strcmp(bufstring,"SUBTRACTED")==0)
	 {
	 scanf("%s",bufstring);//- operator oder stringeinlesefunktion schon mit_drin?!
	   offval1-=atoi(bufstring);
     }
     if ( strcmp(bufstring,"AND")==0)
     {
     	andflipprint=3;
     	 goto repeat;
	 }
	 	   if ( strcmp(bufstring,"NOT")==0)
	   {
	   	 negate=1;
	   	  goto repeat;
		   }
		   	   if ( strcmp(bufstring,"NOTFLIP")==0)
	   {
	   	 negate=0;
	     goto repeat;
		   }


	 if ( strcmp(bufstring,"FUNCTION")==0)
     {
       scanf("%s",bufstring);
		   if ( strcmp(bufstring,"STRINGMERGE")==0)
		   {
		   	comparelist[n2][1]=FUNCTION|(andflipprint<<2);
		   	comparelist[n2][1]=FUNCTION|0b01000000;
		   }
		    else
		   if ( strcmp(bufstring,"STRINGFOUND")==0)
		   {
		   	comparelist[n2][1]=FUNCTION|(andflipprint<<2);
		   	comparelist[n2][1]=FUNCTION|0b0100000;
		   }
     	
     	
	 }
	 
	 
	 n=0;
	 while ( strcmp(variables[n],bufstring)!=0&&n<num_rows)n++;
	 printf("%d",n);
	 
	  comparelist[n2][0]=n;
	  if ( n==num_rows)comparelist[n2][0]=atoi(bufstring)+1000000+offval1+offval2 >= 2000000 ? 1999999 : atoi(bufstring)+1000000+offval1+offval2;
	   
	   n5=0;
	   if (n==num_rows)
	   {
	    while (bufstring[n5]!='\0'){ if ( isalpha(bufstring[n5]))comparelist[n2][0]=2000000;n5++;}
	    if ( comparelist[n2][0]==2000000)if(bufstring[0]=='$')strcpy(compareliststr[n2*2],bufstring+1); else { printf("Variable nicht gefunden."); goto mainloop;}
       }
       
        repass_compare:
       if ( comparelist[n2][1]<FUNCTION)
       {
	 scanf("%s",bufstring);
	  if ( strcmp(bufstring,"GREATER")==0) comparelist[n2][1]=GREATER|(andflipprint<<2);
	  else
	  if ( strcmp(bufstring,"EQUAL")==0) comparelist[n2][1]=EQUAL|(andflipprint<<2);
	  else
	  if ( strcmp(bufstring,"SMALLER")==0) comparelist[n2][1]=SMALLER|(andflipprint<<2);// war comparelist[n]?!!?!?!
	   else goto repeat;
      }
	 scanf("%s",bufstring);
	 strcpy(compareliststr[n2*2+1],bufstring);
	 n=0;
	 while ( strcmp(variables[n],bufstring)!=0&&n<num_rows)n++;
	  comparelist[n2][2]=n;
	 printf("%d %d",n,num_rows);
	  if ( n==num_rows)comparelist[n2][2]=atoi(bufstring)+1000000+offval1+offval2 >= 2000000 ? 1999999 : atoi(bufstring)+1000000+offval1+offval2;
	   n5=0;
	   if (n==num_rows)
	   {
	    while (bufstring[n5]!='\0'){ if ( isalpha(bufstring[n5]))comparelist[n2][2]=2000000;n5++;}
	    if ( comparelist[n2][2]==2000000)if(bufstring[0]=='$')strcpy(compareliststr[n2*2+1],bufstring+1);else{ printf("Variable nicht gefunden."); goto mainloop;}
       }
       if ( (comparelist[n2][1]&STRINGMERGE)!=0)
       {
	   	strcat(compareliststr[n2*2],compareliststr[n2*2+1]);
       	comparelist[n2][1]=0;
       	goto repeat;
	   }
	  n2++;
	  if (andflipprint==1) andflipprint=0;
	  if ( andflipprint==3)andflipprint=1;
	  
    }while (strcmp(bufstring,";")!=0);
    
    int num_compares;
    num_compares=n2;
    
    int printout=0;
    int bufprintout=2;
    n=0;
    printout=0;
    int n4=0;
    while ( n<num_columns)
    {
    	int oldprintout;
    	
    	printout=1;
        oldprintout=1;
        bufprintout=1;
    	
    	 printout=1;
		n4=0;
		while (n4<num_compares)
		{
			bufprintout=2;
    	 if (oldprintout==0)printout=0; // selbe Variablen im Kreis zuweisen in Schleife?!
    	if((comparelist[n4][1]&0b1100)==0b100)bufprintout=printout, printf("wuerde ausgegeben %d",n4);
     		
	   		cmpstring1[0]='\0', cmpstring2[0]='\0';
		
	 	  if ( comparelist[n4][0]==2000000) strcpy(cmpstring1,compareliststr[n4*2]);
		   else
		 if(comparelist[n4][0]>=1000000) cmp1=comparelist[n4][0]-1000000;
 		else if ( field[n][comparelist[n4][0]].type=='i')cmp1=field[n][comparelist[n4][0]].ptr[0]*256+field[n][comparelist[n4][0]].ptr[1];
     	 else if ( comparelist[n4][0]!=2000000)
		  {
		  strncpy(cmpstring1,field[n][comparelist[n4][0]].ptr,STRLENGTH);
     	 	n5=0;
		 	while(cmpstring1[n5]!='_'&&cmpstring1[n5]!='\0'&&n5<STRLENGTH)n5++;
		 	cmpstring1[n5]='\0';
	      }
	      
     	  if ( comparelist[n4][2]==2000000) strcpy(cmpstring2,compareliststr[n4*2+1]);
		   else
	 	if(comparelist[n4][2]>=1000000) cmp2=comparelist[n4][2]-1000000;
 		else if ( field[n][comparelist[n4][2]].type=='i')cmp2=field[n][comparelist[n4][2]].ptr[0]*256+field[n][comparelist[n4][2]].ptr[1];
    	 else if ( comparelist[n4][2]!=2000000)
		 {
		  strncpy(cmpstring2,field[n][comparelist[n4][2]].ptr,STRLENGTH);
	     	n5=0;
		 	while(cmpstring2[n5]!='_'&&cmpstring2[n5]!='\0'&&n5<STRLENGTH)n5++;
		 	cmpstring2[n5]='\0';
	      }


	if ( comparelist[n4][1]<FUNCTION)
    	{
    	   if ( cmpstring1[0]!='\0'&&cmpstring2[0]!='\0')
	        {
	        	printf("binda");
	         if((comparelist[n4][1]&0b11)==EQUAL)if ( strcmp(cmpstring1,cmpstring2)!=0) printout=0; else printout=1;
			}
			else if (cmpstring1[0]=='\0'&&cmpstring2[0]=='\0')
			{	        
			 if ( (comparelist[n4][1]&0b11)==EQUAL )if(cmp1==cmp2 )printout=1;else printout=0;
	         if ( (comparelist[n4][1]&0b11)==SMALLER )if(cmp1<cmp2 )printout=1; else printout=0;
			 if ( (comparelist[n4][1]&0b11)==GREATER )if(cmp1>cmp2 )printout=1; else printout=0;
	        } else { printf("Fehler beim Vergleich.\n"); return 1; }
	  
	     }
	     else
	      if ( (comparelist[n4][1]&STRINGFOUND)>0 )
		  {
		  	if ( strstr(cmpstring1,cmpstring2)==0);else printout=0;
		  }	
	  	
	 	 if (bufprintout==0)printout=0,printf("ausgabe"),bufprintout=2;
      if ( negate==1)printout^=1;
	    
	  n4++;
	  	  
	    }
	   
	   if ( printout==1)
	   {
	   	n2=0;
	   	
	   	while ( n2< num_rows&&n2<n_selected)
	   	{
	   		   if ( field[n][variables_select[n2]].type=='i')
	   		   {
			   printf("%s %d ",variables[variables_select[n2]], field[n][variables_select[n2]].ptr[0]*256+field[n][variables_select[n2]].ptr[1]);
	           if ( appendonfile==1)fprintf(joinfile,"%c%c",field[n][variables_select[n2]].ptr[0],field[n][variables_select[n2]].ptr[1]);
			  }
			else
			if ( field[n][variables_select[n2]].type=='s')
			{
			  n3=0;
			  
			  
			                            n3=0;
                          while ( field[n][variables_select[n2]].ptr[n3]!='_'&&field[n][variables_select[n2]].ptr[n3]!='\0'&&n3<STRLENGTH)n3++;
                          strncpy(bufstring,field[n][variables_select[n2]].ptr,n3);

			  bufstring[n3]='\0';
			  printf("%s %s ",variables[variables_select[n2]],bufstring);
			  if ( appendonfile==1)
			   {
                            fprintf(joinfile,"%s",bufstring);
                            while ( n3 < STRLENGTH)fprintf(joinfile,"_"),n3++;
		       }
			  
			  	   	    }
		  n2++;
		 }
		         fflush(joinfile);
	   	   		printf("\n");

	   }
	  n++;
     }
	 	
	}

 }
 


}

