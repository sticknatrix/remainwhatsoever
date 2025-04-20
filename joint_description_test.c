#include <stdio.h>
#include <math.h>
#include <string.h>

typedef
struct
{
 float x,y;
 float xv,yv,xv_old,yv_old;
 int element_bind;
 int fixate;
} particle;

int num_particles=0;

particle particles_main[100];

typedef
struct
{
 unsigned char name[30];
 unsigned char attach2name[30];
  int corresponding_element;
  int element_attached_on;
 float angle;
 float x,y;
 int move_n;
}attach;

attach attachings[20];
int num_attachings=0;

typedef
struct
{
 unsigned char name[30];//eine Nachzuordnung nach Name, etwa eine Sortierung?!?
 float x,y;
 float angle;
 int fixate;
 signed int connected_to;
 int particle_assigned;
 int seq;
 int current_move;
 int num_phases; 
 int visited;
}element;
int num_elements=0;
int num_elements_with_attachings=0;

element elements[20];

typedef
struct
{
 int len_causes;
 int len_causing_seq;
 int relative_mode;
 unsigned char refpoint_name[30];
 unsigned int ref_element,ref_particle;
  struct
  {
   float x,y;
   int element,seq;
  }checks[20];
  int len_follow;
  int len_follow_seq;
  struct
  {
   float x,y;
   int element,seq,eval;
  }valueups[20];
  int checkpos;
  int has_checked;
  int evalpos;
  int has_evaluated;
  int globalstates[10][5];
}cause_seq;

int num_causeseqs=0;

cause_seq causeseq[5];

int initlinks(void)
{
	int n=0, n2=0;
	int assignments_a=0,assignments_b=0;
	
	n=0;
	while ( n < num_causeseqs)
	{
		if ( causeseq[n].relative_mode==1)
		{
		n2=0;
	 while( strcmp(elements[n2].name,causeseq[n].refpoint_name)!=0)n2++;
	   if ( n2==num_elements)return 1;
		causeseq[n].ref_element=n2;
	   }
	   n++;
	}
	n=0;
	while (n<num_elements)elements[n].connected_to=-1,n++;
	n=0;
 while ( n2 < num_elements)
 {
  n=0;
  while ( n < num_attachings)
  {
   if ( strcmp(attachings[n].name,elements[n2].name)==0)
   {
    attachings[n].corresponding_element=n2,assignments_a++;
   	int n3=0;
   	n3=0;
   	while ( strcmp(elements[n3].name,attachings[n].attach2name)!=0&&n3<num_attachings)n3++;
   	if ( n3<num_attachings)
   	{
   	  elements[n2].connected_to=n3;
	   }
   }
   if ( strcmp(attachings[n].attach2name,elements[n2].name)==0)attachings[n].element_attached_on=n2,assignments_b++;
    n++;
 	
	 }	
   n2++;
  }
  
  if ( assignments_a<num_attachings||assignments_b<num_attachings)
  {
  	printf("Verweise unvollstaendig.");
   return 1;
  }
  n=0;
  while ( n<num_elements)
  {
   n2=0;
   while ( n2<num_attachings)
   {
   if (strcmp(elements[n].name,attachings[n2].name)==0)
	{
	 num_elements_with_attachings++;
	 break;
    }
   
   	n2++;
   }
   n++;
  }
  return 0;
}

int lookup_joint( int element_bound, float *x_add, float *y_add, signed int move, particle particles[100])
{
 int n=0;
 while ((attachings[n].corresponding_element!=element_bound || attachings[n].move_n!=elements[attachings[n].corresponding_element].current_move)&&n < num_attachings)n++;
 if ( n== num_attachings)return 1 ;
 int addelem;
 int n2=0;
  n2=attachings[n].element_attached_on;
 addelem=n2;
 //printf(">>%f<<",particles[0].x);
 
 n2=0;
  while((attachings[n2].move_n!=attachings[n].move_n+move||attachings[n2].corresponding_element!=element_bound)&&n2<num_attachings)n2++;
 
 //printf("habe bewegung gefunden %d %d %d ",attachings[n2].move_n,attachings[n].move_n,move);
  if (n2==num_attachings)n2=n;
 
 float rotation_add;
 rotation_add=-elements[attachings[n2].element_attached_on].angle;
 if (elements[attachings[n2].element_attached_on].fixate==1) rotation_add=0.0;
 
 *x_add=0*cos(rotation_add+attachings[n2].angle)-attachings[n2].y*sin(rotation_add+attachings[n2].angle);
 *y_add=0*sin(rotation_add+attachings[n2].angle)+attachings[n2].y*cos(rotation_add+attachings[n2].angle);	
 *x_add+=particles[addelem].x;
 *y_add+=particles[addelem].y;
 //printf("moechte um Winkel %f drehen bei %s dranverbundenes Element ist mit %f %s",attachings[n2].angle,attachings[n2].name,
  //elements[attachings[n2].element_attached_on].angle,elements[attachings[n2].element_attached_on].name);getchar();
 //printf("%d soll an Stelle %f %f mit %f %f und Winkel %f\n",attachings[n2].corresponding_element,particles[addelem].x,particles[addelem].y,attachings[n2].x,attachings[n2].y,rotation_add+attachings[n2].angle);getchar();
 elements[attachings[n2].corresponding_element].current_move=attachings[n2].move_n;
  return 0;
}

