#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <functional>

//- FUNCTIONS -
//When called, 'options' will output help for the commands the program can execute
void options();

//Takes a string and sees if it can be converted to a double. If it can't, returns -1. If it can, returns 0.
bool stringToDoubleValidation(std::string input);

//Ensures an input for an account number is in 'openAccounts' vector range
int accountNumberValidation(std::string input, int vectorSize);

//Ensures a sum is within reason
bool sumValidation(double sum, std::string command);

//Checks if an account has been made yet
bool accountCheck(int vectorSize);

//Returns an account number - either what's saved in most recent viewed (recentIndex) or 
int accountDefault(int recentIndex, int vectorSize);