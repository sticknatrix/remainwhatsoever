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
   int ip_stack_c;
   int ip_stack[5];
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
 tasktable[active_task].regsreset.ip_stack_c=vmregs.ip_stack_c;
 tasktable[active_task].regsreset.ip_stack[0]=vmregs.ip_stack[0];
 tasktable[active_task].regsreset.ip_stack[1]=vmregs.ip_stack[1];
 tasktable[active_task].regsreset.ip_stack[2]=vmregs.ip_stack[2];
 tasktable[active_task].regsreset.ip_stack[3]=vmregs.ip_stack[3];
 tasktable[active_task].regsreset.ip_stack[4]=vmregs.ip_stack[4];
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
 vmregs.ip_stack_c=tasktable[tasknum].regsreset.ip_stack_c;
 vmregs.ip_stack[0]=tasktable[tasknum].regsreset.ip_stack[0];
 vmregs.ip_stack[1]=tasktable[tasknum].regsreset.ip_stack[1];
 vmregs.ip_stack[2]=tasktable[tasknum].regsreset.ip_stack[2];
 vmregs.ip_stack[3]=tasktable[tasknum].regsreset.ip_stack[3];
 vmregs.ip_stack[4]=tasktable[tasknum].regsreset.ip_stack[4];
 
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
    //printf("os hat zur¸ckgeschickt %d",syscall_answer.msg);
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
  printf("Lade...");
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
     fgetc(progimage);// the so called linker has to insert one character that fscanf does not overread while reading variable lengths also
	while(!feof(progimage))RAM[loadpos+program_len+255+2]=fgetc(progimage)&255|(fgetc(progimage)<<8),RAM[loadpos+program_len+255+2]>32767? (RAM[loadpos+program_len+255+2]=(65535-RAM[loadpos+program_len+255+2])*-1) : 0 ,program_len++;
  	 RAM[loadpos+program_len+255+2-1]=38;
	   fclose(progimage);
     tasktable[n].len=program_len;
     tasktable[n].varlen=var_len;
     tasktable[n].address=loadpos;
   tasktable[n].regsreset.ip=loadpos+255+2;
   tasktable[n].regsreset.di_off=program_len+loadpos+255+2;
   tasktable[n].regsreset.ip_stack_c=0;
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
      tasktable[n].regsreset.ip_stack[0]-=tasktable[syscall_answer.task].len+tasktable[syscall_answer.task].varlen+255+2;	
      tasktable[n].regsreset.ip_stack[1]-=tasktable[syscall_answer.task].len+tasktable[syscall_answer.task].varlen+255+2;	
      tasktable[n].regsreset.ip_stack[2]-=tasktable[syscall_answer.task].len+tasktable[syscall_answer.task].varlen+255+2;	
      tasktable[n].regsreset.ip_stack[3]-=tasktable[syscall_answer.task].len+tasktable[syscall_answer.task].varlen+255+2;	
      tasktable[n].regsreset.ip_stack[4]-=tasktable[syscall_answer.task].len+tasktable[syscall_answer.task].varlen+255+2;	
      tasktable[n].regsreset.di_off-=tasktable[syscall_answer.task].len+tasktable[syscall_answer.task].varlen+255+2;	
    //tasktable[n].len=0;
   }
   n++;
  }
 //remove
  tasktable[syscall_answer.task].len=0;
  syscall_answer.task=-1;
  printf("beende...");
   os_ready=0;
 }
  else
 if ( syscall_answer.msg==2&&os_ready==1)
 { 
 //print
  printf("[Task %d]%s",syscall_answer.task,syscall_answer.param);
   syscall_answer.task=-1;
  os_ready=0;
 }
  else
 if ( syscall_answer.msg==3&&os_ready==1)
 { 
   printf("[Task %d]Eingabe:",syscall_answer.task);
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
	    //while (n4<100){printf("%d ",RAM[vmregs.ip+n4]),n4++; if ( vmregs.ip+n4==389)printf("dada");}
	if ( RAM[tasktable[active_task].address+102]==1)
	{
	 if ( RAM[tasktable[active_task].address+102+1]==4)
	 {
		//printf("binauchda");
	 	if ( readvaladr!=-1)
	 	RAM[readvaladr]=RAM[tasktable[active_task].address+160];
	 	RAM[tasktable[active_task].address+102]=0;
	 	readvaladr=-1;
	 }
		
	}
	if ( readvaladr!=-1) return;
	if ( RAM[tasktable[active_task].address]==1)return;
	//getchar();
 if ( RAM[vmregs.ip]==0)
 {
  vmregs.ax=RAM[vmregs.di_off+RAM[vmregs.ip+1]];
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==1)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]] =vmregs.ax;
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==29)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]] =RAM[vmregs.di_off+RAM[vmregs.ip+2]];
  vmregs.ip+=3;
 }
 else
 if ( RAM[vmregs.ip]==32)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]] =RAM[vmregs.ip+2];
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
 if ( RAM[vmregs.ip]==39)
 {
   if ( vmregs.greater_flag==1 )
   {
    vmregs.ax=1;
   }
   vmregs.ip++;
  }
  else
 if ( RAM[vmregs.ip]==40)
 {
   if ( vmregs.lesser_flag==1 )
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
  vmregs.ax-=RAM[vmregs.ip+1];
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==7)
 {
  vmregs.ax*=RAM[vmregs.ip+1];
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
  if(RAM[vmregs.ip+1]==0){printf("Division durch Wert 0");return;}
   vmregs.ax/=RAM[vmregs.ip+1];
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
  vmregs.ax-=RAM[vmregs.ip+1];
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==18)
 {
  vmregs.ax*=RAM[vmregs.ip+1];
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
  if(RAM[vmregs.ip+1]==0){printf("Division durch Wert 0");return;}
   vmregs.ax/=RAM[vmregs.ip+1];
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
  if ( vmregs.if_flag==0)vmregs.ip+=RAM[vmregs.ip+1]>0? RAM[vmregs.ip+1]+1 : RAM[vmregs.ip+1];
  else vmregs.ip+=2;
 } 
 else
 if ( RAM[vmregs.ip]==30)
 {
  vmregs.ip+=RAM[vmregs.ip+1]>0 ? RAM[vmregs.ip+1]+1 : RAM[vmregs.ip+1];
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
 
 if ( RAM[vmregs.ip]==33)
 {
  RAM[vmregs.di_off+RAM[vmregs.ip+1]]=RAM[vmregs.di_off+RAM[vmregs.di_off+RAM[vmregs.ip+2]]];
  vmregs.ip+=3;
 }
 else
 if ( RAM[vmregs.ip]==34)
 {
 RAM[vmregs.di_off+RAM[vmregs.di_off+RAM[vmregs.ip+1]]]=vmregs.ax;
  vmregs.ip+=2;
 }
 else
 if ( RAM[vmregs.ip]==35)
 {
  vmregs.ax=RAM[vmregs.ip+1];
  vmregs.ip+=2;
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
 if ( RAM[vmregs.ip]==38)
 {
  RAM[tasktable[active_task].address]=1;
  RAM[tasktable[active_task].address+1]=1;
  vmregs.ip++;
 }
  else
 if ( RAM[vmregs.ip]==36)
 {
  vmregs.ip_stack[vmregs.ip_stack_c]=vmregs.ip+2;
   vmregs.ip_stack_c++;
    if ( vmregs.ip_stack_c==5 ){printf("Adressstack ist uebergelaufen"); return; }
  vmregs.ip+=RAM[vmregs.ip+1]> 0 ? RAM[vmregs.ip+1]+1: RAM[vmregs.ip+1];
  
 }
  else
 if ( RAM[vmregs.ip]==37)
 {
   if ( vmregs.ip_stack_c==0){ printf("Adressstack wuerde unterlaufen");return;}
  vmregs.ip_stack_c--;
  vmregs.ip=vmregs.ip_stack[vmregs.ip_stack_c];
 }


 vmsteps--;
}
 //bei Adressoperator weist man ohne eckige Klammern zu R-Wert, bei Inhaltsoperator kopiert man Speicherzelle in Register und indiziert es anschlieﬂend?!?
 
/* 
 if ( strcmp(input,"movaxvar" 0)==0)
  if ( strcmp(input,"movarax" 1)==0)
  if ( strcmp(input,"movaxval" 2)==0)
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
    movvarval 32
    movrptr 33
    moviax 34
    movaxptr 35
    call  36
    ret  37
    exit 38
    repgmovaxval 39
    replmovaxval 40
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
 syscall_answer.msg=0;
 os_ready=1;
 strcpy(syscall_answer.param,"firstprog.btc");
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
