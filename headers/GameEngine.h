#ifndef GameEngine_h
#define GameEngine_h
#include <iostream>
#include <vector>

#include "headers/LoggingObserver.h"
using namespace std;

class Player;
class Map;
class MapLoader;
class CommandProcessor;
class Command;
class Deck;

//game state
enum GameState
{
    GAME_STATE_UNKNOWN = 0,
    GAME_STATE_START = 1,
    GAME_STATE_MAP_LOAD = 2,
    GAME_STATE_MAP_VALIDATED = 3,
    GAME_STATE_PLAYERS_ADDED = 4,
    GAME_STATE_ASSIGN_REINFORCEMENT = 5,
    GAME_STATE_ISSUE_ORDERS = 6,
    GAME_STATE_EXECUTE_ORDERS = 7,
    GAME_STATE_WIN = 8,
    GAME_STATE_END = 9,
    GAME_STATE_MAX = 0XFFFF,//set a max value to prevent overflow
};
//user input

//GameEngine class
class GameEngine : public ILoggable, public Subject {
public:
    GameEngine(); //constructor
    virtual ~GameEngine(); //destructor
    GameEngine(const GameEngine& gm); //copy constructor using deep methods

    GameState getCurrentState(); //getter
    Deck* getDeck();
    vector<Player*>* getPlayers();
    void addPlayer(Player* p);
    Map* getMap() const;
    
    //--------------------------------------------------------------------------------------------------------------------@nahid
    void setMap(Map* m);

    GameEngine& operator=(const GameEngine& obj);//Assignment operator
    friend ostream& operator<<(ostream& out, const GameState value);//stream insertion operator

    void transition(Command* c = nullptr);
    void game_run();

    virtual string stringToLog();

    static string gameStateToString(GameState gs);

private:

    void setCurrentState(GameState eState);//setter
    void printInputOptions();

    void gameStartUpPhase();
    void distributeTerritories();
    void randomizePlayerOrder();
    void initializePlayers();

    void mainGameLoop();
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();

    bool parseTournamentCommand(Command* c);
    void runTournament();

    void startNewGame();


    GameState* eState;
    CommandProcessor* commandProcessor;
    MapLoader* mapLoader;
    Map* gameMap;
    vector<Player*>* players;
    Deck* deck;

    //Tournament Related Stuff!
    bool isTournament;
    bool tournamentIsFinished;
    int gamesPerMap;
    int turnsPerGame;
    int currentTurn;

    vector<string> mapPaths;
    vector<string> playerStrategies;
    string winnerName;
    vector<string> tournamentResults;
};

vector<string> splitString(string s, char delimiter);
#endif
