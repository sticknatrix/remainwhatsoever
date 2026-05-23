#include <stdio.h>

unsigned char testarray[10];

int offset;
unsigned long int value;
unsigned int emptybuf;
#define assign_bits_to_array(array,width,pos,val) pos*=width;\
                                                offset=pos%8;\
                                                  pos/=8;\
                                                value=val;\
                                                value|=0xffff00;\
                                                emptybuf=0;\
                                                if ( width<8 ) value|= 1 << 8;\
                                                if ( width<7 ) value|= 1 << 7;\
                                                if ( width<6 ) value|= 1 << 6;\
                                                if ( width<5) value|= 1 << 5;\
                                                if ( width<4 ) value|= 1 << 4;\
                                                if ( width<3 ) value|= 1 << 3;\
                                                if ( width<2 ) value|= 1 << 2;\
                                               \
                                                 if ( width> 1 ) emptybuf|=1<<1;\
                                                   if ( width> 2 ) emptybuf|=1<<2;\
                                                 if ( width> 3 ) emptybuf|=1<<3;\
                                                 if ( width> 4 ) emptybuf|=1<<4;\
                                                 if ( width> 5 ) emptybuf|=1<<5;\
                                                 if ( width> 6 ) emptybuf|=1<<6;\
                                                 if ( width> 7 ) emptybuf|=1<<7;\
                                                emptybuf|=1;\
                                                 if ( offset>6 ) value<<=1, value|= 1,emptybuf<<=1;\
                                                 if ( offset>5 ) value<<=1, value|= 1,emptybuf<<=1;\
                                                 if ( offset>4 ) value<<=1, value|= 1,emptybuf<<=1;\
                                                 if ( offset>3 ) value<<=1, value|= 1,emptybuf<<=1;\
                                                 if ( offset>2 ) value<<=1, value|= 1,emptybuf<<=1;\
                                                 if ( offset>1 ) value<<=1, value|= 1,emptybuf<<=1;\
                                                 if ( offset>0 ) value<<=1, value|= 1 ,emptybuf<<=1;\
                                                array[pos]|=emptybuf&0xff;\
                                                array[pos+1]|=(emptybuf>>8)&0xff;\
                                                 array [ pos ]&=(value&0xff);\
                                                 array [ pos +1]&=(value>>8)&0xff;
                                                 
#define read_bits_from_array(array,width,pos,val) pos*=width;\
                                              offset=pos%8;\
                                                 pos/=8;\
                                              value=((array[pos]>>offset)+((array[pos+1]*0x100)>>offset))&0xff;\
                                              emptybuf=1;\
                                               if ( width> 1 ) emptybuf|=1<<1;\
                                                   if ( width> 2 ) emptybuf|=1<<2;\
                                                 if ( width> 3 ) emptybuf|=1<<3;\
                                                 if ( width> 4 ) emptybuf|=1<<4;\
                                                 if ( width> 5 ) emptybuf|=1<<5;\
                                                 if ( width> 6 ) emptybuf|=1<<6;\
                                                 if ( width> 7 ) emptybuf|=1<<7;\
                                                value&=emptybuf;\
                                              val=value;
                                              
                                              
                                              
int main(void)
{
  int n,n2;
 
    int weite,wert,position;
 
 while (1)
 {
  n2=0; 
   while (n2<10 )
   {
    n=0;
    while ( n < 8 )
    {
     printf("%d",(testarray[n2]>>n)&1);
     n++;
    }
    printf(" ");
    n2++;
   }
   scanf("%d %d %d",&weite,&position,&wert);
   assign_bits_to_array(testarray,weite,position,wert)
   scanf("%d %d",&weite,&position);
   read_bits_from_array(testarray,weite,position,wert)
  
   printf(" %d ",wert);
 
  }

}
