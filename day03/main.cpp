#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using schematic = std::vector<std::vector<char>>;

void part1(std::string filePath);
int getWidth(std::string);
int getHeight(std::string);
schematic loadSchematic(std::string);
void printSchematic(schematic schem);
bool nextToSpecialSymbol(int, int, schematic);
bool outOfBounds(int r, int c, int height, int width);

void part2(std::string filePath);
bool nextToTwoPartNumbers(schematic, int, int);
std::string lookAhead(schematic schem, int row, int col, int height, int width);
std::string lookBack(schematic schem, int row, int col, int height, int width);

std::ifstream openFile(std::string filePath) {
  std::ifstream file(filePath);

  if (!file.is_open()) {
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
 *                        --- Day 3 Part 1: Gear Ratios ---
 *
 * You and the Elf eventually reach a gondola lift station; he says the gondola
 * lift will take you up to the water source, but this is as far as he can bring
 * you. You go inside.
 *
 * It doesn't take long to find the gondolas, but there seems to be a problem:
 * they're not moving.
 *
 * "Aaah!"
 *
 * You turn around to see a slightly-greasy Elf with a wrench and a look of
 * surprise. "Sorry, I wasn't expecting anyone! The gondola lift isn't working
 * right now; it'll still be a while before I can fix it." You offer to help.
 *
 * The engineer explains that an engine part seems to be missing from the
 * engine, but nobody can figure out which one. If you can add up all the part
 * numbers in the engine schematic, it should be easy to work out which part is
 * missing.
 *
 * The engine schematic (your puzzle input) consists of a visual representation
 * of the engine. There are lots of numbers and symbols you don't really
 * understand, but apparently any number adjacent to a symbol, even diagonally,
 * is a "part number" and should be included in your sum. (Periods (.) do not
 * count as a symbol.)
 *
 * Here is an example engine schematic:
 *
 * 467..114..
 * ...*......
 * ..35..633.
 * ......#...
 * 617*......
 * .....+.58.
 * ..592.....
 * ......755.
 * ...$.*....
 * .664.598..
 *
 * In this schematic, two numbers are not part numbers because they are not
 * adjacent to a symbol: 114 (top right) and 58 (middle right). Every other
 * number is adjacent to a symbol and so is a part number; their sum is 4361.
 *
 * Of course, the actual engine schematic is much larger. What is the sum of all
 * of the part numbers in the engine schematic?
 *
 */

void part1(std::string filePath) {

  schematic schem = std::move(loadSchematic(filePath));
  std::string buildNumber;
  bool valid = false;
  int sum = 0;

  std::ifstream f(filePath);
  int width = getWidth(filePath);
  int height = getHeight(filePath);

  for (int r = 0; r < height; r++) {
    // check incase number was at end of line
    if (buildNumber.size() > 0) { // we are done with current number
      if (valid)
        sum += std::stoi(buildNumber);

      buildNumber.clear();
      valid = false;
    }
    for (int c = 0; c < width; c++) {
      char ch = schem[r][c];

      if (std::isdigit(ch)) {
        buildNumber.push_back(ch);
        if (nextToSpecialSymbol(r, c, schem))
          valid = true;
      } else if (buildNumber.size() > 0) { // we are done with current number
        if (valid)
          sum += std::stoi(buildNumber);

        buildNumber.clear();
        valid = false;
      }
    }
  }

  // last check if number was at very end
  if (valid && buildNumber.size() > 0)
    sum += std::stoi(buildNumber);

  std::cout << "Part 1: Sum of part numbers is " << sum << "\n";
} // part1

/*
 * part 1 helper methods
 */

// .***.
// .*2*.
// .***.
bool nextToSpecialSymbol(int r, int c, schematic schem) {

  for (int row = r - 1; row < (r - 1) + 3; row++) {
    for (int col = c - 1; col < (c - 1) + 3; col++) {
      if (row == r && col == c)
        continue;
      if (!outOfBounds(row, col, schem.size(), schem[0].size())) {
        char ch = schem[row][col];
        if (!(std::isdigit(ch)) && ch != '.' && ch != ' ')
          return true;
      }
    }
  }
  return false;
}
bool outOfBounds(int r, int c, int height, int width) {
  if (r >= height || r < 0)
    return true;
  if (c >= width || c < 0)
    return true;
  return false;
}
int getWidth(std::string fileName) {
  std::ifstream file(fileName);
  std::string line;
  std::getline(file, line);
  file.seekg(0, file.beg);
  return line.size();
}
int getHeight(std::string fileName) {
  std::ifstream file(fileName);
  int count = 0;
  std::string tmp;
  while (std::getline(file, tmp)) {
    count++;
  }
  file.seekg(0, file.beg);
  return count;
}

schematic loadSchematic(std::string filePath) {
  std::ifstream file(filePath);

  int width = getWidth(filePath);
  int height = getHeight(filePath);

  // initialize an 2D vector
  schematic result =
      std::vector<std::vector<char>>(height, std::vector<char>(width, '.'));

  int row = 0;
  int col = 0;

  std::string line;
  while (std::getline(file, line)) {
    for (auto &ch : line) {
      result[row][col] = ch;
      col++;
    }
    row++;
    col = 0;
  }

  return result;
}

void printSchematic(schematic schem) {
  for (int r = 0; r < schem.size(); r++) {
    for (int c = 0; c < schem[0].size(); c++) {
      std::cout << schem[r][c];
    }
    std::cout << "\n";
  }
}

/**
 *                            --- Part Two ---
 *
 * The engineer finds the missing part and installs it in the engine! As the
 * engine springs to life, you jump in the closest gondola, finally ready to
 * ascend to the water source.
 *
 * You don't seem to be going very fast, though. Maybe something is still wrong?
 * Fortunately, the gondola has a phone labeled "help", so you pick it up and
 * the engineer answers.
 *
 * Before you can explain the situation, she suggests that you look out the
 * window. There stands the engineer, holding a phone in one hand and waving
 *with the other. You're going so slowly that you haven't even left the station.
 *You exit the gondola.
 *
 * The missing part wasn't the only issue - one of the gears in the engine is
 * wrong. A gear is any * symbol that is adjacent to exactly two part numbers.
 *Its gear ratio is the result of multiplying those two numbers together.
 *
 * This time, you need to find the gear ratio of every gear and add them all up
 *so that the engineer can figure out which gear needs to be replaced.
 *
 * Consider the same engine schematic again:
 *
 * 467..114..
 * ...*......
 * ..35..633.
 * ......#...
 * 617*......
 * .....+.58.
 * ..592.....
 * ......755.
 * ...$.*....
 * .664.598..
 *
 * In this schematic, there are two gears. The first is in the top left; it has
 * part numbers 467 and 35, so its gear ratio is 16345. The second gear is in
 *the lower right; its gear ratio is 451490. (The * adjacent to 617 is not a
 *gear because it is only adjacent to one part number.) Adding up all of the
 *gear ratios produces 467835.
 *
 * What is the sum of all of the gear ratios in your engine schematic?
 *
 *
 */

void part2(std::string filePath) {
  std::ifstream file(filePath);
  schematic schem = loadSchematic(filePath);

  int height = schem.size();
  int width = schem[0].size();
  int sum = 0;

  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      char ch = schem[r][c];
      // found potential gear!
      if (ch == '*') {
        if (nextToTwoPartNumbers(schem, r, c)) {
          int ratio = 0;

          std::vector<std::string> possible;
          std::string l;
          std::string m;
          std::string ri;

          l = lookBack(schem, r + 1, c - 1, height, width);
          m = lookAhead(schem, r + 1, c, height, width);
          ri = lookAhead(schem, r + 1, c + 1, height, width);

          if (l.empty() && m.empty() && !ri.empty()) {
            possible.emplace_back(ri);
          } else if ((l.empty() && !m.empty() && ri.empty()) ||
                     (l.empty() && !m.empty() && !ri.empty())) {
            possible.emplace_back(m);
          } else if (!l.empty() && m.empty() && ri.empty()) {
            possible.emplace_back(l);
          } else if (!l.empty() && m.empty() && !ri.empty()) {
            possible.emplace_back(l);
            possible.emplace_back(ri);
          } else if (!l.empty() && !m.empty() && ri.empty()) {
            std::string tmp = l + m;
            possible.emplace_back(tmp);
          } else if (!l.empty() && !m.empty() && !ri.empty()) {
            std::string tmp = l + m;
            possible.emplace_back(tmp);
          }

          l = lookBack(schem, r - 1, c - 1, height, width);
          m = lookAhead(schem, r - 1, c, height, width);
          ri = lookAhead(schem, r - 1, c + 1, height, width);

          if (l.empty() && m.empty() && !ri.empty()) {
            possible.emplace_back(ri);
          } else if ((l.empty() && !m.empty() && ri.empty()) ||
                     (l.empty() && !m.empty() && !ri.empty())) {
            possible.emplace_back(m);
          } else if (!l.empty() && m.empty() && ri.empty()) {
            possible.emplace_back(l);
          } else if (!l.empty() && m.empty() && !ri.empty()) {
            possible.emplace_back(l);
            possible.emplace_back(ri);
          } else if (!l.empty() && !m.empty() && ri.empty()) {
            std::string tmp = l + m;
            possible.emplace_back(tmp);
          } else if (!l.empty() && !m.empty() && !ri.empty()) {
            std::string tmp = l + m;
            possible.emplace_back(tmp);
          }

          l = lookBack(schem, r, c - 1, height, width);
          ri = lookAhead(schem, r, c + 1, height, width);

          if (!l.empty())
            possible.emplace_back(l);
          if (!ri.empty())
            possible.emplace_back(ri);

          for (auto &p : possible) {
            if (!p.empty()) {
              if (ratio == 0)
                ratio = std::stoi(p);
              else
                ratio *= std::stoi(p);
            }
          }
          sum += ratio;
        }
      }
    }
  }
  std::cout << "Part 2: Sum of gear ratios is " << sum << "\n";
}
std::string lookBack(schematic schem, int row, int col, int height, int width) {
  std::string result;
  char ch;
  if (!outOfBounds(row, col, height, width))
    ch = schem[row][col];
  else
    return result;

  while (!outOfBounds(row, col, height, width) && isdigit(ch)) {
    result.insert(0, 1, ch);
    col--;
    if (!outOfBounds(row, col, height, width))
      ch = schem[row][col];
    else
      return result;
  }
  return result;
}
std::string lookAhead(schematic schem, int row, int col, int height,
                      int width) {
  std::string result;
  char ch;
  if (!outOfBounds(row, col, height, width))
    ch = schem[row][col];
  else
    return result;

  while (!outOfBounds(row, col, height, width) && isdigit(ch)) {
    result.push_back(ch);
    col++;

    if (!outOfBounds(row, col, height, width))
      ch = schem[row][col];
    else
      return result;
  }
  return result;
}

