#include "../rttp.h"

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
  }
}
