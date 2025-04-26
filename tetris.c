#include <time.h>

/* modify a compiler to print a circuit instead of assembler code generated?!

not just trying to emulate, construct the brick game from this with granted and sold components?!*/

int posreset;

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

//if ( ram_address_from_addwork(address)==57)getchar();

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

printf("memaddress %d ",ram_address_from_addwork(&address));

//if (ram_address_from_addwork(&address)>50 && ram_address_from_addwork(&address)<101)getchar();

binary_adder operand1, operand2;

/* MOVD */

if ( !instruction[0]&&!instruction[1]&&!instruction[2]&&!instruction[3]&&

!instruction[4]&&!instruction[5]&&!instruction[6]&&!instruction[7])

{

instruction_fetch();

instruction_operand_fetch(&operand1);

//printf("Lade in Datenregister");

loadin(&operand1,&DATAREG);

//printf("der Wert ist %d",ram_address_from_addwork(&DATAREG));

}

else /* MOVA */

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

//printf("MOVA %d",ram_address_from_addwork(&ADDRREG)),getchar();

//loadin(&operand1,&ADDRREG);

}

else /* MOVDO */

if ( instruction[0]&&instruction[1]&&!instruction[2]&&!instruction[3]&&

!instruction[4]&&!instruction[5]&&!instruction[6]&&!instruction[7])

{

instruction_fetch();

instruction_operand_fetch(&operand1);

//printf("schreibe heraus");

storeout(&operand1,&DATAREG);

}

else /* MOVDA */

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

//printf("MOVDA %d",ram_address_from_addwork(&DATAREG)),getchar();

}

else /* JCMP */

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

} //printf("Vergleich von DATAREG mit %d falsch",operand1.bit_2[0]);//,getchar();

}

else /* MOVAO */

if ( !instruction[0]&&!instruction[1]&&instruction[2]&&instruction[3]&&

instruction[4]&&instruction[5]&&instruction[6]&&instruction[7])

{

instruction_fetch();

instruction_operand_fetch(&operand1);

//printf("schreibe heraus"),getchar();

storeout(&operand1,&ADDRREG);

}

else /* ADD */

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

else /* MOVAA*/

if ( instruction[0]&&instruction[1]&&instruction[2]&&instruction[3]&&

instruction[4]&&instruction[5]&&instruction[6]&&!instruction[7])

{

instruction_fetch();

instruction_operand_fetch(&operand1);

//printf("schreibe heraus");

loadin(&operand1,&ADDRREG);

//binary_addition(&DATAREG);

//printf("Datareg ist jetzt %d",ram_address_from_addwork(&DATAREG));//,getchar();

}

else /* STOREBLOCK */

if ( instruction[0]&&instruction[1]&&instruction[2]&&instruction[3]&&

instruction[4]&&instruction[5]&&instruction[6]&&instruction[7])

