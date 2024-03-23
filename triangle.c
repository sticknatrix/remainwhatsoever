int main(void)
{

unsigned char screen[160][120];
  int x,y;
  
 struct
 {
  float x,y,z;

  } points[3];

  float pixbuf[100000][3];

  float deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;
  float countx1, countx2;

 int n=0, n2=0;
 float pixelx,pixely,pixelz;
 float pixelxs,pixelys,pixelzs;

 float targetx,targety,targetz;
  float target2x, target2y, target2z;
  float slopea, slopeb;
  float buf;

   float xzerr=0.0,yzerr=0.0;
while (1)
{
 n2=0;
  //xzerr+=1.0;
  points[0].x=10.0;//+xzerr;
  points[0].y=10.0;
  points[0].z=10.0;


  points[1].x=10.0;
  points[1].y=40.0;
  points[1].z=10.0;

  points[2].x=40.0;
  points[2].y=20.0;
  points[2].z=10.0;









/// Verfahren beginnt hier ///

  pixelxs=points[0].x;
  pixelys=points[0].y;
  pixelzs=points[0].z;



    target2x=points[1].x;
    target2y=points[1].y;
    target2z=points[1].z;

  if ( target2x-pixelxs == 0.0 )pixelxs+=0.1;
deltay1=(target2y-pixelys)/fabs((target2x-pixelxs));
    if ( target2x-pixelxs == 0.0 )pixelxs+=0.1;
 deltaz1=(target2z-pixelzs)/fabs((target2x-pixelxs));
 
  countx1= target2x-pixelxs > 0.0 ? 1.0 : -1.0;
  if ( fabs(deltay1) > fabs(deltaz1) )
  {
  	if ( deltay1 > 1.0 ) deltaz1/=deltay1, countx1/=deltay1, deltay1=1.0;
    else
    if ( deltay1 < -1.0 ) deltaz1/=-deltay1, countx1/=-deltay1, deltay1=-1.0;
  }
  else
  if ( fabs(deltaz1) > fabs(deltay1) )
  {
  	if ( deltaz1 > 1.0 ) deltay1/=deltaz1, countx1/=deltaz1, deltaz1=1.0;
    else
 	if ( deltaz1 < -1.0 ) deltay1/=-deltaz1, countx1/=-deltaz1, deltaz1=-1.0;
  
  }

do
 { 
  	
 	 pixelx=pixelxs;
     pixely=pixelys;
     pixelz=pixelzs;

  
    //if ( target2y-pixelys == 0.0 )pixelys+=0.001;   
   
    pixelxs+=countx1;
    pixelys+=deltay1;  
    pixelzs+=deltaz1;
  
  
  
    targetx=points[2].x;
    targety=points[2].y;
    targetz=points[2].z;


   if ( targetx-pixelx == 0.0 )pixelx+=0.1;
deltay2=(targety-pixely)/fabs((targetx-pixelx));
    if ( targetx-pixelx == 0.0 )pixelx+=0.1;
 deltaz2=(targetz-pixelz)/fabs((targetx-pixelx));

  
  countx2= targetx-pixelx > 0.0 ? 1.0 : -1.0;
  if ( fabs(deltay2) > fabs(deltaz2) )
  {
    if ( deltay2 > 1.0 ) deltaz2/=deltay2,  countx2/=deltay2, deltay2=1.0;
    else
    if ( deltay2 < -1.0 ) deltaz2/=-deltay2,  countx2/=-deltay2, deltay2=-1.0;
  
  }
  else
  if ( fabs(deltaz2) > fabs(deltay2) )
  {
    
    if ( deltaz2 > 1.0 ) deltay2/=deltaz2, countx2/=deltaz2, deltaz2=1.0;
    else
   if ( deltaz2 < -1.0 ) deltay2/=-deltaz2, countx2/=-deltaz2, deltaz2=-1.0;
   
   }
  
  
   //printf("Starte an %f %f %f\n", pixelxs, pixelys, pixelzs), getch();
  
 do
  {
    pixelx+=countx2;
	pixely+=deltay2;
	pixelz+=deltaz2; 
   
    pixbuf[n2][0]=pixelx,
    pixbuf[n2][1]=pixely,
    pixbuf[n2][2]=pixelz,
    pixbuf[n2][3]=217; 
   n2++; 
 
   //printf("%f %f %f\n", pixelx, pixely, pixelz);
   //getch();

 
 } while ( fabs (pixelx - targetx )+ fabs(pixely-targety)+fabs(pixelz-targetz) > 3 );
 


}while( fabs ( pixelys-target2y )+fabs ( pixelxs-target2x )+fabs ( pixelzs-target2z ) > 3  );


 
system("cls\n");
  
  y=0;
  while ( y < 90)
   {
   	x=0;
   	while ( x < 160)
   	{
   		screen[x][y]=' ';
   		x++;
	   }
   	
   	y++;
   }
  
  n=0;
  while ( n < n2)
  {
	  if ( pixbuf[n][0] > 0 && pixbuf[n][0] < 160 && pixbuf[n][1] > 0 && pixbuf[n][1] < 90) screen[  (int)pixbuf[n][0]  ] [  (int)pixbuf[n][1]  ]=176;
    n++;	
  }
  
  y=0;
  while ( y < 90)
   {
   	x=0;
   	while ( x < 160)
   	{
   		printf("%c%c%c%c",screen[x][y], screen[x+1][y], screen[x+2][y], screen[x+3][y]);
   		x+=4;
	   }
   	printf("\n");
   	y++;
   }
  

}

}