int evalpos_bak[5], has_checked_bak[5],checkpos_bak[5];

void backupsequencepos(void)
{
	int n=0;
	n=0;
	while ( n < num_causeseqs)
	{
	 evalpos_bak[n]=causeseq[n].evalpos;
	 checkpos_bak[n]=causeseq[n].checkpos;
	 has_checked_bak[n]=causeseq[n].has_checked;
     n++;
    }    
}

void setbacksequence(void)
{
	int n=0;
	n=0;
	while ( n < num_causeseqs)
	{
	 causeseq[n].evalpos=evalpos_bak[n];
	 causeseq[n].checkpos=checkpos_bak[n];
	 causeseq[n].has_checked=has_checked_bak[n];
    n++;
   } 
}


signed int eval_sequence (particle particles[100])
{
	signed int eval;
	eval=0;
  int n=0,n2=0,n3=0;
while (n<num_causeseqs)
{ 
  if ( causeseq[n].evalpos==causeseq[n].len_follow_seq)causeseq[n].has_checked=0,causeseq[n].evalpos=0;
  if ( causeseq[n].checkpos==causeseq[n].len_causing_seq)causeseq[n].has_checked=1,causeseq[n].checkpos=0;
  
   n2=0;
   while ( n2<causeseq[n].len_follow)
   {
   if ( causeseq[n].has_checked==1)
  {
    	n3=0;
  if ( causeseq[n].valueups[n2].seq==causeseq[n].evalpos)
  {
  	if ( causeseq[n].relative_mode==0)
  	{
   if ( fabs(particles[causeseq[n].valueups[n2].element].x-causeseq[n].valueups[n2].x)<3&&
         fabs(particles[causeseq[n].valueups[n2].element].y-causeseq[n].valueups[n2].y)<3 )
   		eval+=causeseq[n].valueups[n2].eval;
    }
    else
	if ( causeseq[n].relative_mode==1) 
    {
      if ( fabs(particles[causeseq[n].valueups[n2].element].x-causeseq[n].valueups[n2].x-particles[causeseq[n].ref_element].x)<3&&
         fabs(particles[causeseq[n].valueups[n2].element].y-causeseq[n].valueups[n2].y-particles[causeseq[n].ref_element].y)<3 )
   		eval+=causeseq[n].valueups[n2].eval;
	}
   }
  }
   n2++;
 }
  
  n2=0;
 while ( n2<causeseq[n].len_causes)
  {
 	n3=0;
  if ( causeseq[n].checks[n2].seq==causeseq[n].checkpos)
  {
  	if ( causeseq[n].relative_mode==0)
  	{
   if ( (fabs(particles[causeseq[n].checks[n2].element].x-causeseq[n].checks[n2].x)>3||
         fabs(particles[causeseq[n].checks[n2].element].y-causeseq[n].checks[n2].y)>3 )&&causeseq[n].checks[n2].seq==causeseq[n].checkpos)
   		causeseq[n].checkpos=0;
    }
    else if ( causeseq[n].relative_mode==1)
    {
   if ( (fabs(particles[causeseq[n].checks[n2].element].x-causeseq[n].checks[n2].x-particles[causeseq[n].ref_element].x)>3||
         fabs(particles[causeseq[n].checks[n2].element].y-causeseq[n].checks[n2].y-particles[causeseq[n].ref_element].y)>3 )&&
		 causeseq[n].checks[n2].seq==causeseq[n].checkpos)
   		causeseq[n].checkpos=0;
    	
	}
    
  }
   n2++;
   }
   

 if ( causeseq[n].has_checked==0) causeseq[n].checkpos++;
 else causeseq[n].evalpos++;
  n++; 
  
 }
  return eval;
}


