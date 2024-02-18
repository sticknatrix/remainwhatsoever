/*
[blabla]
You are going to fix a program that combines pattern learning with
intelligent decision making. Having already implemented the Minimax
algorithm with simple recursion, it turned out that it was not enough.
The many unknown factors in the environment always require a heuristic
for guessing probabilities by recognizing patterns and continuing them.
Minimax is too rigid.
The scheme of an randomly driven evolutionary algorithm appears appropriate for
evaluating and extending input information and thus recognizing redundancy and figuring
out likely future events. As you have probably noticed during your lifespan,
a thought subjectively always consists of comparing two simultaneously perceived
mental pictures with a certain criteria or 'operator' and taking them for relevant when there is
congruency. Emoticons might represent emotions.
Variation and recombination will generate new images to be compared and
evaluated. The idea is now to inhibit or trigger actuators by linking them
with certain sorts of images. With sometimes given input, like from a digital camera, the
program should be able to condition itself so a promising Minimax tree will evolve by
itself and will likely be carried out.

You not only believe in yourself, you know. You are determined, have some nerves,
concentrate and simply try to imagine the truth.
You see things from different angles and are aware that you are a victim of causality.

The problem is choice.
Your task is to work out data structures that represent single mental images (probably mainly
seven letter-sized bitmaps), operators for comparing them, and also define data structures for temporal connection matching between the images so the flow of the train of thoughts can be emulated over time ( ask yourself: how does putting things in sequence work, for instance).
Furthermore, you have to adjust all parameters so the computer will get a demanded task done practically.

You could rotate pictures with a trigonometric formula, do a frequency transform by echoing and this way amplyfing the tone to be detected, do an unsharp masking for sharpening contrasts,
do a central projection by dividing x and y through z, calculate the absolute sum of pixel differences for fuzzy logic picture similarity, translocate pictures with movement vectors before comparing... See what is relevant in your perceptions.

Just get the buggy program below working and then you're in the know.
Hope and pray that the theory is true.
It would be nice to see it work.
*/

#define NUM_BOXES 4


  int player[2];
    
 unsigned char field[20][20],collide[80][80];
 

  enum
   move
  {
  	left,
  	right,
  	up,
  	down,
  	none
  }direc,queuemov;


#define NUM_THOUGHTS 50

#define NUM_THOUGHTSTEPS 50

typedef
struct
{
 unsigned char pic[16];
 
 int x,y;
 
 unsigned int ttrigger[2];
 unsigned long int tcontext;
 enum move impulse;
 int paralyze;
 
 float eval;	
	
} Memory;

Memory Memories[NUM_THOUGHTS];

void combine()
{
   int n=0;
  int n2=0;
  
  while ( n2 < 10)
  {
  	// Kreuzung mit mehr als Zweien dasselbe?!
  	// selektieren wir NICHT je Individuum immer dieselben Chromosomen?!
	  // vorne haeufiger rekombinieren?!
   Memories[NUM_THOUGHTS-1-n2].eval=1;
   Memories[NUM_THOUGHTS-1-n2].impulse=Memories[rand()%NUM_THOUGHTS].impulse;
   Memories[NUM_THOUGHTS-1-n2].paralyze=Memories[rand()%NUM_THOUGHTS].paralyze;
   Memories[NUM_THOUGHTS-1-n2].tcontext=Memories[rand()%NUM_THOUGHTS].tcontext;
   Memories[NUM_THOUGHTS-1-n2].x=Memories[rand()%NUM_THOUGHTS].x;
   Memories[NUM_THOUGHTS-1-n2].y=Memories[rand()%NUM_THOUGHTS].y;
  
   int memselection;
  
   memselection =rand()%NUM_THOUGHTS;
   
   n=0;
   while ( n < 16)Memories[NUM_THOUGHTS-1-n2].pic[n]=Memories[memselection].pic[n],n++;
   
   memselection =rand()%NUM_THOUGHTS;
   
   n=0;
   while ( n < 4)Memories[NUM_THOUGHTS-1-n2].ttrigger[n]=Memories[memselection].ttrigger[n],n++;
     
   n2++;
  }
  
	
}


