#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include <iostream>
#include <algorithm>
#include <map>
#include <stack>
#include <queue>
#include <sstream>
#include "headers/Map.h"
#include "headers/Player.h"

#pragma region Territory
//Constructors
Territory::Territory() { 
	territoryId = -1;
	armies = 0;
	name = nullptr;
	continentId = -1;
	ownedBy = nullptr;
}

//Copy constructor, NOTE: It does not do a deep copy of the adjacent territories!
Territory::Territory(const Territory& territory){
	territoryId = territory.territoryId;
	name = new string(*territory.name);
	continentId = territory.continentId;

	if (territory.ownedBy == nullptr)
		ownedBy = nullptr;
	else
		ownedBy = new Player(*territory.ownedBy);

	armies = territory.armies;
	adjacentTerritories = territory.adjacentTerritories;
}

Territory::Territory(int id, string territoryName, int territoryContinentId){
	territoryId = id;
	name = new string(territoryName);
	continentId = territoryContinentId;
	armies = 0;
	ownedBy = nullptr;
}

Territory::Territory(int id, string territoryName, int territoryContinentId, vector<Territory*> adjacencies){
	territoryId = id;
	name = new string(territoryName);
	continentId = territoryContinentId;
	adjacentTerritories = adjacencies;
}

//Destructors
Territory::~Territory(){
	delete name;
	ownedBy = nullptr;
}

//Setters
void Territory::setOwningPlayer(Player* player){
	this->ownedBy = player;
}

void Territory::setName(string name){
	this->name = new string(name);
}

void Territory::setContinentId(int continentId){
	if (continentId < 0)
		this->continentId = 0;
	else
		this->continentId = continentId;
}

void Territory::setArmyCount(int armies){
	if (armies < 0)
		this->armies = 0;
	else
		this->armies = armies;
}

//Getters
Player* Territory::getOwningPlayer(){
	return ownedBy;
}

string Territory::getName(){
	return *name;
}

int Territory::getContinentId(){
	return continentId;
}

int Territory::getArmyCount(){
	return armies;
}

vector<Territory*> Territory::getAdjacencies(){
	return adjacentTerritories;
}

int Territory::getId(){
	return territoryId;
}

//Adds a territory to this territories adjacencies
void Territory::addAdjacentTerritory(Territory* territory){
	if (territory == nullptr)
		return;

	this->adjacentTerritories.push_back(territory);
}

//Removes all adjacent territories
void Territory::clearAdjacencies(){
	this->adjacentTerritories.clear();
}

//Gets a territory with a specific id, otherwise nullptr
Territory* Territory::getAdjacentTerritoryWithId(int id){
	for (Territory* t : adjacentTerritories) {
		if (t->getId() == id)
			return t;
	}
	return nullptr;
}

//Gets a territory with a specific name, otherwise nullptr
Territory* Territory::getAdjacentTerritoryWithName(string name){
	for (Territory* t : adjacentTerritories) {
		if (t->getName() == name)
			return t;
	}
	return nullptr;
}

Territory& Territory::operator=(const Territory& territory){
	this->name = new string(*territory.name);
	this->continentId = territory.continentId;
	this->ownedBy = new Player(*territory.ownedBy);
	this->armies = territory.armies;
	this->adjacentTerritories = territory.adjacentTerritories;

	return *this;
}

ostream& operator<<(ostream& stream, Territory& territory){

	stream << "Territory(name = " << *territory.name << ", continentId = " << territory.getContinentId() << ", armies = " << territory.armies << ", adjacencies = " << territory.adjacentTerritories.size() << ")" << endl;
	return stream;
}

#pragma endregion Territory implementation

#pragma region Continent
//Constructors
Continent::Continent(){
	continentId = -1;
	additionalArmies = 0;
	name = nullptr;
}

Continent::Continent(int id, string continentName, int armyBonus){
	continentId = id;
	name = new string(continentName);
	additionalArmies = armyBonus;
}

//Copy Constructor, NOTE: It doesn't do a deep copy of the member territories!
Continent::Continent(const Continent& continent){
	continentId = continent.continentId;
	name = new string(*continent.name);
	additionalArmies = continent.additionalArmies;

	memberTerritories = continent.memberTerritories; 
}

//Destrucotrs
Continent::~Continent(){
	delete name;
	name = nullptr;
}

