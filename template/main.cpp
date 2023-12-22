#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

/**
 * declarations
 */
std::vector<std::string> tokenize(std::string line);
std::shared_ptr<std::ifstream> openFile(std::string filePath);

void part1(std::string inputFile);

void part2(std::string inputFile);

/**
 * util
 */
std::shared_ptr<std::ifstream> openFile(std::string filePath) {
    std::shared_ptr<std::ifstream> file = std::make_shared<std::ifstream>(filePath);
    if (!file->is_open()) {
        std::cout << "Coudn't find input file\n";
        exit(1);
    }
    return file;
}

std::vector<std::string> tokenize(std::string line) {
    std::vector<std::string> tokens;

    std::stringstream stream(line);
    std::string save;

    while (std::getline(stream, save, ' ')) {
        if (!save.empty()) tokens.push_back(save);
    }
    return tokens;
}

/**
 * main function
 */
int main(int argc, char** argv) {
    std::string inputFile;
    if (argc > 1) inputFile = std::string(argv[1]);
    else {
        std::cout << "No input file given\n";
        return 1;
    }

    part1(inputFile);
    part2(inputFile);

    return 0;
};

/**
 * Part 1
 */
void part1(std::string inputFile) { std::shared_ptr<std::ifstream> file = openFile(inputFile); }

/**
 * Part 2
 */
void part2(std::string inputFile) { std::shared_ptr<std::ifstream> file = openFile(inputFile); }
