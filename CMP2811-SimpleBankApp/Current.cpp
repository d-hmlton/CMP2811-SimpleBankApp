#include "Current.h"

//Money Print Lambda Function
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

Current::Current(double x) {
	balance = x;
	history.push_back(new Transaction("initial deposit", balance));
}

void Current::deposit(std::string desc, double sum) {
	balance = balance + sum;
	history.push_back(new Transaction(desc, sum));
}

void Current::historySearch(double value) {
	double sumStore;
	float hundredthOfValue = value / 100;

	//Loops through the transactions
	for (int i = 0; i < history.size(); i++) {
		int transactionsFound = 0;
		sumStore = history[i]->getSum();
		//If the transaction is within 1% of the provided value's value
		if ((sumStore < (value + hundredthOfValue)) && (sumStore > (value - hundredthOfValue))) {
			history[i]->toString();
			transactionsFound = transactionsFound + 1;
		}
		std::cout << transactionsFound << " transaction(s) found." << std::endl;
		if (transactionsFound == 0) { std::cout << "You can view all transactions on a specific account with \'view\'." << std::endl; }
	}
}

void Current::toString() {
	std::cout << "Current account | Balance: " << moneyPrintDouble(balance) << std::endl;

	for (int i = 0; i < history.size(); i++) {
		history[i]->toString();
	}
}

bool Current::withdraw(std::string desc, double sum) {
	//Checks if the withdrawal would push the account over the overdraft
	if ((balance - sum) < overdraft) {
		std::cout << "Withdrawals and transfers must not exceed the account overdraft!" << std::endl;
		return false;
	}
	balance = balance - sum;
	history.push_back(new Transaction(desc, -sum));
	return true;
}