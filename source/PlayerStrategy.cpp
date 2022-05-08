#include "headers/PlayerStrategy.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "headers/Player.h"
#include "headers/Map.h"
#include "headers/Orders.h"
#include "headers/Cards.h"
#include "headers/GameEngine.h"
using namespace std;
using std::cout;
using std::cin;


//*********************************************
// PlayerStrategy
//*********************************************

//cunctructors
PlayerStrategy::PlayerStrategy() {
	player = nullptr;
}
PlayerStrategy::PlayerStrategy(Player* p) {
	player=p;
}

//Destructor
PlayerStrategy:: ~PlayerStrategy() {
	player = nullptr;
}

//copy cunstructor
PlayerStrategy::PlayerStrategy(const PlayerStrategy& p) {
	player = p.player;
}

//Assignment operator
PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& p) {
	this->player = p.player;

	return *this;
}

// operator << overload
std::ostream& operator<<(std::ostream& out, PlayerStrategy& ps) {

	out<< " **Player Strategy**" << endl;
	return out;
}


//*********************************************
//          Human Player
//*********************************************
// Requires user interaction to make descision


//Cunstructors
HumanPlayerStrategy::HumanPlayerStrategy() {
	player = nullptr;
}

HumanPlayerStrategy::HumanPlayerStrategy(Player* p) {
	player = p;
}

//Destructor
HumanPlayerStrategy::~HumanPlayerStrategy() {

	player = nullptr;
}

//copy cunstructor
HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& hp) {
	player = hp.player;
}

//Assignment operator
HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& hp) {
	this->player = hp.player;

	return *this;
}



// Displays the list of player's territory and let the player choose the territories to defend
std::vector<Territory*> HumanPlayerStrategy::toDefend() {

	//Territories to defend
	std::vector<Territory*> defend;

	//Displaying the name of own territories
	std::vector<Territory*> ownedTerritories;
	ownedTerritories = player->getPlayerTerritories();

	cout << "\n This are the territories you own: \n";
	for (int i = 0; i < ownedTerritories.size(); i++) {
		cout<< i <<" - "<< ownedTerritories.at(i)->getName() << endl;
	}


	// user descision to choose more territories to defend
	string toContinue = "y";

	do {

		//Ask the player to choose the territories he/she wish to defend
		cout << "\n Enter the number on the left side of the territory you wish to defend:";

		int userInput;
		cin >> userInput;

		//Making sure the user input is valid
		if (userInput > ownedTerritories.size()) {
			cout << "\n not valid.... Please enter another number:  ";
			continue;
		}

		//To avoid duplicates: Checking if the territory is already in the defend list
		bool isInList = std::find(defend.begin(), defend.end(), ownedTerritories.at(userInput))
			!= defend.end();

		//Adding the chosen territpry to the defend list if not already added
		if(!isInList)
			defend.push_back(ownedTerritories.at(userInput));

		//Ask if the user wish to add more territories to the defend list
		if (ownedTerritories.size() > defend.size()) {
			cout << "\n Want to add more? (enter \'y\' for yes): ";
			cin >> toContinue;
		}
		else {
			toContinue = "n";
		}

	}while(toContinue=="y");


	//Returning the defend list that the user has chosen
	return defend;

	}



// Displays the list of player's territory and let the player choose the territories to attack
std::vector<Territory*> HumanPlayerStrategy::toAttack() {

	//getting the territories the player owns
	std::vector<Territory*> ownedTerritories;
	ownedTerritories = player->getPlayerTerritories();

	//list of territories to that can be attacked
	std::vector<Territory*> canAttack;

	//list of territories to attack
	std::vector<Territory*> attack;

	
	//making a list of territories that can be attacked
	for (int i = 0; i < ownedTerritories.size(); i++) {

			//finding adjacent territtories to each of the player's territories that doesn't belong to this player
			std::vector<Territory*> adjList = ownedTerritories.at(i)->getAdjacencies();
			for (int j = 0; j < adjList.size(); j++) {
				Territory* adjTerr = adjList[j];
				Player* adjOwner = (*adjTerr).getOwningPlayer();
				if (adjOwner->getPlayerName() != this->player->getPlayerName()) {
					canAttack.push_back(adjTerr);
				}
			}
		}

	//Displaying the canAttack list
	cout << " This are the territories you can attack: \n";
	for (int i = 0; i < canAttack.size(); i++) {
		cout << i << " - " << canAttack.at(i)->getName() << endl;
	}
	
	//user descision to choose more territories to attack
	string toContinue = "y";

	do {

		//Ask the player to choose the territories he/she wish to attack
		cout << "\n Enter the number on the left side of the territory you wish to attack:";

		int userInput;
		cin >> userInput;

		//Making sure the user input is valid
		if (userInput > canAttack.size()) {
			cout << "\n not valid.... Please enter another number:  ";
			continue;
		}

		//To avoid duplicates: Checking if the territory is already in the defend list
		bool isInList = std::find(attack.begin(), attack.end(), canAttack.at(userInput))
			!= attack.end();

		//Adding the chosen territpry to the defend list if not already added
		if (!isInList)
			attack.push_back(canAttack.at(userInput));

		//Ask if the user wish to add more territories to the defend list
		if(canAttack.size()>attack.size()){
			cout << "\n Want to add more? (enter \'y\' for yes) :";
			cin >> toContinue;
		}
		else {
			toContinue = "n";
		}

	} while (toContinue == "y");


	return attack;

	}



