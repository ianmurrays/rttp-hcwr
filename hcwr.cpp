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
  this->input = input;
  
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
  this->rttp = new RTTP(distances.size(), (2 * (distances.size() - 1) + OFF_DAYS), MAX_CONSECUTIVE_OFF_DAYS, MAX_CONSECUTIVE_GAMES, MAX_CONSECUTIVE_AWAY_GAMES);
  
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
  clock_t start = clock(); // Store the time we begin the algorithm.
  
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
    //RAW_OUTPUT("Starting iteration #");
    //RAW_OUTPUT_NL(i);
    std::cout << '\r';
    RAW_OUTPUT("Iterating ... [");
    int percent = (int)(((float)i / (float)this->maxRestarts) * 100) + 1;
    int lines = percent / 5;
    for (int l = 0; l < lines; l++) RAW_OUTPUT_NO_ARROW("|");
    for (int l = 0; l < (20 - lines); l++) RAW_OUTPUT_NO_ARROW("-");
    RAW_OUTPUT_NO_ARROW("] ");
    RAW_OUTPUT_NO_ARROW(percent);
    RAW_OUTPUT_NO_ARROW("%");
    RAW_OUTPUT_NO_ARROW(" | Best so far [");
    RAW_OUTPUT_NO_ARROW(this->cost);
    RAW_OUTPUT_NO_ARROW("/");
    RAW_OUTPUT_NO_ARROW(this->restrictionsNotMet);
    RAW_OUTPUT_NO_ARROW("] | Running time [");
    RAW_OUTPUT_NO_ARROW((int)(((double)clock() - start) / CLOCKS_PER_SEC));
    RAW_OUTPUT_NO_ARROW(" secs]");
    
    std::cout.flush();
    
    // Say progress if on mac
    #ifdef __APPLE_CC__
    static bool said = false;
    static int last_cost = -1;
    if (percent % 5 == 0 && ! said)
    {
      char command[1024];
      if (last_cost > this->cost)
      {
        sprintf(command, "say \"Best so far: %d and %d restrictions not met. Running time is %d seconds.\" &", this->cost, this->restrictionsNotMet, (int)(((double)clock() - start) / CLOCKS_PER_SEC));
      }
      else
      {
        sprintf(command, "say \"No improvements. Running time is %d seconds.\" &", (int)(((double)clock() - start) / CLOCKS_PER_SEC));
      }
      
      system(command);
      said = true;
      last_cost = this->cost;
    }
    else if (percent % 5 != 0)
    {
      said = false;
    }
    #endif
    
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
      if (this->rttp->objectiveFunction() < this->cost && this->rttp->nonRelaxedRestrictions()) // The latter to avoid accepting randomized invalid solutions as better
      {
        // It is, store it.
        this->storeCurrentSolutionAsBest();
        /*RAW_OUTPUT_NO_ARROW(" (found better: ");
        RAW_OUTPUT_NO_ARROW(this->cost);
        RAW_OUTPUT_NO_ARROW(" ["); RAW_OUTPUT_NO_ARROW(this->rttp->objectiveFunctionNotPenalized()); RAW_OUTPUT_NO_ARROW("]");
        RAW_OUTPUT_NO_ARROW(" doesn't meet "); 
        RAW_OUTPUT_NO_ARROW(this->rttp->numberOfRestrictionsNotMet()); 
        RAW_OUTPUT_NL(" restrictions)");*/
      }
      else if(this->rttp->objectiveFunction() >= this->cost && BREAK_ON_NO_BETTER_SOLUTION)
      {
        // We already have the best point. stop iterating.
        break;
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
  
  
  // This enables us to check restrictions not met and save the current solution to a file
  this->rttp->G = this->G;
  this->rttp->O = this->O;
  this->rttp->V = this->V;
  
  OUTPUT_BEGIN("Writing solution to " + this->input + ".sol");
  this->rttp->saveCurrentSolutionToFile(this->input + ".sol");
  OUTPUT_END;
  
  
  if (this->valid)
  {
    OUTPUT("Solution is valid!");
  }
  else
  {
    OUTPUT("Solution is NOT valid.");
    
    if (this->rttp->noConsecutiveHomeGames()) 
    {
      OUTPUT("noConsecutiveHomeGames is met");
    }
    else
    {
      OUTPUT("noConsecutiveHomeGames is NOT met");
    }

    if (this->rttp->lengthOfGames()) 
    {
      OUTPUT("lengthOfGames is met");
    }
    else
    {
      OUTPUT("lengthOfGames is NOT met");
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
    
    if (this->rttp->roundConsistency()) 
    {
      OUTPUT("roundConsistency is met");
    }
    else
    {
      OUTPUT("roundConsistency is NOT met");
    }
    
    if (this->rttp->freeGamesConsistency()) 
    {
      OUTPUT("freeGamesConsistency is met");
    }
    else
    {
      OUTPUT("freeGamesConsistency is NOT met");
    }
    
    if (this->rttp->teamShouldNotPlayItself()) 
    {
      OUTPUT("teamShouldNotPlayItself is met");
    }
    else
    {
      OUTPUT("teamShouldNotPlayItself is NOT met");
    }
  }
}