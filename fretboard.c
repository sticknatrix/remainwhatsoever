/* enter chord notes, type h for b */

int fretboard[6];


  int chord[4]={-256,-256,-256,-256};

int num_voicings=0;
int voicingdupe[6*100000];

int hasalready (int *voicing, unsigned int *nomute)
{
	int n=0;
  while ( n < num_voicings*6)
  	{
  		if ( (voicingdupe[n]==voicing[0]||nomute[0]==0)&&
		  (voicingdupe[n+1]==voicing[1]||nomute[1]==0)&&
		  (voicingdupe[n+2]==voicing[2]||nomute[2]==0)&&
		  (voicingdupe[n+3]==voicing[3]||nomute[3]==0)&&
		  (voicingdupe[n+4]==voicing[4]||nomute[4]==0)&&
		  (voicingdupe[n+5]==voicing[5]||nomute[5]==0))
		  return 1;
	   n+=6;
	  }
	  voicingdupe[num_voicings*6]=voicing[0],
		  voicingdupe[num_voicings*6+1]=voicing[1],
		  voicingdupe[num_voicings*6+2]=voicing[2],
		  voicingdupe[num_voicings*6+3]=voicing[3],
		  voicingdupe[num_voicings*6+4]=voicing[4],
		  voicingdupe[num_voicings*6+5]=voicing[5];
	  num_voicings++;
	  return 0;
	
}


void fret(int *allowed_old, int rec_depth)
{
 int allowed[6];
 int n=0;
 int c1,c2,c3;
 int xpos,ypos,barreopen;
 ypos=0;
 while ( ypos<6)
 {
   xpos=0;
   while ( xpos<12)
   {
   	barreopen=0;
   	while ( barreopen<3)
   	{
   		int donesomething;
   		donesomething=0;
   		n=0;
   		while ( n<6)allowed[n]=allowed_old[n],n++;
            int widest;
            widest=256;
			n=0;
            while ( n < 6){if ( allowed[n]<widest&&allowed[n]!=0)widest=allowed[n];n++;}
 
 #define NOSTRINGBEFORE(x) ((allowed[0]==x||allowed[0]==0)&&(allowed[1]==x||allowed[1]==0)&&(allowed[2]==x||allowed[2]==0)\
                           &&(allowed[3]==x||allowed[3]==0)&&(allowed[4]==x||allowed[4]==0)&&(allowed[5]==x||allowed[5]==0))
 
   		if ( barreopen==0)
   		{
   		 //if ( allowed[ypos]==xpos-1||(allowed[ypos]==xpos-2&&rec_depth==3)||allowed[ypos]==0)allowed[ypos]=xpos,donesomething=1;	
   		if ( (xpos<widest+4&&xpos>=widest)||widest==256)allowed[ypos]=xpos,donesomething=1;	
   			
		}
		else if ( barreopen==1)
		{
			int barcount;
		 if ( widest==xpos-1||widest==xpos-2||widest==256)
		 {
		 	donesomething=1;
		 	barcount=ypos;
		 	while ( barcount<6)
		  allowed[barcount]=xpos,barcount++;
		}
    	}
		n=0;
		int isinit;
		isinit=0;
		unsigned int notmute[6];
		while (n<6)notmute[n]=0,n++;
		n=0;
		while (n<4)
		{
			int isinit_flag;
			isinit_flag=0;
		 int n2=0;
		 n2=0;
		 if ( chord[n]!=-256)
		 while( n2<6)
		 {
		  if ( chord[n]==allowed[n2]+fretboard[n2]||chord[n]+12==allowed[n2]+fretboard[n2]){isinit_flag=1,notmute[n2]=1;}
		  n2++;
		 } else isinit+=1;
		 if ( isinit_flag==1)isinit++;
		 n++;
		}
		if ( isinit==4&&!hasalready(allowed,notmute))
		{
		 if ( notmute[5]==1)printf("E|-%d-|\n",allowed[5]);
		 else printf("E|---|\n");
		if ( notmute[4]==1)printf("H|-%d-|\n",allowed[4]);
		 else printf("H|---|\n");
		if ( notmute[3]==1)printf("G|-%d-|\n",allowed[3]);
		 else printf("G|---|\n");
		if ( notmute[2]==1)printf("D|-%d-|\n",allowed[2]);
		 else printf("D|---|\n");
		if ( notmute[1]==1)printf("A|-%d-|\n",allowed[1]);
		 else printf("A|---|\n");
		if ( notmute[0]==1)printf("E|-%d-|\n\n",allowed[0]);
		 else printf("E|---|\n\n");
			
		}
		if ( rec_depth<3&&donesomething==1)fret(allowed,rec_depth+1);
		
   		barreopen++;
	   }
   	
   	xpos++;
   }
   ypos++;
 }
 
	
}

int main(void)
{
  int state=0;
  unsigned char note;
  int skipscan=0;
  int value;
  int n=0;
  
  
  fretboard[0]=4,fretboard[1]=9,fretboard[2]=2,fretboard[3]=7,fretboard[4]=11,fretboard[5]=4;
  
  
 while (1)
 {
 if ( skipscan==0)scanf("%c",&note);
 skipscan=0;
   if ( state==0)
   {	
	switch(note)
	{
	 case 'c': value=0; break;
	 case 'd': value=2; break;
	case 'e': value=4; break;
	case 'f': value=5; break;
	case 'g': value=7; break;
	case 'a': value=9; break;
	case 'h': value=11; break;
	case 0xd: case 0xa: state=2; skipscan=1;continue;
	default: printf("Fehler");continue;
	}
	state=1;
    }
    else if(state==1)
    {
     if ( note=='#') value++,state=0;
     else if ( note=='b')value--,state=0;
     else state=0,skipscan=1;
    printf("E|-%d------|\n",value-fretboard[5]<0? value+12-fretboard[5]: value-fretboard[5]);
    printf("H|--%d-----|\n",value-fretboard[4]<0? value+12-fretboard[4]: value-fretboard[4]);
    printf("G|---%d----|\n",value-fretboard[3]<0? value+12-fretboard[3]: value-fretboard[3]);
    printf("D|----%d---|\n",value-fretboard[2]<0? value+12-fretboard[2]: value-fretboard[2]);
    printf("A|-----%d--|\n",value-fretboard[1]<0? value+12-fretboard[1]: value-fretboard[1]);
    printf("E|------%d-|\n",value-fretboard[0]<0? value+12-fretboard[0]: value-fretboard[0]);
    printf("\n");
    state=0;
    chord[n]=value;
    n++; if ( n>3)break;
    	
	}
	else if ( state==2)break;
	
  }
	
    int allowed[6];
    n=0;
    while (n<6)allowed[n]=0,n++;
	fret(allowed,0);
	printf("Programm beendet.");
}
