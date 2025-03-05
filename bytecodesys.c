//#define bytecodesys
//#define byteassembler
#define precompiler

#ifdef bytecodesys

#include <stdio.h>

signed int RAM[64000];

/* no multitasking securing needed here?!??*/

typedef
struct
{
 int ax, cx, dx,
  cs,ip;
  int if_flag;int greater_flag; int lesser_flag;
  int di_off;
}vmregs_;

vmregs_ vmregs;

struct
{
 int address; int len;int varlen;
 vmregs_ regsreset;
}tasktable[10];

int active_task;

void schedule(void)
{
 static int tasknum=0;
 int count=0;
 
 if ( active_task!=-1)
 {
 tasktable[active_task].regsreset.ax=vmregs.ax,
 tasktable[active_task].regsreset.cx=vmregs.cx,
 tasktable[active_task].regsreset.dx=vmregs.dx,
 tasktable[active_task].regsreset.cs=vmregs.cs,
 tasktable[active_task].regsreset.ip=vmregs.ip;
 tasktable[active_task].regsreset.if_flag=vmregs.if_flag;
 tasktable[active_task].regsreset.greater_flag=vmregs.greater_flag;
 tasktable[active_task].regsreset.lesser_flag=vmregs.lesser_flag;
 tasktable[active_task].regsreset.di_off=vmregs.di_off;
 }
 
 tasknum=(active_task+1)%10;
 while ( tasktable[tasknum].len==0)
 {
  tasknum++;
  tasknum%=10;
   if ( ++count==10)break;
 }
 active_task=tasknum;
  //printf("bin in Task %d gewechselt",tasknum);
 
 vmregs.ax=tasktable[tasknum].regsreset.ax,
 vmregs.cx=tasktable[tasknum].regsreset.cx,
 vmregs.dx=tasktable[tasknum].regsreset.dx,
 vmregs.cs=tasktable[tasknum].regsreset.cs,
 vmregs.ip=tasktable[tasknum].regsreset.ip;
 vmregs.if_flag=tasktable[tasknum].regsreset.if_flag;
 vmregs.greater_flag=tasktable[tasknum].regsreset.greater_flag;
 vmregs.lesser_flag=tasktable[tasknum].regsreset.lesser_flag;
 vmregs.di_off=tasktable[tasknum].address+tasktable[tasknum].len+255+2;
}

struct
{
 signed int task;
 int msg;
  int os_reply;
 unsigned char param[255];
  int valparam;
}queued_message[100],syscall_answer;

int msgcnt=0;
int os_ready=0;
 

void delete_message(int n)
{
 while (n<msgcnt)
 {
   queued_message[msgcnt].msg=queued_message[msgcnt+1].msg;
   queued_message[msgcnt].task=queued_message[msgcnt+1].task;
   queued_message[msgcnt].os_reply=queued_message[msgcnt+1].os_reply;
   strncpy(queued_message[msgcnt].param,queued_message[msgcnt+1].param,255);
    queued_message[msgcnt].valparam=queued_message[msgcnt+1].valparam;
  n++;
 }
 msgcnt--;

}

void message_queue(void)
{
 int n=0;
     int n3=0;

 while ( n < 10 )
 {
  if (tasktable[n].len!=0)
  {
   if ( RAM[tasktable[n].address]==1 )
   {
    RAM[tasktable[n].address]=0;
    queued_message[msgcnt].msg=RAM[tasktable[n].address+1];
    queued_message[msgcnt].task=n;
    queued_message[msgcnt].os_reply=0;
     n3=0;
     while ( n3<50)queued_message[msgcnt].param[n3]=RAM[(tasktable[n].address+2+n3)],n3++;
   	  queued_message[msgcnt].valparam=RAM[(tasktable[n].address+160)];
	   msgcnt++;
   }
  }
   n++;
 }
 if ( syscall_answer.task>=0 )
 {
    //printf("os hat zurückgeschickt %d",syscall_answer.msg);
   queued_message[msgcnt].msg=syscall_answer.msg;
   queued_message[msgcnt].task=syscall_answer.task;
   queued_message[msgcnt].os_reply=1;
    syscall_answer.task=-1;
   strncpy(queued_message[msgcnt].param,syscall_answer.param,49);
    queued_message[msgcnt].valparam=syscall_answer.valparam;
	queued_message[msgcnt].param[49]='\0';
   msgcnt++;
 }

 n=msgcnt-1;
 while ( n >=0 )
 {
  if ( queued_message[n].os_reply==1)
  {
  	//printf("Message %d geht an Task",queued_message[n].msg);
   if ( RAM[tasktable[queued_message[n].task].address+102]==0 )
   {
    RAM[tasktable[queued_message[n].task].address+102]=1;
    RAM[tasktable[queued_message[n].task].address+102+1]=queued_message[n].msg;
   	 // erste Indizierung noch kein R Wert?!??
      n3=0;
     while ( n3<50)RAM[(tasktable[queued_message[n].task].address+2)+n3]=queued_message[n].param[n3]=RAM,n3++;
      RAM[tasktable[queued_message[n].task].address+160]=queued_message[n].valparam;
	 delete_message(n);
   }
  }
  else
  {
   if ( os_ready==0)
   {
   	//printf("kopiere ans system %d %c%c",tasktable[n].address,RAM[(tasktable[n].address+2)],RAM[(tasktable[n].address+3)]);
   syscall_answer.msg=queued_message[n].msg;
   syscall_answer.task=queued_message[n].task;
   strncpy(syscall_answer.param,queued_message[n].param,49);
   syscall_answer.valparam=queued_message[n].valparam;
   syscall_answer.param[49]='\0';
    //printf("%s",syscall_answer.param);
    delete_message(n);
    os_ready=1;
   }

  }
  n--;
 }


}