bool nextToTwoPartNumbers(schematic schem, int row, int col) {
  int height = schem.size();
  int width = schem[0].size();

  int count = 0;

  bool topLeft = outOfBounds(row - 1, col - 1, height, width)
                     ? false
                     : isdigit(schem[row - 1][col - 1]);
  bool topMid = outOfBounds(row - 1, col, height, width)
                    ? false
                    : isdigit(schem[row - 1][col]);
  bool topRight = outOfBounds(row - 1, col + 1, height, width)
                      ? false
                      : isdigit(schem[row - 1][col + 1]);

  bool midLeft = outOfBounds(row, col - 1, height, width)
                     ? false
                     : isdigit(schem[row][col - 1]);
  bool midRight = outOfBounds(row, col + 1, height, width)
                      ? false
                      : isdigit(schem[row][col + 1]);

  bool botLeft = outOfBounds(row + 1, col - 1, height, width)
                     ? false
                     : isdigit(schem[row + 1][col - 1]);
  bool botMid = outOfBounds(row + 1, col, height, width)
                    ? false
                    : isdigit(schem[row + 1][col]);
  bool botRight = outOfBounds(row + 1, col + 1, height, width)
                      ? false
                      : isdigit(schem[row + 1][col + 1]);

  if (topLeft && !topMid && topRight)
    count += 2;
  else if (!(!topLeft && !topMid && !topRight))
    count += 1;

  if (botLeft && !botMid && botRight)
    count += 2;
  else if (!(!botLeft && !botMid && !botRight))
    count += 1;

  if (midLeft)
    count += 1;
  if (midRight)
    count += 1;

  return count == 2;
}
