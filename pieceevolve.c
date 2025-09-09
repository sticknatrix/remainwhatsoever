/*
type or console redirect 20 times time x y z particle number(1-4)
when time is 0, it are the starting coordinates, otherwise, it are the coordinates to be checked at time point t
pieces are tried out that should cause these conditions
*/


typedef
struct
{
 signed char tearline[5*5*5];
 float eval;	
}pointsevolve;

pointsevolve points[20];

#define TRAEGSCHWELLE 0.01
#define SCHEITEL 3.0
#define LADUNG 1.0
#define MAXKRAFT 4.0
#define ELEK 0
#define PROT 1
#define TIMESLICE 100.0

typedef
  struct
{
 float x,y,z;
 float xv,yv,zv;
 float xv_old, yv_old,zv_old ;
   int elec_or_prot;
  signed int type;//0 bedeutet Leerstelle
} particle;

struct
{
 int t,x,y,z,num;

}impactcheck[20];

#define TZAHL (5*5*5+4)


void sort(void)
{
	
	int n,n2;
  unsigned char swap[125];
   float evalswap;
   int arr_add;
 n2=1;
 while ( n2<19)
 {
  n=19;
  while ( n>=n2)
  {
	if ( points[n-1].eval<points[n].eval)
    {
  	//printf("binda"),getchar();
    //	printf("vertausche %f mit %f %d\n",tiles[n-1].eval,tiles[n].eval,n);
	 evalswap=points[n-1].eval;
     points[n-1].eval=points[n].eval;
     points[n].eval=evalswap;
  	 arr_add=0;
  	 while(arr_add<125)
  	 {
  	   swap[arr_add]=points[(n-1)].tearline[arr_add];
	   points[n-1].tearline[arr_add]=points[n].tearline[arr_add];
	   points[n].tearline[arr_add]=swap[arr_add];
	  arr_add++;
     }
    }
  	  n--;	
   }
 	n2++;
 }
 
 

}

void recombine(void)
{
 int n=10;
 while (n<15)
 {
   int n2;
   n2=0;
   while ( n2 <125)points[n].tearline[n2]=points[rand()%20].tearline[n2],n2++;
   points[n].eval=0;
  n++;
 }
	
	
}

void mutate(void)
{
	 int n=15;
 while (n<20)
 {
   int n2;
   n2=0;
   while ( n2 <125)points[n].tearline[n2]=rand()%2-1,n2++;
   points[n].eval=0;
  n++;
 }
	

}

void ffill(unsigned char *space, int x, int y, int z, int num)
{
 if ( space[x+y*5+z*5*5]==0)space[x+y*5+z*5*5]=num;
  else return;
 if ( x > 0) ffill(space,x-1,y,z,num);
 if ( x < 4) ffill(space,x+1,y,z,num);
  if ( y > 0) ffill(space,x,y-1,z,num);
  if ( y < 4) ffill(space,x,y+1,z,num);
  /*if ( z > 0) ffill(space,x,y,z-1,num);
  if ( z < 4) ffill(space,x,y,z+1,num);*/
}

