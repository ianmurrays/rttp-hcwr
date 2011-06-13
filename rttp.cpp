#include "rttp.h"
#include "schedule.cpp" // Includes a round-robin scheduler
#include <iostream>

RTTP::RTTP(int numberOfTeams, int numberOfDays, int maxConsecutiveOffDays, int maxConsecutiveGames, int maxConsecutiveAwayGames)
{
  this->numberOfTeams = numberOfTeams;
  this->numberOfDays = numberOfDays;
  this->maxConsecutiveOffDays = maxConsecutiveOffDays;
  this->maxConsecutiveGames = maxConsecutiveGames;
  this->maxConsecutiveAwayGames = maxConsecutiveAwayGames;
  
  if (this->maxConsecutiveGames > this->numberOfDays)
  {
    std::cout << "FATAL: maxConsecutiveGames cannot be bigger than numberOfDays" << endl;
    exit(1);
  }
  
  if (this->maxConsecutiveAwayGames > this->numberOfDays)
  {
    std::cout << "FATAL: maxConsecutiveAwayGames cannot be bigger than numberOfDays" << endl;
    exit(1);
  }
  
  // Initialize all problem variables
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    vector<int> G_temp,
                O_temp,
                V_temp, 
                C_temp;
    
    for (size_t j = 0; j < (size_t)this->numberOfDays; j++)
    {
      G_temp.push_back(0);
      O_temp.push_back(0);
      V_temp.push_back(0);
      C_temp.push_back(0);
    }
    
    this->G.push_back(G_temp);
    this->O.push_back(O_temp);
    this->V.push_back(V_temp);
    this->C.push_back(C_temp);
    
    // Seed the random number generator
    srand((unsigned)time(0));
  }
  
  // Initialize travelCosts
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    vector<int> tC_temp;
    
    for (size_t j = 0; j < (size_t)this->numberOfTeams; j++)
    {
      tC_temp.push_back(0);
    }
    
    this->travelCosts.push_back(tC_temp);
  }
}

// -----------------------------------------------------------------------------------

RTTP::~RTTP()
{
  // Free all four variables
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    this->G[i].clear();
    this->O[i].clear();
    this->V[i].clear();
    this->C[i].clear();
    
    this->travelCosts[i].clear();
  }
  
  this->G.clear();
  this->O.clear();
  this->V.clear();
  this->C.clear();
  this->travelCosts.clear();
}

// -----------------------------------------------------------------------------------

int RTTP::getMaxConsecutiveOffDays()
{
  return this->maxConsecutiveOffDays;
}

// -----------------------------------------------------------------------------------

int RTTP::getNumberOfTeams()
{
  return this->numberOfTeams;
}

// -----------------------------------------------------------------------------------

int RTTP::getNumberOfDays()
{
  return this->numberOfDays;
}

// -----------------------------------------------------------------------------------

int RTTP::getMaxConsecutiveGames()
{
  return this->maxConsecutiveGames;
}

// -----------------------------------------------------------------------------------

bool RTTP::noConsecutiveHomeGames()
{ 
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++) // for each team
  {
    for (size_t d = 0; d < (size_t)(this->numberOfDays - 1); d++) // for each day - 1
    {
      int sumOfG = 0;
      
      for (size_t k = 0; k <= 1; k++)
      {
        if (this->G[i][d + k] == G_HOMEGAME)
        {
          sumOfG++;
        }
      }
      
      if (sumOfG > NO_CONSECUTIVE_HOMEGAMES)
      {
        return false;
      }
    }
  }
  
  return true; // All sums of G turned out to be less than or equal to 1
}

// -----------------------------------------------------------------------------------

bool RTTP::lengthOfGames() // This restriction's name is wrong, should be lengthOfGames
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++) // for each team
  {
    for (size_t d = 0; d < (size_t)(this->numberOfDays - this->maxConsecutiveGames); d++) // for each day - max_cons_games
    {
      int sumOfG = 0;
      
      for (size_t k = 0; k <= (size_t)this->maxConsecutiveGames; k++)
      {
        if (this->G[i][d + k] != G_OFFDAY) // ie. if team is playing, wherever ...
        {
          sumOfG++;
        }
      }
      
      if (sumOfG > this->maxConsecutiveGames)
      {
        return false;
      }
    }
  }
  
  return true;
}

// -----------------------------------------------------------------------------------

