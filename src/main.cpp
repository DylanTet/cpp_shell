#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

std::unordered_set<std::string> builtins = {"echo", "type", "exit"};

void check_command(std::string &total_command) {
  std::vector<std::string> split_command;
  std::stringstream ss(total_command);
  std::string word;

  while (ss >> word) {
    split_command.push_back(word);
  }

  if (split_command[0] == "echo") {
    std::string res_string;
    for (int i = 1; i < split_command.size(); i++) {
      res_string += split_command[i] + " ";
    }
    std::cout << res_string << '\n';
  } else if (split_command[0] == "type") {
    if (builtins.find(split_command[1]) != builtins.end()) {
      std::cout << split_command[1] << " is a shell builtin\n";
    } else {
      std::cout << split_command[1] << ": not found\n";
    }
  } else {
    std::cout << split_command[0] << ":"
              << " command not found\n";
  }
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    if (input == "exit 0") {
      return 0;
    }

    check_command(input);
  }
}
