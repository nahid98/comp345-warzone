#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include "headers/CommandProcessing.h"
#include "headers/Map.h"

// Command Processor Class

//default Constructor
Command::Command()
{
}

//paramaterized constructor
Command::Command(std::string command)
{
	this->command = command;
}

//destructor
Command::~Command()
{
}

//Copy Constructor
Command::Command(const Command& command)
{
	this->command = command.command;
	this->command = command.effect;
	this->isValid = command.isValid;
}

//assignment operator
Command& Command::operator=(const Command& command)
{
	this->effect = command.effect;
	this->command = command.command;
	this->isValid = command.isValid;
	return *this;
}

//stream insertion operator
std::ostream& operator<<(std::ostream& stream, Command& command)
{
	stream << "The command is " + command.command << " " << "The  command effect is " + command.effect << std::endl;

	return stream;
}

// setter for effect 
void Command::saveEffect(std::string effect)
{
	this->effect = effect;
	Notify(this);
}

//getter for effect
std::string Command::getEffect()
{
	return this->effect;
}

//getter for command
std::string Command::getCommand()
{
	return this->command;
}

bool Command::getIsValid()
{
	return isValid;
}

void Command::setIsValid(bool val)
{
	isValid = val;
}

std::string Command::stringToLog()
{
	return "Command effect was " + effect;
}

//Command processsing class

//Constructor
CommandProcessor::CommandProcessor()
{
	this->commandList = {};
}

//Copy constructor
CommandProcessor::CommandProcessor(CommandProcessor& commandProcessor)
{
	this->commandList = commandProcessor.commandList;
}

//Destructor
CommandProcessor::~CommandProcessor()
{
	for (Command* command : commandList) {
		delete command;
	}
}

//assignment operator
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& commandProcessor)
{
	this->commandList = commandProcessor.commandList;
	return *this;
}

//stream insertion operator
std::ostream& operator<<(std::ostream& stream, CommandProcessor& commandProcessor)
{
	for (auto command : commandProcessor.getCommandList())
		stream << "THe command is " + command->getCommand() << " " << "The command effect is " + command->getEffect() << std::endl;

	return stream;
}

// gets userinput and verifies if the command is valid or not
//returns the command as a string
Command* CommandProcessor::getCommand(GameState state)
{
	std::string command = readCommand();

	Command* p = new Command(command);
	saveCommand(p);

	p->setIsValid(validate(state, p));
	return p;
}

// getter for commandlist
std::list<Command*> CommandProcessor::getCommandList()
{
	return this->commandList;
}


//validation rules for the different commands in various states
//returns a boolean depending on the  command and state
//bool CommandProcessor::validate(GameState state, std::string command)
bool CommandProcessor::validate(GameState state, Command* c)
{
	if (std::regex_match(c->getCommand(), std::regex("-console|-file\\s.*")) && state == GameState::GAME_STATE_UNKNOWN) {
		return true;
	}
	else if (std::regex_match(c->getCommand(), std::regex("loadmap\\s+[\\w\\d\\-\\/]+\\.map\\s*")) && (state == GameState::GAME_STATE_START || state == GameState::GAME_STATE_MAP_LOAD)) {
		return true;
	}
	else if (std::regex_match(c->getCommand(), std::regex("tournament.*")) && state == GameState::GAME_STATE_START
			&& isValidTournament(c)) {
		return true;
	}
	else if (!c->getCommand().compare("validatemap") && state == GameState::GAME_STATE_MAP_LOAD) {
		return true;
	}
	else if (std::regex_match(c->getCommand(), std::regex("addplayer\\s+[\\w\\d\\s]+")) && (state == GameState::GAME_STATE_MAP_VALIDATED || state == GameState::GAME_STATE_PLAYERS_ADDED)) {
		return true;
	}
	else if (!c->getCommand().compare("gamestart") && state == GameState::GAME_STATE_PLAYERS_ADDED) {
		return true;
	}
	else if (!c->getCommand().compare("replay") && state == GameState::GAME_STATE_WIN) {
		return true;
	}
	else if (!c->getCommand().compare("quit") && state == GameState::GAME_STATE_WIN) {
		return true;
	}


	c->saveEffect("Command \'" + c->getCommand() + "\' is not valid in state " + GameEngine::gameStateToString(state));
	return false;
}

string CommandProcessor::stringToLog()
{
	return "Command " + commandList.back()->getCommand() + " was saved.";
}

// ask user to enter a command
// returns a string containing users input
std::string CommandProcessor::readCommand()
{
	std::string commandName;
	std::cout << "Enter a command" << std::endl;
	std::getline(std::cin, commandName);
	return commandName;
}

// adds a Command to the list
void CommandProcessor::saveCommand(Command* command)
{
	this->commandList.push_back(command);
	Notify(this);
}

