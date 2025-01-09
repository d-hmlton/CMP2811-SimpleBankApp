#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>
#include <ctime>
#include <string>

//#include "MoneyPrintLambda.h"

class Transaction
{
	std::string desc; //The nature of the transaction
	time_t pureTime; //The date & time
	double sum; //The (monetary) value

public:
	Transaction(std::string x, double y);

	void toString() const;

	double getSum() const;
};