/* ist die angegebene Frequenz im Signal enthalten, wird sie verstaerkt, weil auf jede Signalaenderung in der
 entsprechenden Richtung und Staerke eine Sinusschwingung nachhallt.
 Das erzeugt dann einen Resonanzeffekt.
 Raw oder .snd - Eingabedatei mit 8 Bit unsigned char mono, an der Kommandozeile der erste Parameter die Datei und singstar.raw ist die Ausgabedatei. Mit Bloodshed GCC kompilieren und zum Bearbeiten ist wohl Goldwave am besten geeignet.
 Ausgabe mit 16 Bit unsigned char mono, ich glaube little endian.

  Es filtert das gesamte Sprachband nach den Toenen, die in der Standardstimmung vorkommen.
  Das Resultat mischt man dann mit dem unbearbeiteten Gesang zusammen und man hat dann zwar Stoergeraeusche,
  aber die Toene scheinen besser getroffen.

 */


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char signal[12000000];
float signalb[4000];

void interpolate(long int start, long int count) // Glaettung auf die fuer die Zielfrequenz benoetigte Samplerate
{
 signed long int buf, count2, off=0, end=0;

 end=4000/count;

while (off<end)
{
  buf=0;
  count2=count;
 while (count2 > 0){ 
   buf+=signal[start+count2+off*count]; count2--; }

   buf/=count;
   if ( buf > 255) buf=255;
   if ( buf < 0 ) buf=0;

  while ( count2 < count)
 {
  signalb[count2+off*count]=(unsigned char)buf;
 count2++;
 }
 off++;
 

}

}


// mit vorgespeicherten Sinustabellen laeuft es schneller
float sintab[128];

void sininit(void)
{
 float value;
 value=-6.4;

 while ( value < 6.4 )
 {
  sintab[(int)((value+6.4)*10.0)]=sin(value);
  value+=0.1;
 }

}

/*
inline float SIN(float value)
{
 signed int sign;
 signed int index;

 if ( value < 0 ) sign=-1;
 else sign=+1;
  index= ((int)( value*10 )%64);
  index+=63;

 return sintab[index];
}
*/
#define SIN(val) sintab[(int)((val)*10) %64 +63]


float filterin[8000];
unsigned short int output[12000000];

int writeit=0;

