#include <math.h>
#include <stdio.h>
#include <stdlib.h>



float sqrtab[1000000];

void sqrinit(void)
{
 int count=0;
 while ( count < 1000000 )
 {
  sqrtab[count]=sqrt((float)count/1000.0 ) ;
  count++;
 }

}


float SQRT(float quad)
{
  return ( sqrt(quad) );
 if ( quad < 0.0 ) quad*=-1.0 ;


 if (quad > 999 ) return ( sqrt(quad) );

 return sqrtab[ (unsigned int)( quad*1000.0) ];

}


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

#define SIN(val) sintab[(int)(val*10) %64 +63]

/*
inline float COS(float value)
{
 signed int sign;
 signed int index;

 if ( value < 0 ) sign=-1;
 else sign=+1;
  index= ((int)( value*10 )%64);
  index+=63;

 return  costab[index];
}

*/

#define COS(val) costab[(int)(val*10) %64 +63]



struct
{
 long unsigned x_next, y_next; /* korrekte Abstaende von Atom 2 und 3 zu Atom 1 */
 unsigned int num_atoms;
 long signed int atoms[64][10]; /* die ersten drei bestimmen den Winkel, zweites x, drittes y,
                                 die restlichen vier sind die Nachbarbindungsatome, letztes Elek or Prot */
 unsigned int num_pixels;
 signed long pixgroup[8192][4]; /* Pixeldarstellung */
 float x_angl, y_angl, z_angl; /* Drehwinkel des Objekts */
 float x,y,z; /* Koordinaten. Ergeben sich aber dann aus den Atomkoordinaten. */
}input_obj[32];

float pixelbuf[231072][4];
unsigned int pixelnum=0;
unsigned int objnum=0;

#define ZOOM 120.0 /* Skalierung der Zentralprojektion */
#define VOXDEPTH 1

