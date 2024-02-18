#include <stdio.h>
#include <stdlib.h>

#define PLAYER 1
#define COM    2
#define EMPTY  0
#define NO_WINNER 0

#define KI_LOOKSFORWARD 8

unsigned char brett[7][7];
unsigned char brett_lookahead[7][7];
int end;

int insert_chip(int slot, int opponent)
{
 int height=0;

 while( brett[height][slot]!= EMPTY && height < 7 ) height++;
 if ( height < 7 ){  brett[height][slot]=opponent; return 0; }
 else return 1;

}

void debug_display_brett()
{
  int c1=0, c2=0;
 
 system("cls");
 
 c1=6;
 while(c1>=0)
 {
  c2=0;
  while(c2<7 )
  {
   switch( brett_lookahead[c1][c2] )
   {
     case PLAYER: putch('P'); break;
     case COM : putch('C'); break;
     default : putch(' '); break;
    }
   c2++;
  }
   putch('\n' );
  c1--;
 } 
 printf("1234567");
 
}

int highest_chip[7];

int KI_insert_chip(int slot, int opponent)
{
 int height=0;

 while( brett_lookahead[height][slot]!= EMPTY && height < 7 ) height++;
 if ( height < 7 ){  brett_lookahead[height][slot]=opponent;
                     highest_chip[slot]=height; return 0; }
 else return 1;


}

int KI_remove_chip(int slot)
{
 brett_lookahead[highest_chip[slot]][slot]=EMPTY;
 highest_chip[slot]--;
}



void check_victory()
{
 int height=0, slot=0;
 int count=0;

 do
 {
  while ( brett[height][slot]==EMPTY  && slot < 7 )
  {
   height++; if ( height == 7 ) height=0, slot++;

  }
   if ( slot== 7 ) return;
  if ( brett[height][slot]==PLAYER )
  {
   count=0;
   while ( height+count < 7 && count < 4 && brett[height+count][slot]==PLAYER )
    count++;
   if ( count==4 ) { end=1; return; }

   count=0;
   while ( slot+count < 7 && count < 4 && brett[height][slot+count]==PLAYER )
    count++;
   if ( count==4 ) { end=1; return; }

   count=0;
   while ( slot+count < 7 &&
           height+ count < 7 && count < 4 && brett[height+count][slot+count]==PLAYER )
    count++;
   if ( count==4 ) { end=1; return; }

      count=0;
   while ( slot+count < 7 &&
           height - count >= 0 && count < 4 && brett[height-count][slot+count]==PLAYER )
    count++;
   if ( count==4 ) { end=1; return; }


  }
  else if ( brett[height][slot]==COM )
  {
   count=0;
   while ( height+count < 7 && count < 4 && brett[height+count][slot]==COM )
    count++;
   if ( count==4 ) { end=1; return; }

   count=0;
   while ( slot+count < 7 && count < 4 && brett[height][slot+count]==COM )
    count++;
   if ( count==4 ) { end=1; return; }

   count=0;
   while ( slot+count < 7 &&
           height+ count < 7 && count < 4 && brett[height+count][slot+count]==COM )
    count++;
   if ( count==4 ) { end=1; return; }

      count=0;
   while ( slot+count < 7 &&
           height - count >= 0 && count < 4 && brett[height-count][slot+count]==COM )
    count++;
   if ( count==4 ) { end=1; return; }

   }

   height++; if ( height == 7 ) height=0, slot++;

  } while(1);
}


void player_move()
{
 unsigned char c;
 
 do
 {
  do { c=getch(); } while( !( c  >= '1' && c < '8' ) && c != 'q' );
   if ( c== 'q' ) { end=1; return; }
 } while( insert_chip(c-0x31,PLAYER ) != 0 );

}


int KI_upperlinefull()
{
  int col=0;

  while( col < 7 )
  {
   if ( brett_lookahead[6][col]==EMPTY ) return 0;
   col++;
  }
  return 1;
}

