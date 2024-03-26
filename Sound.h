unsigned sb_base_port=0;

struct
{
 unsigned char data[64000];
 unsigned long pos;
}sound_buf;



int sb_init (void)
{
 clock_t delay,get_rate ;
 unsigned char cmd ;
 unsigned int ctr;

 sb_base_port=0x216;

 while(1)
 {
  outportb(sb_base_port,1 );
  delay = clock()+100000;
  while ( delay > clock() );
  outportb(sb_base_port,0 ) ;
  delay = clock()+100000;
  while( delay >clock() );

  cmd=inportb( sb_base_port+0x4 );
  if( cmd==0xaa) break;

  sb_base_port+=0x10;

  if ( sb_base_port <= 0x286 ) continue;
  return 1;
 }

 sb_base_port-=0x6;
 outportb( sb_base_port+0x4,0 );
 outportb( sb_base_port+0x5,0 );
 outportb( sb_base_port+0xc,0xd1 );

 while (sound_buf.pos<64000) sound_buf.data[sound_buf.pos]=127,
                              sound_buf.pos++;
 sound_buf.pos=0;

 return 0;


}


void mix(unsigned char vol)
{
 //fr Version 1...
 outportb(sb_base_port+0x4,0x2),
 outportb(sb_base_port+0x5,vol>>4),
 outportb(sb_base_port+0x4,0xa),
 outportb(sb_base_port+0x5,vol>>4);

 //fr Version 2...
 outportb(sb_base_port+0x4,0x4),
 outportb(sb_base_port+0x5,(vol>>4)+(vol>>4)*0x10),
 outportb(sb_base_port+0x4,0x22),
 outportb(sb_base_port+0x5,(vol>>4)+(vol>>4)*0x10);

 //fr Version 3...
 outportb(sb_base_port+0x4,0x30),
 outportb(sb_base_port+0x5,vol),
 outportb(sb_base_port+0x4,0x31),
 outportb(sb_base_port+0x5,vol),
 outportb(sb_base_port+0x4,0x32),
 outportb(sb_base_port+0x5,vol),
 outportb(sb_base_port+0x4,0x33),
 outportb(sb_base_port+0x5,vol);

 return;

}




inline void sb_play( unsigned long len)
{
 static char i_pos=0;

 while( len>0)
 {
  if( i_pos%4 ==0 )
  {
   while(inportb(sb_base_port+0xc)>>7!= 0);
   outportb(sb_base_port+0xc, 0x10 );
   outportb(sb_base_port+0xc, sound_buf.data[sound_buf.pos] );
  }
  else
  if( i_pos%4 ==1 )
  {
   while(inportb(sb_base_port+0xc)>>7!= 0);
   outportb(sb_base_port+0xc, 0x10 );
   outportb(sb_base_port+0xc, (sound_buf.data[sound_buf.pos]+sound_buf.data[sound_buf.pos]+sound_buf.data[sound_buf.pos]+sound_buf.data[sound_buf.pos+1])/4 );
  }
  else
  if( i_pos%4 ==2 )
  {
   while(inportb(sb_base_port+0xc)>>7!= 0);
   outportb(sb_base_port+0xc, 0x10 );
   outportb(sb_base_port+0xc, (sound_buf.data[sound_buf.pos]+sound_buf.data[sound_buf.pos]+sound_buf.data[sound_buf.pos+1]+sound_buf.data[sound_buf.pos+1])/4 );
  }
  else
  if( i_pos%4 ==3 )
  {
   while(inportb(sb_base_port+0xc)>>7!= 0);
   outportb(sb_base_port+0xc, 0x10 );
   outportb(sb_base_port+0xc, (sound_buf.data[sound_buf.pos]+sound_buf.data[sound_buf.pos+1]+sound_buf.data[sound_buf.pos+1]+sound_buf.data[sound_buf.pos+1])/4 );
   sound_buf.data[sound_buf.pos]=127;
  }

  ++i_pos= i_pos%4, --len;

  if( i_pos%4 ==0 )
  {
   ++sound_buf.pos=sound_buf.pos%64000;
  }

 }


}


void sb_fill(unsigned char *sample, unsigned len)
{
 unsigned long pos=sound_buf.pos;
 for(;len>0;len--)
  sound_buf.data[++pos=pos%64000]+= (*sample++)-127;

}
