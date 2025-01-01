/* ------------------------------------------------------
CMP2811: Applied Programming Paradigms
Driver program for "LincBank" Assessment Item 2
Semester A 2024

During marking, we will use the exact same notation
as provided in the brief, so make sure you follow that guideline.

You should avoid removing parts of this driver program, but
add to it the necessary code to produce your implementation.

Good luck!
------------------------------------------------------ */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <vector>
#include <stack> //Used to store 'Transaction' objects for the 'Account' class
#include <string>
#include <ctime> //Needed to store a 'time_t' timestamp for the 'Transaction' class

//- CLASS DEFINITIONS -
//Transaction stores data on each deposit and withdrawal
class Transaction
{
	std::string desc; //The nature of the transaction
	time_t timestamp; //The date & time
	double value; //The (monetary) value

public:
	void toString() {}
};

//InterestEarning is a sub-type of abstract class called an interface - pure virt funcs ONLY
class InterestEarning
{
public:
	virtual void computeInterest() = 0;
};

//Account is an abstract class, meaning you can't create instances for it
class Account
{
public:
	virtual void deposit() = 0;
	virtual void toString() = 0;
	virtual void withdraw() = 0;
};

//Current accounts don't have interest
class Current : public Account
{
	int overdraft = 500;
	double balance;
	std::stack<Transaction*> history;

public:
	Current(double initial) { balance = initial; }

	void deposit() {}
	void toString() {}
	void withdraw() {}
};

//Savings accounts have 0.85% interest normally, 1.15% as ISAs
class Savings : public Account, public InterestEarning
{
	double balance;
	double interestRate;
	bool isa;

public:
	Savings(double initial, bool isIsa) {
		isa = false; //Default setting on regular accounts
		interestRate = 0.85; //Default rate on regular accounts
		if (isIsa == true) { isa = true; interestRate = 1.15; }
		balance = initial;
	}

	void computeInterest() {}
	void deposit() {}
	void toString() {}
	void withdraw() {}
};

//- STANDARD FUNCTIONS -
//When called, 'options' will output help for the commands the program can execute
static void options() {
	std::cout << "OPTIONS:" << std::endl;
	std::cout << "open [type] [initial_deposit] - Open a current (1), savings (2) or ISA (3) account" << std::endl;
	std::cout << "view [index] - View balance and recent transactions" << std::endl;
	std::cout << "withdraw [sum] - Withdraw funds from most recently viewed account" << std::endl;
	std::cout << "deposit [sum] - Deposit funds into most recently viewed account" << std::endl;
	std::cout << "transfer [source] [destination] [sum] - Transfer funds between accounts" << std::endl;
	std::cout << "project [years] - Project balance forward in time for most recently viewed account" << std::endl;
	std::cout << "exit - Close this application" << std::endl;
	std::cout << "options - View these options again" << std::endl;
};

//Takes a string and sees if it can be converted to a double. If it can't, returns -1. If it can, returns 0.
static int parameterValidation(std::string input) {
	try {
		double output = std::stod(input);
	} catch (std::invalid_argument) { //If the input isn't a valid number
		return -1;
	}

	return 0; //No errors found
}

//Vector to store the open accounts
std::vector <Account*> openAccounts;

int main()
{
	std::vector <std::string> parameters;
	std::string userCommand;
	// you may also want to store a collection of opened accounts here

	std::cout << "~~~ Welcome to LincBank! ~~~\n" << std::endl;
	options();

	while (userCommand != "exit") //Will loop until user enters "exit"
	{
		parameters.clear(); // clear ready for next command
		std::cout << std::endl << ">>> ";

		std::getline(std::cin, userCommand);
		char* cstr = new char[userCommand.length() + 1];
		strcpy(cstr, userCommand.c_str());

		char* token;
		token = strtok(cstr, " ");

		while (token != NULL)
		{
			parameters.push_back(token);
			token = strtok(NULL, " ");
		}

		//Parameter Validation - ensures parameters are numbers by attempting to convert them to doubles
		for (int i = 1; i < parameters.size(); i++) {
			if (parameterValidation(parameters[i]) == -1) {
				std::cout << "Parameter " << i+1 << " (" << parameters[i] << ") is not a valid number." << std::endl;
				continue; //Jumps to the next loop
			}
		}

		// Define all commands as per the brief
		std::string command = parameters[0];

		//SHOW OPTIONS
		if (command.compare("options") == 0) 
		{
			options(); //Calls the options function
		}

		//OPEN NEW ACCOUNT
		else if (command.compare("open") == 0) 
		{
			std::string type = parameters[1]; //This parameter says the type of bank accounts (1 current, 2 standard savings, 3 isa)
			double initial = stod(parameters[2]); //This parameter says the initial balance

			//Current account
			if (type.compare("1") == 0) {
				openAccounts.push_back(new Current(initial)); //New current account!

				std::cout << "Current account created!" << std::endl; //Output message (current)
			} 

			//Savings account
			else if ((type.compare("2") == 0) || (type.compare("3") == 0)) { //'OR' - Will run for both standard and isa accounts
				bool isIsa = false; //Tells the constructor if the savings account is or isn't an ISA

				//ISA account
				if (type.compare("3") == 0) {

					//ISAs must have a minimum £1000 initial deposit - this checks if that's true
					if (initial < 1000) {
						std::cout << "The initial deposit of \x9C" << initial << " is too low. An ISA account must have a minimum"
							<< "initial deposit of \x9C" << "1000.\nPlease enter a higher initial deposit." << std::endl;
						continue;
					}

					isIsa = true; //User requested an ISA account - sets to true
				} 

				openAccounts.push_back(new Savings(initial, isIsa)); //New savings (normal/isa) account!

				if (isIsa == false) { std::cout << "Savings account created!" << std::endl; } //Savings message (current)
				if (isIsa == true) { std::cout << "ISA account created!" << std::endl; } //ISA message (current)
			}

			//If the first parameter (type) isn't one of the three valid inputs
			else {
				std::cout << "A valid account type was not entered. Enter \'options\' to see the types again." << std::endl;
				continue; //Resets loop
			}

			std::cout << "Account Number: " << openAccounts.size() << std::endl;

			// allow a user to open an account
			// e.g., Account* a = new Savings(...);
		}

		//VIEW ACCOUNT(S)
		else if (command.compare("view") == 0)
		{
			// display an account according to an index (starting from 1)
			// alternatively, display all accounts if no index is provided
		}
		else if (command.compare("withdraw") == 0)
		{
			// allow user to withdraw funds from an account
		}
		else if (command.compare("deposit") == 0)
		{
			// allow user to deposit funds into an account
		}
		else if (command.compare("transfer") == 0)
		{
			// allow user to transfer funds between accounts
			// i.e., a withdrawal followed by a deposit!
		}
		else if (command.compare("project") == 0)
		{
			// compute compound interest t years into the future
		}
		//else if (command.compare("search"))
		//{
		//	allow users to search their account history for a transaction
		//  (this is a stretch task)
		//}

	}

	std::cout << "Press any key to quit...";
	std::getchar();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

// References:
// "How to use std::stod properly" [https://stackoverflow.com/questions/26008321/how-to-use-stdstod-properly]