bool RTTP::lengthOfOffDays()
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++) // for each team
  {
    for (size_t d = 0; d < (size_t)(this->numberOfDays - this->maxConsecutiveOffDays); d++) // for each day - max_cons_off_days
    {
      int sumOfG = 0;
      
      for (size_t k = 0; k <= (size_t)this->maxConsecutiveOffDays; k++)
      {
        if (this->G[i][d + k] == G_OFFDAY) // ie. if team is not playing
        {
          sumOfG++;
        }
      }
      
      if (sumOfG > this->maxConsecutiveOffDays) // FIXME: I'm not sure this is correct.
      {
        return false;
      }
    }
  }
  
  return true;
}

// -----------------------------------------------------------------------------------

bool RTTP::lengthOfAwayGames()
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    for (size_t k = 0; k < (size_t)this->numberOfDays; k++)
    {
      for (size_t d = 0; d < ((size_t)this->numberOfDays - k); d++)
      {
        int localGames = 0;
        
        for (size_t m = 0; m <= k; m++)
        {
          if (this->G[i][d + m] == G_HOMEGAME)
          {
            localGames++;
          }
        }
        
        if (localGames == 0)
        {
          int roadGames = 0;
          for (size_t m = 0; m <= k; m++)
          {
            if (this->G[i][d + m] == G_ROADGAME)
            {
              roadGames++;
            }
          }
          
          std::cout << "roadGames = " << roadGames << "; maxConsecutiveAwayGames = " << maxConsecutiveAwayGames << endl;
          if (roadGames > this->maxConsecutiveAwayGames) // FIXME: I don't know if this is ok
          {
            return false;
          }
        }
      }
    }
  }
  std::cout << "true!" << endl;
  return true;
}

// -----------------------------------------------------------------------------------

bool RTTP::doubleRoundRobinTournament()
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++) // forall teams i
  {
    for (size_t j = 0; j < (size_t)this->numberOfTeams; j++) // forall teams j
    {
      if (i != j)
      {
        int sumOfGO_1 = 0, // 4.29
            sumOfGO_2 = 0; // 4.30
            
        for (size_t d = 0; d < (size_t)this->numberOfDays; d++) // sum over all days
        {
          // Restriction 4.29
          if (this->G[i][d] == G_ROADGAME && this->O[i][d] == (int)j)
          {
            sumOfGO_1++;
          }
          
          // Restriction 4.30
          if (this->G[i][d] == G_HOMEGAME && this->O[i][d] == (int)j)
          {
            sumOfGO_2++;
          }
        }
        
        if (sumOfGO_1 != 1 || sumOfGO_2 != 1)
        {
          return false;
        }
      }
    }
  }
  
  return true;
}

// -----------------------------------------------------------------------------------

bool RTTP::stayAtHomeOnHomeGameDay()
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++) // forall teams i
  {
    for (size_t d = 0; d < (size_t)this->numberOfDays; d++) // forall days d
    {
      if (this->G[i][d] == G_HOMEGAME && this->V[i][d] != (int)i)
      {
        return false;
      }
    }
  }
  
  return true;
}

// -----------------------------------------------------------------------------------

bool RTTP::stayAtOpponentOnRoadGameDay()
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++) // forall teams i
  {
    for (size_t d = 0; d < (size_t)this->numberOfDays; d++) // forall days d
    {
      if (this->G[i][d] == G_ROADGAME && this->V[i][d] != this->O[i][d])
      {
        return false;
      }
    }
  }

  return true;
}

// -----------------------------------------------------------------------------------

bool RTTP::stayAtPreviousVenueOnOffDay()
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++) // forall teams i
  {
    for (size_t d = 1; d < (size_t)this->numberOfDays; d++) // forall days d (had to start from d = 1 to avoid crashing)
    {
      if (this->G[i][d] == G_OFFDAY && this->V[i][d] != this->V[i][d - 1])
      {
        return false;
      }
    }
  }

  return true;
}

// -----------------------------------------------------------------------------------

/** 
 * Makes sure that, if team A plays against B on a round, team B plays against team A
 * on the same round.
 */
bool RTTP::roundConsistency()
{
  /*for (size_t d = 0; d < (size_t)this->numberOfDays; d++)
  {
    for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
    {
      for (size_t i2 = 0; i2 < (size_t)this->numberOfTeams; i2++)
      {
        if (i != i2 && this->O[i][d] == this->O[i2][d])
        {
          return false;
        }
      }
    }
  }*/
  
  for (size_t d = 0; d < (size_t)this->numberOfDays; d++)
  {
    for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
    {
      if ((int)this->O[i][d] != O_NOOPONENT && this->O[this->O[i][d]][d] != (int)i)
      {
        return false;
      }
    }
  }
  
  return true;
}

