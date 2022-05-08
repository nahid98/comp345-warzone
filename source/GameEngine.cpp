#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "headers/GameEngine.h"
#include "headers/Player.h"
#include "headers/Map.h"
#include "headers/Orders.h"
#include "headers/Cards.h"
#include "headers/CommandProcessing.h"
#include "headers/Cards.h"
/**
 * constructor of GameEngine class
 */
GameEngine::GameEngine()
{
	eState = new GameState(GAME_STATE_UNKNOWN);
	players = new vector<Player*>(0);
	gameMap = nullptr;
	deck = new Deck(50);
	mapLoader = new MapLoader();
	commandProcessor = new CommandProcessor();

	isTournament = false;
	tournamentIsFinished = false;
	gamesPerMap = 0;
	turnsPerGame = 50;
	currentTurn = 0;
	winnerName = "";
}
/**
 * Copy constructor of GameEngine class
 */
GameEngine::GameEngine(const GameEngine& obj)
{
	eState = new GameState;
	*eState = *obj.eState;

	gameMap = new Map(*obj.gameMap);
	mapLoader = new MapLoader(*obj.mapLoader);
	players = new vector<Player*>(*obj.players);

	deck = new Deck(*obj.deck);
	commandProcessor = new CommandProcessor(*obj.commandProcessor);


	isTournament = obj.isTournament;
	tournamentIsFinished = obj.tournamentIsFinished;
	gamesPerMap = obj.gamesPerMap;
	turnsPerGame = obj.turnsPerGame;
	currentTurn = obj.currentTurn;
	winnerName = obj.winnerName;
}

/**
 * destructor of GameEngine class
 */
GameEngine::~GameEngine()
{
	delete eState;
	delete gameMap;
	delete mapLoader;

	for (Player* p : *players) {
		delete p;
	}

	delete players;
	delete deck;
	delete commandProcessor;

	gameMap = nullptr;
	players = nullptr;
	mapLoader = nullptr;
	deck = nullptr;
	commandProcessor = nullptr;
	eState = NULL;
}

/**
 * setter
 * @param lState local state of GameState
 */

 //-----------------------------------------------------------------------------------------------@nahid
void GameEngine::setMap(Map* m) {
	gameMap = m;
}

void GameEngine::setCurrentState(GameState lState)
{
	*eState = lState;
}
void GameEngine::printInputOptions()
{
	cout << "In state " + gameStateToString(*eState) + ", options (";
	switch (*eState) {
	case GAME_STATE_UNKNOWN:
		cout << "-console | -file {filepath}";
		break;
	case GAME_STATE_START:
		cout << "\n\tloadmap {filepath} \n\ttournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns> \n\t\t[[lists should be separated by commas]]";
		break;
	case GAME_STATE_MAP_LOAD:
		cout << "loadmap {filepath} | validatemap";
		break;
	case GAME_STATE_MAP_VALIDATED:
		cout << "addplayer {name} (Aggressive|Cheater|Neutral|Benevolent|Human)";
		break;
	case GAME_STATE_PLAYERS_ADDED:
		cout << "addplayer {name} (Aggressive|Cheater|Neutral|Benevolent|Human) | gamestart";
		break;
	case GAME_STATE_WIN:
		cout << "replay | quit";
		break;
	default:
		return;
	}

	cout << ")" << endl;
}
/**
 * getter
 * @return lState local state of GameState
 */
GameState GameEngine::getCurrentState()
{
	return *eState;
}

/**
 * Assignment operator of GameEngine class
 */

GameEngine& GameEngine::operator=(const GameEngine& obj)
{
	//TODO: assign the rest of the data members
	this->eState = new GameState(*obj.eState);
	return *this;
}

/**
 * Stream insertion operator of GameEngine class
 */
ostream& operator<<(ostream& out, const GameState value) {
	const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
	//#p is the preprocessor stringifying p. So calling PROCESS_VAL(ErrorA) will output: case(ErrorA): s = "ErrorA"; break;.
	switch (value) {
		PROCESS_VAL(GAME_STATE_UNKNOWN);
		PROCESS_VAL(GAME_STATE_START);
		PROCESS_VAL(GAME_STATE_MAP_LOAD);
		PROCESS_VAL(GAME_STATE_MAP_VALIDATED);
		PROCESS_VAL(GAME_STATE_PLAYERS_ADDED);
		PROCESS_VAL(GAME_STATE_ASSIGN_REINFORCEMENT);
		PROCESS_VAL(GAME_STATE_ISSUE_ORDERS);
		PROCESS_VAL(GAME_STATE_EXECUTE_ORDERS);
		PROCESS_VAL(GAME_STATE_WIN);
	}
#undef PROCESS_VAL

	return out << s;
}
vector<string> splitString(string s, char delimiter)
{
	vector<string> result;
	std::stringstream ss(s);

	std::string item;
	while (std::getline(ss, item, delimiter)) {
		result.push_back(item);
	}

	return result;
}
/**
 * free function: get user input
 * @param lState local state
 * @return Name string of the user input
 */
