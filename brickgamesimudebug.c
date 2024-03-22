#include <time.h>
/* modify a compiler to print a circuit instead of assembler code generated?!
   not just trying to emulate, construct the brick game from this with granted and sold components?!*/

unsigned char ram[256];

 typedef
 struct
 {
  int bit[8];
  int bit_2[8];
  int carry[8];

 }binary_adder;
 
 int instr_ptr;
 
 int ram_address_from_addwork(binary_adder *regs)
 {
  return regs->bit_2[0]+regs->bit_2[1]*2+regs->bit_2[2]*4+regs->bit_2[3]*8+regs->bit_2[4]*16+regs->bit_2[5]*32+regs->bit_2[6]*64+regs->bit_2[7]*128;
  	
 	
 }

	int instruction[8];
 binary_adder address,DATAREG,ADDRREG;
 
int binary_addition(binary_adder *regs)
{
	int n=0;
	while (n<8)regs->carry[n]=0,n++;
	n=0;
	while (n<8)
	{
     regs->carry[n+1]=regs->bit[n]&regs->bit_2[n];
     regs->carry[n+1]|=regs->carry[n]&regs->bit_2[n];//Carrie-Ann?!
      regs->carry[n+1]|=regs->carry[n]&regs->bit[n];
      
	 regs->bit_2[n]=regs->bit_2[n]^regs->carry[n];//Carrie-Ann?!
     regs->bit_2[n]=regs->bit[n]^regs->bit_2[n];
  	 
	 
	 //regs.bit[n]=regs.bit[n]^regs.bit[n];
      //regs.bit[n+1]=regs[n].carry^regs.bit[n+1];
      //regs->bit[n+1]=regs->carry[n+1]|regs->bit[n+1];
   n++;
	}
 
 }

 
void instruction_fetch(); 

void loadin(binary_adder *address, binary_adder *data)
{
	int memaddress;
  memaddress=ram_address_from_addwork(address);//printf("Transferiere von Addresse %d %d",memaddress,ram[memaddress]);
    data->bit_2[0]=ram[memaddress]&1;
	data->bit_2[1]=(ram[memaddress]>>1)&1;
	data->bit_2[2]=(ram[memaddress]>>2)&1;
	data->bit_2[3]=(ram[memaddress]>>3)&1;
	data->bit_2[4]=(ram[memaddress]>>4)&1;
	data->bit_2[5]=(ram[memaddress]>>5)&1;
	data->bit_2[6]=(ram[memaddress]>>6)&1;
	data->bit_2[7]=(ram[memaddress]>>7)&1;
	
}
 
void storeout(binary_adder *address, binary_adder *data)
{
	int memaddress;
  memaddress=ram_address_from_addwork(address);
    ram[memaddress]=0;
	ram[memaddress]|=data->bit_2[0];
	ram[memaddress]|=data->bit_2[1]<<1;
	ram[memaddress]|=data->bit_2[2]<<2;
	ram[memaddress]|=data->bit_2[3]<<3;
	ram[memaddress]|=data->bit_2[4]<<4;
	ram[memaddress]|=data->bit_2[5]<<5;
	ram[memaddress]|=data->bit_2[6]<<6;
	ram[memaddress]|=data->bit_2[7]<<7;
	//printf("memaddress %d ist %d wohl",memaddress,ram[memaddress]);
	
} 
 
void instruction_operand_fetch(binary_adder *data);

