#include "shell.h"
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

using namespace std;

void echo(const vector<string> &tokens) {
    for (size_t i = 1; i < tokens.size(); ++i) {
        cout << tokens[i];
        if (i < tokens.size() - 1) {
            cout << " ";
        }
    }
    cout << "\n";
}

void type(const vector<string> &tokens, const vector<string> &paths) {
    if (tokens.size() < 2) {
        cout << "type: missing argument\n";
        return;
    }

    const char *command = tokens[1].c_str();
    for (const auto &builtin : BUILTINS) {
        if (strcmp(command, builtin.c_str()) == 0) {
            cout << command << " is a shell builtin\n";
            return;
        }
    }

    bool found = false;
    for (const auto &path : paths) {
        string file = path + "/" + command;
        if (filesystem::exists(file)) {
            cout << command << " is " << file << "\n";
            found = true;
            break;
        }
    }
    if (!found) {
        cout << command << ": not found\n";
    }
}

void pwd() {
    string cwd = filesystem::current_path();
    cout << cwd << "\n";
}

void cd(const vector<string> &tokens) {
    if (tokens.size() < 2 || tokens[1] == "~") {
        const string HOME = getenv("HOME");
        chdir(HOME.c_str());
        return;
    }

    const string path = tokens[1];
    if (filesystem::exists(path)) {
        chdir(path.c_str());
        string cwd = filesystem::current_path();
    } else {
        cout << "cd: " << path << ": No such file or directory\n";
    }
}

vector<string> get_paths() {
    const string PATH = getenv("PATH");
    vector<string> paths;
    string path;
    for (char ch : PATH) {
        if (ch == ':') {
            paths.push_back(path);
            path.clear();
        } else {
            path += ch;
        }
    }
    paths.push_back(path);
    return paths;
}

void arg_parse(const vector<string> &tokens) {
    if (tokens.empty()) {
        return;
    }

    if (tokens[0] == "exit") {
        exit(0);
    }

    vector<string> paths = get_paths();
    const string &command = tokens[0];

    if (command == "echo") {
        echo(tokens);
    } else if (command == "type") {
        type(tokens, paths);
    } else if (command == "pwd") {
        pwd();
    } else if (command == "cd") {
        cd(tokens);
    } else {
        // If command exists in path, execute it
        bool found = false;
        for (const auto &path : paths) {
            string file = path + "/" + command;
            if (filesystem::exists(file)) {
                // Execute file with arguments
                string args;
                for (size_t j = 1; j < tokens.size(); ++j) {
                    args += tokens[j];
                    if (j < tokens.size() - 1) {
                        args += " ";
                    }
                }
                system((file + " " + args).c_str());
                found = true;
                break;
            }
        }

        if (!found) {
            cout << command << ": command not found\n";
        }
    }
}