string get_user_input(GameState lState) {
	string Name;
	cout << "The current game state is (" << lState << ")\n";
	cout << "Please type your command with lower-case letters:" << "\n";
	getline(cin, Name);
	return Name;
}


Deck* GameEngine::getDeck()
{
	return deck;
}

vector<Player*>* GameEngine::getPlayers()
{
	return players;
}

void GameEngine::addPlayer(Player* p)
{
	players->push_back(p);
}

Map* GameEngine::getMap() const
{
	return gameMap;
}

void GameEngine::transition(Command* c) {

	//Get user input unless a command was passed in
	if (c == nullptr) {
		do
		{
			printInputOptions();
			c = commandProcessor->getCommand(*eState);
		} while (!c->getIsValid());
	}

	/// Transition from one state to another based on the command
	switch (*eState) {
		case GameState::GAME_STATE_UNKNOWN:
			if (regex_match(c->getCommand(), regex("-file\\s.+"))) {
				string location = c->getCommand().substr(6);
				delete commandProcessor;
				commandProcessor = new FileCommandProcessorAdapter(location);
			}
			setCurrentState(GameState::GAME_STATE_START);
			break;
		case GameState::GAME_STATE_START:
			if (regex_match(c->getCommand(), regex("tournament.*")) && parseTournamentCommand(c)) {
				isTournament = true;
				tournamentIsFinished = false;
				tournamentResults.clear();
			}
			else {
				mapLoader->setFilePath(c->getCommand().substr(8));
				if (gameMap = mapLoader->generateMap()) {
					setCurrentState(GameState::GAME_STATE_MAP_LOAD);
					c->saveEffect("Successfully loaded map " + c->getCommand().substr(8));
				}
				else {
					c->saveEffect("Map was not valid.");
				}
			}
			break;

		case GameState::GAME_STATE_MAP_LOAD:
			if (regex_match(c->getCommand(), regex("loadmap.*"))) {

				if (gameMap != nullptr)
					delete gameMap;

				mapLoader->setFilePath(c->getCommand().substr(8)); 
				if (gameMap = mapLoader->generateMap()) {

					setCurrentState(GameState::GAME_STATE_MAP_LOAD);
					c->saveEffect("Successfully loaded map " + c->getCommand().substr(8));
				}
				else {
					c->saveEffect("Map was not valid.");
				}
			}
			else {

				if (gameMap->validate()) {

					setCurrentState(GameState::GAME_STATE_MAP_VALIDATED);
					c->saveEffect("Map was successfully validated");
				}
				else {
					c->saveEffect("Map failed to validate");
				}
			}

			break;

		case GameState::GAME_STATE_MAP_VALIDATED:
		{

			std::smatch matches;
			string command = c->getCommand();

			if (regex_match(command, matches, regex("^addplayer\\s(.*)\\s(Aggressive|Neutral|Benevolent|Cheater|Human)$"))) {
				players->push_back(new Player(matches[1], this));
				players->back()->setStrategy(matches[2]);

				c->saveEffect("Added player with name " + players->back()->getPlayerName() + " and strategy " + players->back()->getStrategy());

				setCurrentState(GameState::GAME_STATE_PLAYERS_ADDED);
			}

			break;
		}
		case GameState::GAME_STATE_PLAYERS_ADDED:
		{
			std::smatch matches;
			string command = c->getCommand();

			if (regex_match(c->getCommand(), regex("^addplayer\\s.+"))) {
				if (regex_match(command, matches, regex("^addplayer\\s(.+)\\s(Aggressive|Neutral|Benevolent|Cheater|Human)$"))) {

					if (players->size() == 6) {
						c->saveEffect("Failed to add a new player, there are already 6 players!");
						cout << "Failed to add player, already at 6!" << endl;
						return;
					}

					players->push_back(new Player(matches[1], this));
					players->back()->setStrategy(matches[2]);
					c->saveEffect("Added player with name " + players->back()->getPlayerName() + " and strategy " + players->back()->getStrategy());

					setCurrentState(GameState::GAME_STATE_PLAYERS_ADDED);
				}
			}
			else {
				if (players->size() < 2) {
					c->saveEffect("Failed to start the game, too few players, currently there are " + players->size());
					cout << "too few players!" << endl;
					return;
				}

				setCurrentState(GameState::GAME_STATE_ASSIGN_REINFORCEMENT);
				c->saveEffect("Game has successfully started.");
			}

			break;
		}
		case GameState::GAME_STATE_WIN:
			if (c->getCommand().compare("quit") == 0) {
				cout << "Thank you for playing!" << endl;
				setCurrentState(GameState::GAME_STATE_END);
			}
			else {
				startNewGame();
				setCurrentState(GameState::GAME_STATE_START);
			}
			break;
		default:
			cout << "not sure how you got here!" << endl;
			exit(1);
		}

}

