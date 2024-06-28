#include "shell.h"

using namespace std;

int main() {
  cout << unitbuf;
  cerr << unitbuf;

  string input;

  while (true) {
    cout << "$ ";
    getline(cin, input);

    vector<string> tokens;
    string token;
    for (char c : input) {
      if (c == ' ') {
        if (!token.empty()) {
          tokens.push_back(token);
          token.clear();
        }
      } else {
        token += c;
      }
    }

    if (!token.empty()) {
      tokens.push_back(token);
    }

    arg_parse(tokens);
  }
}