//Setters
void Continent::setName(string name){
	this->name = new string(name);
}

void Continent::setAdditionalArmies(int bonus){
	if (bonus < 0)
		this->additionalArmies = 0;
	else
		this->additionalArmies = bonus;
}

//Getters
int Continent::getContinentId()
{
	return continentId;
}

string Continent::getName(){
	return *this->name;
}

int Continent::getBonusArmyCount(){
	return this->additionalArmies;
}

vector<Territory*> Continent::getMembers(){
	return this->memberTerritories;
}

//Adds a territory to the continents members
void Continent::addTerritory(Territory* territory){
	if (territory == nullptr)
		return;

	this->memberTerritories.push_back(territory);
}

//Removes all members from the continent
void Continent::clearMembers()
{
	memberTerritories.clear();
}

//Retrieves a territory with a specific id, otherwise it returns nullptr
Territory* Continent::getTerritoryWithId(int id){
	for (Territory* t : memberTerritories) {
		if (t->getContinentId() == id)
			return t;
	}

	return nullptr;
}

//Retrieves a territory with a specific name, otherwise it returns a nullptr
Territory* Continent::getTerritoryWithName(string name)
{
	for (Territory* t : memberTerritories) {
		if (t->getName() == name)
			return t;
	}

	return nullptr;
}

Continent& Continent::operator=(const Continent& continent)
{
	this->name = new string(*continent.name);
	this->memberTerritories = continent.memberTerritories;
	this->additionalArmies = continent.additionalArmies;

	return *this;
}

ostream& operator<<(ostream& stream, Continent& continent)
{
	stream << "Continent(name = " << *continent.name << ", bonus = " << continent.additionalArmies << ", Member Territories = {";
	for (Territory* t : continent.memberTerritories) {
		stream << t->getName() << ", ";
	}

	stream << "}" << endl;
	return stream;
}

#pragma endregion Continent implementation

#pragma region Map

//Constructors
Map::Map(){}

Map::Map(vector<Territory*> territories, vector<Continent*> continents){
	this->nodes = territories;
	this->continents = continents;
}

//Copy constructor that recreates the entirety of the Map object
//This includes all Continents, Territories, and Borders.
Map::Map(const Map& map){
	if (&map == nullptr)
		return;

	//Recreate the territories (without borders)
	for (Territory* t : map.nodes) {
		Territory* newTerritory = new Territory(*t);
		newTerritory->clearAdjacencies();
		this->nodes.push_back(newTerritory);
	}

	//Recreate the continents (without territory members)
	for (Continent* c : map.continents) {
		Continent* newContinent = new Continent(*c);
		newContinent->clearMembers();
		this->continents.push_back(newContinent);
	}

	//Create continent membership and borders from old map in new map
	for (Territory* t : map.nodes) {
		//add territory to respective continent's members
		Continent* continentInNewMap = this->getContinentWithId(t->getContinentId());
		Territory* territoryInNewMap = this->getTerritoryWithId(t->getId());

		continentInNewMap->addTerritory(territoryInNewMap);

		//Recreate the borders
		for (Territory* destination : t->getAdjacencies()) {
			Territory* borderTerritory = this->getTerritoryWithId(destination->getId());
			territoryInNewMap->addAdjacentTerritory(borderTerritory);
		}
	}
}

//Destructors 
Map::~Map(){
	for (Territory* t : nodes) {
		if (t != nullptr)
			delete t;
	}
	for (Continent* c : continents) {
		if(c != nullptr)
			delete c;
	}
}

//Getters
vector<Territory*> Map::getTerritories(){
	return nodes;
}

vector<Continent*> Map::getContinents(){
	return continents;
}

//Utility Functions

//Adds a territory to the array of territory objects managed by the map
void Map::addTerritory(Territory* territory){
	if (territory == nullptr)
		return;

	nodes.push_back(territory);
}


//Adds a continent to the vector of continent objects managed by the map
void Map::addContinent(Continent* continent) {
	if (continent == nullptr)
		return;

	continents.push_back(continent);
}

//Retrieves a territory with a certain ID, returns nullptr if not found
Territory* Map::getTerritoryWithId(int id){
	for (Territory* t : nodes) {
		if (t->getId() == id)
			return t;
	}

	return nullptr;
}

