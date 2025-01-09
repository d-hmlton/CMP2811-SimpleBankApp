#include "Transaction.h"

auto moneyPrintDouble = [](double money)
	{
		std::string toPrint;

		//Negative check
		bool isNegative = false;
		if (money < 0) { isNegative = true; money = money * -1; toPrint = "-"; }

		//Double to string with precision
		std::ostringstream sstream;
		sstream << std::fixed << std::setprecision(2) << money;
		std::string moneyStr = sstream.str();

		//Return
		toPrint = toPrint + "\x9C" + moneyStr;
		return toPrint;
	};

Transaction::Transaction(std::string x, double y) {
	desc = x;
	sum = y;
	pureTime = time(nullptr); //Grabs the current time
}

void Transaction::toString() {
	std::string timestamp = std::ctime(&pureTime); //Formats the time as a string to be output
	timestamp.erase(timestamp.find('\n', 0), 1); //ctime, for whatever reason, adds \n at the end. This removes that
	//Credit to user 'Thantos' on cprogramming forums in 2005 for this idea

	std::cout << "-- " << desc << ": " << moneyPrintDouble(sum) << " on " << timestamp << std::endl;
}

double Transaction::getSum() {
	return sum;
}