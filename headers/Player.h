#pragma once
#include <string>
#include <iostream>
#include <vector>

class Territory;

class Cards;

class Hand;

class Deck;

class OrderList;

class Orders;

class GameEngine;
class PlayerStrategy;


class Player {

public:
	// Default Constructor
	Player();

	// Constructor
	Player(std::string n , GameEngine* engine);

	// Copy Constructor
	Player(const Player& p);

	// = operator overload
	Player& operator= (const Player& p);

	// Deconstructor
	virtual ~Player();
	void setReinforcementPool(int amount);

	int getReinforcementPool();

	void addTerritory(Territory* t);

	void removeTerritory(Territory* t);

	//Assignment required functions
	std::vector<Territory*> toDefend();  

	std::vector<Territory*> toAttack();   

	void issueOrder();                   

	// operator << overload
	friend std::ostream& operator<<(std::ostream& stream, Player& p);

	// Getters

	std::string getPlayerName();

	Hand* getPlayerHand();

	OrderList* getPlayerOrders();

	GameEngine* getGameEngine();

	std::vector<Territory*> getPlayerTerritories();

	//keeps track of the players with whom the currunt player is in peace with
	void setPeaceWith(Player* anotherPlayer);

	std::vector<Player*>* getPeaceList();

	//indicates if a player conquered a territory during their turn
	void setShouldGetCard(bool val);
  
	bool getShouldGetCard();

	int numberOfAdvances;
	int numberOfDeploys;
	int numberOfDeployedUnits;

	
	void setHand(Hand* hand);

	std::vector<std::string> playerTypes{"Human", "Aggressive", "Benevolent", "Neutral", "Cheater" };
	void setStrategy(std::string pt);
	std::string getStrategy();

private:

	PlayerStrategy* strategy;
	std::string playerType;


	std::vector<Territory*> playerTerritories;
	OrderList* playerOrders;
	Hand* playerHand;
	GameEngine* gameEngine;
	std::string name;
	int reinforcementPool;

	std::vector<Player*> inPeace;
	bool shoulGetCard;
};