void syscall(void)
{
 int n=0; 
 if ( syscall_answer.msg==0&&os_ready==1)
 {
 //load
  int loadpos=0;
  while ( n<10)
  {
   if(tasktable[n].len!=0)loadpos+=tasktable[n].len+tasktable[n].varlen+255+2;
    n++;
   }
   n=0;
   while (tasktable[n].len!=0&&n<10)n++;
   	 if ( n==10){printf("Zuviele Programme geladen");return;}
    int program_len=0,var_len;
    FILE *progimage;
    progimage=fopen(syscall_answer.param,"rb");
     fscanf(progimage,"%d\n",&var_len);
	while(!feof(progimage))RAM[loadpos+program_len+255+2]=fgetc(progimage)&255|(fgetc(progimage)<<8),RAM[loadpos+program_len+255+2]>32767? (RAM[loadpos+program_len+255+2]=(65535-RAM[loadpos+program_len+255+2])*-1) : 0 ,program_len++;
  	 RAM[loadpos+program_len+255+2-1]=-1;
	   fclose(progimage);
     tasktable[n].len=program_len;
     tasktable[n].varlen=var_len;
     tasktable[n].address=loadpos;
   tasktable[n].regsreset.ip=loadpos+255+2;
   tasktable[n].regsreset.di_off=program_len+loadpos+255+2;
  os_ready=0;
  syscall_answer.task=-1;
  //printf("Habe geladen Task %d Adresse %d Programmlaenge %d Variablenlaenge %d",n,tasktable[n].address,tasktable[n].len,tasktable[n].varlen);
 }
  else
 if ( syscall_answer.msg==1&&os_ready==1)
 { 
 printf("Beende Task %d",syscall_answer.task);
 //memcompact
  n=0;
  while (n<10)
  {
   if ( tasktable[n].address>tasktable[syscall_answer.task].address)
   {
   	memmove(RAM+tasktable[n].address-tasktable[syscall_answer.task].len-tasktable[syscall_answer.task].varlen-255-2,RAM+tasktable[n].address,
	   tasktable[syscall_answer.task].len+tasktable[syscall_answer.task].varlen+255+2);
	   tasktable[n].address-=tasktable[syscall_answer.task].len+tasktable[syscall_answer.task].varlen+255+2;
      tasktable[n].regsreset.ip-=tasktable[syscall_answer.task].len+tasktable[syscall_answer.task].varlen+255+2;	
      tasktable[n].regsreset.di_off-=tasktable[syscall_answer.task].len+tasktable[syscall_answer.task].varlen+255+2;	
    tasktable[n].len=0;
   }
   n++;
  }
 //remove
  tasktable[syscall_answer.task].len=0;
  syscall_answer.task=-1;
   os_ready=0;
 }
  else
 if ( syscall_answer.msg==2&&os_ready==1)
 { 
 //print
  printf("%s",syscall_answer.param);
   syscall_answer.task=-1;
  os_ready=0;
 }
  else
 if ( syscall_answer.msg==3&&os_ready==1)
 { 
   printf("Eingabe:");
  scanf("%d",&syscall_answer.valparam);
  syscall_answer.param[49]='\0';
  syscall_answer.msg=4;
 //input
  os_ready=0;
 }
}

