//Grafik... ////////////

#include <grx.h>

unsigned char vscreen[64000];
char *spritetable;

struct
{
 int r;
 int g;
 int b;

}palette[256];

unsigned brightnz;

set_palette( unsigned col, int r, int g, int b )
{
 palette[col].r=r,
 palette[col].g=g,
 palette[col].b=b;

}

mask_screen()
{
 auto volatile c=0;

 do
 {
  GrSetColor(c,
              ((long)palette[c].r*brightnz)/100,
              ((long)palette[c].g*brightnz)/100,
              ((long)palette[c].b*brightnz)/100 );
 } while(c++ < 256 );

}


unsigned char show_bmp(char *bmp_file)
{
 ifstream bmp;
 struct
 {
  int color;
  unsigned char r,g,b;
 } mask;
 mask.color=0;
 unsigned long int x_max,y_max;
 unsigned char detect_str[2];

 bmp.open(bmp_file,ios::in|ios::bin|ios::nocreate);
 if ( bmp.fail() ) return 1;

 bmp.read(detect_str,sizeof(unsigned char)*2);
 if ( detect_str[0]!='B'||detect_str[1]!='M')
 {
  bmp.close();
  return 2;
 }
 else
 {
  bmp.seekg(10);
  bmp.read(detect_str,sizeof(unsigned char)*2);
  if (detect_str[0]!='6'|| detect_str[1]!='\x4')
  {
   bmp.close();
   return 3;
  }
 }

 bmp.seekg(18);  // X-Koordinaten
 bmp.read(&x_max,sizeof(long unsigned int));
 bmp.seekg(22);  // Y-Koordinaten
 bmp.read(&y_max,sizeof(long unsigned int));
 bmp.seekg(53); // Position der Maskierung

 while ( mask.color != 256 )
 {
  bmp.seekg(bmp.tellg()+1);
  bmp.read(&mask.b,sizeof(unsigned char)),
  bmp.read(&mask.g,sizeof(unsigned char)),
  bmp.read(&mask.r,sizeof(unsigned char)),
  set_palette( mask.color, mask.r,mask.g,mask.b );
  mask.color++;
 }
 if(x_max%4!=0)x_max+=4-(x_max%4);
 unsigned char disp[(x_max*y_max)];
 bmp.seekg(bmp.tellg()+1);
 bmp.read(disp,sizeof(unsigned char)*x_max*y_max );
 signed int x,y;
 double ipv_x=(double)x_max/320.0;
 double ipv_y=(double)y_max/200.0;
 x=0;
 y=0;

 while(y<199)
 {
  x=0;
  while(x<320)
  {
   GrPlotNC(x,199-y,disp[((unsigned int)(y*ipv_y)*x_max+(unsigned int)(x*ipv_x))]);
   x++;
  }
  y++;
 }
 bmp.close();

}



void initgraph(void)
{
 clock_t delay;
 brightnz=0;

 GrSetBIOSMode(0,320,200,256);

 do
 {
  delay=clock()+50000,
  mask_screen(),
  show_bmp("palette"),// die Anzeige des Bitmaps setzt die Palette(!)
  brightnz+=10;
   while( delay > clock() )if( delay-clock() > 50000 ) delay=clock();
 } while( brightnz < 110 );

}



void transfer_screen(unsigned long int wobble)
{
 unsigned x=0, y=0;

 while( y < 200)
 {
  x=0;

  if( wobble==0 )
  {
   while(x < 320)
   {
    GrPlotNC(x,y, vscreen[x+y*320] );
    GrPlotNC(x+1,y, vscreen[x+1+y*320] );
    GrPlotNC(x+2,y, vscreen[x+2+y*320] );
    GrPlotNC(x+3,y, vscreen[x+3+y*320] );   

    if(x%20==0 &&blaster==0 ) --sb_buf_len,
       sb_play(1); // Soundwiedergabe im DirectMode; unsch”nerweise notwendig

    x+=4;
   } 
  }
  else
  {
   long unsigned temp;
   temp=(unsigned long)((sin(wobble++)+2)*5);
   GrPlotNC(0,y,0 ), GrPlotNC(1,y,0), GrPlotNC(2,y,0), GrPlotNC(3,y,0 ),
   GrPlotNC(4,y,0 ), GrPlotNC(5,y,0), GrPlotNC(6,y,0), GrPlotNC(7,y,0 ),
   GrPlotNC(8,y,0 ), GrPlotNC(9,y,0), GrPlotNC(10,y,0);

   while(x < 320)
   {
    GrPlotNC(temp+x,y, vscreen[x+y*320] );
    GrPlotNC(temp+x+1,y, vscreen[x+1+y*320] );
    GrPlotNC(temp+x+2,y, vscreen[x+2+y*320] );
    GrPlotNC(temp+x+3,y, vscreen[x+3+y*320] );

    if(x%20==0 &&blaster==0 ) --sb_buf_len,
       sb_play(1); // Soundwiedergabe im DirectMode; unsch”nerweise notwendig

    x+=4;
   } 
  }

  y++;
 }
}

