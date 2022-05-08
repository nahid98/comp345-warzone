#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include <list>
#include <string>
#include <fstream>

#include "headers/LoggingObserver.h"
using namespace std;

/*
	Observer Class 	
*/

//Destructor, intentionally empty, nothing to do here.
Observer::~Observer()
{
}

//Constructor, intentionally empty, nothing to do here.
Observer::Observer()
{
}

/*
	Log Observer Class
*/

bool LogObserver::recreatedGameLog = false;

//Constructors
LogObserver::LogObserver()
{
	_fileLocation = "";
	_subject = nullptr;

}

LogObserver::LogObserver(string fileLocation)
{
	_fileLocation = fileLocation;
	_subject = nullptr;

	if (!recreatedGameLog) {
		remove(fileLocation.c_str());
		recreatedGameLog = true;
	}
}

LogObserver::LogObserver(Subject* s)
{
	_fileLocation = "";
	_subject = s;
	s->Attach(this);
}

//Copy constructor
LogObserver::LogObserver(const LogObserver& observer)
{
	_fileLocation = observer._fileLocation;
	_subject = observer._subject;
}

//Destructor
LogObserver::~LogObserver()
{
	if(_subject)
		_subject->Detach(this);

}

//Update Method, this takes a loggable object and logs it's effect to the 
//file using its implemented stringToLog() method from ILoggable.
void LogObserver::Update(ILoggable* loggable)
{
	//Create an output file stream
	ofstream fileStream;
	fileStream.open(_fileLocation, ios::out | ios::app);

	//Make sure we've successfully opened it, otherwise do nothing.
	if (!fileStream.is_open())
		return;

	//Write the text to the file.
	fileStream << loggable->stringToLog() << std::endl;
	fileStream.close();
}

//log file setter method
void LogObserver::setLogFile(string fileLocation)
{
	_fileLocation = fileLocation;

	if (!recreatedGameLog) {
		remove(fileLocation.c_str());
		recreatedGameLog = true;
	}
}

//assignment operator 
LogObserver& LogObserver::operator=(const LogObserver& observer)
{
	this->_fileLocation = observer._fileLocation;
	this->_subject = observer._subject;

	return *this;
}

//Log Observer stream insertion 
std::ostream& operator<<(std::ostream& out, const LogObserver& logObserver)
{
	out << "LogObserver{Subject = " << logObserver._subject << " , fileLocation = " << logObserver._fileLocation << "}" << endl;
	return out;
}

/*
	Subject Class
*/

//Attach method attaches a generic observer to its list of observers
void Subject::Attach(Observer* obs)
{
	_observers->push_back(obs);
}

//Detach method removes a generic observer from its list of observers
void Subject::Detach(Observer* obs)
{
	_observers->remove(obs);
}

//Notify method notifies all attached observers and calls their respective update
void Subject::Notify(ILoggable* loggable)
{
	for (list<Observer*>::iterator iterator = _observers->begin(); iterator != _observers->end(); ++iterator) {
		(*iterator)->Update(loggable);
	}
}

Subject& Subject::operator=(const Subject& subject)
{
	this->_observers = subject._observers;

	return *this;
}

//Constructors
Subject::Subject()
{
	_observers = new list<Observer*>;
	logger = new LogObserver();
	logger->setLogFile("./gamelog.txt");
	this->Attach(logger);
}

Subject::Subject(const Subject& subject)
{
	_observers = subject._observers;
	logger = subject.logger;
}

//Destructor
Subject::~Subject()
{
	delete logger;
	delete _observers;
}