void vm_execute(void)
{
	
	if ( tasktable[active_task].len==0){printf("inaktiver Task");getchar();return;}
    int vmsteps=10;
     static signed int readvaladr=-1;
    while (vmsteps>0)
    {
       //printf("Bin an Adresse %d di ist %d offset Befehl %d %d %d\n",vmregs.ip,vmregs.di_off,RAM[vmregs.ip],tasktable[active_task].address,vmregs.if_flag);
	   //int n4;n4=0;
	    //while (n4<100)printf("%d ",RAM[vmregs.ip+n4]),n4++;
	if ( RAM[tasktable[active_task].address+102]==1)
	{
	 if ( RAM[tasktable[active_task].address+102+1]==4)
	 {
		printf("binauchda");
	 	if ( readvaladr!=-1)
	 	RAM[readvaladr]=RAM[tasktable[active_task].address+160];
	 	RAM[tasktable[active_task].address+102]=0;
	 	readvaladr=-1;
	 }
		
	}
	if ( readvaladr!=-1) return;
	if ( RAM[tasktable[active_task].address]==1)return;
	getchar();
 if ( RAM[vmregs.ip]==0)
 {
  vmregs.ax=RAM[vmregs.di_off+RAM[vmregs.ip+1]];
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip+tasktable[active_task].address]==1)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]] =vmregs.ax;
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip+tasktable[active_task].address]==29)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]] =RAM[vmregs.di_off+RAM[vmregs.ip+2]];
  vmregs.ip+=3;
 }
 else
 if ( RAM[vmregs.ip]==2)
 {
  vmregs.ax=RAM[vmregs.ip+1];
  vmregs.ip+=2;
 }
  else
 if ( RAM[vmregs.ip]==26)
 {
   if ( vmregs.if_flag==1 )
   {
    vmregs.ax=1;
   }
   vmregs.ip++;
  }
 else
 if ( RAM[vmregs.ip]==3)
 {
  vmregs.ax+=RAM[vmregs.ip+1];
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==4)
 {
  vmregs.ax+=RAM[vmregs.di_off+RAM[vmregs.ip+1]];
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==5)
 {
  vmregs.ax-=RAM[vmregs.di_off+RAM[vmregs.ip+1]];
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==6)
 {
  vmregs.ax-=RAM[vmregs.di_off+RAM[vmregs.ip+1]];
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==7)
 {
  vmregs.ax*=RAM[vmregs.di_off+RAM[vmregs.ip+1]];
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==8)
 {
  vmregs.ax*=RAM[vmregs.di_off+RAM[vmregs.ip+1]];
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==9)
 {
  if( RAM[vmregs.di_off+RAM[vmregs.ip+1]]==0){printf("Division durch 0");return;}
  vmregs.ax/=RAM[vmregs.di_off+RAM[vmregs.ip+1]];
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==10)
 {
  if(RAM[tasktable[active_task].address+1]==0){printf("Division durch Wert 0");return;}
   vmregs.ax/=RAM[vmregs.di_off+RAM[vmregs.ip+1]];
  vmregs.ip+=2;
 } 
 
 else
 if ( RAM[vmregs.ip]==14)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]]+=vmregs.ax;
   vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==15)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]]+=vmregs.ax;
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==16)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]]-=vmregs.ax;
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==17)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]]-=vmregs.ax;
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==18)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]]*=vmregs.ax;
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==19)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]]*=vmregs.ax;
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==20)
 {
  if( vmregs.ax==0){printf("Division durch 0");return;}
  RAM[vmregs.di_off+RAM[vmregs.ip+1]]/=vmregs.ax;
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==21)
 {
  if(vmregs.ax==0){printf("Division durch Wert 0");return;}
   RAM[vmregs.di_off+RAM[vmregs.ip+1]]/=vmregs.ax;
  vmregs.ip+=2;
 }
 
 else
 if ( RAM[vmregs.ip]==11)
 {
  if(RAM[vmregs.ip+1]==
     RAM[vmregs.ip+2])
   vmregs.if_flag=1; else vmregs.if_flag=0;
  
  if(RAM[vmregs.ip+1]>
     RAM[vmregs.ip+2])
   vmregs.greater_flag=1; else vmregs.greater_flag=0;
  
  if(RAM[vmregs.ip+1]<
     RAM[vmregs.ip+2])
   vmregs.lesser_flag=1; else vmregs.lesser_flag=0;
  
  
   vmregs.ip+=3;
 }
 else
 if ( RAM[vmregs.ip]==12)
 {
 /*	printf(">>%d %d<<",RAM[vmregs.di_off+RAM[vmregs.ip+1]],
     RAM[vmregs.ip+2]);*/
  if(RAM[vmregs.di_off+RAM[vmregs.ip+1]]==
     RAM[vmregs.ip+2])
   vmregs.if_flag=1; else vmregs.if_flag=0;
 
  if(RAM[vmregs.di_off+RAM[vmregs.ip+1]]>
     RAM[vmregs.ip+2])
   vmregs.greater_flag=1; else vmregs.greater_flag=0;
 
 if(RAM[vmregs.di_off+RAM[vmregs.ip+1]]<
     RAM[vmregs.ip+2])
   vmregs.lesser_flag=1; else vmregs.lesser_flag=0;
 
   vmregs.ip+=3;
 }
 else
 if ( RAM[vmregs.ip]==27)
 {
  if(vmregs.ax==RAM[vmregs.ip+1])
   vmregs.if_flag=1; else vmregs.if_flag=0;
 
  if(vmregs.ax>RAM[vmregs.ip+1])
   vmregs.greater_flag=1; else vmregs.greater_flag=0;
 
 if(vmregs.ax<RAM[vmregs.ip+1])
   vmregs.lesser_flag=1; else vmregs.lesser_flag=0;
 
   vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==28)
 {
 //printf(">> %d %d<<",vmregs.ax,RAM[vmregs.di_off+RAM[vmregs.ip+1]]);
  if(vmregs.ax==RAM[vmregs.di_off+RAM[vmregs.ip+1]])
   vmregs.if_flag=1; else vmregs.if_flag=0;
   
  if(vmregs.ax>RAM[vmregs.di_off+RAM[vmregs.ip+1]])
   vmregs.greater_flag=1; else vmregs.greater_flag=0;
  if(vmregs.ax<RAM[vmregs.di_off+RAM[vmregs.ip+1]])
   vmregs.lesser_flag=1; else vmregs.lesser_flag=0;
 
   vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==13)
 {
  if ( vmregs.if_flag==0)vmregs.ip+=RAM[vmregs.ip+1];
  else vmregs.ip+=2;
 } 
 else
 if ( RAM[vmregs.ip]==30)
 {
  vmregs.ip+=RAM[vmregs.ip+1];
 } 
 else
 if ( RAM[vmregs.ip]==22)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]]=vmregs.ax;
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==25)
 {
  RAM[tasktable[active_task].address]=1;
  RAM[tasktable[active_task].address+1]=2;
  vmregs.ip++;
   int n3=0;
  n3=0;
  while (RAM[vmregs.ip]!=0)
   RAM[tasktable[active_task].address+2+n3]=RAM[vmregs.ip],vmregs.ip++,n3++;
   //vmregs.ip++;printf("habe ip inkrementiert");
   RAM[tasktable[active_task].address+2+n3]='\0';
   vmregs.ip++;
   //printf("%c>>%d<< %d",RAM[vmregs.ip-3],RAM[tasktable[active_task].address+3],tasktable[active_task].address);
 }
 else
 if ( RAM[vmregs.ip]==24)
 {
  RAM[tasktable[active_task].address]=1;
  RAM[tasktable[active_task].address+1]=2;
  vmregs.ip++;
   int n3=0;
  n3=0;
  if (RAM[vmregs.di_off+RAM[vmregs.ip]]<0)
  RAM[tasktable[active_task].address+2+0]='-'; else RAM[tasktable[active_task].address+2+0]=' '; 
   int n4=0,increase_position=0;n4=0;increase_position=0;
   RAM[tasktable[active_task].address+2+1+n4]=((abs(RAM[vmregs.di_off+RAM[vmregs.ip]]))/10000)%10+0x30;
   if(((abs(RAM[vmregs.di_off+RAM[vmregs.ip]]))/10000)%10!=0||increase_position==1)n4++,increase_position=1;
   RAM[tasktable[active_task].address+2+1+n4]=((abs(RAM[vmregs.di_off+RAM[vmregs.ip]]))/1000)%10+0x30;
   if(((abs(RAM[vmregs.di_off+RAM[vmregs.ip]]))/1000)%10!=0||increase_position==1)n4++,increase_position=1;
   RAM[tasktable[active_task].address+2+1+n4]=((abs(RAM[vmregs.di_off+RAM[vmregs.ip]]))/100)%10+0x30;
   if(((abs(RAM[vmregs.di_off+RAM[vmregs.ip]]))/100)%10!=0||increase_position==1)n4++,increase_position=1;
   RAM[tasktable[active_task].address+2+1+n4]=((abs(RAM[vmregs.di_off+RAM[vmregs.ip]]))/10)%10+0x30;
  if(((abs(RAM[vmregs.di_off+RAM[vmregs.ip]]))/10)%10!=0||increase_position==1)n4++,increase_position=1;
   RAM[tasktable[active_task].address+2+1+n4]=((abs(RAM[vmregs.di_off+RAM[vmregs.ip]]))%10)+0x30;
    n4++;
   RAM[tasktable[active_task].address+2+1+n4]='\0';
   vmregs.ip+=1;
 }
 else
 if ( RAM[vmregs.ip]==31)
 {
  RAM[tasktable[active_task].address]=1;
  RAM[tasktable[active_task].address+1]=3;
  readvaladr=vmregs.di_off+RAM[vmregs.ip+1];
  vmregs.ip+=2;
  //printf ("habe input gelesen");
 }
  else
 if ( RAM[vmregs.ip]==-1)
 {
  RAM[tasktable[active_task].address]=1;
  RAM[tasktable[active_task].address+1]=1;
  vmregs.ip++;
 }

 vmsteps--;
}
 //bei Adressoperator weist man ohne eckige Klammern zu R-Wert, bei Inhaltsoperator kopiert man Speicherzelle in Register und indiziert es anschließend?!?
 
/* 
 if ( strcmp(input,"movaxvar" 0)==0)
  if ( strcmp(input,"movarax" 1)==0)
  if ( strcmp(input,"movvaxval" 2)==0)
  if ( strcmp(input,"addaxval" 3)==0)
  if ( strcmp(input,"addaxvar" 4)==0)
  if ( strcmp(input,"subaxvar") 5)
 if ( strcmp(input,"subaxval") 6)
 if ( strcmp(input,"mulaxval") 7)
if ( strcmp(input,"mulaxvar" 8)==0)
 if ( strcmp(input,"divaxvar" 9)==0)
 if ( strcmp(input,"divaxval" 10)==0)
 if ( strcmp(input,"cmpvalval" 11)==0)
 if ( strcmp(input,"cmpvarval" 12)==0)
 if ( strcmp(input,"jz" 13)==0)
  if ( strcmp(input,"addaxval" 14)==0)
  if ( strcmp(input,"addvarax" 15)==0)
  if ( strcmp(input,"subvarax" 16)==0)
 if ( strcmp(input,"subaxval" 17)==0)
 if ( strcmp(input,"mulaxval" 18)==0)
 if ( strcmp(input,"mulvarax" 19)==0)
 if ( strcmp(input,"divvarax" 20)==0)
 if ( strcmp(input,"divaxval" 21)==0)
  if ( strcmp(input,"movvarax" 22)==0)
  if ( strcmp(input,"PRINTvar" 24)==0)
  if ( strcmp(input,"PRINTstr" 25)==0)
    repzmovaxval 26
    cmpaxval 27
    cmpaxvar 28
    movvarvar 29
    jmp 30
    INPUTvar 31
*/

}