/**
 * function game_run() of GameEngine class
 * To update the current state by a valid command, reject the command if it is invalid
 */
void GameEngine::game_run() {

	transition();
	while (*eState != GameState::GAME_STATE_END) {

		transition();

		if (isTournament) {
			runTournament();

			isTournament = false;
			tournamentIsFinished = false;
			tournamentResults.clear();
		}
		else {
			gameStartUpPhase();
			mainGameLoop();
		}

		transition();
	}
}//end of game_run()


void GameEngine::gameStartUpPhase() {
	while (*eState != GameState::GAME_STATE_ASSIGN_REINFORCEMENT) {
		transition();
	}

	distributeTerritories();
	randomizePlayerOrder();
	initializePlayers();
}

void GameEngine::distributeTerritories()
{
	vector<Territory*> territories = gameMap->getTerritories();
	int index = 0;
	for (Territory* t : territories) {
		t->setOwningPlayer(players->at(index));
		players->at(index)->addTerritory(t);

		index = (index + 1) % players->size();
	}
}

void GameEngine::randomizePlayerOrder()
{
	random_shuffle(players->begin(), players->end());
}

void GameEngine::initializePlayers()
{
	//Set reinforcements and draw two cards
	for (Player* p : *players) {
		p->setReinforcementPool(50);
		deck->draw(*p->getPlayerHand());
		deck->draw(*p->getPlayerHand());
	}
}

void GameEngine::mainGameLoop() {
	bool noWinner = true;
	int winnerIndex = 0;
	while (noWinner) {

		if (currentTurn > turnsPerGame) {
			setCurrentState(GameState::GAME_STATE_WIN);
			winnerName = "Draw";
			break;
		}

		//Check for winner
		if (players->size() == 1) {
			noWinner = false;
			winnerName = players->at(0)->getPlayerName();
			setCurrentState(GameState::GAME_STATE_WIN);
			break;
		}

		reinforcementPhase();
		issueOrdersPhase();
		executeOrdersPhase();

		//Check if player has lost

		for (int i = 0; i < players->size(); i++) {
			if ((*players)[i]->getPlayerTerritories().size() == 0) {
				cout << "PLAYER " << (*players)[i]->getPlayerName() << " HAS BEEN ELIMINATED\n";
				delete *(players->begin() + i);
				players->erase(players->begin() + i);
				
				continue;
			}

			if (players->at(i)->getShouldGetCard()) {
				players->at(i)->setShouldGetCard(false);
				deck->draw(*players->at(i)->getPlayerHand());
			}

			players->at(i)->getPeaceList()->clear();
		}

		currentTurn++;
	}

	if (noWinner) {
		cout << "Game ended in a draw!" << endl;
	}
	else {
		cout << "PLAYER " << (*players)[0]->getPlayerName() << " HAS WON THE GAME!" << endl;
	}
}

void GameEngine::reinforcementPhase() {
	
	//Get map Continents
	vector<Continent*> continents = gameMap->getContinents();
	
	//Iterate through each player
	for (int i = 0; i < players->size(); i++) {
		//Get each players territories
		Player* p = (*players)[i];
		vector<Territory*> t = p->getPlayerTerritories();
		int continentBonus = 0;

		//Iterate through each territory of each player and add the continentId of each territory to a vector
		vector<int> cids = {};
		for (int k = 0; k < t.size(); k++) {
			cids.push_back(t[k]->getContinentId());
		}

		//Iterate through each continent and check if the player owns all territories of said continent
		for (int j = 0; j < continents.size(); j++) {
			int continentId = continents[j]->getContinentId();
			int continentSize = continents[j]->getMembers().size();
			if (count(cids.begin(), cids.end(), continentId) == continentSize) {
				continentBonus += gameMap->getContinentWithId(continentId)->getBonusArmyCount();
			}
		}

		//Get amount of reinforcements

		int armyBonus = floor(t.size() / 3) + continentBonus;
		if (armyBonus < 3)
			armyBonus = 3;
		//Add them to player's reinforcementPool
		p->setReinforcementPool( p->getReinforcementPool() + armyBonus);
	}

	setCurrentState(GameState::GAME_STATE_ISSUE_ORDERS);
}

