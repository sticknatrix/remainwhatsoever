#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned char cmpstring[255];

int permutate(unsigned char cmpstring[255], unsigned char verdreh[255], unsigned char vergleich[255], int lenreach, unsigned int stelletaken[255], int rec_depth)
{
  unsigned int n=0,n2=0;
  unsigned char buf;
  verdreh[lenreach]='\0';
  if ( strlen(cmpstring) != strlen(vergleich)) return 0;
	
	if ( lenreach > strlen(vergleich)) return 0;
	
	if ( rec_depth > 10) return 1;
	
	//if ( strncmp(verdreh,vergleich,lenreach)!= 0) { return 1;}
     		
     		//printf("%s %d\n",verdreh,strlen(cmpstring));
 	  if ( lenreach==strlen(cmpstring) )
	   {
 	     	//  printf("%s\n", verdreh );
	  if ( strcmp(verdreh, vergleich)==0 )
 	     { 
		 	
 	   		   return 2;
			 // while ( n< strlen(verdreh)) printf("%d ", stellefrei[n]), n++;
		      
			  }
			  return 0;
         }
	 n2=0;
	 
	   while ( n2 < strlen(cmpstring) )
	   {
		   if ( stelletaken[n2]==0  )
		   {
		   	verdreh[lenreach]=cmpstring[n2]; verdreh[lenreach+1]='\0';
			stelletaken[n2]=1;
			if ( lenreach==strlen(cmpstring)-1 )printf("%s fuer %s\n", verdreh,vergleich);
			 /*if(*/	permutate(cmpstring, verdreh, vergleich, lenreach+1, stelletaken, rec_depth+1);// == 2 ) return 2;
	        
			 stelletaken[n2]=0;
	        
		 }
    
    
	    n2++;	  
     }

   	    


}

int main(int argc, char *argv[])
{
  unsigned char dummystr[6];
  unsigned char stellefrei[255];
  dummystr[0]='\0';
  unsigned char compare[]={'M','A','D','R','E','\0'};
  unsigned char aword[]={'P','A','T','E','R','\0'};
 
   int n=0;
   while (n<255)stellefrei[n]=0,n++;
 
  permutate(compare, dummystr,aword, 0, stellefrei, 0); 	        
	 return 0;
	
}