int main(void)
{
 syscall_answer.task=-1;
  active_task=-1;
 int n=0;
 while ( n < 10 )tasktable[n].len=0,n++;
 
 syscall_answer.msg=0;
 os_ready=1;
 strcpy(syscall_answer.param,"firstprog.btc");
 syscall();
   getchar(); 
 schedule();
  os_ready=0;
 syscall();
   getchar(); 
  getchar();
  schedule();
   getchar();
  vm_execute();
  getchar();
  message_queue();
  getchar();
  syscall();
  getchar();
 while(1)
 { 
  schedule();
  vm_execute();
  message_queue();
   syscall();
 } 

}
#elif defined byteassembler

   unsigned char varreplace[128][30];
    int varcount=0;

   unsigned char labelreplace[128][30];
   int labelcount=0;


unsigned int varfornum(unsigned char *string)
{
 int n=0;
 while (  n < varcount)
 {
  if (strcmp(&(varreplace[n][0]),string)==0) break;
  n++;
 }
  if ( n==varcount) strcpy(&(varreplace[n][0]),string), varcount++;
  return n;
}

unsigned int labelfornum(unsigned char *string)
{
 int n=0;
 while ( n < labelcount)
 {
   if (strcmp(&(labelreplace[n][0]),string)==0)break;
  n++;
 }
  if ( n==labelcount) strcpy(&(labelreplace[n][0]),string),labelcount++;

 return n;
}


int main(void)
{
 unsigned char input[255];
   int input_val;
   signed int machinecode[4096];
  int labeloccurence[128][2];
 int labeladress[128];
    int labeloccur=0;
    int adr=0;

 while (1)
 {
  scanf("%s",input);

  if ( strcmp(input,"movaxval")==0)
  {
   machinecode[adr]=2;adr++;
    scanf("%d",&input_val);
   machinecode[adr]=input_val; adr++;
 
  }
  else
  if ( strcmp(input,"movaxvar")==0)
  {
   machinecode[adr]=0;adr++;
    scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;
  }
  else
  if ( strcmp(input,"addaxval")==0)
  {
   machinecode[adr]=3;adr++;
    scanf("%d",&input_val);
   machinecode[adr]=input_val; adr++;
  
  }
   else
  if ( strcmp(input,"movvarvar")==0)
  {
   machinecode[adr]=29;adr++;
    scanf("%s",input);
   machinecode[adr]=varfornum(input); adr++;
    scanf("%s",input);
   machinecode[adr]=varfornum(input); adr++;
  
  }
  else
  if ( strcmp(input,"addaxvar")==0)
  {
   machinecode[adr]=4;adr++;
    scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;

  }
  else
  if ( strcmp(input,"subaxvar")==0)
  {
   machinecode[adr]=5;adr++;
     scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;

  }
 else
 if ( strcmp(input,"subaxval")==0)
  {
   machinecode[adr]=6;adr++;
    scanf("%d",&input_val);
   machinecode[adr]=input_val; adr++;
  
  }
 else
 if ( strcmp(input,"mulaxval")==0)
  {
   machinecode[adr]=7;adr++;
    scanf("%d",&input_val);
   machinecode[adr]=input_val; adr++;
 
  }
  else
 if ( strcmp(input,"mulaxvar")==0)
  {
   machinecode[adr]=8;adr++;
    scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;

  }
  else
 if ( strcmp(input,"divaxvar")==0)
  {
   machinecode[adr]=9;adr++;
     scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;

  }
  else
 if ( strcmp(input,"divaxval")==0)
  {
   machinecode[adr]=10;adr++;
    scanf("%d",&input_val);
   machinecode[adr]=input_val; adr++;
 
  }
  else
 if ( strcmp(input,"cmpvalval")==0)
  {
   machinecode[adr]=11;adr++;
    scanf("%d",&input_val);
   machinecode[adr]=input_val; adr++;
 
  scanf("%d",&input_val);
   machinecode[adr]=input_val; adr++;
 
  }
  else
 if ( strcmp(input,"cmpvarval")==0)
  {
   machinecode[adr]=12;adr++;
   scanf("%s",input);
    machinecode[adr]=varfornum(input);adr++;
   scanf("%d",&input_val);
    machinecode[adr]=input_val;adr++;
     
}
  else
 if ( strcmp(input,"jz")==0)
  {
   machinecode[adr]=13;adr++;
   scanf("%s",input);
    machinecode[adr]='@';labeloccurence[labeloccur][0]=adr,labeloccurence[labeloccur][1]=labelfornum(input);
    labeloccur++;
   adr++;
  }
  else
 if ( strcmp(input,"jmp")==0)
  {
   machinecode[adr]=30;adr++;
   scanf("%s",input);
    machinecode[adr]='@';labeloccurence[labeloccur][0]=adr,labeloccurence[labeloccur][1]=labelfornum(input);
    labeloccur++;
   adr++;
  }
else
  if ( input[0]==':')
  {
   labeladress[labelfornum(input+1)]=adr;  
  }
  else
  if ( strcmp(input,"addvarax")==0)
  {
   machinecode[adr]=15;adr++;
    scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;

  }
  else
  if ( strcmp(input,"subvarax")==0)
  {
   machinecode[adr]=16;adr++;
     scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;

  }
  else
 if ( strcmp(input,"mulvarax")==0)
  {
   machinecode[adr]=19;adr++;
    scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;

  }
  else
 if ( strcmp(input,"divvarax")==0)
  {
   machinecode[adr]=20;adr++;
     scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;

  }
   else
  if ( strcmp(input,"movvarax")==0)
  {
   machinecode[adr]=22;adr++;
    scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;
  }
   else
  if ( strcmp(input,"PRINTvar")==0)
  {
   machinecode[adr]=24;adr++;
    scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;
  }
   else
  if ( strcmp(input,"INPUTvar")==0)
  {
   machinecode[adr]=31;adr++;
    scanf("%s",input);
   machinecode[adr]=varfornum(input);adr++;
  }
   else
  if ( strcmp(input,"PRINTstr")==0)
  {
   machinecode[adr]=25;adr++;
    scanf("%s",input);
    int n2=1;
    n2=1;
    while (input[n2]!='\0')machinecode[adr]=input[n2],adr++,n2++;
    machinecode[adr]=0,adr++;
  }
   else
  if ( strcmp(input,"repzmovaxval")==0)
  {
   machinecode[adr]=26;adr++;
 }
   else
  if ( strcmp(input,"cmpaxval")==0)
  {
   machinecode[adr]=27;adr++;
    scanf("%d",&input_val);
   machinecode[adr]=input_val; adr++;
   }
   else
  if ( strcmp(input,"cmpaxvar")==0)
  {
   machinecode[adr]=28;adr++;
   scanf("%s",input);
    machinecode[adr]=varfornum(input);adr++;
   }
 
  else
  if ( strcmp(input,"end")==0) break;  
 }
 int n=0;
 while ( n < labeloccur )
 {
  machinecode[labeloccurence[n][0]]=labeladress[labeloccurence[n][1]]-labeloccurence[n][0];
   n++;
 }
 n=0;
 printf("%d\n",varcount);
 while ( n < adr) printf("%c%c",machinecode[n]&255,machinecode[n]>>8),n++;
//while ( n < adr) printf("%d ",machinecode[n]),n++;



}