void HumanPlayerStrategy::issueOrder() {

	//Gettign the defend and attack territories
	vector<Territory*> attackList = toAttack();
	vector<Territory*> defendList = toDefend();

	// To store user input ( name of the territory)
	string targetTerrName;
	string sourceTerrName = "";

	//To check user input
	bool targetIsValid = false;
	bool sourceIsValid = false;

	//To check card availabilty to creat some orders
	bool cardIsValid = false;

	//To check if user wishes to add more orders
	string addmore = " ";

	//Target Territory
	Territory* targetTerr = nullptr;// = new Territory();

	//Territory to advance from(source territory)
	Territory* sourceTerr = nullptr;// = new Territory();


	//Displaying both toAttack territories and toDefend Territories
	cout << " \n These are the territories : \n";
	cout << " \n *** Your\'s : \n";
	for (int i = 0; i < defendList.size(); i++) {
		cout << " - " << defendList.at(i)->getName() << endl;
	}
	cout << " \n *** Not your\'s : \n";
	for (int i = 0; i < attackList.size(); i++) {
		cout << " - " << attackList.at(i)->getName() << endl;
	}



	////////////////////////////////////////////////////////////////////////////
	//---------------- First issuing all deploy orders    --------------------//
	////////////////////////////////////////////////////////////////////////////

	
	cout << " \n First, deploying all troops:\n";

	int reinforcementPool = player->getReinforcementPool();

	//Deploying armies as long as the player has armies to deploy
	while(reinforcementPool>0){

		cout << " Reinforcement pool: " << reinforcementPool << " armies" << endl;

		//----------
		//*** Step1: choosing the territory to deploy the armies to
		//----------

		cout << " \n Choose the trritory to deploy to :\n ";

		//making sure the input is valid (Chosen territory is in defendList)
		do {
			cin >> targetTerrName;

			for (int i = 0; i < defendList.size(); i++) {
				if (defendList.at(i)->getName() == targetTerrName) {
					targetIsValid = true;
					targetTerr = defendList.at(i);
				}
			}

			if (targetIsValid == false)
				cout << "\n Not valis....Try again...\n";

		} while (targetIsValid == false);

		//----------
		//*** Step2: choosing the number of armies to be deployed
		//----------

		cout << "\nNumber of armies to deploy ?";
		int armyNum;
		cin >> armyNum;

		if (armyNum > reinforcementPool) {
				armyNum = reinforcementPool;
				reinforcementPool = 0;
		}
		else {
			reinforcementPool = reinforcementPool - armyNum;
		}
			
		//---------
		//*** Step3: Creating the Deploy order and adding it to teh orderlist
		//---------

		player->getPlayerOrders()->addOrder(new Deploy(this->player, targetTerr, armyNum));

	}



	////////////////////////////////////////////////////////////////////////////
	//                Adding other orders to the orderList                    //
	////////////////////////////////////////////////////////////////////////////

	
	//getting the territories the player owns
	std::vector<Territory*> ownedTerritories;
	ownedTerritories = player->getPlayerTerritories();


		do {


			// Asking user to enter the name of the orders he/she wants to add to their orderList
			cout << " \n Avalable orders:\n";
			cout << " \n   -   Advance   -   Airlift   -   Bomb   -   Blockade   -   Negotiate   -\n";
			cout << "\n Enter the name of the order you wish to add to your list: ";

			//user's chosen order
			string toAdd = "";
			cin >> toAdd;


		//////////////////-----Advance order (3 Steps)-----///////////////////////


			if (toAdd == "advance") {

				//setting isValid to false for both source and target territories
				targetIsValid = false;
				sourceIsValid = false;

				//----------
				//*** Step1: choosing the source & target territory to advance the armies
				//----------

				cout << "\n Territory to advance from (source): ";
				cin >> sourceTerrName;
				cout << "\n Territory to advance to (target): ";
				cin >> targetTerrName;

				//Making sure the choices are valid
				for (int i = 0; i < player->getPlayerTerritories().size(); i++) {
					if (ownedTerritories.at(i)->getName() == sourceTerrName) {
						sourceTerr = ownedTerritories.at(i);
						sourceIsValid = true;
					}
				}
				for (int i = 0; i < defendList.size(); i++) {
					if (defendList.at(i)->getName() == targetTerrName) {
						targetTerr = defendList.at(i);
						targetIsValid = true;
					}
				}
				for (int i = 0; i < attackList.size(); i++) {
					if (attackList.at(i)->getName() == targetTerrName) {
						targetTerr = attackList.at(i);
						targetIsValid = true;
					}
				}

				//----------
				//*** Step2: choosing the number of armies to be advanced
				//----------

				cout << "\nNumber of armies to advance with ?";
				int armyNum;
				cin >> armyNum;

				//----------
				//*** Step3: Creating the Deploy order and adding it to teh orderlist
				//----------
				
				if (sourceIsValid && targetIsValid) {

					player->getPlayerOrders()->addOrder(new Advance(this->player, sourceTerr, targetTerr, armyNum));

				}
		}


		//////////////////-----Airlift order (3 Steps)-----///////////////////////


		else if (toAdd == "airlift") {

			//setting isValid to false for both source and target territories
			targetIsValid = false;
			sourceIsValid = false;
			cardIsValid = false;

			//----------
			//*** Step1: choosing the source & target territory to advance the armies
			//----------

			cout << "\n Territory to Airlift from (source): ";
			cin >> sourceTerrName;
			cout << "\n Territory to Airlift to (target): ";
			cin >> targetTerrName;

			//Making sure the choices are valid
			
			for (int i = 0; i < defendList.size(); i++) {
				if (defendList.at(i)->getName() == targetTerrName) {
					targetTerr = defendList.at(i);
					targetIsValid = true;
				}
			}
			for (int i = 0; i < defendList.size(); i++) {
				if (defendList.at(i)->getName() == targetTerrName) {
					targetTerr = defendList.at(i);
					targetIsValid = true;
				}
			}

			//----------
			//*** Step2: choosing the number of armies
			//----------

			cout << "\nNumber of armies to airlift ?";
			int armyNum;
			cin >> armyNum;

			//----------
			//*** Step3: Creating the Airlift order and adding it to teh orderlist
			//----------

			//Only able to creat the order when "Airlift card" played
			for (int i = 0; i < player->getPlayerHand()->getNumOfCardsInHand(); i++) {
				Cards* c = player->getPlayerHand()->getCardAt(i);
				switch (c->getCardType()) {
				case Cards::airlift:
					cardIsValid = true;
					player->getPlayerHand()->removeFromHand(i);
					player->getGameEngine()->getDeck()->returnToDeck(c);
					break;
				}
			}

			if (cardIsValid == true && sourceIsValid && targetIsValid)
				player->getPlayerOrders()->addOrder(new Airlift(this->player, sourceTerr, targetTerr, armyNum));
			else
				cout << "\n Should play airlift card for this.....no airlift card detected.\n";

		}


		//////////////////-----Bomb order ( 2 Steps)-----///////////////////////


		else if (toAdd == "bomb") {

			//setting isValid to false for source and target territories, and card availability
			targetIsValid = false;
			sourceIsValid = false;
			cardIsValid = false;

			//----------
			//*** Step1: choosing the source & target territories
			//----------

			cout << "\n Territory to Bomb from (source): ";
			cin >> sourceTerrName;
			cout << "\n Territory to Bomb to (target): ";
			cin >> targetTerrName;

			//Making sure the choices are valid
			for (int i = 0; i < player->getPlayerTerritories().size(); i++) {
				if (ownedTerritories.at(i)->getName() == sourceTerrName) {
					sourceTerr = ownedTerritories.at(i);
					sourceIsValid = true;
				}
			}
			for (int i = 0; i < defendList.size(); i++) {
				if (defendList.at(i)->getName() == targetTerrName) {
					targetTerr = defendList.at(i);
					targetIsValid = true;
				}
			}
			for (int i = 0; i < attackList.size(); i++) {
				if (attackList.at(i)->getName() == targetTerrName) {
					targetTerr = attackList.at(i);
					targetIsValid = true;
				}
			}

			//----------
			//*** Step2: Creating the Bomb order and adding it to teh orderlist
			//----------

			//Only able to creat the order when "Bomb card" played
			for (int i = 0; i < player->getPlayerHand()->getNumOfCardsInHand(); i++) {
				Cards* c = player->getPlayerHand()->getCardAt(i);
				switch (c->getCardType()) {
				case Cards::bomb:
					cardIsValid = true;
					player->getPlayerHand()->removeFromHand(i);
					player->getGameEngine()->getDeck()->returnToDeck(c);
					break;
				}
			}

			if (cardIsValid == true && sourceIsValid && targetIsValid)//-----------------------------------------@nahid
				player->getPlayerOrders()->addOrder(new Bomb(this->player, sourceTerr, targetTerr));

		}


		//////////////////-----Blockade order ( 2 Steps)-----///////////////////////


		else if (toAdd == "blockade") {

			//setting isValid to false for target territories and card avalability
			targetIsValid = false;
			cardIsValid = false;
			

			//----------
			//*** Step1: choosing the target territory to blockade
			//----------

			cout << "\n Territory for Blockade (target): ";
			cin >> targetTerrName;


			//Making sure the choices are valid
			for (int i = 0; i < defendList.size(); i++) {
				if (defendList.at(i)->getName() == targetTerrName) {
					targetTerr = defendList.at(i);
					targetIsValid = true;
				}
			}
			

			//----------
			//*** Step2: Creating the Blockade order and adding it to teh orderlist
			//----------

			//Only able to creat the order when "Blockade card" played
			for (int i = 0; i < player->getPlayerHand()->getNumOfCardsInHand(); i++) {
				Cards* c = player->getPlayerHand()->getCardAt(i);
				switch (c->getCardType()) {
				case Cards::blockade:
					cardIsValid = true;
					player->getPlayerHand()->removeFromHand(i);
					player->getGameEngine()->getDeck()->returnToDeck(c);
					break;
				}
			}

			if (cardIsValid == true && targetIsValid)
				player->getPlayerOrders()->addOrder(new Blockade(this->player, targetTerr));

		}


		///////////////////////-----Negotiate order ( 2 Steps)-----///////////////////////


		else if (toAdd == "negotiate") {

			//setting card avalability to false
			cardIsValid = false;

			//name of opponent player
			string otherPlayer = " ";

			//----------
			//*** Step1: choosing the player to negotiate with
			//----------

			//execute methose in negotiate class is based on player's names
			cout << "\n Name of the player to negotiate with : ";
			cin >> otherPlayer;

			//----------
			//*** Step2: Creating the Negotiate order and adding it to teh orderlist
			//----------

			//Only able to create the order when "Blockade card" played
			for (int i = 0; i < player->getPlayerHand()->getNumOfCardsInHand(); i++) {
				Cards* c = player->getPlayerHand()->getCardAt(i);
				switch (c->getCardType()) {
				case Cards::diplomacy:
					cardIsValid = true;
					player->getPlayerHand()->removeFromHand(i);
					player->getGameEngine()->getDeck()->returnToDeck(c);
					break;
				}
			}

			//finding the other player
			if (cardIsValid == true)
				for(int i=0;i<player->getGameEngine()->getPlayers()->size();i++)
					if(player->getGameEngine()->getPlayers()->at(i)->getPlayerName()==otherPlayer)
						player->getPlayerOrders()->addOrder(new Negotiate(this->player, player->getGameEngine()->getPlayers()->at(i)));

		}


		cout << " \n Add more orders ? (\'y\' for yes):";
		cin >> addmore;

	}while (addmore == "y");

}