//Retrieves a territory with a certain name, returns nullptr if not found
Territory* Map::getTerritoryWithName(string name){
	for (Territory* t : nodes) {
		if (t->getName() == name)
			return t;
	}

	return nullptr;
}

//Retrieves a continent with a certain ID, returns nullptr if not found
Continent* Map::getContinentWithId(int id){
	for (Continent* c : continents) {
		if (c->getContinentId() == id)
			return c;
	}

	return nullptr;
}


//Retrieves a continent with a certain name, returns nullptr if not found
Continent* Map::getContinentWithName(string name){

	for (Continent* c : continents) {
		if (c->getName() == name)
			return c;
	}

	return nullptr;
}


/*
	validate makes the following checks to determine if a map is valid or not

	It checks for a connected Graph
		This means going through the nodes and making sure all nodes are reachable from eachother.
	It checks for Unique Territories
		This means going through every continent and verifying a territory only appears once in it's member territories
	It checks for connected Continents
		This means making sure each Continent(subgraph) is a connected component
*/
bool Map::validate(){
	if (!isConnectedGraph()) {
		cerr << "The graph was not strongly connected, therefore it failed validation" << endl;
		return false;
	}

	if (!hasUniqueTerritories()) {
		cerr << "The graph has territories which are members of more than one continents, therefore it failed validation" << endl;
		return false;
	}

	if (!hasConnectedContinents()) {
		cerr << "The graph has subgraphs which are not connected components, therefore it failed validation" << endl;
		return false;
	}

	return true;
}

//Static Transpose method, given a map it returns a transpose of the map. That is if there is edge (a, b), then edge (b, a) is in the transpose
Map* Map::transpose(Map* map)
{
	if (map == nullptr)
		return nullptr;

	Map* transpose = new Map();

	vector<Continent*> originalContinents = map->getContinents();
	vector<Territory*> originalTerritories = map->getTerritories();

	//add all continents to transpose, no difference there
	for (Continent* c  : originalContinents) {
		Continent* newContinent = new Continent(c->getContinentId(), c->getName(), c->getBonusArmyCount());
		transpose->addContinent(newContinent);
	}

	//add all territories with no edges
	for (Territory* t : originalTerritories) {
		Continent* continent = transpose->getContinentWithId(t->getContinentId());
		Territory* newTerritory = new Territory(t->getId(), t->getName(), continent->getContinentId());

		newTerritory->setOwningPlayer(t->getOwningPlayer());
		newTerritory->setArmyCount(t->getArmyCount());
		transpose->addTerritory(newTerritory);
		continent->addTerritory(newTerritory);

	}

	//Add all edges
	for (Territory* t : originalTerritories) {
		for (Territory* adjacent : t->getAdjacencies()) {
			//Get adjacent territory from transpose
			Territory* transposeTerritory = transpose->getTerritoryWithId(adjacent->getId());

			//Make a border (adjacent, t), instead of originals (t, adjacent)
			transposeTerritory->addAdjacentTerritory(transpose->getTerritoryWithId(t->getId()));

		}
	}

	return transpose;
}

//Map Assignment operator overload
Map& Map::operator=(const Map& map){
	for (Territory* t : this->nodes)
		delete t;

	for (Continent* c : this->continents)
		delete c;

	//Recreate the territories (without borders)
	for (Territory* t : map.nodes) {
		Territory* newTerritory = new Territory(*t);
		newTerritory->clearAdjacencies();
		this->nodes.push_back(newTerritory);
	}

	//Recreate the continents (without territory members)
	for (Continent* c : map.continents) {
		Continent* newContinent = new Continent(*c);
		newContinent->clearMembers();
		this->continents.push_back(newContinent);
	}

	for (Territory* t : map.nodes) {
		//Add each territory to a continent's members
		Continent* continentInNewMap = this->getContinentWithId(t->getContinentId());
		Territory* territoryInNewMap = this->getTerritoryWithId(t->getId());

		continentInNewMap->addTerritory(territoryInNewMap);

		//Recreate the Borders
		for (Territory* destination : t->getAdjacencies()) {
			Territory* borderTerritory = this->getTerritoryWithId(destination->getId());
			territoryInNewMap->addAdjacentTerritory(borderTerritory);
		}
	}

	return *this;
}

