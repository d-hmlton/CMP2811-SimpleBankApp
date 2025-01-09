#pragma once

#include <vector>

#include "Transaction.h"

//Account is an abstract class, meaning you can't create instances for it
class Account
{
protected:
	double balance=0;
	std::vector<Transaction*> history;

public:
	virtual void computeInterest(int years) = 0;
	virtual void deposit(std::string desc, double sum) = 0;
	virtual void historySearch(double value) = 0;
	virtual void toString() = 0;
	virtual bool withdraw(std::string desc, double sum) = 0;
};