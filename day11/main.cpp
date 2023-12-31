#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <utility>
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
    std::shared_ptr<std::ifstream> file = std::make_shared<std::ifstream>(
            filePath);
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
 * Declarations/Structs
 */
using grid = std::vector<std::vector<char>>;
const ulong EXPANSION_SIZE = 1;

struct expansionLines {
    std::vector<int> rows;
    std::vector<int> columns;
    expansionLines(std::vector<int> r, std::vector<int> c)
        : rows(std::move(r)), columns(std::move(c)) {}
};

struct position {
    ulong row, col;
    position(ulong r, ulong c) : row(r), col(c) {}
    position() : row(-1), col(-1) {}
    bool operator==(const position& other) {
        return row == other.row && col == other.col;
    }
    bool operator!=(const position& other) {
        return row != other.row || col != other.col;
    }

    void operator=(const position& other) {
        row = other.row;
        col = other.col;
    }
};

void setupGrid(grid& m, int width, int height) {
    for (int r = 0; r < height; r++) {
        std::vector<char> tmp;
        for (int c = 0; c < width; c++) { tmp.push_back('.'); }
        m.push_back(tmp);
    }
}

void getWidthAndHeight(int& width, int& height, std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(inputFile);
    height = 0;
    std::string line;
    std::getline(*file, line);
    width = line.length();
    height++;
    while (std::getline(*file, line)) { height++; }
}
void printGrid(const grid grid) {
    int height = grid.size();
    int width = grid[0].size();

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) { std::cout << grid[r][c]; }
        std::cout << std::endl;
    }
}

std::vector<position> findGalaxies(grid u) {
    int width = u[0].size();
    int height = u.size();
    std::vector<position> galaxies;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (u[row][col] != '.') galaxies.push_back(position(row, col));
        }
    }

    return galaxies;
}

ulong getDistanceBetweenPoints(position p1, position p2) {
    ulong sum1, sum2;
    if (p1.col > p2.col) sum1 = p1.col - p2.col;
    else
        sum1 = p2.col - p1.col;

    if (p1.row > p2.row) sum2 = p1.row - p2.row;
    else
        sum2 = p2.row - p1.row;

    return sum1 + sum2;
}

expansionLines getLines(grid universe) {
    int height = universe.size();
    int width = universe[0].size();

    std::vector<int> rowsThatNeedExpanding;
    std::vector<int> columnsThatNeedExpanding;

    bool hasGalaxies = false;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (universe[row][col] != '.') {
                hasGalaxies = true;
                break;
            }
        }
        if (!hasGalaxies) rowsThatNeedExpanding.push_back(row);
        hasGalaxies = false;
    }

    hasGalaxies = false;
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if (universe[row][col] != '.') {
                hasGalaxies = true;
                break;
            }
        }
        if (!hasGalaxies) columnsThatNeedExpanding.push_back(col);
        hasGalaxies = false;
    }
    return expansionLines(rowsThatNeedExpanding, columnsThatNeedExpanding);
}
void positionsAfterExpanding(position& from, position& to,
                             const expansionLines& lines, ulong expansionSize) {
    int expansionRowFrom = 0;
    int expansionColFrom = 0;
    int expansionRowTo = 0;
    int expansionColTo = 0;

    for (auto& rowToExpand : lines.rows) {
        if (from.row > rowToExpand) expansionRowFrom++;
        if (to.row > rowToExpand) expansionRowTo++;
    }

    for (auto& columnToExpand : lines.columns) {
        if (from.col > columnToExpand) expansionColFrom++;
        if (to.col > columnToExpand) expansionColTo++;
    }

    from.col = from.col + (expansionColFrom * expansionSize);
    from.row = from.row + (expansionRowFrom * expansionSize);

    to.col = to.col + (expansionColTo * expansionSize);
    to.row = to.row + (expansionRowTo * expansionSize);
}

/**
 * Part 1
 */
void part1(std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(inputFile);

    int width, height;
    getWidthAndHeight(width, height, inputFile);

    grid universe;
    setupGrid(universe, width, height);

    int row = 0;
    std::string line;
    while (std::getline(*file, line)) {
        for (int col = 0; col < line.length(); col++) {
            universe[row][col] = line[col];
        }
        row++;
    }

    std::vector<position> galaxies = findGalaxies(universe);
    expansionLines lines = getLines(universe);

    std::vector<position> done;
    long sum = 0;
    for (auto from : galaxies) {
        for (auto to : galaxies) {
            if (from != to
                && std::find(done.begin(), done.end(), to) == done.end()) {
                position gf = from;
                position gt = to;
                positionsAfterExpanding(gf, gt, lines, 1);
                long d = getDistanceBetweenPoints(gf, gt);
                sum += d;
            }
        }
        done.push_back(from);
    }

    std::cout << "Part 1: Sum of lowest distance between galaxies is " << sum
              << "\n";
}

/**
 * Part 2
 */

