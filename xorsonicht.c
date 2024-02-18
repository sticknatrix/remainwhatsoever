#include <stdio.h>
#include <fcntl.h>

int main(int argc, char*argv[])
{

 int n, lenstr;
  if ( argc==2)
  {
   lenstr=strlen(argv[1]);
   n=0;
       FILE *input;
       input=fopen(".\\xoreingabe.txt","rb");
        printf("xorausgabe.txt wird ueberschrieben, wenn vorhanden, sonst Programmabbruch (strg+c) nicht y eingeben");
        if ( getchar()!='y'){
        	fclose(input); return 1; }
       FILE *output;
       output=fopen(".\\xorausgabe.txt","wb");
 
 long long int dtypefreqvalue=0;
 int n2=0;
 unsigned long int bit_was_set[8]={0,0,0,0,0,0,0,0};
	 int pos=0;
   while (!feof(input))
   {
   	unsigned char c;
   	c=fgetc(input);
   	/*if(isalpha(c))*/
	   bit_was_set[0]+=c&1;
	   bit_was_set[1]+=(c>>1)&1;
	   bit_was_set[2]+=(c>>2)&1;
	   bit_was_set[3]+=(c>>3)&1;
	   bit_was_set[4]+=(c>>4)&1;
	   bit_was_set[5]+=(c>>5)&1;
	   bit_was_set[6]+=(c>>6)&1;
	   bit_was_set[7]+=(c>>7)&1;
    /*if ( isalpha(c))*/c=(c^argv[1][n%lenstr]),fputc(c,output);
    n++;
   }
   fclose(input);
    (float)bit_was_set[0]/((float)n)< 0.5? (bit_was_set[0]=0) : (bit_was_set[0]=1); 
    (float)bit_was_set[1]/((float)n)< 0.5? (bit_was_set[1]=0) : (bit_was_set[1]=1); 
    (float)bit_was_set[2]/((float)n)< 0.5? (bit_was_set[2]=0) : (bit_was_set[2]=1); 
    (float)bit_was_set[3]/((float)n)< 0.5? (bit_was_set[3]=0) : (bit_was_set[3]=1); 
    (float)bit_was_set[4]/((float)n)< 0.5? (bit_was_set[4]=0) : (bit_was_set[4]=1); 
    (float)bit_was_set[5]/((float)n)< 0.5? (bit_was_set[5]=0) : (bit_was_set[5]=1); 
    (float)bit_was_set[6]/((float)n)< 0.5? (bit_was_set[6]=0) : (bit_was_set[6]=1); 
    (float)bit_was_set[7]/((float)n)< 0.5? (bit_was_set[7]=0) : (bit_was_set[7]=1); 
   printf("das haeufigste Byte des Datentypen ist %d\n",bit_was_set[0]+(bit_was_set[1]<<1)+(bit_was_set[2]<<2)+(bit_was_set[3]<<3)+(bit_was_set[4]<<4)
                                                      +(bit_was_set[5]<<5)+(bit_was_set[6]<<6)+(bit_was_set[7]<<7));

  } 
  else
  {
    int XORstring[30]; 
    n=0;
    int pos1[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int pos2[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int pos3[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int pos4[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int pos5[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int pos6[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int pos7[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int pos8[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
     
       FILE *input;
       input=fopen(".\\xorausgabe.txt","rb");
       
       int dtypemfvalue;
       int numchars=0;
       
       printf("Byteweisen haeufigsten Wert der Datenart eingeben?!");
       scanf("%d",&dtypemfvalue);
       printf("Stringzeichenlaenge eingeben?! (max. 30)",&numchars);
       scanf("%d",&numchars);
       if ( numchars>30||numchars==0){fclose(input);return;}
  
    while (!feof(input))
    {
    	unsigned char c;
       c=fgetc(input);
     pos1[n%numchars]+=(((unsigned char)c^(int)dtypemfvalue) >> 7 )&1;     
     pos2[n%numchars]+=(((unsigned char)c^(int)dtypemfvalue) >> 6 )&1;     
     pos3[n%numchars]+=(((unsigned char)c^(int)dtypemfvalue) >> 5 )&1;     
     pos4[n%numchars]+=(((unsigned char)c^(int)dtypemfvalue) >> 4 )&1;     
     pos5[n%numchars]+=(((unsigned char)c^(int)dtypemfvalue) >> 3 )&1;     
     pos6[n%numchars]+=(((unsigned char)c^(int)dtypemfvalue) >> 2 )&1;     
     pos7[n%numchars]+=(((unsigned char)c^(int)dtypemfvalue) >> 1 )&1;     
     pos8[n%numchars]+=(((unsigned char)c^(int)dtypemfvalue)  )&1;     
     n++;     
    }
    fclose(input);

    int n2=0;
    while(n2<numchars)
    {
     if ( (float)pos1[n2]/((float)n/numchars)<0.5 ) pos1[n2]=0; else pos1[n2]=1;
     if ( (float)pos2[n2]/((float)n/numchars)<0.5 ) pos2[n2]=0; else pos2[n2]=1;
     if ( (float)pos3[n2]/((float)n/numchars)<0.5 ) pos3[n2]=0; else pos3[n2]=1;
     if ( (float)pos4[n2]/((float)n/numchars)<0.5 ) pos4[n2]=0; else pos4[n2]=1;
     if ( (float)pos5[n2]/((float)n/numchars)<0.5 ) pos5[n2]=0; else pos5[n2]=1;
     if ( (float)pos6[n2]/((float)n/numchars)<0.5 ) pos6[n2]=0; else pos6[n2]=1;
     if ( (float)pos7[n2]/((float)n/numchars)<0.5 ) pos7[n2]=0; else pos7[n2]=1;
     if ( (float)pos8[n2]/((float)n/numchars)<0.5 ) pos8[n2]=0; else pos8[n2]=1;
      XORstring[n2]=(pos1[n2]<<7) +(pos2[n2]<<6)+(pos3[n2]<<5)+(pos4[n2]<<4)+(pos5[n2]<<3)+(pos6[n2]<<2)+(pos7[n2]<<1)+pos8[n2];
     n2++;

    }
    n=0;
    while ( n< numchars)
    printf("%c",XORstring[n]),fflush(stdout),n++;
	
  }

}