int main(int argc, char*argv[])
{

  FILE *input, *input2;
  unsigned int long n=0, n2=0, n3=0, filesize, randoff;
  float steigung, frequenz, frequenz_gen, frequenz_ausgeb, stoplen, maxsteigung,frequenz_gen_2, frequenz_ausgeb_2;
float compfrequenz, smallest,
      biggest, loudest, loudest2, buf;
  
  if ( argc != 2 )
  {
   printf("Falsche Anzahl Argumente!\n");
   return 1;
  
  }

  sininit();
  input=fopen(argv[1],"rb");


  if ( input == NULL )
  {
   printf("Datei nicht gefunden!\n");
   return 1;
  }

  
 fread( signal, sizeof(unsigned char), 12000000, input);
   fseek(input, 0 , SEEK_END);
 filesize = ftell(input);
  if ( filesize > 12000000) { printf("Eingabedatei zu gross.\n");
   return 1;
  }
 fclose(input);

 input2=fopen(".\\mask","wb");

  n=0;
  while ( n < filesize) output[n]=32767,n++;
  n=0;
  while ( n < filesize )
  {
   
  loudest=0.0;
  loudest2=0.0;
   //printf("Block %d\n",n/4000);
 

#define CUTOFF 3500
   compfrequenz=CUTOFF;
    while ( compfrequenz > 100 )
   {
     
     compfrequenz=compfrequenz/1.0175;
	/*
	 switch((int)compfrequenz)
	 {
	 	
	 	
	  case (int)CUTOFF/1: 
	  compfrequenz=1046*2; break;
      case (int)(1046*2) : compfrequenz=988*2; break;
	  case (int)(988*2): compfrequenz=932*2; break;
      case (int)(932*2): compfrequenz= 880*2; break;
      case (int)(880*2): compfrequenz=830*2; break;
      case (int)(830*2): compfrequenz=784*2; break;
      case (int)(784*2): compfrequenz=740*2; break;
      case (int)(740*2): compfrequenz= 698*2; break;
      case (int)(698*2): compfrequenz=660*2; break;
      case (int)(660*2): compfrequenz=622*2; break;
      case (int)(622*2): compfrequenz=588*2; break;
      case (int)(588*2): compfrequenz=554*2; break;
      case (int)(554*2): compfrequenz=1046; break;
	  
      case (int)(1046) : compfrequenz=988; break;
	  case (int)(988): compfrequenz=932; break;
      case (int)(932): compfrequenz= 880; break;
      case (int)(880): compfrequenz=830; break;
      case (int)(830): compfrequenz=784; break;
      case (int)(784): compfrequenz=740; break;
      case (int)(740): compfrequenz= 698; break;
      case (int)(698): compfrequenz=660; break;
      case (int)(660): compfrequenz=622; break;
      case (int)(622): compfrequenz=588; break;
      case (int)(588): compfrequenz=554; break;
      case (int)(554): compfrequenz=523; break;
	
      case (int)(523) : compfrequenz=494; break;
	  case (int)(494): compfrequenz=466; break;
      case (int)(466): compfrequenz= 440; break;
      case (int)(440): compfrequenz=415; break;
      case (int)(415): compfrequenz=392; break;
      case (int)(392): compfrequenz=370; break;
      case (int)(370): compfrequenz= 349; break;
      case (int)(349): compfrequenz=330; break;
      case (int)(330): compfrequenz=311; break;
      case (int)(311): compfrequenz=294; break;
      case (int)(294): compfrequenz=277; break;
      case (int)(277): compfrequenz=261; break;
    }
    compfrequenz=(int)compfrequenz*1;
     */
     
     next:
     stoplen=44100/compfrequenz;
     frequenz= 3.14 /(44100/compfrequenz) ;
    
   n2=0;
   while ( n2 < 8000 )
   {
    filterin[n2]=0.0;n2++;
    
   }

#define OVERSKIPP (44100/(compfrequenz*2))

   interpolate(n,OVERSKIPP);  
    
    n3=OVERSKIPP;
     while ( n3 < 4000) // mit der Steigung des Signals einen Sinus-Nachhall in einem sich wiederholenden Array-Buffer erzeugen
    {
     steigung= signalb[n3]-signalb[n3-(int)(OVERSKIPP)];
      steigung/=2;
	
     if ( fabs(steigung) > 0.0)
	 {	        
     n2=0;
     while (  n2 < 4000 )
     {		 
      buf=  SIN((float)n2*frequenz)*steigung;
       filterin[n2+n3]+=buf;
       
	   n2+=OVERSKIPP;
	  }
    }
     n3+=OVERSKIPP;

    }

   
   n2=0;
   smallest=0.0;
   biggest=0.0;
   
    while ( n2<8000)
   {
     if ( filterin[n2]/(compfrequenz/4000) < smallest) smallest=filterin[n2]/(compfrequenz/4000);
     if ( filterin[n2]/(compfrequenz/4000) > biggest ) biggest=filterin[n2]/(compfrequenz/4000);
    n2++;
   } // Maximalausschlag im Buffer ergibt die Amplitude der zu filternden Frequenz
 
  smallest+=100000.0, biggest+=100000.0;
  steigung=fabs(smallest-biggest);
  
  
  steigung=((int)steigung/1)*1;
  
   
  if ( steigung >= loudest)
   {
    loudest2=loudest, frequenz_gen_2=frequenz_gen, frequenz_ausgeb_2=frequenz_ausgeb;
   loudest=steigung, frequenz_gen= 3.14 /(44100/compfrequenz), frequenz_ausgeb=compfrequenz ;
   }
  
  
  
 }
 
  
  if ( loudest >= 10.0) // unter diesem Amplituden-Schwellwert wird alles geloescht
  {
   writeit=1;  	
   //frequenz_gen= 3.14 /(44100/compfrequenz);
   //frequenz_ausgeb=compfrequenz ;
   frequenz_ausgeb/=4.0;
   frequenz_ausgeb_2/=4.0;
   
   n2=0;
   
   randoff=rand()%4000; // so hallt und klirrt es nicht so arg beim Aneinanderreihen der Bloecke
   while ( n2 < 4000){ 
    output[n2+n]+=(signed int)(SIN((float)n2*frequenz_gen*1.00+randoff)*steigung*1.0);
    //output[n2+n]+=(signed char)(SIN((float)n2*frequenz_gen*1.26+randoff)*steigung*0.01); // Schwingung in der Ausgabe schonmal rekonstruieren
    //output[n2+n]+=(signed char)(SIN((float)n2*frequenz_gen*1.5+randoff)*steigung*0.01); // Schwingung in der Ausgabe schonmal rekonstruieren
    
    n2++;
   }

   //printf("%d:%d\n", (int)compfrequenz*2, (int)(steigung)*1);
  
    printf("%d %d %d\n", (int)frequenz_ausgeb, loudest2>2.0 ? (int)frequenz_ausgeb_2 : (int)frequenz_ausgeb, (int)steigung);
   }
   else printf("0 0 0\n");
  
  
  n+=4000; // naechster Block
  
    

    

  }

/*
 n=0;
  while ( n < filesize) output[n]=output[n] /* +signal[n]*//*,n++;
 
  input2=fopen(".\\singstar.raw", "wb");
  fwrite( output, sizeof(unsigned char), filesize*2, input2);
  fclose(input2);*/

}
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



  #define noteC    0
     #define noteCis  1
     #define noteD    2
     #define noteDis  3
     #define noteE    4
     #define noteF    5
     #define noteFis  6
     #define noteG    7
     #define noteGis  8
     #define noteA    9
     #define noteB   10
     #define noteH   11
     #define noteC2  12
     #define noteCis2 13
     #define noteD2    14
     #define noteDis2  15
     #define noteE2    16
     #define noteF2   17
     #define noteFis2 18
     #define noteG2   19
     #define noteGis2 20
     #define noteA2  21
     #define noteB2  22
     #define noteH2  23
     #define noteC3  24
     
     #define CDUR  0
     #define AMOLL 1
     #define FDUR  2
     #define DMOLL 3
     #define GDUR  4
     #define EMOLL 5

 #define    CisDUR      10
     #define BbMOLL  11
     #define FisDUR  12
     #define DisMOLL 13
     #define GisDUR  14
     #define FMOLL   15
     #define EisMOLL 15
     #define HDim   16



