#include "hcwr.h"
#include <iostream>

#define DEBUG(x) std::cout << "[DEBUG] " << x << endl;

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
}

HCWR::~HCWR()
{
  delete this->rttp;
}