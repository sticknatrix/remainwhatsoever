/*
die Freaks und Programmier-Honks haben eine als noch Uralte Glaettungs-Raffungs-Methode wieder ausgegraben?!
*/

#include <stdio.h>
#include <fcntl.h>

float sintab[128];
float costab[128];

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

void cosinit(void)
{
 float value;
 value=-6.4;

 while ( value < 6.4 )
 {
  costab[(int)((value+6.4)*10.0)]=cos(value);
  value+=0.1;
 }

}

#define SIN(val) sintab[(int)((val)*10) %64 +63]
#define COS(val) costab[(int)((val)*10) %64 +63]



int main(void)
{
 struct
  {
    float x_start,y_start,z_start;
	float x_end,y_end,z_end;
	unsigned int shade;
	int runlen;
   }*lines;
   
    FILE *RLE;
   
    FILE *output=fopen(".\\outrleln.raw","wb");//unlikely that a file was named like this, anyway watch first better
    int n=0;
    float y_angl=0.0;
   
   lines=malloc(100000*sizeof(*lines));
	
	
	sininit(), cosinit();
	RLE=fopen("RunlngText","rb");
	
	while(!feof(RLE))
	{
	 fscanf(RLE, "%f %f %d %d\n",&(lines[n].x_start),&(lines[n].y_start),&lines[n].runlen,&lines[n].shade);		
	  lines[n].z_start=50,lines[n].z_end=50;
	  lines[n].x_end=lines[n].x_start+lines[n].runlen;
	  lines[n].y_end=lines[n].y_start;
	  lines[n].z_start=50, lines[n].z_end=50;
	  n++;
	}
	
	int num_lines=n;
	
     float pixbuf[2][3];
     unsigned char screen[160][120], screen_z[160][120];
	
	float xshift=60,zshift=-100,yshift=20;
	int n2;
	while (1)
	{
 	 int x,y;
 	 y=0;
	 while ( y < 120 )
	 {
	  x=0;
	  while (x < 160)
	  {
	  	screen[x][y]=' ';
	  	screen_z[x][y]=100000;
	  	x++;
	  }
	  y++;
	 }
		n2=0;
		while ( n2 < num_lines)
		{
		 pixbuf[0][0]=lines[n2].x_start,
		 pixbuf[0][1]=lines[n2].y_start,
		 pixbuf[0][2]=lines[n2].z_start,
		 pixbuf[1][0]=lines[n2].x_end,
		 pixbuf[1][1]=lines[n2].y_end,
		 pixbuf[1][2]=lines[n2].z_end;
		 
           n=0;
        while ( n < 2)
        {
         float x_turned, y_turned, z_turned;
        	  x_turned= +(pixbuf[(int)n][0]-xshift)*COS(y_angl)+(pixbuf[(int)n][2]-zshift)*SIN(y_angl);
          z_turned= -(pixbuf[(int)n][0]-xshift)*SIN(y_angl)+(pixbuf[(int)n][2]-zshift)*COS(y_angl);

            pixbuf[(int)n][0]=x_turned+xshift;
            pixbuf[(int)n][1]-=yshift;
           pixbuf[(int)n][2]=z_turned+zshift;
         n++;
       }

#define SCALE 120.0
#define CAMX (xshift)
#define CAMY (yshift)
#define CAMZ (zshift)

  n=0;

 pixbuf[0][0]=SCALE*(pixbuf[0][0]-CAMX)/(pixbuf[0][2]-zshift)/1+80,pixbuf[0][1]=SCALE*(pixbuf[0][1]-CAMY)/(pixbuf[0][2]-zshift)/1+40,
 pixbuf[1][0]=SCALE*(pixbuf[1][0]-CAMX)/(pixbuf[1][2]-zshift)/1+80,pixbuf[1][1]=SCALE*(pixbuf[1][1]-CAMY)/(pixbuf[1][2]-zshift)/1+40;
 	
	
	float xsloper, ysloper;
	
	  float xdraw, ydraw, z_lap;
	
	     xdraw=pixbuf[0][0], ydraw=pixbuf[0][1];
	     float deltax, deltay;
	     deltax=pixbuf[0][0]-pixbuf[1][0];
	     if ( deltax==0)deltax+=0.1;
	    deltay=pixbuf[0][1]-pixbuf[1][1];
	    if ( deltay==0)deltay+=0.1;
	    
		 ysloper=(pixbuf[0][1]-pixbuf[1][1])/(deltax);
       	  xsloper=(pixbuf[0][0]-pixbuf[1][0])/(deltay);
	 	
	 	//if ( ysloper==0)ysloper=0.1;if(xsloper==0)xsloper=0.1;
	     if ( fabs(ysloper)>=fabs(xsloper))
		  {
		   if ( deltay >= 1.0 ) { xsloper/=ysloper; ysloper=-1.0;/*xsloper/=xsloper;*/ }
		   else if ( deltay < 1.0){ xsloper/=ysloper; ysloper=1.0; }
	     }
		else
	     if ( fabs(xsloper)>=fabs(ysloper))
		  {
		   if ( deltax >= 1.0 ) { ysloper/=xsloper; xsloper=-1.0;/*xsloper/=xsloper;*/ }
		   else if ( deltax < 1.0){ ysloper/=xsloper; xsloper=1.0; }
	     }

	
	z_lap=(pixbuf[0][2]-pixbuf[1][2])/(pixbuf[0][0]-pixbuf[1][0]);
	
	float slopx, slopy;
	slopx=0, slopy=0;

 
	unsigned char greyscale;
	greyscale=lines[n2].shade;
	/*
	  if ( greyscale < 32) greyscale=' ';
        else
      if ( greyscale < 64) greyscale='.';
        else
        if ( greyscale < 96 ) greyscale=':';
         else
        if ( greyscale < 128 ) greyscale=176;
         else
        if ( greyscale < 196) greyscale=177;
         else greyscale=219;*/
        
	    float slopz;
	    slopz=pixbuf[0][2]-zshift;
	    //printf("%f %f %f %f\n",xdraw+slopx,pixbuf[1][0],ydraw+slopy,pixbuf[1][1] ),getchar();
	
	float topbehind;
	topbehind=xsloper>=0? 1.0 : -1.0;
	if ( topbehind==-1.0)z_lap*=1.0;
     
	 while ( ((xdraw+slopx)-pixbuf[1][0]>0&&topbehind==-1.0) || ((xdraw+slopx)-pixbuf[1][0]<0&&topbehind==1.0) )
	  {	
	  //printf("%d %f %f %f %f\n",n2, xdraw+slopx,pixbuf[1][0],ydraw+slopy,pixbuf[1][1] ),getchar();
	  
	 //printf("%f %f %f %f\n",xdraw+slopx,ydraw+slopy,pixbuf[1][0],pixbuf[1][1]);
	if ( slopz>0)
	if ( xdraw+slopx>=0 && xdraw+slopx<160&&ydraw+slopy>=0&&ydraw+slopy<90)
	 screen[(int)(xdraw+slopx)][(int)(ydraw+slopy)]=greyscale,
	 screen[(int)(xdraw+slopx)][(int)(ydraw+1+slopy)]=greyscale,
	 //screen[(int)(xdraw+slopx)][(int)(ydraw+2+slopy)]=greyscale,
	 screen_z[(int)(xdraw+slopx)][(int)(ydraw+slopy)]=slopz,
	 screen_z[(int)(xdraw+slopx)][(int)(ydraw+1+slopy)]=slopz;
	 //screen_z[(int)(xdraw+slopx)][(int)(ydraw+2+slopy)]=slopz;
	 slopx+=xsloper;
	 slopy+=ysloper;
	  slopz+=z_lap;
	}
	
   n2++;
  }
  /*
    	if ( kbhit())
    	{
    		unsigned char c;
    		c=getch();
    		   if(c=='u' ) xshift-= -20*SIN(-y_angl), zshift-= -20*COS(-y_angl);
    		   if(c=='j' )  xshift+= -20*SIN(-y_angl), zshift+= -20*COS(-y_angl);
    		   if(c=='h' ) y_angl+=0.4;
    		   if(c=='k' ) y_angl-=0.4;
    		   if(c=='a' )yshift-=10;
    		   if(c=='y' ) yshift+=10;
    }*/
    
    y_angl+=0.2;
    if (y_angl>=0.5)y_angl=-0.5, yshift+=20;
    /*if ( yshift>=50)yshift=-20, xshift+=20;*/
    if ( yshift>=80)yshift=-20, zshift-=50;
    if ( zshift<=-300) { fclose(output);return;}
	
    //system("cls\n");
    y=0;
    while (y < 90 )
    {
     x=0;
     while ( x < 160 /*158*/ )
     {
      //printf("%c%c%c%c%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y],screen[x+3][y],screen[x+4][y],screen[x+5][y],screen[x+6][y],screen[x+7]);
      //x+=8;
      x++;
      fputc(screen[x][y],output);
     }
     y++;
     //printf("\n");
    }        

	
		
  }

}

