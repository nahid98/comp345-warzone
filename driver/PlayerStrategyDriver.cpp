//-----------------------------------------------------------------------------
// In order to be able to demonstrate player strategies simply and clearly, 
// dummy player,gameEngine, territories,and continents were made 
//----------------------------------------------------------------------------

#include <iostream>
#include <time.h>
#include "headers/Map.h"
#include "headers/Player.h"
#include "headers/Orders.h"
#include "headers/Cards.h"
#include "headers/CommandProcessing.h"
#include "headers/GameEngine.h"
#include "headers/LoggingObserver.h"
#include "headers/PlayerStrategy.h"

#include <random>
#include <vector>

using namespace std;
using std::cout;
using std::cin;




	

int PlayerStrategyDriver() {


	//Creating a game engin
	GameEngine* engine = new GameEngine();


	//Creating 3 players
	Player p1 = Player("p1",engine);
	Player p2 =  Player("p2",engine);
	Player p3 = Player("p3",engine);
	Player p4 = Player("p4", engine);

		
	//creating 3 different simple continents
	Continent* c1= new Continent( 1, "A", 1);
	Continent* c2= new Continent( 2, "B", 1);
	Continent* c3= new Continent( 3, "C", 1);

	//creating 6 different territories and adding them to the continents
	Territory* t1 = new Territory(1, "t1", 1);
	Territory* t2 = new Territory(2, "t2", 2);
	Territory* t3 = new Territory(3, "t3", 2);
	Territory* t4 = new Territory(4, "t4", 3);
	Territory* t5 = new Territory(5, "t5", 3);
	Territory* t6 = new Territory(6, "t6", 3);
	Territory* t7 = new Territory(6, "t7", 3);
		
	//Setting adjacencies
	t1->addAdjacentTerritory(t2);
	t2->addAdjacentTerritory(t1);

	t1->addAdjacentTerritory(t6);
	t6->addAdjacentTerritory(t1);

	t2->addAdjacentTerritory(t3);
	t3->addAdjacentTerritory(t2);

	t3 ->addAdjacentTerritory(t4);
	t4->addAdjacentTerritory(t3);

	t4 ->addAdjacentTerritory(t5);
	t5->addAdjacentTerritory(t4);

	t5 ->addAdjacentTerritory(t6);
	t6->addAdjacentTerritory(t5);

	t6->addAdjacentTerritory(t7);
	t7->addAdjacentTerritory(t6);

	t7->addAdjacentTerritory(t1);
	t1->addAdjacentTerritory(t7);
		

	//setting an army unit of 5 for each territory
	t1->setArmyCount(5);
	t2->setArmyCount(5);
	t3->setArmyCount(3);
	t4->setArmyCount(5);
	t5->setArmyCount(5);
	t6->setArmyCount(1);
	t7->setArmyCount(5);

		
	//giving territories to the players
	p1.addTerritory(t1);
	p1.addTerritory(t2);
	p2.addTerritory(t3);
	p2.addTerritory(t4);
	p3.addTerritory(t5);
	p3.addTerritory(t6);
	p4.addTerritory(t7);
		
		
	//Creating pointers to the players
	Player* player1 = &p1;
	Player* player2 = &p2;
	Player* player3 = &p3;
	Player* player4 = &p4;

	t1->setOwningPlayer(player1);
	t2->setOwningPlayer(player1);
	t3->setOwningPlayer(player2);
	t4->setOwningPlayer(player2);
	t5->setOwningPlayer(player3);
	t6->setOwningPlayer(player3);
	t7->setOwningPlayer(player4);



	engine->addPlayer(player1);
	engine->addPlayer(player2);
	engine->addPlayer(player3);
	engine->addPlayer(player4);

	Map* map = new Map({ t1,t2,t3,t4,t5,t6,t7 }, { c1,c2,c3 });

	engine->setMap(map);
		
	//--------------------------------------------------
	//              HumanPlayerStrategy
	//--------------------------------------------------
	cout << "\n--------------------------------------------------" << endl;
	cout << "            Human Player Strategy" << endl;
	cout << "--------------------------------------------------" << endl;
		
	//Demonstarting human player strategy using player1
	player1->setReinforcementPool(5);

	Cards* bombCard = new Cards("bomb");

	player1->getPlayerHand()->addToHand(bombCard);

	player1->setStrategy("Human");
	player1->issueOrder();
		
		
	//--------------------------------------------------
	//              AggressivePlayerStrategy
	//--------------------------------------------------
	cout << "\n--------------------------------------------------" << endl;
	cout << "            Aggressive Player Strategy" << endl;
	cout << "--------------------------------------------------" << endl;

	//Demonstarting aggressive player strategy using player1
	player2->setReinforcementPool(2);

	player2->setStrategy("Aggressive");

	cout << "*** Displaying player2's territories and its armies before issuing order:\n";
	for (int i = 0; i < player2->getPlayerTerritories().size(); i++)
		cout << "-   " << player2->getPlayerTerritories().at(i)->getName() << " : " << player2->getPlayerTerritories().at(i)->getArmyCount() << endl;

	player2->issueOrder();

	cout << "*** The result of the agressive player's orders :\n";
	//vector<Orders*> p2Orders = player2->getPlayerOrders()->get_orderList();

	for (int i = 0; i < player2->getPlayerOrders()->get_orderList().size(); i++){
		cout << player2->getPlayerOrders()->get_orderList()[i]->get_order_id()<<" :\n";
		player2->getPlayerOrders()->get_orderList()[i]->execute();
		cout << endl;
		}


		
	//--------------------------------------------------
	//              BenevolentPlayerStrategy
	//--------------------------------------------------
	cout << "\n--------------------------------------------------" << endl;
	cout << "            Benevolent Player Strategy" << endl;
	cout << "--------------------------------------------------" << endl;
		

	player3->setReinforcementPool(2);

	player3->setStrategy("Benevolent");

	cout << "*** Displaying player3's territories and its armies before issuing order:\n";
	for (int i = 0; i < player3->getPlayerTerritories().size(); i++)
		cout << "-   " << player3->getPlayerTerritories().at(i)->getName() << " : " << player3->getPlayerTerritories().at(i)->getArmyCount() << endl;

	player3->issueOrder();


	cout << "********** The result of the benevolent player's orders :\n";

	for (int i = 0; i < player3->getPlayerOrders()->get_orderList().size(); i++)
		player3->getPlayerOrders()->get_orderList()[i]->execute();
		

	//--------------------------------------------------
	//              NeutralPlayerStrategy
	//--------------------------------------------------
	cout << "\n--------------------------------------------------" << endl;
	cout << "            Neutral Player Strategy" << endl;
	cout << "--------------------------------------------------" << endl;


	player4->setStrategy("Neutral");

	player4->issueOrder();

	cout << "********** The result of the Neutral player's orders :\n";

	for (int i = 0; i < player4->getPlayerOrders()->get_orderList().size(); i++)
		player4->getPlayerOrders()->get_orderList()[i]->execute();

	cout << endl;

	//--------------------------------------------------
	//              CheaterPlayerStrategy
	//--------------------------------------------------
	cout << "\n--------------------------------------------------" << endl;
	cout << "            Cheater Player Strategy" << endl;
	cout << "--------------------------------------------------" << endl;

	player4->setStrategy("Cheater");

	//To demonstrate that startegy can change dynamically, we set other players to be neutral
	player1->setStrategy("Neutral");
	player2->setStrategy("Neutral");
	player3->setStrategy("Neutral");


	cout << "********** The players strategies before cheater's action :\n";
	cout << " -    Player 1 : " << player1->getStrategy();
	cout << " -    Player 2 : " << player2->getStrategy();
	cout << " -    Player 3 : " << player3->getStrategy();
	cout << " -    Player 4 : " << player4->getStrategy();
	cout << "\n";

	cout << "********** Cheater's territories BEFORE cheating :\n";
		
	for (int i = 0; i < player4->getPlayerTerritories().size(); i++)
		cout << "- " << player4->getPlayerTerritories().at(i)->getName() << endl;

	//player3 cheating
	player4->issueOrder();

	cout << "********** Cheater's territories AFTER cheating :\n";
		
	for (int i = 0; i < player4->getPlayerTerritories().size(); i++)
		cout << "- " << player4->getPlayerTerritories().at(i)->getName() << endl;

	cout << "********** The players strategies After cheater's action :\n";
	cout << " -    Player 1 : " << player1->getStrategy();
	cout << " -    Player 2 : " << player2->getStrategy();
	cout << " -    Player 3 : " << player3->getStrategy();
	cout << " -    Player 4 : " << player4->getStrategy();
	cout << endl;
		
	return 0;
}