int KI_victory_check(int start, int who)
{
 int height=0, slot=0;
 int count=0;

 slot= start -3 > 0 ? start-3 : 0;

 do
 {
  while ( brett_lookahead[height][slot] != who && slot < 7 )
  {
   height++; if ( height == highest_chip[slot]+2 ) height=0, slot++;

  }
   if ( slot== 7 ) return NO_WINNER;
  if ( brett_lookahead[height][slot]==PLAYER )
  {
   count=0;

   if ( height+3 < 7 )
   while ( count < 4 && brett_lookahead[height+count][slot]==PLAYER )++count;
   if ( count==4 ) { return PLAYER; }

   count=0;
   if ( slot+3 < 7 )
   while ( count < 4 && brett_lookahead[height][slot+count]==PLAYER )++count;
   if ( count==4 ) { return PLAYER; }

   count=0;
   if ( slot+3 < 7 && height+3 < 7 )
   while ( count < 4 && brett_lookahead[height+count][slot+count]==PLAYER )++count;
   if ( count==4 ) { return PLAYER; }

      count=0;
   if ( slot+3 < 7 && height-3 >= 0 )
   while ( count < 4 && brett_lookahead[height-count][slot+count]==PLAYER )++count;
   if ( count==4 ) { return PLAYER; }


  }
  else if ( brett_lookahead[height][slot]==COM )
  {
   count=0;
   if ( height+3 < 7 )
   while ( count < 4 && brett_lookahead[height+count][slot]==COM )++count;
   if ( count==4 ) { return COM; }

   count=0;
   if ( slot+3 < 7 )
   while ( count < 4 && brett_lookahead[height][slot+count]==COM )++count;
   if ( count==4 ) { return COM; }

   count=0;
   if ( slot+3 < 7 && height+3 < 7 )
   while ( count < 4 && brett_lookahead[height+count][slot+count]==COM )++count;
   if ( count==4 ) { return COM; }

      count=0;
   if ( slot+3 < 7 && height-3 >= 0 )
   while ( count < 4 && brett_lookahead[height-count][slot+count]==COM ) ++count;
   if ( count==4 ) { return COM; }

   }

   height++; if ( height == 7 ) height=0, slot++;

  } while(1);
}


int commove_dig(int n, int from, int rec_depth)
{
 int retcode, ret_change, ret_ctr, retbuf, retbuf2, retbuf2_sub;

     if ( rec_depth > 7 ) return NO_WINNER;

     /* debug_display_brett(); */


    if ( from==COM )
      {
       if ( KI_victory_check(n,COM)==COM ){ return COM; }

        n=0;

        ret_change=NO_WINNER;
        ret_ctr=7;
        retcode=PLAYER;
        retbuf=NO_WINNER;
        retbuf2=0, retbuf2_sub=0;

        while ( n < 7 )
       {
        if ( KI_insert_chip(n,PLAYER) != 0  ){ n++; retbuf2_sub++; if ( n==7 && ret_ctr==7 ) ret_change=NO_WINNER; continue; }
         if ( KI_victory_check(n,PLAYER)==PLAYER ) { KI_remove_chip(n); return PLAYER; }

           if ( (retbuf=commove_dig(n, PLAYER, rec_depth+1 )) == PLAYER ) ret_ctr=0;
          if ( retbuf==COM ) retbuf2++;

          KI_remove_chip(n);
          n++;
         }

         if ( ret_ctr==0 ) ret_change=PLAYER;
         else if( retbuf2 > 0 && retbuf2+retbuf2_sub==7 ) ret_change=COM;

      }

      if ( from==PLAYER )
      {

    if ( KI_victory_check(n,PLAYER)==PLAYER ){ return PLAYER; }


        n=0;
        retcode=PLAYER;
        ret_change=PLAYER;
        ret_ctr=0;
        retbuf=NO_WINNER;
        retbuf2=NO_WINNER;

        while ( n < 7 )
        {
         if ( KI_insert_chip(n,COM) != 0  ){ n++; if ( n==7 && ret_ctr!=6 ) ret_change=NO_WINNER; continue; }
         if ( KI_victory_check(n,COM)==COM ) { KI_remove_chip(n); return COM; }

         if(  (retbuf=commove_dig(n, COM, rec_depth+1 ))!= PLAYER ) ret_ctr=7;
          else if ( ret_ctr!=7 ) ret_ctr=6;

          if ( retbuf==COM ) retbuf2=COM;

         KI_remove_chip(n);

         n++;
        }

        if ( ret_ctr!=6 ) { ret_change=NO_WINNER; if ( retbuf2==COM ) ret_change=COM; }

      }

      retcode=ret_change;



      if ( rec_depth == 0 )
      {
       if ( retcode== COM ) printf("Computer gewinnt,");
       else if ( retcode== NO_WINNER ) printf("Keiner gewinnt");
       else if ( retcode== PLAYER ) printf("Spieler gewinnt");

      }


    return retcode;
}