unsigned char signal[12000000];
// mit vorgespeicherten Sinustabellen laeuft es schneller
float sintab[128];

void sininit(void)
{
 float value;
 value=-6.4;

 while ( value < 6.4 )
 {
  sintab[(int)((value+6.4)*10.0)]=sin(value);
  value+=0.1;
 }

}

unsigned char *stages;

int harmonchord(int note, int chord)
{
	
	//printf(">>%d %d<<\n",note,chord),getch()=='x'? exit(0) : 0;
int chordtested=0;

while ( chordtested<16)
{

	chordtested++;
	
	//goto *chordtested;

#define NOTECHORD(A,AN,B,BN) ((A==AN&&B==BN)||(A==BN&&B==AN))
	
if ( strstr(stages,"6")!=NULL)
{

if (NOTECHORD(note,AMOLL,chord,AMOLL)) return AMOLL;	
 if (NOTECHORD(note,AMOLL,chord,HDim)) return -1;	
 if (NOTECHORD(note,AMOLL,chord,CDUR)) return AMOLL;	
 if (NOTECHORD(note,AMOLL,chord,DMOLL)) return DMOLL;	
 if (NOTECHORD(note,AMOLL,chord,EMOLL)) return AMOLL;	
 if (NOTECHORD(note,AMOLL,chord,FDUR)) return FDUR;	
 if (NOTECHORD(note,AMOLL,chord,GDUR)) return HDim;	
}  else if (note==AMOLL||chord==AMOLL){ note=chord=AMOLL; continue;}
 
if ( strstr(stages,"7")!=NULL)
{

 if (NOTECHORD(note,HDim,chord,AMOLL)) return -1;	
 if (NOTECHORD(note,HDim,chord,HDim)) return HDim;	
 if (NOTECHORD(note,HDim,chord,CDUR)) return -1;	
 if (NOTECHORD(note,HDim,chord,DMOLL)) return GDUR;	
 if (NOTECHORD(note,HDim,chord,EMOLL)) return EMOLL;	
 if (NOTECHORD(note,HDim,chord,FDUR)) return HDim;	
 if (NOTECHORD(note,HDim,chord,GDUR)) return GDUR;	
}  else if (note==HDim||chord==HDim){ note=chord=HDim; continue;}
 
 if ( strstr(stages,"1")!=NULL)
{
 if (NOTECHORD(note,CDUR,chord,AMOLL)) return CDUR;	
 if (NOTECHORD(note,CDUR,chord,HDim)) return -1;	
 if (NOTECHORD(note,CDUR,chord,CDUR)) return CDUR;	
 if (NOTECHORD(note,CDUR,chord,DMOLL)) return -1;	
 if (NOTECHORD(note,CDUR,chord,EMOLL)) return CDUR;	
 if (NOTECHORD(note,CDUR,chord,FDUR)) return FDUR;	
 if (NOTECHORD(note,CDUR,chord,GDUR)) return CDUR;	
} else if (note==CDUR||chord==CDUR){ note=chord=CDUR; continue;}
 
 if ( strstr(stages,"2")!=NULL)
{

 if (NOTECHORD(note,DMOLL,chord,AMOLL)) return DMOLL;	
 if (NOTECHORD(note,DMOLL,chord,HDim)) return HDim;	
 if (NOTECHORD(note,DMOLL,chord,CDUR)) return -1;	
 if (NOTECHORD(note,DMOLL,chord,DMOLL)) return DMOLL;	
 if (NOTECHORD(note,DMOLL,chord,EMOLL)) return -1;	
 if (NOTECHORD(note,DMOLL,chord,FDUR)) return DMOLL;	
 if (NOTECHORD(note,DMOLL,chord,GDUR)) return GDUR;	
} else if (note==DMOLL||chord==DMOLL){ note=chord=DMOLL; continue;}

if ( strstr(stages,"3")!=NULL)
{
 
 if (NOTECHORD(note,EMOLL,chord,AMOLL)) return AMOLL;	
 if (NOTECHORD(note,EMOLL,chord,HDim)) return HDim;	
 if (NOTECHORD(note,EMOLL,chord,CDUR)) return CDUR;	
 if (NOTECHORD(note,EMOLL,chord,DMOLL)) return -1;	
 if (NOTECHORD(note,EMOLL,chord,EMOLL)) return EMOLL;	
 if (NOTECHORD(note,EMOLL,chord,FDUR)) return -1;	
 if (NOTECHORD(note,EMOLL,chord,GDUR)) return GDUR;	
} else if (note==EMOLL||chord==EMOLL){ note=chord=EMOLL; continue;}
 
if ( strstr(stages,"4")!=NULL)
{
  //printf("binda %d %d\n",note,chord),getch()=='x' ? exit(0) : 0 ;
 if (NOTECHORD(note,FDUR,chord,AMOLL)) return FDUR;	
 if (NOTECHORD(note,FDUR,chord,HDim)) return HDim;	
 if (NOTECHORD(note,FDUR,chord,CDUR)) return FDUR;	
 if (NOTECHORD(note,FDUR,chord,DMOLL)) return FDUR;	
 if (NOTECHORD(note,FDUR,chord,EMOLL)) return -1;	
 if (NOTECHORD(note,FDUR,chord,FDUR)) return FDUR;	
 if (NOTECHORD(note,FDUR,chord,GDUR)) return -1;	
} else if (note==FDUR||chord==FDUR){ note=chord=FDUR; continue;}

 
if ( strstr(stages,"5")!=NULL)
{ 

 if (NOTECHORD(note,GDUR,chord,AMOLL)) return -1;	
 if (NOTECHORD(note,GDUR,chord,HDim)) return GDUR;	
 if (NOTECHORD(note,GDUR,chord,CDUR)) return CDUR;	
 if (NOTECHORD(note,GDUR,chord,DMOLL)) return GDUR;	
 if (NOTECHORD(note,GDUR,chord,EMOLL)) return CDUR;	
 if (NOTECHORD(note,GDUR,chord,FDUR)) return -1;	
 if (NOTECHORD(note,GDUR,chord,GDUR)) return GDUR;	
 }
 else if ( note==GDUR||chord==GDUR){ note=chord=GDUR; continue;}
 }

 return -1;
 

}