// operator << overload
	std::ostream& operator<<(std::ostream & out, HumanPlayerStrategy & hps) {

		out << "\n Human Player Strategy. \n";
		return out;
	}


//*********************************************
//          Aggressive Player
//*********************************************

//Cunstructors
AggressivePlayerStrategy::AggressivePlayerStrategy() {
	player = nullptr;
	}
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* p) {
	player = p;
	}

//Destructor
AggressivePlayerStrategy::~AggressivePlayerStrategy() {
	player = nullptr;
}

//copy cunstructor
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy& ap) {
	player = ap.player;
}

//Assignment operator
AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy& ap) {
	this->player = ap.player;

	return *this;
}

//************** To Defend funtion  *****************


//Sorting the player's territory list based on the stronger 
std::vector<Territory*> AggressivePlayerStrategy::toDefend() {

	//A copy of the player's territory list
	std::vector<Territory*> copyList = player->getPlayerTerritories();

	//The defend list of the player's territory 
	std::vector<Territory*> defend;// = new std::vector<Territory*>;

	//To store the index of the territory with the max number of armies
	int max_index = 0;

	//Kepp track of the territories that are added to the defend list so far
	vector<int> listOdAddedMax = vector<int>();
	bool alreadyAddded = true;

	//Sorting based in number of armies
	for (int i = 0; i < copyList.size();i++) {
		
			for (int j = 0; j < copyList.size()-1; j++) {                               
				if (copyList.at(j)->getArmyCount() < copyList.at(j+1)->getArmyCount()) 
				{ 
					//Check if this territory is already added to the defend list
					alreadyAddded = std::find(listOdAddedMax.begin(), listOdAddedMax.end(), j+1)
					!= listOdAddedMax.end();

					if(alreadyAddded ==false)
						max_index = j+1;
				}
			}

			//making sure the territory at copyList[max_index] is not already added to the defend list (to avoid duplicates)
			alreadyAddded = std::find(listOdAddedMax.begin(), listOdAddedMax.end(), max_index)
				!= listOdAddedMax.end();

			if(alreadyAddded==false){
				defend.push_back(copyList.at(max_index));
				listOdAddedMax.push_back(max_index);
			}
		
			max_index=0;

	}

	//Cehck for any remaining territories that hasn't been added to the defend list
	for (int j = 0; j < copyList.size(); j++) {                                     
		
			alreadyAddded = std::find(listOdAddedMax.begin(), listOdAddedMax.end(), j)
				!= listOdAddedMax.end();

			if (alreadyAddded == false)
				defend.push_back(copyList.at(j));
		
	}


	return defend;

}

