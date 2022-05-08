#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <algorithm>
#include <random>
#include "headers/Cards.h"
#include "headers/Map.h"
#include "headers/Orders.h"
#include "headers/Player.h"
#include "headers/GameEngine.h"


Cards::Cards(std::string cardName) {

	if (cardName == "bomb")
		this->cardType = cardTypes::bomb;
	else if (cardName == "reinforcement")
		this->cardType = cardTypes::reinforcement;
	else if (cardName == "blockade")
		this->cardType = cardTypes::blockade;
	else if (cardName == "airlift")
		this->cardType = cardTypes::airlift;
	else if (cardName == "diplomacy")
		this->cardType = cardTypes::diplomacy;

}



//Parameterized constructor, takes an enum containing the different card types
Cards::Cards(cardTypes cardType)
{
	this->cardType = cardType;
}

//Copy constructor
Cards::Cards(const Cards& card)
{
	this->cardType = card.cardType;
}

//Destructor
Cards::~Cards()
{
}

//Stream insertion operator
std::ostream& operator<<(std::ostream& stream, Cards& card)
{
	stream << "Card type with enum value "<< card.getCardType() << std::endl;
	return stream;
}

//Assignment operator
Cards& Cards::operator=(const Cards& card)
{
	this->cardType = card.cardType;
	return *this;
}


//Plays the card
void Cards::play(Player* p)
{
	p->getPlayerHand()->removeFromHand(this);
	p->getGameEngine()->getDeck()->returnToDeck(this);
	Orders* o;

	vector<Territory*> attackable = p->toAttack();
	vector<Territory*> defendable = p->toDefend();

	switch (this->cardType) {
	case cardTypes::airlift:
		o = new Airlift(p, defendable.at(0), defendable.at(defendable.size()-1), defendable.at(0)->getArmyCount());
		break;
	case cardTypes::blockade:
		o = new Blockade(p, defendable.at(defendable.size() - 1));
		break;
	case cardTypes::bomb:
		o = new Bomb(p, defendable.at(0), attackable.at(0));
		break;
	case cardTypes::diplomacy:
		o = new Negotiate(p, attackable.back()->getOwningPlayer());
		break;
	case cardTypes::reinforcement:
		p->setReinforcementPool(p->getReinforcementPool() + 5);
		return;
	default:
		return;
	}

	p->getPlayerOrders()->addOrder(o);
}

//Prints the type of the card
void Cards::print()
{
	std::cout << this->cardType << std::endl;
}

//Returns the card type
Cards::cardTypes Cards::getCardType()
{
	return cardType;
}


//Deck class

//Paramaterized constructor
Deck::Deck(int deckSize)
{
	this->deckSize = deckSize;
	Deck::intializeDeck();
	Deck::shuffle();
}

//Destructor
Deck::~Deck()
{
	for (Cards* card : deck) {
		delete card;
	}
}

//Copy Constructor
Deck::Deck(const Deck& deck)
{
	this->deck = deck.deck;
}


//Stream insertion operator
std::ostream& operator<<(std::ostream& stream, Deck& deck)
{
	stream << "cards in deck " << deck.cardsIndeck() << std::endl;
	return stream;
}


//Adds an equal amount of each type of card to the deck. deck size should be in multiples of 5
void Deck::intializeDeck()
{
	const Cards::cardTypes cardNames[5] = { Cards::bomb, Cards::reinforcement, Cards::blockade, Cards::airlift,Cards::diplomacy };

	for (int i = 0; i < this->deckSize / 5; i++) {
		for (Cards::cardTypes cards : cardNames) {

			deck.push_back(new Cards(cards));
		}
	}
}

//Prints all cards in the deck
void Deck::printDeck()
{
	for (Cards* deck : Deck::deck) {
		deck->print();
	}
}

//Deletes the top card of the deck
void Deck::destroyTopCard()
{
	deck.erase(deck.begin());
}

//Returns top card
Cards* Deck::topCard()
{
	return deck.front();
}

//Draw a card and add it to a hand. also destroys the card from the deck once card is placed in hand
void Deck::draw(Hand& hand)
{
	if (!deck.size())
		return;
	hand.addToHand(this->topCard());
	this->destroyTopCard();
}

//Returns the number of cards contained in the deck
int Deck::cardsIndeck()
{
	return deck.size();
}

//Returns a card to the deck and then shuffles the deck.
void Deck::returnToDeck(Cards* card)
{
	deck.push_back(card);
	this->shuffle();
}

//Assignment operator
Deck& Deck::operator=(const Deck& deck)
{
	this->deck = deck.deck;

	return *this;
}

/*
Shuffles the deck
produces non-deterministic random numbers(random seed).Then takes the random seed generated and passes it to the default_random_engine
we then pass the default_random_engine to the shuffle so we can get different combinations of shuffled decks
*/
void Deck::shuffle()
{
	std::random_device randNum;
	std::default_random_engine random(randNum());
	std::shuffle(deck.begin(), deck.end(), random);
}


// Hand Class

//Constructor
Hand::Hand()
{
	std::vector<Cards*> hand = vector<Cards*>();
}

//Destructor
Hand::~Hand()
{
	for (Cards* card : hand) {
		delete card;
	}
}

//Copy constructor
Hand::Hand(const Hand& hand)
{
	this->hand = hand.hand;
}


//Stream insertion operator
std::ostream& operator<<(std::ostream& stream, Hand hand)
{
	stream << "cards in hand " << hand.getNumOfCardsInHand() << std::endl;
	return stream;
}

//Assignment operator
Hand& Hand::operator=(const Hand& hand)
{
	this->hand = hand.hand;
	return *this;
}

//Returns integer value of amound of cards being held in a hand
int Hand::getNumOfCardsInHand()
{
	return hand.size();
}

//Prints out all cards types in a hand, including its index it the hand
void Hand::handPrint()
{
	int index = 0;
	for (Cards* hand : Hand::hand) {
		std::cout << "card hand index:" << index << " ";
		index++;
		hand->print();
	}
	if (this->getNumOfCardsInHand() == 0) {
		std::cout << "No cards in hand" << std::endl;
	}
}

//Adds a card to the hand.
void Hand::addToHand(Cards* card)
{
	hand.push_back(card);
}

//Removes a card from the hand, given the provided index
void Hand::removeFromHand(int cardIndex)
{
	hand.erase(hand.begin() + cardIndex);
}

void Hand::removeFromHand(Cards* c)
{
	hand.erase(std::remove(hand.begin(), hand.end(), c), hand.end());
}

//Plays the card at the provided index
Cards* Hand::playFromHand(int cardIndex)
{

	Cards* card = this->getCardAt(cardIndex);
	std::cout << "card played " << card->getCardType() << std::endl;
	
	return card;
}

//Returns the card that at the provided index
Cards* Hand::getCardAt(int cardIndex)
{
	return hand[cardIndex];
}
