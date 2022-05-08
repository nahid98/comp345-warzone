#pragma once
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <regex>

#include "headers/LoggingObserver.h"
#include "headers/GameEngine.h"
class CommandProcessor;
class Command;
class FileCommandProcessorAdapter;
class FileLineReader;

class CommandProcessor : public Subject, public ILoggable
{
public:
	// Constructors
	CommandProcessor();

	// Copy consturctor
	CommandProcessor(CommandProcessor& commandProcessor);

	// Destructor
	virtual ~CommandProcessor();

	//assignment operator
	CommandProcessor& operator= (const CommandProcessor& commandProcessor);

	//stream insertion operator
	friend std::ostream& operator<<(std::ostream& stream, CommandProcessor& commandProcessor);

	Command* getCommand(GameState gameState);

	std::list<Command*> getCommandList();

	bool validate(GameState gameState, Command* command);

	string stringToLog();

protected:

	virtual std::string readCommand();
	void saveCommand(Command* command);
	bool isValidTournament(Command* c);

private: 
	std::list<Command*> commandList;

	friend class GameEngine;
};


class Command : public Subject, public ILoggable
{
public:
	//constructors
	Command();
	Command(std::string command);

	//destructor
	virtual ~Command();
	
	//copy constructor
	Command(const Command& command);

	//assignment operator
	Command& operator= (const Command& command);

	//stream insertion operator
	friend std::ostream& operator<<(std::ostream& stream, Command& command);

	void saveEffect(std::string effect);
	
	std::string getEffect();
	std::string getCommand();
	bool getIsValid();
	void setIsValid(bool val);

	std::string stringToLog();
private:
	std::string effect;
	std::string command;

	bool isValid;
};

class FileCommandProcessorAdapter : public CommandProcessor
{
public:
	//constructor
	FileCommandProcessorAdapter(std::string fileName);

	//Destructor
	~FileCommandProcessorAdapter();

	//copy constructor
	FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fileCommandProcessorAdapter);

	//assignment operator
	FileCommandProcessorAdapter& operator= (const FileCommandProcessorAdapter& fileCommandProcessorAdapter);

	//stream insertion operator
	friend std::ostream& operator<<(std::ostream& stream, FileCommandProcessorAdapter& fileCommandProcessorAdapter);

	std::string readCommand();

private:
	std::string fileName;
	FileLineReader* fileLineReader;
};

class FileLineReader
{
public:
	//Constructor
	FileLineReader(std::string filename);

	//Destructor
	~FileLineReader();

	//copy constructor
	FileLineReader(const FileLineReader& fileLineReader);

	//assignment operator
	FileLineReader& operator= (const FileLineReader& fileLineReader);

	//stream insertion operator
	friend std::ostream& operator<<(std::ostream& stream, FileLineReader& fileLineReader);

	std::string readLineFromFile();

private:
	std::string fileName;
	int lineCount;
};