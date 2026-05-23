#include <stdio.h>

typedef struct
{
 int content;
 int max_amount;
 int target_amount;
}bucket;


bucket init_buckets[20];

int num_buckets;
int rec_depth_max=999999;

signed int action[20][2];


void backtrack(bucket *buckets_old, int rec_depth)
{
 bucket buckets[20];
 int n=0,n2,n3;
 n3=-2;
 while (n3<num_buckets)
 {
 n2=0;
 while ( n2<num_buckets)
 {
  if ( n2==n3){n2++;continue;}
  n=0;
  while ( n < num_buckets)
  {
   buckets[n].content=buckets_old[n].content;
   buckets[n].max_amount=buckets_old[n].max_amount;
   buckets[n].target_amount=buckets_old[n].target_amount;
   n++;
  }

  if ( n3==-2)
  {
  if (buckets[n2].content==0){n2++;continue;}
   buckets[n2].content=0;
   action[rec_depth][0]=n2;
   action[rec_depth][1]=-2;
  }
  else
  if ( n3==-1)
  {
   if ( buckets[n2].content==buckets[n2].max_amount){n2++;continue;}
   buckets[n2].content=buckets[n2].max_amount;
   action[rec_depth][0]=n2;
   action[rec_depth][1]=-1;
  }
  else
  {
    int buf;
    buf=buckets[n2].content;
   buckets[n2].content+=buckets[n3].content;
   if ( buckets[n2].content> buckets[n2].max_amount ) buckets[n2].content=buckets[n2].max_amount; 
   buckets[n3].content-=buckets[n2].content-buf;
   action[rec_depth][0]=n2;
   action[rec_depth][1]=n3;
  }
  int n4=0;
  n4=0;
  while (n4<num_buckets)
  {
   if ( buckets[n4].content!=buckets[n4].target_amount)break;
   n4++;
  }
  if ( n4!= num_buckets)
  {
   if ( rec_depth<30) backtrack(buckets,rec_depth+1);
  }
  else if (rec_depth<=rec_depth_max)
  {
   n4=0;
   printf("Loesung\n");
   while ( n4<=rec_depth)
   {
    printf("Kanister %d ",action[n4][0]);
    if ( action[n4][1]==-2)
    {
     printf("leeren\n");
    }
    else
    if ( action[n4][1]==-1)
    {
     printf("fuellen\n");
    }
    else
    printf("mit Kanister %d fuellen\n",action[n4][1]);
   
    n4++;
    rec_depth_max=rec_depth;
   }
  
  }

  n2++;
  }
  n3++;
 }



}


int main(void)
{
  int n=0;
  
  printf("Anzahl Kanister eingeben:");
  scanf("%d",&num_buckets);
  
  n=0;
  while ( n < num_buckets)
  {
   printf("Fuer Kanister %d Fuellstand, Maximalmenge, Soll-Fuellstand:",n);
   scanf("%d %d %d",&init_buckets[n].content,&init_buckets[n].max_amount,&init_buckets[n].target_amount);
   n++;
  }
  
  backtrack(init_buckets,0);

}