////////////////////////////////////////////////////////////////
//////////////////////  ISSUE ORDERS ///////////////////////////
////////////////////////////////////////////////////////////////

void GameEngine::issueOrdersPhase() {

	/////////////////////////////// DEPLOY ISSUE ///////////////////////////////////

	//Shallow copy players to playerRoster so players can be removed popped from vector when order is issued without 
	//removing them from the players vector
	vector<Player*> playerRoster = *players;

	for (int i = 0; i < playerRoster.size(); i++) {
		//Because of cheater, need to check for eliminations here as well, what a nightmare!
		if ((*players)[i]->getPlayerTerritories().size() == 0) {
			cout << "PLAYER " << (*players)[i]->getPlayerName() << " HAS BEEN ELIMINATED\n";
			delete* (players->begin() + i);
			players->erase(players->begin() + i);
			playerRoster.erase(playerRoster.begin() + i);
			continue;
		}

		playerRoster[i]->issueOrder();
	}

	setCurrentState(GameState::GAME_STATE_EXECUTE_ORDERS);
}


////////////////////////////////////////////////////////////////
//////////////////////  EXECUTE ORDERS /////////////////////////
////////////////////////////////////////////////////////////////

void GameEngine::executeOrdersPhase() {
	///////////////// EXECUTE DEPLOY ///////////////////////////////

	bool nonDeployOrders = true;
	bool deployOrders = true;

	//Shallow copy players to playerRoster so players can be removed popped from vector when order is issued without 
	//removing them from the players vector
	vector<Player*> playerRoster = *players;
	int playerNum = 0;
	while (deployOrders) {
    
    
		//Decides whose turn it is
		playerNum = playerNum % playerRoster.size();
		//Player* p = playerRoster[playerNum];

		//Gets the players orders list
		vector<Orders*> p_orders = playerRoster[playerNum]->getPlayerOrders()->get_orderList();

		//Gets the first order to be implemented
		Orders* currentOrder = (p_orders.size() > 0) ? *p_orders.begin() : nullptr;

		//Checks if this order is a deploy order
		if (currentOrder && currentOrder->get_order_id() == "deploy") {
			currentOrder->execute();
			playerRoster[playerNum]->getPlayerOrders()->delet_order(currentOrder);
			++playerNum;
		}
		else {
			//Removes from roster if orderlist is empty or not a deploy order
			playerRoster.erase(playerRoster.begin() + playerNum);
		}
    
		if (playerRoster.size() == 0) {
			deployOrders = false;
		}
	}

	///////////////////////////// EXECUTE OTHER ORDERS /////////////////////////////////

	playerRoster = *players;
	while (nonDeployOrders) {
		//Decides whose turn it is
		playerNum = playerNum % playerRoster.size();
		Player* p = playerRoster[playerNum];

		//Checks if orderlist is empty
		if (p->getPlayerOrders()->get_orderList().size() > 0) {
			//Gets the order to be implemented
			Orders* currentOrder = *p->getPlayerOrders()->get_orderList().begin();

			//Validates Order
			bool valid = currentOrder->validate();
			if (!valid) {
				cout<< currentOrder->get_order_id() << " is invalid" << "\n";
			}

			//Executes order
			currentOrder->execute();
			p->getPlayerOrders()->delet_order(currentOrder);
			playerNum++;
		}
		else {
			//Removes player from roster
			playerRoster.erase(playerRoster.begin() + playerNum);
		}
		// Moves to reinforcement phase
		if (playerRoster.size() == 0) {
			nonDeployOrders = false;
		}
	}
}

string GameEngine::gameStateToString(GameState gs)
{
	switch (gs) {
	case GAME_STATE_UNKNOWN:
		return "GAME_STATE_UNKNOWN";
	case GAME_STATE_START:
		return "GAME_STATE_START";
	case GAME_STATE_MAP_LOAD:
		return "GAME_STATE_MAP_LOAD";
	case GAME_STATE_MAP_VALIDATED:
		return "GAME_STATE_MAP_VALIDATED";
	case GAME_STATE_PLAYERS_ADDED:
		return "GAME_STATE_PLAYERS_ADDED";
	case GAME_STATE_ASSIGN_REINFORCEMENT:
		return "GAME_STATE_ASSIGN_REINFORCEMENT";
	case GAME_STATE_ISSUE_ORDERS:
		return "GAME_STATE_ISSUE_ORDERS";
	case GAME_STATE_EXECUTE_ORDERS:
		return "GAME_STATE_EXECUTE_ORDERS";
	case GAME_STATE_WIN:
		return "GAME_STATE_WIN";
	case GAME_STATE_END:
		return "GAME_STATE_END";
	default:
		return "UNDEFINED_STATE";
	}
}

