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
#include <iomanip>
#include <sstream>
#include <string>
#include <functional>

#include "Current.h"
#include "Functions.h"
#include "Savings.h"

//FIX FOR LINKER ERROR
auto moneyPrintBASE = [](double money)
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

//- MAIN PROGRAM -
int main() {
	//VARIABLES
	std::vector<Account*> openAccounts; //Vector to store the open accounts
	int accountsSize = 0; //int that is updated with the size of the vector. MUCH more efficient than calling .size() constantly


	int recentIndex = -1; //Int that will store the most recently viewed account, needed for the deposit and withdraw options
	//If recentIndex is -1, it means an account hasn't been specifically viewed; in this case, deposit / withdraw will default
	//  to the most recently made account - or, the last account in the openAccounts index
	int currentAccountNum = -1; //-1 if not current exists. If not, one exists, and this is the location for it
	int isaAccountNum = -1; //Same as above but for ISAs

	std::vector <std::string> parameters;
	int parametersSize = 0;

	std::string userCommand;
	// you may also want to store a collection of opened accounts here

	std::cout << "~~~ Welcome to LincBank! ~~~\n" << std::endl;
	options();

	while (userCommand != "exit") //Will loop until user enters "exit"
	{
		parameters.clear(); // clear ready for next command
		parametersSize = 0;
		std::cout << std::endl << ">>> ";

		std::getline(std::cin, userCommand);
		char* cstr = new char[userCommand.length() + 1];
		strcpy(cstr, userCommand.c_str());

		//Exception handling - resets loop if user gives no input. Will crash without this check!!
		if (cstr[0] == '\0') { continue; }

		char* token;
		token = strtok(cstr, " ");

		while (token != NULL)
		{
			parameters.push_back(token);
			parametersSize = parametersSize + 1;
			token = strtok(NULL, " ");
		}

		//Parameter Validation - ensures parameters are numbers by attempting to convert them to doubles
		bool exceptionFound = false;
		for (int i = 1; i < parametersSize; i++) {
			if (stringToDoubleValidation(parameters[i]) == false) {
				std::cout << "Parameter " << i+1 << " (" << parameters[i] << ") is not a valid number." << std::endl;
				exceptionFound = true;
				break;
			}
		}
		if (exceptionFound == true) { std::cout << "Please try again." << std::endl; continue; } //Resets back to prompt

		// Define all commands as per the brief
		std::string command = parameters[0];

		//PRE-EMPTIVE CHECKS
		//Checks if parameters are present, and an account exists, for deposit / withdraw / project. You can combine, all need one param
		if ((command.compare("deposit") == 0) || command.compare("withdraw") == 0 || command.compare("project") == 0) {
			//Ensures accounts exist to run these commands
			int ifAccounts = accountCheck(accountsSize);
			if (ifAccounts == false) { continue; } //Resets back to prompt
			
			if (parametersSize < 2) {
				if (command.compare("project") != 0) { std::cout << "\'" << command << "\' requires a [sum]. "; }
				else if (command.compare("project") == 0) { std::cout << "\'project\' requires [years]. "; }
				std::cout << "Please try again." << std::endl;
				continue; //Resets back to prompt
			}
		}
		//Does the same thing for view / transfer / search
		if ((command.compare("view") == 0) || (command.compare("transfer") == 0) || (command.compare("search") == 0)) {
			//Ensures accounts exist to run these commands
			int ifAccounts = accountCheck(accountsSize);
			if (ifAccounts == false) { continue; } //Resets back to prompt

			if (command.compare("transfer") == 0) {
				if (parametersSize < 4) {
					std::cout << "\'transfer\' requires a [source], a [destination], and a [sum]. Please try again." << std::endl;
					continue; //Resets back to prompt
				}
			}
			if (command.compare("search") == 0) {
				if (parametersSize < 3) {
					std::cout << "\'search\' requires an [account number], and a [value]. Please try again." << std::endl;
					continue;
				}
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
			if (parametersSize < 3) {
				std::cout << "\'" << command << "\' requires a [type] and an [initial_deposit]. Please try again." << std::endl;
				continue; //Resets back to prompt
			}
			
			std::string type = parameters[1]; //This parameter says the type of bank accounts (1 current, 2 standard savings, 3 isa)
			double initial = stod(parameters[2]); //This parameter says the initial balance

			//Checking if initial deposit is valid
			if (initial > 100000) {
				std::cout << "Initial deposits larger than \x9C" << "100000 are not supported. Please try again." << std::endl;
				continue;
			}
			if (initial < 0) { std::cout << "An initial deposit can't be negative! Please try again." << std::endl; continue; }

			//Current account
			if (type.compare("1") == 0) {
				if (currentAccountNum != -1) {
					std::cout << "You can only have one current account. Please select a different account type." << std::endl;
					continue; //Resets back to prompt
				}

				openAccounts.push_back(new Current(initial)); //New current account!
				accountsSize = accountsSize + 1; //Increments the accountsSize int
				std::cout << "Current account created!" << std::endl; //Output message (current)
				currentAccountNum = accountsSize - 1; //No new current accounts can be created
			} 

			//Savings account
			else if ((type.compare("2") == 0) || (type.compare("3") == 0)) { //'OR' - Will run for both standard and isa accounts
				bool isIsa = false; //Tells the constructor if the savings account is or isn't an ISA

				//ISA account
				if (type.compare("3") == 0) {
					if (isaAccountNum != -1) {
						std::cout << "You can only have one ISA account. Please select a different account type." << std::endl;
						continue; //Resets back to prompt
					}

					//ISAs must have a minimum £1000 initial deposit - this checks if that's true
					if (initial < 1000) {
						std::cout << "The initial deposit of " << moneyPrintBASE(initial) << " is too low. An ISA account must have a minimum"
							<< " initial deposit of \x9C" << "1000.\nPlease enter a higher initial deposit." << std::endl;
						continue; //Resets back to prompt
					}

					isIsa = true; //User requested an ISA account - sets to true
					isaAccountNum = accountsSize; //No new ISA accounts can be created
				} 

				//Runs for savings + ISA
				openAccounts.push_back(new Savings(initial, isIsa)); //New savings (normal/isa) account!
				accountsSize = accountsSize + 1; //Increments the accountsSize int
				if (isIsa == false) { std::cout << "Savings account created!" << std::endl; } //Output message (savings)
				if (isIsa == true) { std::cout << "ISA account created!" << std::endl; } // Output message (isa)
			}

			//If the first parameter (type) isn't one of the three valid inputs
			else {
				std::cout << "A valid account type was not entered. Enter \'options\' to see the types again." << std::endl;
				continue; //Resets back to prompt
			}

			std::cout << "Account Number: " << accountsSize << std::endl;

			// allow a user to open an account
			// e.g., Account* a = new Savings(...);
		}

		//VIEW ACCOUNT(S)
		else if (command.compare("view") == 0)
		{
			//If an account number is provided
			if (parametersSize > 1) {
				int accountNum = accountNumberValidation(parameters[1], accountsSize); //Hands input over to validation function
				if (accountNum == -1) { continue; } //If validation function says the input is invalid, resets back to prompt
				openAccounts[accountNum]->toString(); //Otherwise, views account details
				recentIndex = accountNum; //Saves the viewed account as the most recently viewed account
			}
			//If no account number is provided
			else { 
				for (int i = 0; i < accountsSize; i++) {
					openAccounts[i]->toString();
				}
			}
		}

		//WITHDRAW FROM / DEPOSIT TO ACCOUNTS
		else if ((command.compare("withdraw") == 0) || (command.compare("deposit") == 0)) {
			//Checks the sum is reasonable / possible
			double sum = stod(parameters[1]); //Grabs sum from vector
			bool isValid = sumValidation(sum, command);
			if (isValid == false) { continue; }

			//Grabs the account for withdraw and deposit to work with. Ideally, this is already set, but sometimes isn't
			int accountNum = accountDefault(recentIndex, accountsSize);

			//Withdraws or deposits
			if (command.compare("withdraw") == 0) {
				bool withdrawFine = openAccounts[accountNum]->withdraw("withdrawal", sum);
				if (withdrawFine == false) { continue; }
			}
			else if (command.compare("deposit") == 0) { openAccounts[accountNum]->deposit("deposit", sum); }
			
			//Printing account details
			openAccounts[accountNum]->toString();
		}

		//TRANSFER BETWEEN ACCOUNTS
		else if (command.compare("transfer") == 0)
		{
			//Checks there are a requisite number of accounts open
			if (accountsSize == 1) {
				std::cout << "There must be at least two open accounts to perform a transfer. Please open a second account." << std::endl;
				continue;
			}

			//Checks the account numbers are valid
			int source = accountNumberValidation(parameters[1], accountsSize); //Hands the first account number to validation
			if (source == -1) { continue; } //If invalid, resets back to prompts
			int destination = accountNumberValidation(parameters[2], accountsSize); //Hands second
			if (destination == -1) { continue; } //Same as above

			//Checks whether the source and destination are the same
			if (source == destination) { std::cout << "[source] and [destination] can't be the same. Please try again." << std::endl; continue; }

			//Checks the sum is reasonable / possible
			double sum = stod(parameters[3]); //Grabs sum from vector
			bool isValid = sumValidation(sum, command); //Hands it to validation
			if (isValid == false) { continue; } //Same as above

			//Begins transfer process!
			std::cout << "Starting transfer..." << std::endl;
			bool withdrawFine = openAccounts[source]->withdraw("transfer to account " + std::to_string(destination + 1), sum);
			//If a problem occured withdrawing, aborts the transfer
			if (withdrawFine == false) { std::cout << "Transfer failed!" << std::endl; continue; }
			openAccounts[destination]->deposit("transfer from account " + std::to_string(source + 1), sum);
			std::cout << "Transfer successful!" << std::endl;
			openAccounts[source]->toString();
			openAccounts[destination]->toString();
		}

		//PROJECT INTEREST ON ACCOUNTS
		else if (command.compare("project") == 0)
		{
			int years = stoi(parameters[1]);

			//Fetches the most recently viewed account
			int accountNum = accountDefault(recentIndex, accountsSize);
			
			//Checks if the account is the current account, which has no interest
			if (accountNum == currentAccountNum) {
				std::cout << "A current account does not accrue interest, so the balance will never change." << std::endl;
				std::cout << "Please view a savings account to project interest on." << std::endl; continue;
			}

			//Calls the compute interest method
			openAccounts[accountNum]->computeInterest(years);

			// compute compound interest t years into the future
		}

		else if (command.compare("search") == 0)
		{
			//Ensures account number is valid
			int account = accountNumberValidation(parameters[1], accountsSize);
			if (account == -1) { continue; }

			//Ensures value is valid
			double value = stod(parameters[2]);
			bool isValid = sumValidation(value, "transfer");
			if (isValid == false) { continue; }

			//Calls the search method
			openAccounts[account]->historySearch(value);

		//	allow users to search their account history for a transaction
		//  (this is a stretch task)
		}
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
// "Stop ctime from breaking line?"
//  [https://cboard.cprogramming.com/cplusplus-programming/64859-stop-ctime-breaking-line.html]
// "Convert float to string with precision & number of decimal digits specified?"
//  [https://stackoverflow.com/questions/29200635/convert-float-to-string-with-precision-number-of-decimal-digits-specified]