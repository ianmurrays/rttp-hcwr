#include "../rttp.h"

#define PENDING(x) printf("[PENDING] %s\n", x)

SUITE(RTTP)
{
  TEST(ConstructorShouldAssignAllValues)
  {
    int numberOfTeams = 1,
        numberOfDays = 2,
        maxConsecutiveOffDays = 3,
        maxConsecutiveGames = 4;
    
    RTTP * rttp = new RTTP(numberOfTeams, numberOfDays, maxConsecutiveOffDays, maxConsecutiveGames);
    
    CHECK_EQUAL(numberOfTeams, rttp->getNumberOfTeams());
    CHECK_EQUAL(numberOfDays, rttp->getNumberOfDays());
    CHECK_EQUAL(maxConsecutiveGames, rttp->getMaxConsecutiveGames());
    CHECK_EQUAL(maxConsecutiveOffDays, rttp->getMaxConsecutiveOffDays());
    
    delete rttp;
  }
  
  TEST(ConstructorShouldInitializeAllVariables)
  {
    int numberOfTeams = 1,
        numberOfDays = 2;
    
    RTTP * rttp = new RTTP(numberOfTeams, numberOfDays, 1, 1);

    CHECK_EQUAL(numberOfTeams, rttp->G.size());
    CHECK_EQUAL(numberOfDays, rttp->G[0].size()); // Check the first one only.

    CHECK_EQUAL(numberOfTeams, rttp->O.size());
    CHECK_EQUAL(numberOfDays, rttp->O[0].size()); // Check the first one only.

    CHECK_EQUAL(numberOfTeams, rttp->V.size());
    CHECK_EQUAL(numberOfDays, rttp->V[0].size()); // Check the first one only.

    CHECK_EQUAL(numberOfTeams, rttp->C.size());
    CHECK_EQUAL(numberOfDays, rttp->C[0].size()); // Check the first one only.
    
    delete rttp;
  }
  
  TEST(NoConsecutiveHomeGames)
  {
    RTTP * rttp = new RTTP(2, 4, 1, 1);
    
    CHECK( ! rttp->noConsecutiveHomeGames()); // This should be invalid at the beginning.

    // Make them all valid
    for (size_t i = 0; i < rttp->getNumberOfTeams(); i++)
    {
      for (size_t d = 0; d < rttp->getNumberOfDays(); d++)
      {
        rttp->G[i][d] = 1;
      }
    }
    
    CHECK(rttp->noConsecutiveHomeGames());
    
    // Now set one combination to be invalid
    rttp->G[1][1] = 0;
    rttp->G[1][2] = 0;
    CHECK( ! rttp->noConsecutiveHomeGames());
    
    delete rttp;
  }
  
  TEST(LengthOfHomeGames)
  {
    RTTP * rttp = new RTTP(2, 4, 1, 1);
    CHECK( ! rttp->lengthOfHomeGames()); // This should be false at the beginning.
    
    // Make them all valid
    for (size_t i = 0; i < rttp->getNumberOfTeams(); i++)
    {
      for (size_t d = 0; d < rttp->getNumberOfDays(); d++)
      {
        rttp->G[i][d] = 2;
      }
    }
    
    CHECK(rttp->lengthOfHomeGames()); // This should be true now
    
    // Make an invalid combination
    rttp->G[0][1] = rttp->G[0][2] = 1;
    CHECK( ! rttp->lengthOfHomeGames());
    
    delete rttp;
  }
  
  TEST(LengthOfOffDays)
  {
    RTTP * rttp = new RTTP(2, 4, 1, 1);
    CHECK( ! rttp->lengthOfOffDays()); // Should be false upon instantiation.
    
    // Make them all valid
    for (size_t i = 0; i < rttp->getNumberOfTeams(); i++)
    {
      for (size_t d = 0; d < rttp->getNumberOfDays(); d++)
      {
        rttp->G[i][d] = G_OFFDAY;
      }
    }
    
    CHECK(rttp->lengthOfOffDays()); 
    
    // Invalid combination, ie. a team that has no off days
    for (size_t d = 0; d < rttp->getNumberOfDays(); d++)
    {
      rttp->G[0][d] = G_HOMEGAME;
    }
    
    CHECK( ! rttp->lengthOfOffDays());
    
    delete rttp;
  }
  
  TEST(LengthOfAwayGames)
  {
    PENDING("LengthOfAwayGames");
  }
  
  TEST(DoubleRoundRobinTournament)
  {
    RTTP * rttp = new RTTP(2, 2, 1, 1);
    CHECK( ! rttp->doubleRoundRobinTournament());
    
    // Each team should play the other at home and away
    rttp->G[0][0] = rttp->G[1][1] = G_HOMEGAME;
    rttp->G[1][0] = rttp->G[0][1] = G_ROADGAME;
    
    rttp->O[0][0] = rttp->O[0][1] = 1;
    rttp->O[1][0] = rttp->O[1][1] = 0;
    
    CHECK(rttp->doubleRoundRobinTournament());
    
    // Make an invalid combination
    rttp->O[0][0] = 0; // Plays against himself 
    
    CHECK( ! rttp->doubleRoundRobinTournament());
    
    delete rttp;
  }
}
