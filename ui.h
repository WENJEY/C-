#ifndef UI_H
#define UI_H

#include "common.h"

void enableColors();
void logo();
void boxLine();
void boxTitle(const string& title);
void boxRow(const string& text);
void boxLineEq();
void boxCenter(const string& text);
void boxInner(const string& leftText, const string& rightText);
void boxSplitLine();
void boxSplitRow(const string& leftText, const string& rightText);
void displayUserTypeMenu();

#endif