// -----------------------------------------------------------------------------------

/**
 * Makes sure the number of free games per round is even.
 */
bool RTTP::freeGamesConsistency()
{
  for (size_t d = 0; d < (size_t)this->numberOfDays; d++)
  {
    int freeGames = 0;
    for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
    {
      if (this->G[i][d] == G_OFFDAY)
      {
        freeGames++;
      }
    }
    
    if (freeGames % 2 != 0)
    {
      return false;
    }
  }
  
  return true;
}

// -----------------------------------------------------------------------------------

bool RTTP::teamShouldNotPlayItself()
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    for (size_t d = 0; d < (size_t)this->numberOfDays; d++)
    {
      if (this->O[i][d] == (int)i)
      {
        return false;
      }
    }
  }
  
  return true;
}

// -----------------------------------------------------------------------------------

bool RTTP::validSolution()
{
  return this->noConsecutiveHomeGames() && 
         this->lengthOfGames() &&
         this->lengthOfOffDays() && 
         this->lengthOfAwayGames() && 
         this->doubleRoundRobinTournament() &&
         this->stayAtHomeOnHomeGameDay() && 
         this->stayAtOpponentOnRoadGameDay() && 
         this->stayAtPreviousVenueOnOffDay() &&
         this->roundConsistency() &&
         this->freeGamesConsistency() &&
         this->teamShouldNotPlayItself();
}

// -----------------------------------------------------------------------------------

RTTP * RTTP::setIndividualCost(size_t teamA, size_t teamB, int cost)
{
  if (teamA == teamB)
  {
    cost = 0; // Cost should be zero to travel from A to A.
  }
  
  this->travelCosts[teamA][teamB] = this->travelCosts[teamB][teamA] = cost;
  
  return this; // Concatenation :D
}

// -----------------------------------------------------------------------------------

bool RTTP::nonRelaxedRestrictions()
{
  //return true;
  //return this->doubleRoundRobinTournament();
  return this->noConsecutiveHomeGames() && 
         this->lengthOfGames() &&
         this->lengthOfOffDays() && 
         this->lengthOfAwayGames() && 
         this->doubleRoundRobinTournament() &&
         this->stayAtHomeOnHomeGameDay() && 
         this->stayAtOpponentOnRoadGameDay() && 
         this->stayAtPreviousVenueOnOffDay() &&
         this->roundConsistency() &&
         this->freeGamesConsistency() &&
         this->teamShouldNotPlayItself();
}

// -----------------------------------------------------------------------------------

int RTTP::objectiveFunction()
{
  return this->objectiveFunctionNotPenalized() + this->numberOfRestrictionsNotMet() * PENALIZE_COST;
}

// -----------------------------------------------------------------------------------

int RTTP::objectiveFunctionNotPenalized() 
{
  int cost = 0;
  
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    int teamCost = 0;
    for (size_t d = 1; d < (size_t)this->numberOfDays; d++)
    {
      //cout << "i = " << i << endl;
      //cout << "d = " << d << endl;
      //cout << "V = " << this->V[i][d] << endl;
      teamCost += this->travelCosts[this->V[i][d - 1]][this->V[i][d]];
    }
    
    cost += teamCost;
  }
  
  return cost;
}

// -----------------------------------------------------------------------------------

void RTTP::generateNeighbour()
{
  // Set the oponent of a random team on a random date to a random team, 
  // and fix other variables accordingly.
  size_t i = (size_t)(rand() % this->numberOfTeams);
  size_t d = (size_t)(rand() % this->numberOfDays);
  size_t new_i = (size_t)(rand() % (this->numberOfTeams + 1) - 1);
  
  this->O[i][d] = new_i;
  
  // Set Game variable as well
  if (this->O[i][d] == O_NOOPONENT)
  {
    this->G[i][d] = G_OFFDAY;
  }
  else
  {
    this->G[i][d] = (rand() % 2); // G_HOMEGAME or G_ROADGAME
  }
  
  this->fixVariables(); // This fixes the Venues variable.
}

// -----------------------------------------------------------------------------------

