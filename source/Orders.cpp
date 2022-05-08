#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
//---------------------------------------------------------------------------------------------------------------------
// This file consists of the "Orders" class and it subclasses implementations
// Subclasses of "Orders" are: "Deploy", "Advance", "Bomb", "Blockade", "Airlift", "Negotiate", and "OrderList"
// For better demonstartion that the code performs correctly, all constructors will print a message to the screen when sucessfuly create an object
//-------------------------------------------------------------------------------------------------------------------

#include "headers/Player.h"
#include "headers/Orders.h"
#include "headers/LoggingObserver.h"
#include "headers/Map.h"
#include "headers/GameEngine.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;
using std::cout;



Orders::Orders():player() {
	orderID = "NotDefined";
	isExecuted = false;
}

Orders::Orders(string orderName) {
	orderID = orderName;
	isExecuted = false;
}

//***
Orders::Orders(Player* player) {
	this->player = player;
	isExecuted = false;
}


//Assignment operator
Orders& Orders::operator=(const Orders& theOrder) {

	this->orderID = theOrder.orderID;
	this->isExecuted = theOrder.isExecuted;
	this->player = theOrder.player;

	return *this;
}

//Copy constructor
Orders::Orders(const Orders& theOrder) {

	this->orderID = theOrder.orderID;
	this->isExecuted = theOrder.isExecuted;
	this->player = theOrder.player;
}


Orders::~Orders() {
}


Player* Orders::getPlayerOfOrder()
{
	return player;
}

void Orders::set_order_id(string theID) {
	orderID = theID;
}

string Orders::get_order_id() {
	return orderID;
}



//****************************************************
// Definitions for the subclasses (different orders)
//****************************************************


//---------------------Deploy Order-------------------------
// To deploy a number of armies to the player's territory
//----------------------------------------------------------

Deploy::Deploy():Orders(), territoryToDeploy()
{
	army = 0;
	set_order_id("deploy");

	cout << " Deploy is created" << endl;
}



Deploy::Deploy(Player* player, Territory* territory, int armyN) : Orders(player) {

	this->territoryToDeploy = territory;
	this->army = armyN;
	set_order_id("deploy");

	cout << "Deploy Order is created..\n";
}


//Assignment operstor
Deploy& Deploy::operator=(const Deploy& d) {
	Orders::operator=(d);

	army = d.army;
	territoryToDeploy = d.territoryToDeploy;

	return *this;
};

// Copy Constructor
Deploy::Deploy(const Deploy& d) :Orders(d) {

	this->army = d.army;
	this->territoryToDeploy = d.territoryToDeploy;

}

Deploy::~Deploy() {
	 territoryToDeploy = nullptr;
}

//If the target territory (territoryToDeploy) does not belong to the player, Diploy order is invalid
bool Deploy::validate() {
	
	vector<Territory*> playerTerr = player->getPlayerTerritories();
	bool territoryIsInList = std::find(playerTerr.begin(), playerTerr.end(), territoryToDeploy)
		!= playerTerr.end();

	return territoryIsInList;
}

// takes a certain number of armies from the reinforcement pool and add them to the target territory(territoryToDeploy)
void Deploy::execute() {
	if (validate()) {

		//if the number of the armies to be deployed is greater than the available armies, then all the armiens will be deployed
		if (player->getReinforcementPool() - army < 0) {
			army = player->getReinforcementPool();
		}

		//taking the armies from the reinforcement pool
		player->setReinforcementPool((player->getReinforcementPool()) - army);

		//adding the army to the desinated territory
		int currentArmy = territoryToDeploy->getArmyCount();
		territoryToDeploy->setArmyCount(currentArmy + army);

		isExecuted = true;
		cout << "\nPlayer " << player->getPlayerName() << " deployed " << army << " armies into the " << territoryToDeploy->getName()<<endl;
		cout << "This territory had " << currentArmy << " armies and now has " << territoryToDeploy->getArmyCount()<<endl;

		Notify(this);
	}
	else {
		cout << "The Deploy order is not valid to execute"<<endl;
	}

}

string Deploy::stringToLog()
{
	return player->getPlayerName() + " deployed " + to_string(army) + " units to " + territoryToDeploy->getName() + ", it now has " + to_string(territoryToDeploy->getArmyCount()) + " units.";
}