bool CommandProcessor::isValidTournament(Command* c)
{
	string validStrategies[] = { "Aggressive", "Neutral", "Cheater", "Benevolent" };
	cout << c->getCommand() << endl;
	vector<string> result = splitString(c->getCommand(), ' ');
	vector<string> mapPaths;
	vector<string> playerStrategies;
	int gamesPerMap = 0;
	int turnsPerGame = 0;
	int index = 1;

	if (result.size() != 9) {
		cout << "Invalid tournament command!" << endl;
		return false;
	}

	try {
		while (index != result.size()) {
			if (result[index] == "-M") {
				mapPaths = splitString(result[++index], ',');
			}
			else if (result[index] == "-P") {
				playerStrategies = splitString(result[++index], ',');

				for (string s : playerStrategies) {
					if (find(std::begin(validStrategies), std::end(validStrategies), s) == std::end(validStrategies)) {
						cout << s << " is an Invalid Strategy!" << endl;
						return false;
					}
				}
			}
			else if (result[index] == "-G") {
				gamesPerMap = stoi(result[++index]);
			}
			else if (result[index] == "-D") {
				turnsPerGame = stoi(result[++index]);
			}
			else {
				cout << "Invalid option [" << result[index] << "]" << endl;
				return false;
			}

			index++;
		}
	}
	catch (...) {
		std::cerr << "entered a non-integer when expecting an integer" << endl;
		return false;
	}

	MapLoader* tempLoader = new MapLoader();

	for (string path : mapPaths) {
		tempLoader->setFilePath(path);
		Map* m = tempLoader->generateMap();
		if (m == nullptr || !m->validate()) {
			delete m;
			delete tempLoader;
			return false;
		}

		delete m;
	}

	delete tempLoader;


	//Make sure all the values are within an acceptable range
	if (mapPaths.size() < 1 || mapPaths.size() > 5 ||
		playerStrategies.size() < 2 || playerStrategies.size() > 4 ||
		gamesPerMap < 1 || gamesPerMap > 5 || turnsPerGame < 10 || turnsPerGame > 50) {
		return false;
	}
	return true;
}


// FileLineReaderclass

//constructor
FileLineReader::FileLineReader(std::string fileName)
{
	this->fileName = "commandFiles/"+fileName;
	this->lineCount = 0;
}

//destructor
FileLineReader::~FileLineReader()
{

}

//copy constructor
FileLineReader::FileLineReader(const FileLineReader& fileLineReader)
{
	this->fileName = fileLineReader.fileName;
	this->lineCount = fileLineReader.lineCount;
}

//assignment operator
FileLineReader& FileLineReader::operator=(const FileLineReader& fileLineReader)
{
	this->fileName = fileLineReader.fileName;
	this->lineCount = fileLineReader.lineCount;
	return *this;
}

//stream insertion operator
std::ostream& operator<<(std::ostream& stream, FileLineReader& fileNameReader)
{
	stream << "FileLineReader Called" << std::endl;

	return stream;
}

//reads from a text file 
//returns the string on each line of the txt file
std::string FileLineReader::readLineFromFile()
{
	std::ifstream commandFile;
	std::string line;
	commandFile.open(this->fileName);

	if (commandFile.is_open()) {
		for (int i = 0; i <= lineCount; i++) {
			if (commandFile.eof()) {
				cout << "There are no more lines to process in " << fileName << ", exitting with error.";
				exit(1);
			}
			getline(commandFile, line);
		}
		lineCount++;

		return line;
	}
	else{
		std::cout << "File can not be opened";
		exit(1);
	}
	return " File Can't be opened";
}


// FileCommandProcessorAdapter class

//constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(std::string fileName)
{
	this->fileName = fileName;
	fileLineReader = new FileLineReader(fileName);
}

//destructor
FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
	delete fileLineReader;
}


//copy constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fileCommandProcessorAdapter)
{
	this->fileLineReader = fileCommandProcessorAdapter.fileLineReader;
	this->fileName = fileCommandProcessorAdapter.fileName;

}
//assignment operator
FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& fileCommandProcessorAdapter)
{
	this->fileLineReader = fileCommandProcessorAdapter.fileLineReader;
	this->fileName = fileCommandProcessorAdapter.fileName;
	return *this;
}

//stream insertion operator
std::ostream& operator<<(std::ostream& stream, FileCommandProcessorAdapter& fileCommandProcessorAdapter)
{
	for (auto command : fileCommandProcessorAdapter.getCommandList())
		stream << "THe command is " + command->getCommand() << " " << "The command effect is " + command->getEffect() << std::endl;

	return stream;
}

// allows commands to be read from a textfile
// returns the command on the given line of the textfile
std::string FileCommandProcessorAdapter::readCommand()
{
	std::string command = fileLineReader->readLineFromFile();
	return command;
}