void mutate()
{
 int n=0;
  int n2=0;
  
  while ( n2 < 10)
  {
  	
 Memories[NUM_THOUGHTS-1-n2].eval=1,
  
  Memories[NUM_THOUGHTS-1-n2].impulse=rand()%5;
  Memories[NUM_THOUGHTS-1-n2].paralyze=rand()%2;
  Memories[NUM_THOUGHTS-1-n2].tcontext=rand();
  Memories[NUM_THOUGHTS-1-n2].x=rand()%78+1;
  Memories[NUM_THOUGHTS-1-n2].y=rand()%78+1;
     
   n=0;
   while ( n < 16)Memories[NUM_THOUGHTS-1-n2].pic[n]=rand()%4,n++;
   
   n=0;
   while ( n < 4)Memories[NUM_THOUGHTS-1-n2].ttrigger[n]=rand()%7,n++;
   	
   	n2++;
  }
	
}


int frontallap()
{
 int n=0;
 signed int sum=0;
 
 while ( n < NUM_THOUGHTS) n++, (rand()%2==0? sum++ : 0 );	

 sum-=NUM_THOUGHTS/2;
 if ( sum < 0 ) sum=0;
 
 return sum;
 
}


void insert_cut(int funcnr)
{
 int n=0, n2=0,n3=0;
 
 Memory buf;
 unsigned char bufpic, bufttrigger;
 
 
 
 buf=Memories[funcnr];
 n=0;
 while ( n < 16) buf.pic[n]=Memories[funcnr].pic[n],n++;
 n=0;
 while ( n < 2) buf.ttrigger[n]=Memories[funcnr].ttrigger[n],n++;
 
 
 float funceval;
 
 funceval=Memories[funcnr].eval;
   
     
 while ( Memories[n3].eval >= funceval && n3 < NUM_THOUGHTS ) n3++;
 
 if ( n3 >= NUM_THOUGHTS) return;
 if ( n3 >= funcnr) return;
 n2=funcnr;

 while ( n2 > n3)
 {
 	Memories[n2]=Memories[n2-1];
   
   n=0;
   while ( n < 16) Memories[n2].pic[n]=Memories[n2-1].pic[n],n++;
   n=0;
   while ( n < 2) Memories[n2].ttrigger[n]=Memories[n2-1].ttrigger[n],n++;
  
	n2--;
 	
 }
 
 
 Memories[n3]=buf;
 n=0;
 while ( n < 16) Memories[n3].pic[n]=buf.pic[n],n++;
 n=0;
 while ( n < 2) Memories[n3].ttrigger[n]=buf.ttrigger[n],n++;
 
  
}