std::ostream& Deploy::printAction(std::ostream& out) const {
	out << "Deploy order.";
	if (isExecuted) {
		out << " This order was executed, its effect was [deploy enemy].";
	}
	return out;
}
std::ostream& operator<<(std::ostream& out, const Deploy& toOutput) {
	return toOutput.printAction(out);
}


//---------------Advance order---------------------------------
// Tell a certain number of armies to move
//from a source territory to a target territory
//--------------------------------------------------------------
Advance::Advance() :Orders(), sourceTerritory(), targetTerritory() {

	army = 0;
	set_order_id("advance");
	cout << "Advance order is created" << endl;
}


Advance::Advance(Player* player, Territory* sourceTerritory,
	Territory* targetTerritory, int armyNum) : Orders(player) {

	set_order_id("advance");

	this->sourceTerritory = sourceTerritory;
	this->targetTerritory = targetTerritory;
	this->army = armyNum;


	cout << "Advance Order created..\n";
}



//Assignment operator
Advance& Advance::operator=(const Advance& a) {
	Orders::operator=(a);
	sourceTerritory = a.sourceTerritory;
	targetTerritory = a.targetTerritory;
	army = a.army;

	return *this;
}

// Copy Constructor
Advance::Advance(const Advance& a) :Orders(a) {

	this->sourceTerritory = a.sourceTerritory;
	this->targetTerritory = a.targetTerritory;
	this->army = a.army;

}

Advance::~Advance() {
	targetTerritory = nullptr;

	sourceTerritory = nullptr;

	player = nullptr;


}

bool Advance::validate() {

	//check if the currunt player is in peace with the owner of the target territory
	Player* otherPlayer = targetTerritory->getOwningPlayer();
	
	vector<Player*>* playerPeaceList = player->getPeaceList();
	bool isInPeace = std::find(playerPeaceList->begin(), playerPeaceList->end(), otherPlayer)
		!= playerPeaceList->end();

	if (isInPeace) {
		cout << " Can't issue attack since you are in Peace with this player.\n";
		return false;
	}

	// checking is source territory belongs to the player
	vector<Territory*> playerTerr = player->getPlayerTerritories();
	bool territoryIsInList = std::find(playerTerr.begin(), playerTerr.end(), sourceTerritory)
		!= playerTerr.end();

	//if the source territory should belongs to the current player, and the target territory is adjacent to it, then the order is valid
	if (territoryIsInList==true) {
		//check if the target territory is adjacent to the source territory
		vector<Territory*> adjacentTerritories = sourceTerritory->getAdjacencies();

		bool territoryIsAdjacent = std::find(adjacentTerritories.begin(), adjacentTerritories.end(), targetTerritory)
			!= adjacentTerritories.end();

		if (territoryIsAdjacent)
			return true;
		else
			return false;

	}
	else 
		return false;

}


//if both source and target territory belong to the player then armies are moved from source territory to target territory
// if target territory doesn't belong to the player, and attack will be isued

