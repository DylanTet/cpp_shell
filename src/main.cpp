#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

void check_command(std::string &command) {
  if (command.find("echo") != std::string::npos) {
    std::string echo_string = command.erase(0, 5);
    std::cout << echo_string << '\n';
  } else {
    std::cout << command << ":"
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
