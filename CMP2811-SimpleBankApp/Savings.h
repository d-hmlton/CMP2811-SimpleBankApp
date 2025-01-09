#pragma once

#include "Account.h"

class Savings : public Account
{
	double interestRate;
	bool isIsa;

public:
	Savings(double x, bool y);

	void computeInterest(int years);

	void deposit(std::string desc, double sum);

	void historySearch(double value);

	void toString();

	bool withdraw(std::string desc, double sum);
};