instruction_process(void)
{
   binary_adder operand1, operand2;
 if ( !instruction[0]&&!instruction[1]&&!instruction[2]&&!instruction[3]&&
    !instruction[4]&&!instruction[5]&&!instruction[6]&&!instruction[7])
	{
		instruction_fetch();
	 instruction_operand_fetch(&operand1);
	 //printf("Lade in Datenregister");		
     loadin(&operand1,&DATAREG);
	 //printf("der Wert ist %d",ram_address_from_addwork(&DATAREG));
	}	
 else
  if ( instruction[0]&&!instruction[1]&&!instruction[2]&&!instruction[3]&&
    !instruction[4]&&!instruction[5]&&!instruction[6]&&!instruction[7])
	{
		instruction_fetch();
	 instruction_operand_fetch(&operand1);
     ADDRREG.bit_2[0]=operand1.bit_2[0];
     ADDRREG.bit_2[1]=operand1.bit_2[1];
     ADDRREG.bit_2[2]=operand1.bit_2[2];
     ADDRREG.bit_2[3]=operand1.bit_2[3];
     ADDRREG.bit_2[4]=operand1.bit_2[4];
     ADDRREG.bit_2[5]=operand1.bit_2[5];
     ADDRREG.bit_2[6]=operand1.bit_2[6];
     ADDRREG.bit_2[7]=operand1.bit_2[7];
     
       //printf("MOVA %d%d",ADDRREG.bit_2[0],ADDRREG.bit_2[1]),getchar();
	 //loadin(&operand1,&ADDRREG);		
	}	
 else
  if ( instruction[0]&&instruction[1]&&!instruction[2]&&!instruction[3]&&
    !instruction[4]&&!instruction[5]&&!instruction[6]&&!instruction[7])
	{
		instruction_fetch();
	 instruction_operand_fetch(&operand1);
	 //printf("schreibe heraus");
     storeout(&operand1,&DATAREG);		
	}	
 else
  if ( instruction[0]&&instruction[1]&&instruction[2]&&!instruction[3]&&
     !instruction[4]&&!instruction[5]&&!instruction[6]&&!instruction[7])
  {
     DATAREG.bit_2[0]=ADDRREG.bit_2[0];
     DATAREG.bit_2[1]=ADDRREG.bit_2[1];
     DATAREG.bit_2[2]=ADDRREG.bit_2[2];
     DATAREG.bit_2[3]=ADDRREG.bit_2[3];
     DATAREG.bit_2[4]=ADDRREG.bit_2[4];
     DATAREG.bit_2[5]=ADDRREG.bit_2[5];
     DATAREG.bit_2[6]=ADDRREG.bit_2[6];
     DATAREG.bit_2[7]=ADDRREG.bit_2[7];
     
     //printf("Datareg ist %d",DATAREG.bit_2[0],DATAREG.bit_2[1]),getchar();
  }
else
  if ( instruction[0]&&instruction[1]&&instruction[2]&&instruction[3]&&
     !instruction[4]&&!instruction[5]&&!instruction[6]&&!instruction[7])
  {
    instruction_fetch();
    instruction_operand_fetch(&operand1);
    instruction_fetch();
    instruction_operand_fetch(&operand2);
    if ( !((operand1.bit_2[0]^DATAREG.bit_2[0])||(operand1.bit_2[1]^DATAREG.bit_2[1])||(operand1.bit_2[2]^DATAREG.bit_2[2])||
	(operand1.bit_2[3]^DATAREG.bit_2[3])||(operand1.bit_2[4]^DATAREG.bit_2[4])||(operand1.bit_2[5]^DATAREG.bit_2[5])||
	(operand1.bit_2[6]^DATAREG.bit_2[6])||(operand1.bit_2[7]^DATAREG.bit_2[7])))
	{
      address.bit_2[0]=operand2.bit_2[0];
      address.bit_2[1]=operand2.bit_2[1];
      address.bit_2[2]=operand2.bit_2[2];
      address.bit_2[3]=operand2.bit_2[3];
      address.bit_2[4]=operand2.bit_2[4];
      address.bit_2[5]=operand2.bit_2[5];
      address.bit_2[6]=operand2.bit_2[6];
      address.bit_2[7]=operand2.bit_2[7];
	  //printf("Vergleich von DATAREG mit %d wahr %d",operand1.bit_2[0]);//,getchar();	
 	}	//printf("Vergleich von DATAREG mit %d falsch",operand1.bit_2[0]);//,getchar();	

  }
else
  if ( instruction[0]&&instruction[1]&&instruction[2]&&instruction[3]&&
     instruction[4]&&!instruction[5]&&!instruction[6]&&!instruction[7])
 {
   instruction_fetch();
   instruction_operand_fetch(&operand1);
   DATAREG.bit[0]=operand1.bit_2[0];
   DATAREG.bit[1]=operand1.bit_2[1];
   DATAREG.bit[2]=operand1.bit_2[2];
   DATAREG.bit[3]=operand1.bit_2[3];
   DATAREG.bit[4]=operand1.bit_2[4];
   DATAREG.bit[5]=operand1.bit_2[5];
   DATAREG.bit[6]=operand1.bit_2[6];
   DATAREG.bit[7]=operand1.bit_2[7];
   
   binary_addition(&DATAREG);
   //printf("Datareg ist jetzt %d",ram_address_from_addwork(&DATAREG));//,getchar();
 }
else
  if ( instruction[0]&&instruction[1]&&instruction[2]&&instruction[3]&&
     instruction[4]&&!instruction[5]&&instruction[6]&&!instruction[7])
 {
   instruction_fetch();
   instruction_operand_fetch(&operand1);
   DATAREG.bit[0]=ADDRREG.bit_2[0];
   DATAREG.bit[1]=ADDRREG.bit_2[1];
   DATAREG.bit[2]=ADDRREG.bit_2[2];
   DATAREG.bit[3]=ADDRREG.bit_2[3];
   DATAREG.bit[4]=ADDRREG.bit_2[4];
   DATAREG.bit[5]=ADDRREG.bit_2[5];
   DATAREG.bit[6]=ADDRREG.bit_2[6];
   DATAREG.bit[7]=ADDRREG.bit_2[7];
   
   binary_addition(&DATAREG);
   //printf("Datareg ist jetzt %d",ram_address_from_addwork(&DATAREG));//,getchar();
 }
else
  if ( instruction[0]&&instruction[1]&&instruction[2]&&instruction[3]&&
     instruction[4]&&instruction[5]&&instruction[6]&&instruction[7])
 {
   instruction_fetch();
   instruction_operand_fetch(&operand1);
   binary_adder count;
   count.bit_2[0]=operand1.bit_2[0];
   count.bit_2[1]=operand1.bit_2[1];
   count.bit_2[2]=operand1.bit_2[2];
   count.bit_2[3]=operand1.bit_2[3];
   count.bit_2[4]=operand1.bit_2[4];
   count.bit_2[5]=operand1.bit_2[5];
   count.bit_2[6]=operand1.bit_2[6];
   count.bit_2[7]=operand1.bit_2[7];
   while(! (count.bit_2[0]==0&&count.bit_2[1]==0&&count.bit_2[2]==0&&count.bit_2[3]==0&&count.bit_2[4]==0
            &&count.bit_2[5]==0&&count.bit_2[6]==0&&count.bit_2[7]==0) )
    {
    count.bit[0]=1, count.bit[1]=1;
	count.bit[2]=1,count.bit[3]=1;
	count.bit[4]=1,count.bit[5]=1;
   count.bit_2[6]=1,count.bit_2[7]=1;
   
    ADDRREG.bit[0]=1, ADDRREG.bit[1]=0;
	ADDRREG.bit[2]=0,ADDRREG.bit[3]=0;
	ADDRREG.bit[4]=0,ADDRREG.bit[5]=0;
   ADDRREG.bit[6]=0,ADDRREG.bit[7]=0;
   
   DATAREG.bit[0]=ADDRREG.bit_2[0];
   DATAREG.bit[1]=ADDRREG.bit_2[1];
   DATAREG.bit[2]=ADDRREG.bit_2[2];
   DATAREG.bit[3]=ADDRREG.bit_2[3];
   DATAREG.bit[4]=ADDRREG.bit_2[4];
   DATAREG.bit[5]=ADDRREG.bit_2[5];
   DATAREG.bit[6]=ADDRREG.bit_2[6];
   DATAREG.bit[7]=ADDRREG.bit_2[7];
   
   binary_addition(&count);
   binary_addition(&ADDRREG);
   storeout(&DATAREG,&ADDRREG);
  }
   //printf("Datareg ist jetzt %d",ram_address_from_addwork(&DATAREG));//,getchar();
 }
 else
  if ( instruction[0]&&instruction[1]&&instruction[2]&&instruction[3]&&
     instruction[4]&&instruction[5]&&instruction[6]&&!instruction[7])
{
	binary_adder cmp_buf;
 			instruction_fetch();
	 instruction_operand_fetch(&operand1);
	 ADDRREG.bit[0]=operand1.bit[0],
	 ADDRREG.bit[1]=operand1.bit[1],
	 ADDRREG.bit[2]=operand1.bit[2],
	 ADDRREG.bit[3]=operand1.bit[3],
	 ADDRREG.bit[4]=operand1.bit[4],
	 ADDRREG.bit[5]=operand1.bit[5],
	 ADDRREG.bit[6]=operand1.bit[6],
	 ADDRREG.bit[7]=operand1.bit[7],
      loadin(&ADDRREG,&cmp_buf);
	  binary_addition(&ADDRREG);
	  loadin(&ADDRREG,&DATAREG);
 			instruction_fetch();
	 instruction_operand_fetch(&operand2);
    
    if ( !((cmp_buf.bit_2[0]^DATAREG.bit_2[0])||(cmp_buf.bit_2[1]^DATAREG.bit_2[1])||(cmp_buf.bit_2[2]^DATAREG.bit_2[2])||
	(cmp_buf.bit_2[3]^DATAREG.bit_2[3])||(cmp_buf.bit_2[4]^DATAREG.bit_2[4])||(cmp_buf.bit_2[5]^DATAREG.bit_2[5])||
	(cmp_buf.bit_2[6]^DATAREG.bit_2[6])||(cmp_buf.bit_2[7]^DATAREG.bit_2[7])))
    {
	 address.bit_2[0]=operand2.bit_2[0];
      address.bit_2[1]=operand2.bit_2[1];
      address.bit_2[2]=operand2.bit_2[2];
      address.bit_2[3]=operand2.bit_2[3];
      address.bit_2[4]=operand2.bit_2[4];
      address.bit_2[5]=operand2.bit_2[5];
      address.bit_2[6]=operand2.bit_2[6];
      address.bit_2[7]=operand2.bit_2[7];
    }
	 
 }
	
}
 
 void instruction_operand_fetch(binary_adder *data)
 {
    data->bit_2[0]=instruction[0];
	data->bit_2[1]=instruction[1];
	data->bit_2[2]=instruction[2];
	data->bit_2[3]=instruction[3];
	data->bit_2[4]=instruction[4];
	data->bit_2[5]=instruction[5];
	data->bit_2[6]=instruction[6];
	data->bit_2[7]=instruction[7];
	
 }
 