#elif defined precompiler

/*
  trying to add ann algebra solve operator into the syntax of the programming language and
    making algebra obsolete
    
    additional info is carried through r values
    put the sums on the left and the factors on the right and then divide
    
    
    type in
    ( ( 2 * x ) - 10 ) S ( 3 * ( x - 10 ) ) ;
      
      see what it does. I'm not certain whether it really works full
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int var_name_count=0;

void variable_countup(char *name, int do_countup)
{

 char appendix[255];
 char *base="rval";
 if ( do_countup == 1 ) ++var_name_count;
   itoa(var_name_count, appendix, 10);
   
 strcpy(name,base);
 strcat( name, appendix );
 return ;

}

int isnum(char c)
{
 if ( c>=0x30 && c <=0x39 ) return 1;
 return 0;

}

unsigned char flushstring[1000],flushprefix[100],flushout[1000];

void sequencepoint()
{
 fprintf(stderr,"%s%s",flushprefix,flushstring);	
  flushstring[0]='\0', flushprefix[0]='\0',flushout[0]='\0';
 	
}


struct
{
 int type;
 char text[255];
 signed int mulvar;
 signed int sumvar;
 signed int onx;

}token[255];

static int num_tokens;

struct
{
 unsigned char labelnum[20];	
 unsigned int type;
}labelstack[100];

unsigned int label_name_count=0;
unsigned int label_stack_count=0;

unsigned char labelname[255];

void label_countup(char *name, int do_countup)
{

 char appendix[255];
 char *base="label";
 if ( do_countup == 1 ) ++label_name_count;
   itoa(label_name_count, appendix, 10);
   
 strcpy(name,base);
 strcat( name, appendix );
 return ;

}


void remove_tokens( int position, int amount_removed )
{

 while( position+ amount_removed <= num_tokens )
 {
  token[position].type=token[position+amount_removed].type;
  strcpy( token[position].text, token[position+amount_removed].text );
   token[position].sumvar=token[position+amount_removed].sumvar;
   token[position].mulvar=token[position+amount_removed].mulvar;
    token[position].onx=token[position+amount_removed].onx;

  position++;
 }

 num_tokens-=amount_removed;

}



int main(void)
{
 int position;
 int txtcount;
 int left_limit, right_limit;
 int reduction_type=0;
 char rvalnum[20];
   signed int sumbuf,mulbuf;
   signed int mulbufr, sumbufr;
   signed int onxbuf;
          

   flushstring[0]='\0', flushprefix[0]='\0',flushout[0]='\0';

  newline:

 	sequencepoint();

  position=0;
  reduction_type=0;
 do
 {
 	sequencepoint();
  txtcount=0;
  scanf("%s", token[position].text );

  if(token[position].text[0]=='+' 
     && strlen(token[position].text )== 1 ) token[position].type='+',
     txtcount++;
  else
   if(token[position].text[0]=='-'
      && strlen(token[position].text )== 1 ) token[position].type='-',
      txtcount++;
  else
   if(token[position].text[0]=='*'
      && strlen(token[position].text )== 1 ) token[position].type='*',
      txtcount++;
  else
   if(token[position].text[0]=='/'
       && strlen(token[position].text )== 1 ) token[position].type='/',
       txtcount++;
    else
   if(token[position].text[0]=='S'
       && strlen(token[position].text )== 1 ) token[position].type='S',
       txtcount++;
  else
   if(token[position].text[0]=='('
       && strlen(token[position].text )== 1 ) token[position].type='(',
       txtcount++;
  else
   if(token[position].text[0]==')'
      && strlen(token[position].text )== 1 ) token[position].type=')',
      txtcount++;       
  else
    if(token[position].text[0]=='='
        && strlen(token[position].text )== 1 ) token[position].type='=',
        txtcount++;
  else
    if(token[position].text[0]==':'
        && strlen(token[position].text )== 1 ) token[position].type=':',
        txtcount++;
  else
    if(token[position].text[0]=='@' ) token[position].type='@',
        txtcount++;
   else
    if(token[position].text[0]==';'
        && strlen(token[position].text )== 1 ) token[position].type=';',
        txtcount++;
  else
   if ( strcmp(token[position].text,"if")==0)
   {
   	token[position].type='i';
   	
   }
     else
   if ( strcmp(token[position].text,"for")==0)
   {
   	token[position].type='f';
   	
   }
     else
   if ( strcmp(token[position].text,"next")==0)
   {
   	token[position].type='x';
   	
   }
       else
   if ( strcmp(token[position].text,"break")==0)
   {
   	token[position].type='b';
   	
   }
 
      else
   if ( strcmp(token[position].text,"print")==0)
   {
   	token[position].type='p';
   	
   }
     else
   if ( strcmp(token[position].text,"input")==0)
   {
   	token[position].type='\?';
   	
   }
  else
    if ( isalpha( token[position].text[0] ) )
    {
     txtcount=0;
     
     while( isalpha(token[position].text[txtcount] )||isnum(token[position].text[txtcount] ) )txtcount++;
     token[position].type='v'; /* Variable */
     token[position].onx=0;
     token[position].sumvar=0,token[position].mulvar=1;
     if ( token[position].text[0]=='x') token[position].onx=1;
	 if ( token[position].text[txtcount] != '\0' ) {printf("ungueltiger Variablenname, kein Schluesselwort");return 1;}
    }
  else
    if ( isnum( token[position].text[0] )||token[position].text[0]=='-' )
  {
    txtcount=0;
    while( isnum( token[position].text[txtcount] )||(token[position].text[txtcount]=='-'&&txtcount==0) ) txtcount++;
    token[position].type='n'; /* numerische Konstante */
     token[position].sumvar=atoi(token[position].text),token[position].mulvar=0;
      token[position].onx=0;
	if ( token[position].text[txtcount] != '\0' ){printf("ungueltige numerische Konstante");return 1;}
  
  }
   else { printf("Lexikalischer Fehler.\n"); getch(); return 1;}

  position++;
 }while(token[position-1].type!= ';' );
  num_tokens=position-1;


 while(reduction_type!=';' )
 {
   reduction_type=';';
  /* debug

    position=0;

    while(position < num_tokens+1 )
    {
     printf("Position: %d String: %s Typ: %c\n", position, token[position].text,
      token[position].type );
     position++;
    }
    getch();

    debug Ende  */


  left_limit=0, right_limit=num_tokens;
 
 
   position=right_limit-1;
   while( position >=left_limit )
   {
    if ( token[position].type=='i' )
      if ( token[position+1].type== 'v' || token[position+1].type=='n' || token[position+1].type=='r')
      {
       reduction_type='i', right_limit=position+1, left_limit=position;
       break;
      }
     position--;
     
   }
   
   
   
   position=right_limit-1;
   while( position >=left_limit )
   {
    if ( token[position].type=='f' )
      if ( token[position+1].type== 'v' || token[position+1].type=='n' || token[position+1].type=='r')
      {
       reduction_type='f', right_limit=position+1, left_limit=position;
       break;
      }
     position--;
     
   }
   
   
  
     
   position=right_limit;
   while( position >=left_limit )
   {
    if ( token[position].type=='x' )
      {
	   reduction_type='n', right_limit=position+1, left_limit=position;
	   break;
      }
     position--; 
 
   }
   
   
   position=right_limit;
   while( position >=left_limit )
   {
    if ( token[position].type=='b' )
      {
	   reduction_type='b', right_limit=position+1, left_limit=position;
	   break;
      }
     position--; 
 
   }
 
 
  position=left_limit;
  while( token[position].type!= ')'&& position < num_tokens )
  {
   position++;
  }
   if ( token[position].type== ')' )
   {
    right_limit=position;
    left_limit=position;

    while(token[left_limit].type != '(' && left_limit > 0 ) left_limit--;

    if(token[left_limit].type=='(' )
    {
     reduction_type='(';
    }
    else return 1;

   }

   position=left_limit;
   while( position+2 <= right_limit )
   {
    if ( token[position].type=='v' || token[position].type=='n'  || token[position].type=='r' )
      if ( token[position+1].type=='*' || token[position+1].type=='/' )
        if ( token[position+2].type=='v' || token[position+2].type=='n'|| token[position+2].type=='r' )
         { reduction_type=token[position+1].type;
           right_limit=position+2, left_limit=position;
           break; }
     position++;
   }
   position=left_limit;
   while( position+2 <= right_limit )
   {
    if ( token[position].type=='v' || token[position].type=='n' || token[position].type=='r' )
      if ( token[position+1].type=='+' || token[position+1].type=='-' )
        if ( token[position+2].type=='v' || token[position+2].type=='n' || token[position+2].type=='r')
         { reduction_type=token[position+1].type;
           right_limit=position+2, left_limit=position;
           break; }
     position++;
   }
   
   
   position=left_limit;
   while( position+2 <= right_limit )
   {
    if ( token[position].type=='v' || token[position].type=='n' || token[position].type=='r' )
      if ( token[position+1].type=='S' )
        if ( token[position+2].type=='v' || token[position+2].type=='n' || token[position+2].type=='r')
         { reduction_type=token[position+1].type;
           right_limit=position+2, left_limit=position;
           break; }
     position++;
   }


   position=right_limit-2;
   while( position >=left_limit )
   {
    if ( token[position].type=='v' )
     if( token[position+1].type=='=' )
      if ( token[position+2].type== 'v' || token[position+2].type=='n' || token[position+2].type=='r')
      {
       reduction_type='=', right_limit=position+2, left_limit=position;
       break;
      }
     position--;

   }
   

   position=right_limit-2;
   while( position >=left_limit )
   {
    if ( token[position].type=='v'|| token[position+2].type=='n' || token[position+2].type=='r')
     if( token[position+1].type==':' )
      if ( token[position+2].type== 'v' || token[position+2].type=='n' || token[position+2].type=='r')
      {
       reduction_type=':', right_limit=position+2, left_limit=position;
       break;
      }
     position--;

   }
   
   
   
   position=right_limit-1;
   while( position >=left_limit )
   {
    if ( token[position].type=='p' )
      if ( token[position+1].type== 'v' || token[position+1].type=='@' || token[position+1].type=='r')
      {
       reduction_type='p', right_limit=position+1, left_limit=position;
       break;
      }
     position--;
    }
    
    
   position=right_limit-1;
   while( position >=left_limit )
   {
    if ( token[position].type=='\?' )
      if ( token[position+1].type== 'v' )
      {
       reduction_type='\?', right_limit=position+1, left_limit=position;
       break;
      }
     position--;
    }
   
   if ( reduction_type=='('  ) /* Klammern */
   {
     if ( right_limit-left_limit != 2 ) { printf("Fehlerhafter Klammerinhalt.\n");
           getch(); return 1; }
    if ( token[left_limit+1].type!='n' && token[left_limit+1].type!='v' &&
         token[left_limit+1].type !='r' ) { printf("Syntaxfehler bei Klammerung.\n");
          return 1; }

    variable_countup( rvalnum,1 );

    if( token[left_limit+1].type!= 'n' )
       sprintf(flushout,"movvarvar %s %s\n",rvalnum,token[left_limit+1].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"movvarval %s %s\n",rvalnum,token[left_limit+1].text ),strcat(flushstring,flushout);

      sumbuf=token[left_limit+1].sumvar,
      mulbuf=token[left_limit+1].mulvar;
      onxbuf=token[left_limit+1].onx;
    remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    token[left_limit].sumvar=sumbuf;
    token[left_limit].mulvar=mulbuf;
    token[left_limit].onx=onxbuf;
	strcpy( token[left_limit].text, rvalnum );
   }
   else
   if ( reduction_type=='+'  ) /* Addition */
   {

    variable_countup( rvalnum,1 );

    if( token[left_limit+2].type!= 'n' )
       sprintf(flushout,"movaxvar %s\n",token[left_limit+2].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"movaxval %s\n",token[left_limit+2].text ),strcat(flushstring,flushout);

    if( token[left_limit].type!= 'n' )
       sprintf(flushout,"addaxvar %s\n",token[left_limit].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"addaxval %s\n",token[left_limit].text ),strcat(flushstring,flushout);

     sprintf(flushout,"movvarax %s\n", rvalnum ),strcat(flushstring,flushout);

      sumbuf=token[left_limit].sumvar+token[left_limit+2].sumvar,
      mulbuf=token[left_limit].mulvar+token[left_limit+2].mulvar;
      onxbuf=token[left_limit].mulvar*token[left_limit].onx+token[left_limit+2].mulvar*token[left_limit+2].onx;
	     if ( onxbuf!=0)onxbuf=1; else onxbuf=0;
	     
	remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );
    
    token[left_limit].sumvar=sumbuf;
    token[left_limit].mulvar=mulbuf;
    token[left_limit].onx=onxbuf;
	token[left_limit].sumvar+=atoi(token[left_limit].text)+atoi(token[left_limit+2].text);
   

   }
   else
   if ( reduction_type=='-'  ) /* Subtraktion */
   {
     variable_countup( rvalnum,1 );

    if( token[left_limit].type!= 'n' )
       sprintf(flushout,"movaxvar %s\n",token[left_limit].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"movaxval %s\n",token[left_limit].text ),strcat(flushstring,flushout);

    if( token[left_limit+2].type!= 'n' )
       sprintf(flushout,"subaxvar %s\n",token[left_limit+2].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"subaxval %s\n",token[left_limit+2].text ),strcat(flushstring,flushout);

     sprintf(flushout,"movvarax %s\n", rvalnum ),strcat(flushstring,flushout);
     
     
	  sumbuf=token[left_limit].sumvar-token[left_limit+2].sumvar;
       mulbuf=token[left_limit].mulvar-token[left_limit+2].mulvar;
       onxbuf=token[left_limit].mulvar*token[left_limit].onx+token[left_limit+2].mulvar*token[left_limit+2].onx;
	      if ( onxbuf!=0)onxbuf=1; else onxbuf=0;
	
    remove_tokens(left_limit,2 );

   
    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );
    token[left_limit].sumvar=sumbuf;
    token[left_limit].mulvar=mulbuf;
    token[left_limit].onx=onxbuf;
     token[left_limit].sumvar-=atoi(token[left_limit].text)+atoi(token[left_limit+2].text);
   

   }
   else
   if ( reduction_type=='*'  ) /* Multiplikation */
   {
     variable_countup( rvalnum,1 );

    if( token[left_limit+2].type!= 'n' )
       sprintf(flushout,"movaxvar %s\n",token[left_limit+2].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"movaxval %s\n",token[left_limit+2].text ),strcat(flushstring,flushout);

    if( token[left_limit].type!= 'n' )
       sprintf(flushout,"mulaxvar %s\n",token[left_limit].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"mulaxval %s\n",token[left_limit].text ),strcat(flushstring,flushout);

     sprintf(flushout,"movvarax %s\n", rvalnum ),strcat(flushstring,flushout);


      sumbuf=(token[left_limit].sumvar)*(token[left_limit+2].sumvar),
      mulbuf=((token[left_limit].sumvar+token[left_limit].mulvar))*((token[left_limit+2].sumvar+token[left_limit+2].mulvar));
      
      if ( token[left_limit+2].onx==1&&token[left_limit+2].sumvar!=0)mulbuf-=(token[left_limit+2].sumvar)*
	                                                                        ((token[left_limit].sumvar+token[left_limit].mulvar));
      if ( token[left_limit].onx==1&&token[left_limit].sumvar!=0)mulbuf-=(token[left_limit].sumvar)*
	                                                                        ((token[left_limit+2].sumvar+token[left_limit+2].mulvar));
	  
	  if ( token[left_limit].onx==0&&token[right_limit].onx==0)onxbuf=0; else onxbuf=1;
   
    remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );
    
	token[left_limit].sumvar=sumbuf;
    token[left_limit].mulvar=mulbuf;
    token[left_limit].onx=onxbuf;
    token[left_limit].mulvar-=atoi(token[left_limit].text)+atoi(token[left_limit+2].text);
   
   }
   else
   if ( reduction_type=='/'  ) /* Division */
   {
     variable_countup( rvalnum,1 );

    if( token[left_limit].type!= 'n' )
       sprintf(flushout,"movaxvar %s\n",token[left_limit].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"movaxval %s\n",token[left_limit].text ),strcat(flushstring,flushout);

    if( token[left_limit+2].type!= 'n' )
       sprintf(flushout,"divaxvar %s\n",token[left_limit+2].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"divaxval %s\n",token[left_limit+2].text ),strcat(flushstring,flushout);

     sprintf(flushout,"movvarax %s\n", rvalnum ),strcat(flushstring,flushout);

    
    
        if ( token[left_limit+2].sumvar!=0&&token[left_limit+2].mulvar!=0)
        {
      sumbuf=(token[left_limit].sumvar/token[left_limit+2].sumvar)*token[left_limit].mulvar*token[left_limit].onx/token[left_limit+2].mulvar*token[left_limit+2].onx,
      mulbuf=(token[left_limit].sumvar/token[left_limit+2].sumvar)*token[left_limit].mulvar*token[left_limit].onx/token[left_limit+2].mulvar*token[left_limit+2].onx;
       } else sumbuf=-1, mulbuf=-1;
   
   
	remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );
     
	 token[left_limit].sumvar=sumbuf;
    token[left_limit].mulvar=mulbuf;
    token[left_limit].onx=(mulbuf > 0 ? 1 : 0 );
	token[left_limit].mulvar+=atoi(token[left_limit].text)+atoi(token[left_limit+2].text);
    
   } 
   else
   if ( reduction_type=='S'  ) /* Algebra-Solve */
   {
     variable_countup( rvalnum,1 );

          
       mulbuf=token[left_limit].mulvar*token[left_limit].onx,mulbufr=token[left_limit+2].mulvar*token[left_limit+2].onx;
       sumbuf=token[left_limit].sumvar,sumbufr=token[left_limit+2].sumvar;
         
	  // printf("%d %d %d %d\n",sumbuf,sumbufr,mulbuf,mulbufr);
       
       mulbuf-=mulbufr;
	   sumbufr-=sumbuf;
       
       
       if ( mulbuf!=0)
       sumbufr/=mulbuf;
      
     sprintf(flushout,"movaxval %d\n", sumbufr ),strcat(flushstring,flushout); 
     sprintf(flushout,"movvarax %s\n", rvalnum ),strcat(flushstring,flushout);

    remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );

   }
   else
   if ( reduction_type== '='  ) /* Zuweisung */
   {
     variable_countup( rvalnum,1 );

     if ( token[left_limit].type=='r' ){  printf("Zuweisung zu R-Wert !\n"); return 1; }

    if( token[left_limit+2].type!= 'n' )
       sprintf(flushout,"movaxvar %s\n"
              "movvarax %s\n",token[left_limit+2].text, token[left_limit].text ),strcat(flushstring,flushout);
    else
       sprintf(flushout,"movaxvar %s\n"
              "movvarax %s\n",token[left_limit+2].text, token[left_limit].text ),strcat(flushstring,flushout);

     sprintf(flushout,"movvarax %s\n", rvalnum ),strcat(flushstring,flushout);

    remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );

   }
   else
   if ( reduction_type== ':'  ) /* Vergleich */
   {
     variable_countup( rvalnum,1 );

    
    if( token[left_limit+2].type!= 'n'&&token[left_limit].type!= 'n' )
       sprintf(flushout,"movaxvar %s\n"
              "cmpaxvar %s\n",token[left_limit+2].text, token[left_limit].text ),strcat(flushstring,flushout);
    else
	if( token[left_limit].type== 'n'&&token[left_limit+2].type!= 'n' )
       sprintf(flushout,"movaxvar %s\n"
              "cmpaxval %s\n",token[left_limit+2].text, token[left_limit].text ),strcat(flushstring,flushout);
    else
	if( token[left_limit].type!= 'n'&&token[left_limit+2].type== 'n' )
       sprintf(flushout,"movaxval %s\n"
              "cmpaxvar %s\n",token[left_limit+2].text, token[left_limit].text ),strcat(flushstring,flushout);
    else
	if( token[left_limit].type== 'n'&&token[left_limit+2].type== 'n' )
       sprintf(flushout,"movaxval %s\n"
              "cmpaxval %s\n",token[left_limit+2].text, token[left_limit].text ),strcat(flushstring,flushout);

       sprintf(flushout,
              "movaxval 0\n"
			  "repzmovaxval 1\n",token[left_limit+2].text, token[left_limit].text ),strcat(flushstring,flushout);

     sprintf(flushout,"movvarax %s\n", rvalnum ),strcat(flushstring,flushout);

    remove_tokens(left_limit,2 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );

   }
   else
   if ( reduction_type== 'p'  ) /* Ausgabe */
   {
     variable_countup( rvalnum,1 );

    if( token[left_limit+1].type!= '@' )
       sprintf(flushout,"\nPRINTvar %s\n",token[left_limit+1].text),strcat(flushstring,flushout);
     else
       sprintf(flushout,"\nPRINTstr %s\n",token[left_limit+1].text),strcat(flushstring,flushout);
     
    remove_tokens(left_limit,1 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );

   }
   
   else
   if ( reduction_type== '\?'  ) /* Vergleich */
   {
     variable_countup( rvalnum,1 );

     if ( token[left_limit].type=='r' || token[left_limit].type=='n' ){  printf("Zuweisung zu R-Wert oder Konstante!\n"); return 1; }

    if( token[left_limit+1].type== 'v' )
       sprintf(flushout,"INPUTvar %s\n",token[left_limit+1].text),strcat(flushstring,flushout);
       
    remove_tokens(left_limit,1 );

    token[left_limit].type='r';
    strcpy( token[left_limit].text, rvalnum );
    
   }
   else
   if ( reduction_type=='i')
   {
	labelstack[label_stack_count].type='i';
	label_countup(labelstack[label_stack_count].labelnum,1);
   	label_stack_count++;

    if ( token[left_limit+1].type=='n')
   	sprintf(flushout,"cmpvalval %s 1\n"
	      "jz %s\n",token[left_limit+1].text, 
		   labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout);
	  else
	  sprintf(flushout,"cmpvarval %s 1\n"
	       "jz %s\n",token[left_limit+1].text, 
		   labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout);
	    
    remove_tokens(left_limit,1 );
   	 token[left_limit].type='c';
    
   }
   else if ( reduction_type=='f')
   {
   	labelstack[label_stack_count].type='f';
   	label_countup(labelstack[label_stack_count].labelnum,1);
   	label_stack_count++;
   	labelstack[label_stack_count].type='f';
   	label_countup(labelstack[label_stack_count].labelnum,1);
   	label_stack_count++;
    if ( token[left_limit+1].type=='n')
   	sprintf(flushout,"\n:%s\n",labelstack[label_stack_count-2].labelnum),strcat(flushprefix,flushout),
	        
			sprintf(flushout,
			"cmpvalval %s 1\n"
			"jz %s\n",token[left_limit+1].text,labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout);
   else
   	sprintf(flushprefix,"\n:%s\n",labelstack[label_stack_count-2].labelnum),
	  sprintf(flushout,
	         "cmpvarval %s 1\n"
			"jz %s\n",token[left_limit+1].text,labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout);
	   
	      remove_tokens(left_limit,1 );
  	 token[left_limit].type='c';
   
	       
   }
   else if ( reduction_type=='n')
   {
   	if ( label_stack_count==0 ){ if ( num_tokens==1)fprintf(stderr,"end");return; }
   if(label_stack_count<2)
	   if(labelstack[label_stack_count-1].type=='f') return;
   	
   	if ( labelstack[label_stack_count-1].type=='i')sprintf(flushout,":%s\n",labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout),
	   label_stack_count--;
   	else
   	if ( labelstack[label_stack_count-1].type=='f')sprintf(flushout,"\njmp %s\n"
	                                                       "\n%s:\n",labelstack[label_stack_count-2].labelnum,
														   labelstack[label_stack_count-1].labelnum),strcat(flushstring,flushout),
														   label_stack_count-=2 ;
   	else return;
   	
   	 token[left_limit].type='c';
   	
   }
     else if ( reduction_type=='b')
   {
   	int findlabel;
   	findlabel=label_stack_count+1;
   	
   	while(labelstack[findlabel-1].type!='f'&&findlabel > 1 ) findlabel--;
   if ( findlabel==1 ) {printf("Keine For-Schleife vorhanden.");return;}
   	
	   sprintf(flushout,"\njmp %s\n",labelstack[findlabel-1].labelnum),strcat(flushstring,flushout);
   	
   	 token[left_limit].type='c';
   	
   }
 
   else if ( reduction_type==';')
   {
   	if ( right_limit> 1 ) return;
	remove_tokens(left_limit,1 );
    goto newline;
   	
   }
   else
   {
    if( num_tokens > 1 ) { printf("Operator fehlt.\n"); getch(); return 1; }
    else if ( num_tokens==1 )
       if( ( token[0].type!= 'v' && token[0].type!= 'r' &&
          token[0].type!='n') || token[1].type != ';' )
          { printf("Operator fehlt.\n"); getch(); return 1; }

    if ( num_tokens==0 && token[0].type!=';' ) { printf("Syntaxfehler.\n");
          getch(); return 1; }

   }

  }
  getch();
}