bool GameEngine::parseTournamentCommand(Command* c)
{
	//Split string into vector
	vector<string> result = splitString(c->getCommand(), ' ');
	int index = 1;

	//Parse game options 
	while (index != result.size()) {
		if (result[index] == "-M") {
			mapPaths = splitString(result[++index], ',');
		}
		else if (result[index] == "-P") {
			playerStrategies = splitString(result[++index], ',');
		}
		else if (result[index] == "-G") {
			gamesPerMap = stoi(result[++index]);
		}
		else if (result[index] == "-D") {
			turnsPerGame = stoi(result[++index]);
		}
		else {
			cout << "Invalid option [" << result[index] << "]" << endl;
		}

		index++;
	}


	isTournament = true;

	return true;
}

void GameEngine::runTournament()
{
	//For each map
	for (int currentMapIndex = 0; currentMapIndex < mapPaths.size(); currentMapIndex++) {
		//For each game
		for (int currentGameIndex = 0; currentGameIndex < gamesPerMap; currentGameIndex++) {
			
			//Simulate gameStartupPhase without user interaction
			commandProcessor->saveCommand((new Command("loadmap " + mapPaths[currentMapIndex])));
			transition(commandProcessor->commandList.back());

			commandProcessor->saveCommand(new Command("validatemap "));
			transition(commandProcessor->commandList.back());

			for (string s : playerStrategies) {
				commandProcessor->saveCommand(new Command("addplayer " + s + " " + s));
				transition(commandProcessor->commandList.back());
			}

			commandProcessor->saveCommand(new Command("gamestart"));
			transition(commandProcessor->commandList.back());

			distributeTerritories();
			randomizePlayerOrder();
			initializePlayers();

			//Run the game
			mainGameLoop();

			//Retrieve game result
			tournamentResults.push_back(winnerName);

			//If there is still another game to be played, replay
			if (tournamentResults.size() != gamesPerMap * mapPaths.size()) {
				commandProcessor->saveCommand(new Command("replay"));
				transition(commandProcessor->commandList.back());
			}
		}
	}

	tournamentIsFinished = true;
	Notify(this);
}

//Resets the GameEngine and prepares it for a new game
void GameEngine::startNewGame()
{
	delete gameMap;
	delete deck;

	for (Player* p : *players) {
		delete p;
	}
	players->clear();

	deck = new Deck(50);
	deck->shuffle();

	winnerName = "";

	currentTurn = 0;
}

//To be called in Notify, returns the string to be logged by the LogObserver
string GameEngine::stringToLog()
{
	string result = "";

	//If a tournament just completed, notify the tournamnet results
	if (isTournament && tournamentIsFinished) {
		result += "Tournament Mode:\n";
		result += "M:";

		for (string path : mapPaths) {
			result += "\t" + path;
		}
		result += "\n";
		result += "P:";

		for (string strategy : playerStrategies) {
			result += "\t" + strategy;
		}
		result += "\n";
		result += "G: " + to_string(gamesPerMap) + "\n";
		result += "D: " + to_string(turnsPerGame) + "\n\n";


		int width = 0;
		for (string s : mapPaths) {
			width = (width < s.size()) ? s.size() + 1 : width;
		}

		ostringstream tournamentResultsStream;
		tournamentResultsStream << "Results:\n";
		tournamentResultsStream.width(width);
		tournamentResultsStream << "|";

		for (int i = 1; i <= gamesPerMap; i++) {
			tournamentResultsStream.width(15);
			tournamentResultsStream << "Game " + to_string(i);
		}
		tournamentResultsStream << "\n";

		for (int j = 0; j < mapPaths.size(); j++) {
			tournamentResultsStream << mapPaths.at(j);
			tournamentResultsStream.width(width - mapPaths.at(j).size());
			tournamentResultsStream << "|";

			for (int k = 0; k < gamesPerMap; k++) {
				tournamentResultsStream.width(15);
				tournamentResultsStream << " " + tournamentResults.at((j * gamesPerMap) + k) + "|";
			}
			tournamentResultsStream << "\n";
		}
		result += tournamentResultsStream.str();

		return result;
	}
	//Tournament isn't done yet, notify of game state transitions
	result = "GameEngine has changed state to " + gameStateToString(*eState);

	return result;
}