//Tells a certain number of armies to move from a source territory to the target territory
void Advance::execute() {

	//The order executes if its valid
	if (validate()) {

		//check if the target territory also belongs to the player
		vector<Territory*> playerTerr = player->getPlayerTerritories();
		bool targetTerritoryIsInList = std::find(playerTerr.begin(), playerTerr.end(), targetTerritory)
			!= playerTerr.end();


		Notify(this);

		//If the target territory also belongs to this player, then army usnit will move to the target territory from the source territory
		if (targetTerritoryIsInList==true) {

			//army units to be moved
			int armyToMove = army;
			//if the number of army units is greater than the available army units in the source terrotory then all the army units in the source territory will move to the target territory
			//else, the chosen army units will move to the target terrotorry
			if (sourceTerritory->getArmyCount() < army) {

				armyToMove = sourceTerritory->getArmyCount();
				
			}
			
			int sOriginalArmy = sourceTerritory->getArmyCount();
			int tOriginalArmy = targetTerritory->getArmyCount();

			sourceTerritory->setArmyCount(sourceTerritory->getArmyCount() - armyToMove);
			targetTerritory->setArmyCount(targetTerritory->getArmyCount() + armyToMove);

			cout << armyToMove << " army units has moved from the " << sourceTerritory->getName() << " to " << targetTerritory->getName() << endl;
			cout << "\nBefore : " << sourceTerritory->getName() << " had " << sOriginalArmy << " armies, " << targetTerritory->getName() << " had " << tOriginalArmy<<endl;
			cout << "After: " << sourceTerritory->getName() << " has " << sourceTerritory->getArmyCount() << " armies, " << targetTerritory->getName() << " has " << targetTerritory->getArmyCount()<<endl;
		
		}
		//else if the target territory doesn't belong to the same player, then attack will be issued
		else {
			//If the enemy is a Neutral player, make them aggressive!
			if (targetTerritory->getOwningPlayer()->getStrategy() == "Neutral") {
				targetTerritory->getOwningPlayer()->setStrategy("Aggressive");
			}

			//Armies of each territory before attack
			cout << sourceTerritory->getName() << " : "<<sourceTerritory->getArmyCount() << endl;
			cout << targetTerritory->getName() << " : " << targetTerritory->getArmyCount() << endl;		

			//army units to be moved
			int armyToMove = army;
			int originalArmyCount = armyToMove;
			//if the number of army units is greater than the available army units in the source terrotory then all the army units in the source territory will move to the target territory
			//else, the chosen army units will move to the target terrotorry
			if (sourceTerritory->getArmyCount() < armyToMove) {
				armyToMove = sourceTerritory->getArmyCount();
			}

			//Armies moving out if the source territory
			sourceTerritory->setArmyCount(sourceTerritory->getArmyCount() - armyToMove);

			//The Defending army
			int defArmy = targetTerritory->getArmyCount();

			srand(static_cast<unsigned int>(time(NULL)));
			int diceroll;

			//continue attacke until the player's army or the defending army is defeated
			while (defArmy > 0 && armyToMove > 0 ) {

				//every attacking army has 60% chances of killing one defending army
				diceroll = (rand() % 10) + 1;
				if (diceroll < 7)
					defArmy--;

				//every defending army has 70% chances of killing one attacking army
				diceroll = (rand() % 8) + 1;
				if (diceroll < 8)
					armyToMove--;
			}

			targetTerritory->setArmyCount(defArmy);

			if (defArmy == 0 && armyToMove > 0) {

				cout << " player won\n";
				
				//Transfering the ownership
				Player* enemy = targetTerritory->getOwningPlayer();
				enemy->removeTerritory(targetTerritory);
				targetTerritory->setOwningPlayer(player);
				player->addTerritory(targetTerritory);
				targetTerritory->setArmyCount(armyToMove);

				player->setShouldGetCard(true);

				
			}
			    
		

			cout << player->getPlayerName() << " attacked " << targetTerritory->getName() << " with " << originalArmyCount << " army units\n";
			cout << sourceTerritory->getName() << " now has " << sourceTerritory->getArmyCount() << " while " << targetTerritory->getName() << " has " << targetTerritory->getArmyCount() << " and belongs to " <<targetTerritory->getOwningPlayer()->getPlayerName() << endl;
			}

			

			isExecuted = true;
			cout << "Advanced armies into the territory.\n";
	}
	else {

		cout << " Advance order not valid to be executed\n";

	}

}