/*
this is crappier than Cobol, but anyway...

Test run ...

x = 0 ;
y = 0 ;
xc = 0 ;
yc = 0 ;
for ( yc : 0 ) ;
x = 0 ;
for ( xc : 0 ) ;
x = x + 1 ;
if ( x : 10 ) ;
xc = 1 ;
next ;
screen = 1 ;
next ;
xc = 0 ;
y = y + 1 ;
if ( y : 100 ) ;
yc = 1 ;
next ;
next ;
next ;


produced...

mov ax,0
mov [x],ax
mov [rval1],ax
mov ax,0
mov [y],ax
mov [rval2],ax
mov ax,0
mov [xc],ax
mov [rval3],ax
mov ax,0
mov [yc],ax
mov [rval4],ax

:label1
mov ax,0
cmp ax,[yc]
mov ax,0
repz mov ax,1
mov [rval5],ax
mov [rval6],[rval5]
cmp [rval6],TRUE
jz label2
mov ax,0
mov [x],ax
mov [rval7],ax

:label3
mov ax,0
cmp ax,[xc]
mov ax,0
repz mov ax,1
mov [rval8],ax
mov [rval9],[rval8]
cmp [rval9],TRUE
jz label4
mov ax,1
add ax,[x]
mov [rval10],ax
mov ax,[rval10]
mov [x],ax
mov [rval11],ax
mov ax,10
cmp ax,[x]
mov ax,0
repz mov ax,1
mov [rval12],ax
mov [rval13],[rval12]
cmp [rval13],TRUE
jz label5
mov ax,1
mov [xc],ax
mov [rval14],ax
:label5
mov ax,1
mov [screen],ax
mov [rval15],ax

jmp label3

label4:
mov ax,0
mov [xc],ax
mov [rval16],ax
mov ax,1
add ax,[y]
mov [rval17],ax
mov ax,[rval17]
mov [y],ax
mov [rval18],ax
mov ax,100
cmp ax,[y]
mov ax,0
repz mov ax,1
mov [rval19],ax
mov [rval20],[rval19]
cmp [rval20],TRUE
jz label6
mov ax,1
mov [yc],ax
mov [rval21],ax
:label6

jmp label1

label2:
*/
#endif