void part2(std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(inputFile);

    int width, height;
    getWidthAndHeight(width, height, inputFile);

    grid universe;
    setupGrid(universe, width, height);

    int row = 0;
    std::string line;
    while (std::getline(*file, line)) {
        for (int col = 0; col < line.length(); col++) {
            universe[row][col] = line[col];
        }
        row++;
    }

    std::vector<position> galaxies = findGalaxies(universe);
    expansionLines lines = getLines(universe);

    std::vector<position> done;
    long sum = 0;
    for (auto from : galaxies) {
        for (auto to : galaxies) {
            if (from != to
                && std::find(done.begin(), done.end(), to) == done.end()) {
                position gf = from;
                position gt = to;
                positionsAfterExpanding(gf, gt, lines, 999999);
                long d = getDistanceBetweenPoints(gf, gt);
                sum += d;
            }
        }
        done.push_back(from);
    }

    std::cout << "Part 2: Sum of lowest distance between galaxies is " << sum
              << "\n";
}

/**
 *                   --- Day 11: Cosmic Expansion ---
 *
 * You continue following signs for "Hot Springs" and eventually come across
 * an observatory. The Elf within turns out to be a researcher studying
 * cosmic expansion using the giant telescope here.
 *
 * He doesn't know anything about the missing machine parts; he's only
 * visiting for this research project. However, he confirms that the hot
 * springs are the next-closest area likely to have people; he'll even take
 * you straight there once he's done with today's observation analysis.
 *
 * Maybe you can help him with the analysis to speed things up?
 *
 * The researcher has collected a bunch of data and compiled the data into a
 * single giant image (your puzzle input). The image includes empty space
 * (.) and galaxies
 * (#). For example:
 *
 * ...#......
 * .......#..
 * #.........
 * ..........
 * ......#...
 * .#........
 * .........#
 * ..........
 * .......#..
 * #...#.....
 *
 * The researcher is trying to figure out the sum of the lengths of the
 * shortest path between every pair of galaxies. However, there's a catch:
 * the universe expanded in the time it took the light from those galaxies to
 * reach the observatory.
 *
 * Due to something involving gravitational effects, only some space
 *expands. In fact, the result is that any rows or columns that contain no
 *galaxies should all actually be twice as big.
 *
 * In the above example, three columns and two rows contain no galaxies:
 *
 *    v  v  v
 *  ...#......
 *  .......#..
 *  #.........
 * >..........<
 *  ......#...
 *  .#........
 *  .........#
 * >..........<
 *  .......#..
 *  #...#.....
 *    ^  ^  ^
 *
 * These rows and columns need to be twice as big; the result of cosmic
 * expansion therefore looks like this:
 *
 * ....#........
 * .........#...
 * #............
 * .............
 * .............
 * ........#....
 * .#...........
 * ............#
 * .............
 * .............
 * .........#...
 * #....#.......
 *
 * Equipped with this expanded universe, the shortest path between every
 * pair of galaxies can be found. It can help to assign every galaxy a unique
 * number:
 *
 * ....1........
 * .........2...
 * 3............
 * .............
 * .............
 * ........4....
 * .5...........
 * ............6
 * .............
 * .............
 * .........7...
 * 8....9.......
 *
 * In these 9 galaxies, there are 36 pairs. Only count each pair once; order
 * within the pair doesn't matter. For each pair, find any shortest path
 * between the two galaxies using only steps that move up, down, left, or
 * right exactly one . or # at a time. (The shortest path between two
 * galaxies is allowed to pass through another galaxy.)
 *
 * For example, here is one of the shortest paths between galaxies 5 and 9:
 *
 * ....1........
 * .........2...
 * 3............
 * .............
 * .............
 * ........4....
 * .5...........
 * .##.........6
 * ..##.........
 * ...##........
 * ....##...7...
 * 8....9.......
 *
 * This path has length 9 because it takes a minimum of nine steps to get
 * from galaxy 5 to galaxy 9 (the eight locations marked # plus the step onto
 * galaxy 9 itself). Here are some other example shortest path lengths:
 *
 *     Between galaxy 1 and galaxy 7: 15
 *     Between galaxy 3 and galaxy 6: 17
 *     Between galaxy 8 and galaxy 9: 5
 *
 * In this example, after expanding the universe, the sum of the shortest
 * path between all 36 pairs of galaxies is 374.
 *
 * Expand the universe, then find the length of the shortest path between
 * every pair of galaxies. What is the sum of these lengths?
 *
 *                                 --- Part Two ---
 * The galaxies are much older (and thus much farther apart) than the researcher
 * initially estimated.
 *
 * Now, instead of the expansion you did before, make each empty row or column
 * one million times larger. That is, each empty row should be replaced with
 * 1000000 empty rows, and each empty column should be replaced with 1000000
 * empty columns.
 *
 * (In the example above, if each empty row or column were merely 10 times
 * larger, the sum of the shortest paths between every pair of galaxies would be
 * 1030. If each empty row or column were merely 100 times larger, the sum of
 * the shortest paths between every pair of galaxies would be 8410. However,
 * your universe will need to expand far beyond these values.)
 *
 * Starting with the same initial image, expand the universe according to these
 * new rules, then find the length of the shortest path between every pair of
 * galaxies. What is the sum of these lengths?
 *
 *
 */