int joints_backup[20][5];
int joints_backup_best[20][5];
void setjoints(int what, int rec_depth)
{
 int n=0;
 if ( what==0)
 {
  while (n < num_elements) joints_backup[n][rec_depth]=elements[n].current_move,n++;
 }else
 if ( what==1)
 {
  while (n < num_elements) elements[n].current_move=joints_backup[n][rec_depth],n++; 	
 }
 else
 if ( what==2)
 {
 while (n < num_elements) joints_backup_best[n][rec_depth]=elements[n].current_move,n++; 	
 }
 else
 if ( what==3)
 {
 while (n < num_elements) elements[n].current_move=joints_backup_best[n][rec_depth],n++; 	
 }
	
	
}


int backtrack(particle particles_call[100], int depthsteps)
{
	signed int retcode=-10000,bestret,eval=0;
 signed int movements[21]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};	
  int nm=0;
  int n=0,n2=0,n3=0;
   int iter;


particle particles[100];
 n=0;
 while ( n < 100)particles[n].element_bind=particles_call[n].element_bind, particles[n].fixate=particles_call[n].fixate, particles[n].x=particles_call[n].x,particles[n].y=particles_call[n].y,particles[n].xv=particles_call[n].xv,particles[n].yv=particles_call[n].yv,n++;
int move_iterations=1,buf=num_elements_with_attachings;

while ( buf>0)move_iterations*=3,buf--;
 
#define LADUNG 10.0
#define MAXKRAFT 3.0
#define SCHEITEL 1.0
#define TIMESLICE 50.0
#define TRAEGSCHWELLE 0.25

particle particles_bak[100];
float x_buf,y_buf,power,distance,sign;

if ( depthsteps>4)return 0;