void computer_move()
{

  struct
  {
  signed long int bewertung;
   long int win_possible;
   long int loss, blocked, noloss;
   long int loss_e1, loss_e2, loss_e3, loss_e4;
   long int noloss_e1, noloss_e2, noloss_e3;
   int verboten;
  } zugwahl[7];
   int bester_zug=0;
   /*
   long int losscomb[343][3];
   long int losscombs;
   long int loss_identic_e1, loss_identic_e2, loss_identic_e3; */

   int dupectr=0;
   int dupectr2=0;
   long int dupebuf;

   signed long int bewertung_upscale;

  int c1=0, c2=0;
  int move1=0, move2=0, move3=0, move4=0, move5=0, move6=0, move7=0, move8, move9;
  int n=0;

  while(n < 7 )
  {

   c1=0, c2=0;
   while(c1<7 )
   {

    c2=0;
    while( c2 < 7 )
    {
     brett_lookahead[c1][c2]=brett[c1][c2];
     c2++;
    }
    c1++;
   }

   c2=0;
   while ( c2< 7 )
   {
    c1=0;
    while ( brett_lookahead[c1][c2]!= EMPTY ) c1++;
    highest_chip[c2]=c1;
    c2++;
   }

   while ( KI_insert_chip(n,COM ) != 0 && n < 7 )
     {
                                       zugwahl[n].verboten= 1,
                                       zugwahl[n].bewertung=0,
                                       zugwahl[n].win_possible=0,
                                         n++;
      }
    if ( n == 7 ) break;
    else zugwahl[n].bewertung=0, zugwahl[n].verboten=0, zugwahl[n].win_possible=0,
         zugwahl[n].loss=0, zugwahl[n].blocked=0, zugwahl[n].noloss=0,
         zugwahl[n].loss_e1=0, zugwahl[n].loss_e2=0,zugwahl[n].loss_e3=0, zugwahl[n].loss_e4=0,
         zugwahl[n].noloss_e1=0,zugwahl[n].noloss_e2=0,zugwahl[n].noloss_e3=0, zugwahl[n].loss_e4=0,

         /* losscombs=0; */

         dupectr=0;

    move1=0, move2=0, move3=0, move4=0, move5=0, move6=0, move7=0, move8=0, move9=0;
    bewertung_upscale=0;

    bewertung_upscale=commove_dig(n,COM,0);

    if ( bewertung_upscale==PLAYER ) zugwahl[n].bewertung=-1000000;
    else if ( bewertung_upscale==COM ) zugwahl[n].bewertung=1000000;
    else zugwahl[n].bewertung=0;

    /* goto skipruns; */

   while ( move3 < 7 ) /* bei Kombination 13652321 wars 50000 ohne move7 nicht mehr mit KI_LOOKSFORWARD */
   {

    if ( KI_victory_check(n,COM)==COM ){ /* zugwahl[n].noloss=1;  zugwahl[n].loss=0; */ zugwahl[n].win_possible++; goto newcomb; }

    if ( KI_insert_chip(move4,PLAYER) != 0  ){ goto newcomb; }
    if ( KI_victory_check(move4,PLAYER)==PLAYER ) { zugwahl[n].loss=1; zugwahl[n].bewertung-=7; zugwahl[n].noloss=1;
      KI_remove_chip(move4); goto newcomb; }


    if ( KI_insert_chip(move1,COM) != 0  ) { zugwahl[n].bewertung--; zugwahl[n].blocked++; KI_remove_chip(move4); goto newcomb; }
    if (KI_victory_check(move1,COM)==COM ){ /* zugwahl[n].loss=0; */ zugwahl[n].win_possible++;
      KI_remove_chip(move1), KI_remove_chip(move4);goto newcomb; }

    if ( KI_insert_chip(move5,PLAYER) != 0 ) { /* zugwahl[n].noloss=1, zugwahl[n].loss=0; */ KI_remove_chip(move1); KI_remove_chip(move4); goto newcomb; }
    if (KI_victory_check(move5,PLAYER)==PLAYER ){ zugwahl[n].loss=1; zugwahl[n].bewertung-=7; zugwahl[n].noloss=1;
      KI_remove_chip(move5),
      KI_remove_chip(move1), KI_remove_chip(move4); goto newcomb; }

    if ( KI_insert_chip(move2,COM) != 0 ) { zugwahl[n].bewertung--; zugwahl[n].blocked++; KI_remove_chip(move5); KI_remove_chip(move1); KI_remove_chip(move4); goto newcomb; }
    if (KI_victory_check(move2,COM)==COM ){ /* zugwahl[n].noloss=1, zugwahl[n].loss=0; */ zugwahl[n].win_possible++;
      KI_remove_chip(move2), KI_remove_chip(move5),
      KI_remove_chip(move1), KI_remove_chip(move4);goto newcomb; }


    if ( KI_insert_chip(move6,PLAYER) != 0 ) { /* zugwahl[n].noloss=1, zugwahl[n].loss=0; */ KI_remove_chip(move2); KI_remove_chip(move5); KI_remove_chip(move1); KI_remove_chip(move4); goto newcomb; }
    if (KI_victory_check(move6,PLAYER)==PLAYER ){
          zugwahl[n].loss=1; zugwahl[n].bewertung-=7; zugwahl[n].noloss=1;
      KI_remove_chip(move6), KI_remove_chip(move2), KI_remove_chip(move5),
      KI_remove_chip(move1), KI_remove_chip(move4); goto newcomb; }


    if ( KI_insert_chip(move3,COM) != 0 ) { zugwahl[n].bewertung--; zugwahl[n].blocked++; KI_remove_chip(move6); KI_remove_chip(move2); KI_remove_chip(move5); KI_remove_chip(move1); KI_remove_chip(move4);goto newcomb; }
    if (KI_victory_check(move3,COM)==COM ){ /* zugwahl[n].noloss=1, zugwahl[n].loss=0; */ zugwahl[n].win_possible++;
      KI_remove_chip(move3), KI_remove_chip(move6), KI_remove_chip(move2), KI_remove_chip(move5),
      KI_remove_chip(move1), KI_remove_chip(move4);goto newcomb; }



   /*
  if ( KI_insert_chip(move7,PLAYER) != 0 ) { KI_remove_chip(move3 );KI_remove_chip(move6 );KI_remove_chip(move2); KI_remove_chip(move5); KI_remove_chip(move1); KI_remove_chip(move4); goto newcomb; }
   if (KI_victory_check(move7,PLAYER)==PLAYER ){ zugwahl[n].loss=1; zugwahl[n].bewertung-=7; zugwahl[n].noloss=1;
     KI_remove_chip(move7);KI_remove_chip(move3 );
     KI_remove_chip(move6), KI_remove_chip(move2), KI_remove_chip(move5),
     KI_remove_chip(move1), KI_remove_chip(move4);  goto newcomb; }



      if ( KI_insert_chip(move8,COM) != 0 ) { zugwahl[n].blocked++; KI_remove_chip(move7); KI_remove_chip(move3); KI_remove_chip(move6); KI_remove_chip(move2); KI_remove_chip(move5); KI_remove_chip(move1); KI_remove_chip(move4);goto newcomb; }
    if (KI_victory_check(move8,COM)==COM ){ zugwahl[n].win_possible++;
      KI_remove_chip(move8),KI_remove_chip(move7), KI_remove_chip(move3), KI_remove_chip(move6), KI_remove_chip(move2), KI_remove_chip(move5),
      KI_remove_chip(move1), KI_remove_chip(move4);goto newcomb; }

     if ( KI_insert_chip(move9,PLAYER) != 0 ) { KI_remove_chip(move8 ); KI_remove_chip(move7); KI_remove_chip(move3 );KI_remove_chip(move6 );KI_remove_chip(move2); KI_remove_chip(move5); KI_remove_chip(move1); KI_remove_chip(move4); goto newcomb; }
   if (KI_victory_check(move9,PLAYER)==PLAYER ){ zugwahl[n].bewertung--;
      KI_remove_chip(move9 ); KI_remove_chip(move8 );KI_remove_chip(move7);KI_remove_chip(move3 );
     KI_remove_chip(move6), KI_remove_chip(move2), KI_remove_chip(move5),
     KI_remove_chip(move1), KI_remove_chip(move4);goto newcomb; }



    KI_remove_chip(move9), KI_remove_chip(move8),  KI_remove_chip(move7), */
    KI_remove_chip(move3), KI_remove_chip(move6), KI_remove_chip(move2), KI_remove_chip(move5),
    KI_remove_chip(move1), KI_remove_chip(move4);


    newcomb:

     move4++;

    if ( move4==7 ) move5++, move4=0;
    if ( move5==7 ) move6++, move5=0;
    if ( move6==7 )
      /* move7++, move6=0;
    if ( move7==7 ) */
    {
      /*
      if ( zugwahl[n].loss==0 ) printf("Zug %d ist sicher!\n", n);
       else
       {
         losscomb[losscombs][0]=move1;
         losscomb[losscombs][1]=move2;
         losscomb[losscombs][2]=move3;
         losscombs++;
        }
      zugwahl[n].loss=0;
       */
      move1++, move6=0;
    }
    if ( move1==7 ) move2++, move1=0;
    if ( move2==7 ) move3++, move2=0;

    /* if ( move3==7 ) move4++, move3=0, move2=0, move1=0; */
    

   }

   /* skipruns: */


   /*
   if ( bewertung_upscale < 117649 )
   zugwahl[n].bewertung*= 117649/(117649-bewertung_upscale),
   zugwahl[n].win_possible*= 117649/(117649-bewertung_upscale) ;
   */

   n++;

  }

  bester_zug=0;
  n=0; while ( zugwahl[n].verboten ==1 && n<6) n++, bester_zug=n;
   n=0;
    do { if ( zugwahl[bester_zug].bewertung == zugwahl[n].bewertung &&
               zugwahl[n].verboten == 0 && zugwahl[n].win_possible >
                 zugwahl[bester_zug].win_possible )
                bester_zug=n;
          else  if ( zugwahl[n].bewertung > zugwahl[bester_zug].bewertung &&
                zugwahl[n].verboten == 0  )
                bester_zug=n;

               /* mindestens soviele Verlierkombinationen, wie der Computer insgesamt Zugmoeglichkeiten hat

               if ( zugwahl[n].bewertung >= 7*7*7*7 ) printf( "Zug %d hat die Bewertung : %d\n", n, zugwahl[n].bewertung ), getch(); */

                n++; }
      while ( n < 7 ) ;
  if ( zugwahl[bester_zug].verboten == 1 ) { printf( "Kein Zug moeglich!\n"), getch(); end=1; }
  else
  insert_chip( bester_zug, COM )!= 0 ? ( printf("Fehler"), getch()) : 0;
   
}


void display_brett()
{
  int c1=0, c2=0;
 
 system("cls");
 
 c1=6;
 while(c1>=0)
 {
  c2=0;
  while(c2<7 )
  {
   switch( brett[c1][c2] )
   {
     case PLAYER: putch('P'); break;
     case COM : putch('C'); break;
     default : putch(' '); break;
    }
   c2++;
  }
   putch('\n' );
  c1--;
 } 
 printf("1234567");
 
}


int main(void)
{
 int c1=0, c2=0;
 end=0;
 
 while(c1<7 )
 {
  c2=0;
  while(c2<7 )
  {
   brett[c1][c2]=0;
   c2++;
  }
  c1++;
 } 
 
 
 do
 {
  display_brett();

  player_move();
  check_victory();
  display_brett();
  if ( end==1 ) break;

  computer_move();
  check_victory();
  display_brett();

 } while( end==0 );

 printf("\nSpiel beendet!\n"); getch();

}