//************** To Attack funtion  *****************


//Player only attacks the territories that doesn't own them
std::vector<Territory*> AggressivePlayerStrategy::toAttack() {

	//getting the territories owned by this player
	std::vector<Territory*> ownedTerritories = player->getPlayerTerritories();

	//list of territories owned by enemy
	std::vector<Territory*> attack;


	//making a list of territories owned by enemy
	for (int i = 0; i < ownedTerritories.size(); i++) {

		//finding adjacent territtories to each of the player's territories that doesn't belong to this player
		std::vector<Territory*> adjList = ownedTerritories.at(i)->getAdjacencies();
		for (int j = 0; j < adjList.size(); j++) {
			Territory* adjTerr = adjList[j];
			Player* adjOwner = (*adjTerr).getOwningPlayer();
			if (adjOwner->getPlayerName() != this->player->getPlayerName()) {
				attack.push_back(adjTerr);
			}
		}
	}


	return attack;
}



//************** The issueOrder funtion  *****************


//Aggressive Player advances or deploys armies to its strongest territory and then attacks the enemy ( 3 Steps)
void AggressivePlayerStrategy::issueOrder() {


	//Gettign the defend and attack territories
	vector<Territory*> attackList = toAttack();
	vector<Territory*> defendList = toDefend();

	//Player's strongest territory
	Territory* strongestTerr;
	strongestTerr = defendList.at(0);

	//----------------
	//********** Step1: Deploy all troops to it's strongest territory
	//----------------
	int deployedToStrongest = player->getReinforcementPool();
	player->getPlayerOrders()->addOrder(new Deploy(this->player, strongestTerr, deployedToStrongest));


	//----------------
	//********** Step2: Advance armies to the strongest territory
	//----------------

	//finding all player's adjacent territories to the strongest territory
	vector<Territory*> adjList = vector<Territory*>();
	adjList = strongestTerr->getAdjacencies();

	//Move non-strongest territory units towards strongest territory!
	bool movingTowardsStrongest = false;
	for (Territory* t : defendList) {
		if (t != strongestTerr && t->getArmyCount() != 0) {
			vector<Territory*> path = player->getGameEngine()->getMap()->shortestPath(t, strongestTerr);
			player->getPlayerOrders()->addOrder(new Advance(this->player, t, path[1], t->getArmyCount()));
			movingTowardsStrongest = true;
		}
	}

	if (!movingTowardsStrongest) {
		vector<Territory*> enemyTerritoriesAdjToStrongest = strongestTerr->getAdjacencies();
		for (int i = enemyTerritoriesAdjToStrongest.size() - 1; i >= 0; i--) {

			if (enemyTerritoriesAdjToStrongest[i]->getOwningPlayer() == this->player) {
				enemyTerritoriesAdjToStrongest.erase(enemyTerritoriesAdjToStrongest.begin() + i);
			}
		}

		//No enemies immediately nearby! Start marching towards the closest enemy!
		if (enemyTerritoriesAdjToStrongest.size() == 0) {
			vector<Territory*> path = player->getGameEngine()->getMap()->shortestPath(strongestTerr, attackList);

			player->getPlayerOrders()->addOrder(new Advance(this->player, strongestTerr, path[1], strongestTerr->getArmyCount() + deployedToStrongest));
		}
	}
	

	//----------------
	//********** Step3: Attacking enemy as long as possible
	//----------------

	//finding player's territories adjacent to the attackable territories
	for (int i = 0; i < attackList.size(); i++) {

		vector<Territory*> adjToEnemy = vector<Territory*>();
		adjToEnemy = attackList.at(i)->getAdjacencies();

		//store valid source territories to randomly choose from
		vector<Territory*> randSourceTerr = vector<Territory*>();

		//choosing source territories that are owned by this player and has army 
		for (int j = 0; j < adjToEnemy.size(); j++)
			if (adjToEnemy[j]->getOwningPlayer()->getPlayerName() == player->getPlayerName())
				if(adjToEnemy[j]->getArmyCount()!=0)
					randSourceTerr.push_back(adjToEnemy[j]);


		//issuing attack advance orders if valid source territories avalable
		if(randSourceTerr.size()>0){

			//choosing a source player randomly
			srand(static_cast<unsigned int>(time(NULL)));
			int randSourceIndex;

			randSourceIndex = (rand() % randSourceTerr.size());


			//choosing a army number randomly
			srand(static_cast<unsigned int>(time(NULL)));
			int randArmyNum;

			if (randSourceTerr[randSourceIndex]->getArmyCount() == 0)
				continue;

			randArmyNum = (rand() % randSourceTerr[randSourceIndex]->getArmyCount());
			
			//isuing advance order
			player->getPlayerOrders()->addOrder(new Advance(this->player, randSourceTerr[randSourceIndex], attackList.at(i), randArmyNum));

		}

	}

	//Issue cards!
	//Aggressive player will play all types of cards except for blockade and diplomacy
	Hand* h = player->getPlayerHand();
	for (int i = h->getNumOfCardsInHand()-1; i >= 0; i--) {
		Cards* c = h->getCardAt(i);

		if (c->getCardType() == Cards::cardTypes::diplomacy || c->getCardType() == Cards::cardTypes::blockade)
			return;

		c->play(player);
	}
}