int main(void)
{
 FILE *input, *floor;


struct
 {
  float x,y,z;
  float xv,yv,zv;
  float xv_old, yv_old, zv_old;
  int elec_or_prot;

  float distance;
  unsigned int objID;
  signed int bondNum1, bondNum2, bondNum3, bondNum4, bondNum5, bondNum6;
  float nearest;
 }teilchen[2048];
 
 struct
 {
 	float x,y,z;
 } floor_teil[25];
 int floor_cz, floor_cx;
unsigned int ATOMNUM=0;
 int touch=0;

 float power, old_power, x_buf, y_buf, z_buf, x_buf2, y_buf2, z_buf2, sign;
 float cmpadd_x, cmpadd_y;
 float x_cmp, y_cmp, z_cmp;

  signed int vox_x, vox_y, vox_z;

struct
{
 float x,y,z;
 float x_angl, y_angl, z_angl;

} cam; /* die Projektionsflaeche und der Projektionspunkt */

  float average_vecx, average_vecy, average_vecz;
unsigned char floorLmap[512][512];
unsigned char floorHmap[512][512];
unsigned char screen[160][96];
unsigned int zbuf[160][96]; /* vorderster Z-Wert eines projizierten Punkts pro Bildschirmkoordinate. */
long int score=0;
float game_ended=1000;

unsigned char c;

float speed=0.0, driveangl=0.0, xtomid, ztomid;
signed int vavc1, vavc2, vavc3;

signed long int counter=0;
float distsample;
long int old_pix_num;
signed long int counter2=0;
long unsigned int n,n2, n3;
signed int screen_x, screen_y;
int framereduce, framered2;

float x_turned, y_turned, z_turned,
      x_turned2, y_turned2, z_turned2,
      x_turned3, y_turned3, z_turned3;

      sqrinit(), sininit(), cosinit();

input=fopen(".\\3ddata.txt","rb");
floor=fopen(".\\floor.map", "rb");
if ( floor==NULL ) return;
fread(floorHmap, sizeof(unsigned char), 512*512, floor);
fread(floorLmap, sizeof(unsigned char), 512*512, floor);
fclose(floor);

vox_z=0;
while ( vox_z < 512 )
{
 vox_x=0;
 while ( vox_x < 512 )
 {
 
    c=floorLmap[vox_x][vox_z] ;
    if ( c < 64 ) c=' ';
    else
    if ( c < 128 ) c= 176;
    else
    if ( c < 196 ) c= 177;
    else
    if ( c < 255 )c= '@';
    floorLmap[vox_x][vox_z]=c;
    
    floorHmap[vox_x][vox_z]=15-floorHmap[vox_x][vox_z]/2.5; // glatter Boden Test (0)
    //floorHmap[vox_x][vox_z]=0;
    
  vox_x++;
 }
 vox_z++;
}

if ( input==NULL ) return;

   /* die Atomgruppen und die korrespondierenden Pixelgruppen mitsamt relativer
     Positionierung zueinander aus der Eingabedatei laden */

 while ( ! feof(input) )
 {

  fscanf(input,"%d %d", &input_obj[objnum].x_next,&input_obj[objnum].y_next );
  fscanf(input,"%d", &(input_obj[objnum].num_atoms) );
  counter=0;
  while ( counter < input_obj[objnum].num_atoms )
  {
   fscanf(input,"%d %d %d %d %d %d %d %d %d %d", &input_obj[objnum].atoms[(int)counter][0],
                                              &input_obj[objnum].atoms[(int)counter][1],
                                              &input_obj[objnum].atoms[(int)counter][2],


                                      &input_obj[objnum].atoms[(int)counter][3],
                                      &input_obj[objnum].atoms[(int)counter][4],
                                      &input_obj[objnum].atoms[(int)counter][5],
                                      &input_obj[objnum].atoms[(int)counter][6],
                                      &input_obj[objnum].atoms[(int)counter][7],
                                      &input_obj[objnum].atoms[(int)counter][8],  /* Bindungen */
									  &input_obj[objnum].atoms[(int)counter][9]  /* Elek or Prot */
                                        );

   counter++;
  }

  fscanf(input,"%d", &(input_obj[objnum].num_pixels) );
  counter=0;
  while ( counter < input_obj[objnum].num_pixels )
  {
   fscanf(input,"%d %d %d %d", &input_obj[objnum].pixgroup[(int)counter][0],
                               &input_obj[objnum].pixgroup[(int)counter][1],
                               &input_obj[objnum].pixgroup[(int)counter][2],
                               &input_obj[objnum].pixgroup[(int)counter][3] );

     // Kalibrierungsdebug ACHTUNG bei korrekten Daten!!!
        input_obj[objnum].pixgroup[(int)counter][2]-=10;
        input_obj[objnum].pixgroup[(int)counter][0]-=10;
        
        input_obj[objnum].pixgroup[(int)counter][1]-=10; // Hoehenkorrektur 1
	 // bis hier
   counter++;
  }

  objnum++;

 }
fclose(input);
objnum--;

counter=0;
counter2=0;
ATOMNUM=0;

while ( counter2 < objnum ) /* Atomgruppen in die Physiksimulation uebertragen */
{
 counter=0;
 while ( counter < input_obj[counter2].num_atoms )
 {
  teilchen[ATOMNUM].x = input_obj[counter2].atoms[(int)counter][0];
  teilchen[ATOMNUM].y = input_obj[counter2].atoms[(int)counter][1];
  teilchen[ATOMNUM].z = input_obj[counter2].atoms[(int)counter][2];
  teilchen[ATOMNUM].bondNum1=input_obj[counter2].atoms[(int)counter][3];
  teilchen[ATOMNUM].bondNum2=input_obj[counter2].atoms[(int)counter][4];
  teilchen[ATOMNUM].bondNum3=input_obj[counter2].atoms[(int)counter][5];
  teilchen[ATOMNUM].bondNum4=input_obj[counter2].atoms[(int)counter][6];
  teilchen[ATOMNUM].bondNum5=input_obj[counter2].atoms[(int)counter][7];
  teilchen[ATOMNUM].bondNum6=input_obj[counter2].atoms[(int)counter][8];


  teilchen[ATOMNUM].elec_or_prot= input_obj[counter2].atoms[(int)counter][9]; /* Elek or Prot */
  teilchen[ATOMNUM].objID = counter2;

  teilchen[ATOMNUM].xv=0;
  teilchen[ATOMNUM].yv=0;
  teilchen[ATOMNUM].zv=0;


  teilchen[ATOMNUM].xv_old=0;
  teilchen[ATOMNUM].yv_old=0;
  teilchen[ATOMNUM].zv_old=0;


  counter++;
  ATOMNUM++;
 }
 counter2++;
}


cam.x=0,cam.y=0, cam.z=0;
cam.x_angl=0, cam.y_angl=0, cam.z_angl=0;


while(1)
{


framered2=0;
for(framereduce=0; framereduce < 10 ; framereduce++ )
{

  /* Tastatureingaben entgegennehmen */

	
	 if ( kbhit() )
 {
  c=getch();
  framered2=50;

  }
   else if ( framered2 == 0 ) c=0;
   framered2--;

   if(c=='u' ) cam.x-= -2*SIN(-cam.y_angl), cam.z-= -2*COS(-cam.y_angl);
   if(c=='j' ) cam.x+= -2*SIN(-cam.y_angl), cam.z+= -2*COS(-cam.y_angl);
   if(c=='h'  ) cam.y_angl+=0.04;
   if(c=='k' ) cam.y_angl-=0.04;
   if(c=='e') cam.y-=2.0; // war 2.0
   if(c=='d') cam.y+=2.0;
   if(c=='x') return;
   if ( c=='p' ) getch();

   if ( c=='v' ) if ( driveangl < 1.0 )driveangl+=0.2; /* eine Vektoraenderung koennte auch programmcodegesteuert erfolgen */
   if ( c=='n' ) if ( driveangl > - 1.0 ) driveangl-=0.2 ; /* z.B. zusaetzliche Angabe ueber Objekttyp */
   if ( c=='g' )  speed < 0.16 ? speed+=0.04 : (speed=0.16); /* vielleicht externer Bytecodeuebersetzer und Interpreter in diesem Programm */
   if ( c=='b' )  speed > -0.08 ? speed-=0.04 : (speed=-0.06);

  
     if ( speed > 0 ) speed-=0.01;
     else if ( speed < 0 ) speed+=0.01;

        if ( driveangl > 0.0 ) driveangl-=0.03;
        else if ( driveangl < 0.0 ) driveangl+=0.03;


     xtomid= -(teilchen[13].x - teilchen[5].x)*COS(-driveangl)
      -(teilchen[13].z - teilchen[5].z)*SIN(-driveangl);

      ztomid=  (teilchen[13].x - teilchen[5].x)*SIN(-driveangl)
      -(teilchen[13].z - teilchen[5].z)*COS(-driveangl);

     if ( touch==1)teilchen[5].xv+= xtomid*speed, teilchen[5].zv+=ztomid*speed;


        //cam.x=teilchen[13].x, cam.y=teilchen[13].y-20, cam.z=teilchen[13].z+170;
        //cam.y_angl=3.14;

    //  if ( fabs(speed) > 0.3 ) cam.x=teilchen[13].x - 30 , cam.z= teilchen[13].z + 120, cam.y_angl=driveangl+3.14, cam.y=14;

   /* Physik-Simulation. Je Eigenteilchen die Kraftwirkung aller umgebenden
     Fremdteilchen berechnen und auf die Vektoraenderung aufsummieren,
     danach bewegen. */

#define PHYSLICE 11 // war 11 5.1.17


/* printf("Das X von Teilchen 0:%f Teilchen 1:%f",teilchen[0].x, teilchen[1].x); */

n3=0;
while( n3 < PHYSLICE )
{
   if ( n3%1 == 0 )
   {
   n=0;
                   
   n=0;
   while (n < ATOMNUM )
   {

    n2=0;
    while ( n2 < ATOMNUM )
    {
     if ( n==n2 /* || fabs(teilchen[n].x-teilchen[n2].x) > 20 || // Distanzbegrenzung reingepatcht
                      fabs(teilchen[n].y-teilchen[n2].y) > 20 ||
                      fabs(teilchen[n].z-teilchen[n2].z) > 20 */) { n2++; continue; }
     power=
    SQRT(
    ( teilchen[n].x - teilchen[n2].x)  *
    ( teilchen[n].x - teilchen[n2].x )+
       ( teilchen[n].y - teilchen[n2].y) *
    ( teilchen[n].y - teilchen[n2].y)+
     ( teilchen[n].z - teilchen[n2].z) *
    ( teilchen[n].z - teilchen[n2].z) ) ;
  
    old_power=power;

   x_buf=(teilchen[n].x - teilchen[n2].x);
   y_buf=(teilchen[n].y - teilchen[n2].y);
   z_buf=(teilchen[n].z - teilchen[n2].z);
  
  #define SCHEITEL 10.0  /* die Stelle der ladungsneutralen Luecke um den Kern */
  #define LADUNG   17.0      /* neu --> war 17.0 */
  #define TRAEGSCHWELLE 1.0 /* ist die Kraft schwaecher als das, findet keine Abgabe statt */
  #define MAXKRAFT 4.0 /* so wird die aus der Distanz sich ergebende Kraft nie zu gross */
  #define PROT 1
  #define ELEK 0

  #define TIMESLICE 45.0 // war gerade eben 45.0
  #define VERLANGSFAC 1.00 // 0.999 /* war 0.993 allgemeiner Verlust Richtung 0, vielleicht wegen der Schwerkraft */

  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;

    power= ( LADUNG / ((power *power )+MAXKRAFT) *sign  ) ;

 
          if ( old_power != 0 )
          {
          y_buf*=power/old_power;
          x_buf*=power/old_power;
          z_buf*=power/old_power;
        } else x_buf=0, y_buf=0, z_buf=0;

    if ( (
	
	       ( teilchen[n].elec_or_prot==PROT
            && teilchen[n2].elec_or_prot==ELEK ) ||
          ( teilchen[n].elec_or_prot==ELEK
           && teilchen[n2].elec_or_prot==PROT )) &&
           teilchen[n].objID==teilchen[n2].objID
           /*
           ||

              (
           teilchen[n].objID==teilchen[n2].objID &&
           ( n2==teilchen[n].bondNum1 ||
                  n2==teilchen[n].bondNum2 ||
                  n2==teilchen[n].bondNum3 ||
                  n2==teilchen[n].bondNum4 ||
                  n2==teilchen[n].bondNum5 ||
                  n2==teilchen[n].bondNum6)
               ) 
           */

      ) {  y_buf*=-1.0; x_buf*=-1.0; z_buf*=-1.0;


         /*
           if ( ( n2==teilchen[n].bondNum1 ||
                  n2==teilchen[n].bondNum2 ||
                  n2==teilchen[n].bondNum3 ||
                  n2==teilchen[n].bondNum4 ||
                  n2==teilchen[n].bondNum5 ||
                  n2==teilchen[n].bondNum6)
                && teilchen[n].objID==teilchen[n2].objID )
           
           */
		   {
            #define KOHAERENZ 1
            x_buf*=KOHAERENZ,
            y_buf*=KOHAERENZ,
            z_buf*=KOHAERENZ;
              }
			  
			 

           }


    if ( (old_power=SQRT ( x_buf*x_buf+y_buf*y_buf +z_buf*z_buf)) > TRAEGSCHWELLE )
   {
      power= (old_power-TRAEGSCHWELLE)/ old_power;
      x_buf*=power; y_buf*=power; z_buf*=power;
      
	  //if ( teilchen[n].objID != teilchen[n2].objID) printf("touche"), getch(); // test 
      
    }  else x_buf=0, y_buf=0, z_buf=0;

    
 
 
    n2++;


    teilchen[n].xv+=x_buf;
    teilchen[n].yv+=y_buf;
    teilchen[n].zv+=z_buf;

    }


    n++;
   }


	n=0;
	while ( n < ATOMNUM)
	{
		teilchen[n].yv+=0.009;
		n++;
     }

} // Aussetzer Ende

n=0;
   while (n < ATOMNUM )
   {  floor_cx=0;
	   while ( floor_cx < 5)
	   {
	     floor_cz=0;
	     while ( floor_cz < 5)
	     {
		 
		  floor_teil[floor_cx*5+floor_cz].x=(int)trunc(teilchen[n].x-2.5)+floor_cx;
        
		if ( teilchen[n].x-2+floor_cx >= 0 &&
		      teilchen[n].x-2+floor_cx <= 511 &&
			  teilchen[n].z-2+floor_cz >= 0 &&
		      teilchen[n].z-2+floor_cz <= 511)
		  floor_teil[floor_cx*5+floor_cz].y=floorHmap[(int)trunc(teilchen[n].x-2.5)+floor_cx][(int)trunc(teilchen[n].z-2.5)+floor_cz]+25;
          else floor_teil[floor_cx*5+floor_cz].y=40; // Hoehenkorrektur 2
          
          
		  floor_teil[floor_cx*5+floor_cz].z=(int)trunc(teilchen[n].z-2.5)+floor_cz;
          floor_cz++;
	   }
	   floor_cx++;
      }
      
    n2=0;
    while ( n2 < 25 )
    {
     power=
    SQRT(
    ( teilchen[n].x - floor_teil[n2].x)  *
    ( teilchen[n].x - floor_teil[n2].x )+
       ( teilchen[n].y - floor_teil[n2].y) *
    ( teilchen[n].y - floor_teil[n2].y)+
     ( teilchen[n].z - floor_teil[n2].z) *
    ( teilchen[n].z - floor_teil[n2].z) ) ;
  
    old_power=power;
    
     if ( n==5 && old_power < 5 ) touch=1;
     else if ( n==5) touch=0; // antreibendes Rad beruehrt den Boden

   x_buf=(teilchen[n].x - floor_teil[n2].x);
   y_buf=(teilchen[n].y - floor_teil[n2].y);
   z_buf=(teilchen[n].z - floor_teil[n2].z);
   
  
  #define BODENSCHEITEL 0.0 // WAR 0.5  geaendert am 5.1.17
  /* if ( power-BODENSCHEITEL < 0.0 ) sign= -1.0; else */ sign= 1.0;
   power=power-BODENSCHEITEL;

   #define MAXKRAFTB 0.01  // war 4.0 geaendert am 5.1.17
   #define LADUNGB   20.0  // war 17.0 geaendert am 5.1.17
    power= ( LADUNGB / ((power*100*power )+MAXKRAFTB) *sign  ) ;

 
          if ( old_power != 0 )
          {
          y_buf*=power/old_power;
          x_buf*=power/old_power;
          z_buf*=power/old_power;
        } else x_buf=0, y_buf=0, z_buf=0;

#define TRAEGSCHWELLEB 0.05 // 0.01 // war 2.0 // war 1.0 geandert am 5.1.17
    if ( (old_power=SQRT ( x_buf*x_buf+y_buf*y_buf +z_buf*z_buf)) > TRAEGSCHWELLEB )
   {
      power= (old_power-TRAEGSCHWELLE)/ old_power;
      // auskommentiert am 5.1.17
	  // x_buf*=power; y_buf*=power; z_buf*=power;
      // bockten sich in den Boden
	  #define REIBUNG 0.996
      teilchen[n].xv*=REIBUNG, teilchen[n].yv*=REIBUNG, teilchen[n].zv*=REIBUNG;
    }  else
	         x_buf=0, y_buf=0, z_buf=0;
    
 
 
    n2++;


    teilchen[n].xv+=x_buf;
    teilchen[n].yv+=y_buf;
    teilchen[n].zv+=z_buf;

    }


    n++;
   }




 	n=0;
     while ( n < ATOMNUM )
  {

    teilchen[n].xv*=VERLANGSFAC; teilchen[n].yv*=VERLANGSFAC; teilchen[n].zv*=VERLANGSFAC;

   teilchen[n].x+=(teilchen[n].xv/TIMESLICE); teilchen[n].y+=(teilchen[n].yv/TIMESLICE) ;
    teilchen[n].z+=(teilchen[n].zv/TIMESLICE);

    teilchen[n].xv=(teilchen[n].xv_old+teilchen[n].xv)/2;
    teilchen[n].yv=(teilchen[n].yv_old+teilchen[n].yv)/2;
    teilchen[n].zv=(teilchen[n].zv_old+teilchen[n].zv)/2;

    teilchen[n].xv_old=teilchen[n].xv;
    teilchen[n].yv_old=teilchen[n].yv;
    teilchen[n].zv_old=teilchen[n].zv;


   n++;
  }

  

  n3++;
 }

} // Framereduce Ende

  counter=0;
  counter2=0;
 while ( counter2 < objnum )  /* Koordinaten des ersten Atoms zu Objektkoordinaten zuweisen */
 {
  input_obj[counter2].x=teilchen[(int)counter].x;
  input_obj[counter2].y=teilchen[(int)counter].y;
  input_obj[counter2].z=teilchen[(int)counter].z;
  teilchen[(int)counter].distance=0;
  teilchen[(int)counter].nearest=100000;
  counter++;

  while ( teilchen[(int)counter].objID == teilchen[(int)counter-1].objID ) counter++;
 counter2++;
 }

counter2=0;
counter=0;
while ( counter < objnum )  /* aus den Koordinaten der bewegten Atome
                               den naechsten korrespondierenden Drehwinkel der
                               grafischen Darstellung ermitteln (Winkel durchprobieren) */
{

   y_buf=-3.3;
   x_buf=-3.3;
   z_buf=-3.3; // probier mal 5.1.17 -0.3 debug


  while ( x_buf < 3.3 )
  {
   y_buf=-4.0;
   while ( y_buf < 3.3 )
   {
    z_buf=-4.0;
    while ( z_buf < 3.3)
    {
     teilchen[counter2].distance=0;
     n=1;

while ( n < 3 )
{

        cmpadd_x=0, cmpadd_y=0;

       if ( n==1 ) cmpadd_x=input_obj[(int)counter].x_next;
       if ( n==2 ) cmpadd_y=input_obj[(int)counter].y_next;

    /* y-Achse */


       x_turned= +(cmpadd_x)*COS(y_buf)+(0)*SIN(y_buf);
       y_turned=cmpadd_y;
       z_turned= -(cmpadd_x)*SIN(y_buf)+(0)*COS(y_buf);


 /* x-Achse */


       y_turned2= +(y_turned)*COS(x_buf)-(z_turned)*SIN(x_buf);
       x_turned2=x_turned;
       z_turned2= (y_turned)*SIN(x_buf)+(z_turned)*COS(x_buf);

 /* z-Achse  */


       x_turned3= +(x_turned2)*COS(z_buf)-(y_turned2)*SIN(z_buf);
       z_turned3=z_turned2;
       y_turned3= (x_turned2)*SIN(z_buf)+(y_turned2)*COS(z_buf);

       x_cmp=x_turned3+teilchen[counter2].x,
       y_cmp=y_turned3+teilchen[counter2].y,
       z_cmp=z_turned3+teilchen[counter2].z;

       x_cmp= fabs(x_cmp-teilchen[counter2+n].x);
       y_cmp= fabs(y_cmp-teilchen[counter2+n].y);
       z_cmp= fabs(z_cmp-teilchen[counter2+n].z);

       teilchen[counter2].distance+= (x_cmp+y_cmp+z_cmp);

       n++;
}


       if ( teilchen[counter2].distance < teilchen[counter2].nearest )
      {
        input_obj[(int)counter].x_angl=x_buf,
        input_obj[(int)counter].y_angl=y_buf,
        input_obj[(int)counter].z_angl=z_buf;

        teilchen[counter2].nearest=teilchen[counter2].distance;

       }

     z_buf+=0.2;
    }

    y_buf+=0.2;
   }
   x_buf+=0.2; // war 0.2 debug
  }

  while ( teilchen[counter2].objID == teilchen[counter2+1].objID && counter2 < ATOMNUM-1) counter2++;

  /*printf("X-Wink: %f Y-Wink: %f Z-Wink: %f\n",input_obj[counter].x_angl, input_obj[counter].y_angl,
                              input_obj[counter].z_angl ); *//* debug */

  counter2++;
 counter++;

}

/* printf("X-Wink: %f Y-Wink: %f Z-Wink: %f  %d\n",input_obj[1].x_angl, input_obj[1].y_angl,
                              input_obj[1].z_angl, counter2 ); */ /* debug */


   /* Bildschirm leeren, Z-Buffer initialisieren */
     screen_y=0;
  while(screen_y<96)
  {
   screen_x=0;
   while(screen_x<160)
   {
    screen[screen_x][screen_y]=219;
    zbuf[screen_x][screen_y]=100000;
    screen_x++;
   }
   screen_y++;
   }

#define BUFX 0
#define BUFY 1
#define BUFZ 2
#define VALUE 3

 pixelnum=0;

counter2=0;


while( counter2 < objnum ) /* fuer jeden einzelnen Pixel dessen Drehungen
                             anhand der Objektpositionierung berechnen */
{
 counter=0;
  while(counter < input_obj[counter2].num_pixels && counter < input_obj[counter2].num_pixels)
  {
   if ( input_obj[counter2].pixgroup[counter][VALUE] != 0 )
   {
    pixelbuf[pixelnum][BUFX]=input_obj[counter2].pixgroup[counter][BUFX]  +input_obj[counter2].x ;
    pixelbuf[pixelnum][BUFY]=input_obj[counter2].pixgroup[counter][BUFY] +input_obj[counter2].y ;
    pixelbuf[pixelnum][BUFZ]=input_obj[counter2].pixgroup[counter][BUFZ] +input_obj[counter2].z ;
    pixelbuf[pixelnum][VALUE]=(unsigned char)input_obj[counter2].pixgroup[counter][VALUE];


/* y-Achse */


       x_turned= +(pixelbuf[pixelnum][BUFX]-input_obj[counter2].x)*COS(input_obj[counter2].y_angl)+(pixelbuf[pixelnum][BUFZ]-input_obj[counter2].z)*SIN(input_obj[counter2].y_angl);
       y_turned=pixelbuf[pixelnum][BUFY]    -input_obj[counter2].y; /* Subtraktion eingefuegt */
       z_turned= -(pixelbuf[pixelnum][BUFX]-input_obj[counter2].x)*SIN(input_obj[counter2].y_angl)+(pixelbuf[pixelnum][BUFZ]-input_obj[counter2].z)*COS(input_obj[counter2].y_angl);


 /* x-Achse */


       z_turned2= (y_turned)*SIN(input_obj[counter2].x_angl)+(z_turned)*COS(input_obj[counter2].x_angl);
       y_turned2= (y_turned)*COS(input_obj[counter2].x_angl)-(z_turned)*SIN(input_obj[counter2].x_angl);
       x_turned2=x_turned;

 /* z-Achse  */


       y_turned3= (x_turned2)*SIN(input_obj[counter2].z_angl)+(y_turned2)*COS(input_obj[counter2].z_angl);
       x_turned3= (x_turned2)*COS(input_obj[counter2].z_angl)-(y_turned2)*SIN(input_obj[counter2].z_angl);
       z_turned3= z_turned2;

      pixelbuf[pixelnum][BUFX]=x_turned3+input_obj[counter2].x;
      pixelbuf[pixelnum][BUFY]=y_turned3+input_obj[counter2].y;
      pixelbuf[pixelnum][BUFZ]=z_turned3+input_obj[counter2].z;

     pixelnum++;
    }
     if ( pixelbuf[pixelnum-1][BUFZ]-cam.z > ZOOM*4 )    counter+=2;
    else counter++;
                    /* unterschiedliche Pixelzahlen nach Abstand */

  }
  counter2++;

}

 counter=0;

 old_pix_num=pixelnum;
 distsample=0.0;
 while ( counter < old_pix_num ) /* Projektion aller Pixel zur Kamera, vorher: Kameradrehung berechnen */
 {
  
/* y-Achse */


       x_turned= +(pixelbuf[(int)counter][BUFX]-cam.x)*COS(cam.y_angl)+(pixelbuf[(int)counter][BUFZ]-cam.z)*SIN(cam.y_angl);
       y_turned=pixelbuf[(int)counter][BUFY];
       z_turned= -(pixelbuf[(int)counter][BUFX]-cam.x)*SIN(cam.y_angl)+(pixelbuf[(int)counter][BUFZ]-cam.z)*COS(cam.y_angl);

         pixelbuf[(int)counter][BUFX]=x_turned+cam.x;
        pixelbuf[(int)counter][BUFY]=y_turned;
        pixelbuf[(int)counter][BUFZ]=z_turned+cam.z;


        if ( z_turned < ZOOM )
        {

        #define DOUBLEFILL(xoff,yoff) \
         pixelbuf[(int)pixelnum][BUFX]=x_turned+xoff+cam.x; \
        pixelbuf[(int)pixelnum][BUFY]=y_turned+yoff;\
        pixelbuf[(int)pixelnum][BUFZ]=z_turned+cam.z;\
        pixelbuf[(int)pixelnum][VALUE]=pixelbuf[(int)counter][VALUE];\
\
         pixelnum++;\

         DOUBLEFILL (-1*z_turned/ZOOM,-1*z_turned/ZOOM)
         DOUBLEFILL (1*z_turned/ZOOM,-1*z_turned/ZOOM)
         DOUBLEFILL (0,-1*z_turned/ZOOM)
         DOUBLEFILL (-1*z_turned/ZOOM,0)
         DOUBLEFILL (1*z_turned/ZOOM,0 )
         DOUBLEFILL (-1*z_turned/ZOOM,1*z_turned/ZOOM)
         DOUBLEFILL (0,1*z_turned/ZOOM)
         DOUBLEFILL (1*z_turned/ZOOM,1*z_turned/ZOOM)

        }


  counter++;
 }


 // Hoehenkarte

 vox_z=0;
 while ( vox_z < 512 )
 {
  vox_x=0;

  while ( vox_x < 512 )
  {
       x_turned= +(vox_x-cam.x)*COS(cam.y_angl)+(vox_z-cam.z)*SIN(cam.y_angl);
       y_turned=floorHmap[vox_x][vox_z]; // Hoehenkorrektur 3
       z_turned= -(vox_x-cam.x)*SIN(cam.y_angl)+(vox_z-cam.z)*COS(cam.y_angl);

       x_turned+=cam.x;
       z_turned+=cam.z;

    #define SETVOXEL(xf,yf,zf) \
  if ( (z_turned-(float)cam.z)/ZOOM>0  ) \
      { \
       if( (79.0 + ( (signed int)xf -cam.x)/(((signed int)zf-cam.z)/ZOOM)) < 160 && 47.0 + ((yf-cam.y)/(((signed int)zf-cam.z)/ZOOM)) < 90.0 && \
           79.0 + (( (signed int)xf -cam.x)/(((signed int)zf-cam.z)/ZOOM)) >= 0 && 47.0 + ((yf-cam.y)/(((signed int)zf-cam.z)/ZOOM)) >= 0   ) \
       if ( zbuf[ (signed int) (79.0 + (( (signed int)xf -cam.x)/(((signed int)zf-(float)cam.z)/ZOOM))) ] [ (signed int)(47.0 + ((yf-cam.y)/(((signed int)zf-cam.z)/ZOOM))) ] > ((signed int)zf-cam.z) ) \
            zbuf[ (signed int) (79.0 + \
            (( (signed int)xf -cam.x))/ \
            (((signed int)zf-(float)cam.z)/ZOOM)) ] \
            [ (signed int)(47.0 + ((yf-cam.y)/(((signed int)zf-(float)cam.z)/ZOOM))) ] \
            =((signed int)zf-cam.z), \
       screen[ (signed int) (79.0 + (( (signed int)xf -cam.x)/(((signed int)zf-(float)cam.z)/ZOOM))) ] [ (signed int)(47.0 + (((signed int)yf-cam.y)/(((signed int)zf-cam.z)/ZOOM))) ]=(unsigned char)floorLmap[vox_x][vox_z]; }\


   SETVOXEL(  ((signed int)x_turned-1),(35+y_turned), ((signed int)z_turned ))
    SETVOXEL(  ((signed int)x_turned),(35+y_turned), ((signed int)z_turned ))
     SETVOXEL(  ((signed int)x_turned+1),(35+y_turned-1), ((signed int)z_turned ))
      SETVOXEL(  ((signed int)x_turned-1),(35+y_turned-1), ((signed int)z_turned ))
     SETVOXEL(  ((signed int)x_turned),(35+y_turned-1), ((signed int)z_turned ))
     SETVOXEL(  ((signed int)x_turned+1),(35+y_turned), ((signed int)z_turned ))
     SETVOXEL(  ((signed int)x_turned-1),(35+y_turned+1), ((signed int)z_turned ))
     SETVOXEL(  ((signed int)x_turned),(35+y_turned+1), ((signed int)z_turned ))
     SETVOXEL(  ((signed int)x_turned+1),(35+y_turned+1), ((signed int)z_turned ))
    
	  
	    // Mehrfachsetzen am 5.1.17 auskommentiert
   vox_x++;
   if ( z_turned > ZOOM ) vox_x++;
   if ( z_turned > ZOOM*3 ) vox_x++;
   if ( z_turned > ZOOM*6 ) vox_x+=2;

  }

  if ( z_turned > ZOOM ) vox_z++;
   if ( z_turned > ZOOM*3 ) vox_z++;
   if ( z_turned > ZOOM*6 ) vox_z+=2;

  vox_z++;
 }



 while ( pixelnum > 0 )
 {
  pixelnum--;

   /* die eigentliche Projektion */

  if ( (pixelbuf[pixelnum][BUFZ]-(float)cam.z)/ZOOM>0  )
      {
       if( (79.0 + (pixelbuf[pixelnum][BUFX]-cam.x)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM)) < 160 && 47.0 + ((pixelbuf[pixelnum][BUFY]-cam.y)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM)) < 90.0 &&
           79.0 + ((pixelbuf[pixelnum][BUFX]-cam.x)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM)) >= 0 && 47.0 + ((pixelbuf[pixelnum][BUFY]-cam.y)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM)) >= 0   )
       if ( zbuf[ (signed int) (79.0 + ((pixelbuf[pixelnum][BUFX]-cam.x)/((pixelbuf[pixelnum][BUFZ]-(float)cam.z)/ZOOM))) ] [ (signed int)(47.0 + ((pixelbuf[pixelnum][BUFY]-cam.y)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM))) ] > (pixelbuf[pixelnum][BUFZ]-cam.z) )
            zbuf[ (signed int) (79.0 +
            ((pixelbuf[pixelnum][BUFX]-cam.x))/
            ((pixelbuf[pixelnum][BUFZ]-(float)cam.z)/ZOOM)) ]
            [ (signed int)(47.0 + ((pixelbuf[pixelnum][BUFY]-cam.y)/((pixelbuf[pixelnum][BUFZ]-(float)cam.z)/ZOOM))) ]
            =(pixelbuf[pixelnum][BUFZ]-cam.z),
       screen[ (signed int) (79.0 + ((pixelbuf[pixelnum][BUFX]-cam.x)/((pixelbuf[pixelnum][BUFZ]-(float)cam.z)/ZOOM))) ] [ (signed int)(47.0 + ((pixelbuf[pixelnum][BUFY]-cam.y)/((pixelbuf[pixelnum][BUFZ]-cam.z)/ZOOM))) ]=pixelbuf[pixelnum][VALUE];
    }


 }


   /* Uebertragen des Screenbuffers auf den Bildschirm ( hier: Textmodus,
     zukuenftig: Grafikmodus ). */
  system("cls\n");

   /* Teilchendump debug */

 