void RTTP::generateBestNeighbour()
{
  vector< vector<int> > Gor = this->G, 
                        Oor = this->O, 
                        Vor = this->V,
                        G, O, V;
  
  bool foundBetter = false;
  int cost = this->objectiveFunction();
  
  // Try to find neighbour
  for (int end = 0; end < MAX_ITERATIONS_FOR_NEIGHBOUR; end++)
  {
    // Only do one swap!
    this->swapWeek();
    this->swapGameType();
    this->swapRandom();
    
    if ( true )
    {
      if ((this->objectiveFunction() < cost)/* && this->nonRelaxedRestrictions()*/)
      {
        foundBetter = true;
        G = this->G;
        O = this->O;
        V = this->V;
        cost = this->objectiveFunction();
      }
    }
  }
  
  if ( ! foundBetter)
  {
    this->G = Gor;
    this->O = Oor;
    this->V = Vor;
  }
  else
  {
    this->G = G;
    this->O = O;
    this->V = V;
  }
}

bool RTTP::swapGameType()
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    for (size_t d = 0; d < (size_t)this->numberOfDays; d++)
    {
      if (rand()%100 < SWAP_GAMETYPE_THRESHOLD && this->G[i][d] != G_OFFDAY && this->O[i][d] != O_NOOPONENT)
      {
        int temp = this->G[i][d];
        this->G[i][d] = this->G[this->O[i][d]][d];
        this->G[this->O[i][d]][d] = temp;
        
        return true;
      }
      
      this->fixVariables(); // This fixes the Venues variable.
    }
  }
  
  return false;
}

bool RTTP::swapRandom()
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    for (size_t d = 0; d < (size_t)this->numberOfDays; d++)
    {
      if (rand()%100 < RANDOM_SWAP_THRESHOLD)
      {
        //*
        size_t new_i = (size_t)(rand() % (this->numberOfTeams + 1) - 1);
        this->O[i][d] = new_i;
        //*/
        //this->O[i][d] = (this->O[i][d] + 1 == this->numberOfTeams) ? -1 : (this->O[i][d] + 1);

        // Set Game variable as well
        if (this->O[i][d] == O_NOOPONENT)
        {
          this->G[i][d] = G_OFFDAY;
        }
        else
        {
          this->G[i][d] = (rand() % 2); // G_HOMEGAME or G_ROADGAME

          // Force some doubleRoundRobin magic
          //*
          size_t new_d = (size_t)(rand() % this->numberOfDays);
          this->O[i][new_d] = this->O[i][d]; // He should play the same opponent twice
          this->G[i][new_d] = this->G[i][d] == G_HOMEGAME ? G_ROADGAME : G_HOMEGAME;
          //*/
        }
        this->fixVariables(); // This fixes the Venues variable.
        return true;
      }
    }
  }
  
  return false;
}

bool RTTP::swapWeek()
{
  // Swap one week for another
  if (rand()%100 < SWAP_WEEK_THRESHOLD)
  {
    int i = rand() % this->numberOfTeams;
    int i2 = rand() % this->numberOfTeams;
    int d = rand() % this->numberOfDays;
    int d2 = rand() % this->numberOfDays;
    
    //for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
    //{
      int temp = this->O[i][d];
      this->O[i][d] = this->O[i][d2];
      this->O[i][d2] = temp;
      temp = this->O[i2][d];
      this->O[i2][d] = this->O[i2][d2];
      this->O[i2][d2] = temp;
      
      temp = this->G[i][d];
      this->G[i][d] = this->G[i][d2];
      this->G[i][d2] = temp;
      temp = this->G[i2][d];
      this->G[i2][d] = this->G[i2][d2];
      this->G[i2][d2] = temp;
    //}
    
    this->fixVariables(); // This fixes the Venues variable.
    return true;
    
    /*if ((this->objectiveFunction() < cost) && this->nonRelaxedRestrictions())
    {
      foundBetter = true;
      G = this->G;
      O = this->O;
      V = this->V;
      cost = this->objectiveFunction();
    }*/
  }
  
  return false;
}

// -----------------------------------------------------------------------------------

void RTTP::fixVariables()
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    for (size_t d = 0; d < (size_t)this->numberOfDays; d++)
    {
      switch (this->G[i][d])
      {  
        case G_HOMEGAME:
        this->V[i][d] = i;
        break;
        
        case G_ROADGAME:
        this->V[i][d] = (this->O[i][d] == O_NOOPONENT ? i : this->O[i][d]); // This is a very dirty fix for a bug I haven't found. Sometimes for a G_ROADGAME, the oponent is O_NOOPONENT =S
        break;
        
        case G_OFFDAY:
        if (d > 0) 
        {
          this->V[i][d] = this->V[i][d - 1];
        }
        else
        {
          this->V[i][d] = i;
        }
        break;
      }
    }
  }
}

// -----------------------------------------------------------------------------------