while ( move_iterations>0)
{
 if ( depthsteps!=-1)setjoints(0,depthsteps);

n3=0;
while ( n3 < num_elements_with_attachings)
{
n=0;
while ( n < num_particles)
{
    if ( elements[n].connected_to!=-1)
  elements[n].angle=atan2( particles[elements[n].connected_to].x-particles[n].x,particles[elements[n].connected_to].y-particles[n].y);
    else
    {
     int n4=0;	
      n4=0;
     while (n4 < num_elements)
     {
      if(elements[n4].connected_to==n)
	   elements[n4].angle+=atan2( particles[elements[n4].particle_assigned].xv-particles[n].xv,particles[elements[n4].particle_assigned].yv-particles[n].yv)/
	    num_elements_with_attachings; //irgendein Drehimpuls halt?!?haengt auch von der Masse ab?!? der Drehimpuls wuerde nur durch Gelenke weitergegeben,
	   //sonst waere ein Punkt ja rund?!?
	  n4++;
     }
	}
  n++;
}
n=0;
n2=0;
while ( n < num_particles)
{	

 if ( elements[n].connected_to!=-1)
   if ( lookup_joint(particles[n].element_bind,&x_buf,&y_buf,depthsteps!=-1 ?movements[n2]:0,particles)==0 )
 {
 	 x_buf=(x_buf-particles[n].x)/1.0;
 	y_buf=(y_buf-particles[n].y)/1.0;
	//nach aussen jedes Kollisionsverhalten vorspiegelbar, aber Gesamtimpuls bleibt erhalten?!?
   //"modelliert" man es jetzt halt so, als gaebe es nur Stoesse und keine Zuege und Selbstauskuehlung?!??	 
    particles[n].x+=x_buf,
    particles[n].y+=y_buf;
   n2++;
  }
 //printf("%d %f %f\n",movements[n],particles[n].xv,particles[n].yv);getchar();
 n++;
}
n3++;
}
 n=0;
 while ( n < 100)particles_bak[n].x=particles[n].x,particles_bak[n].y=particles[n].y,particles_bak[n].xv=particles[n].xv,particles_bak[n].yv=particles[n].yv,n++;
 n3=0;
 while(n3<10)
 {
    n=0;
 while ( n < num_particles)
{


  n2=0;
  while ( n2 < num_particles)
  {
   if ( n2==n ) { n2++; continue;}

  x_buf=0, y_buf=0;
  

  power=
   sqrt ( 
    ( particles[n2].x - particles[n].x )  *
    ( particles[n2].x  - particles[n].x )+
       ( particles[n2].y - particles[n].y) *
    ( particles[n2].y - particles[n].y) );
  
    distance=power;

   x_buf=(particles[n2].x-particles[n].x);
   y_buf=(particles[n2].y-particles[n].y);


   if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   
    power=power-SCHEITEL;
  
  
  if ( power!= 0)
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
  else power=0.0;
  
    if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
    if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
    }  else x_buf=0, y_buf=0 ;


  particles[n2].xv+=x_buf,
  particles[n2].yv+=y_buf;
  n2++;
 }
 n++;
}

n=0;
while ( n < num_particles)
{
 if ( particles[n].fixate==0)
  {
  particles[n].x+=particles[n].xv/TIMESLICE;
  particles[n].y+=particles[n].yv/TIMESLICE;
 }
 n++;
}

n=0;
int n4=0;

while ( n < num_particles)
{
 n4=rand()%num_particles;
    if ( elements[particles[n4].element_bind].connected_to!=-1)
   {
    particles[elements[particles[n4].element_bind].connected_to].xv_old=particles[n4].xv_old=(particles[elements[particles[n4].element_bind].connected_to].xv+
    particles[n].xv)/2.0;
    particles[elements[particles[n4].element_bind].connected_to].yv_old=particles[n4].yv_old=(particles[elements[particles[n4].element_bind].connected_to].yv+
    particles[n].yv)/2.0; // cool out internal
   particles[elements[particles[n4].element_bind].connected_to].yv=(particles[elements[particles[n4].element_bind].connected_to].yv_old+
   particles[elements[particles[n4].element_bind].connected_to].yv*9.0)/10.0;
   particles[elements[particles[n4].element_bind].connected_to].xv=(particles[elements[particles[n4].element_bind].connected_to].xv_old+
   particles[elements[particles[n4].element_bind].connected_to].xv*9.0)/10.0;
   particles[n4].xv=(particles[n4].xv*9.0+particles[n4].xv_old)/10.0;
   particles[n4].yv=(particles[n4].yv*9.0+particles[n4].yv_old)/10.0;
   }
   n++;
}

 n3++;
}
if ( depthsteps==-1)
{
 n=0;
 while (n<num_particles)
 {
  particles_main[n].x=particles[n].x;
  particles_main[n].y=particles[n].y;
  particles_main[n].xv=particles[n].xv,
  particles_main[n].yv=particles[n].yv;	
   n++;
 }	

}
if ( depthsteps==-1) return 0;
backupsequencepos();
eval=eval_sequence(particles);
#define EVAL_LIMIT -100
if ( eval>EVAL_LIMIT)
bestret=backtrack(particles,depthsteps+1);
setbacksequence();
if ( bestret+eval>retcode)
{
//if ( bestret+eval > 0 &&depthsteps==0) printf("%d",eval),getchar();
 retcode=bestret+eval;
  setjoints(2,depthsteps);
}
setjoints(1,depthsteps);
 n=0;
 while ( n < 100)particles[n].x=particles_bak[n].x,particles[n].y=particles_bak[n].y,particles[n].xv=particles_bak[n].xv,particles[n].yv=particles_bak[n].yv,n++;

  iter=1;
  while (iter==1)
  {
   if ( movements[nm]<1)movements[nm]++,iter=0,nm=0;
   else
   {
    movements[nm]=-1;
    nm++;
   }
  }
 move_iterations--;
 
   int screen[120][50];
	int x,y;  
  /*
  y=0;
   while ( y < 50)
   {
   	x=0;
   	while(x<120)
   	{
     screen[x][y]=' ';
   	 x++;	
	}
    y++;	
   }
   n=0;
   while (n<num_elements)
   {
   	if ( particles[n].x>=0&&particles[n].x<120&&particles[n].y>=0&&particles[n].y<50)screen[(int)particles[n].x][(int)particles[n].y]=n+0x30<0x3a? n+0x30: n-10+0x41;
   	n++;
   }
   
   system("cls");
    y=0;
   while ( y < 50)
   {
   	x=0;
   	while(x<120)
   	{
     printf("%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y]);
   	 x+=4;	
	}
	printf("\n");
   	y++;
   }
  */
 }

return retcode;
}


 struct
 {
  unsigned char text[30];
  int type;
  unsigned char identify[255];
 }token[50];


int num_tokens;

