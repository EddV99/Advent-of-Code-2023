#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
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
 * Part 1
 */

struct position {
    int row;
    int col;
    char pipe;
    bool visited;
    position(int row, int col, char pipe)
        : row(row), col(col), pipe(pipe), visited(false) {}
    position() : row(-1), col(-1) {}

    bool operator==(const position& other) {
        return (row == other.row) && (col == other.col);
    }
};

enum pipeShape { NS, EW, NE, NW, SW, SE, START, NONE };
enum direction { N, E, S, W };

pipeShape getPipeShape(char ch) {
    if (ch == '|') return NS;
    if (ch == '-') return EW;
    if (ch == 'L') return NE;
    if (ch == 'J') return NW;
    if (ch == '7') return SW;
    if (ch == 'F') return SE;
    if (ch == 'S') return START;
    return NONE;
}
using grid = std::vector<std::vector<position>>;

struct pipeMaze {
    grid maze;
    position start;
    int width;
    int height;
};

void getWidthAndHeightOfPipeMaze(int& width, int& height,
                                 std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(inputFile);
    height = 0;
    std::string line;
    std::getline(*file, line);
    width = line.length();
    height++;

    while (std::getline(*file, line)) { height++; }
}

void printGrid(const grid maze) {
    int height = maze.size();
    int width = maze[0].size();

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) { std::cout << maze[r][c].pipe; }
        std::cout << std::endl;
    }
}

void setupGrid(grid& m, int width, int height) {
    for (int r = 0; r < height; r++) {
        std::vector<position> tmp;
        for (int c = 0; c < width; c++) { tmp.push_back(position(r, c, '.')); }
        m.push_back(tmp);
    }
}

/**
 * Check if in range of maze
 */
bool inRange(const pipeMaze& maze, int row, int col) {
    return (row < maze.height && row >= 0) && (col < maze.width && col >= 0);
}

/**
 * Check if two pipes connect from a direction
 *  Assume we are looking from pipe 1 to pipe 2
 */
bool pipesConnect(char p1, char p2, direction looking) {
    pipeShape pipe1 = getPipeShape(p1);
    pipeShape pipe2 = getPipeShape(p2);

    if (pipe1 == NONE || pipe2 == NONE) return false;

    bool southEntrance = pipe2 == SE || pipe2 == SW || pipe2 == NS;
    bool northEntrance = pipe2 == NS || pipe2 == NE || pipe2 == NW;
    bool eastEntrance = pipe2 == SE || pipe2 == EW || pipe2 == NE;
    bool westEntrance = pipe2 == SW || pipe2 == EW || pipe2 == NW;

    if (pipe1 == START) {
        if (looking == N) return southEntrance;
        if (looking == S) return northEntrance;
        if (looking == W) return eastEntrance;
        if (looking == E) return westEntrance;
    } else if (pipe1 == NS) {
        if (looking == N) return southEntrance;
        if (looking == S) return northEntrance;
    } else if (pipe1 == EW) {
        if (looking == E) return westEntrance;
        if (looking == W) return eastEntrance;
    } else if (pipe1 == NE) {
        if (looking == N) { return southEntrance; }
        if (looking == E) return westEntrance;
    } else if (pipe1 == NW) {
        if (looking == N) return southEntrance;
        if (looking == W) return eastEntrance;
    } else if (pipe1 == SW) {
        if (looking == S) return northEntrance;
        if (looking == W) return eastEntrance;
    } else if (pipe1 == SE) {
        if (looking == S) return northEntrance;
        if (looking == E) return westEntrance;
    }
    return false;
}
/**
 * Get position of connected pipe next to it.
 *
 * The problem guarantees a loop, so each pipe has one unvisited pipe it can
 * traverse. The start should have two, so just find the first one.
 */
position getNext(const pipeMaze& maze, position pipe) {
    int row = pipe.row;
    int col = pipe.col;
    char pipeCh = maze.maze[row][col].pipe;

    // look up
    if (inRange(maze, row - 1, col)
        && pipesConnect(pipeCh, maze.maze[row - 1][col].pipe, N)
        && !maze.maze[row - 1][col].visited) {
        return maze.maze[row - 1][col];
    }
    // look down
    if (inRange(maze, row + 1, col)
        && pipesConnect(pipeCh, maze.maze[row + 1][col].pipe, S)
        && !maze.maze[row + 1][col].visited) {
        return maze.maze[row + 1][col];
    }
    // look right
    if (inRange(maze, row, col + 1)
        && pipesConnect(pipeCh, maze.maze[row][col + 1].pipe, E)
        && !maze.maze[row][col + 1].visited) {
        return maze.maze[row][col + 1];
    }
    // look left
    if (inRange(maze, row, col - 1)
        && pipesConnect(pipeCh, maze.maze[row][col - 1].pipe, W)
        && !maze.maze[row][col - 1].visited) {
        return maze.maze[row][col - 1];
    }

    return position();
}

int getFarthestPath(pipeMaze maze) {
    int startRow = maze.start.row;
    int startCol = maze.start.col;
    std::vector<position> nextToStart;

    position left = getNext(maze, maze.start);
    maze.maze[left.row][left.col].visited = true;
    position right = getNext(maze, maze.start);
    maze.maze[right.row][right.col].visited = true;

    int count = 1;
    while (true) {
        count++;

        left = getNext(maze, left);
        if (left.row == -1) break;
        maze.maze[left.row][left.col].visited = true;


        right = getNext(maze, right);
        if (right.row == -1) break;
        maze.maze[right.row][right.col].visited = true;
    }

    return count;
}