// operator << overload
std::ostream& operator<<(std::ostream& out, AggressivePlayerStrategy& aps) {
	out << "\n The Agressive Player Strategy.\n";
	return out;
}



//*********************************************
//          Benevolent Player
//*********************************************



//Cunstructors
BenevolentPlayerStrategy::BenevolentPlayerStrategy() {
	player = nullptr;
}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* p) {
	player = p;
}

//Destructor
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {
	player = nullptr;
}

//copy cunstructor
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy& bp) {
	player = bp.player;
}

//Assignment operator
BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy& bp) {
	this->player = bp.player;

	return *this;
}

//************** To Defend funtion  *****************


std::vector<Territory*> BenevolentPlayerStrategy::toDefend() {

	//A copy of the player's territory list
	std::vector<Territory*> copyList;
	copyList = player->getPlayerTerritories();

	//The defend list of the player's territory 
	std::vector<Territory*> defend;

	//To store the index of the territory with the max number of armies
	int min_index = 0;

	//Kepp track of the territories that are added to the defend list so far
	vector<int> listOfAddedMin = vector<int>();
	bool alreadyAddded = true;
	
	
	//Sorting based in number of armies
	for (int i = 0; i < copyList.size(); i++) {

		for (int j = 0; j < copyList.size() - 1; j++) {
			if (copyList.at(j)->getArmyCount() > copyList.at(j + 1)->getArmyCount())
			{
				//Check if this territory is already added to the defend list
				alreadyAddded = std::find(listOfAddedMin.begin(), listOfAddedMin.end(), j + 1)
					!= listOfAddedMin.end();

				if (alreadyAddded == false)
					min_index = j + 1;
			}
		}

		//making sure the territory at copyList[max_index] is not already added to the defend list (to avoid duplicates)
		alreadyAddded = std::find(listOfAddedMin.begin(), listOfAddedMin.end(), min_index)
			!= listOfAddedMin.end();

		if (alreadyAddded == false) {
			defend.push_back(copyList.at(min_index));
			listOfAddedMin.push_back(min_index);
		}

		min_index = 0;

	}


	//Check for any remaining territories that hasn't been added to the defend list
	for (int j = 0; j < copyList.size(); j++) {

		alreadyAddded = std::find(listOfAddedMin.begin(), listOfAddedMin.end(), j)
			!= listOfAddedMin.end();

		if (alreadyAddded == false)
			defend.push_back(copyList.at(j));

	}


	return defend;

}



