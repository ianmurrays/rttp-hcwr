#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <csignal>
#include <iostream>
#include "rttp.h"

using namespace std;

#ifndef _HCWR_HEADER
#define _HCWR_HEADER

#define OFF_DAYS 1
#define MAX_CONSECUTIVE_OFF_DAYS 1
#define MAX_CONSECUTIVE_GAMES 5

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
     * Input file
     */
    string input;
    
    /**
     * Holds best solution so far
     */
    vector< vector<int> > G, O, V;
    
    /**
     * Other info about the current best solution
     */
    bool valid;
    int cost;
    int restrictionsNotMet;
    
    /**
     * Holds RTTP instance
     */
    RTTP * rttp;
  
  public:
    HCWR(string input, int maxIterations, int maxRestarts);
    ~HCWR();
    
    bool continue_iterating; // This is changed to false when SIGINT is captured
    
    void start();
    void storeCurrentSolutionAsBest();
};
#endif