void part1(std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(inputFile);

    int height;
    int width;
    getWidthAndHeightOfPipeMaze(width, height, inputFile);
    pipeMaze maze;
    setupGrid(maze.maze, width, height);
    maze.width = width;
    maze.height = height;


    std::string line;

    // fill grid with pipes
    int row = 0;
    while (std::getline(*file, line)) {
        for (int col = 0; col < width; col++) {
            char ch = line[col];
            if (ch == 'S') {
                maze.start.row = row;
                maze.start.col = col;
                maze.start.pipe = 'S';
                maze.start.visited = false;
            }
            maze.maze[row][col].pipe = ch;
        }
        row++;
    }

    auto start = std::chrono::high_resolution_clock::now();
    int result = getFarthestPath(maze);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end - start);
    std::cout << "Part 1: Steps farthest from start is " << result
              << "\nDuration was " << duration.count() << "ms\n";
}

/**
 * Part 2
 */
void part2(std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(inputFile);

    std::string line;
    while (std::getline(*file, line)) {
        std::vector<std::string> tokens = tokenize(line);
    }
}


/**
 *                      --- Day 10: Pipe Maze ---
 *
 * You use the hang glider to ride the hot air from Desert Island all the way up
 * to the floating metal island. This island is surprisingly cold and there
 * definitely aren't any thermals to glide on, so you leave your hang glider
 * behind.
 *
 * You wander around for a while, but you don't find any people or animals.
 * However, you do occasionally find signposts labeled "Hot Springs" pointing in
 * a seemingly consistent direction; maybe you can find someone at the hot
 * springs and ask them where the desert-machine parts are made.
 *
 * The landscape here is alien; even the flowers and trees are made of metal. As
 * you stop to admire some metal grass, you notice something metallic scurry
 * away in your peripheral vision and jump into a big pipe! It didn't look like
 * any animal you've ever seen; if you want a better look, you'll need to get
 * ahead of it.
 *
 * Scanning the area, you discover that the entire field you're standing on is
 * densely packed with pipes; it was hard to tell at first because they're the
 * same metallic silver color as the "ground". You make a quick sketch of all of
 * the surface pipes you can see (your puzzle input).
 *
 * The pipes are arranged in a two-dimensional grid of tiles:
 *
 *     | is a vertical pipe connecting north and south.
 *     - is a horizontal pipe connecting east and west.
 *     L is a 90-degree bend connecting north and east.
 *     J is a 90-degree bend connecting north and west.
 *     7 is a 90-degree bend connecting south and west.
 *     F is a 90-degree bend connecting south and east.
 *     . is ground; there is no pipe in this tile.
 *     S is the starting position of the animal; there is a pipe on this tile,
 *       but your sketch doesn't show what shape the pipe has.
 *
 * Based on the acoustics of the animal's scurrying, you're confident the pipe
 * that contains the animal is one large, continuous loop.
 *
 * For example, here is a square loop of pipe:
 *
 * .....
 * .F-7.
 * .|.|.
 * .L-J.
 * .....
 *
 * If the animal had entered this loop in the northwest corner, the sketch would
 * instead look like this:
 *
 * .....
 * .S-7.
 * .|.|.
 * .L-J.
 * .....
 *
 * In the above diagram, the S tile is still a 90-degree F bend: you can tell
 * because of how the adjacent pipes connect to it.
 *
 * Unfortunately, there are also many pipes that aren't connected to the loop!
 * This sketch shows the same loop as above:
 *
 * -L|F7
 * 7S-7|
 * L|7||
 * -L-J|
 * L|-JF
 *
 * In the above diagram, you can still figure out which pipes form the main
 * loop: they're the ones connected to S, pipes those pipes connect to, pipes
 * those pipes connect to, and so on. Every pipe in the main loop connects to
 * its two neighbors (including S, which will have exactly two pipes connecting
 * to it, and which is assumed to connect back to those two pipes).
 *
 * Here is a sketch that contains a slightly more complex main loop:
 *
 * ..F7.
 * .FJ|.
 * SJ.L7
 * |F--J
 * LJ...
 *
 * Here's the same example sketch with the extra, non-main-loop pipe tiles also
 * shown:
 *
 * 7-F7-
 * .FJ|7
 * SJLL7
 * |F--J
 * LJ.LJ
 *
 * If you want to get out ahead of the animal, you should find the tile in the
 * loop that is farthest from the starting position. Because the animal is in
 * the pipe, it doesn't make sense to measure this by direct distance. Instead,
 * you need to find the tile that would take the longest number of steps along
 * the loop to reach from the starting point - regardless of which way around
 * the loop the animal went.
 *
 * In the first example with the square loop:
 *
 * .....
 * .S-7.
 * .|.|.
 * .L-J.
 * .....
 *
 * You can count the distance each tile in the loop is from the starting point
 * like this:
 *
 * .....
 * .012.
 * .1.3.
 * .234.
 * .....
 *
 * In this example, the farthest point from the start is 4 steps away.
 *
 * Here's the more complex loop again:
 *
 * ..F7.
 * .FJ|.
 * SJ.L7
 * |F--J
 * LJ...
 *
 * Here are the distances for each tile on that loop:
 *
 * ..45.
 * .236.
 * 01.78
 * 14567
 * 23...
 *
 * Find the single giant loop starting at S. How many steps along the loop does
 * it take to get from the starting position to the point farthest from the
 * starting position?
 */