{

instruction_fetch();

instruction_operand_fetch(&operand1);

binary_adder count,buf;

/*

count.bit_2[0]=operand1.bit_2[0];

count.bit_2[1]=operand1.bit_2[1];

count.bit_2[2]=operand1.bit_2[2];

count.bit_2[3]=operand1.bit_2[3];

count.bit_2[4]=operand1.bit_2[4];

count.bit_2[5]=operand1.bit_2[5];

count.bit_2[6]=operand1.bit_2[6];

count.bit_2[7]=operand1.bit_2[7];

*/

count.bit_2[0]=ADDRREG.bit_2[0];

count.bit_2[1]=ADDRREG.bit_2[1];

count.bit_2[2]=ADDRREG.bit_2[2];

count.bit_2[3]=ADDRREG.bit_2[3];

count.bit_2[4]=ADDRREG.bit_2[4];

count.bit_2[5]=ADDRREG.bit_2[5];

count.bit_2[6]=ADDRREG.bit_2[6];

count.bit_2[7]=ADDRREG.bit_2[7];

DATAREG.bit_2[0]=ADDRREG.bit_2[0];

DATAREG.bit_2[1]=ADDRREG.bit_2[1];

DATAREG.bit_2[2]=ADDRREG.bit_2[2];

DATAREG.bit_2[3]=ADDRREG.bit_2[3];

DATAREG.bit_2[4]=ADDRREG.bit_2[4];

DATAREG.bit_2[5]=ADDRREG.bit_2[5];

DATAREG.bit_2[6]=ADDRREG.bit_2[6];

DATAREG.bit_2[7]=ADDRREG.bit_2[7];

DATAREG.bit[0]=operand1.bit_2[0];

DATAREG.bit[1]=operand1.bit_2[1];

DATAREG.bit[2]=operand1.bit_2[2];

DATAREG.bit[3]=operand1.bit_2[3];

DATAREG.bit[4]=operand1.bit_2[4];

DATAREG.bit[5]=operand1.bit_2[5];

DATAREG.bit[6]=operand1.bit_2[6];

DATAREG.bit[7]=operand1.bit_2[7];

//binary_addition(&count);

binary_addition(&DATAREG);
DATAREG.bit[0]=1,DATAREG.bit[1]=1,DATAREG.bit[2]=1,
DATAREG.bit[3]=1,DATAREG.bit[4]=1,DATAREG.bit[5]=1,
DATAREG.bit[6]=1,DATAREG.bit[7]=1;
ADDRREG.bit[0]=1,ADDRREG.bit[1]=1,ADDRREG.bit[2]=1,
ADDRREG.bit[3]=1,ADDRREG.bit[4]=1,ADDRREG.bit[5]=1,
ADDRREG.bit[6]=1,ADDRREG.bit[7]=1;

binary_addition(&DATAREG);
binary_addition(&ADDRREG);


while(! (count.bit_2[0]==0&&count.bit_2[1]==0&&count.bit_2[2]==0&&count.bit_2[3]==0&&count.bit_2[4]==0

&&count.bit_2[5]==0&&count.bit_2[6]==0&&count.bit_2[7]==0) )

{

count.bit[0]=1, count.bit[1]=1;

count.bit[2]=1,count.bit[3]=1;

count.bit[4]=1,count.bit[5]=1;

count.bit[6]=1,count.bit[7]=1;

loadin(&ADDRREG,&buf);

if(DATAREG.bit_2[7]==1)storeout(&DATAREG,&buf);

ADDRREG.bit[0]=1, ADDRREG.bit[1]=1;

ADDRREG.bit[2]=1,ADDRREG.bit[3]=1;

ADDRREG.bit[4]=1,ADDRREG.bit[5]=1;

ADDRREG.bit[6]=1,ADDRREG.bit[7]=1;

DATAREG.bit[0]=1, DATAREG.bit[1]=1;

DATAREG.bit[2]=1,DATAREG.bit[3]=1;

DATAREG.bit[4]=1,DATAREG.bit[5]=1;

DATAREG.bit[6]=1,DATAREG.bit[7]=1;

/*

DATAREG.bit[0]=ADDRREG.bit_2[0];

DATAREG.bit[1]=ADDRREG.bit_2[1];

DATAREG.bit[2]=ADDRREG.bit_2[2];

DATAREG.bit[3]=ADDRREG.bit_2[3];

DATAREG.bit[4]=ADDRREG.bit_2[4];

DATAREG.bit[5]=ADDRREG.bit_2[5];

DATAREG.bit[6]=ADDRREG.bit_2[6];

DATAREG.bit[7]=ADDRREG.bit_2[7];

*/

binary_addition(&DATAREG);

binary_addition(&ADDRREG);

binary_addition(&count);

}

//printf("Datareg ist jetzt %d",ram_address_from_addwork(&DATAREG));//,getchar();

}

else /* JCMPS */

if ( !instruction[0]&&instruction[1]&&instruction[2]&&instruction[3]&&

instruction[4]&&instruction[5]&&instruction[6]&&instruction[7])