std::ostream& Advance::printAction(std::ostream& out) const {
	out << "Advance order.";
	if (isExecuted) {
		out << " This order was executed, its effect was [Advance enemy].";
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, const Advance& toOutput) {
	return toOutput.printAction(out);
}


string Advance::stringToLog()
{
	if (sourceTerritory->getOwningPlayer() == targetTerritory->getOwningPlayer()) {
		return player->getPlayerName() + " advanced " + to_string(army) + " unit(s) from " + sourceTerritory->getName() + " to " + targetTerritory->getName() + ".";
	}

	return player->getPlayerName() + " attacked with " + to_string(army) + " unit(s) from " + sourceTerritory->getName() + " to " + targetTerritory->getName() + ".";
}

//-----------------Bomb order----------------------------------
//Removes half of the armies in enemy's territory
//--------------------------------------------------------------

Bomb::Bomb() :Orders(), sourceTerritory(), targetTerritory() {
	
	set_order_id("bomb");
	cout << "Bomb order is created" << endl;
}

Bomb::Bomb(Player* player, Territory* sourceTerritory, Territory* targetTerritory)
	: Orders(player) {

	set_order_id("bomb");
	this->sourceTerritory = sourceTerritory;
	this->targetTerritory = targetTerritory;

	cout << "Bomb Order created..\n";
}

//Assignment operator
Bomb& Bomb::operator=(const Bomb& b) {
	Orders::operator=(b);
	sourceTerritory = b.sourceTerritory;
	targetTerritory = b.targetTerritory;

	return *this;
}

// Copy Constructor
Bomb::Bomb(const Bomb& b) :Orders(b) {

	this->sourceTerritory = b.sourceTerritory;
	this->targetTerritory = b.targetTerritory;
}

Bomb::~Bomb() {

	sourceTerritory = nullptr;

	targetTerritory = nullptr;

	player = nullptr;
}


std::ostream& Bomb::printAction(std::ostream& out) const {
	out << "Bomb order.";
	if (isExecuted) {
		out << " This order was executed, its effect was [Bomb enemy].";
	}
	return out;
}
std::ostream& operator<<(std::ostream& out, const Bomb& toOutput) {
	return toOutput.printAction(out);
}

//Target territory should not belong to the current player & sould be adjacent to the source territory
bool Bomb::validate() {

	//check if the currunt player is in peace with the owner of the target territory
	Player* otherPlayer = targetTerritory->getOwningPlayer();
	vector<Player*>* playerPeaceList = player->getPeaceList();
	bool isInPeace = std::find(playerPeaceList->begin(), playerPeaceList->end(), otherPlayer)
		!= playerPeaceList->end();

	if (isInPeace) {
		cout << " Can't issue attack since you are in Peace with this player.\n";
		return false;
	}
	
	vector<Territory*> playerTerr = player->getPlayerTerritories();

	//chcek if source territory and target territory belong to the player
	bool sourceTerrIsInList = std::find(playerTerr.begin(), playerTerr.end(), sourceTerritory)
		!= playerTerr.end();

	bool targetTerrIsInList = std::find(playerTerr.begin(), playerTerr.end(), targetTerritory)
		!= playerTerr.end();

	if (targetTerrIsInList==true)
		cout << "target belongs to player\n";
	if (sourceTerrIsInList==false)
		cout << "source doesn't belong to player\n";

	//If source territory belongs to the player and the target territory doesn't, we check to see if these territories are adjacent
	if (sourceTerrIsInList==true && targetTerrIsInList==false) {

		bool isAdjacent = false;

		//check if the target territory is adjacent to one of playre's territories
		for (int i = 0; i < playerTerr.size(); i++) {
			vector<Territory*> adjacentTerritories = playerTerr.at(i)->getAdjacencies();

			bool territoryIsAdjacent = std::find(adjacentTerritories.begin(), adjacentTerritories.end(), targetTerritory)
				!= adjacentTerritories.end();

			if (territoryIsAdjacent == true)
				cout << " is adjacent\n";

			if (territoryIsAdjacent)
				isAdjacent = true;

		}

		return isAdjacent;

	}
	else
		return false;

}

void Bomb::execute() {

	if (validate()) {

		cout << "Before:  "<<sourceTerritory->getName() << " : " << sourceTerritory->getArmyCount() << ", " << targetTerritory->getName() << " : " << targetTerritory->getArmyCount() << endl;

		int currentArmy = targetTerritory->getArmyCount();
		targetTerritory->setArmyCount(currentArmy / 2);
		
		Player* enemy = targetTerritory->getOwningPlayer();
		if (enemy->getStrategy() == "Neutral")
			enemy->setStrategy("Aggressive");

		//If the enemy's territory has 0 armies then the player has conquered it
		if (targetTerritory->getArmyCount() == 0) {

			player->setShouldGetCard(true);

			//transfering teh ownership
			enemy->removeTerritory(targetTerritory);
			targetTerritory->setOwningPlayer(player);
			player->addTerritory(targetTerritory);


		}

		isExecuted = true;


		cout << " Bomb attack result:\n";
		cout << "After:   " <<sourceTerritory->getName() << " : " << sourceTerritory->getArmyCount() << ", " << targetTerritory->getName() << " : " << targetTerritory->getArmyCount() << endl;

		Notify(this);
	}
	else
		cout << " Bomb order not valid to be executed\n";
}


string Bomb::stringToLog()
{
	return player->getPlayerName() + " bombed " + targetTerritory->getName() + ", it now has " + to_string(targetTerritory->getArmyCount()) + " unit(s).";
}

//---------------Blockade order----------------------------------------------------
//Doubles the number of armies on a target territory
// that belongs to the player, and transfers its ownership to a NEutral player
//---------------------------------------------------------------------------------

Blockade::Blockade() :Orders(), targetTerritory() {
	set_order_id("blockade");
	cout << "Blockade order is creatd" << endl;
}

Blockade::Blockade(Player* player, Territory* targetTerritory) : Orders(player) {
	set_order_id("blockade");
	this->targetTerritory = targetTerritory;

	cout << "Blockade Order created..\n";
}

//Assignment operator
Blockade& Blockade::operator=(const Blockade& bl) {
	Orders::operator=(bl);
	targetTerritory = bl.targetTerritory;
	return *this;
}

// Copy Constructor
Blockade::Blockade(const Blockade& bl) :Orders(bl) {

	this->targetTerritory = bl.targetTerritory;

}

Blockade::~Blockade() {

	targetTerritory = nullptr;
	
	player = nullptr;
}



std::ostream& Blockade::printAction(std::ostream& out) const {
	out << "Blockade order.";
	if (isExecuted) {
		out << " This order was executed, its effect was [Blockade enemy].";
	}
	return out;
}
std::ostream& operator<<(std::ostream& out, const Blockade& toOutput) {
	return toOutput.printAction(out);
}
//Target territory should belong to the player
bool Blockade::validate() {

	vector<Territory*> playerTerr = player->getPlayerTerritories();
	bool territoryIsInList = std::find(playerTerr.begin(), playerTerr.end(), targetTerritory)
		!= playerTerr.end();

	return territoryIsInList;
}

// Doubles the army on the target territory and transfers its ownership to a Neutral player
void Blockade::execute() {

	if (validate()) {


		cout << " Before: " << targetTerritory->getName() << " had " << targetTerritory->getArmyCount() << endl;

		//doubling the number of armies in the target territory
		int currentArmy = targetTerritory->getArmyCount();
		targetTerritory->setArmyCount(currentArmy * 2);

		//remove theh target territory from player's territories
		player->removeTerritory(targetTerritory);
		Player* neutralPlayer = nullptr;

		//Attempt to find a neutral player
		for (Player* p : *player->getGameEngine()->getPlayers()) {
			if (p->getStrategy() == "Neutral") {
				neutralPlayer = p;
				break;
			}
		}

		//If there isnt a neutral player, make one!
		if (!neutralPlayer) {
			neutralPlayer = new Player("Neutral-" + to_string(player->getGameEngine()->getPlayers()->size()), player->getGameEngine());
			neutralPlayer->getGameEngine()->addPlayer(neutralPlayer);
			neutralPlayer->setStrategy("Neutral");
		}

		//move the ownership of the target territory to a nuetral player
		neutralPlayer->addTerritory(targetTerritory);
		targetTerritory->setOwningPlayer(neutralPlayer);
		isExecuted = true;
		
		cout << " Now: " << targetTerritory->getName() << " has " << targetTerritory->getArmyCount() << endl;
		cout << " It now belongs to " << targetTerritory->getOwningPlayer()->getPlayerName()<<endl;
		Notify(this);

	}
	else
		cout << " Order cannot be executed as it is not valid...\n";
}



string Blockade::stringToLog()
{
	return player->getPlayerName() + " blockaded " + targetTerritory->getName() + ", it now has " + to_string(targetTerritory->getArmyCount()) + " units and belongs to no one.";
}

//-----------------------------Airlift order--------------------------------------
//moves certain number of armies from 
//one of player's territories to another one(territories dont need to be adjacent)
//--------------------------------------------------------------------------------

Airlift::Airlift() :Orders(), sourceTerritory(), targetTerritory() {
	army = 0;
	set_order_id("airlift order");
	cout << "Airlift order iscreated" << endl;
}

Airlift::Airlift(Player* player, Territory* sourceTerritory, Territory* targetTerritory, int armyNum)
	: Orders(player) {
	set_order_id ("airlift");
	this->army = armyNum;
	this->sourceTerritory = sourceTerritory;
	this->targetTerritory = targetTerritory;

	cout << "Airlift Order created..\n";
}

//Assignment operator
Airlift& Airlift::operator=(const Airlift& a) {
	Orders::operator=(a);
	army = a.army;
	sourceTerritory = a.sourceTerritory;
	targetTerritory = a.targetTerritory;
	return *this;
}

// Copy Constructor
Airlift::Airlift(const Airlift& a) :Orders(a) {
	this->army = a.army;
	this->sourceTerritory = a.sourceTerritory;
	this->targetTerritory = a.targetTerritory;

}

Airlift::~Airlift() {
	
	targetTerritory = nullptr;

	sourceTerritory = nullptr;
	
	player = nullptr;
}

//both source territory and target territory should belong to the player
bool Airlift::validate() {

	vector<Territory*> playerTerr = player->getPlayerTerritories();
	bool sourceTerrIsInList = std::find(playerTerr.begin(), playerTerr.end(), sourceTerritory)
		!= playerTerr.end();

	bool targetTerrIsInList = std::find(playerTerr.begin(), playerTerr.end(), targetTerritory)
		!= playerTerr.end();

	return(sourceTerrIsInList && targetTerrIsInList);
}

//The selected number of armies will move from the source territory to the target territory
void Airlift::execute() {
	if (validate()) {

		cout << sourceTerritory->getName() << " had " << sourceTerritory->getArmyCount() << " armies, and " << targetTerritory->getName() << " had " << targetTerritory->getArmyCount() << " armies.\n ";
		int armyToMove = army;

		// If the armies available in the source territory are less than the number 
		//of armies the player wants to move, then all armies in the source territory will be mived
		if (sourceTerritory->getArmyCount() < army)
			armyToMove = sourceTerritory->getArmyCount();

		sourceTerritory->setArmyCount((sourceTerritory->getArmyCount()) - armyToMove);
		targetTerritory->setArmyCount((targetTerritory->getArmyCount()) + armyToMove);

		cout << armyToMove << " armies moved.\n";
		cout << sourceTerritory->getName() << " now has " << sourceTerritory->getArmyCount() << " armies, and " << targetTerritory->getName() << " has " << targetTerritory->getArmyCount() << " armies.\n ";

		isExecuted = true;

		Notify(this);
	}
	else
		cout << "The airlift order is not valid to be executed\n";
}


std::ostream& Airlift::printAction(std::ostream& out) const {
	out << "Airlift order.";
	if (isExecuted) {
		out << " This order was executed, its effect was [Airlift enemy].";
	}
	return out;
}
std::ostream& operator<<(std::ostream& out, const Airlift& toOutput) {
	return toOutput.printAction(out);
}


string Airlift::stringToLog()
{
	return player->getPlayerName() + " airlifted " + to_string(army) + " unit(s) from " + sourceTerritory->getName() + " to " + targetTerritory->getName() + ".";
}

//------------------------------Negotiate order--------------------------------
// Results in the target player and the current player to not to be 
//able to attack each other until this turn ends
//-----------------------------------------------------------------------------


Negotiate::Negotiate() :Orders(), opponent() {
	set_order_id("negotiate");
	cout << "Negotiate order created" << endl;	
}

Negotiate::Negotiate(Player* player, Player* opponent) : Orders(player) {
	this->opponent = opponent;
	set_order_id("negotiate");

	cout << "Negotiate Order created..\n";
}

//Assignment operator
Negotiate& Negotiate::operator=(const Negotiate& n) {
	Orders::operator=(n);
	opponent = n.opponent;
	return *this;
}

// Copy Constructor
Negotiate::Negotiate(const Negotiate& n) :Orders(n) {
	this->opponent = n.opponent;
}

Negotiate::~Negotiate() {
	
	opponent = nullptr;

	player = nullptr;
}

//Target player shouldn't be the player curruntly playing
bool Negotiate::validate() {

	if (player == opponent)
		return false;
	else
		return true;
}


void Negotiate::execute() {

	if (validate()) {

		player->setPeaceWith(opponent );
		opponent->setPeaceWith(player);


		isExecuted = true;
		cout << player->getPlayerName() << " negotiated successfuly with " << opponent->getPlayerName()<<endl;
		Notify(this);
	}
	else
		cout << " Not valit to Negotiate.\n";
}

std::ostream& Negotiate::printAction(std::ostream& out) const {
	out << "Negotitaet order.";
	if (isExecuted) {
		out << " This order was executed, its effect was [Negotiate enemy].";
	}
	return out;
}
std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput) {
	return toOutput.printAction(out);
}