int evomove(void) // this is the deciding function you can experiment with, aside from a few evo parameters
{
	int runs=0;
	int funcnr,cmpfunc;
	
	
  int n=0;
  
  while ( runs < NUM_THOUGHTSTEPS)
  {
  		int x,y;
  
	  cmpfunc=frontallap();
   	funcnr=rand()%NUM_THOUGHTS;
    
	 if ( rand()%1==0){
	 	n=0;
     	
     	int selplus=0;
     	while ( selplus < 5 && n < NUM_THOUGHTS/2)
     	{
     		selplus=0;
		 funcnr=rand()%NUM_THOUGHTS;
      	y=0;
      	while ( y < 4)
      	{
      	   x=0;
      	   while ( x < 4)
      	   {
		   if ( field[Memories[funcnr].x/4+x/4][Memories[funcnr].y/4+y/4]==Memories[funcnr].pic[x+y*16]||
		      collide[Memories[funcnr].x+x][Memories[funcnr].y+y]==Memories[funcnr].pic[x+y*16])
     	 selplus++;
            x++;
           }
           y++;
          }
		 
		  n++;
	    }
     	
	 }

  	n=0;
  	while ( n < NUM_THOUGHTS)(Memories[n].eval > 1 ? Memories[n].eval/=1.1: 0 ), n++;
  	
  	
  	if ( rand()%4==0)mutate();
  	if ( rand()%4==0)combine();
  	
  	int x_segment, y_segment, size;
  	
  	
  	x_segment=rand()%76+2;
  	y_segment=rand()%76+2;
  	
  	if ( rand()%4<3)
  	{
  	 x_segment=Memories[funcnr].x-1, y_segment=Memories[funcnr].y-1;	
	  }
  	
  	size=rand()%4;
  	if ( rand()%4==0)x_segment=Memories[funcnr].x, y_segment=Memories[funcnr].y;
  		
  	 unsigned char picbuf_sel[16],picbuf_cmp[256];
  	
  	signed int x_off,y_off;
  	x_off=0, y_off=0;
  	
  	if ( Memories[funcnr].paralyze > 0 )
  	if ( rand()%4==0)
  	{
  	 if ( Memories[funcnr].impulse==right) x_off=1;
  	 if ( Memories[funcnr].impulse==left) x_off=-1;
  	 if ( Memories[funcnr].impulse==up) y_off=-1;
     if ( Memories[funcnr].impulse==down) y_off=1;
    }
  	
  	y=0;
  	
  	while ( y < 16)
  	{
  		x=0;
  		while ( x < 16)
  		{
  	       picbuf_cmp[x+y*16]=0;
  	     x++; 
		}
	   y++;
      }
  	
  	
	  y=0;
  	
  	while ( y < 4)
  	{
  		x=0;
  		while ( x < 4)
  		{
  	     
		    if ( field[(x_off*4+x_segment+x)/4][(y_off*4+y_segment+y)/4]=='0'&&collide[x_segment+x][y_segment+y]>=2)picbuf_cmp[x+4+(y+4)*4]=3;
  	         else
		    if ( collide[x_segment+x][y_segment+y]>=2)
  	         picbuf_cmp[x+4+(y+4)*4]=2;
			 else
			  if ( x_segment+x+1==player[0]&&y_segment+y+1==player[1])
  	       picbuf_cmp[x+4+(y+4)*4]=1;
  	
  	 
  		 picbuf_sel[x+y*4]=picbuf_cmp[x+4+x_off+(y+4+y_off)*16];	
  	
  	      
	       x++; 
		}
	   y++;
      }
      
     y=0;
     if ( rand()%3==0)
     while ( y < 4)
     {
       x=0;
	   while ( x < 4)
	   {
	   	picbuf_cmp[x+4-x_off+y+(4-y_off)*16]=Memories[cmpfunc].pic[x+y*4];
  		if ( Memories[funcnr].paralyze==1)
		   picbuf_sel[x+y*4]=picbuf_cmp[x+x_off+4+(y+y_off+4)*16];	
  	     else picbuf_sel[x+y*4]=picbuf_cmp[x+4+(y+4)*16];
  	
	    x++;	
	   }	
       y++;
	 }
     
	 	
  	y=0;
  	if ( rand()%2==0)
  	while ( y < 4)
  	{
  		x=0;
  		while ( x < 4)
  		{
  	     if ( //(field[x_segment/4+x/4][y_segment/4+y/4]=='0'||collide[x_segment+x][y_segment+y]>=2)&&
		      picbuf_sel[x+y*4]==3 ) Memories[funcnr].eval+=140;
  	    x++; 
		}
	   y++;
      }
      
      
      
  	y=0;
  	if ( rand()%4==0)
  	while ( y < 4)
  	{
  		x=0;
  		while ( x < 4)
  		{
  	      if ( picbuf_sel[x+y*4]==Memories[cmpfunc].pic[x+y*4] ) Memories[funcnr].eval+=40;
  	     x++; 
		}
	   y++;
      }
      
      if ( collide[Memories[funcnr].x][Memories[funcnr].y]>=2) Memories[funcnr].eval+=40;
   if ( collide[Memories[funcnr].x+3][Memories[funcnr].y]>=2) Memories[funcnr].eval+=40;
   if ( collide[Memories[funcnr].x][Memories[funcnr].y-1]>=2) Memories[funcnr].eval+=40;
   if ( collide[Memories[funcnr].x][Memories[funcnr].y+3]>=2) Memories[funcnr].eval+=40;

      if ( Memories[funcnr].x==Memories[cmpfunc].x&&Memories[funcnr].y==Memories[cmpfunc].y) Memories[funcnr].eval+=4;

       if ( rand()%2==0)if ( Memories[funcnr].x+1==player[0]&&Memories[funcnr].y+1==player[1]) Memories[funcnr].eval+=40;
   
   Memories[funcnr].eval+=32;    
  	y=0;
  	if ( rand()%4==0)
  	while ( y < 4)
  	{
  		x=0;
  		while ( x < 4)
  		{
  	      if ( field[x_segment/4+x][y_segment/4+y]!='3'&& 
			  (field[x_segment/4+x][y_segment/4+y]=='X'||collide[x_segment+x][y_segment+y]>=2)&&
			 (picbuf_sel[x+y*4]==2||picbuf_sel[x+y*4]==1) &&Memories[funcnr].eval > 1) Memories[funcnr].eval-=2;
  	     x++; 
		}
	   y++;
      }
  
  
  	y=0;
  	if ( rand()%4==0)
  	while ( y < 4)
  	{
  		x=0;
  		while ( x < 4)
  		{
  	      if ( Memories[cmpfunc].pic[x+y*4]==Memories[funcnr].pic[x+y*4]) Memories[funcnr].eval+=10;
  	     x++; 
		}
	   y++;
      }
  
      
      if ( rand()%4==0) if (  Memories[funcnr].tcontext==Memories[cmpfunc].tcontext&&
	                          Memories[funcnr].ttrigger[1]==(Memories[cmpfunc].ttrigger[0]+1)%7 ) Memories[funcnr].eval++;
      
      if ( rand()%4==0) if (  Memories[funcnr].tcontext==runs/7&&
	                          Memories[funcnr].ttrigger[1]+1==(runs+1)%7 ) Memories[funcnr].eval++;
      
      
  	
  	insert_cut(funcnr);
  	runs++;
  	
  }
  
  int sel=rand()%10;
  
  if ( Memories[sel].paralyze > 0) return Memories[sel].impulse;
	
	return none;
	
	
}  