//Finds the shortest path between two territorys
vector<Territory*> Map::shortestPath(Territory* source, Territory* target)
{
	vector<Territory*> path;

	int result = 0;
	map<Territory*, bool> visitedTerritories;
	map<Territory*, Territory*> parent;

	queue<Territory*> bfsQueue;
	if (nodes.size() == 0)
		return path;

	//Initialize every territory as being unvisited
	for (Territory* t : nodes) {
		visitedTerritories[t] = false;
		parent[t] = nullptr;
	}

	bfsQueue.push(source);
	visitedTerritories[source] = true;
	
	while (!bfsQueue.empty()) {
		Territory* current = bfsQueue.front();
		bfsQueue.pop();

		for (Territory* t : current->getAdjacencies()) {
			if (!visitedTerritories[t]) {
				visitedTerritories[t] = true;
				bfsQueue.push(t);
				parent[t] = current;
			}
		}
	}

	if (!parent[target]) {
		return path;
	}
	else {
		for (Territory* t = target; t != nullptr; t = parent[t]) {
			path.push_back(t);
		}

		reverse(path.begin(), path.end());
	}


	return path;
}

//Finds the shortest path between a source territory and a vector of targets
vector<Territory*> Map::shortestPath(Territory* source, vector<Territory*> targets)
{
	vector<Territory*> path;

	int result = 0;
	map<Territory*, bool> visitedTerritories;
	map<Territory*, Territory*> parent;

	queue<Territory*> bfsQueue;

	Territory* closestTerr = nullptr;

	if (nodes.size() == 0)
		return path;

	//Initialize every territory as being unvisited
	for (Territory* t : nodes) {
		visitedTerritories[t] = false;
		parent[t] = nullptr;
	}

	bfsQueue.push(source);
	visitedTerritories[source] = true;

	while (!bfsQueue.empty() && closestTerr == nullptr) {
		Territory* current = bfsQueue.front();
		bfsQueue.pop();

		for (Territory* t : current->getAdjacencies()) {
			if (!visitedTerritories[t]) {
				visitedTerritories[t] = true;
				bfsQueue.push(t);
				parent[t] = current;

				if (find(targets.begin(), targets.end(), t) != targets.end()) {
					closestTerr = t;
					break;
				}
			}
		}
	}

	if (!parent[closestTerr]) {
		return path;
	}
	else {
		for (Territory* t = closestTerr; t != nullptr; t = parent[t]) {
			path.push_back(t);
		}

		reverse(path.begin(), path.end());
	}


	return path;
}

ostream& operator<<(ostream& stream, MapLoader& mapLoader)
{
	stream << "MapLoader(filePath = \"" << mapLoader.filePath << "\")" << endl;
	return stream;
}

//Map stream insertion overload
ostream& operator<<(ostream& stream, Map& map)
{
	stream << "Map(" << endl;
	stream << "\tContinents" << endl;
	int counter = 0;
	for (Continent* c : map.continents) {
		stream << "\t[" << counter++ << "] " << *c;
	}

	counter = 0;
	stream << "\tTerritories" << endl;
	for (Territory* t : map.nodes) {
		stream << "\t[" << counter++ << "] " << *t;
	}
	stream << ")" << endl;
	return stream;
}

/**
	Validates that the entire map is a connected graph
	That is, every node is reachable from an arbitrary node
*/
bool Map::isConnectedGraph(){

	int territoryCount = this->nodes.size();
	//DFS to see if reachable territories = territories 
	if (DFSUtil(nodes) != territoryCount)
		return false;

	//DFS on the transpose of the graph to see if all territories are still reachable
	Map* transpose = Map::transpose(this);
	if (DFSUtil(transpose->getTerritories()) != territoryCount) {
		delete transpose;
		return false;
	}

	delete transpose;
	return true;
}

/**
	Validates that every continent is a connected component
*/
bool Map::hasConnectedContinents(){
	//For every continent, we do a depth first search.
	for (Continent* c : continents) {
		int size = c->getMembers().size();
		if (DFSUtil(c->getMembers(), c) != size) {
			return false;
		}
	}

	Map* transpose = Map::transpose(this);
	for (Continent* c : transpose->getContinents()) {
		int size = c->getMembers().size();
		if (DFSUtil(c->getMembers(), c) != size) {
			delete transpose;
			return false;
		}
	}

	delete transpose;
	return true;
}