/*
  screen[(int)teilchen[0].x/10+80][(int)teilchen[0].y/10+20]='1';
  screen[(int)teilchen[1].x/10+80][(int)teilchen[1].y/10+20]='2';
  screen[(int)teilchen[2].x/10+80][(int)teilchen[2].y/10+20]='3';
  screen[(int)teilchen[3].x/10+80][(int)teilchen[3].y/10+20]='4';
  screen[(int)teilchen[4].x/10+80][(int)teilchen[4].y/10+20]='5';
  screen[(int)teilchen[5].x/10+80][(int)teilchen[5].y/10+20]='6';
  screen[(int)teilchen[6].x/10+80][(int)teilchen[6].y/10+20]='7';    
*/
  screen[79][47]='X'; /* debug */

  screen_y=0;

  while(screen_y<85)
  {
   screen_x=0;
   while(screen_x<160)
   {
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",screen[screen_x][screen_y],
                              screen[screen_x+1][screen_y],
                              screen[screen_x+2][screen_y],
                              screen[screen_x+3][screen_y],
                              screen[screen_x+4][screen_y],
                              screen[screen_x+5][screen_y],
                              screen[screen_x+6][screen_y],
                              screen[screen_x+7][screen_y],
							  screen[screen_x+8][screen_y],
                              screen[screen_x+9][screen_y],
                              screen[screen_x+10][screen_y],
                              screen[screen_x+11][screen_y],
                              screen[screen_x+12][screen_y],
                              screen[screen_x+13][screen_y],
                              screen[screen_x+14][screen_y],
                              screen[screen_x+15][screen_y]);
    screen_x+=16;
   }
    printf("\n");
   screen_y++;
   }
    /*
    printf("%f %f %f\n"
           "%f %f %f\n"
           "%f %f %f\n",teilchen[0].x, teilchen[0].y, teilchen[0].z,
                        teilchen[4].x, teilchen[4].y, teilchen[4].z,  // war 4,5
                        teilchen[5].x, teilchen[5].y, teilchen[5].z );          */
    if ( score < 1000 && game_ended >= 15 )
    printf("%d %f\n", 1000-score, game_ended=SQRT( (256-teilchen[27].x)*(256-teilchen[27].x)+
                                   (256-teilchen[27].z)*(256-teilchen[27].z)+
                                   (32-teilchen[27].y)*(32-teilchen[27].y)) ),
     (game_ended > 15 ? score++ : 0);
     else { if ( game_ended > 15 ) printf("Game Over!"); else printf("%d Punkte",1000-score); };




  /*  printf("%f %f %d\n",cam.x,cam.y,pixelbuf[12][VALUE] ); */ /*debug*/
}

/* Klar macht es erstmal einen Scheisseindruck.
  Aber ich irre mich nicht in der Annahme, dass der Grundstein gelegt ist und dass
  hier ein enormes Potential drinsteckt.
  Es ist der Grundstock fuer eine 3D-Engine und eine Physik-Simulation.
  Mit den richtigen Parametern kann das eindrucksvoll und toll werden. */

 }
