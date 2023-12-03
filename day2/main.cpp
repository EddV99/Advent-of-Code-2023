#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

void part1(std::string);
std::string getID(std::string line);
bool isPossible(std::string);

void part2(std::string);

std::unique_ptr<std::ifstream> openFile(std::string filePath) {
  std::unique_ptr<std::ifstream> file =
      std::make_unique<std::ifstream>(filePath);
  if (!file->is_open()) {
    std::cout << "Coudn't find input file\n";
    exit(1);
  }
  return file;
}

int main(int argc, char **argv) {
  std::string inputFile;
  if (argc > 1)
    inputFile = std::string(argv[1]);
  else {
    std::cout << "No input file given\n";
    return 1;
  }
  part1(inputFile);
  part2(inputFile);
  return 0;
};

/*
 * Part 1
 *
 * Example 1:
 * Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
 * Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
 * Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
 * Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
 * Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
 *
 * possible if bag only contained 12 red cubes, 13 green cubes, and 14 blue
 * cubes
 *
 * In example 1, games 1, 2, and 5 are possible
 *
 */
void part1(std::string filePath) {
  std::unique_ptr<std::ifstream> file = openFile(filePath);
  std::string line;
  int sum = 0;
  while (std::getline(*file, line)) {
    std::string id = getID(line);
    if (isPossible(line))
      sum += std::stoi(id);
  } // while
  std::cout << "Part 1: Sum of possible games is " << sum << "\n";
}
/*
 * part 1 helper methods
 */
bool isPossible(std::string line) {
  const int RED_LIMIT = 12;
  const int BLUE_LIMIT = 14;
  const int GREEN_LIMIT = 13;
  int redCount = 0;
  int blueCount = 0;
  int greenCount = 0;

  std::string num;
  for (int i = line.find(":"); i < line.size(); i++) {
    if (line[i] == ';') {
      if ((redCount > RED_LIMIT) || (blueCount > BLUE_LIMIT) ||
          (greenCount > GREEN_LIMIT)) {
        return false;
      }
      redCount = 0;
      blueCount = 0;
      greenCount = 0;
    }
    if (std::isdigit(line[i]))
      num.push_back(line[i]);
    else if (num.size() > 0 && line[i] == 'b') {
      blueCount += std::stoi(num);
      num.clear();
    } else if (num.size() > 0 && line[i] == 'r') {
      redCount += std::stoi(num);
      num.clear();
    } else if (num.size() > 0 && line[i] == 'g') {
      greenCount += std::stoi(num);
      num.clear();
    }
  }

  if (redCount > RED_LIMIT || blueCount > BLUE_LIMIT ||
      greenCount > GREEN_LIMIT) {
    return false;
  }

  return true;
}
std::string getID(std::string line) {
  // Game X..
  // 012345
  std::string id;
  for (int i = 5; i < line.size(); i++) {
    if (!std::isdigit(line[i]))
      break;
    else
      id.push_back(line[i]);
  }
  return id;
}
int getPower(std::string line){
  int minRed = 0;
  int minBlue = 0;
  int minGreen = 0;
  std::string numStr;

  for (int i = line.find(":"); i < line.size(); i++) {
    //if (line[i] == ';') {
        
    //}
    if (std::isdigit(line[i]))
      numStr.push_back(line[i]);
    else if (numStr.size() > 0 && line[i] == 'b') {
      int num = std::stoi(numStr);
      if(minBlue < num)
        minBlue = num;
      numStr.clear();
    } else if (numStr.size() > 0 && line[i] == 'r') {
      int num = std::stoi(numStr);
      if(minRed < num)
        minRed = num;
      numStr.clear();
    } else if (numStr.size() > 0 && line[i] == 'g') {
      int num = std::stoi(numStr);
      if(minGreen < num)
        minGreen = num;
      numStr.clear();
    }
  }

  return minRed * minBlue * minGreen;
}
void part2(std::string filePath) {
  std::unique_ptr<std::ifstream> file = openFile(filePath);
  std::string line;
  int sum = 0;
  while(std::getline(*file, line)){
    sum += getPower(line);
  }
  std::cout << "Part 2: Sum of powers of games is " << sum << "\n";
}