void instruction_fetch()
{
	int memaddress;
 binary_adder ramline;
 ramline.bit_2[0]=address.bit_2[0];	
 ramline.bit_2[1]=address.bit_2[1];	
 ramline.bit_2[2]=address.bit_2[2];	
 ramline.bit_2[3]=address.bit_2[3];	
 ramline.bit_2[4]=address.bit_2[4];	
 ramline.bit_2[5]=address.bit_2[5];	
 ramline.bit_2[6]=address.bit_2[6];	
 ramline.bit_2[7]=address.bit_2[7];	

	
	memaddress=ram_address_from_addwork(&ramline);
//printf("transferiere von Addresse %d",memaddress);
	instruction[0]=ram[memaddress]&1;
	instruction[1]=(ram[memaddress]>>1)&1;
	instruction[2]=(ram[memaddress]>>2)&1;
	instruction[3]=(ram[memaddress]>>3)&1;
	instruction[4]=(ram[memaddress]>>4)&1;
	instruction[5]=(ram[memaddress]>>5)&1;
	instruction[6]=(ram[memaddress]>>6)&1;
	instruction[7]=(ram[memaddress]>>7)&1;
	

   address.bit[0]=1;
   address.bit[1]=0;
   address.bit[2]=0;
   address.bit[3]=0;
   address.bit[4]=0;
   address.bit[5]=0;
   address.bit[6]=0;
   address.bit[7]=0;
    binary_addition(&address);
   
}