void remove_tokens( int position, int amount_removed )
{

 while( position+ amount_removed <= num_tokens )
 {
  token[position].type=token[position+amount_removed].type;
  strcpy( token[position].text, token[position+amount_removed].text );
 strcpy( token[position].identify, token[position+amount_removed].identify );

  position++;
 }

 num_tokens-=amount_removed;

}

int main(void)
{
 FILE *definitions;
 definitions=fopen("definitions.txt","rb");
 
 struct
 {
  unsigned char name[30];
  int state;
 }modestack[8];
 int stackcount=0;
 
 int n=0;
 int n2=0;
 while (!feof(definitions))
 {
  n=0;
  while ( !feof(definitions) )
  {
  	fscanf(definitions,"%s",token[n].text);
   if ( strcmp(token[n].text,"limb")==0)token[n].type='l';
   else
   if ( strcmp(token[n].text,"setxy")==0)token[n].type='x';
   else
   if ( strcmp(token[n].text,"fixate")==0)token[n].type='0';
   else
   if ( strcmp(token[n].text,"attach")==0)token[n].type='a';
   else
   if ( strcmp(token[n].text,"rotation")==0)token[n].type='r';
   else
   if ( strcmp(token[n].text,"coordinates")==0)token[n].type='c';
   else
   if ( strcmp(token[n].text,"movement")==0)token[n].type='m';
   else
   if ( strcmp(token[n].text,"sequence")==0)token[n].type='s';
   else
   if ( strcmp(token[n].text,"cause")==0)token[n].type='i';
   else
   if ( strcmp(token[n].text,"is")==0)token[n].type='C';
   else
   if ( strcmp(token[n].text,"relative")==0)token[n].type='o';
   else
   if ( strcmp(token[n].text,"len")==0)token[n].type='e';
   else
   if ( strcmp(token[n].text,"followup")==0)token[n].type='f';
   else
   if ( strcmp(token[n].text,"next")==0)token[n].type='N';
   else
   if ( isalpha(token[n].text[0]))
   {
   	n2=1;
  	#define ISNUM(x) (x >= 0x30&&x<=0x39)
  	
  	while ( (isalpha(token[n].text[n2])||ISNUM(token[n].text[n2]))&&token[n].text[n2]!='\0')n2++;
  	if ( token[n].text[n2]!='\0'){printf("lexikalischer Fehler.");return 1 ;}
  	token[n].type='v';
  	
   }
   else
   if ( ISNUM(token[n].text[0])||token[n].text[0]=='-')
   {
  	n2=1;
  	while ( ISNUM(token[n].text[n2])&&token[n].text[n2]!='\0')n2++;
  	if ( token[n].text[n2]!='\0'){printf("lexikalischer Fehler.");return 1 ;}
  	token[n].type='n';
   }
   else
   if ( strcmp(token[n].text,";")==0){token[n].type=';';break;}
   n++;
  }
  int right_limit,left_limit;
  right_limit=n-1;
  num_tokens=n;
  left_limit=0;
  right_limit=n;
  int position;
  int reduction_type;
  
  reduction_type=0;
  
  while(reduction_type!=';')
  {
   printf("Stackcount ist %d %s",stackcount,token[left_limit].text);
  reduction_type=';';
 
  position=left_limit;
  while ( position+2<right_limit)
  {
   if (token[position].type=='c'&&token[position+1].type=='n'&&token[position+2].type=='n')
   {
  	 right_limit=position+2;
  	 left_limit=position;
  	 reduction_type='c';
  	 break;
 	}  
   position++;
   }
 
  position=left_limit;
  while ( position+1<right_limit)
  {
   if (token[position].type=='r'&&token[position+1].type=='n')
   {
  	 right_limit=position+1;
  	 left_limit=position;
  	 reduction_type='r';
  	 break;
 	}  
   position++;
   }
 
  
  position=left_limit;
  while ( position+2<right_limit)
  {
   if (token[position].type=='a'&&token[position+1].type=='v'&&token[position+2].type=='v'&&token[position+3].type=='n')
   {
  	 right_limit=position+3;
  	 left_limit=position;
  	 reduction_type='a';
  	 break;
 	}  
   position++;
   }
  
  position=left_limit;
  while ( position<right_limit)
  {
   if (token[position].type=='N')
   {
  	 right_limit=position;
  	 left_limit=position;
  	 reduction_type='N';
  	 break;
 	}  
   position++;
   }
 position=left_limit;
  while ( position+1<right_limit)
  {
   if (token[position].type=='e'&&token[position+1].type=='n')
   {
  	 right_limit=position+1;
  	 left_limit=position;
  	 reduction_type='e';
  	 break;
 	}  
   position++;
   }
 
 position=left_limit;
  while ( position+1<right_limit)
  {
   if (token[position].type=='o'&&token[position+1].type=='v')
   {
  	 right_limit=position+1;
  	 left_limit=position;
  	 reduction_type='o';
  	 break;
 	}  
   position++;
   }
    
      position=left_limit;
  while ( position<right_limit)
  {
   if (token[position].type=='i')
   {
  	 right_limit=position;
  	 left_limit=position;
  	 reduction_type='i';
  	 break;
 	}  
   position++;
   }
   
         position=left_limit;
  while ( position<right_limit)
  {
   if (token[position].type=='f')
   {
  	 right_limit=position;
  	 left_limit=position;
  	 reduction_type='f';
  	 break;
 	}  
   position++;
   }

         position=left_limit;
  while ( position<right_limit)
  {
   if (token[position].type=='s')
   {
  	 right_limit=position;
  	 left_limit=position;
  	 reduction_type='s';
  	 break;
 	}  
   position++;
   }

  
  position=left_limit;
  while ( position+1<right_limit)
  {
  if (token[position].type=='l'&&token[position+1].type=='v')
   {
  	 right_limit=position+1;
  	 left_limit=position;
  	 reduction_type='l';
  	 break;
 	}  
   position++;
   }

  position=left_limit;
  while ( position+1<right_limit)
  {
  if (token[position].type=='0'&&token[position+1].type=='v')
   {
  	 right_limit=position+1;
  	 left_limit=position;
  	 reduction_type='0';
  	 break;
 	}  
   position++;
  }
     position=left_limit;
  while ( position+3<right_limit)
  {
   if (token[position].type=='x' &&token[position+1].type=='v'&&token[position+2].type=='n'&&token[position+3].type=='n')
   {
  	 right_limit=position+3;
  	 left_limit=position;
  	 reduction_type='x';
  	 break;
 	}  
   position++;
   }

     position=left_limit;
  while ( position+5<right_limit)
  {
   if (token[position].type=='C' &&token[position+1].type=='v'&&token[position+2].type=='n'&&token[position+3].type=='n'&&token[position+4].type=='n'
        &&token[position+5].type=='n')
   {
  	 right_limit=position+5;
  	 left_limit=position;
  	 reduction_type='F';
  	 break;
 	}  
   position++;
   }
  if ( reduction_type!='F')
  {
     position=left_limit;
  while ( position+4<right_limit)
  {
   if (token[position].type=='C' &&token[position+1].type=='v'&&token[position+2].type=='n'&&token[position+3].type=='n'&&token[position+4].type=='n')
   {
  	 right_limit=position+4;
  	 left_limit=position;
  	 reduction_type='C';
  	 break;
 	}  
   position++;
   }
 }

if ( stackcount>0)
{
 
 if ( (modestack[stackcount-1].state==2&&stackcount==1)||modestack[stackcount-1].state==3||modestack[stackcount-1].state==4)
 {
   if ( reduction_type=='i')
   {
   	if ( modestack[stackcount-1].state!=2)return 1;
   	modestack[stackcount].state=3;
   	stackcount++;
   	token[left_limit].type='R';
	 }
   else
   if ( reduction_type=='f')
   {
   	if ( modestack[stackcount-1].state!=3)return 1;
   	modestack[stackcount].state=4;
   	stackcount++;
	token[left_limit].type='R';
   	 }
     else
   if ( reduction_type=='e')
   {
   	if ( modestack[stackcount-1].state==3)causeseq[num_causeseqs].len_causing_seq=atoi(token[left_limit+1].text);
   	else
	   if ( modestack[stackcount-1].state==4)causeseq[num_causeseqs].len_follow_seq=atoi(token[left_limit+1].text);
   	else return 1;
	remove_tokens(left_limit,1);
	  token[left_limit].type='R';
	 }
    else
   if ( reduction_type=='N')
   {
   	if ( modestack[stackcount-1].state!=4)return 1;
   	   stackcount-=3;
	  token[left_limit].type='R';
	  num_causeseqs++;
	 }
	    else
   if ( reduction_type=='o')
   {
   	if ( modestack[stackcount-1].state!=2)return 1;
   	   causeseq[num_causeseqs].relative_mode=1;
	
	strcpy(causeseq[num_causeseqs].refpoint_name,token[left_limit+1].text);
	remove_tokens(left_limit,1);
	  token[left_limit].type='R';
	
	 }
   else	 
	 if ( reduction_type=='C')
   {
   	int count;
   	count=0;
   	while(count<num_elements)
   	{
    	if ( strcmp(elements[count].name,token[left_limit+1].text)==0 )break;
   		count++;
	   }
	   if ( count==num_elements)return 1;
	   
	   if ( modestack[stackcount-1].state==3)
	   {
	   	causeseq[num_causeseqs].checks[causeseq[num_causeseqs].len_causes].element=count;
	   	causeseq[num_causeseqs].checks[causeseq[num_causeseqs].len_causes].x=atoi(token[left_limit+2].text);
	   	causeseq[num_causeseqs].checks[causeseq[num_causeseqs].len_causes].y=atoi(token[left_limit+3].text);
	   	causeseq[num_causeseqs].checks[causeseq[num_causeseqs].len_causes].seq=atoi(token[left_limit+4].text);
	   	causeseq[num_causeseqs].len_causes++;
	   }
	   else return 1;
	
	   remove_tokens(left_limit,4);
	   token[left_limit].type='R';
    }
	else
	if ( reduction_type=='F')
    {
	int count;
   	count=0;
   	while(count<num_elements)
   	{
 		if ( strcmp(elements[count].name,token[left_limit+1].text)==0 )break;
   		count++;
	   }
	   if ( count==num_elements)return 1;
	   if ( modestack[stackcount-1].state==4)
	   {
	    causeseq[num_causeseqs].valueups[causeseq[num_causeseqs].len_follow].element=count;
	   	causeseq[num_causeseqs].valueups[causeseq[num_causeseqs].len_follow].x=atoi(token[left_limit+2].text);
	   	causeseq[num_causeseqs].valueups[causeseq[num_causeseqs].len_follow].y=atoi(token[left_limit+3].text);
	   	causeseq[num_causeseqs].valueups[causeseq[num_causeseqs].len_follow].seq=atoi(token[left_limit+4].text);
	   	causeseq[num_causeseqs].valueups[causeseq[num_causeseqs].len_follow].eval=atoi(token[left_limit+5].text);
	   	causeseq[num_causeseqs].len_follow++;	    	
       }
       else return 1;
	   remove_tokens(left_limit,5);
	   token[left_limit].type='R';
   }   
	 else if ( reduction_type==';')
	 {
	 	if ( num_tokens>2) return 1;
	 	else if ( num_tokens==2&&token[left_limit].type!='R')return 1;
	 	
	 }
	 else return 1;

 }
 else
 if(modestack[stackcount-1].state==1&&stackcount==1)
 {
  if ( reduction_type=='c')
  {
    attachings[num_attachings].x=atoi(token[left_limit+1].text);
    attachings[num_attachings].y=atoi(token[left_limit+2].text);
  	remove_tokens(left_limit,2);
	  token[left_limit].type='R';
   }
   else
  if ( reduction_type=='r')
  {
    attachings[num_attachings].angle=(float)atoi(token[left_limit+1].text)/10.0;
     remove_tokens(left_limit,1);
	token[left_limit].type='R';
   }
   else
  if ( reduction_type=='N')
  {
  	token[left_limit].type='R';
  	num_attachings++;
	  stackcount--;
   }
   else if ( reduction_type==';')
   {
   	if ( num_tokens==1||(num_tokens==2&&token[left_limit].type=='R')); else return 1;
   }
   else return 1;
 }
}
else
{
if ( reduction_type=='l')
   {
   	int finddouble=0;
   	while ( finddouble<num_elements)
   	{
   		if ( strcmp(elements[finddouble].name,token[left_limit+1].text)==0)
   		{
   	 	 printf("Bezeichner bereits verwendet.");
   	 	 return 1;
   	    }
   	    finddouble++;
	   }
    strcpy(elements[num_elements].name,token[left_limit+1].text);
    elements[num_elements].fixate=0;
	num_elements++;
	remove_tokens(left_limit,1);
	token[left_limit].type='R';
   	
   }
   else
if ( reduction_type=='0')
   {
   	int finddouble=0;
   	while ( finddouble<num_elements)
   	{
   		if ( strcmp(elements[finddouble].name,token[left_limit+1].text)==0)break;
   	    finddouble++;
	   }
	   if ( finddouble==num_elements){printf("Bezeichner nicht vorhanden.");return 1 ;}
    elements[finddouble].fixate=1;
	remove_tokens(left_limit,1);
	token[left_limit].type='R';
   }
   else
   if ( reduction_type=='a')
   {
    if ( strcmp(token[left_limit+1].text,token[left_limit+2].text)==0)
    {
     printf("Selbstreferenz bei Verbindung.");return 1 ;		
	}
	strcpy(attachings[num_attachings].name,token[left_limit+1].text);
	strcpy(attachings[num_attachings].attach2name,token[left_limit+2].text);
	attachings[num_attachings].move_n=atoi(token[left_limit+3].text);
	remove_tokens(left_limit,3);
	token[left_limit].type='R';
	if ( stackcount > 0) return 1;
	modestack[stackcount].state=1;
	stackcount++;
   }
   else
   if ( reduction_type=='x')
   {
   	int count;
   	count=0;
   	while(count<num_elements)
   	{
   		if ( strcmp(elements[count].name,token[left_limit+1].text)==0 )
   		{
   		   	elements[count].x=atoi(token[left_limit+2].text);
   			elements[count].y=atoi(token[left_limit+3].text);
   			
		   }
   		count++;
	   }
	   remove_tokens(left_limit,3);
	   token[left_limit].type='R';
   }
   else
   if ( reduction_type=='N')break;
   else
   if ( reduction_type=='s')
   {
   	modestack[stackcount].state=2;
	causeseq[num_causeseqs].relative_mode=0;
	causeseq[num_causeseqs].len_causes=0,
	causeseq[num_causeseqs].len_follow=0;
	token[left_limit].type='R';
   	stackcount++;
   }
   else
   if ( reduction_type==';')
   {
   	if ( num_tokens==1||(num_tokens==2&&token[left_limit].type=='R'));else return 1;
   }
   else
   return 1;

}

}

 }
fclose(definitions);

   n=0;
   while (n<num_elements)
   {
   	particles_main[n].element_bind=n;
   	elements[n].particle_assigned=n;
   	elements[n].current_move=0;//eventuell Initialisierung erlauben?!?
	elements[n].angle=0.0;
	if ( n < 5)causeseq[n].evalpos=0,causeseq[n].checkpos,causeseq[n].has_checked=0;
	particles_main[n].x=elements[n].x,
   	particles_main[n].y=elements[n].y;
   	particles_main[n].fixate=elements[n].fixate;
   	particles_main[n].xv=0,particles_main[n].yv=0;
   	n++;
   }
   num_particles=num_elements;
 
 printf("binda");
 if (initlinks()==1 ) return 1;
 
  unsigned char screen[120][50];
  int x=0,y=0;
  int moveseq[21];
 while (1)
 {
 backtrack(particles_main, 0);
   /*setbacksequence();*/printf(">>binda<<");
   setjoints(3,0);backtrack(particles_main,-1);eval_sequence(particles_main);
   
   y=0;
   while ( y < 50)
   {
   	x=0;
   	while(x<120)
   	{
     screen[x][y]=' ';
   	 x++;	
	}
    y++;	
   }
   
   n=0;
   while (n<num_elements)
   {
   	if ( particles_main[n].x>=0&&particles_main[n].x<120&&particles_main[n].y>=0&&particles_main[n].y<50)screen[(int)particles_main[n].x][(int)particles_main[n].y]=n+0x30<0x3a? n+0x30: n-10+0x41;
   	n++;
   }
   
   system("cls");
    y=0;
   while ( y < 50)
   {
   	x=0;
   	while(x<120)
   	{
     printf("%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y]);
   	 x+=4;	
	}
	printf("\n");
   	y++;
   }
 
 }

}
/*
for testing
limb hand ;
limb finger ;
fixate hand ;
setxy finger 5 10 ;
attach finger hand 0 ;
coordinates 3 6 ;
rotation -10 ;
next ;

setxy hand 0 20 ;

limb ball ;
setxy ball 12 14 ;

attach finger hand 1 ;
coordinates 3 6 ;
rotation -20 ;
next ;

limb fingertip ;
setxy fingertip 25 40 ;

attach fingertip finger 0 ;
coordinates 4 6 ;
rotation 31 ;
next ;

attach fingertip finger 1 ;
coordinates 4 4 ;
rotation 41 ;
next ;

sequence ;
cause ;
followup ;
len 2 ;
is ball 12 11 0 100 ;
is fingertip 10 14 1 300 ;
next ;

next ;
*/
