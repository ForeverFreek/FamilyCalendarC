// Event
// The class used to specify a single event on a single day
//

#pragma once

// ***************************************** INCLUDES *********************************************
#include <boost\date_time\gregorian\gregorian.hpp>
// ************************************************************************************************

class Event
{
	public:
		Event();
		Event(boost::gregorian::date startDate);
		~Event();

		// Getters/setters
		boost::gregorian::date getDate() const;
		void                   setDate(boost::gregorian::date newDate);

	private:
		// Variables
		boost::gregorian::date date;
};