unsigned char screen[7*20];

void videoclear(void)
{
  int n=0;
 while(n<7*16)
 {
  ram[n+128]=0;
  n++; 
 }
	
}

 
void freeze(void)
{
  int n=0;
 if ( ram[97]==1)
 {
 while(n<7*16)
 {
  if (screen[n]!=' ')ram[n+128]=1;
  n++; 
 }
 ram[97]=0;	
 }
	
	
}

int drawscreen(int nodraw)
{
 int n=0;
    int hvflip;
	 if (ram[96])hvflip=1; else hvflip=0;
  int errorfill=0;
 while(n<7*16)
 {
  if (n<7*15)screen[n]=ram[n+128]==0 ? ' ' : 177;
  else screen[n]=177,ram[n+128]=1;
  n++; 
 }
 n=0;
 while(n<9)
 {
 	if ( hvflip==1)
 	{
 	 (ram[128+(ram[7*16+9+128]*7> 15*7 ? 15*7 : ram[7*16+9+128]*7)+ram[7*16+9+1+128]+(8-n)%3+((8-n)/3)*7]==1 && ram[7*16+128+(8-n)]==1)/*||ram[7*16+9+128]>13*/ ? errorfill=1 : 0;//pure?
      //if(errorfill==1)break;
    (screen[(ram[7*16+9+128]*7> 15*7 ? 15*7 : ram[7*16+9+128]*7)+ram[7*16+9+1+128]+(8-n)%3+(n/3)*7]=ram[7*16+128+(8-n)]==1? 219 : ' '),n++;
   }
   	else if ( hvflip==0)
 	{
 		//printf("Koordinaten %d Wert %d\n",ram[128+ram[7*16+9+128]*7+ram[7*16+9+1+128]+n%3+(n/3)*7],ram[7*16+9+128]*7+ram[7*16+9+1+128]+n%3+(n/3)*7);
 	 (ram[128+(ram[7*16+9+128]*7> 15*7 ? 15*7 : ram[7*16+9+128]*7)+ram[7*16+9+1+128]+n%3+(n/3)*7]==1 && ram[7*16+128+n]==1)/*||ram[7*16+9+128]>13*/ ? errorfill=1 : 0;//pure?
      //if(errorfill==1)break;
    (screen[(ram[7*16+9+128]*7> 15*7 ? 15*7 : ram[7*16+9+128]*7) +ram[7*16+9+1+128]+n%3+(n/3)*7]=ram[7*16+128+n]==1? 219 : ' '),n++;
   }
 } 
 n=0;
 if ( nodraw==0)
 {
   system("cls");
  while ( n<7*15)printf("%c%c%c%c%c%c%c|\n",screen[n],screen[n+1],screen[n+2],screen[n+3],screen[n+4],screen[n+5],screen[n+6]),n+=7;
  printf("-------");
 }
 //printf("%d ist errorfill",errorfill);//getchar();
 ram[7*16+9+3+128]=errorfill;
  if ( errorfill==1&&ram[7*16+9+128]*7<=1)printf("eigentlich game over"),getchar();
 
 return errorfill;
}

