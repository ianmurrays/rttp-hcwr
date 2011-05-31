#include "rttp.h"

RTTP::RTTP(int numberOfTeams, int numberOfDays, int maxConsecutiveOffDays, int maxConsecutiveGames)
{
  this->numberOfTeams = numberOfTeams;
  this->numberOfDays = numberOfDays;
  this->maxConsecutiveOffDays = maxConsecutiveOffDays;
  this->maxConsecutiveGames = maxConsecutiveGames;
  
  // Initialize all problem variables
  for (size_t i = 0; i < this->numberOfTeams; i++)
  {
    vector<int> G_temp,
                O_temp,
                V_temp, 
                C_temp;
    
    for (size_t j = 0; j < this->numberOfDays; j++)
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
  }
}

RTTP::~RTTP()
{
  // TODO: Free up memory?
}

int RTTP::getMaxConsecutiveOffDays()
{
  return this->maxConsecutiveOffDays;
}

int RTTP::getNumberOfTeams()
{
  return this->numberOfTeams;
}

int RTTP::getNumberOfDays()
{
  return this->numberOfDays;
}

int RTTP::getMaxConsecutiveGames()
{
  return this->maxConsecutiveGames;
}

bool RTTP::noConsecutiveHomeGames()
{ 
  for (size_t i = 0; i < this->numberOfTeams; i++) // for each team
  {
    for (size_t d = 0; d < (this->numberOfDays - 1); d++) // for each day - 1
    {
      int sumOfG = 0;
      
      for (size_t k = 0; k <= 1; k++)
      {
        if (this->G[i][d + k] == G_HOMEGAME)
        {
          sumOfG++;
        }
      }
      
      if (sumOfG > 1)
      {
        return false;
      }
    }
  }
  
  return true; // All sums of G turned out to be less or equal to 1
}

bool RTTP::lengthOfHomeGames()
{
  for (size_t i = 0; i < this->numberOfTeams; i++) // for each team
  {
    for (size_t d = 0; d < (this->numberOfDays - this->maxConsecutiveGames); d++) // for each day - max_cons_games
    {
      int sumOfG = 0;
      
      for (size_t k = 0; k <= this->maxConsecutiveGames; k++)
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