void v_line_up(int x, int y, char c)
{
 while( y > -1 )
 {
  vscreen[x+y*320]=c;
  --y;
 }


}

void v_line_down(int x, int y, char c)
{
 while( y < 200 )
 {
  vscreen[x+y*320]=c;
  ++y;
 }


}


void dumpnum(unsigned x_st, unsigned y_st, unsigned long numba)
{
 unsigned pos=1000000,
                      x,y;
 static char font[10][30]={
                         {0,0,0xff,0,0,
                          0,0xff,0,0xff,0,
                          0xff,0,0,0,0xff,
                          0xff,0,0,0,0xff,
                          0,0xff,0,0xff,0,
                          0,0,0xff,0,0 },

                         {0,0,0xff,0xff,0,
                          0,0xff,0,0xff,0,
                          0xff,0,0,0xff,0,
                          0,0,0,0xff,0,
                          0,0,0,0xff,0,
                          0,0,0xff,0xff,0xff },

                         {0,0xff,0xff,0xff,0,
                          0xff,0,0,0,0xff,
                          0,0,0,0,0xff,
                          0,0,0,0xff,0,
                          0,0,0xff,0,0,
                          0,0xff,0xff,0xff,0xff },

                         {0xff,0xff,0xff,0xff,0,
                          0,0,0,0,0xff,
                          0,0xff,0xff,0xff,0,
                          0,0,0,0,0xff,
                          0,0,0,0,0xff,
                          0xff,0xff,0xff,0xff,0 },

                         {0,0xff,0,0xff,0,
                          0,0xff,0,0xff,0,
                          0xff,0,0,0xff,0,
                          0xff,0xff,0xff,0xff,0xff,
                          0,0,0,0xff,0,
                          0,0,0,0xff,0 },

                         {0xff,0xff,0xff,0xff,0,
                          0xff,0,0,0,0,
                          0xff,0xff,0xff,0xff,0,
                          0,0,0,0,0xff,
                          0,0,0,0,0xff,
                          0xff,0xff,0xff,0xff,0 },

                         {0,0xff,0xff,0xff,0,
                          0xff,0,0,0,0,
                          0xff,0xff,0xff,0xff,0,
                          0xff,0,0,0,0xff,
                          0xff,0,0,0,0xff,
                          0,0xff,0xff,0xff,0 },

                         {0xff,0xff,0xff,0xff,0,
                          0,0,0,0,0xff,
                          0,0,0,0,0xff,
                          0,0,0,0xff,0,
                          0,0,0xff,0,0,
                          0,0xff,0,0,0 },

                         {0,0xff,0xff,0,0,
                          0xff,0,0,0xff,0,
                          0,0xff,0xff,0,0,
                          0xff,0,0,0xff,0,
                          0xff,0,0,0xff,0,
                          0,0xff,0xff,0,0 },

                         {0,0,0xff,0xff,0,
                          0,0xff,0,0,0xff,
                          0,0,0xff,0xff,0xff,
                          0,0,0,0xff,0,
                          0,0,0,0xff,0,
                          0,0xff,0xff,0,0 }

                                       };

  while( (numba/pos)==0 && pos > 1 ) pos/=10;

  while( pos > 0 )
  {
   y=0;
   while( y<6 )
   {
    x=0;
    while( x<5)
    {
     if( font[ (numba/pos)%10 ][x+y*5] != 0 )
     vscreen[ (x+x_st)+(y+y_st)*320]=font[ (numba/pos)%10 ][x+y*5];
     ++x;
    }
    ++y;
   }
   x_st+=5;
   pos/=10;
  }


}


