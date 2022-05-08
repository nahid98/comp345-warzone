#pragma once
#include <vector>
#include <string>
#include <iostream>

class Orders;
class Hands;
class Deck;
class Cards;
class Player;

class Cards
{
public:

	Cards(std::string cardName);

	enum cardTypes { bomb = 0, reinforcement, blockade, airlift, diplomacy };
	//constructor
	Cards(cardTypes cardType);

	//copy constructor
	Cards(const Cards& card);

	//destructor
	~Cards();

	//stream insertion operator
	friend std::ostream& operator<<(std::ostream& stream, Cards& card);

	//assignment operator
	Cards& operator=(const Cards& card);

	//utility functions
	void play(Player* p);
	void print();

	//getter
	cardTypes getCardType();

private:
	//type of card
	cardTypes cardType;
};

class Hand
{
public:
	//Constructor
	Hand();

	//Deconstructor
	~Hand();

	//copy constructor
	Hand(const Hand& hand);

	//stream insertion operator
	friend std::ostream& operator<<(std::ostream& stream, Hand& hand);

	//assignment operator
	Hand& operator= (const Hand& hand);

	//getters
	Cards* getCardAt(int cardIndex);
	int getNumOfCardsInHand();

	//utility functions
	void handPrint();
	void addToHand(Cards* card);
	void removeFromHand(int cardIndex);
	void removeFromHand(Cards* c);
	Cards* playFromHand(int cardIndex);

private:
	//vector of cards
	std::vector<Cards*> hand;
};

class Deck
{
public:
	//constructor
	Deck(int deckSize);

	//Destructor
	~Deck();

	//copy constructor
	Deck(const Deck& deck);

	//stream insertion operator
	friend std::ostream& operator<<(std::ostream& stream, Deck& deck);

	//assignment operator
	Deck& operator= (const Deck& deck);

	//utility functions
	void shuffle();
	void intializeDeck();
	void printDeck();
	void destroyTopCard();
	void draw(Hand& hand);
	Cards* topCard();
	void returnToDeck(Cards* card);

	//getters
	int cardsIndeck();

private:
	//vector of cards
	std::vector<Cards*> deck;

	//size of the deck
	int deckSize;
};
