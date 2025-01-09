#pragma once

#include "Account.h"

class Current : public Account
{
    int overdraft = -500; //Balance cannot go below this number

public:
	Current(double x);

	void deposit(std::string desc, double sum);

	void historySearch(double value);

	void toString();

	bool withdraw(std::string desc, double sum);

	void computeInterest(int years) {}
};

