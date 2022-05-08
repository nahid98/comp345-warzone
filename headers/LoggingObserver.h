#pragma once

#include <string>
#include <list>
#include <fstream>

using std::list;
using std::string;
using std::ofstream;

class Subject;
class GameEngine;

/***
*	Loggable abstract Interface for subjects which are capable of outputting to a log file.
*/
class ILoggable
{
public:
	//pure virtual method to be implemented by classes which inherit from this class
	virtual string stringToLog() = 0;
};

/***
*	Observer abstract class, attaches to a subject such that it is 
*   updated whenever the subject notifies.
*/
class Observer
{
public:
	//Destructor
	~Observer();

	virtual void Update(ILoggable* loggable) = 0;


protected:
	//Constructor
	Observer();
};

/***
*	LogObserver class is a concrete observer which logs specific actions
*	throughout the program.
*/
class LogObserver : public Observer
{
public:

	//Constructors
	LogObserver();
	LogObserver(string fileLocation);
	LogObserver(Subject* s);
	LogObserver(const LogObserver& observer); //copy constructor

	//Destructor (virtual as it has virtual functions)
	virtual ~LogObserver();

	void Update(ILoggable* loggable);

	void setLogFile(string fileLocation);

	LogObserver& operator =(const LogObserver& observer); //assignment operator
	friend std::ostream& operator << (std::ostream& out, const LogObserver& logObserver); //stream insertion operator

private:
	static bool recreatedGameLog;
	string _fileLocation;
	Subject* _subject;
};

/***
*	Subject class is to be inherited by objects which can be 
*	observed by any kind of Observer. It's function is to 
*   notify all observers attached to itself when appropriate.
*/
class Subject
{
public:
	//Constructors
	Subject();
	Subject(const Subject& subject); // copy constructor

	//Destructors
	virtual ~Subject();

	//Methods
	virtual void Attach(Observer* obs);
	virtual void Detach(Observer* obs);
	virtual void Notify(ILoggable* loggable);

	//Assignment Operator
	Subject& operator =(const Subject& subject);
private:
	list<Observer*>* _observers;
	LogObserver* logger;
};
