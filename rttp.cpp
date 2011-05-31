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