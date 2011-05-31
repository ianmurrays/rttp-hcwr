#include <vector>

using namespace std;

class RTTP
{
  private:
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
    vector<int> G;
    
    /**
     * i x d "matrix" that for each O_{id},
     * contains the oponent for team i on day d.
     * Domain is all teams.
     */
    vector<int> O: 
    
    /**
     * i x d "matrix" that contains where team i
     * stays on day d.
     */
    vector<int> V;
    
    /**
     * i x d "matrix" where C_{id} is the cost of 
     * travel of team i in day d.
     */
    vector<int> C;
    
    /****************************************************************/
    
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
     * Constructor
     */
    RTTP(int numberOfTeams, int numberOfDays, int maxConsecutiveOffDays, int maxConsecutiveGames);
    
    /**
     * Destructor
     */
    ~RTTP();
        
    /****************************************************************/
    
    /**
     * Tests all restrictions on the current solution candidate
     * to see if it's valid
     */
    bool validSolution();
    
    /****************************************************************/
    
    /**
     * Evaluates the candidate solution and returns its cost
     */
    int objectiveFunction();
};