int simrun(int entry)
{
	int n,n2;
 particle teilchen[5*5*5+4];
 float x_buf,y_buf,z_buf;
 float power,distance;
 float sign;
  int time=0;
  float eval=0;

unsigned char screen[120][120];
  int x,y,z,shapenum=1;
  signed char tearline_work[125];
  n=0;
  while(n<125)tearline_work[n]=points[entry].tearline[n],n++;
  
  z=0;
  while ( z < 1)
  {
    y=0;
    while ( y < 5)
    {
	  x=0;
	  while ( x < 5)
	  {
       if ( (tearline_work[x+y*5+z*5*5]==0))
       {
  	     ffill(tearline_work,x,y,z,shapenum);
			  shapenum++;
       }
       x++;
      }
      y++;
    }
    z++;
	}
	
 n=0;
  while ( n<5*5)
  {
   teilchen[n].type=tearline_work[n%5+((n/5)%5)*5+(n/25)*25];
   if ( teilchen[n].type==-1)teilchen[n].type=0;
    teilchen[n].xv=0,teilchen[n].yv=0,teilchen[n].zv=0;	
     teilchen[n].xv_old=0,teilchen[n].yv_old=0,teilchen[n].zv_old=0;
    
	 teilchen[n].x=n%5*3,
     teilchen[n].y=(n/5)%5*3,
     teilchen[n].z=(n/25)*3;
     
     teilchen[n].elec_or_prot=n%2;
    n++;
  }
  teilchen[125].type=0,teilchen[126].type=0,teilchen[127].type=0,teilchen[128].type=0;
  n=0; 
  while (n<20)
  {
  	 impactcheck[n].num%=4;
	 
	  if ( impactcheck[n].t==0)
  	{
  	 if ( impactcheck[n].num<4)teilchen[5*5*5+impactcheck[n].num].x=impactcheck[n].x,
  	                           teilchen[5*5*5+impactcheck[n].num].y=impactcheck[n].y,
  	                           teilchen[5*5*5+impactcheck[n].num].z=impactcheck[n].z,
  	                           teilchen[5*5*5+impactcheck[n].num].xv=0,
  	                           teilchen[5*5*5+impactcheck[n].num].yv=0,
  	                           teilchen[5*5*5+impactcheck[n].num].zv=0,
  	                           teilchen[5*5*5+impactcheck[n].num].xv_old=0,
  	                           teilchen[5*5*5+impactcheck[n].num].yv_old=0,
  	                           teilchen[5*5*5+impactcheck[n].num].zv_old=0,
  	                           teilchen[5*5*5+impactcheck[n].num].type=127,
  	                           teilchen[5*5*5+impactcheck[n].num].elec_or_prot=PROT;
  	                       	  }
	  n++;
  }

  while(time<100)
 {
  n=0;
 while ( n < TZAHL)
{
	if ( teilchen[n].type==0){n++;continue;}
  n2=0;
  while ( n2 < TZAHL)
  {
  	if ( (n2>=25&&n2<125)||(n>=25&&n<125)){n2++;continue;}
   if ( n2==n||teilchen[n2].type==0||teilchen[n].type==0||fabs(teilchen[n2].x-teilchen[n].x)>100||fabs(teilchen[n2].y-teilchen[n].y)>100) { n2++; continue;}

  x_buf=0, y_buf=0,z_buf=0;
  

  power= sqrt(( teilchen[n].x - teilchen[n2].x )  *
    ( teilchen[n].x  - teilchen[n2].x )+
       ( teilchen[n].y - teilchen[n2].y) *
    ( teilchen[n].y - teilchen[n2].y)+ 
      (teilchen[n].z- teilchen[n2].z) *
       (teilchen[n].z - teilchen[n2].z) );
    distance=power;

   x_buf=(teilchen[n].x-teilchen[n2].x);
   y_buf=(teilchen[n].y-teilchen[n2].y);
   z_buf=(teilchen[n].z-teilchen[n2].z);
 
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   
   power=power-SCHEITEL;
  
  if ( power!= 0)
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
  else power=0.0;

    if ( ((teilchen[n].elec_or_prot==ELEK && teilchen[n2].elec_or_prot==PROT)||
         (teilchen[n].elec_or_prot==PROT && teilchen[n2].elec_or_prot==ELEK))&& (teilchen[n].type==teilchen[n2].type) 

) x_buf*=-1.0, y_buf*=-1.0,z_buf*=-1.0;    
   
          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
          z_buf*=power/distance;

        } else x_buf=0, y_buf=0,z_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power; z_buf*=power;
    }  else x_buf=0, y_buf=0, z_buf=0 ;


 
    teilchen[n].xv+=x_buf, teilchen[n].yv+=y_buf,teilchen[n].zv+=z_buf;
    
    n2++;
  }

  n++;

 }
 
  n=0;
 while (n < TZAHL)
 {
 if (n<25||n>124)
 {
  teilchen[n].x+=teilchen[n].xv/TIMESLICE,
  teilchen[n].y+=teilchen[n].yv/TIMESLICE;
  teilchen[n].z+=teilchen[n].zv/TIMESLICE;
  teilchen[n].xv=(teilchen[n].xv_old*1.0+teilchen[n].xv)/2.0;
   teilchen[n].yv=(teilchen[n].yv_old*1.0+teilchen[n].yv)/2.0;
   teilchen[n].zv=(teilchen[n].zv_old*1.0+teilchen[n].zv)/2.0;
  teilchen[n].xv_old=teilchen[n].xv;
  teilchen[n].yv_old=teilchen[n].yv;
  teilchen[n].zv_old=teilchen[n].zv;
  }
  n++;
  }
  
  n=0;
  while (n<20)
  {
   if ( impactcheck[n].t!=0)
   {   	
   	eval-=fabs(impactcheck[n].t-time)+fabs(impactcheck[n].x-teilchen[impactcheck[n].num+5*5*5].x)+
   	+fabs(impactcheck[n].y-teilchen[impactcheck[n].num+5*5*5].y)+
   	+fabs(impactcheck[n].z-teilchen[impactcheck[n].num+5*5*5].z);
   }
   n++;  	
  }
  
  if ((time)%1000==0/*0*/)
{
 x=0;
while ( x < 100)
{
 y=0;
 while ( y < 43)
{
 screen[x][y]=' ';
 y++;
}
x++;
}

n=0;
while ( n < TZAHL)
{
	if ( n < 25||n>124)
{
  if ( teilchen[n].type!=0&&(teilchen[n].x+10) > 0 && (teilchen[n].x+10) < 100 && (teilchen[n].y+10) > 0 && (teilchen[n].y+10) < 42)
 screen[(int)(teilchen[n].x+10)][(int)(teilchen[n].y+10)]=teilchen[n].type== 127 ? 'X' : teilchen[n].type+0x30;
}
n++;
}



system("cls\n");
  y=0;
 while ( y < 43)
 {
  x=0;
  while ( x < 100)
 {
 printf("%c",screen[x][y] );
  x++;
 }
  printf("\n");
 y++;
 }

}

  
  time++;
 }
 return eval;
}


void init(void)
{
 int n=0;
 while (n<20)
 {
   int n2;
   n2=0;
   while ( n2 <125)points[n].tearline[n2]=rand()%2-1,n2++;
   points[n].eval=0;
  scanf("%d %d %d %d %d\n",&(impactcheck[n].t),&(impactcheck[n].x),&(impactcheck[n].y),&(impactcheck[n].z),&(impactcheck[n].num));
  n++;
 }
	
}

int main(void)
{
	
	init();
 int n=0;
  do
  {
  	n=0;
  	while(n<20)
  	points[n].eval=simrun(n),n++;
  	sort();
  recombine();
  mutate();
  printf("%f\n",points[0].eval);
  } while (!kbhit());

 int x,y;
 
  n=0;
  while ( n < 25)
  {
    if ( points[0].tearline[n]!=-1)printf("O");else printf(" ");
   if ( n%5==0)printf("\n");
   n++;
  }
}
