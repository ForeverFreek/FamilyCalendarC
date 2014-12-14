// Comparators
#include "Comparators.h"

// compEventByDate
// A function to compare objects of the type <Event> by their date-member (boost::gregorian::date)
bool compEventByDate(const Event &eventOne, const Event &eventTwo)
{
	return (eventOne.getDate()<eventTwo.getDate());
} // compareEventDate()