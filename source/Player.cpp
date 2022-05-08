#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "headers/Player.h"
#include "headers/Map.h"
#include "headers/Orders.h"
#include "headers/Cards.h"
#include "headers/PlayerStrategy.h"
#include "headers/GameEngine.h"








void Player::setHand(Hand* hand) {
	this->playerHand = hand;
}

void Player::setStrategy(std::string pt) {
	
	playerType = pt;

	if (strategy)
		delete strategy;

	if( pt==playerTypes[0])
		strategy = new HumanPlayerStrategy(this);
	else if(pt== playerTypes[1])
		strategy = new AggressivePlayerStrategy(this);
	else if (pt == playerTypes[2])
		strategy = new BenevolentPlayerStrategy(this);
	else if (pt == playerTypes[3])
		strategy = new NeutralPlayerStrategy(this);
	else if (pt == playerTypes[4])
		strategy = new CheaterPlayerStrategy(this);

}

string Player::getStrategy() {
	return playerType;
}

//Default Constructor
Player::Player() {
	name = "";
	playerOrders = nullptr;
	playerHand = nullptr;
	strategy = nullptr;
	gameEngine = nullptr;
	reinforcementPool = 0;
	shoulGetCard = false;

	numberOfAdvances = 0;
	numberOfDeploys = 0;
	numberOfDeployedUnits = 0;

}

//Constructor
Player::Player(std::string n, GameEngine* engine)
{
	name = n;
	playerOrders = new OrderList();
	playerHand = new Hand();
	strategy = nullptr;
	reinforcementPool = 0;
	gameEngine = engine;

	numberOfAdvances = 0;
	numberOfDeploys = 0;
	numberOfDeployedUnits = 0;
}

//Copy Constructor
Player::Player(const Player& p) {
	playerTerritories = p.playerTerritories;

	name = p.name;
	playerOrders = new OrderList(*p.playerOrders);
	playerHand = new Hand(*p.playerHand);
	reinforcementPool = p.reinforcementPool;

	inPeace = p.inPeace;
	shoulGetCard = p.shoulGetCard;
	strategy = p.strategy;
	gameEngine = p.gameEngine;

	numberOfAdvances = p.numberOfAdvances;
	numberOfDeploys = p.numberOfDeploys;
	numberOfDeployedUnits = p.numberOfDeployedUnits;

}

//Copy-Assignment Operator
Player& Player::operator=(const Player& p) {
	this->name = p.name;
	this->playerTerritories = p.playerTerritories;
	this->reinforcementPool = p.reinforcementPool;
	this->gameEngine = p.gameEngine; 
	this->strategy = p.strategy;
	this->inPeace = p.inPeace;
	this->shoulGetCard = p.shoulGetCard;
	this->numberOfAdvances = p.numberOfAdvances;
	this->numberOfDeploys = p.numberOfDeploys;
	this->numberOfDeployedUnits = p.numberOfDeployedUnits;

	return *this;
}

//Destructor
Player::~Player() {
	
	delete playerOrders;
	delete playerHand;
	delete strategy;

	playerOrders = nullptr;
	playerHand = nullptr;

}

//Add Territory
void Player::addTerritory(Territory* t) {
	playerTerritories.push_back(t);
}

//Remove Territory
void Player::removeTerritory(Territory* t) {
	for (int i = 0; i < playerTerritories.size(); i++) {
		if (playerTerritories[i] == t) {
			playerTerritories.erase(playerTerritories.begin() + i);
		}
	}
}

void Player::setReinforcementPool(int amount) {
	reinforcementPool = amount;
}

int Player::getReinforcementPool() {
	return reinforcementPool;
}

//Returns Defendable Territories (All player territories for now)
std::vector<Territory*> Player::toDefend() {
	return strategy->toDefend();
}

//Returns Attackable Territories (Territories adjacent to Defendable/Owned Territories)
std::vector<Territory*> Player::toAttack() {

	return strategy->toAttack();
}

//Creates and issues an order to the players Order List
void Player::issueOrder() {
	return strategy->issueOrder();
}

ostream& operator<<(ostream& stream, Player& p) {
	stream << p.getPlayerName();
	return stream;
}

std::string Player::getPlayerName() {
	return name;
}

Hand* Player::getPlayerHand() {
	return playerHand;
}

OrderList* Player::getPlayerOrders() {
	return playerOrders;
}

GameEngine* Player::getGameEngine()
{
	return gameEngine;
}

std::vector<Territory*> Player::getPlayerTerritories() {
	return playerTerritories;
}


void Player::setPeaceWith(Player* anotherPlayer) {
	inPeace.push_back(anotherPlayer);
};

std::vector<Player*>* Player::getPeaceList() {
	return &inPeace;
}

bool Player::getShouldGetCard() {
	return shoulGetCard;
}

void  Player::setShouldGetCard(bool val) {
	shoulGetCard = val;
}
