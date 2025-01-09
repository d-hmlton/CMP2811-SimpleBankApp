#pragma once

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include <iostream>
#include <iomanip> //For moneyPrinter
#include <sstream> //For moneyPrinter

class Transaction
{
	std::string desc; //The nature of the transaction
	time_t pureTime; //The date & time
	double sum; //The (monetary) value

public:
	Transaction(std::string x, double y);

	void toString();

	double getSum();
};

#endif //TRANSACTION_H