int solvechord(int tone, int tone2)
{
	tone/=4,tone2/=4;
    int msbpos=1, bitshift=-4;
    int tonebuf=tone;
	while (tonebuf > 0 )tonebuf/=2, bitshift++;
     if(bitshift>0)
	 tone>>=bitshift, tone<<=bitshift;
       
       
       tonebuf=tone2;
	    bitshift=-4;
   while (tonebuf > 0 )tonebuf/=2, bitshift++;
     if ( bitshift>0)
	 tone2>>=bitshift, tone2<<=bitshift;

	 
    tone2=solvechord_in(tone2);
	//tone2=solvechord_in(tone);
			
   	switch(tone)
    {
 	     case  (494*2)&0xffff8: return harmonchord(tone2,EMOLL); //h break;
	  case  (466/2)&0xfff8: return -1; //b break;
      case  (440/2)&0xfff8: return harmonchord(tone2,AMOLL);//a break;
      case  (415/2)&0xfff8: return -1; //gis break;
      case  (392/2)&0xfff8: return harmonchord(tone2,GDUR);//g break;
      case  (370/2)&0xfff8: return -1; //fis break;
      case  (349/2)&0xfff8: return harmonchord(tone2,FDUR);//f break;
      case  (330/2)&0xfff8: return harmonchord(tone2,EMOLL);//e break;
      case  (311/2)&0xfff8: return -1; //dis break;
      case  (294/2)&0xfff8: return harmonchord(tone2,DMOLL);//d break;
      case  (277/2)&0xfff8: return -1;//cis break;
      case  (261/2)&0xfff8: return harmonchord(tone2,CDUR);//c break;
   
	     case  (494/4)&0xfffc: return harmonchord(tone2,EMOLL); //h break;
	  case  (466/4)&0xfffc: return -1; //b break;
      case  (440/4)&0xfffc: return harmonchord(tone2,AMOLL);//a break;
      case  (415/4)&0xfffc: return -1; //gis break;
      case  (392/4)&0xfffc: return harmonchord(tone2,GDUR);//g break;
      case  (370/4)&0xfffc: return -1; //fis break;
      case  (349/4)&0xfffc: return harmonchord(tone2,FDUR);//f break;
      case  (330/4)&0xfffc: return harmonchord(tone2,EMOLL);//e break;
      case  (311/4)&0xfffc: return -1; //dis break;
      case  (294/4)&0xfffc: return harmonchord(tone2,DMOLL);//d break;
      case  (277/4)&0xfffc: return -1;//cis break;
      case  (261/4)&0xfffc: return harmonchord(tone2,CDUR);//c break;
   
  //Rundung und Frequenz obacht; fugengift
	
       case  (494/8)&0xfffe: return harmonchord(tone2,EMOLL); //h break;
	  case  (466/8)&0xfffe: return -1; //b break;
      case  (440/8)&0xfffe: return harmonchord(tone2,AMOLL);//a break;
      case  (415/8)&0xfffe: return -1; //gis break;
      case  (392/8)&0xfffe: return harmonchord(tone2,GDUR);//g break;
      case  (370/8)&0xfffe: return -1; //fis break;
      case  (349/8)&0xfffe: return harmonchord(tone2,FDUR);//f break;
      case  (330/8)&0xfffe: return harmonchord(tone2,EMOLL);//e break;
      case  (311/8)&0xfffe: return -1; //dis break;
      case  (294/8)&0xfffe: return harmonchord(tone2,DMOLL);//d break;
      case  (277/8)&0xfffe: return -1;//cis break;
      case  (261/8)&0xfffe: return harmonchord(tone2,CDUR);//c break;
   
      }
	return -1;
	
}