//************** To Attack funtion  *****************


std::vector<Territory*> BenevolentPlayerStrategy::toAttack() {

	//getting the territories owned by this player
	std::vector<Territory*> ownedTerritories;
	ownedTerritories = player->getPlayerTerritories();

	//list of territories owned by enemy
	std::vector<Territory*> attack;// = new std::vector<Territory*>;


	//making a list of territories owned by enemy
	for (int i = 0; i < ownedTerritories.size(); i++) {

		//finding adjacent territtories to each of the player's territories that doesn't belong to this player
		std::vector<Territory*> adjList = ownedTerritories.at(i)->getAdjacencies();
		for (int j = 0; j < adjList.size(); j++) {
			Territory* adjTerr = adjList[j];
			Player* adjOwner = (*adjTerr).getOwningPlayer();
			if (adjOwner->getPlayerName() != this->player->getPlayerName()) {
				attack.push_back(adjTerr);
			}
		}
	}


	return attack;

}


//************** The issueOrder funtion  *****************



void BenevolentPlayerStrategy::issueOrder() {

	//Gettign the defend and attack territories
	vector<Territory*> attackList = toAttack();
	vector<Territory*> defendList = toDefend();


	//----------------
	//********** Step1: Deploy troops to weakest territories
	//----------------

	//Reinforcement pool
	int reinforcementPool = player->getReinforcementPool();

	//number of territories with zero armies (weakest territories)
	int vacantTerr=0;

	//counting number of territories with zero armies (weakest territories)
	for (int i = 0; i <defendList.size(); i++) {
		if (defendList.at(i)->getArmyCount() == 0)
			vacantTerr++;

		if (defendList.at(i)->getArmyCount() > 0)
			break;
	}


	// If there are vacant territories the armies will be deployed there
	if (vacantTerr > 0) {

		//In case there are less troops than the number of vacant territories
		if (reinforcementPool < vacantTerr) {

			//will be giving each vacant territory 1 army as long as possible
			for (int i = defendList.size(); i > 0; i--) {
				
				//If there is still troops to be deployed
				if (reinforcementPool > 0) {
					
					player->getPlayerOrders()->addOrder(new Deploy(this->player, defendList.at(i-1), 1));

					reinforcementPool=reinforcementPool-1;
				}
			}
		}
		//In case there are more troops than the vacant territories
		else {

			int armyNum = 0;

			//Dividing available troops amongst vacant territories
			armyNum = round(reinforcementPool / vacantTerr);

			//To make sure number of armies to be deployed is not zero
			if (armyNum == 0)
				armyNum = 1;

			//Giving the weakest territory an additional troop that the rest, to make sure all troops will be deployed 
			player->getPlayerOrders()->addOrder(new Deploy(this->player, defendList.at(0), armyNum+1));

			//Distributing troops amongst the rest vacant territories
			for (int i =0; i <defendList.size()-1; i++) {

					player->getPlayerOrders()->addOrder(new Deploy(this->player, defendList.at(i + 1), armyNum));
			}
		}
	}
	//else it will be distributed amongst 2 weakest territories
	else {

		int armyNum = 0;

		//Making sure there is at leaset two territories if players defend list
		if(defendList.size()>1){

			//Dividing available troops amongst 2 weakest territories
			armyNum = round(reinforcementPool / 2);

			//To make sure number of armies to be deployed is not zero
			if (armyNum == 0)
				armyNum = 1;

			//Index of the weakest territory
			int weakestTerrIndex = 0;

			//Deploying trrops to the 2 weakest territories
			player->getPlayerOrders()->addOrder(new Deploy(this->player, defendList.at(weakestTerrIndex), armyNum+1));
			player->getPlayerOrders()->addOrder(new Deploy(this->player, defendList.at(weakestTerrIndex+1), armyNum));
		}
		//If player has only one territory to defend
		else {


			//Dividing available troops amongst 2 weakest territories
			armyNum =reinforcementPool;

			//Deploying trrops to the 2 weakest territories
			player->getPlayerOrders()->addOrder(new Deploy(this->player, defendList.at(0), armyNum));
			
		}

	}
	


//----------------
//********** Step2: Advance troops from strongest territories to weakest territories
//----------------
	
	//army to be moved
	int armyNum = 0;

	//list of target territories (weakest territories)
	vector<Territory*> targetTerrList = defendList;

	//list of source territories (strongest)
	vector<Territory*> sourceTerrList;
	for (int i = 1; i < defendList.size()+1; i++)
		sourceTerrList.push_back(defendList.at((defendList.size()) - i));


	//Kepp track of the territories that recived army
	vector<int> gotArmy = vector<int>();
	bool alreadyAdded = true;


	//Advancing armies from each teritory of the player to the weakest adjacent territory
	for (int i = 0; i < defendList.size(); i++) {


		vector<Territory*> adjacentTerritories = sourceTerrList.at(i)->getAdjacencies();


		for (int j = 0; j < targetTerrList.size(); j++) {

			bool isAdjacent = std::find(adjacentTerritories.begin(), adjacentTerritories.end(), targetTerrList.at(j))
				!= adjacentTerritories.end();

			//In target territory is adjacent and has less army than source territory, advance order is issued
			if (isAdjacent == true && sourceTerrList.at(i)->getArmyCount() > targetTerrList.at(j)->getArmyCount()) {

				//half of the difference in number of armies of both territories will be moved
				armyNum = (sourceTerrList.at(i)->getArmyCount() - targetTerrList.at(j)->getArmyCount())/2;


				alreadyAdded = std::find(gotArmy.begin(), gotArmy.end(), j)
					!= gotArmy.end();


				if (alreadyAdded == false){

					//issuing advance order
					player->getPlayerOrders()->addOrder(new Advance(this->player, sourceTerrList.at(i), targetTerrList.at(j), armyNum));

					gotArmy.push_back(j);
				}
				

			}
		}
	}


	//Benevolent player will only play diplomacy, reinforcement, blockade, or airlift cards!
	Hand* h = player->getPlayerHand();
	for (int i = h->getNumOfCardsInHand() - 1; i >= 0; i--) {
		Cards* c = h->getCardAt(i);

		if (c->getCardType() == Cards::cardTypes::diplomacy ||
			c->getCardType() == Cards::cardTypes::reinforcement ||
			c->getCardType() == Cards::cardTypes::airlift ||
			c->getCardType() == Cards::cardTypes::blockade)
			c->play(player);
	}
}

