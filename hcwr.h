#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "rttp.h"

#define OFF_DAYS 1
#define MAX_CONSECUTIVE_OFF_DAYS 1
#define MAX_CONSECUTIVE_GAMES 4

using namespace std;

class HCWR
{
  private:
    /**
     * Defines how many iterations to perform since a restart
     */
    int maxIterations;
    
    /**
     * How many restarts to make
     */
    int maxRestarts;
    
    /**
     * Holds best solution so far
     */
    vector< vector<int> > G, O, V;
    
    /**
     * Holds RTTP instance
     */
    RTTP * rttp;
  
  public:
    HCWR(string input, int maxIterations, int maxRestarts);
    ~HCWR();
    
    void start();
};