int solvechord_in(int tone)
{
 printf("solvechord_in():%d\n",tone);

	 switch((tone))
	 {
	 	
	     case  (494/2)&0xfff8: return HDim; //h break;
	  case  (466/2)&0xfff8: return -1;; //b break;
      case  (440/2)&0xfff8: return AMOLL;//a break;
      case  (415/2)&0xfff8: return -1; //gis break;
      case  (392/2)&0xfff8: return GDUR;//g break;
      case  (370/2)&0xfff8: return -1; //fis break;
      case  (349/2)&0xfff8: return FDUR;//f break;
      case  (330/2)&0xfff8: return EMOLL;//e break;
      case  (311/2)&0xfff8: return -1; //dis break;
      case  (294/2)&0xfff8: return DMOLL;//d break;
      case  (277/2)&0xfff8: return -1;//cis break;
      case  (261/2)&0xfff8: return CDUR;//c break;
   
	 case  (494/4)&0xfffc: return HDim; //h break;
	  case  (466/4)&0xfffc: return -1; //b break;
      case  (440/4)&0xfffc: return AMOLL;//a break;
      case  (415/4)&0xfffc: return -1; //gis break;
      case  (392/4)&0xfffc: return GDUR;//g break;
      case  (370/4)&0xfffc: return -1; //fis break;
      case  (349/4)&0xfffc: return FDUR;//f break;
      case  (330/4)&0xfffc: return EMOLL;//e break;
      case  (311/4)&0xfffc: return -1; //dis break;
      case  (294/4)&0xfffc: return DMOLL;//d break;
      case  (277/4)&0xfffc: return -1; //cis break;
      case  (261/4)&0xfffc: return CDUR; //c break;
  //Rundung und Frequenz obacht; fugengift
	
      case  (494/8)&0xfffe: return HDim; //h break;
	  case  (466/8)&0xfffe: return -1; //b break;
      case  (440/8)&0xfffe: return AMOLL;//a break;
      case  (415/8)&0xfffe: return -1; //gis break
      case  (392/8)&0xfffe: return GDUR;//g break;
      case  (370/8)&0xfffe: return -1; //fis break;
      case  (349/8)&0xfffe: return FDUR;//f break;
      case  (330/8)&0xfffe: return EMOLL; //e break;
      case  (311/8)&0xfffe: return -1; //dis break;
      case  (294/8)&0xfffe: return DMOLL; //d break;
      case  (277/8)&0xfffe: return -1; //cis break;
      case  (261/8)&0xfffe: return CDUR; //c break;
    
	 
	}
	
	return -1;

}