// operator << overload
std::ostream& operator<<(std::ostream& out, BenevolentPlayerStrategy& bps) {
	out << "\n The Benevolent Player Strategy.\n";
	return out;
}




//*********************************************
//          Neutral Player
//*********************************************


//Cunstructors
NeutralPlayerStrategy::NeutralPlayerStrategy() {
	player = nullptr;
}
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* p) {
	player = p;
}

//Destructor
NeutralPlayerStrategy::~NeutralPlayerStrategy() {
	player = nullptr;
}

//copy cunstructor
NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& np) {
	player = np.player;
}

//Assignment operator
NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy& np) {
	this->player = np.player;

	return *this;
}

std::vector<Territory*> NeutralPlayerStrategy::toDefend() {
	return player->getPlayerTerritories();
}

std::vector<Territory*> NeutralPlayerStrategy::toAttack() {

	//since this player doesn't attack any territory, its attack list will be empty
	vector<Territory*> attack;// = new vector<Territory*>();

	return attack;
}

void NeutralPlayerStrategy::issueOrder() {

	//Neutral player doesn't issue any order
	cout << "\nNeutal player ----- won't issue any order\n";

	//Not even a card order!
}

// operator << overload
std::ostream& operator<<(std::ostream& out, NeutralPlayerStrategy& nps) {
	out << "\n The Neutral Player Strategy.\n";
	return out;
}


