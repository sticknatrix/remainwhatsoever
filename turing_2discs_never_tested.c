#include <stdio.h>

unsigned int statechange[14][3]=
                            {
                            {0,0,3},
                            {0,1,1},
                            {1,1,2},
                            {1,0,4},
                            {4,0,6},//cmptrue
                            {3,0,0},//mov
                            {2,0,0},//jmp
                            {4,1,5},//cmp
                            {5,1,1},//cmpfalse
                            {5,0,0},//cmpfalse eigentlich fehler
                            {6,1,1},//cmptrue
                            {6,0,0},//cmptrue
                            {3,1,1},//mov
                            {2,1,1}};//jmp


struct
{
 int pos;
 int state;
 int token;
}instr_head;


struct
{
 int pos;
 int signal;
 int membit;
}datadr_head;


unsigned char mem[256];
unsigned char instr_mem[256];


int statechange_lookup (int head, int token)
{
 int n=0;
 while ( statechange[n][0]!=head&&statechange[n][1]!=token)n++;
 if ( n==14) return -1;
 return statechange[n][2]; 
}

void process_datahead()
{
 if ( datadr_head.signal==1)
 {
  datadr_head.membit=mem[datadr_head.pos];
  datadr_head.signal=2;
 }
 if ( datadr_head.signal==2)
 {
  if ( mem[datadr_head.pos]==datadr_head.membit) instr_head.token=1;
  else instr_head.token=0;
 }
 if ( datadr_head.signal==3)
 {
  datadr_head.membit=mem[datadr_head.pos];
 }
 
 if ( datadr_head.signal==4)
 {
  mem[datadr_head.pos]=datadr_head.membit;
 }
 
 

}

void state_tok00(void)
{
 instr_head.state=statechange_lookup(instr_head.state, instr_mem[instr_head.pos]);
 instr_head.pos++;
}


void state_tok01(void)
{
 instr_head.state=statechange_lookup(instr_head.state, instr_mem[instr_head.pos]);
 instr_head.pos++;
}

void state_tok10(void)
{
 instr_head.state=statechange_lookup(instr_head.state, instr_mem[instr_head.pos]);
 
 datadr_head.pos=0;
 int n=0;
 while (n<8)
 {
 datadr_head.pos+=instr_mem[instr_head.pos];
 datadr_head.pos*=2;
  instr_head.pos++;
  n++;
 }
 datadr_head.signal=1;
 process_datahead();

 datadr_head.pos=0;
 n=0;
 while (n<8)
 {
 datadr_head.pos+=instr_mem[instr_head.pos];
 datadr_head.pos*=2;
  instr_head.pos++;
  n++;
 }
 process_datahead();
 
 
}


void state_tok11(void)
{
 instr_head.state=statechange_lookup(instr_head.state,instr_head.token=instr_mem[instr_head.pos);
 instr_head.pos++; 
  instr_head.pos=instr_mem[instr_head.pos]*2*2*2*2*2*2*2+
                 instr_mem[instr_head.pos+1]*2*2*2*2*2*2+
                 instr_mem[instr_head.pos+2]*2*2*2*2*2+
                 instr_mem[instr_head.pos+3]*2*2*2*2+
                 instr_mem[instr_head.pos+4]*2*2*2+
                 instr_mem[instr_head.pos+5]*2*2+
                 instr_mem[instr_head.pos+6]*2+
                 instr_mem[instr_head.pos];

}

void state_tok20(void)
{
 instr_head.state=statechange_lookup(instr_head.state, instr_head.token=instr_mem[instr_head.pos]);
 instr_head.pos++;

}

void state_tok21(void)
{
 instr_head.state=statechange_lookup(instr_head.state, instr_head.token=instr_mem[instr_head.pos]);
 instr_head.pos++;
}//mitm Bleistift Werkzeug auf Seite koennte man das und das machen, wie Zwischenablage?!?

void state_tok30(void)
{
 instr_head.token=1;
  instr_head.state=statechange_lookup(instr_head.state,instr_head.token);

}

void state_tok31(void)
{
 datadr_head.pos=0;
 int n=0;
 while (n<8)
 {
 datadr_head.pos+=instr_mem[instr_head.pos];
 datadr_head.pos*=2;
  instr_head.pos++;
  n++;
 }
 datadr_head.signal=3;
 process_datahead();
  datadr_head.signal=4;
 
 datadr_head.pos=0;
 n=0;
 while (n<8)
 {
 datadr_head.pos+=instr_mem[instr_head.pos];
 datadr_head.pos*=2;
  instr_head.pos++;
  n++;
 }
 process_datahead();
 
 instr_head.state=statechange_lookup(instr_head.state, instr_heak.token=instr_mem[instr_head.pos]);
 instr_head.pos++;

}

void state_tok40(void)
{
 instr_head.state=statechange_lookup(instr_head.state,instr_head.token=instr_mem[instr_head.pos]);
 instr_head.pos++;
}

void state_tok41(void)
{
 instr_head.state=statechange_lookup(instr_head.state,instr_head.token=instr_mem[instr_head.pos]);
 instr_head.pos++;
}


void state_tok50(void)
{
 printf("Bit-Wert ungueltig fuer Status!\n");
}

void state_tok51(void)
{
 instr_head.pos+=3+8;
 instr_head.state=statechange_lookup(instr_head.state, instr_head.token=instr_mem[instr_head.pos]);
 instr_head.pos++;

}

void state_tok60(void)
{
  instr_head.state=statechange_lookup(instr_head.state,instr_head.token=instr_mem[instr_head.pos]);
 instr_head.pos++;
}

void state_tok61(void)
{
  instr_head.state=statechange_lookup(instr_head.state,instr_head.token=instr_mem[instr_head.pos]);
 instr_head.pos++;
}

int main(void)
{

while(1)
{
 state_tok00(),state_tok01(),state_tok10(),state_tok11(),state_tok20(),
  state_tok21(),state_tok30(),state_tok31(),state_tok40(),state_tok41(),
  state_tok50(),state_tok51(),state_tok60(),state_tok61();
}

}