{

binary_adder cmp_buf,adr_buf;

instruction_fetch();

instruction_operand_fetch(&operand1);

loadin(&ADDRREG,&adr_buf);

//printf("%d %d %d\n",ram_address_from_addwork(&ADDRREG),ram_address_from_addwork(&adr_buf),ram[91]);

//loadin(&adr_buf,&ADDRREG);

/*adr_buf.bit_2[0]= */ADDRREG.bit_2[0]=adr_buf.bit_2[0],

/*adr_buf.bit_2[1]= */ADDRREG.bit_2[1]=adr_buf.bit_2[1],

/*adr_buf.bit_2[2]= */ADDRREG.bit_2[2]=adr_buf.bit_2[2],

/*adr_buf.bit_2[3]= */ADDRREG.bit_2[3]=adr_buf.bit_2[3],

/*adr_buf.bit_2[4]= */ADDRREG.bit_2[4]=adr_buf.bit_2[4],

/*adr_buf.bit_2[5]= */ADDRREG.bit_2[5]=adr_buf.bit_2[5],

/*adr_buf.bit_2[6]= */ADDRREG.bit_2[6]=adr_buf.bit_2[6],

/*adr_buf.bit_2[7]= */ADDRREG.bit_2[7]=adr_buf.bit_2[7],

/*adr_buf.bit_2[0]= */ADDRREG.bit[0]=operand1.bit_2[0],

/*adr_buf.bit_2[1]= */ADDRREG.bit[1]=operand1.bit_2[1],

/*adr_buf.bit_2[2]= */ADDRREG.bit[2]=operand1.bit_2[2],

/*adr_buf.bit_2[3]= */ADDRREG.bit[3]=operand1.bit_2[3],

/*adr_buf.bit_2[4]= */ADDRREG.bit[4]=operand1.bit_2[4],

/*adr_buf.bit_2[5]= */ADDRREG.bit[5]=operand1.bit_2[5],

/*adr_buf.bit_2[6]= */ADDRREG.bit[6]=operand1.bit_2[6],

/*adr_buf.bit_2[7]= */ADDRREG.bit[7]=operand1.bit_2[7],

binary_addition(&ADDRREG);

// printf("%d %d\n",ram_address_from_addwork(&ADDRREG),ram_address_from_addwork(&adr_buf));

int linestr_flag;

linestr_flag=1;

//printf("binda"),getchar(),getchar();

while(

((adr_buf.bit_2[0]^ADDRREG.bit_2[0])||(adr_buf.bit_2[1]^ADDRREG.bit_2[1])||(adr_buf.bit_2[2]^ADDRREG.bit_2[2])||

(adr_buf.bit_2[3]^ADDRREG.bit_2[3])||(adr_buf.bit_2[4]^ADDRREG.bit_2[4])||(adr_buf.bit_2[5]^ADDRREG.bit_2[5])||

(adr_buf.bit_2[6]^ADDRREG.bit_2[6])||(adr_buf.bit_2[7]^ADDRREG.bit_2[7])))

{

ADDRREG.bit[0]=1, ADDRREG.bit[1]=1;

ADDRREG.bit[2]=1,ADDRREG.bit[3]=1;

ADDRREG.bit[4]=1,ADDRREG.bit[5]=1;

ADDRREG.bit[6]=1,ADDRREG.bit[7]=1;

binary_addition(&ADDRREG);

loadin(&ADDRREG,&cmp_buf);

if ( ((cmp_buf.bit_2[0]^DATAREG.bit_2[0])||(cmp_buf.bit_2[1]^DATAREG.bit_2[1])||(cmp_buf.bit_2[2]^DATAREG.bit_2[2])||

(cmp_buf.bit_2[3]^DATAREG.bit_2[3])||(cmp_buf.bit_2[4]^DATAREG.bit_2[4])||(cmp_buf.bit_2[5]^DATAREG.bit_2[5])||

(cmp_buf.bit_2[6]^DATAREG.bit_2[6])||(cmp_buf.bit_2[7]^DATAREG.bit_2[7])))

{

linestr_flag=0;

// break;

}

//printf("binda %d %d %d",ram_address_from_addwork(&DATAREG),ram_address_from_addwork(&cmp_buf),ram_address_from_addwork(&ADDRREG)),getchar();

} instruction_fetch();

instruction_operand_fetch(&operand2);

if ( linestr_flag==1)

{

//printf("bin an Zeile loeschen gesprungen %d",ram_address_from_addwork(&ADDRREG)),getchar();

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

if ( ram[107]==1)

{

while(n<7*16)

{

if (screen[n]!=' ')ram[n+128]=1;

n++;

}

ram[107]=0;

}

}

int rightcolumn(void)

{

int hvflip3;

if (ram[106])hvflip3=1; else hvflip3=0;

int middle=0;

if ( (ram[16*7+128+1]==1||ram[16*7+128+4]==1||ram[16*7+128+7]==1)) middle=1;

if ( (ram[16*7+128+1]==1||ram[16*7+128+4]==1||ram[16*7+128+7]==1))middle=1;

if ( hvflip3==1&&(ram[16*7+128]==1||ram[16*7+128+3]==1||ram[16*7+128+6]==1)) return 0;

if ( hvflip3==0&&(ram[16*7+128+2]==1||ram[16*7+128+5]==1||ram[16*7+128+8]==1)) return 0;

if ( middle==1)return 1;

return 2;

}

int leftcolumn(void)

{

int middle=0;

int hvflip2;

if (ram[106])hvflip2=1; else hvflip2=0;

if ( (ram[16*7+128+1]==1||ram[16*7+128+4]==1||ram[16*7+128+7]==1))middle=1;

if ( (ram[16*7+128+1]==1||ram[16*7+128+4]==1||ram[16*7+128+7]==1))middle=1;

if ( hvflip2==0&&(ram[16*7+128]==1||ram[16*7+128+3]==1||ram[16*7+128+6]==1)) return 0;

if ( hvflip2==1&&(ram[16*7+128+2]==1||ram[16*7+128+5]==1||ram[16*7+128+8]==1)) return 0;

if (middle==1)return 1;

return 2;

}

int drawscreen(int nodraw)

{

int n=0;

int hvflip;

if (ram[106])hvflip=1; else hvflip=0;

int errorfill=0;

while(n<7*16)

{

if (n<7*15)screen[n]=ram[n+128]==0 ? ' ' : 177;

else screen[n]=177,ram[n+128]=1;

n++;

}

n=0/*leftcolumn()*3*/;

while(n<9/*rightcolumn()*3+3*/)

{

if ( hvflip==1)

{

if((ram[7*16+128+(8-n)]==0||ram[7*16+9+1+128]+(n)%3<=6)

||ram[7*16+9+1+128]<=8)

{

(ram[128+(ram[7*16+9+128]*7> 15*7 ? 15*7 : ram[7*16+9+128]*7)+ram[7*16+9+1+128]-2+(n)%3+((n)/3)*7]==1 && ram[7*16+128+(8-n)]==1)/*||ram[7*16+9+128]>13*/ ? errorfill=1 : 0;//pure?

//if(errorfill==1)break;

(screen[(ram[7*16+9+128]*7> 15*7 ? 15*7 : ram[7*16+9+128]*7)+ram[7*16+9+1+128]-2+(n)%3+((n)/3)*7]=ram[7*16+128+(8-n)]==1? 219 :

screen[(ram[7*16+9+128]*7> 15*7 ? 15*7 : ram[7*16+9+128]*7)+ram[7*16+9+1+128]-2+(n)%3+((n)/3)*7]);

}else posreset=1;

}

else

if ( hvflip==0)

{

if ((ram[7*16+128+(n)]==0||ram[7*16+9+1+128]-2+(n)%3<=6)

||ram[7*16+9+1+128]<=8)

{

//printf("Koordinaten %d Wert %d\n",ram[128+ram[7*16+9+128]*7+ram[7*16+9+1+128]+n%3+(n/3)*7],ram[7*16+9+128]*7+ram[7*16+9+1+128]+n%3+(n/3)*7);

(ram[128+(ram[7*16+9+128]*7> 15*7 ? 15*7 : ram[7*16+9+128]*7)+ram[7*16+9+1+128]-2+n%3+(n/3)*7]==1 && ram[7*16+128+n]==1)/*||ram[7*16+9+128]>13*/ ? errorfill=1 : 0;//pure?

//if(errorfill==1)break;

(screen[(ram[7*16+9+128]*7> 15*7 ? 15*7 : ram[7*16+9+128]*7) +ram[7*16+9+1+128]-2+n%3+(n/3)*7]=ram[7*16+128+n]==1? 219 : screen[(ram[7*16+9+128]*7> 15*7 ? 15*7 : ram[7*16+9+128]*7) +ram[7*16+9+1+128]-2+n%3+(n/3)*7]);

}else posreset=1;

}

n++;

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

if ( errorfill==1)ram[103]=1;

if ( errorfill==1&&ram[7*16+9+128]<=2)printf("eigentlich game over"),getchar();

return errorfill;

}

int main(void)

{

#define MOVD 0b0

#define MOVA 0b1

#define MOVDO 0b11

#define MOVDA 0b111

#define JCMP 0b1111

#define ADD 0b11111

#define MOVAA 0b1111111

#define STORB 0b11111111

#define JCMPS 0b11111110

#define MOVAO 0b11111100

#define RAM(n,c) ram[n]=c;

address.bit_2[0]=0;

address.bit_2[1]=0;

address.bit_2[2]=0;

address.bit_2[3]=0;

address.bit_2[4]=0;

address.bit_2[5]=0;

address.bit_2[6]=0;

address.bit_2[7]=0;

ram[107]=0;

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

RAM(6,ADD) RAM(7,1)

RAM(8,MOVDO) RAM(9,7*16+9+128)

RAM(10,MOVD) RAM(11,7*16+3+9+128)

RAM(12,JCMP) RAM(13,1) RAM(14,28)

RAM(15,MOVD) RAM(16,7*16+9+128)

RAM(17,MOVDO) RAM(18,108)

RAM(19,MOVD) RAM(20,7*16+9+1+128)

RAM(21,MOVDO) RAM(22,109)

RAM(23,MOVD) RAM(24,102)

RAM(25,JCMP) RAM(26,1) RAM(27,0)

RAM(28,MOVD) RAM(29,108)

RAM(30,MOVDO) RAM(31,7*16+9+128)

RAM(32,MOVD) RAM(33,109)

//RAM(34,MOVDO) RAM(35,7*16+9+1+128)

RAM(34,MOVDA) RAM(35,MOVDA)

//

RAM(36,MOVA) RAM(37,1)

//RAM(35,MOVD) RAM(36,22)

// RAM(37,MOVDA)//nop-Ersatz teilweise?!

RAM(38,MOVAO) RAM(39,107)

RAM(40,MOVD) RAM(41,107)

RAM(42,JCMP) RAM(43,1) RAM(44,40)

RAM(45,MOVA) RAM(46,0)

RAM(47,MOVDA)

RAM(48,MOVDO) RAM(49,7*16+3+9+128)

RAM(50,MOVDO) RAM(51,7*16+9+128)

//RAM(47,MOVA) RAM(48,0)

//RAM(49,JCMP) RAM(50,0) RAM(51,0)

RAM(52,MOVA) RAM(53,128)

RAM(54,MOVDA)

RAM(55,MOVDO) RAM(56,101)

RAM(57,MOVD) RAM(58,102)

RAM(59,MOVA) RAM(60,101)

RAM(61,JCMPS) RAM(62,7) RAM(63,81)

RAM(64,MOVD) RAM(65,101)

RAM(66,MOVDO) RAM(67,101)

RAM(68,ADD) RAM(69,7)

RAM(70,MOVDO) RAM(71,101)

RAM(72,JCMP) RAM(73,7*15+128) RAM(74,0)

RAM(75,MOVA) RAM(76,0)

RAM(77,MOVDA)

RAM(78,JCMP) RAM(79,0) RAM (80,57)

RAM(81,MOVAA) RAM(82,101)

RAM(83,MOVDA)

RAM(84,MOVDA)

RAM(85,MOVDA)

RAM(86,STORB) RAM(87,7)

RAM(88,MOVA) RAM(89,0)

RAM(90,MOVDA)

RAM(91,JCMP) RAM(92,0) RAM(93,57)

RAM(94,MOVD) RAM(95,102)

RAM(96,MOVDA)

RAM(97,JCMP) RAM(98,0) RAM(99,0)

RAM(100,102)

RAM(102,1)

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

unsigned char pos=2,rotate=0;

int npos=11,npos2=11;

unsigned char tileblockstream[]={1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1};

videoclear();

int rotate_old, pos_old;

clock_t test;

ram[7*16+9+1+128]=0;

pos_old=pos,

rotate_old=rotate;

posreset=2;

int fillcount=0;

while(1)

{

test=clock()+10;

printf(">%d<",rightcolumn());

if ( pos<2-leftcolumn() )pos=2-leftcolumn();

if ( pos>6+rightcolumn()) pos=6+rightcolumn();

if (kbhit())

{

switch(getch())

{

case 'b': pos > 2-leftcolumn() &&ram[107]==0&&ram[7*16+3+9+128]==0? pos-- : pos;break;

case 'm': pos<6+rightcolumn() &&ram[107]==0&&ram[7*16+3+9+128]==0? pos++ : pos; break;

case 'n' : rotate=(rotate+1)%2; break;

}

}

if ( ram[103]==1)

{

if (ram[7*16+128+9]==0)

ram[7*16+128+(fillcount++)]=ram[105];

if ( fillcount==9)fillcount=0,ram[103]=0;

}

if(ram[107]==0)ram[106]=rotate;

ram[105]=tileblockstream[(npos++)%27];

//pos_old=ram[7*16+9+1+128];

#define GETPOLLINPUT (ram[7*16+9+2+128]=pos);

GETPOLLINPUT

//pos_old=ram[7*16+9+1+128];

ram[7*16+9+1+128]=pos;

if (drawscreen(1)==1/*||posreset==1*/)ram[106]=rotate=rotate_old,pos=pos_old,ram[7*16+9+1+128]=pos_old;else pos_old=pos,rotate_old=rotate,posreset=0;

drawscreen(npos%8);

freeze();

if (npos%8==0) printf("\n _____\n |%c%c%c|\n |%c%c%c|\n |%c%c%c|\n -----",ram[16*7+128]+0x30,ram[16*7+128+1]+0x30,ram[16*7+128+2]+0x30,ram[16*7+128+3]+0x30,ram[16*7+128+4]+0x30

,ram[16*7+128+5]+0x30,ram[16*7+128+6]+0x30,ram[16*7+128+7]+0x30,ram[16*7+128+8]+0x30);

if (npos%8==0) printf("\n _____\n |%c%c%c|\n |%c%c%c|\n |%c%c%c|\n -----",

tileblockstream[(npos)%27]+177, tileblockstream[(npos+1)%27]+177, tileblockstream[(npos+2)%27]+177, tileblockstream[(npos+3)%27]+177,

tileblockstream[(npos+4)%27]+177, tileblockstream[(npos+5)%27]+177, tileblockstream[(npos+6)%27]+177, tileblockstream[(npos+7)%27]+177,

tileblockstream[(npos+8)%27]+177);

printf("%d ",ram[101]);

instruction_fetch();

instruction_process();

pos_old=ram[7*16+9+1+128];

while(test>clock());

}

}
