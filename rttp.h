#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

#ifndef _RTTP_HEADER
#define _RTTP_HEADER

#define G_HOMEGAME 0
#define G_ROADGAME 1
#define G_OFFDAY   2

#define O_NOOPONENT -1

class RTTP
{
  #ifndef _RTTP_TEST // This allows us to test private methods and variables easily, maybe I shouldn't do this :(
  public:
  #else
  private:
  #endif
    /**
     * Defines the maximum days a team can have bias.
     */
    int maxConsecutiveOffDays;
    
    /**
     * Defines the maximum consecutive days a team can play.
     */
    int maxConsecutiveGames;
    
    /**
     * Defines number of days to play
     */
    int numberOfDays;
    
    /**
     * Defines number of teams playing
     */
    int numberOfTeams;
    
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
     * i x d "matrix" where C_{id} is the cost of 
     * travel of team i in day d.
     */
    vector< vector<int> > C;
    
    /**
     * Holds travel costs 
     */
    vector< vector<int> > travelCosts;
    
    // -----------------------------------------------------------------------------------
    
    /**
     * Constraint testing functions, according to Renjun Bao's original
     * paper for RTTP.
     */
    bool noConsecutiveHomeGames(); /* 4.25 */
    bool lengthOfHomeGames(); /* 4.26 */
    bool lengthOfOffDays(); /* 4.27 */
    bool lengthOfAwayGames(); /* 4.28 */
    bool doubleRoundRobinTournament(); /* 4.29, 4.30 */
    bool stayAtHomeOnHomeGameDay(); /* 4.31 */
    bool stayAtOpponentOnRoadGameDay(); /* 4.32 */
    bool stayAtPreviousVenueOnOffDay(); /* 4.33 */
  
  public:
    /**
     * Constructor. Initializes all problem variables as well.
     */
    RTTP(int numberOfTeams, int numberOfDays, int maxConsecutiveOffDays, int maxConsecutiveGames);
    
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
    
    // -----------------------------------------------------------------------------------
    
    int numberOfRestrictionsNotMet();
    
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
    
    void generateNeighbour();
    void fixVariables();
};
#endif