#define SIN(val) sintab[(int)((val)*10) %64 +63]


float filterin[4000];
unsigned short int output[12000000];

int main(int argc, char*argv[])
{

  FILE *ofile;
  unsigned int long n=0, n2=0, n3=0, filesize, randoff;
  float steigung, frequenz, frequenz_gen, frequenz_ausgeb;
float compfrequenz, smallest,
      biggest, loudest, buf;
      
      int chord[4];
      int tone;
      int tones;  

   float buf_notes[4][16];
    int bufn=0, bufn2=0, bufsel=0;
      int bufmodulocount=0;
      int repsel=0, reprand=0;

  sininit();
  /*
  unsigned char dummy[255];
  strcpy(dummy,argv[1]);
*/
  stages=malloc(sizeof(unsigned char)*255);
  strcpy(stages,argv[1]);
/*
 n=0;
  while (argv[1][n]!='\0')stages[n]=argv[1][n],n++;
*/
  //scanf("%ud",&filesize);
   filesize=12000000;
   
   
  if ( filesize > 12000000)filesize=12000000;
  n=0;
  while ( n < filesize) output[n]=32767,n++;
  n=0;
  while ( n < filesize )
  {
   
  loudest=0.0;
   printf("Beat %d\n",n/4000);
 
    unsigned int chordchoice, chordchoice2, volume;
      scanf("%d %d %d\n",&chordchoice,&chordchoice2, &volume);
      chordchoice=solvechord(chordchoice,chordchoice2);   
       //printf("%d\n",chordchoice),getch()=='x'? exit(0) : 0;

    tones=0;
   
    while ( tones < 3 )
   {
    if ( chordchoice!=-1)
	{
   	
     unsigned int cadence=0;
     int beatcopy=0;
     
	/*
	 if ( rand()%2==0&&cadence==0)chordchoice=AMOLL,cadence++;
	 else
	 if ( rand()%2==0||rand()%2==0&&cadence==1)chordchoice=FDUR,cadence++;
	 else
	 if ( cadence==2&&rand()%2==0)chordchoice=GDUR,cadence++;
	  if ( cadence==3 ) cadence=0;
     */
     
     /*
	 if ( n%320000==0 || n%325000==0|| n%330000==0||n%335000==0||
	      n%340000==0 || n%345000==0|| n%350000==0||n%355000==0
	       ) chordchoice=FDUR, cadence=1;
     if ( n%360000==0 || n%365000==0|| n%370000==0||n%375000==0||
	      n%380000==0 || n%385000==0|| n%390000==0||n%400000==0) chordchoice=GDUR, cadence=1;
     */
     
     if ( n%4000==0 )
     {
	   if ( chordchoice== CDUR)
	   {
	   	chord[0]=noteC;
	   	chord[1]=noteE;
	   	chord[2]=noteG;
	   	chord[3]=noteC2;
	   	
	   }
	   
	   
	   if ( chordchoice== AMOLL)
	   {
	   	chord[0]=noteA;
	   	chord[1]=noteC;
	   	chord[2]=noteE;
	   	chord[3]=noteC2;
	   }
	   
	   
	   
	   if ( chordchoice== FDUR)
	   {
	   	chord[0]=noteF;
	   	chord[1]=noteC;
	   	chord[2]=noteA;
	   	chord[3]=noteC2;
	   }
	   
	   
	   
	   if ( chordchoice== DMOLL)
	   {
	   	chord[0]=noteD;
	   	chord[1]=noteF;
	   	chord[2]=noteA;
	   	chord[3]=noteD2;
	   }
	   
	   
	   if ( chordchoice== GDUR)
	   {
	   	chord[0]=noteG;
	   	chord[1]=noteH;
	   	chord[2]=noteD;
	   	chord[3]=noteG2;
	   }
	   
	   
	   if ( chordchoice== EMOLL)
	   {
	   	chord[0]=noteE;
	   	chord[1]=noteG;
	   	chord[2]=noteH;
	   	chord[3]=noteE2;
	   }
	   
	   
	   if ( chordchoice== HDim)
	   {
	   	chord[0]=noteH;
	   	chord[1]=noteDis;
	   	chord[2]=noteA;
	   	chord[3]=noteH2;
	   }
     
     
     }
     
     tone=chord[rand()%4];
     
	   //typedef struct { unsigned int;} test;
	  
	  if ( tones==3) switch ( rand()%8)
	                {
	                  case 0: tone=noteC2; break;
					  case 1: tone=noteD2; break;
					  case 2: tone=noteE2; break;
					  case 3: tone=noteF2; break;
					  case 4:  tone=noteG2; break;
					  case 5:  tone=noteA2; break;
					  case 6:  tone=noteH2; break;
					  case 7:  tone=noteH; break;
					  case 8:  tone=noteA; break;
					  
	                	
					}
					
	      if ( rand()%1==0)beatcopy=1;
		
		if ( bufmodulocount%64<16&&beatcopy==1)
		{	 
	     buf_notes[0][bufn]=tone, bufn++, bufn%=16;
	     if ( bufn==0) beatcopy=0;
		}
	   else if ( bufmodulocount<32 && beatcopy==1)
	   {
	    buf_notes[1][bufn]=tone, bufn++, bufn%=16;
	     if ( bufn==0) beatcopy=0;
	   }
	   else if ( bufmodulocount<48==2 && beatcopy==1)
	   {
	    buf_notes[2][bufn]=tone, bufn++, bufn%=16;
	     if ( bufn==0) beatcopy=0;
	   }
	   else if ( bufmodulocount<64==3 && beatcopy==1)
	   {
	    buf_notes[3][bufn]=tone, bufn++, bufn%=16;
	     if ( bufn==0) beatcopy=0;
	   }
	   
	    bufmodulocount++;
	    
	    if ( rand()%4==0&& reprand==0) reprand=16, bufsel=rand()%4;
	    if ( reprand>0)tone=buf_notes[repsel][16-reprand],reprand--, printf("dada");

	 switch((tone))
	 {
	 	
	  case (int)24 : compfrequenz=988*2; break;
	  case (int)23: compfrequenz=932*2; break;
      case (int)22: compfrequenz= 880*2; break;
      case (int)21: compfrequenz=830*2; break;
      case (int)20: compfrequenz=784*2; break;
      case (int)19: compfrequenz=740*2; break;
      case (int)18: compfrequenz= 698*2; break;
      case (int)17: compfrequenz=660*2; break;
      case (int)16: compfrequenz=622*2; break;
      case (int)15: compfrequenz=588*2; break;
      case (int)14: compfrequenz=554*2; break;
	  
	  
	  case (int)13: compfrequenz=1046; break;
      case (int)12 : compfrequenz=988; break;
	  case (int)11: compfrequenz=932; break;
      case (int)10: compfrequenz= 880; break;
      case (int)9: compfrequenz=830; break;
      case (int)8: compfrequenz=784; break;
      case (int)7: compfrequenz=740; break;
      case (int)6: compfrequenz= 698; break;
      case (int)5: compfrequenz=660; break;
      case (int)4: compfrequenz=622; break;
      case (int)3: compfrequenz=588; break;
      case (int)2: compfrequenz=554; break;
      case (int)1: compfrequenz=523; break;
      case (int)-1: compfrequenz=0; break;
	/*
      case (int)(523) : compfrequenz=494; break;
	  case (int)(494): compfrequenz=466; break;
      case (int)(466): compfrequenz= 440; break;
      case (int)(440): compfrequenz=415; break;
      case (int)(415): compfrequenz=392; break;
      case (int)(392): compfrequenz=370; break;
      case (int)(370): compfrequenz= 349; break;
      case (int)(349): compfrequenz=330; break;
      case (int)(330): compfrequenz=311; break;
      case (int)(311): compfrequenz=294; break;
      case (int)(294): compfrequenz=277; break;
      case (int)(277): compfrequenz=261; break;
    
	 */
	 
   }
   
   }
   	 
    if ( chordchoice!=-1)
	{	
   frequenz_gen= 3.14 /(44100/compfrequenz);
   frequenz_ausgeb=compfrequenz ;
   
   n2=0;
   
   steigung=(rand()%4 )*100.0;
   
    int randirate=rand()*4000;
   randoff=rand()%4000; // so hallt und klirrt es nicht so arg beim Aneinanderreihen der Bloecke
   while ( n2 < randirate%40000){ 
    output[n2+n]+=(signed int)(SIN((float)n2*frequenz_gen+randoff)*steigung*1.9*(float)((rand()%8)*0.01+0.96));
    n2++;
   }
  }
  /*
   n2=0;
   if ( n%8000==0)
   {
    randoff=rand()%1000;
    while ( n2 < 4000){ 
      output[n2+n+4000]+=(float)(SIN(n2*(3.14 /(44100/440.0))))*300*(float)((4000-n2)/2000*(rand()%10)*0.01)*20.0*((volume+90)/100);
     if(rand()%2==0)output[n2+n+4000]+=(float)(SIN((float)n2*(3.14 /(44100/450.0))))*300*(float)((4000-n2)/2000*(rand()%10)*0.01)*20.0;
     n2++;
    }
   }
*/

   //printf("%d:%d\n", (int)compfrequenz*2, (int)(steigung)*1);
  
    //printf("%d:%d\n", (int)frequenz_ausgeb, (int)steigung);
  
  tones++;
 }

  
  
  n+=4000; // naechster Block
  
  
  }


 n=0;
  //while ( n < filesize) output[n]=output[n] /* +signal[n]*/,n++;
 
  ofile=fopen(".\\music.raw", "wb");
  fwrite( output, sizeof(unsigned char), filesize*2, ofile);
  fclose(ofile);

}
