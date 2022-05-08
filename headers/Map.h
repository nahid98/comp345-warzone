#pragma once
#include <fstream>
#include <string>
#include <vector>


using namespace std;

class Player;
class Territory;
class Map;
class Continent;

/*
	Class Map Loader

	This class is in charge of parsing a map file and returning a Map object.
	If a map file isn't valid, it will return a null pointer.

*/
class MapLoader {
private:

	//Location of the map file on disk
	string* filePath;
	static const int CONTINENT_NAME_INDEX = 0;
	static const int CONTINENT_BONUS_INDEX = 1;
	static const int CONTINENT_EXPECTED_PROPERTIES = 3;

	static const int COUNTRY_ID_INDEX = 0;
	static const int COUNTRY_NAME_INDEX = 1;
	static const int COUNTRY_CONTINENT_ID_INDEX = 2;
	static const int COUNTRY_EXPECTED_PROPERTIES = 5;

	vector<string> splitLine(string line, char delimiter);

	bool createContinents(ifstream& stream, Map* m, string* currentLine);
	bool createCountries(ifstream& stream, Map* m, string* currentLine);
	bool createBorders(ifstream& stream, Map* m, string* currentLine);
	void readUntilLine(ifstream& stream, string line, string* currentLine);

public:
	MapLoader();
	MapLoader(string filePath);
	MapLoader(const MapLoader& ml);

	~MapLoader();

	void setFilePath(string path);

	Map* generateMap();

	MapLoader& operator= (const MapLoader& ml);
	friend ostream& operator<<(ostream& stream, MapLoader& mapLoader);
};

/*
	Class Map
	
	The Map is implemented as a graph data structure and represents a valid map file in memory
	It contains an internal array of nodes(territories) and an array of continents.
*/
class Map {
public:
	//Constructors
	Map();
	Map(vector<Territory*> territories, vector<Continent*> continents);
	Map(const Map& map);

	//Destructor
	~Map();

	//Getters
	vector<Territory*> getTerritories();
	vector<Continent*> getContinents();

	//util
	void addTerritory(Territory* territory);
	void addContinent(Continent* continent);

	Territory* getTerritoryWithId(int id);
	Territory* getTerritoryWithName(string name);

	Continent* getContinentWithId(int id);
	Continent* getContinentWithName(string name);

	bool validate();
	static Map* transpose(Map* map);

	//operator overloads
	Map& operator= (const Map& map);
	friend ostream& operator<<(ostream& stream, Map& map);

	vector<Territory*> shortestPath(Territory* source, Territory* target);
	vector<Territory*> shortestPath(Territory* source, vector<Territory*> targets);
private:
	//internal functions for validate method
	bool isConnectedGraph();
	bool hasConnectedContinents();
	bool hasUniqueTerritories();

	//internal function for DFS, returns # territories visited
	static int DFSUtil(vector<Territory*> territories, Continent* c = nullptr); 


	//The array of nodes(territories)
	vector<Territory*> nodes;
	//The array of continents
	vector<Continent*> continents;
};

/*
	Class Continent

	The Continent class represents a subgraph in a Map which contains more than one member territories.
	Continents give a bonus (additionalArmies) if a Player owns all territories which are members of the Continent
*/

class Continent {
private:
	int continentId;
	string* name;
	int additionalArmies;
	vector<Territory*> memberTerritories;
public:

	//Constructors
	Continent();
	Continent(int continentId, string continentName, int armyBonus);
	Continent(const Continent& continent);

	//Destructors
	~Continent();

	//setters
	void setName(string name);
	void setAdditionalArmies(int bonus);

	//getters
	int getContinentId();
	string getName();
	int getBonusArmyCount();
	vector<Territory*> getMembers();

	//Utility functions
	void addTerritory(Territory* territory);
	void clearMembers();

	Territory* getTerritoryWithId(int id);
	Territory* getTerritoryWithName(string name);

	//operator overloads
	Continent& operator= (const Continent& continent);
	friend ostream& operator<<(ostream& stream, Continent& continent);
};

/*
	Class Territory

	The Territory class represents nodes in our Map.
	Each territory belongs to only one player, belongs to only one continent, and contains a number of armies
	Each adjacent territory is kept in a list of pointers to the respective Territory objects
*/
class Territory {
private:

	int territoryId;
	int continentId;
	string* name;
	int armies;
	Player* ownedBy;
	vector<Territory*> adjacentTerritories;

public:
	//Constructors
	Territory();
	Territory(const Territory& territory);
	Territory(int id, string territoryName, int continent);
	Territory(int id, string territoryName, int continent, vector<Territory*> adjacencies);

	//Destructor
	~Territory();

	//setters
	void setOwningPlayer(Player* player);
	void setName(string name);
	void setContinentId(int continentId);
	void setArmyCount(int armies);


	//getters
	Player* getOwningPlayer();
	string getName();
	int getContinentId();
	int getArmyCount();
	vector<Territory*> getAdjacencies();
	int getId();

	//Utility functions
	void addAdjacentTerritory(Territory* territory);
	void clearAdjacencies();

	Territory* getAdjacentTerritoryWithId(int id);
	Territory* getAdjacentTerritoryWithName(string name);


	//operator overloads
	Territory& operator= (const Territory& territory);
	friend ostream& operator<<(ostream& stream, Territory& territory);
};
