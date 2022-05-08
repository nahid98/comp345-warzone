#ifndef ORDERS_H
#define ORDERS_H
#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "headers/LoggingObserver.h"

class Map;
class Player;
class Territory;
class Cards;
class Deck;


class Orders : public ILoggable, public Subject
{
public:

	// Default constructor
	Orders();

	//constructor to set the name of the order
	Orders(string orderName);

	//constructor to set the player
	Orders(Player* player);


	//Assignment operstor
	Orders& operator=(const Orders& theOrder);

	// Copy Constructor
	Orders(const Orders& theOrder);

	//Distructor
	virtual ~Orders();

	// the function for validating the order
	virtual bool validate() = 0;

	// the function for executing the order
	virtual void execute() = 0;

	Player* getPlayerOfOrder();


	//setting the Id of the order
	void set_order_id(string theID);

	//getting the id of the order
	string get_order_id();


	//The stream insertion operator
	friend std::ostream& operator<<(ostream& output, const Orders& order);

	virtual std::ostream& printAction(std::ostream& out) const = 0;


	virtual string stringToLog() = 0;

private:

	string orderID;    //Id of the order

protected:

	//to check if the order got executed
	bool isExecuted;
	//a pointr to the player
	Player* player;

};


//--------------------------
//The subclasses
//--------------------------


class Deploy : public Orders {

public:
	//Default constructor
	Deploy();

	//Constructor
	Deploy(Player* player, Territory* territory, int army);

	//Assignment operstor
	Deploy& operator=(const Deploy& d);

	// Copy Constructor
	Deploy(const Deploy& d);

	//Distructor
	virtual ~Deploy();

	virtual bool validate();
	virtual void execute();

	friend std::ostream& operator<<(std::ostream& out, const Deploy& toOutput);

	virtual std::ostream& printAction(std::ostream& out) const;

	virtual string stringToLog();

	//a pointer to the territory the armies are to be deployed to
	Territory* territoryToDeploy;

private:

	int army;
};


class Advance : public Orders {

public:
	//Default constructor
	Advance();

	//cunstructor
	Advance(Player* player, Territory* sourceTerritory,
		Territory* targetTerritory, int armyNum);

	//Assignment operstor
	Advance& operator=(const Advance& a);

	// Copy Constructor
	Advance(const Advance& a);

	//Distructor
	virtual ~Advance();

	virtual std::ostream& printAction(std::ostream& out) const;

	virtual bool validate();
	virtual void execute();

	virtual string stringToLog();

private:

	int army;
	Territory* sourceTerritory;
	Territory* targetTerritory;
};


class Bomb : public Orders {

public:
	//Default constructor
	Bomb();

	//constructor
	Bomb(Player* player, Territory* sourceTerritory, Territory* targetTerritory);

	//Assignment operstor
	Bomb& operator=(const Bomb& b);

	// Copy Constructor
	Bomb(const Bomb& b);

	//Distructor
	virtual ~Bomb();

	friend std::ostream& operator<<(std::ostream& out, const Bomb& toOutput);

	virtual std::ostream& printAction(std::ostream& out) const;

	virtual bool validate();
	virtual void execute();

	virtual string stringToLog();
private:

	Territory* sourceTerritory;
	Territory* targetTerritory;

};




class Blockade : public Orders {

public:
	//Default constructor
	Blockade();

	//Constructor
	Blockade(Player* player, Territory* targetTerritory);

	//Assignment operator
	Blockade& operator=(const Blockade& bl);

	//Copy Constructor
	Blockade(const Blockade& bl);

	//Distructor
	virtual ~Blockade();

	friend std::ostream& operator<<(std::ostream& out, const Blockade& toOutput);

	virtual std::ostream& printAction(std::ostream& out) const;

	virtual bool validate();
	virtual void execute();

	virtual string stringToLog();
private:
	Territory* targetTerritory;

};


class Airlift : public Orders {

public:
	//Default constructor
	Airlift();

	//Constructor
	Airlift(Player* player, Territory* sourceTerritory, Territory* targetTerritory, int armyNum);

	//Assignment operstor
	Airlift& operator=(const Airlift& a);

	// Copy Constructor
	Airlift(const Airlift& a);

	//Distructor
	virtual ~Airlift();

	friend std::ostream& operator<<(std::ostream& out, const Airlift& toOutput);

	virtual std::ostream& printAction(std::ostream& out) const;

	virtual bool validate();
	virtual void execute();

	virtual string stringToLog();
private:
	int army;
	Territory* sourceTerritory;
	Territory* targetTerritory;

};



class Negotiate : public Orders {

public:
	//Default constructor
	Negotiate();

	//Constructor
	Negotiate(Player* player, Player* opponent);

	//Assignment operstor
	Negotiate& operator=(const Negotiate& n);

	// Copy Constructor
	Negotiate(const Negotiate& n);

	//Distructor
	virtual ~Negotiate();

	friend std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput);

	virtual std::ostream& printAction(std::ostream& out) const;

	virtual bool validate();
	virtual void execute();

	virtual string stringToLog();
private:

	Player* opponent;
};

//----------------------------------
// The OrderList class
//----------------------------------

class OrderList : public ILoggable, public Subject//------------------------------@nahid
{

public:
	//constructor
	OrderList();

	//Assignment operstor
	OrderList& operator=(const OrderList& theList);

	// Copy Constructor
	OrderList(const OrderList& theList);

	//distructor
	virtual ~OrderList();

	// Add the ordrs to the order list
	void addOrder(Orders* theOrder);

	// the vector of the OrderList
	vector<Orders*> get_orderList();

	// To delet orders from the orderlist
	void delet_order(Orders* theOrder);

	//To move orders in the orderlist
	void move_order(int curruntPosition, int newPosition);

	virtual string stringToLog();

private:
	vector<Orders*> orderList_vec;
};

#endif // !ORDERS_H