//*********************************************
//          Cheater Player
//*********************************************

//Cunstructors
CheaterPlayerStrategy::CheaterPlayerStrategy() {
	player = nullptr;
}

CheaterPlayerStrategy::CheaterPlayerStrategy(Player* p) {
	player = p;
}

//Destructor
CheaterPlayerStrategy::~CheaterPlayerStrategy() {
	player = nullptr;
}

//copy cunstructor
CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy& cp) {
	player = cp.player;
}

//Assignment operator
CheaterPlayerStrategy& CheaterPlayerStrategy::operator=(const CheaterPlayerStrategy& cp) {
	this->player = cp.player;

	return *this;
}


std::vector<Territory*> CheaterPlayerStrategy::toDefend() {
	return player->getPlayerTerritories();
}

std::vector<Territory*> CheaterPlayerStrategy::toAttack() {

	//getting the territories owned by this player
	std::vector<Territory*> ownedTerritories;// = new std::vector<Territory*>;
	ownedTerritories = player->getPlayerTerritories();

	//list of territories owned by enemy
	std::vector<Territory*> attack;// = new std::vector<Territory*>;


	//making a list of territories owned by enemy
	for (int i = 0; i < ownedTerritories.size(); i++) {

		//finding adjacent territtories to each of the player's territories that doesn't belong to this player
		std::vector<Territory*> adjList = ownedTerritories.at(i)->getAdjacencies();
		for (int j = 0; j < adjList.size(); j++) {
			Territory* adjTerr = adjList[j];
			Player* adjOwner = (*adjTerr).getOwningPlayer();
			if (adjOwner->getPlayerName() != this->player->getPlayerName()) {
				attack.push_back(adjTerr);
			}
		}
	}

	return attack;

}

void CheaterPlayerStrategy::issueOrder() {

	//cheater player automatically conquers enemy territories (once per turn)
	
	//Gettign the attack territories
	vector<Territory*> attackList = toAttack();

	//Randomly choosing a territory from enemy's territory
	srand(static_cast<unsigned int>(time(NULL)));
	int randIndex;

	randIndex = (rand() % attackList.size());

	//conquery the randomly chosen territory(transfering ownership)

	Player* enemy = attackList.at(randIndex)->getOwningPlayer();
	
	//If the player that was attcked was a neutral player, he will become aggressive
	if (enemy->getStrategy() == "Neutral")
		enemy->setStrategy("Aggressive");

	enemy->removeTerritory(attackList.at(randIndex));
	attackList.at(randIndex)->setOwningPlayer(player);
	player->addTerritory(attackList.at(randIndex));


	//Never issues a card order, just cheats!
}

// operator << overload
std::ostream& operator<<(std::ostream& out, CheaterPlayerStrategy& cps) {
	out << "\n The Cheater Player Strategy.\n";
	return out;
}


