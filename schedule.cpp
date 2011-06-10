/* Schedule.c
 *
 * Determines a schedule for n-players playing n/2 games
 * per match for n-1 matches, where the same two players
 * never meet more than once.
 *
 * Statistically speaking find (n/2)*(n-1) unique combinations
 * of n items.
 *
 * Richard A. DeVenezia
 * Copyright 1992
 */

//#include <cstdio>
#include <cstdlib>

#define FALSE 0
#define TRUE  1

#define MaxTeams 20
#define MaxCombinations (MaxTeams/2)*(MaxTeams-1)

struct game { int one, two; };

// Wrap it in a struct so it is contained and variables do not interfer with
// my own. (by Ian Murray)
typedef struct {
  int   teams;        /* number of teams (golf) or players (tennis?) */
  long  combinations; /* total combinations needed to schedule season */
  int   a, b, c, i, m,
        startC,
        matchCount,
        roundCount,
        myIndex;

  long  round_set;    /* used during searches, if a bit is set then that team has
                       * already been tenatively scheduled for the round */
  long  totalChecks;

  struct game tourn[1+MaxCombinations];   /* games */
  int    mList[1+MaxTeams/2];             /* matches */
  struct game cList[1+MaxCombinations];   /* combinations list */
  int    cUsed[1+MaxCombinations];        /* combinations used list */


  /*
  void ShowSchedule()
  {
    int myIndex, r, m ;

    fprintf( stdout, "\n%d teams", teams );
    fprintf( stdout, "\n        ");
    for (r=1; r <= teams/2; r++) fprintf( stdout, " Game%d", r);
    fprintf( stdout, "\n" );
    fprintf( stdout, "        +-");
    for (r=1; r <= (teams/2)*6-2; r++) fprintf( stdout, "-" );
    fprintf( stdout, "\n" );

    myIndex = 1;
    for (r=1; r <= teams-1; r++) {
      fprintf( stdout, "Week %2d |", r);
      for (m=1; m <= teams/2; m++) {
        fprintf( stdout, "%2d&%2d ", tourn[myIndex].one, tourn[myIndex].two );
        myIndex++;
      }
      fprintf( stdout, "\n" );
    }
    fprintf( stdout, "\n%ld combinations tried\n\n", totalChecks );
  }
  //*/

  void ClearArrays()
  {
    int i;

    for (i=0; i <= MaxCombinations; i++) { tourn[i].one = 0; tourn[i].two = 0; }
    for (i=0; i <= MaxTeams/2; i++) mList[i] = 0;
    for (i=0; i <= MaxCombinations; i++) { cList[i].one = 0; cList[i].two = 0; }
    for (i=0; i <= MaxCombinations; i++) cUsed[i] = 0;
  }

  void schedule(int set_teams)
  {
    teams = set_teams;

    //while (teams <= MaxTeams) {

      combinations = teams/2 * (teams-1);
      totalChecks = 0;

      ClearArrays();

      /* set up list of all combinations */  /*       a       */
      m = 1;                                 /* b 1 2 3 4 5   */
      for (a=1;   a < teams; a++)            /* 1             */
      for (b=a+1; b <=teams; b++) {          /* 2 .           */
        cList[m].one = a;                    /* 3 . .         */
        cList[m].two = b;                    /* 4 . . .       */
        m++;                                 /* 5 . . . .     */
      }

      roundCount = 1;
      myIndex = 1;

      while (roundCount <= teams-1) {

        matchCount = 1;
        round_set = 0;
        for (i=0; i <= MaxTeams/2; i++) mList[i] = 0;
        startC = roundCount;

        /* proceed with optimism, we will find matches to fill out the round */

        while (matchCount <= teams/2) {

              c = combinations + 1;
              while (c > combinations) {

                c = startC;

                /* find an unused pair that would be legitimate */
                while ((c <= combinations) &&
                       ((round_set & (1 << cList[c].one)) ||
                        (round_set & (1 << cList[c].two)) ||
                        (cUsed[c])
                       )
                      )
                  c++;

                if (c > combinations) {
               /* did not find an unused legitimate pair, back off */
                  do {
                   mList[matchCount] = 0;

                   matchCount--;
                   myIndex--;

                   round_set &= ~(1 << cList[mList[matchCount]].one);
                   round_set &= ~(1 << cList[mList[matchCount]].two);

                   cUsed[mList[matchCount]] = FALSE;

                   tourn[myIndex].one = 0;
                   tourn[myIndex].two = 0;

                  } while (cList[mList[matchCount]  ].one !=
                           cList[mList[matchCount]+1].one);

                  startC = mList[matchCount] + 1;
                }
              }

             /* found a match that fits into the round, keep going until
              * all matches in the round have been scheduled
              */

              tourn[myIndex] = cList[c];
              totalChecks++;

              //if ((totalChecks % 1000) == 0) fprintf( stdout, "%d\033A\n", totalChecks );

              cUsed[c] = TRUE;
              mList[matchCount] = c;

              startC = 1;

              round_set |= (1 << cList[c].one);
              round_set |= (1 << cList[c].two);

              myIndex++;
              matchCount++;
        }

        /* found enough matches to fill a round, schedule the next round */
        roundCount++;
      }

      /* yahoo!, scheduled all the rounds */

      //fprintf( stdout, "          " );
      //ShowSchedule();

      /* try and make a schedule using two more teams */
      //teams += 2;
    //}
  }
} Scheduler;


/*int main()
{
  Scheduler scheduler;
  scheduler.schedule(6);
  scheduler.ShowSchedule();
  return 0;
}*/