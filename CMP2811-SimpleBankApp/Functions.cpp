#include "Functions.h"

void options() {
	std::cout << "OPTIONS:" << std::endl;
	std::cout << "open [type] [initial_deposit] - Open a current (1), savings (2) or ISA (3) account" << std::endl;
	std::cout << "view [account number] - View balance and recent transactions" << std::endl;
	std::cout << "withdraw [sum] - Withdraw funds from most recently viewed account" << std::endl;
	std::cout << "deposit [sum] - Deposit funds into most recently viewed account" << std::endl;
	std::cout << "transfer [source] [destination] [sum] - Transfer funds between accounts" << std::endl;
	std::cout << "project [years] - Project balance forward in time for most recently viewed account" << std::endl;
	std::cout << "search [account number] [value] - Provides transaction(s) closest to the value provided" << std::endl;
	std::cout << "exit - Close this application" << std::endl;
	std::cout << "options - View these options again" << std::endl;
}

bool stringToDoubleValidation(std::string input) {
	try {
		double output = std::stod(input);
	}
	catch (std::invalid_argument) { //If the input isn't a valid number (to be a double)
		return false;
	}
	return true; //No errors found
}

int accountNumberValidation(std::string input, int vectorSize) {
	int accountNum = stoi(input) - 1; //Index is one digit lower than account number

	//Checks if the account number given is out of range of openAccounts, or a negative number
	if (vectorSize < accountNum + 1 || accountNum < 0) {
		std::cout << "The [account number] provided is not assigned to an open account. Please try again." << std::endl;
		return -1; //Resets back to prompt
	}

	return accountNum;
}

bool sumValidation(double sum, std::string command) {
	if (sum > 100000) { std::cout << "Sums larger than \x9C" << "100000 are not supported." << std::endl; }
	else if (sum == 0) { std::cout << "You can't \'" << command << "\' nothing!" << std::endl; }
	else if (sum < 0) { std::cout << "\'" << command << "\' negative money... that's against the rules!" << std::endl; }
	else { return true; } //No issues found

	return false; //Issue found
}

bool accountCheck(int vectorSize) {
	if (vectorSize == 0) {
		std::cout << "An account has not been opened yet." << std::endl;
		return false; //"There aren't any accounts"
	}
	return true; //"There are accounts"
}

int accountDefault(int recentIndex, int vectorSize) {
	int accountNum;
	if (recentIndex == -1) { accountNum = (vectorSize - 1); } //If nothing has been saved to recentIndex
	else { accountNum = recentIndex; } //If something has
	return accountNum;
}