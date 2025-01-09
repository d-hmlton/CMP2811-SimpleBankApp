#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <functional>

//-LAMBDA FUNCTIONS-
//Variant of 'moneyPrint' for doubles
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

//Variant of 'moneyPrint' for integers
auto moneyPrintInt = [](int money)
	{
		return "\x9C" + std::to_string(money);
	};