string Negotiate::stringToLog()
{
	return player->getPlayerName() + " negotiated peace with " + opponent->getPlayerName() + " for the rest of the round.";
}


//****************************************************
// Definitions for OrderList class
//****************************************************


// The default constructor to make the first orderlist by defaul
OrderList::OrderList() {
}

//Assignment operator
OrderList& OrderList::operator=(const OrderList& theList) {
	this->orderList_vec = theList.orderList_vec;
	return *this;
}

// Copy Constructor
OrderList::OrderList(const OrderList& theList) {
	//this->orderList_vec = theList.orderList_vec;
	for (auto it = theList.orderList_vec.begin(); it != theList.orderList_vec.end(); ++it) {
		orderList_vec.push_back(*it);
	}
	//copy(theList.orderList_vec->begin(), theList.orderList_vec->end(), back_inserter(this->orderList_vec));

}

OrderList::~OrderList() {
	for (auto order : orderList_vec) {
		delete order;
	}

	orderList_vec.clear();
	//delete orderList_vec;
	//orderList_vec = nullptr;

}

//The function to add an order to the order list
// Since this funaction may be called many times and is effected by the user input, a pointer is created instead of making a copy of the order passes to it
void OrderList::addOrder(Orders* theOrder) {
	orderList_vec.push_back(theOrder);

	Notify(this);
}