int RTTP::numberOfRestrictionsNotMet()
{
  int notMet = 0;
  notMet += (this->noConsecutiveHomeGames() ? 0 : 1);
  notMet += (this->lengthOfGames() ? 0 : 1);
  notMet += (this->lengthOfOffDays() ? 0 : 1);
  notMet += (this->lengthOfAwayGames() ? 0 : 1);
  notMet += (this->doubleRoundRobinTournament() ? 0 : 1);
  notMet += (this->stayAtHomeOnHomeGameDay() ? 0 : 1);
  notMet += (this->stayAtOpponentOnRoadGameDay() ? 0 : 1);
  notMet += (this->stayAtPreviousVenueOnOffDay() ? 0 : 1);
  notMet += (this->roundConsistency() ? 0 : 1);
  notMet += (this->freeGamesConsistency() ? 0 : 1);
  notMet += (this->teamShouldNotPlayItself() ? 0 : 1);
  
  return notMet;
}

// -----------------------------------------------------------------------------------

void RTTP::generateRandomSolution()
{
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    for (size_t d = 0; d < (size_t)this->numberOfDays; d++)
    {
      size_t new_i = (size_t)(rand() % (this->numberOfTeams + 1) - 1);

      this->O[i][d] = new_i;

      // Set Game variable as well
      if (this->O[i][d] == O_NOOPONENT)
      {
        this->G[i][d] = G_OFFDAY;
      }
      else
      {
        this->G[i][d] = (rand() % 2); // G_HOMEGAME or G_ROADGAME
      }
    }
  }
  
  this->fixVariables();
}

// -----------------------------------------------------------------------------------

void RTTP::generateInitialDoubleRoundRobinSolution()
{
  Scheduler sched; // Will be used to schedule a RR tournament
  sched.schedule(this->numberOfTeams);
  
  // Let's use that schedule to create an initial (not necesarilly valid)
  // solution.
  int column = 1;
  for (int r = 1; r <= this->numberOfTeams - 1; r++) 
  {
    for (int m = 1; m <= this->numberOfTeams / 2; m++) 
    {
      int team_1 = ((rand() % 2) ? sched.tourn[column].one : sched.tourn[column].two) - 1,
          team_2 = ((team_1 == sched.tourn[column].two - 1) ? sched.tourn[column].one : sched.tourn[column].two) - 1;
      
      int week = r - 1;
      
      this->O[team_1][week] = team_2;
      this->O[team_2][week] = team_1;
      int game_type = rand() % 2;
      this->G[team_1][week] = game_type;
      this->G[team_2][week] = (game_type == G_ROADGAME ? G_HOMEGAME : G_ROADGAME);
      //this->V[team_1][week] = this->V[team_2][week] = team_1;
      
      // Double round robin
      int new_week = week + (this->numberOfTeams - 1);
      this->O[team_1][new_week] = team_2;
      this->O[team_2][new_week] = team_1;
      this->G[team_1][new_week] = (game_type == G_ROADGAME ? G_HOMEGAME : G_ROADGAME);
      this->G[team_2][new_week] = game_type;
      //this->V[team_1][new_week] = this->V[team_2][new_week] = team_1;
      
      column++;
    }
  }
  
  // Add enough days to cover the off days configured
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    for (size_t d = (2 * (this->numberOfTeams - 1)); d < (size_t)this->numberOfDays; d++)
    {
      this->O[i][d] = O_NOOPONENT;
      this->G[i][d] = G_OFFDAY;
    }
  }
  
  this->fixVariables(); // Fixes venues.
}

// -----------------------------------------------------------------------------------

void RTTP::saveCurrentSolutionToFile(string fileName)
{
  ofstream output;
  output.open(fileName.c_str(), ios::out | ios::app);
  
  // Write the header
  output << "  ";
  for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
  {
    output << i << "   ";
  }
  
  output << endl;
  
  // Iterate dates.
  for (size_t d = 0; d < (size_t)this->numberOfDays; d++)
  {
    for (size_t i = 0; i < (size_t)this->numberOfTeams; i++)
    {
      // ROADGAME or HOMEGAME?
      if (this->G[i][d] == G_ROADGAME)
      {
        output << " @";
      }
      else
      {
        output << "  ";
      }
      
      if (this->O[i][d] == O_NOOPONENT)
      {
        output << "- ";
      }
      else
      {
        output << this->O[i][d] << " ";
      }
    }
    
    output << endl;
  }
  output << endl << "Cost: " << this->objectiveFunctionNotPenalized() << " [" << (this->validSolution() ? "VALID" : "INVALID") << "]" << endl << endl;
  output.close();
}