/**
	Validates that any territory which belongs to a continent only 
	appears once in all continent's member territories.

*/
bool Map::hasUniqueTerritories(){
	//Map territories to the continents they belong to
	std::map<Territory*, Continent*> m;

	for (Continent* currentContinent : continents) {
		for (Territory* territory : currentContinent->getMembers()) {
			//If a territory already has an entry in the map, then it belongs to two continents, not allowed!
			if (m.count(territory) != 0)
				return false;

			m[territory] = currentContinent;
		}
	}
	return true;
}

//A public function that does a DFS on the territories of the map
//Simply returns the # of visited nodes
int Map::DFSUtil(vector<Territory*> territories, Continent* c)
{
	int result = 0;
	//Use a map of territory -> bool for whether or not it was visited
	//Note we can no longer use a parallel array as we're dealing with a subset of the graph
	map<Territory*, bool> visitedTerritories;

	stack<Territory*> dfsStack;
	if (territories.size() == 0)
		return 0;

	//Initialize every territory as being unvisited
	for (Territory* t : territories) {
		visitedTerritories[t] = false;
	}

 	dfsStack.push(territories[0]);

	//Keep repeating DFS algorithm until stack has no more nodes to visit
	while (!dfsStack.empty()) {
		Territory* t = dfsStack.top();
		dfsStack.pop();

		visitedTerritories[t] = true;
		vector<Territory*> adjacencies = t->getAdjacencies();

		//Set current node as visited
		for (Territory* adjacentTerritory : adjacencies) {
			//Add adjacent nodes to stack IF they're part of the same continent AND they haven't been visited
			if (c != nullptr) {
				if (adjacentTerritory->getContinentId() == c->getContinentId() && !visitedTerritories[adjacentTerritory]) {
					dfsStack.push(adjacentTerritory);
				}
			}
			//Only need to check for visited if not dealing with continents
			else {
				if (!visitedTerritories[adjacentTerritory]) {
					dfsStack.push(adjacentTerritory);
				}
			}
		}
	}

	//Make sure every territory in the passed vector is visited
	for (Territory* t : territories) {
		if (visitedTerritories[t])
			result++;
	}

	return result;
}

#pragma endregion Map Implementation

#pragma region MapLoader

MapLoader::MapLoader(){
	filePath = nullptr;
}

MapLoader::MapLoader(string filePath){
	this->filePath = new string(filePath);
}

MapLoader::MapLoader(const MapLoader& ml){
	this->filePath = new string(*ml.filePath);
}

MapLoader::~MapLoader(){
	delete filePath;
}

void MapLoader::setFilePath(string filePath) {
	if (this->filePath)
		delete this->filePath;
	this->filePath = new string(filePath);
}

/*
	Opens a map file on disk and attempts to parse it to generate
	a proper Map object.

	If anything goes wrong during the parsing, it returns a nullptr.
*/
Map* MapLoader::generateMap(){
	Map* m = new Map();
	bool parsedSuccessfully = true;
	string currentLine;

	ifstream fileStream;
	fileStream.open(*filePath);

	//Failed to open a filestream!
	if (!fileStream) {
		cout << "could not open file at " << *filePath << endl;
		delete m;
		return nullptr;
		//exit(1); 
	}

	//Read until the [continents] section
	readUntilLine(fileStream, "[continents]", &currentLine);

	if (fileStream.eof()) {
		parsedSuccessfully = false;
	}

	//Create continents by parsing each line (up until an empty line or next section)
	if (!createContinents(fileStream, m, &currentLine)) {
		parsedSuccessfully = false;
	}

	//Read until the [countries] section
	readUntilLine(fileStream, "[countries]", &currentLine);

	if (fileStream.eof()) {
		parsedSuccessfully = false;
	}

	//Create countries by parsing each line (up until an empty line or next section)
	if (!createCountries(fileStream, m, &currentLine)) {
		parsedSuccessfully = false;
	}

	//Read until the [borders] section
	readUntilLine(fileStream, "[borders]", &currentLine);

	if (fileStream.eof()) {
		parsedSuccessfully = false;
	}

	//Create adjacencies based on the borders
	if (!createBorders(fileStream, m, &currentLine)) {
		parsedSuccessfully = false;
	}

	if (!parsedSuccessfully) {
		delete m;
		return nullptr;
	}

	fileStream.close();

	return m;
}
MapLoader& MapLoader::operator=(const MapLoader& ml){
	this->filePath = new string(*ml.filePath);

	return *this;
}
/*
	Utility function to separate a string based on a character delimiter.
	Returns it as a vector of strings.
*/
vector<string> MapLoader::splitLine(string line, char delimiter){
	vector<string> result;

	stringstream stream(line);
	string next;
	
	//While theres lines 
	while (getline(stream, next, delimiter)) {
		//prevent empty items being pushed onto results
		if(next != "")
			result.push_back(next);
	}

	return result;
}

