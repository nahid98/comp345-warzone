#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"
using namespace std;



class Player;

class PlayerStrategy
{
public:

	Player* player;

	//cunctructors
	PlayerStrategy();
	PlayerStrategy(Player* p);

	//Destructor
	virtual ~PlayerStrategy();
	
	//copy cunstructor
	PlayerStrategy(const PlayerStrategy& p);

	//Assignment operator
	PlayerStrategy& operator=(const PlayerStrategy& p);

	// The three virtual functions
	virtual std::vector<Territory*> toDefend()=0;

	virtual std::vector<Territory*> toAttack()=0;

	virtual void issueOrder()=0;

	// operator << overload
	friend std::ostream& operator<<(std::ostream& stream, PlayerStrategy& ps);
};


//*********************************************
//          Human Player
//*********************************************

class HumanPlayerStrategy : public PlayerStrategy {

public:

	//Cunstructors
	HumanPlayerStrategy();
	HumanPlayerStrategy(Player* p);

	//Destructor
	~HumanPlayerStrategy();

	//copy cunstructor
	HumanPlayerStrategy(const HumanPlayerStrategy& hp);

	//Assignment operator
	HumanPlayerStrategy& operator=(const HumanPlayerStrategy& hp);


	std::vector<Territory*> toDefend();

	std::vector<Territory*> toAttack();

	void issueOrder();

	// operator << overload
	friend std::ostream& operator<<(std::ostream& stream, HumanPlayerStrategy& hps);
};



//*********************************************
//          Aggressive Player
//*********************************************

class AggressivePlayerStrategy : public PlayerStrategy {

public:

	//Cunstructors
	AggressivePlayerStrategy();
	AggressivePlayerStrategy(Player* p);

	//Destructor
	~AggressivePlayerStrategy();

	//copy cunstructor
	AggressivePlayerStrategy(const AggressivePlayerStrategy& ap);

	//Assignment operator
	AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& ap);


	std::vector<Territory*> toDefend();

	std::vector<Territory*> toAttack();

	void issueOrder();

	// operator << overload
	friend std::ostream& operator<<(std::ostream& stream, AggressivePlayerStrategy& aps);
};


//*********************************************
//          Benevolent Player
//*********************************************

class BenevolentPlayerStrategy : public PlayerStrategy {

public:

	//Cunstructors
	BenevolentPlayerStrategy();
	BenevolentPlayerStrategy(Player* p);

	//Destructor
	~BenevolentPlayerStrategy();

	//copy cunstructor
	BenevolentPlayerStrategy(const BenevolentPlayerStrategy& bp);

	//Assignment operator
	BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& bp);

	std::vector<Territory*> toDefend();

	std::vector<Territory*> toAttack();

	void issueOrder();

	// operator << overload
	friend std::ostream& operator<<(std::ostream& stream, BenevolentPlayerStrategy& bps);
};



//*********************************************
//          Neutral Player
//*********************************************

class NeutralPlayerStrategy : public PlayerStrategy {

public:

	//Cunstructors
	NeutralPlayerStrategy();
	NeutralPlayerStrategy(Player* p);

	//Destructor
	~NeutralPlayerStrategy();

	//copy cunstructor
	NeutralPlayerStrategy(const NeutralPlayerStrategy& np);

	//Assignment operator
	NeutralPlayerStrategy& operator=(const NeutralPlayerStrategy& np);

	std::vector<Territory*> toDefend();

	std::vector<Territory*> toAttack();

	void issueOrder();

	// operator << overload
	friend std::ostream& operator<<(std::ostream& stream, NeutralPlayerStrategy& nps);
};


//*********************************************
//          Cheater Player
//*********************************************

class CheaterPlayerStrategy : public PlayerStrategy {

public:

	//Cunstructors
	CheaterPlayerStrategy();
	CheaterPlayerStrategy(Player* p);

	//Destructor
	~CheaterPlayerStrategy();

	//copy cunstructor
	CheaterPlayerStrategy(const CheaterPlayerStrategy& cp);

	//Assignment operator
	CheaterPlayerStrategy& operator=(const CheaterPlayerStrategy& cp);

	std::vector<Territory*> toDefend();

	std::vector<Territory*> toAttack();

	void issueOrder();

	// operator << overload
	friend std::ostream& operator<<(std::ostream& stream, CheaterPlayerStrategy& cps);
};

