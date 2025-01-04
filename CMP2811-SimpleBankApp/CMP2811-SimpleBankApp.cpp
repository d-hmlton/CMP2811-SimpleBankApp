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
#include <cmath> //Used to calculate the projected interest for the 'Savings' class
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
	Transaction(std::string x, double y) {
		desc = x;
		value = y;
		timestamp = time(nullptr);
	}

	void toString() {
		std::cout << "-- " << desc << ": \x9C" << printf("%g", value) << " on " << timestamp << std::endl;
	}
};

//InterestEarning is a sub-type of abstract class called an interface - pure virt funcs ONLY
class InterestEarning
{
public:
	virtual void computeInterest(int years) = 0;
};

//Account is an abstract class, meaning you can't create instances for it
class Account
{
protected:
	double balance;
	std::stack<Transaction*> history;

public:
	virtual void deposit(double sum) = 0;
	virtual void toString() = 0;
	virtual void withdraw(double sum) = 0;
};

//Current accounts don't have interest
class Current : public Account
{
	int overdraft = -500; //Balance cannot go below this number

public:
	Current(double x) {
		balance = x;
		history.push(new Transaction("initial deposit", balance));
	}

	void deposit(double sum) {
		balance = balance + sum;
		history.push(new Transaction("deposit", sum));
	}

	void toString() {
		std::cout << "Current account | Balance: \x9C" << printf("%g", balance) << std::endl;
	}

	void withdraw(double sum) {
		//Checks if the withdrawal would push the account over the overdraft
		if ((balance - sum) < overdraft) {
			std::cout << "Withdrawals and transfers must not exceed the account overdraft!" << std::endl;
			return;
		}
		balance = balance - sum;
		history.push(new Transaction("withdrawal", -sum));
	}
};

//Savings accounts have 0.85% interest normally, 1.15% as ISAs
class Savings : public Account, public InterestEarning
{
	double interestRate;
	bool isIsa;

public:
	Savings(double x, bool y) {
		balance = x;
		isIsa = y;
		interestRate = 0.85; //Default rate on regular accounts
		if (isIsa == true) { interestRate = 1.15; } //ISA rate
		history.push(new Transaction("initial deposit", balance));
	}

	void computeInterest(int years) {
		int months = 12 * years; //This is "n * t" in the equation. Will be the exponential later
		double decInterest = interestRate / 100; //The interest rate must be in DECIMAL FORM!

		double projection = (1 + (decInterest / 12)); //Interest rate is divided by the months in a year, then added to 1
		projection = pow(projection, months); //Projection is then calculated to the power of the number of months
		projection = balance * projection; //Finally, the balance is multiplied by the result of the interest formula
		std::cout << "Projected balance: \x9C" << projection << std::endl;
	}

	void deposit(double sum) {
		balance = balance + sum;
		history.push(new Transaction("deposit", sum));
	}

	void toString() {
		if (isIsa == false) { std::cout << "Savings account | Balance: \x9C" << printf("%g", balance) << std::endl; }
		if (isIsa == true) { std::cout << "ISA account | Balance: \x9C" << printf("%g", balance) << std::endl; }
	}

	void withdraw(double sum) {
		//Checks if the withdrawal would make the balance go below zero
		if ((balance - sum) < 0) {
			std::cout << "Withdrawals and transfers cannot take the account balance below zero!" << std::endl;
		}
	}
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

//A handler for sums that deposit and withdrawal share, making sure sums are reasonable
static int sharedSumHandler(std::string command, double sum) {
	if (sum > 10000) { std::cout << "Sums larger than \x9C" << "10,000 are not supported." << std::endl; }
	else if (sum == 0) { std::cout << "You can't " << command << " nothing!" << std::endl; }
	else if (sum < 0) { std::cout << command << "ing negative money... that makes no sense!" << std::endl; }
	else { return 0; } // 0 is returned if there are no problems

	return -1; // -1 is returned if there are problems
}

//A function to grab the account for withdraw and deposit to work with. Ideally, this is already set, but sometimes isn't
static int recentAccountGrabber() {
	int accountNum;
	if (recentIndex == -1) { accountNum = (openAccounts.size() - 1); }
	else { accountNum = recentIndex; }
	return accountNum;
}

//VARIABLES
std::vector <Account*> openAccounts; //Vector to store the open accounts
int recentIndex = -1; //Int that will store the most recently viewed account, needed for the deposit and withdraw options
//If recentIndex is -1, it means an account hasn't been specifically viewed; in this case, deposit / withdraw will default
//  to the most recently made account - or, the last account in the openAccounts index

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

		//Checks if parameters are present for deposit / withdraw / project - you can combine, since they all need one
		if ((command.compare("deposit") == 0) || command.compare("withdraw") == 0 || command.compare("project") == 0) {
			if (parameters.size() < 2) {
				std::cout << "\'" << command << "\' requires a value. Please try again." << std::endl;
				continue;
			}
		}

		//SHOW OPTIONS
		if (command.compare("options") == 0) 
		{
			options(); //Calls the options function
		}

		//OPEN NEW ACCOUNT
		else if (command.compare("open") == 0) 
		{
			//Checking if parameters are present
			if (parameters.size() < 3) {
				std::cout << "\'" << command << "\' requires a [type] value and an [initial_deposit] value.\nPlease try again." << std::endl;
				continue;
			}
			
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

				//Runs for savings + ISA
				openAccounts.push_back(new Savings(initial, isIsa)); //New savings (normal/isa) account!
				if (isIsa == false) { std::cout << "Savings account created!" << std::endl; } //Output message (savings)
				if (isIsa == true) { std::cout << "ISA account created!" << std::endl; } // Output message (isa)
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

		else if ((command.compare("withdraw") == 0) || (command.compare("deposit"))) {
			double sum = stod(parameters[1]); //Grabs sum from vector

			//Checks the sum is reasonable / possible
			int isValid = sharedSumHandler(command, sum);
			if (isValid == -1) { continue; }

			int accountNum = recentAccountGrabber();

			if (command.compare("withdraw")) { openAccounts[accountNum]->withdraw(sum); }
			else if (command.compare("deposit")) { openAccounts[accountNum]->deposit(sum); }
			

			// allow user to withdraw funds from an account
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
// "How to use std::stod properly"
//	[https://stackoverflow.com/questions/26008321/how-to-use-stdstod-properly]
// "Inserting a "£" sign in an output string in C++"
//	[https://stackoverflow.com/questions/3831289/inserting-a-%C2%A3-sign-in-an-output-string-in-c]