/*
	Creates and adds continents to the passed map.
	Creates continents up until an empty line, end of file, or the next section [countries]
*/
bool MapLoader::createContinents(ifstream& stream, Map* m, string* currentLine){
	int continentCounter = 1;
	getline(stream, *currentLine, '\n');
	while (!stream.eof()) {
		if (*currentLine == "" || *currentLine == "[countries]")
			break;
		vector<string> data = splitLine(*currentLine, ' ');

		if (data.size() != CONTINENT_EXPECTED_PROPERTIES) {
			//delete m;
			return false;
		}

		//Try to create a continent object, may throw exception from stoi (if string cant be turned into an int)
		try {
			Continent* c = new Continent(continentCounter++, data[CONTINENT_NAME_INDEX], stoi(data[CONTINENT_BONUS_INDEX]));
			m->addContinent(c);
		}
		catch (const std::exception& e) {
			cerr << "Error trying to parse continent :" << e.what() << endl;
			//delete m;
			return false;
		}

		getline(stream, *currentLine, '\n');
	}

	return true;
}

/*
	Creates and adds countries to the passed map.
	Creates countries up until an empty line, end of file, or the next section [borders]

	If a line is an invalid country, returns false
	If all lines are valid countries , returns true
*/
bool MapLoader::createCountries(ifstream& stream, Map* m, string* currentLine){
	int countryCounter = 1;
	getline(stream, *currentLine, '\n');

	while (!stream.eof()) {
		if (*currentLine == "" || *currentLine == "[borders]")
			break;
		vector<string> data = splitLine(*currentLine, ' ');

		try {
			if (data.size() != COUNTRY_EXPECTED_PROPERTIES || countryCounter != stoi(data[COUNTRY_ID_INDEX])) {
				return false;
			}

			//Create the territory object, add it to the map, and add it to the appropriate continent
			Territory* t = new Territory(countryCounter++, data[COUNTRY_NAME_INDEX], stoi(data[COUNTRY_CONTINENT_ID_INDEX]));
			m->addTerritory(t);
			m->getContinentWithId(stoi(data[COUNTRY_CONTINENT_ID_INDEX]))->addTerritory(t);
		}
		catch (const std::exception& e) {
			cerr << "Error trying to parse country :" << e.what() << endl;
			return false;
		}

		getline(stream, *currentLine, '\n');
	}

	return true;
}

/*
	Creates and adds borders to the passed map.
	Creates borders up until an empty line or the end of file

	returns true if all borders were successfully created
*/
bool MapLoader::createBorders(ifstream& stream, Map* m, string* currentLine){
	getline(stream, *currentLine, '\n');
	while (*currentLine != "") {
		vector<string> data = splitLine(*currentLine, ' ');

		//Add a border between the first id and all other ids on the line
		for (int i = 1; i < data.size(); i++) {


			try {
				Territory* origin = m->getTerritoryWithId(stoi(data[0]));
				Territory* destination = m->getTerritoryWithId(stoi(data[i]));

				origin->addAdjacentTerritory(destination);
			}
			catch (const std::exception& e) {
				cerr << "Error trying to parse continent :" << e.what() << endl;
				return false;
			}
		}

		if (stream.eof())
			break;

		getline(stream, *currentLine, '\n');
	}
	return true;
}

/*
	Given a stream, this function simply reads until a line that equals the passed in line.
*/
void MapLoader::readUntilLine(ifstream& stream, string line, string* currentLine){
	while (!stream.eof() && *currentLine != line) {
		getline(stream, *currentLine, '\n');
	}
}

#pragma endregion MapLoader implementation