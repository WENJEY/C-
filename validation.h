#ifndef VALIDATION_H
#define VALIDATION_H

#include "common.h"

int getValidatedInput(int min, int max);
bool isValidName(const string& name);
bool isValidAge(const string& age);
bool isValidMalaysianIC(const string& ic);
bool isValidPassport(const string& passport);
void formatName(string& name);
bool isValidEmail(const string& email);
bool isValidPhoneNumber(const string& phone);
bool isValidPassword(const string& password);
string getSecurePassword(bool showPrompt = true);
void pauseEnter();
string padNumber(int value, int width);
int getIntInRange(int minVal, int maxVal);
bool confirmYesNo(const string& prompt);
double roundMoney(double amount);

#endif
