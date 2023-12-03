#include <fstream>
#include <iostream>
#include <memory>
#include <string>

void part1(std::string);

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

  return 0;
};


void part1(std::string filePath){
  std::unique_ptr<std::ifstream> file = openFile(filePath);

}