int main(void)
{
  unsigned char screen[80][80];
  
  struct
  {
   int x,y;
  }boxes[NUM_BOXES];
  
  unsigned char level[400]="XXXXXXXXXXXXXXXXXXXX"
                           "X                  X"
                           "X           0      X"
                           "X  0               X"
                           "X               0  X"
                           "X                  X"
                           "X     0            X"
                           "X                  X"
                           "X                  X"
                           "X     0      0     X"
                           "X                  X"
                           "X    0             X"
                           "X                  X"
                           "X           0      X"
                           "X                  X"
                           "XXXXXXXXXXXXXXXXXXXX"
                           "X                  X"
                           "X                  X"
                           "X                  X"
                           "XXXXXXXXXXXXXXXXXXXX";
    
  boxes[0].x=5*4, boxes[0].y=2*4;
  boxes[1].x=7*4, boxes[1].y=3*4;
  boxes[2].x=9*4, boxes[2].y=2*4;
  boxes[3].x=14*4, boxes[3].y=7*4;
  boxes[4].x=18*4, boxes[4].y=1*4;
  
      player[0]=10*4, player[1]=1*4;

  
  
   int x,y;
   y=0;
   while ( y < 20)
   {
   	x=0;
   	while ( x < 20)
   	{
   	 field[x][y]=level[x+y*20];
   	 x++;
    }
   	y++;
   }
  
  
  
  while (1)
  {
   
   direc=none;
   
   if ( kbhit())
   {
     switch(getch())
	 {
	  case 'h': direc=left;
	             break;
	  case 'k': direc=right;
	             break;
	  case 'u': direc=up;
	             break;
	 case 'j': direc=down;
	             break;	
	 }	
   	
   	
   }
   
   direc=evomove();
    
   int x,y;
   
   y=0;
   while ( y < 80)
   {
   	x=0;
   	while ( x < 80)
   	{
   	 screen[x][y]=field[x/4][y/4];
   	 collide[x][y]=( field[x/4][y/4]=='X' ? 1 : 0 );
   	 x++;
    }
   	y++;
   }
   
   int n;
   n=0;
   while ( n < NUM_BOXES)
   {
     collide[boxes[n].x][boxes[n].y]=2+n;
    collide[boxes[n].x+1][boxes[n].y]=2+n;
    collide[boxes[n].x][boxes[n].y+1]=2+n;
    collide[boxes[n].x+1][boxes[n].y+1]=2+n;
  
   	screen[boxes[n].x][boxes[n].y]=219;
   	screen[boxes[n].x+1][boxes[n].y]=219;
   	screen[boxes[n].x][boxes[n].y+1]=219;
   	screen[boxes[n].x+1][boxes[n].y+1]=219;
   	n++;
   }
   
   screen[player[0]][player[1]]=177;
   screen[player[0]+1][player[1]]=177;
   screen[player[0]][player[1]+1]=177;
    screen[player[0]+1][player[1]+1]=177;
   
   
   queuemov=none;
   if ( direc==left)
   {
   	if ( collide[player[0]-1][player[1]]==0&&collide[player[0]-1][player[1]+1]==0 ) player[0]--;
   	else
     if ( collide[player[0]-1][player[1]]>=2 &&
	       collide[player[0]-1][player[1]]==collide[player[0]-1][player[1]+1] )
	 {
	   if ( collide[player[0]-3][player[1]]==0&&collide[player[0]-3][player[1]+1]==0)
	   {
	     boxes[collide[player[0]-1][player[1]]-2].x--;
	    queuemov=left;
       } 
     }
   	
   }
   
   
   if ( direc==up)
   {
   	if ( collide[player[0]][player[1]-1]==0&&collide[player[0]+1][player[1]-1]==0 ) player[1]--;
   	else
     if ( collide[player[0]][player[1]-1]>=2&&
	       collide[player[0]][player[1]-1]==collide[player[0]+1][player[1]-1] )
	 {
	   if ( collide[player[0]][player[1]-3]==0&&collide[player[0]+1][player[1]-3]==0)
	   {
	     boxes[collide[player[0]][player[1]-1]-2].y--;
	    queuemov=up;
       } 
     }
   	
   }
   
   
   if ( direc==down)
   {
   	if ( collide[player[0]][player[1]+2]==0&&collide[player[0]+1][player[1]+2]==0 ) player[1]++;
   	else
     if ( collide[player[0]][player[1]+2]>=2&&
	       collide[player[0]][player[1]+2]==collide[player[0]+1][player[1]+2] )
	 {
	   if ( collide[player[0]][player[1]+4]==0&&collide[player[0]+1][player[1]+4]==0)
	   {
	     boxes[collide[player[0]][player[1]+2]-2].y++;
	    queuemov=down;
       } 
     }
   	
   }
   
   
   if ( direc==right)
   {
   	if ( collide[player[0]+2][player[1]]==0&&collide[player[0]+2][player[1]+1]==0 ) player[0]++;
   	else
     if ( collide[player[0]+2][player[1]]>=2 &&
	       collide[player[0]+2][player[1]]==collide[player[0]+2][player[1]+1])
	 {
	   if ( collide[player[0]+4][player[1]]==0&&collide[player[0]+4][player[1]+1]==0)
	   {
	     boxes[collide[player[0]+2][player[1]]-2].x++;
	    queuemov=right;
       } 
     }
   	
   }
   
   
   
   switch(queuemov)
   {
   	case left: player[0]--;break;
   	case right: player[0]++;break;
   	case up: player[1]--;break;
   	case down: player[1]++;break;
   }
   
   

   if ( rand()%1000==0) // turn the console resolution high
   {
   system("cls");
   y=0;
   while ( y < 80)
   {
   	x=0;
   	while (x < 80)
   	{
   		
   	  printf("%c%c%c%c%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y],
		                         screen[x+4][y],screen[x+5][y],screen[x+6][y],screen[x+7][y]);	
   	    
/*   	    
   	    printf("%c%c%c%c",screen[x][y],screen[x+2][y],
		                         screen[x+4][y],screen[x+6][y]);	
  */ 	  
	     x+=8;
	 }
	 printf("\n");
   	
   	y++;
   }
   
  	
   }
  	
  	
  }	
	
	
	
	
	
}