int main(void)
{
	#define MOVD 0b0
	#define MOVA 0b1
	#define MOVDO 0b11
	#define MOVDA 0b111
	#define JCMP  0b1111
	#define ADD  0b11111
	#define ADDDA  0b111111
	#define STORB 0b1111111
	#define JCMPS 0b1111110
	
  #define RAM(n,c) ram[n]=c;
  
  address.bit_2[0]=0;
  address.bit_2[1]=0;
  address.bit_2[2]=0;
  address.bit_2[3]=0;
  address.bit_2[4]=0;
  address.bit_2[5]=0;
  address.bit_2[6]=0;
  address.bit_2[7]=0;
  ram[97]=0;
  ram[7*16+3+9+128]=0;
  
  ram[7*16+128]=1;
  ram[7*16+128+1]=1;
  ram[7*16+128+2]=1;
  ram[7*16+128+3]=1;
  ram[7*16+128+4]=1;
  ram[7*16+128+5]=1;
  ram[7*16+128+6]=1;
  ram[7*16+128+7]=1;
  ram[7*16+128+8]=1;
  
  RAM(0,MOVD) RAM(1,7*16+9+2+128)
  RAM(2,MOVDO) RAM(3,7*16+9+1+128)
  RAM(4,MOVD) RAM(5,7*16+9+128)
  RAM(6,MOVDO) RAM(7,98)
  RAM(8,ADD) RAM(9,1)
  RAM(10,MOVDO) RAM(11,7*16+9+128)
  RAM(12,MOVD) RAM(13,7*16+9+1+128)
  RAM(14,MOVDO) RAM(15,99)
  RAM(16,MOVD) RAM(17,7*16+3+9+128)
  RAM(18,JCMP) RAM(19,1) RAM(20,27)
  RAM(21,MOVA) RAM(22,1)
  RAM(23,MOVDA)
  RAM(24,JCMP) RAM(25,1) RAM(26,0)
  RAM(27,MOVD) RAM(28,98)
  RAM(29,MOVDO) RAM(30,7*16+9+128)
  RAM(31,MOVD) RAM(32,99)
  RAM(33,MOVDO) RAM(34,7*16+9+1+128)
   RAM(35,MOVA) RAM(36,1)
   //RAM(35,MOVD) RAM(36,22)
   RAM(37,MOVDA)
  RAM(38,MOVDO) RAM(39,97)
  RAM(40,MOVA) RAM(41,0)
  RAM(42,MOVDA)
  RAM(43,MOVDO) RAM(44,7*16+3+9+128)
  RAM(45,MOVDO) RAM(46,7*16+9+128)
  RAM(47,MOVA) RAM(49,0)
  RAM(50,JCMP) RAM(51,0) RAM(52,0)
  
  /*
  
  RAM(0,MOVA) RAM(0,0)
  RAM(0,MOVDA)
  RAM(0,MOVDO) RAM(0,100)
  RAM(0,MOVD) RAM(0,100)
  RAM(0,ADD) RAM(0,1)
  RAM(0,MOVDO) RAM(0,100)
  RAM(0,MOVA) RAM(0,100)
  RAM(0,MOVA) RAM(0,101)
  RAM(MOVDA)
  RAM(0,ADDDA)
  RAM(0,MOVDO) RAM(0,102)
  RAM(0,MOVA) RAM(0,102)
  RAM(0,JCMP) RAM(0,1) RAM(0,dada)
  
  RAM(0,JCMP) RAM(0,7) RAM(0,da)
  
  
  
  */
  
  unsigned char pos=0,rotate=0;
  int npos=0;
  
   unsigned char tileblockstream[]={1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1};

 videoclear();
  
  int rotate_old, pos_old;
  clock_t test;
   ram[7*16+9+1+128]=0;
  
  while(1)
  {
  	test=clock()+50;
     rotate_old=rotate,
     pos_old=pos;
  
  	if (kbhit())
  	{
  	 switch(getch())
	   {
	   	case 'b': pos > 0 ? pos-- : pos;break;
	   	case 'm': pos<4 ? pos++ : pos; break;
	   	case 'n' : rotate=(rotate+1)%2; break;
		   }
	  }
	  ram[96]=rotate;
	  ram[95]=tileblockstream[npos++%27];
	  pos_old=ram[7*16+9+1+128];
  #define GETPOLLINPUT (ram[7*16+9+2+128]=pos);
  GETPOLLINPUT
   ram[7*16+9+1+128]=pos;
  if (drawscreen(1)==1)ram[96]=rotate=rotate_old,pos=pos_old;
   ram[7*16+9+1+128]=pos_old;
 
   drawscreen(npos%8);
   freeze();
  instruction_fetch();
  instruction_process();
  while(test>clock());
  }
  

}
