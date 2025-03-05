#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unistd.h>
#include <unordered_set>
#include <vector>

std::unordered_set<std::string> builtins = {"echo", "type", "exit"};
std::vector<std::string> existing_paths;

std::string search_file(const std::string &directory,
                        const std::string &filename) {
  try {
    if (!std::filesystem::exists(directory) ||
        !std::filesystem::is_directory(directory)) {
      return "";
    }

    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
      if (entry.is_regular_file() && entry.path().filename() == filename) {
        return entry.path();
      }
    }

    return "";

  } catch (std::filesystem::filesystem_error &e) {
    std::cerr << "Filesystem error: " << e.what() << '\n';
    return "";
  }
}

void run_provided_program(std::vector<std::string> &full_command) {
  std::string exe_with_args;
  for (int i = 0; i < full_command.size(); i++) {
    exe_with_args += ' ';
    exe_with_args += full_command[i];
  }

  system(exe_with_args.c_str());
}

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
    std::string found_path;
    for (std::string &dir : existing_paths) {
      std::string path_searched = search_file(dir, split_command[1]);
      if (path_searched.size() > 0) {
        found_path = path_searched;
        break;
      }
    }

    if (builtins.find(split_command[1]) != builtins.end()) {
      std::cout << split_command[1] << " is a shell builtin\n";
      return;
    } else if (found_path.size() > 1) {
      std::cout << split_command[1] << " is " << found_path << '\n';
    } else {
      std::cout << split_command[1] << ": not found\n";
    }

  } else {
    bool found_path;
    for (std::string &dir : existing_paths) {
      std::string path_searched = search_file(dir, split_command[0]);
      if (path_searched.size() > 0) {
        found_path = true;
        break;
      }
    }

    if (!found_path) {
      std::cout << split_command[0] << ":"
                << " command not found\n";
    } else {
      run_provided_program(split_command);
    }
  }
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  const char *path = getenv("PATH");
  std::stringstream ss(path);
  std::string curr_path;

  while (std::getline(ss, curr_path, ':')) {
    existing_paths.emplace_back(curr_path);
  }

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
