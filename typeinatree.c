#include <stdio.h>

/*
  little nonsense program.
  just practicing tree structures..
  */


unsigned char screen[160][120];


typedef struct
{
 int value;

 void
 *parent,
 *left, *right;

} baum;


void sortbaum ( baum *root)
{	
	 
 if ( root->left!= NULL && root->right!=NULL)
 {
 	if ( ((baum *)(root->left))->value > ((baum *)(root->right))->value)
 	 {
	  baum *buf;
 	  buf=root->left;  
      root->left=root->right;
      root->right=buf;
	 }
	 	
  }	

  
  if ( root->left != NULL) sortbaum( root->left);
  if ( root->right!= NULL) sortbaum ( root->right);
	
}


void insert ( baum *leaf, int value, int lorr )
{
 if ( lorr == 0 )
 {
  leaf->left=malloc ( sizeof ( baum ));
  leaf=leaf->left;
  leaf->value=value;
  leaf->left=NULL, leaf->right=NULL;

 }

  if ( lorr == 1 )
 {
  leaf->right=malloc ( sizeof ( baum ));
  leaf=leaf->right;
  leaf->value=value;
  leaf->left=NULL, leaf->right=NULL;

 }


}


void *returndeepest(baum *baump, int lorr)
{
 if ( lorr==0 ) if ( baump->left != NULL) return returndeepest(baump->left, 0 ); else return baump;
 if ( lorr==1 ) if ( baump->right != NULL) return returndeepest(baump->right, 1 ); else return baump;
	
}

void *returnnexthigher(baum *baump, int value )
{
 
 if ( baump->left != NULL) if ( value <= baump->value) return returnnexthigher(baump->left, value );
 if ( baump->right != NULL)if ( value >= baump->value) return returnnexthigher(baump->right, value );

 return baump;
}



void dump( baum *root, int depth, int x, int y)
{

 if ( root->left!=NULL) dump(root->left,depth+1, x -20/((depth+1)*2), y+5);
 if ( root->right!=NULL) dump(root->right,depth+1,x+20/((depth+1)*2), y+5);

 if ( x < 160 && y < 90 && y > 0 && x > 0 )screen[x][y]=root->value;
 
	
}



int main(void)
{
 int value;
 baum *baump;
	

  baump=malloc(sizeof(baum));
  
  baump->value='x';
  baump->left=NULL, baump->right=NULL;
  
  	while ( 1)
  	{ while ( !isalnum(tolower(value=getch())) );
		           
		           baum *leafptr;
		           
		           leafptr=returnnexthigher(baump,value);
				   
				   if ( leafptr->value >= value)
				   insert( leafptr ,value,0);
				   else
				   insert( leafptr ,value,1);
				   

		   	
	  		
		    		
   int x=0, y=0;
 
   while ( y < 80)
   {
   x=0;
   while ( x < 160)
   {
   	screen[x][y]=' ';
  	x++;
	  }	
 	 y++;
    }
  	
	 dump(baump,0,60,5);	
  
  system("cls\n");
   y=0;
   while ( y < 50)
   {
   	int x;
   x=0;
   while ( x < 160)
   {
   	printf("%c",screen[x][y]);
  	  x++;
	  }	
	  printf("\n");
 	 y++;
    }
  
   unsigned char squeeze[160];
   
   x=0;
   while ( x < 160) squeeze[x]=' ',x++;
   y=0;
   while ( y < 80)
   {
   	x=0;
   	while ( x < 160)
   	{
   	 if ( screen[x][y]!=' ') squeeze[x]=screen[x][y];	
   	  x++;
	}
   	y++;
   }
  	 x=0; while ( x < 160) printf("%c",squeeze[x]),x++;	
		  	
	}
	
}
