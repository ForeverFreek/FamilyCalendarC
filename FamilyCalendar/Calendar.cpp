// ****************************************** INCLUDES ********************************************
#include "Calendar.h"
// ***************************************** -INCLUDES- *******************************************

// ################################################################################################
// #################################### CONSTRUCTOR/DESTRUCTOR ####################################
// ################################################################################################

// Calendar()
//
Calendar::Calendar()
{
	curProgramState = MAINMENU;

	curDate = boost::gregorian::date(boost::gregorian::day_clock::local_day());

	showMonth.month = curDate.month();
	showMonth.year  = curDate.year();
} // Calendar()

// ~Calendar()
//
Calendar::~Calendar()
{

} // ~Calendar()

// ################################################################################################
// ############################################ PUBLIC ############################################
// ################################################################################################

// mainLoop()
//
void Calendar::doMainLoop()
{
	do
	{
		switch(curProgramState)
		{
			case MAINMENU:
				doMainMenu();
				break;
			
			case SHOWMONTH:
				doShowMonth();
				break;

			default:
				std::cout << "No valid program state set. Program will be shut down safely." << std::endl;
				setCurProgramState(QUIT);
				break;
		}
	} while (getCurProgramState() != QUIT);
} // mainLoop()

// ################################################################################################
// ########################################## PRIVATE #############################################
// ################################################################################################

// doMainMenu()
// Prints the mainMenu and returns the following user input
//
void Calendar::doMainMenu()
{
	// Variables
	std::string input;

	printMainMenu();

	input = getValidInput();

	boost::algorithm::to_lower(input);

	std::cout << "\n\n";

	// Input processing
	if(input.compare("q") == 0)
	{
		setCurProgramState(QUIT);
	}
	else if(input.compare("cm") == 0)
	{
		setCurProgramState(SHOWMONTH);
	}
	else
	{
		std::cout << "No valid input.\n\n\n";
	}
} //doMainMenu()

// doShowMonth()
// Prints the current month, including all events processes further user input
//
void Calendar::doShowMonth()
{
	// Variables
	std::string            input = "";

	printMonth(showMonth);

	printShowMonthMenu();

	input = getValidInput();

	boost::algorithm::to_lower(input);

	std::cout << "\n\n";

	// Input processing
	if(input.compare("b") == 0)
	{
		// Return to main menu
		setCurProgramState(MAINMENU);
	}
	else if(input.compare("am") == 0)
	{
		// Change the month to be shown according to user input
		int year;
		int month;

		std::cout << "Year: ";
		std::cin >> year;

		std::cout << "Month: ";
		std::cin >> month;

		if(month < 1 || month > 12)
		{
			std::cout << "Invalid month." << std::endl;
		}
		else
		{
			showMonth.month = month;
			showMonth.year  = year;
			
			std::cout << "\n\n";
		}
	}
	else if(input.compare("e") == 0)
	{
		// Create a new event on a given day if possible
		int day;

		std::cout << "Day: ";
		std::cin >> day;

		try
		{
			createEvent(boost::gregorian::date(showMonth.year, showMonth.month, day));
		}
		catch(...)
		{
			std::cout << "Invalid day." << std::endl;
		}
	}
	else
	{
		std::cout << "No valid input.\n\n\n";
	}
} // doShowMonth()

// *************************************** HELPER METHODS *****************************************

// indentCalendarMonthBeginning()
// Helper-method used to indent the first few days in a print with a correct offset
//
void Calendar::indentCalendarMonthBeginning(int offset)
{
	std::cout << "  |";

	for(int i=1; i<offset; i++)
		std::cout << "     |";
} // indentCalendarMonthBeginning()

// printMonth()
// Helper method used to print the given month [1...12] in a given year
//
void Calendar::printMonth(daylessDate date)
{
	boost::gregorian::date curMonth(date.year, date.month, 1);
	boost::gregorian::date endOfMonth = curMonth.end_of_month();

	std::cout << " ------------------ " << curMonth.month() << " " << curMonth.year() << " ----------------- " << std::endl;
	std::cout << "  | Mon | Tue | Wed | Thu | Fri | Sat | Sun |"    << std::endl;

	// Set off the first few days so if the 1. is a saturday for example it gets printed as a saturday
	int beginningOffset = curMonth.day_of_week();
	indentCalendarMonthBeginning(beginningOffset);
	
	boost::gregorian::date_duration day(1);

	while(curMonth <= endOfMonth)
	{
		if(curMonth.day_of_week() == 1)
		{
			if(curMonth.day() != 1)
			{
				std::cout << "\n  |";
			}
		}

		if(curMonth.day() > 9)
		{
			if(findEvent(curMonth))
			{
				std::cout << curMonth.day() << "  #|";
			}
			else
			{
				std::cout << curMonth.day() << "   |";
			}
		}
		else
		{
			if(findEvent(curMonth))
			{
				std::cout << curMonth.day() << "   #|";
			}
			else
			{
				std::cout << curMonth.day() << "    |";
			}
		}

		curMonth = curMonth+day;
	}

	// Correct the offset of the last few days which are not included in the month (cosmetic)
	while (curMonth.day_of_week() != 1)
	{
		std::cout << "     |";

		curMonth += day;
	}

	std::cout << "\n\n";
} // printMonth()

// createEvent()
// Creates a new event, pushes it into the list and sorts the list by event date. If the current date is later than the given date, no event is created
//
void Calendar::createEvent(boost::gregorian::date date)
{
	if(curDate>date)
		return;

	eventList.push_back(Event(date));
	
	eventList.sort(compEventByDate);
} // createEvent()

// findEvent()
//
bool Calendar::findEvent(boost::gregorian::date date)
{
	for(std::list<Event>::iterator it = eventList.begin(); it != eventList.end(); it++) 
	{
		if(it->getDate()>date)
			return false;

		if(it->getDate()==date)
			return true;
	}

	return false;
}

// printMainMenu()
//
void Calendar::printMainMenu()
{
	std::cout << " -------------------- Menu -------------------- " << std::endl;
	std::cout << "*Show current month..(CM)"                        << std::endl;
	std::cout << "*Quit................(Q)"                         << std::endl;
	std::cout << "What do you want to do?"                          << std::endl;
}

// printShowMonthMenu()
//
void Calendar::printShowMonthMenu()
{
	std::cout << "*Show another month...(AM)" << std::endl;
	std::cout << "*Add event............(E)"  << std::endl;
	std::cout << "*Return to main menu..(B)"  << std::endl;
	std::cout << "What do you want to do?"    << std::endl;
}

std::string Calendar::getValidInput()
{
	std::string input = "";

	while(input.empty())
	{
		std::getline(std::cin, input);
	}

	return input;
}
// ************************************** -HELPER METHODS- ****************************************

// ################################################################################################
// ###################################### GETTERS/SETTERS #########################################
// ################################################################################################

// *************************************** curProgramState ****************************************

int Calendar::getCurProgramState()
{
	return curProgramState;
}

void Calendar::setCurProgramState(Calendar::programState newProgramState)
{
	curProgramState = newProgramState;
}

// ************************************** -curProgramState- ***************************************

// ***************************************** showMonth ********************************************

void Calendar::setShowMonth(int year, int month)
{
	showMonth.month = month;
	showMonth.year  = year;
}

// **************************************** -showMonth- *******************************************