#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

#ifndef _RTTP_HEADER
#define _RTTP_HEADER

#define G_HOMEGAME 0
#define G_ROADGAME 1
#define G_OFFDAY   2

#define O_NOOPONENT -1

#define PENALIZE_COST 10000

#define MAX_ITERATIONS_FOR_NEIGHBOUR 20000
#define NO_CONSECUTIVE_HOMEGAMES 3

/*
// Finds Solutions sometimes
// 90 / 20 / 60
// 95 / 70 / 40
#define SWAP_GAMETYPE_THRESHOLD 90
#define SWAP_WEEK_THRESHOLD 20
#define RANDOM_SWAP_THRESHOLD 60
//*/
//*
// Testing area :)
#define SWAP_WEEK_THRESHOLD 60
#define SWAP_WHOLE_WEEK_THRESHOLD 90
#define SWAP_GAMETYPE_THRESHOLD 40
#define RANDOM_SWAP_THRESHOLD 40
//*/

class RTTP
{
  #ifndef _RTTP_TEST // This allows us to test private methods and variables easily, maybe I shouldn't do this :(
  private:
  #else
  public:
  #endif
    /**
     * Defines the maximum days a team can have bias.
     */
    int maxConsecutiveOffDays;
    
    /**
     * Defines the maximum consecutive days a team can play.
     */
    int maxConsecutiveGames;
    int maxConsecutiveAwayGames;
    
    /**
     * Defines number of days to play
     */
    int numberOfDays;
    
    /**
     * Defines number of teams playing
     */
    int numberOfTeams;
    
    /**
     * i x d "matrix" where C_{id} is the cost of 
     * travel of team i in day d.
     */
    vector< vector<int> > C;
    
    /**
     * Holds travel costs 
     */
    vector< vector<int> > travelCosts;
  
  public:
    /**
     * i x d "matrix" that for each G_{id}, 
     * will be:
     *  0: if team i has a home game on day d
     *  1: if team i has a road game on day d
     *  2: if team i has an off day on day d
     *
     */
    vector< vector<int> > G;
    
    /**
     * i x d "matrix" that for each O_{id},
     * contains the oponent for team i on day d.
     * Domain is all teams.
     */
    vector< vector<int> > O;
    
    /**
     * i x d "matrix" that contains where team i
     * stays on day d.
     */
    vector< vector<int> > V;
    
    /**
     * Constructor. Initializes all problem variables as well.
     */
    RTTP(int numberOfTeams, int numberOfDays, int maxConsecutiveOffDays, int maxConsecutiveGames, int maxConsecutiveAwayGames);
    
    /**
     * Destructor
     */
    ~RTTP();
        
    // -----------------------------------------------------------------------------------
    
    /**
     * Tests all restrictions on the current solution candidate
     * to see if it's valid
     */
    bool validSolution();
    
    // -----------------------------------------------------------------------------------
    
    /**
     * Evaluates the candidate solution and returns its cost
     */
    int objectiveFunction();
    int objectiveFunctionNotPenalized();
    
    // -----------------------------------------------------------------------------------
    
    int numberOfRestrictionsNotMet();
    bool nonRelaxedRestrictions(); // Returns false if one or more non-relaxed restrictions are not met
    
    // -----------------------------------------------------------------------------------
    
    /**
     * Sets the cost matrix. Should be numberOfTeams x numberOfTeams wide
     */
    void setTravelCosts(vector< vector<int> > cost);
    RTTP * setIndividualCost(size_t teamA, size_t teamB, int cost); // This seems more useful
    
    // -----------------------------------------------------------------------------------
    
    int getMaxConsecutiveOffDays();
    int getMaxConsecutiveGames();
    int getNumberOfDays();
    int getNumberOfTeams();
    
    // -----------------------------------------------------------------------------------
    
    void generateNeighbour(); // First Improvement
    void generateBestNeighbour(); // Best Improvement
    void fixVariables();
    bool swapGameType();
    bool swapWeek();
    bool swapWholeWeek();
    bool swapRandom();
    
    // -----------------------------------------------------------------------------------
    
    void generateRandomSolution(); // Not necessarily valid
    void generateInitialDoubleRoundRobinSolution();
    
    // -----------------------------------------------------------------------------------
    
    /**
     * Constraint testing functions, according to Renjun Bao's original
     * paper for RTTP.
     */
    bool noConsecutiveHomeGames(); /* 4.25 */
    bool lengthOfGames(); /* 4.26 */
    bool lengthOfOffDays(); /* 4.27 */
    bool lengthOfAwayGames(); /* 4.28 */
    bool doubleRoundRobinTournament(); /* 4.29, 4.30 */
    bool stayAtHomeOnHomeGameDay(); /* 4.31 */
    bool stayAtOpponentOnRoadGameDay(); /* 4.32 */
    bool stayAtPreviousVenueOnOffDay(); /* 4.33 */
    bool roundConsistency(); /* Added by myself */
    bool freeGamesConsistency(); /* Added by myself */
    bool teamShouldNotPlayItself(); /* Added by myself */
    
    // -----------------------------------------------------------------------------------
    
    void saveCurrentSolutionToFile(string fileName);
};
#endif