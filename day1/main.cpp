#include <algorithm>
#include <cctype>
#include <climits>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

std::unique_ptr<std::ifstream> openFile(std::string filePath) {
  std::unique_ptr<std::ifstream> file =
      std::make_unique<std::ifstream>(filePath);
  if (!file->is_open()) {
    std::cout << "Coudn't find input file\n";
    exit(1);
  }
  return file;
}

void part1(std::string filePath) {
  std::unique_ptr<std::ifstream> file = openFile(filePath);

  bool foundFirst = false;
  char first{};
  char last{};

  int sum = 0;
  std::string line;
  while (std::getline(*file, line)) {
    foundFirst = false;
    for (auto &ch : line) {
      if (std::isdigit(ch)) {
        if (!foundFirst) {
          first = ch;
          foundFirst = true;
        }

        last = ch;
      }
    }
    std::string result;
    result.push_back(first);
    result.push_back(last);
    sum += std::stoi(result);
  }

  std::cout << "Part 1: Sum of calibration values is " << sum << "\n";
}

void part2(std::string filePath) {
  std::unordered_map<std::string, std::string> wordToDigit = {
      {"one", "1"},   {"two", "2"},   {"three", "3"},
      {"four", "4"},  {"five", "5"},  {"six", "6"},
      {"seven", "7"}, {"eight", "8"}, {"nine", "9"},
  };

  std::unordered_map<std::string, size_t> firstOf;
  std::unordered_map<std::string, size_t> lastOf;

  std::unique_ptr<std::ifstream> file = openFile(filePath);
  std::string line;

  int sum = 0;
  while (std::getline(*file, line)) {
    std::string first, last;
    int firstPos, lastPos;
    firstPos = -1;
    lastPos = -1;
    size_t posWord = 0;
    size_t posDigit = 0;

    for (auto &keypair : wordToDigit) {
      posWord = line.find(keypair.first);
      posDigit = line.find(keypair.second);

      if (posWord != std::string::npos) {
        if (firstPos == -1) {
          firstPos = posWord;
          first = keypair.second;
        } else if (posWord < firstPos) {
          firstPos = posWord;
          first = keypair.second;
        }
      }

      if (posDigit != std::string::npos) {
        if (firstPos == -1) {
          firstPos = posDigit;
          first = keypair.second;
        } else if (posDigit < firstPos) {
          firstPos = posDigit;
          first = keypair.second;
        }
      }
      std::string reversedLine = line;
      std::reverse(reversedLine.begin(), reversedLine.end());
      std::string reversedKey = keypair.first;
      std::reverse(reversedKey.begin(), reversedKey.end());

      posDigit = line.rfind(keypair.second);

      posWord = reversedLine.find(reversedKey); 
      if(posWord != std::string::npos){
        posWord += reversedKey.length(); 
        posWord = line.length() - posWord;
      }
      if (posWord != std::string::npos) {
        if (lastPos == -1) {
          lastPos = posWord;
          last = keypair.second;
        } else if (posWord > lastPos) {
          lastPos = posWord;
          last = keypair.second;
        }
      }

      if (posDigit != std::string::npos) {
        if (lastPos == -1) {
          lastPos = posDigit;
          last = keypair.second;
        } else if (posDigit > lastPos) {
          lastPos = posDigit;
          last = keypair.second;
        }
      }
    } // for
      
    std::string result;
    result.append(first);
    result.append(last);
    sum += std::stoi(result);
  } // while
  std::cout << "Part 2: Sum of calibration values is " << sum << "\n";
}

int main(int argc, char **argv) {
  std::string inputFile;
  if (argc > 1)
    inputFile = std::string(argv[1]);
  else {
    std::cout << "No input file given\n";
  }

  part1(inputFile);
  part2(inputFile);
  return 0;
};