//The function to access the orderlist
vector<Orders*> OrderList::get_orderList() {
	return orderList_vec;
}

//The function for deleting an order from the orderlist
void OrderList::delet_order(Orders* theOrder) {
	for (int i = 0; i < orderList_vec.size(); i++)
	{
		if ((orderList_vec.at(i)->get_order_id()) == (theOrder->get_order_id()))
		{
			string toDelete = theOrder->get_order_id();

			orderList_vec.erase(orderList_vec.begin() + i);
			cout << " The order \'" << toDelete << "\' was deleted." << endl;
			delete theOrder;
			return;
		}
	}
}

//The function for moving orders in the orderlist
void OrderList::move_order(int curruntPosition, int newPosition) {

	int orderListSize = orderList_vec.size();

	if (curruntPosition >= 0 && newPosition >= 0 && curruntPosition < orderListSize && newPosition < orderListSize)
	{


		if (curruntPosition < newPosition)
		{
			orderList_vec.insert(orderList_vec.begin() + newPosition + 1, orderList_vec.at(curruntPosition));
			orderList_vec.erase(orderList_vec.begin() + curruntPosition);
		}
		else if (curruntPosition > newPosition)
		{
			orderList_vec.insert(orderList_vec.begin() + newPosition, orderList_vec.at(curruntPosition));
			orderList_vec.erase(orderList_vec.begin() + curruntPosition + 1);
		}

	}
	else if (newPosition == orderListSize)
	{
		orderList_vec.push_back(orderList_vec.at(curruntPosition));
		orderList_vec.erase(orderList_vec.begin() + curruntPosition);
	}
	else
	{
		cout << "The position you chose is not valid" << endl;
	}
}

string OrderList::stringToLog()
{
	Orders* latestOrder = orderList_vec.back();
	string playerName = latestOrder->getPlayerOfOrder()->getPlayerName();
	return playerName + " added a new " + latestOrder->get_order_id() + " to their order list.";
}
