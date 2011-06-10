#include "hcwr.h"
#include <iostream>

#define DEBUG(x) std::cout << "[DEBUG] " << x << endl
#define OUTPUT(x) std::cout << "---> " << x << endl
#define RAW_OUTPUT(x) std::cout << "---> " << x
#define RAW_OUTPUT_NO_ARROW(x) std::cout << x
#define RAW_OUTPUT_NL(x) std::cout << x << endl
#define OUTPUT_BEGIN(x) std::cout << "---> " << x << " ... "
#define OUTPUT_END std::cout << "done!" << endl

// Returns a string with leading/trailing characters of a set stripped
std::string trimmed( std::string const& str, char const* sepSet)
{
  std::string::size_type const first = str.find_first_not_of(sepSet);
  return ( first==std::string::npos ) ? std::string() : str.substr(first, str.find_last_not_of(sepSet)-first+1);
}

// -----------------------------------------------------------------------------------

HCWR::HCWR(string input, int maxIterations, int maxRestarts)
{
  this->maxIterations = maxIterations;
  this->maxRestarts = maxRestarts;
  
  // Open the input file
  ifstream inputFile (input.c_str());
  
  if ( ! inputFile.is_open())
  {
    exit(1);
  }
  
  // Parse it
  string line;
  string number;
  vector< vector<int> > distances; // Stores distances matrix

  while (getline(inputFile, line))
  {
    if (trimmed(line, " \t") == "")
    {
      continue;
    }
    
    stringstream linestream(line);
    vector<int> row;
    
    while (linestream >> number)
    {
      row.push_back(atoi(number.c_str()));
    }
    
    distances.push_back(row);
    row.clear();
  }
  
  // We can now initialize our RTTP instance
  this->rttp = new RTTP(distances.size(), (2 * (distances.size() - 1) + OFF_DAYS), MAX_CONSECUTIVE_OFF_DAYS, MAX_CONSECUTIVE_GAMES);
  
  // Set costs
  for (size_t i = 0; i < distances.size(); i++)
  {
    for (size_t j = 0; j < distances.size(); j++)
    {
      int test = distances[i][j];
      this->rttp->setIndividualCost(i, j, test);
    }
    
    distances[i].clear();
  }
  
  // Done
  distances.clear();
  
  this->continue_iterating = true;
}

// -----------------------------------------------------------------------------------

HCWR::~HCWR()
{
  delete this->rttp;
}

// -----------------------------------------------------------------------------------

void HCWR::storeCurrentSolutionAsBest()
{
  this->G = this->rttp->G;
  this->O = this->rttp->O;
  this->V = this->rttp->V;
  this->cost = this->rttp->objectiveFunction();
  this->valid = this->rttp->validSolution();
  this->restrictionsNotMet = this->rttp->numberOfRestrictionsNotMet();
}

// -----------------------------------------------------------------------------------

void HCWR::start()
{
  OUTPUT("Starting");
  OUTPUT_BEGIN("Generating initial solution");
  //this->rttp->generateRandomSolution();
  this->rttp->generateInitialDoubleRoundRobinSolution();
  OUTPUT_END;
  
  OUTPUT_BEGIN("Storing initial solution as best");
  this->storeCurrentSolutionAsBest();
  OUTPUT_END;
  
  for (int i = 0; i < this->maxRestarts && this->continue_iterating; i++)
  {  
    RAW_OUTPUT("Starting iteration #");
    RAW_OUTPUT_NL(i);
    //std::cout << '\r';
    
    // New random solution
    //this->rttp->generateRandomSolution();
    this->rttp->generateInitialDoubleRoundRobinSolution();
    
    for (int j = 0; j < this->maxIterations; j++)
    {
      // Look for better solution
      //OUTPUT_BEGIN("Generating neighbour");
      this->rttp->generateBestNeighbour();
      //this->rttp->generateBestNeighbour();
      //OUTPUT_END;
      
      // Is it better?
      if (this->rttp->objectiveFunction() < this->cost)
      {
        // It is, store it.
        this->storeCurrentSolutionAsBest();
        RAW_OUTPUT("Found better solution with cost ");
        RAW_OUTPUT_NO_ARROW(this->cost);
        RAW_OUTPUT_NO_ARROW(" ["); RAW_OUTPUT_NO_ARROW(this->rttp->objectiveFunctionNotPenalized()); RAW_OUTPUT_NO_ARROW("]");
        RAW_OUTPUT_NO_ARROW(" (does not meet "); 
        RAW_OUTPUT_NO_ARROW(this->restrictionsNotMet); 
        RAW_OUTPUT_NL(" restrictions)");
      }
    }
    
    //RAW_OUTPUT("Iteration ended #");
    //RAW_OUTPUT_NL(i);
  }
  
  std::cout << endl;
  RAW_OUTPUT("Best solution cost: ");
  RAW_OUTPUT_NL(this->cost);
  RAW_OUTPUT("Restrictions not met: ");
  RAW_OUTPUT_NL(this->restrictionsNotMet);
  if (this->valid)
  {
    OUTPUT("Solution is valid!");
  }
  else
  {
    OUTPUT("Solution is NOT valid.");
    
    // This enables us to check restrictions not met
    this->rttp->G = this->G;
    this->rttp->O = this->O;
    this->rttp->V = this->V;
    
    if (this->rttp->noConsecutiveHomeGames()) 
    {
      OUTPUT("noConsecutiveHomeGames is met");
    }
    else
    {
      OUTPUT("noConsecutiveHomeGames is NOT met");
    }

    if (this->rttp->lengthOfHomeGames()) 
    {
      OUTPUT("lengthOfHomeGames is met");
    }
    else
    {
      OUTPUT("lengthOfHomeGames is NOT met");
    }

    if (this->rttp->lengthOfOffDays()) 
    {
      OUTPUT("lengthOfOffDays is met");
    }
    else
    {
      OUTPUT("lengthOfOffDays is NOT met");
    }

    if (this->rttp->lengthOfAwayGames()) 
    {
      OUTPUT("lengthOfAwayGames is met");
    }
    else
    {
      OUTPUT("lengthOfAwayGames is NOT met");
    }

    if (this->rttp->doubleRoundRobinTournament()) 
    {
      OUTPUT("doubleRoundRobinTournament is met");
    }
    else
    {
      OUTPUT("doubleRoundRobinTournament is NOT met");
    }

    if (this->rttp->stayAtHomeOnHomeGameDay()) 
    {
      OUTPUT("stayAtHomeOnHomeGameDay is met");
    }
    else
    {
      OUTPUT("stayAtHomeOnHomeGameDay is NOT met");
    }

    if (this->rttp->stayAtOpponentOnRoadGameDay()) 
    {
      OUTPUT("stayAtOpponentOnRoadGameDay is met");
    }
    else
    {
      OUTPUT("stayAtOpponentOnRoadGameDay is NOT met");
    }

    if (this->rttp->stayAtPreviousVenueOnOffDay()) 
    {
      OUTPUT("stayAtPreviousVenueOnOffDay is met");
    }
    else
    {
      OUTPUT("stayAtPreviousVenueOnOffDay is NOT met");
    }

  }
}