void dumpstate(long score, int nrj, BOOL boost, BOOL weapon )
{
 dumpnum(280,3,score);
 while( nrj > 0 )
 {
  // je nach der verwendeten Palette sieht das besser aus->
   // v_line_up( 3+nrj,3, ~((unsigned char)nrj) > 255 ? 255 : ~((unsigned char)nrj) ), nrj--;

  v_line_up( 3+nrj,3, 255 ), nrj--;
 }

 char temp;
 if( boost !=0 )temp=255; else temp=0;
 {
  vscreen[5+6*320]=temp,  vscreen[6+6*320]=temp,
  vscreen[5+7*320]=temp,                       vscreen[7+7*320]=temp,
  vscreen[5+8*320]=temp,  vscreen[6+8*320]=temp,
  vscreen[5+9*320]=temp,                       vscreen[7+9*320]=temp,
  vscreen[5+10*320]=temp, vscreen[6+10*320]=temp;
 }

 if( weapon != 0 ) temp=255; else temp=0;
 {
  vscreen[15+6*320]=temp,                                                                        vscreen[19+6*320]=temp,
  vscreen[15+7*320]=temp,                        vscreen[17+7*320]=temp,                          vscreen[19+7*320]=temp,
  vscreen[15+8*320]=temp,                        vscreen[17+8*320]=temp,                          vscreen[19+8*320]=temp,
  vscreen[15+9*320]=temp,                        vscreen[17+9*320]=temp,                          vscreen[19+9*320]=temp,
  vscreen[15+10*320]=temp, vscreen[16+10*320]=temp, vscreen[17+10*320]=temp, vscreen[18+10*320]=temp, vscreen[19+10*320]=temp;
 }


}


unsigned fill_bg( char *file, unsigned long long offset, unsigned setfree, unsigned long debug)
{ 
 static char filename[256]=" ";
 static char *buffer=NULL;

 if( setfree==1 )
 {
  if( buffer!=NULL )delete buffer;
  return (0);
 }

 if( strcmp( filename, file)!=0 )
 {
  if( buffer!= NULL ) free(buffer);
  buffer= (char *)malloc( bg_y_len*320 );
  strcpy( filename, file);
  decode( filename, buffer );
 }

 unsigned x=0, y=0;
 unsigned long y2;

 bg_y_len=debug;// seltsam, aber irgendwie geht der Wert verloren...
                 // (Compiler-Fehler?!?)

 while( y<200 )
 {
  y2= ((offset-200+y)%bg_y_len)*320;
  x=0;
  while(x<320)
  {
   vscreen[x+y*320]=
           buffer[x+ y2];
   vscreen[x+1+y*320]=
           buffer[x+1+ y2];
   vscreen[x+2+y*320]=
           buffer[x+2+ y2];
   vscreen[x+3+y*320]=
           buffer[x+3+ y2];

   x+=4;
  }
   if( blaster==0 )
   sb_play(1), --sb_buf_len; // Soundwiedergabe im DirectMode; unsch”nerweise notwendig
  y++;
 }

 return (0);
}


void put_sprite(long long int x_st, long long int y_st, unsigned char *sprite )
{
 unsigned x=0,y=0;

 while(y<64 )
 {
  x=0;
  while(x<64 )
  {
   if(x+x_st < 320 && y+y_st < 200 && x+x_st>=0 && y+y_st >= 0 )
   {
    vscreen[x_st+x+ (y_st+y)*320]=
    sprite[x+y*64]==255 ?
    vscreen[x_st+x+ (y_st+y)*320]: sprite[x+y*64];
   }
   x++;
  }
  y++;
 }  

}

void put_sprite_mirrored(int x_st, int y_st, unsigned char *sprite )
{
 signed x=0,y=0;

 while(y<64 )
 {
  x=0;
  while(x<64 )
  {
   if( (64-x)+x_st <= 319 && y+y_st < 200 && (64-x)+x_st>=0 && y+y_st >= 0 )
   {
    vscreen[x_st+(64-x)+ (y_st+y)*320]=
    sprite[x+y*64]==255 ?
    vscreen[x_st+(64-x)+ (y_st+y)*320]: sprite[x+y*64];
   }
   x++;
  }
  y++;
 }  

}


char *load_spritetable( unsigned long int n)
// ein Sprite umfaát _immer_ 64x64 Pixel
{
 char *ptr;
 if( (ptr=(char *)malloc( (size_t) n*4096 ))==NULL ) return (char *)NULL;
 if( decode( "sprites", ptr )==0 ) return (char *)NULL;

 return ptr;
}


/////////////bis hier... /////////////////////
