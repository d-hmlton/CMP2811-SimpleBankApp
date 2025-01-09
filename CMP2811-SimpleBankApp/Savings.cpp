#include "Savings.h"

//FIX FOR LINKER ERROR
auto moneyPrintSAVINGS = [](double money)
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

Savings::Savings(double x, bool y) {
	balance = x;
	isIsa = y;
	interestRate = 0.85; //Default rate on regular accounts
	if (isIsa == true) { interestRate = 1.15; } //ISA rate
	history.push_back(new Transaction("initial deposit", balance));
}

void Savings::computeInterest(int years) {
	int months = 12 * years; //This is "n * t" in the equation. Will be the exponential later
	double decInterest = interestRate / 100; //The interest rate must be in DECIMAL FORM!

	double projection = (1 + (decInterest / 12)); //Interest rate is divided by the months in a year, then added to 1
	projection = pow(projection, months); //Projection is then calculated to the power of the number of months
	projection = balance * projection; //Finally, the balance is multiplied by the result of the interest formula
	std::cout << "Projected balance: " << moneyPrintSAVINGS(projection) << std::endl;
}

void Savings::deposit(std::string desc, double sum) {
	balance = balance + sum;
	history.push_back(new Transaction(desc, sum));
}

void Savings::historySearch(double value) {
	double sumStore;
	double hundredthOfValue = value / 100;

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

void Savings::toString() {
	if (isIsa == false) { std::cout << "Savings account | Balance: " << moneyPrintSAVINGS(balance) << std::endl; }
	if (isIsa == true) { std::cout << "ISA account | Balance: " << moneyPrintSAVINGS(balance) << std::endl; }

	for (int i = 0; i < history.size(); i++) {
		history[i]->toString();
	}
}

bool Savings::withdraw(std::string desc, double sum) {
	//Checks if the withdrawal would make the balance go below zero
	if ((balance - sum) < 0) {
		std::cout << "Withdrawals and transfers cannot take the account balance below zero!" << std::endl;
		return false;
	}
	balance = balance - sum;
	history.push_back(new Transaction(desc, -sum));
	return true;
}