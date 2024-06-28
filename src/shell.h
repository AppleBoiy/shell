#ifndef SHELL_H
#define SHELL_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <filesystem>

using namespace std;

typedef vector<string>  vs;

void echo(const vector<string> &tokens);
void type(const vector<string> &tokens, const vector<string> &paths);
void pwd();
void cd(const vector<string> &tokens);
void arg_parse(const vector<string> &tokens);

const string BUILTINS[] = {"echo", "type", "exit", "pwd", "cd"};


#endif // SHELL_H
