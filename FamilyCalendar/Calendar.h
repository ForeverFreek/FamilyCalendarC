// Calendar
// The central class used as a userinterface
//

// ******************************************** TODO **********************************************
// - Refactor the printMonth()-method, as it uses the same code several times
// ************************************************************************************************

// ****************************************** INCLUDES ********************************************
#include "Event.h"
#include "Comparators.h"

#include <iostream>
#include <sstream>
#include <list>
#include <boost\algorithm\string.hpp>
#include <boost\date_time\gregorian\gregorian.hpp>
// ************************************************************************************************

class Calendar
{
	public:
		Calendar();
		~Calendar();

		// Variables
		enum programState {MAINMENU, SHOWMONTH, QUIT};

		// Methods
		void doMainLoop();
		
		// Getters/setters
		void setCurProgramState(programState newProgramState);
		int  getCurProgramState();

		void setShowMonth (int year, int month);

	private:
		// Structs
		struct daylessDate
		{
			int month;
			int year;
		};

		// Variables
		int                    curProgramState;
		daylessDate            showMonth;
		boost::gregorian::date curDate;
		std::list<Event>       eventList;

		// Methods
		void doMainMenu();
		void doShowMonth();

		// Helper methods
		void        indentCalendarMonthBeginning(int offset);
		void        printMonth(daylessDate date);
		void        createEvent(boost::gregorian::date date);
		bool        findEvent(boost::gregorian::date date);
		void        printMainMenu();
		void        printShowMonthMenu();
		std::string getValidInput();

		// Comparators
		bool compareEventDate(const Event eventOne, const Event eventTwo);
};